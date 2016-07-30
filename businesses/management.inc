//Business components management module
//Created by MadCat
//Idea from PEN1

#include "vehicles"

forward IsPlayerAtCompPlace(playerid);
forward IsATruck(playerid);
forward Management_OnPlayerStateChange(playerid, newstate, oldstate);
forward ShowTruckLoad(playerid);
forward BuyComponents(playerid);
forward SellOrPutComponents(playerid);
forward MakeItems();

#define COMPONENT_COST 100

public Management_OnPlayerStateChange(playerid, newstate, oldstate)
{
	Debug("businesses/management.inc  > Management_OnPlayerStateChange - Start");
	if (newstate == PLAYER_STATE_DRIVER){
		if (IsATruck(playerid)){
			new car = GetPlayerVehicleID(playerid);
			new string[256];
			format(string,sizeof(string),gettext(670), PlayerHaul[car][Load],PlayerHaul[car][Capacity]);
			SystemMsg(playerid,COLOUR_INFO,string);
			SystemMsg(playerid,COLOUR_INFO,gettext(671));
			SystemMsg(playerid,COLOUR_INFO,gettext(672));
		}
	}
	Debug("businesses/management.inc  > Management_OnPlayerStateChange - Stop");
}

public IsPlayerAtCompPlace(playerid)
{
	Debug("businesses/management.inc > IsPlayerAtCompPlace - Start");
	if(IsPlayerConnected(playerid))
	{
	    	if(IsPlayerInRangeOfPoint(playerid,50.0,2787.8,-2436.3,13.7))
		{//LS, in Ocean Docs
			Debug("businesses/management.inc > IsPlayerAtCompPlace - Stop");
		    	return 1;
		}
		else if(IsPlayerInRangeOfPoint(playerid,50.0,-1824.35,-107.68,5.71))
		{//SF, near Military Base
			Debug("businesses/management.inc > IsPlayerAtCompPlace - Stop");
		    	return 1;
		}
		else if(IsPlayerInRangeOfPoint(playerid,50.0,2814.8,972.6,10.7))
		{//LV, in TrainYard
			Debug("businesses/management.inc > IsPlayerAtCompPlace - Stop");
		    	return 1;
		}	
	}
	Debug("businesses/management.inc > IsPlayerAtCompPlace - Stop");
	return 0;
}

public IsATruck(playerid)
{
	Debug("businesses/management.inc > IsATruck - Start");
	new pvehmodel = GetVehicleModel(GetPlayerVehicleID(playerid));
	if (pvehmodel == V_BURRITO || pvehmodel == V_RUMPO || pvehmodel == V_YANKEE)
	{	
		Debug("businesses/management.inc > IsATruck - Stop");
		return 1;
	}
	Debug("businesses/management.inc > IsATruck - Stop");
	return 0;
}

public ShowTruckLoad(playerid){
	Debug("businesses/management.inc > ShowTruckLoad - Start");
	if (!IsPlayerInAnyVehicle(playerid)){
		SystemMsg(playerid,COLOUR_ERROR,gettext(673));
		Debug("businesses/management.inc > ShowTruckLoad - Stop");
		return 1;
	}
	if(!IsATruck(playerid))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(674));
	} else {
		new string[256];	
		format(string,sizeof(string),gettext(676), PlayerHaul[GetPlayerVehicleID(playerid)][Load],PlayerHaul[GetPlayerVehicleID(playerid)][Capacity]);
		SystemMsg(playerid,COLOUR_INFO,string);
	}
	Debug("businesses/management.inc > ShowTruckLoad - Stop");
	return 1;
}

