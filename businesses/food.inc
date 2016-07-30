//Dinner Business module.
//Created by MadCat
//Based on Original Business Handler from GTO, created by Iain Gilbert
//Coords from GTAT

#include "base"

forward Dinners_OnGameModeInit();
forward IsPlayerAtDinner(playerid);
forward DinnerPickupCheck(playerid);
forward GetDinnerID(playerid);
forward BuyFood(playerid);
forward SellDinner(playerid);
forward CollectDinner(playerid);
forward BuyDinner(playerid);
forward SetFoodPrice(playerid,value);

new dinnerinfoshowed[MAX_PLAYERS] = 0;

new DinnersBusinessDB[MAX_STRING] = "MultiGTA/Businesses/";

#define MAX_DINNERS 31

enum DinnerInfo {
    Name[MAX_STRING],
    Float:Coord_X,
    Float:Coord_Y,
    Float:Coord_Z,
    Level,
    Cost,
    FoodPrice,
    MaxPrice,
    MoneyNow,
    ItemsInStock,
    Components,
    Owner[MAX_NAME]
}

new Dinners[MAX_DINNERS][DinnerInfo] = {
	{"Cluckin Bell - (LS)East Los Santos",2421.2148,-1506.1133,23.9922,5,50000,15,45,0,1000,0,"Server"},
	{"Cluckin Bell - Tierra Robada",-1211.6859,1832.9542,41.9297,5,50000,15,45,0,1000,0,"Server"},
	{"Cluckin Bell - Bone County",173.7285,1173.5623,14.7578,5,50000,15,45,0,1000,0,"Server"},
	{"Cluckin Bell - (LS)Market",925.7686,-1356.4290,13.3793,5,50000,15,45,0,1000,0,"Server"},
	{"Cluckin Bell - (LV)Old Venturas Strip",2391.6130,2044.8538,10.8203,5,50000,15,45,0,1000,0,"Server"},
	{"Cluckin Bell - (LS)Willowfield",2393.8296,-1898.4539,13.5469,5,50000,15,45,0,1000,0,"Server"},
	{"Cluckin Bell - (LV)Creek",2841.6675,2408.2458,11.0690,5,50000,15,45,0,1000,0,"Server"},
	{"Cluckin Bell - Angel Pine",-2152.1404,-2463.0405,30.8438,5,50000,15,45,0,1000,0,"Server"},
	{"Cluckin Bell - (LV)The Emerald Isle",2103.3701,2231.0176,11.0234,5,50000,15,45,0,1000,0,"Server"},
	{"Cluckin Bell - (LV)Pilgrim",2636.8044,1674.4379,11.0234,5,50000,15,45,0,1000,0,"Server"},
	{"Cluckin Bell - (SF)Ocean Flats",-2668.2656,261.3497,4.6328,5,50000,15,45,0,1000,0,"Server"},
	{"Burger Shot - (LV)Whitewood Estates",1161.8025,2069.3792,10.8203,5,50000,15,45,0,1000,0,"Server"},
	{"Burger Shot - (LV)Redsands East",1876.1475,2070.0571,10.8203,5,50000,15,45,0,1000,0,"Server"},
	{"Burger Shot - (SF)Juniper Hollow",-2354.3809,1002.1288,50.8984,5,50000,15,45,0,1000,0,"Server"},
	{"Burger Shot - (LS)Marina",811.5476,-1614.0164,13.5469,5,50000,15,45,0,1000,0,"Server"},
	{"Burger Shot - (LS)Temple",1206.9552,-918.4834,43.0714,5,50000,15,45,0,1000,0,"Server"},
	{"Burger Shot - (LV)Spinybed",2172.4954,2794.2803,10.8203,5,50000,15,45,0,1000,0,"Server"},
	{"Burger Shot - (SF)Garcia",-2330.7656,-165.0439,35.5547,5,50000,15,45,0,1000,0,"Server"},
	{"Burger Shot - (LV)Old Venturas Strip 1",2466.1345,2034.3503,11.0625,5,50000,15,45,0,1000,0,"Server"},
	{"Burger Shot - (LV)Old Venturas Strip 2",2365.2151,2073.3430,10.8203,5,50000,15,45,0,1000,0,"Server"},
	{"Burger Shot - (SF)Downtown",-1914.4845,830.6221,35.3903,5,50000,15,45,0,1000,0,"Server"},
	{"Well Stacked Pizza - Red County",206.5606,-204.6547,1.5781,5,50000,15,45,0,1000,0,"Server"},
	{"Well Stacked Pizza - Montgomery",1366.9811,251.2224,19.5669,5,50000,15,45,0,1000,0,"Server"},
	{"Well Stacked Pizza - (LS)Idlewood",2103.6042,-1809.2458,13.5547,5,50000,15,45,0,1000,0,"Server"},
	{"Well Stacked Pizza - (LV)Creek",2756.8025,2474.0649,11.0625,5,50000,15,45,0,1000,0,"Server"},
	{"Well Stacked Pizza - (LV)The Emerald Isle",2085.3721,2223.8469,11.0234,5,50000,15,45,0,1000,0,"Server"},
	{"Well Stacked Pizza - (LV)Starfish Casino",2636.1663,1851.7607,11.0234,5,50000,15,45,0,1000,0,"Server"},
	{"Well Stacked Pizza - (LV)Roca Escalante",2353.6938,2530.0681,10.8203,5,50000,15,45,0,1000,0,"Server"},
	{"Well Stacked Pizza - (SF)Financial",-1804.7091,946.0225,24.8906,5,50000,15,45,0,1000,0,"Server"},
	{"Well Stacked Pizza - (SF)Esplanade North",-1720.8422,1356.0684,7.1875,5,50000,15,45,0,1000,0,"Server"},
	{"Well Stacked Pizza - Palomino Creek",2335.6914,78.2459,26.4823,5,50000,15,45,0,1000,0,"Server"}
	
}; 