public BuyComponents(playerid){
	Debug("businesses/management.inc > BuyComponents - Start");
	new tmpcar = GetPlayerVehicleID(playerid);
	new string[256];
	if (!IsPlayerInAnyVehicle(playerid)){
		SystemMsg(playerid,COLOUR_ERROR,gettext(673));
		Debug("businesses/management.inc > BuyComponents - Stop");
		return 1;
	}
	if(!IsPlayerAtCompPlace(playerid))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(675));
		Debug("businesses/management.inc > BuyComponents - Stop");
		return 1;
	}

	if (!IsATruck(playerid)){
		SystemMsg(playerid,COLOUR_ERROR,gettext(674));
		Debug("businesses/management.inc > BuyComponents - Stop");
		return 1;
	}

	if(PlayerHaul[tmpcar][Load] < PlayerHaul[tmpcar][Capacity])
	{
		new amount = PlayerHaul[tmpcar][Capacity]-PlayerHaul[tmpcar][Load];
		new cost = amount*COMPONENT_COST;
		if(oGetPlayerMoney(playerid) >= cost)
			{
				PlayerHaul[tmpcar][Load] = PlayerHaul[tmpcar][Capacity];
				format(string,sizeof(string),gettext(676), PlayerHaul[tmpcar][Load],PlayerHaul[tmpcar][Capacity]);
				SystemMsg(playerid,COLOUR_OK,string);
				format(string,sizeof(string),gettext(677), amount,cost);
				SystemMsg(playerid,COLOUR_OK,string);
				oGivePlayerMoney(playerid,-cost,0);
			}
			else
			{
				format(string,sizeof(string),gettext(678), amount,cost);
				SystemMsg(playerid,COLOUR_ERROR,string);
			}
	}
	else
	{
		format(string,sizeof(string),gettext(676), PlayerHaul[tmpcar][Load],PlayerHaul[tmpcar][Capacity]);
		SystemMsg(playerid,COLOUR_ERROR,string);
	}
	Debug("businesses/management.inc > BuyComponents - Stop");
	return 1;
}