DinnersLoadAll()
{
	Debug("businesses/food.inc > DinnersLoadAll - Start");
	
	new temp[MAX_STRING];
	if (!db_Exists(DatabaseDB)) db_Create(DatabaseDB);
	if (!db_Isset(DatabaseDB,"Dinner_Business_DB")) db_Set(DatabaseDB,"Dinner_Business_DB",DinnersBusinessDB);
	set(temp,db_Get(DatabaseDB,"Dinner_Business_DB"));
	if (strlen(temp) > 0) set(DinnersBusinessDB,temp);
	if (!db_Exists(DinnersBusinessDB))
	{

		DinnersSaveAll();

		return;
	}
	for (new i=0;i<MAX_DINNERS;i++)
	{
		new DinnersBusinessDBname[MAX_STRING];
		format(DinnersBusinessDBname,sizeof(DinnersBusinessDBname),"%s%s.txt",DinnersBusinessDB,Dinners[i][Name]);
		if (!db_Exists(DinnersBusinessDBname))
		{
			DinnersSaveAll();
		}
		set(temp,db_Get(DinnersBusinessDBname,"Name"));
		if (strlen(temp) > 0) set(Dinners[i][Name],temp);
		set(temp,nullstr);
		set(temp,db_Get(DinnersBusinessDBname,"Level"));
		if (strlen(temp) > 0) Dinners[i][Level] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(DinnersBusinessDBname,"Cost"));
		if (strlen(temp) > 0) Dinners[i][Cost] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(DinnersBusinessDBname,"FoodPrice"));
		if (strlen(temp) > 0) Dinners[i][FoodPrice] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(DinnersBusinessDBname,"MaxPrice"));
		if (strlen(temp) > 0) Dinners[i][MaxPrice] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(DinnersBusinessDBname,"MoneyNow"));
		if (strlen(temp) > 0) Dinners[i][MoneyNow] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(DinnersBusinessDBname,"ItemsInStock"));
		if (strlen(temp) > 0) Dinners[i][ItemsInStock] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(DinnersBusinessDBname,"Components"));
		if (strlen(temp) > 0) Dinners[i][Components] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(DinnersBusinessDBname,"Owner"));
		if (strlen(temp) > 0) set(Dinners[i][Owner],temp);
		set(temp,nullstr);
	}
	Debug("businesses/food.inc > DinnersLoadAll - Stop");
	return;

}

DinnersSaveAll()
{
	Debug("businesses/food.inc > DinnersSaveAll - Start");
	new temp[MAX_STRING];
	if (db_Exists(DatabaseDB))
	{
		db_Set(DatabaseDB,"Dinner_Business_DB",DinnersBusinessDB);
	}

	for (new i=0;i<MAX_DINNERS;i++)
	{
		new DinnersBusinessDBname[MAX_STRING];
		format(DinnersBusinessDBname,sizeof(DinnersBusinessDBname),"%s%s.txt",DinnersBusinessDB,Dinners[i][Name]);
		if (db_Exists(DinnersBusinessDBname)){
			continue;
		}
		db_Create(DinnersBusinessDBname);
		db_BeforeBigSaving(DinnersBusinessDBname);
		db_Set(DinnersBusinessDBname,"Name",Dinners[i][Name]);
		valstr(temp,Dinners[i][Level]);
		db_Set(DinnersBusinessDBname,"Level",temp);
		set(temp,nullstr);
		valstr(temp,Dinners[i][Cost]);
		db_Set(DinnersBusinessDBname,"Cost",temp);
		set(temp,nullstr);
		valstr(temp,Dinners[i][FoodPrice]);
		db_Set(DinnersBusinessDBname,"FoodPrice",temp);
		set(temp,nullstr);
		valstr(temp,Dinners[i][MaxPrice]);
		db_Set(DinnersBusinessDBname,"MaxPrice",temp);
		set(temp,nullstr);
		valstr(temp,Dinners[i][MoneyNow]);
		db_Set(DinnersBusinessDBname,"MoneyNow",temp);
		set(temp,nullstr);
		valstr(temp,Dinners[i][ItemsInStock]);
		db_Set(DinnersBusinessDBname,"ItemsInStock",temp);
		set(temp,nullstr);
		valstr(temp,Dinners[i][Components]);
		db_Set(DinnersBusinessDBname,"Components",temp);
		set(temp,nullstr);
		db_Set(DinnersBusinessDBname,"Owner",Dinners[i][Owner]);
		db_AfterBigSaving(DinnersBusinessDBname);

	}
	Debug("businesses/food.inc > DinnersSaveAll - Stop");
	return;
}

public Dinners_OnGameModeInit()
{
	Debug("businesses/food.inc > Dinners_OnGameModeInit - Start");
	new count = 0;
	for (new dbid=0;dbid<MAX_DINNERS;dbid++)
	{
		CreateStreamPickup(1240,1,Dinners[dbid][Coord_X],Dinners[dbid][Coord_Y],Dinners[dbid][Coord_Z]);
		count++;
	}
	new logstring[256];
	format(logstring,sizeof(logstring),"Dinner System Loaded. Dinner Businesses: %d",count);
	WriteLog(logstring);
	Debug("businesses/food.inc > Dinners_OnGameModeInit - Stop");
}

public IsPlayerAtDinner(playerid)
{
	for (new dbid=0;dbid<MAX_DINNERS;dbid++)
	{
		if (IsPlayerInRangeOfPoint(playerid,1.0,Dinners[dbid][Coord_X],Dinners[dbid][Coord_Y],Dinners[dbid][Coord_Z]))
		{
			return 1;
		}
	}
	return 0;
}

public GetDinnerID(playerid)
{
	new lookupid=-1;
	for (new dbid=0;dbid<MAX_DINNERS;dbid++)
	{
		if (IsPlayerInRangeOfPoint(playerid,1.0,Dinners[dbid][Coord_X],Dinners[dbid][Coord_Y],Dinners[dbid][Coord_Z])) lookupid = dbid;
	}
	return lookupid;
}