public SellOrPutComponents(playerid){
	Debug("businesses/management.inc > SellOrPutComponents - Start");
	if (!IsPlayerInAnyVehicle(playerid)){
		SystemMsg(playerid,COLOUR_ERROR,gettext(673));
		Debug("businesses/management.inc > SellOrPutComponents - Stop");
		return 1;
	}

	if (!IsATruck(playerid)){
		SystemMsg(playerid,COLOUR_ERROR,gettext(674));
		Debug("businesses/management.inc > SellOrPutComponents - Stop");
		return 1;
	}

	new tmpcar = GetPlayerVehicleID(playerid);
	if(PlayerHaul[tmpcar][Load] == 0)
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(679));
		Debug("businesses/management.inc > SellOrPutComponents - Stop");
		return 1;
	}
	new temp[256];
	new count = 0;
	for (new id=0;id<MAX_CLOTHESSHOPS;id++)
	{
		if (IsPlayerInRangeOfPoint(playerid,7.0,CShops[id][Coord_X],CShops[id][Coord_Y],CShops[id][Coord_Z]))
		{
			if ((strcomp(CShops[id][Owner],oGetPlayerName(playerid),false) == 0) && (CShops[id][MoneyNow] < (PlayerHaul[tmpcar][Load]*COMPONENT_COST))){
				SystemMsg(playerid,COLOUR_ERROR,gettext(680));
				Debug("businesses/management.inc > SellOrPutComponents - Stop");
				return 1;
			}
			for(new l = PlayerHaul[tmpcar][Load]; l > 0; l--)
			{
				PlayerHaul[tmpcar][Load]--;
				CShops[id][Components]++;
				count++;
			}
			if (strcomp(CShops[id][Owner],oGetPlayerName(playerid),false) == 1){
				SystemMsgFormat(playerid,COLOUR_OK,gettext(681),count);
			} else {
				SystemMsgFormat(playerid,COLOUR_OK,gettext(682),count);
				oGivePlayerMoney(playerid,CShops[id][Components]*COMPONENT_COST,1);
				CShops[id][MoneyNow] = CShops[id][MoneyNow] - CShops[id][Components]*COMPONENT_COST;
				new dinifilename[MAX_STRING];
				format(dinifilename,sizeof(dinifilename),"%s%s.txt",ClothesBusinessDB,CShops[id][Name]);
				if (db_Exists(dinifilename)) {
					valstr(temp,CShops[id][MoneyNow]);
					db_Set(dinifilename,"MoneyNow",temp);
					set(temp,nullstr);
				}
			}
			SystemMsgFormat(playerid,COLOUR_OK,gettext(683),floatround((CShops[id][Components]*COMPONENT_COST)/CShops[id][ClothesPrice]));
			count = 0;
			Debug("businesses/management.inc > SellOrPutComponents - Stop");
			return 1;
		}
	}
	for (new id=0;id<MAX_DRINKBUSINESSES;id++)
	{
		if (IsPlayerInRangeOfPoint(playerid,7.0,DrinkVendors[id][Coord_X],DrinkVendors[id][Coord_Y],DrinkVendors[id][Coord_Z]))
		{
			if ((strcomp(DrinkVendors[id][Owner],oGetPlayerName(playerid),false) == 0) && (DrinkVendors[id][MoneyNow] < (PlayerHaul[tmpcar][Load]*COMPONENT_COST))){
				SystemMsg(playerid,COLOUR_ERROR,gettext(680));
				Debug("businesses/management.inc > SellOrPutComponents - Stop");
				return 1;
			}
			for(new l = PlayerHaul[tmpcar][Load]; l > 0; l--)
			{
				PlayerHaul[tmpcar][Load]--;
				DrinkVendors[id][Components]++;
				count++;
			}
			if (strcomp(DrinkVendors[id][Owner],oGetPlayerName(playerid),false) == 1){
				SystemMsgFormat(playerid,COLOUR_OK,gettext(684),count);
			} else {
				SystemMsgFormat(playerid,COLOUR_OK,gettext(685),count);
				oGivePlayerMoney(playerid,DrinkVendors[id][Components]*COMPONENT_COST,1);
				DrinkVendors[id][MoneyNow] = DrinkVendors[id][MoneyNow] - DrinkVendors[id][Components]*COMPONENT_COST;
				new dinifilename[MAX_STRING];
				format(dinifilename,sizeof(dinifilename),"%s%s.txt",DrinkBusinessDB,DrinkVendors[id][Name]);
				if (db_Exists(dinifilename)) {
					valstr(temp,DrinkVendors[id][MoneyNow]);
					db_Set(dinifilename,"MoneyNow",temp);
					set(temp,nullstr);
				}
			}
			SystemMsgFormat(playerid,COLOUR_OK,gettext(686),floatround((DrinkVendors[id][Components]*COMPONENT_COST)/DrinkVendors[id][DrinkPrice]));
			count = 0;
			Debug("businesses/management.inc > SellOrPutComponents - Stop");
			return 1;
		}
	}
	for (new id=0;id<MAX_MINESHOPS;id++)
	{
		if (IsPlayerInRangeOfPoint(playerid,7.0,MineShops[id][Coord_X],MineShops[id][Coord_Y],MineShops[id][Coord_Z]))
		{
			if ((strcomp(MineShops[id][Owner],oGetPlayerName(playerid),false) == 0) && (MineShops[id][MoneyNow] < (PlayerHaul[tmpcar][Load]*COMPONENT_COST))){
				SystemMsg(playerid,COLOUR_ERROR,gettext(680));
				Debug("businesses/management.inc > SellOrPutComponents - Stop");
				return 1;
			}
			for(new l = PlayerHaul[tmpcar][Load]; l > 0; l--)
			{
				PlayerHaul[tmpcar][Load]--;
				MineShops[id][Components]++;
				count++;
			}
			if (strcomp(MineShops[id][Owner],oGetPlayerName(playerid),false) == 1){
				SystemMsgFormat(playerid,COLOUR_OK,gettext(687),count);
			} else {
				SystemMsgFormat(playerid,COLOUR_OK,gettext(688),count);
				oGivePlayerMoney(playerid,MineShops[id][Components]*COMPONENT_COST,1);
				MineShops[id][MoneyNow] = MineShops[id][MoneyNow] - MineShops[id][Components]*COMPONENT_COST;
				new dinifilename[MAX_STRING];
				format(dinifilename,sizeof(dinifilename),"%s%s.txt",MinesBusinessDB,MineShops[id][Name]);
				if (db_Exists(dinifilename)) {
					valstr(temp,MineShops[id][MoneyNow]);
					db_Set(dinifilename,"MoneyNow",temp);
					set(temp,nullstr);
				}
			}
			SystemMsgFormat(playerid,COLOUR_OK,gettext(689),floatround((MineShops[id][Components]*COMPONENT_COST)/MineShops[id][MinePrice]));
			count = 0;
			Debug("businesses/management.inc > SellOrPutComponents - Stop");
			return 1;
		}
	}
	for (new id=0;id<MAX_PAYNSPRAYS;id++)
	{
		if (IsPlayerInRangeOfPoint(playerid,7.0,PayNSprays[id][Coord_X],PayNSprays[id][Coord_Y],PayNSprays[id][Coord_Z]))
		{
			if ((strcomp(PayNSprays[id][Owner],oGetPlayerName(playerid),false) == 0) && (PayNSprays[id][MoneyNow] < (PlayerHaul[tmpcar][Load]*COMPONENT_COST))){
				SystemMsg(playerid,COLOUR_ERROR,gettext(680));
				Debug("businesses/management.inc > SellOrPutComponents - Stop");
				return 1;
			}
			for(new l = PlayerHaul[tmpcar][Load]; l > 0; l--)
			{
				PlayerHaul[tmpcar][Load]--;
				PayNSprays[id][Components]++;
				count++;
			}
			if (strcomp(PayNSprays[id][Owner],oGetPlayerName(playerid),false) == 1){
				SystemMsgFormat(playerid,COLOUR_OK,gettext(687),count);
			} else {
				SystemMsgFormat(playerid,COLOUR_OK,gettext(688),count);
				oGivePlayerMoney(playerid,PayNSprays[id][Components]*COMPONENT_COST,1);
				PayNSprays[id][MoneyNow] = PayNSprays[id][MoneyNow] - PayNSprays[id][Components]*COMPONENT_COST;
				new dinifilename[MAX_STRING];
				format(dinifilename,sizeof(dinifilename),"%s%s.txt",PayNSprayBusinessDB,PayNSprays[id][Name]);
				if (db_Exists(dinifilename)) {
					valstr(temp,PayNSprays[id][MoneyNow]);
					db_Set(dinifilename,"MoneyNow",temp);
					set(temp,nullstr);
				}
			}
			SystemMsgFormat(playerid,COLOUR_OK,gettext(689),floatround((PayNSprays[id][Components]*COMPONENT_COST)/PayNSprays[id][ResprayPrice]));
			count = 0;
			Debug("businesses/management.inc > SellOrPutComponents - Stop");
			return 1;
		}
	}
	for (new id=0;id<MAX_DINNERS;id++)
	{
		if (IsPlayerInRangeOfPoint(playerid,7.0,Dinners[id][Coord_X],Dinners[id][Coord_Y],Dinners[id][Coord_Z]))
		{
			if ((strcomp(Dinners[id][Owner],oGetPlayerName(playerid),false) == 0) && (Dinners[id][MoneyNow] < (PlayerHaul[tmpcar][Load]*COMPONENT_COST))){
				SystemMsg(playerid,COLOUR_ERROR,gettext(680));
				Debug("businesses/management.inc > SellOrPutComponents - Stop");
				return 1;
			}

			for(new l = PlayerHaul[tmpcar][Load]; l > 0; l--)
			{
				PlayerHaul[tmpcar][Load]--;
				Dinners[id][Components]++;
				count++;
			}
			if (strcomp(Dinners[id][Owner],oGetPlayerName(playerid),false) == 1){
				SystemMsgFormat(playerid,COLOUR_OK,gettext(690),count);
			} else {
				SystemMsgFormat(playerid,COLOUR_OK,gettext(691),count);
				oGivePlayerMoney(playerid,Dinners[id][Components]*COMPONENT_COST,1);
				Dinners[id][MoneyNow] = Dinners[id][MoneyNow] - Dinners[id][Components]*COMPONENT_COST;
				new dinifilename[MAX_STRING];
				format(dinifilename,sizeof(dinifilename),"%s%s.txt",DinnersBusinessDB,Dinners[id][Name]);
				if (db_Exists(dinifilename)) {
					valstr(temp,Dinners[id][MoneyNow]);
					db_Set(dinifilename,"MoneyNow",temp);
					set(temp,nullstr);
				}
			}
			SystemMsgFormat(playerid,COLOUR_OK,gettext(692),floatround((Dinners[id][Components]*COMPONENT_COST)/Dinners[id][FoodPrice]));
			count = 0;
			Debug("businesses/management.inc > SellOrPutComponents - Stop");
			return 1;
		}
	}
	for (new id=0;id<MAX_GSTATIONS;id++)
	{
		if (IsPlayerInRangeOfPoint(playerid,7.0,fuelstations[id][Canister_X],fuelstations[id][Canister_Y],fuelstations[id][Canister_Z]))
		{

			if ((strcomp(fuelstations[id][Owner],oGetPlayerName(playerid),false) == 0) && (fuelstations[id][MoneyNow] < (PlayerHaul[tmpcar][Load]*COMPONENT_COST))){
				SystemMsg(playerid,COLOUR_ERROR,gettext(680));
				Debug("businesses/management.inc > SellOrPutComponents - Stop");
				return 1;
			}

			for(new l = PlayerHaul[tmpcar][Load]; l > 0; l--)
			{
				PlayerHaul[tmpcar][Load]--;
				fuelstations[id][Components]++;
				count++;
			}
			if (strcomp(fuelstations[id][Owner],oGetPlayerName(playerid),false) == 1){
				SystemMsgFormat(playerid,COLOUR_OK,gettext(693),count);
			} else {
				SystemMsgFormat(playerid,COLOUR_OK,gettext(694),count);
				oGivePlayerMoney(playerid,fuelstations[id][Components]*COMPONENT_COST,1);
				fuelstations[id][MoneyNow] = fuelstations[id][MoneyNow] - fuelstations[id][Components]*COMPONENT_COST;
				new dinifilename[MAX_STRING];
				format(dinifilename,sizeof(dinifilename),"%s%s.txt",FuelBusinessDB,fuelstations[id][Name]);
				if (db_Exists(dinifilename)) {
					valstr(temp,fuelstations[id][MoneyNow]);
					db_Set(dinifilename,"MoneyNow",temp);
					set(temp,nullstr);
				}
			}
			SystemMsgFormat(playerid,COLOUR_OK,gettext(695),floatround((fuelstations[id][Components]*COMPONENT_COST)/fuelstations[id][CanisterPrice]));
			count = 0;
			Debug("businesses/management.inc > SellOrPutComponents - Stop");
			return 1;
		}
	}
	for (new id=0;id<MAX_SKYDIVEPLACES;id++)
	{
		if (IsPlayerInRangeOfPoint(playerid,7.0,SkydivePlaces[id][Coord_X],SkydivePlaces[id][Coord_Y],SkydivePlaces[id][Coord_Z]))
		{

			if ((strcomp(SkydivePlaces[id][Owner],oGetPlayerName(playerid),false) == 0) && (SkydivePlaces[id][MoneyNow] < (PlayerHaul[tmpcar][Load]*COMPONENT_COST))){
				SystemMsg(playerid,COLOUR_ERROR,gettext(680));
				Debug("businesses/management.inc > SellOrPutComponents - Stop");
				return 1;
			}

			for(new l = PlayerHaul[tmpcar][Load]; l > 0; l--)
			{
				PlayerHaul[tmpcar][Load]--;
				SkydivePlaces[id][Components]++;
				count++;
			}
			if (strcomp(SkydivePlaces[id][Owner],oGetPlayerName(playerid),false) == 1){
				SystemMsgFormat(playerid,COLOUR_OK,gettext(696),count);
			} else {
				SystemMsgFormat(playerid,COLOUR_OK,gettext(697),count);
				oGivePlayerMoney(playerid,SkydivePlaces[id][Components]*COMPONENT_COST,1);
				SkydivePlaces[id][MoneyNow] = SkydivePlaces[id][MoneyNow] - SkydivePlaces[id][Components]*COMPONENT_COST;
				new dinifilename[MAX_STRING];
				format(dinifilename,sizeof(dinifilename),"%s%s.txt",SkydiveBusinessDB,SkydivePlaces[id][Name]);
				if (db_Exists(dinifilename)) {
					valstr(temp,SkydivePlaces[id][MoneyNow]);
					db_Set(dinifilename,"MoneyNow",temp);
					set(temp,nullstr);
				}
			}
			SystemMsgFormat(playerid,COLOUR_OK,gettext(698),floatround((SkydivePlaces[id][Components]*COMPONENT_COST)/SkydivePlaces[id][SkydivePrice]));
			count = 0;
			Debug("businesses/management.inc > SellOrPutComponents - Stop");
			return 1;
		}
	}
	for (new id=0;id<MAX_ARMDEALERS;id++)
	{
		if (IsPlayerInRangeOfPoint(playerid,7.0,Armdealers[id][Coord_X],Armdealers[id][Coord_Y],Armdealers[id][Coord_Z]))
		{

			if ((strcomp(Armdealers[id][Owner],oGetPlayerName(playerid),false) == 0) && (Armdealers[id][MoneyNow] < (PlayerHaul[tmpcar][Load]*COMPONENT_COST))){
				SystemMsg(playerid,COLOUR_ERROR,gettext(680));
				Debug("businesses/management.inc > SellOrPutComponents - Stop");
				return 1;
			}

			for(new l = PlayerHaul[tmpcar][Load]; l > 0; l--)
			{
				PlayerHaul[tmpcar][Load]--;
				Armdealers[id][Components]++;
				count++;
			}
			if (strcomp(Armdealers[id][Owner],oGetPlayerName(playerid),false) == 1){
				SystemMsgFormat(playerid,COLOUR_OK,gettext(699),count);
			} else {
				SystemMsgFormat(playerid,COLOUR_OK,gettext(700),count);
				oGivePlayerMoney(playerid,Armdealers[id][Components]*COMPONENT_COST,1);
				Armdealers[id][MoneyNow] = Armdealers[id][MoneyNow] - Armdealers[id][Components]*COMPONENT_COST;
				new dinifilename[MAX_STRING];
				format(dinifilename,sizeof(dinifilename),"%s%s.txt",ArmsDealerBusinessDB,Armdealers[id][Name]);
				if (db_Exists(dinifilename)) {
					valstr(temp,Armdealers[id][MoneyNow]);
					db_Set(dinifilename,"MoneyNow",temp);
					set(temp,nullstr);
				}
			}
			if (Armdealers[id][WeaponSell] == WEAPON_PACK){
				SystemMsgFormat(playerid,COLOUR_OK,gettext(701),floatround((Armdealers[id][Components]*COMPONENT_COST)/PACK_COST));
			} else {
				SystemMsgFormat(playerid,COLOUR_OK,gettext(702),floatround((Armdealers[id][Components]*COMPONENT_COST)/GetWeaponCost(Armdealers[id][WeaponSell])));
			}
			count = 0;
			Debug("businesses/management.inc > SellOrPutComponents - Stop");
			return 1;
		}
	}
	SystemMsg(playerid,COLOUR_ERROR,gettext(711));
	Debug("businesses/management.inc > SellOrPutComponents - Stop");
	return 1;
}