public DinnerPickupCheck(playerid)
{
	if (playerid == INVALID_PLAYER_ID) return;
	if (!IsPlayerConnected(playerid)) return;
	if (!IsPlayerAtDinner(playerid)) {dinnerinfoshowed[playerid] = 0; return;}
	if (dinnerinfoshowed[playerid] == 1) return;
	dinnerinfoshowed[playerid] = 1;
	new id=GetDinnerID(playerid);
	SystemMsgFormat(playerid,COLOUR_INFO,gettext(766),Dinners[id][FoodPrice]);
	if (strcomp(Dinners[id][Owner],oGetPlayerName(playerid),false) == 1){
		SystemMsg(playerid,COLOUR_INFO,gettext(767));
		if (Dinners[id][MoneyNow] > 0){
			SystemMsgFormat(playerid,COLOUR_INFO,gettext(768),Dinners[id][MoneyNow]);
		}
	}
	new string[MAX_STRING];
	if (strcomp(Dinners[id][Owner],"Server",false) == 1){
	format(string,sizeof(string),gettext(769),Dinners[id][Cost]+Dinners[id][MoneyNow],Dinners[id][Level]);
	SystemMsg(playerid,COLOUR_INFO,string);
	}
}



public BuyFood(playerid){
		Debug("businesses/food.inc > BuyFood - Start");
		new id=GetDinnerID(playerid);
		if (oGetPlayerMoney(playerid) < Dinners[id][FoodPrice])
		{
			SystemMsg(playerid,COLOUR_ERROR,gettext(770));
			Debug("businesses/food.inc > BuyFood - Stop");
			return 1;
		}
		if (Dinners[id][ItemsInStock] == 0){
			ShowTextForPlayer(playerid, gettext(712), 5000, 1);
			Debug("businesses/food.inc > BuyFood - Stop");
			return 1;
		}
		if (oGetPlayerHealth(playerid) >= GetMaxHealth(playerid)){
			SystemMsg(playerid,COLOUR_ERROR,gettext(771));
			Debug("businesses/food.inc > BuyFood - Stop");
			return 1;
		}
		new string[MAX_STRING];
		if (strcomp(Dinners[id][Owner],"Server",false) == 1){
		oGivePlayerMoney(playerid,-Dinners[id][FoodPrice],0);
		Dinners[id][ItemsInStock]--;
		Dinners[id][MoneyNow]=Dinners[id][MoneyNow]+Dinners[id][FoodPrice];
		new dinifilename[MAX_STRING];
		new temp[256];
		format(dinifilename,sizeof(dinifilename),"%s%s.txt",DinnersBusinessDB,Dinners[id][Name]);
		if (db_Exists(dinifilename))
		{
			valstr(temp,Dinners[id][MoneyNow]);
			db_Set(dinifilename,"MoneyNow",temp);
			set(temp,nullstr);
			valstr(temp,Dinners[id][ItemsInStock]);
			db_Set(dinifilename,"ItemsInStock",temp);
			set(temp,nullstr);
		}
		Player[playerid][FoodEaten]++;
		oSetPlayerHealth(playerid,GetMaxHealth(playerid));
		format(string,sizeof(string), gettext(772),0);
		} else if (strcomp(Dinners[id][Owner],oGetPlayerName(playerid),false) == 1){
		Player[playerid][FoodEaten]++;
		oSetPlayerHealth(playerid,GetMaxHealth(playerid));
		Dinners[id][ItemsInStock]--;
		new dinifilename[MAX_STRING];
		new temp[256];
		format(dinifilename,sizeof(dinifilename),"%s%s.txt",DinnersBusinessDB,Dinners[id][Name]);
		if (db_Exists(dinifilename))
		{
			valstr(temp,Dinners[id][ItemsInStock]);
			db_Set(dinifilename,"ItemsInStock",temp);
			set(temp,nullstr);
		}
		format(string,sizeof(string),gettext(773),0);
		}
		else
		{
		oGivePlayerMoney(playerid,-Dinners[id][FoodPrice],0);
		Dinners[id][ItemsInStock]--;
		new dinifilename[MAX_STRING];
		new temp[256];
		format(dinifilename,sizeof(dinifilename),"%s%s.txt",DinnersBusinessDB,Dinners[id][Name]);
		if (db_Exists(dinifilename))
		{
			valstr(temp,Dinners[id][MoneyNow]);
			db_Set(dinifilename,"MoneyNow",temp);
			set(temp,nullstr);
			valstr(temp,Dinners[id][ItemsInStock]);
			db_Set(dinifilename,"ItemsInStock",temp);
			set(temp,nullstr);
		}
		Player[playerid][FoodEaten]++;
		oSetPlayerHealth(playerid,GetMaxHealth(playerid));
		Dinners[id][MoneyNow]=Dinners[id][MoneyNow]+Dinners[id][FoodPrice];
		format(string,sizeof(string),gettext(772),0);
		}
		SystemMsg(playerid,COLOUR_OK,string);
		new logstring[256];
		format(logstring,sizeof(logstring), "player: %d:  %s: buy food.",playerid,oGetPlayerName(playerid));
		WriteLog(logstring);
		Debug("businesses/food.inc > BuyFood - Stop");
		return 1;
}