public MakeItems()
{
	Debug("businesses/management.inc > MakeItems - Start");
	new temp[MAX_STRING];
	for (new i=0;i<MAX_CLOTHESSHOPS;i++)
	{
		if(CShops[i][Components] > 0)
		{
			CShops[i][ItemsInStock] = CShops[i][ItemsInStock]+floatround((CShops[i][Components]*COMPONENT_COST)/CShops[i][ClothesPrice]);
			CShops[i][Components]=0;
			new dinifilename[MAX_STRING];
			format(dinifilename,sizeof(dinifilename),"%s%s.txt",ClothesBusinessDB,CShops[i][Name]);
			if (db_Exists(dinifilename)) {
				valstr(temp,CShops[i][ItemsInStock]);
				db_Set(dinifilename,"ItemsInStock",temp);
				set(temp,nullstr);
				valstr(temp,CShops[i][Components]);
				db_Set(dinifilename,"Components",temp);
				set(temp,nullstr);
			}
		}
	}
	for (new i=0;i<MAX_DRINKBUSINESSES;i++)
	{
		if(DrinkVendors[i][Components] > 0)
		{
			DrinkVendors[i][ItemsInStock] = DrinkVendors[i][ItemsInStock]+floatround((DrinkVendors[i][Components]*COMPONENT_COST)/DrinkVendors[i][DrinkPrice]);
			DrinkVendors[i][Components]=0;
			new dinifilename[MAX_STRING];
			format(dinifilename,sizeof(dinifilename),"%s%s.txt",DrinkBusinessDB,DrinkVendors[i][Name]);
			if (db_Exists(dinifilename)) {
				valstr(temp,DrinkVendors[i][ItemsInStock]);
				db_Set(dinifilename,"ItemsInStock",temp);
				set(temp,nullstr);
				valstr(temp,DrinkVendors[i][Components]);
				db_Set(dinifilename,"Components",temp);
				set(temp,nullstr);
			}
		}
	}
	for (new i=0;i<MAX_MINESHOPS;i++)
	{
		if(MineShops[i][Components] > 0)
		{
			MineShops[i][ItemsInStock] = MineShops[i][ItemsInStock]+floatround((MineShops[i][Components]*COMPONENT_COST)/MineShops[i][MinePrice]);
			MineShops[i][Components]=0;
			new dinifilename[MAX_STRING];
			format(dinifilename,sizeof(dinifilename),"%s%s.txt",MinesBusinessDB,MineShops[i][Name]);
			if (db_Exists(dinifilename)) {
				valstr(temp,MineShops[i][ItemsInStock]);
				db_Set(dinifilename,"ItemsInStock",temp);
				set(temp,nullstr);
				valstr(temp,MineShops[i][Components]);
				db_Set(dinifilename,"Components",temp);
				set(temp,nullstr);
			}
		}
	}
	for (new i=0;i<MAX_PAYNSPRAYS;i++)
	{
		if(PayNSprays[i][Components] > 0)
		{
			PayNSprays[i][ItemsInStock] = PayNSprays[i][ItemsInStock]+floatround((PayNSprays[i][Components]*COMPONENT_COST)/PayNSprays[i][ResprayPrice]);
			PayNSprays[i][Components]=0;
			new dinifilename[MAX_STRING];
			format(dinifilename,sizeof(dinifilename),"%s%s.txt",PayNSprayBusinessDB,PayNSprays[i][Name]);
			if (db_Exists(dinifilename)) {
				valstr(temp,PayNSprays[i][ItemsInStock]);
				db_Set(dinifilename,"ItemsInStock",temp);
				set(temp,nullstr);
				valstr(temp,PayNSprays[i][Components]);
				db_Set(dinifilename,"Components",temp);
				set(temp,nullstr);
			}
		}
	}
	for (new i=0;i<MAX_DINNERS;i++)
	{
		if(Dinners[i][Components] > 0)
		{
			Dinners[i][ItemsInStock] = Dinners[i][ItemsInStock]+floatround((Dinners[i][Components]*COMPONENT_COST)/Dinners[i][FoodPrice]);
			Dinners[i][Components]=0;
			new dinifilename[MAX_STRING];
			format(dinifilename,sizeof(dinifilename),"%s%s.txt",DinnersBusinessDB,Dinners[i][Name]);
			if (db_Exists(dinifilename)){
				valstr(temp,Dinners[i][ItemsInStock]);
				db_Set(dinifilename,"ItemsInStock",temp);
				set(temp,nullstr);
				valstr(temp,Dinners[i][Components]);
				db_Set(dinifilename,"Components",temp);
				set(temp,nullstr);
			}
		}
	}
	for (new i=0;i<MAX_GSTATIONS;i++)
	{
		if(fuelstations[i][Components] > 0)
		{
			fuelstations[i][ItemsInStock] = fuelstations[i][ItemsInStock]+floatround((fuelstations[i][Components]*COMPONENT_COST)/fuelstations[i][CanisterPrice]);
			fuelstations[i][Components]=0;
			new dinifilename[MAX_STRING];
			format(dinifilename,sizeof(dinifilename),"%s%s.txt",FuelBusinessDB,fuelstations[i][Name]);
			if (db_Exists(dinifilename)) {
				valstr(temp,fuelstations[i][ItemsInStock]);
				db_Set(dinifilename,"ItemsInStock",temp);
				set(temp,nullstr);
				valstr(temp,fuelstations[i][Components]);
				db_Set(dinifilename,"Components",temp);
				set(temp,nullstr);
			}
		}
	}
	for (new i=0;i<MAX_SKYDIVEPLACES;i++)
	{
		if(SkydivePlaces[i][Components] > 0)
		{
			SkydivePlaces[i][ItemsInStock] = SkydivePlaces[i][ItemsInStock]+floatround((SkydivePlaces[i][Components]*COMPONENT_COST)/SkydivePlaces[i][SkydivePrice]);
			SkydivePlaces[i][Components] = 0;
			new dinifilename[MAX_STRING];
			format(dinifilename,sizeof(dinifilename),"%s%s.txt",SkydiveBusinessDB,SkydivePlaces[i][Name]);
			if (db_Exists(dinifilename)){
				valstr(temp,SkydivePlaces[i][ItemsInStock]);
				db_Set(dinifilename,"ItemsInStock",temp);
				set(temp,nullstr);
				valstr(temp,SkydivePlaces[i][Components]);
				db_Set(dinifilename,"Components",temp);
				set(temp,nullstr);
			}
		}
	}
	for (new i=0;i<MAX_ARMDEALERS;i++)
	{
		if(Armdealers[i][Components] > 0)
		{
			if (Armdealers[i][WeaponSell] == WEAPON_PACK){
				Armdealers[i][ItemsInStock] = Armdealers[i][ItemsInStock]+floatround((Armdealers[i][Components]*COMPONENT_COST)/PACK_COST);
				Armdealers[i][Components]=0;
				new dinifilename[MAX_STRING];
				format(dinifilename,sizeof(dinifilename),"%s%s.txt",ArmsDealerBusinessDB,Armdealers[i][Name]);
				if (db_Exists(dinifilename)){
					valstr(temp,Armdealers[i][ItemsInStock]);
					db_Set(dinifilename,"ItemsInStock",temp);
					set(temp,nullstr);
					valstr(temp,Armdealers[i][Components]);
					db_Set(dinifilename,"Components",temp);
					set(temp,nullstr);
				}

			} else {
				Armdealers[i][ItemsInStock] = Armdealers[i][ItemsInStock]+floatround((Armdealers[i][Components]*COMPONENT_COST)/GetWeaponCost(Armdealers[i][WeaponSell]));
				Armdealers[i][Components]=0;
				new dinifilename[MAX_STRING];
				format(dinifilename,sizeof(dinifilename),"%s%s.txt",ArmsDealerBusinessDB,Armdealers[i][Name]);
				if (db_Exists(dinifilename)){
					valstr(temp,Armdealers[i][ItemsInStock]);
					db_Set(dinifilename,"ItemsInStock",temp);
					set(temp,nullstr);
					valstr(temp,Armdealers[i][Components]);
					db_Set(dinifilename,"Components",temp);
					set(temp,nullstr);
				}
			}
		}
	}
	Debug("businesses/management.inc > MakeItems - Stop");
	
}