public SellDinner(playerid){
Debug("businesses/food.inc > SellDinner - Start");
new id=GetDinnerID(playerid);
new temp[MAX_STRING];
	if (id > -1)
	{
		if (IsPlayerRegistered(playerid))
		{
			if(strcomp(Dinners[id][Owner], oGetPlayerName(playerid), true) == 0)
			{
				SystemMsg(playerid,COLOUR_ERROR,gettext(775));
			} else {
				new price;
				price = (Dinners[id][Cost] + Dinners[id][MoneyNow])*85 / 100;
				oGivePlayerMoney(playerid,0+price,1);
				set(Dinners[id][Owner],"Server");
				new dinifilename[MAX_STRING];
				format(dinifilename,sizeof(dinifilename),"%s%s.txt",DinnersBusinessDB,Dinners[id][Name]);
				if (db_Exists(dinifilename))
				{
					db_Set(dinifilename,"Owner","Server");
				}
				format(temp,sizeof(temp),gettext(785),Dinners[id][Name]);
				SystemMsg(playerid,COLOUR_OK,temp);
				set(temp,nullstr);
				new logstring[256];
				format(logstring, sizeof (logstring), "player: %d:  %s: sold the '%s' (Dinner)",playerid,oGetPlayerName(playerid),Dinners[id][Name]);
				WriteLog(logstring);
			}
		} else {
			SystemMsg(playerid,COLOUR_ERROR,gettext(561));
		}
	} else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(782));
	}
	Debug("businesses/food.inc > SellDinner - Stop");
	return 0;
}

public CollectDinner(playerid){
Debug("businesses/food.inc > CollectDinner - Start");
new id=GetDinnerID(playerid);
			new temp[MAX_STRING];
			if (id > -1)
			{
				if (IsPlayerRegistered(playerid))
				{
					if(strcomp(Dinners[id][Owner], oGetPlayerName(playerid), true) == 0)
					{
						SystemMsg(playerid,COLOUR_ERROR,gettext(775));
					} else {
						if(Dinners[id][MoneyNow] > 0)
						{
							oGivePlayerMoney(playerid,0+Dinners[id][MoneyNow],1);
							SystemMsg(playerid,COLOUR_OK,gettext(784));
							Dinners[id][MoneyNow] = 0;
							new dinifilename[MAX_STRING];
							format(dinifilename,sizeof(dinifilename),"%s%s.txt",DinnersBusinessDB,Dinners[id][Name]);
							if (db_Exists(dinifilename))
							{
								valstr(temp,Dinners[id][MoneyNow]);
								db_Set(dinifilename,"MoneyNow",temp);
								set(temp,nullstr);
							}
						} else {
							SystemMsg(playerid,COLOUR_ERROR,gettext(783));
						}
					}
				} else {
					SystemMsg(playerid,COLOUR_ERROR,gettext(561));
				}
			} else {
				SystemMsg(playerid,COLOUR_ERROR,gettext(782));
			}
			Debug("businesses/food.inc > CollectDinner - Stop");
			return 0;
}

public BuyDinner(playerid){
Debug("businesses/food.inc > BuyDinner - Start");
			new id=GetDinnerID(playerid);
			new temp[MAX_STRING];
			if (id > -1)
			{
				if (IsPlayerRegistered(playerid))
				{
					if (Player[playerid][Level] >= Dinners[id][Level])
					{
						new price;
						price = Dinners[id][Cost] + Dinners[id][MoneyNow];
						if(Player[playerid][Money] >= price)
						{
							if(strcomp(Dinners[id][Owner], oGetPlayerName(playerid), true) == 1)
							{
								SystemMsg(playerid,COLOUR_ERROR,gettext(774));
							} else {

								oGivePlayerMoney(playerid,0-price,1);
								set(Dinners[id][Owner],oGetPlayerName(playerid));
								new dinifilename[MAX_STRING];
								format(dinifilename,sizeof(dinifilename),"%s%s.txt",DinnersBusinessDB,Dinners[id][Name]);
								if (db_Exists(dinifilename))
								{
									db_Set(dinifilename,"Owner",Dinners[id][Owner]);
								}
								format(temp,sizeof(temp),gettext(776),Dinners[id][Name]);
								SystemMsg(playerid,COLOUR_OK,temp);
								SystemMsg(playerid,COLOUR_INFO,gettext(777));
								SystemMsg(playerid,COLOUR_INFO,gettext(778));
								SystemMsg(playerid,COLOUR_INFO,gettext(779));
								set(temp,nullstr);

								new logstring[256];
								format(logstring, sizeof (logstring), "player: %d:  %s: bought the '%s' (Dinner)",playerid,oGetPlayerName(playerid),Dinners[id][Name]);
								WriteLog(logstring);

							}
						} else {
							SystemMsg(playerid,COLOUR_ERROR,gettext(780));
						}
					} else {
						SystemMsgFormat(playerid,COLOUR_ERROR,gettext(781),Dinners[id][Level]);
					}
				} else {
					SystemMsg(playerid,COLOUR_ERROR,gettext(561));
				}
			} else {
				SystemMsg(playerid,COLOUR_ERROR,gettext(782));
			}
			Debug("businesses/food.inc > BuyDinner - Stop");
			return 0;
}

public SetFoodPrice(playerid,value){
Debug("businesses/food.inc > SetFoodPrice - Start");
new id=GetDinnerID(playerid);
if (value <= 0){
	SystemMsg(playerid,COLOUR_ERROR,gettext(786));
	Debug("businesses/food.inc > SetFoodPrice - Stop");
	return 1;
}
if (id > -1)
{
	if (IsPlayerRegistered(playerid))
	{
		if(strcomp(Dinners[id][Owner], oGetPlayerName(playerid), true) == 0)
		{
			SystemMsg(playerid,COLOUR_ERROR,gettext(775));
		} else {
			if (value > Dinners[id][MaxPrice]){
				SystemMsgFormat(playerid,COLOUR_ERROR,gettext(787),Dinners[id][MaxPrice]);
				return 1;
			}
			new dinifilename[MAX_STRING];
			format(dinifilename,sizeof(dinifilename),"%s%s.txt",DinnersBusinessDB,Dinners[id][Name]);
			if (db_Exists(dinifilename))
			{
				Dinners[id][FoodPrice] = value;
				new temp[MAX_STRING];
				valstr(temp,Dinners[id][FoodPrice]);
				db_Set(dinifilename,"FoodPrice",temp);
				set(temp,nullstr);
			}
			new string[MAX_STRING];
			format(string,sizeof(string),gettext(788),Dinners[id][Name],Dinners[id][FoodPrice]);
			SystemMsg(playerid,COLOUR_OK,string);
			new logstring[256];
			format(logstring, sizeof (logstring), "player: %d:  %s: set price of '%s' (Dinner) to $%d",playerid,oGetPlayerName(playerid),Dinners[id][Name],Dinners[id][FoodPrice]);
			WriteLog(logstring);
		}
	} else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(561));
	}
} else {
	SystemMsg(playerid,COLOUR_ERROR,gettext(782));
}
Debug("businesses/food.inc > SetFoodPrice - Stop");
return 0;
}