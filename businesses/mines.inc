//Mine shops module.
//Created by MadCat
//Based on Original Business Handler from GTO, created by Iain Gilbert
//Coords and some part of code from GTAT

#include "base"
#include "player"

#define MAX_MINES 5

forward Mines_OnGameModeInit();
forward Mines_OnPlayerCommandText(playerid,text[]);
forward Mines_OnVehicleDeath(vehicleid,killerid);

forward IsPlayerAtMineShop(playerid);
forward MineShopPickupCheck(playerid);
forward MinesTimer();
forward GetMineShopID(playerid);
forward BuyMine(playerid);
forward SellMineShop(playerid);
forward CollectMineShop(playerid);
forward BuyMineShop(playerid);
forward SetMinePrice(playerid,value);

new MineExistTime[MAX_VEHICLES+1][MAX_MINES];
new MineObjectID[MAX_VEHICLES+1][MAX_MINES];
new MinesOnVehicle[MAX_VEHICLES+1];
new MinePlantPos[MAX_VEHICLES+1][MAX_MINES][CoordInfo];

new mineshopinfoshowed[MAX_PLAYERS] = 0;

new MinesBusinessDB[MAX_STRING] = "MultiGTA/Businesses/";

#define MINE_EXISTTIME 60

#define MAX_MINESHOPS 5

enum MineShopInfo {
    Name[MAX_STRING],
    Float:Coord_X,
    Float:Coord_Y,
    Float:Coord_Z,
    Level,
    Cost,
    MinePrice,
    MaxPrice,
    ItemsInStock,
    Components,
    MoneyNow,
    Owner[MAX_NAME]
}

new MineShops[MAX_MINESHOPS][MineShopInfo] = {
	{"Mine Shop - (LS)Temple",1044.6807,-1028.3739,32.1016,10,100000,250,750,1500,0,0,"Server"},
	{"Mine Shop - (LS)Willowfield",2650.0742,-2035.6411,13.5540,10,100000,250,750,1500,0,0,"Server"},
	{"Mine Shop - (SF)Dohery",-1930.6661,234.7708,34.3666,10,100000,250,750,1500,0,0,"Server"},
	{"Mine Shop - (SF)Ocean Flats",-2712.0244,212.7835,4.3212,10,100000,250,750,1500,0,0,"Server"},
	{"Mine Shop - (LV)Come-A-Lot",2391.6128,1041.0583,10.8203,10,100000,250,750,1500,0,0,"Server"}	
}; 

new TimerMines;

MineShopsLoadAll()
{
	Debug("businesses/mines.inc > MineShopsLoadAll - Start");
	
	new temp[MAX_STRING];
	if (!db_Exists(DatabaseDB)) db_Create(DatabaseDB);
	if (!db_Isset(DatabaseDB,"Mines_Business_DB")) db_Set(DatabaseDB,"Mines_Business_DB",MinesBusinessDB);
	set(temp,db_Get(DatabaseDB,"Mines_Business_DB"));
	if (strlen(temp) > 0) set(MinesBusinessDB,temp);
	if (!db_Exists(MinesBusinessDB))
	{

		MineShopsSaveAll();

		return;
	}
	for (new i=0;i<MAX_MINESHOPS;i++)
	{
		new MinesBusinessDBname[MAX_STRING];
		format(MinesBusinessDBname,sizeof(MinesBusinessDBname),"%s%s.txt",MinesBusinessDB,MineShops[i][Name]);
		if (!db_Exists(MinesBusinessDBname))
		{
			MineShopsSaveAll();
		}
		set(temp,db_Get(MinesBusinessDBname,"Name"));
		if (strlen(temp) > 0) set(MineShops[i][Name],temp);
		set(temp,nullstr);
		set(temp,db_Get(MinesBusinessDBname,"Level"));
		if (strlen(temp) > 0) MineShops[i][Level] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(MinesBusinessDBname,"Cost"));
		if (strlen(temp) > 0) MineShops[i][Cost] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(MinesBusinessDBname,"MinePrice"));
		if (strlen(temp) > 0) MineShops[i][MinePrice] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(MinesBusinessDBname,"MaxPrice"));
		if (strlen(temp) > 0) MineShops[i][MaxPrice] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(MinesBusinessDBname,"ItemsInStock"));
		if (strlen(temp) > 0) MineShops[i][ItemsInStock] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(MinesBusinessDBname,"MoneyNow"));
		if (strlen(temp) > 0) MineShops[i][MoneyNow] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(MinesBusinessDBname,"Components"));
		if (strlen(temp) > 0) MineShops[i][Components] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(MinesBusinessDBname,"Owner"));
		if (strlen(temp) > 0) set(MineShops[i][Owner],temp);
		set(temp,nullstr);
	}
	Debug("businesses/mines.inc > MineShopsLoadAll - Stop");
	return;

}

MineShopsSaveAll()
{
	Debug("businesses/mines.inc > MineShopsSaveAll - Start");
	new temp[MAX_STRING];
	if (db_Exists(DatabaseDB))
	{
		db_Set(DatabaseDB,"Mines_Business_DB",MinesBusinessDB);
	}

	for (new i=0;i<MAX_MINESHOPS;i++)
	{
		new MinesBusinessDBname[MAX_STRING];
		format(MinesBusinessDBname,sizeof(MinesBusinessDBname),"%s%s.txt",MinesBusinessDB,MineShops[i][Name]);
		if (db_Exists(MinesBusinessDBname)) {
			continue;
		}
		db_Create(MinesBusinessDBname);
		db_BeforeBigSaving(MinesBusinessDBname);
		db_Set(MinesBusinessDBname,"Name",MineShops[i][Name]);
		valstr(temp,MineShops[i][Level]);
		db_Set(MinesBusinessDBname,"Level",temp);
		set(temp,nullstr);
		valstr(temp,MineShops[i][Cost]);
		db_Set(MinesBusinessDBname,"Cost",temp);
		set(temp,nullstr);
		valstr(temp,MineShops[i][MinePrice]);
		db_Set(MinesBusinessDBname,"MinePrice",temp);
		set(temp,nullstr);
		valstr(temp,MineShops[i][MaxPrice]);
		db_Set(MinesBusinessDBname,"MaxPrice",temp);
		set(temp,nullstr);
		valstr(temp,MineShops[i][MoneyNow]);
		db_Set(MinesBusinessDBname,"MoneyNow",temp);
		set(temp,nullstr);
		valstr(temp,MineShops[i][ItemsInStock]);
		db_Set(MinesBusinessDBname,"ItemsInStock",temp);
		set(temp,nullstr);
		valstr(temp,MineShops[i][Components]);
		db_Set(MinesBusinessDBname,"Components",temp);
		set(temp,nullstr);
		db_Set(MinesBusinessDBname,"Owner",MineShops[i][Owner]);
		db_AfterBigSaving(MinesBusinessDBname);

	}
	Debug("businesses/mines.inc > MineShopsSaveAll - Stop");
	return;
}

stock VehicleWrongForMine(model){

	switch (model){
		case V_MTBIKE: return true;
		case V_BMX: return true;
		case V_BIKE: return true;
		case V_SANCHEZ: return true;
		case V_KART: return true;
		case V_VORTEX: return true;
	}

	if (CanVehicleFly(model)) return true;
	if (IsVehicleBoat(model)) return true;
	if (IsVehicleMotoBike(model)) return true;

	return false;
}

public Mines_OnGameModeInit()
{
	Debug("businesses/mines.inc > Mines_OnGameModeInit - Start");
	new count = 0;
	for (new dbid=0;dbid<MAX_MINESHOPS;dbid++)
	{
		CreateStreamPickup(1225,1,MineShops[dbid][Coord_X],MineShops[dbid][Coord_Y],MineShops[dbid][Coord_Z]);
		CreateStreamMapIcon(27,MineShops[dbid][Coord_X],MineShops[dbid][Coord_Y],MineShops[dbid][Coord_Z]);
		CreateDMapIcon(27,MineShops[dbid][Coord_X],MineShops[dbid][Coord_Y],MineShops[dbid][Coord_Z],gettext(1761),gettext(1762));
		count++;
	}
	new logstring[256];
	format(logstring,sizeof(logstring),"Mines system Loaded. Shops: %d",count);
	WriteLog(logstring);
	TimerMines = SetTimer("MinesTimer",1000,1);
	Debug("businesses/mines.inc > Mines_OnGameModeInit - Stop");
}

public IsPlayerAtMineShop(playerid)
{
	for (new dbid=0;dbid<MAX_MINESHOPS;dbid++)
	{
		if (IsPlayerInRangeOfPoint(playerid,4.0,MineShops[dbid][Coord_X],MineShops[dbid][Coord_Y],MineShops[dbid][Coord_Z]))
		{
			return 1;
		}
	}
	return 0;
}

public GetMineShopID(playerid)
{
	new lookupid=-1;
	for (new dbid=0;dbid<MAX_MINESHOPS;dbid++)
	{
		if (IsPlayerInRangeOfPoint(playerid,4.0,MineShops[dbid][Coord_X],MineShops[dbid][Coord_Y],MineShops[dbid][Coord_Z])) lookupid = dbid;
	}
	return lookupid;
}

public MineShopPickupCheck(playerid)
{
	if (playerid == INVALID_PLAYER_ID) return;
	if (!IsPlayerConnected(playerid)) return;
	new id = GetMineShopID(playerid);
	if (id == -1) {mineshopinfoshowed[playerid] = 0; return;}
	if (mineshopinfoshowed[playerid] == 1) return;
	mineshopinfoshowed[playerid] = 1;
	SystemMsgFormat(playerid,COLOUR_INFO,gettext(811),MineShops[id][MinePrice]);
	if (strcomp(MineShops[id][Owner],oGetPlayerName(playerid),false) == 1){
		SystemMsg(playerid,COLOUR_INFO,gettext(812));
		if (MineShops[id][MoneyNow] > 0){
			SystemMsgFormat(playerid,COLOUR_INFO,gettext(813),MineShops[id][MoneyNow]);
		}
	}
	new string[MAX_STRING];
	if (strcomp(MineShops[id][Owner],"Server",false) == 1){
		format(string,sizeof(string),gettext(814),MineShops[id][Cost]+MineShops[id][MoneyNow],MineShops[id][Level]);
		SystemMsg(playerid,COLOUR_INFO,string);
	}
}

public Mines_OnVehicleDeath(vehicleid,killerid){
	#pragma unused killerid
	MinesOnVehicle[vehicleid] = 0;
	return 0;
}

public Mines_OnPlayerCommandText(playerid,text[])
{
	if (!IsPlayerConnected(playerid)) return 0;

	dcmd(drop,4,text);

	return 0;
}

dcmd_drop(playerid,params[])
{
	#pragma unused params
	Debug("businesses/mines.inc > Command 'drop' - Start");

	if (!IsPlayerInAnyVehicle(playerid))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(815));
		Debug("businesses/mines.inc > Command 'drop' - Stop");
		return 1;
	}
		
	new vid = GetPlayerVehicleID(playerid);

	if (MinesOnVehicle[vid] == 0)
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(816));
		Debug("businesses/mines.inc > Command 'drop' - Stop");
		return 1;
	}
	
	new mineid = GetLowestMineID(vid);
	if (mineid == -1){
		SystemMsg(playerid,COLOUR_ERROR,gettext(820));
	} else {
		new Float:x, Float:y;
		GetXYInBackOfPlayer(playerid,x,y,4.0);
		new Float:z;
		z =(PlayerPos[playerid][Coord_Z]-0.7);
		MinePlantPos[vid][mineid][Coord_X] = x;
		MinePlantPos[vid][mineid][Coord_Y] = y;
		MinePlantPos[vid][mineid][Coord_Z] = z;
		MineObjectID[vid][mineid] = CreateStreamObject(1225,x,y,z,0,0,0,-1);
		for (new v = 1; v < MAX_VEHICLES+1; v++) {
			for(new m=0; m< MAX_MINES; m++){
		    		if (MineExistTime[v][m] != 0) {
		        		new Float:x2, Float:y2, Float:z2;
					x2 = GetXPos(MineObjectID[v][m]); y2 = GetYPos(MineObjectID[v][m]); z2 = GetZPos(MineObjectID[v][m]);
					if(GetPointDistanceToPointEx(x2,y2,z2,MinePlantPos[v][m][Coord_X],MinePlantPos[v][m][Coord_Y],MinePlantPos[v][m][Coord_Z]) >= 1){
				    		SetStreamObjectPos(MineObjectID[v][m],MinePlantPos[v][m][Coord_X],MinePlantPos[v][m][Coord_Y],MinePlantPos[v][m][Coord_Z]);
					}
		    		}
	   		}
		}
		Player[playerid][MinesPlanted]++;
		MinesOnVehicle[vid]--;
		MineExistTime[vid][mineid] = 1;
		SystemMsg(playerid,COLOUR_INFO,gettext(817));
	}

	new logstring[256];
	format(logstring,sizeof(logstring), "player: %d:  %s: set mine.",playerid,oGetPlayerName(playerid));
	WriteLog(logstring);
	Debug("businesses/mines.inc > Command 'drop' - Stop");
	return 1;
}

public MinesTimer(){
	for (new v = 1; v < MAX_VEHICLES+1; v++) {
		for(new m=0; m< MAX_MINES; m++){
		    	if (MineExistTime[v][m] != 0) {
		        	MineExistTime[v][m]++;
		        	if (MineExistTime[v][m] > MINE_EXISTTIME) {
		           		MineExistTime[v][m] = 0;
					DestroyStreamObject(MineObjectID[v][m]);
		        	}
		    	}
	   	}
	}
}

public BuyMine(playerid){
		Debug("businesses/mines.inc > BuyMine - Start");
		new id=GetMineShopID(playerid);
		if (!IsPlayerInAnyVehicle(playerid))
		{
			SystemMsg(playerid,COLOUR_ERROR,gettext(815));
			Debug("businesses/mines.inc > BuyMine - Stop");
			return 1;
		}
		if (oGetPlayerMoney(playerid) < MineShops[id][MinePrice])
		{
			SystemMsg(playerid,COLOUR_ERROR,gettext(818));
			Debug("businesses/mines.inc > BuyMine - Stop");
			return 1;
		}
		if (MineShops[id][ItemsInStock] == 0){
			ShowTextForPlayer(playerid, gettext(712), 5000, 1);
			Debug("businesses/mines.inc > BuyMine - Stop");
			return 1;
		}
            	if(GetPlayerState(playerid) == PLAYER_STATE_PASSENGER){
	            	SystemMsg(playerid,COLOUR_ERROR,gettext(819));
			Debug("businesses/mines.inc > BuyMine - Stop");
			return 1;
            	}

		new vid = GetPlayerVehicleID(playerid);
			
            	if(MinesOnVehicle[vid] == MAX_MINES){
	            	SystemMsg(playerid,COLOUR_ERROR,gettext(820));
			Debug("businesses/mines.inc > BuyMine - Stop");
	            	return 1;
            	}

            	if(VehicleWrongForMine(GetVehicleModel(vid))){
  	            	SystemMsg(playerid,COLOUR_ERROR,gettext(821));
			Debug("businesses/mines.inc > BuyMine - Stop");
	            	return 1;
            	}

		new string[MAX_STRING];
		if (strcomp(MineShops[id][Owner],"Server",false) == 1){
		oGivePlayerMoney(playerid,-MineShops[id][MinePrice],0);
		MineShops[id][MoneyNow]=MineShops[id][MoneyNow]+MineShops[id][MinePrice];
		MineShops[id][ItemsInStock]--;
		new dinifilename[MAX_STRING];
		new temp[256];
		format(dinifilename,sizeof(dinifilename),"%s%s.txt",MinesBusinessDB,MineShops[id][Name]);
		if (db_Exists(dinifilename))
		{
			valstr(temp,MineShops[id][MoneyNow]);
			db_Set(dinifilename,"MoneyNow",temp);
			set(temp,nullstr);
			valstr(temp,MineShops[id][ItemsInStock]);
			db_Set(dinifilename,"ItemsInStock",temp);
			set(temp,nullstr);
		}
		MinesOnVehicle[vid]++;
		format(string,sizeof(string),gettext(822),0);
		} else if (strcomp(MineShops[id][Owner],oGetPlayerName(playerid),false) == 1){
		MinesOnVehicle[vid]++;
		MineShops[id][ItemsInStock]--;
		new dinifilename[MAX_STRING];
		new temp[256];
		format(dinifilename,sizeof(dinifilename),"%s%s.txt",MinesBusinessDB,MineShops[id][Name]);
		if (db_Exists(dinifilename))
		{
			valstr(temp,MineShops[id][MoneyNow]);
			db_Set(dinifilename,"MoneyNow",temp);
			set(temp,nullstr);
			valstr(temp,MineShops[id][ItemsInStock]);
			db_Set(dinifilename,"ItemsInStock",temp);
			set(temp,nullstr);
		}
		format(string,sizeof(string),gettext(823),0);
		}
		else
		{
		oGivePlayerMoney(playerid,-MineShops[id][MinePrice],0);
		MinesOnVehicle[vid]++;
		MineShops[id][ItemsInStock]--;
		MineShops[id][MoneyNow]=MineShops[id][MoneyNow]+MineShops[id][MinePrice];
		new dinifilename[MAX_STRING];
		new temp[256];
		format(dinifilename,sizeof(dinifilename),"%s%s.txt",MinesBusinessDB,MineShops[id][Name]);
		if (db_Exists(dinifilename))
		{
			valstr(temp,MineShops[id][MoneyNow]);
			db_Set(dinifilename,"MoneyNow",temp);
			set(temp,nullstr);
		}
		format(string,sizeof(string),gettext(822),0);
		}
		SystemMsg(playerid,COLOUR_OK,string);
		new logstring[256];
		format(logstring,sizeof(logstring), "player: %d:  %s: set mine on vehicle.",playerid,oGetPlayerName(playerid));
		WriteLog(logstring);
		Debug("businesses/mines.inc > BuyMine - Stop");
		return 1;
}

public SellMineShop(playerid){
Debug("businesses/mines.inc > SellMineShop - Start");
new id=GetMineShopID(playerid);
new temp[MAX_STRING];
	if (id > -1)
	{
		if (IsPlayerRegistered(playerid))
		{
			if(strcomp(MineShops[id][Owner], oGetPlayerName(playerid), true) == 0)
			{
				SystemMsg(playerid,COLOUR_ERROR,gettext(824));
			} else {
				new price;
				price = (MineShops[id][Cost] + MineShops[id][MoneyNow])*85 / 100;
				oGivePlayerMoney(playerid,0+price,1);
				set(MineShops[id][Owner],"Server");
				new dinifilename[MAX_STRING];
				format(dinifilename,sizeof(dinifilename),"%s%s.txt",MinesBusinessDB,MineShops[id][Name]);
				if (db_Exists(dinifilename))
				{
					db_Set(dinifilename,"Owner","Server");
				}
				format(temp,sizeof(temp),gettext(825),MineShops[id][Name]);
				SystemMsg(playerid,COLOUR_OK,temp);
				set(temp,nullstr);
				new logstring[256];
				format(logstring, sizeof (logstring), "player: %d:  %s: sold the '%s' (Mine Shop)",playerid,oGetPlayerName(playerid),MineShops[id][Name]);
				WriteLog(logstring);
			}
		} else {
			SystemMsg(playerid,COLOUR_ERROR,gettext(561));
		}
	} else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(838));
	}
	Debug("businesses/mines.inc > SellMineShop - Stop");
	return 0;
}

public CollectMineShop(playerid){
Debug("businesses/mines.inc > CollectMineShop - Start");
new id=GetMineShopID(playerid);
			new temp[MAX_STRING];
			if (id > -1)
			{
				if (IsPlayerRegistered(playerid))
				{
					if(strcomp(MineShops[id][Owner], oGetPlayerName(playerid), true) == 0)
					{
						SystemMsg(playerid,COLOUR_ERROR,gettext(824));
					} else {
						if(MineShops[id][MoneyNow] > 0)
						{
							oGivePlayerMoney(playerid,0+MineShops[id][MoneyNow],1);
							SystemMsg(playerid,COLOUR_OK,gettext(826));
							MineShops[id][MoneyNow] = 0;
							new dinifilename[MAX_STRING];
							format(dinifilename,sizeof(dinifilename),"%s%s.txt",MinesBusinessDB,MineShops[id][Name]);
							if (db_Exists(dinifilename))
							{
								valstr(temp,MineShops[id][MoneyNow]);
								db_Set(dinifilename,"MoneyNow",temp);
								set(temp,nullstr);
							}
						} else {
							SystemMsg(playerid,COLOUR_ERROR,gettext(827));
						}
					}
				} else {
					SystemMsg(playerid,COLOUR_ERROR,gettext(561));
				}
			} else {
				SystemMsg(playerid,COLOUR_ERROR,gettext(838));
			}
			Debug("businesses/mines.inc > CollectMineShop - Stop");
			return 0;
}

public BuyMineShop(playerid){
Debug("businesses/mines.inc > BuyMineShop - Start");
			new id=GetMineShopID(playerid);
			new temp[MAX_STRING];
			if (id > -1)
			{
				if (IsPlayerRegistered(playerid))
				{
					if (Player[playerid][Level] >= MineShops[id][Level])
					{
						new price;
						price = MineShops[id][Cost] + MineShops[id][MoneyNow];
						if(Player[playerid][Money] >= price)
						{
							if(strcomp(MineShops[id][Owner], oGetPlayerName(playerid), true) == 1)
							{
								SystemMsg(playerid,COLOUR_ERROR,gettext(828));
							} else {

								oGivePlayerMoney(playerid,0-price,1);
								set(MineShops[id][Owner],oGetPlayerName(playerid));
								new dinifilename[MAX_STRING];
								format(dinifilename,sizeof(dinifilename),"%s%s.txt",MinesBusinessDB,MineShops[id][Name]);
								if (db_Exists(dinifilename))
								{
									db_Set(dinifilename,"Owner",MineShops[id][Owner]);
								}
								format(temp,sizeof(temp),gettext(829),MineShops[id][Name]);
								SystemMsg(playerid,COLOUR_OK,temp);
								SystemMsg(playerid,COLOUR_INFO,gettext(830));
								SystemMsg(playerid,COLOUR_INFO,gettext(831));
								SystemMsg(playerid,COLOUR_INFO,gettext(832));
								set(temp,nullstr);

								new logstring[256];
								format(logstring, sizeof (logstring), "player: %d:  %s: bought the '%s' (Mine Shop)",playerid,oGetPlayerName(playerid),MineShops[id][Name]);
								WriteLog(logstring);

							}
						} else {
							SystemMsg(playerid,COLOUR_ERROR,gettext(833));
						}
					} else {
						format(temp,sizeof(temp),gettext(834),MineShops[id][Level]);
						SystemMsg(playerid,COLOUR_ERROR,temp);
						set(temp,nullstr);
					}
				} else {
					SystemMsg(playerid,COLOUR_ERROR,gettext(561));
				}
			} else {
				SystemMsg(playerid,COLOUR_ERROR,gettext(838));
			}
			Debug("businesses/mines.inc > BuyMineShop - Stop");
			return 0;
}

public SetMinePrice(playerid,value){
Debug("businesses/mines.inc > SetMinePrice - Start");
new id=GetMineShopID(playerid);
if (value <= 0){
	SystemMsg(playerid,COLOUR_ERROR,gettext(835));
	Debug("businesses/mines.inc > SetMinePrice - Stop");
	return 1;
}
if (id > -1)
{
	if (IsPlayerRegistered(playerid))
	{
		if(strcomp(MineShops[id][Owner], oGetPlayerName(playerid), true) == 0)
		{
			SystemMsg(playerid,COLOUR_ERROR,gettext(824));
		} else {
			if (value > MineShops[id][MaxPrice]){
				SystemMsgFormat(playerid,COLOUR_ERROR,gettext(836),MineShops[id][MaxPrice]);
				return 1;
			}
			new dinifilename[MAX_STRING];
			format(dinifilename,sizeof(dinifilename),"%s%s.txt",MinesBusinessDB,MineShops[id][Name]);
			if (db_Exists(dinifilename))
			{
				MineShops[id][MinePrice] = value;
				new temp[MAX_STRING];
				valstr(temp,MineShops[id][MinePrice]);
				db_Set(dinifilename,"MinePrice",temp);
				set(temp,nullstr);
			}
			new string[MAX_STRING];
			format(string,sizeof(string),gettext(837),MineShops[id][Name],MineShops[id][MinePrice]);
			SystemMsg(playerid,COLOUR_OK,string);
			new logstring[256];
			format(logstring, sizeof (logstring), "player: %d:  %s: set price of '%s' (Mine Shop) to $%d",playerid,oGetPlayerName(playerid),MineShops[id][Name],MineShops[id][MinePrice]);
			WriteLog(logstring);
		}
	} else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(561));
	}
} else {
	SystemMsg(playerid,COLOUR_ERROR,gettext(838));
}
Debug("businesses/mines.inc > SetMinePrice - Stop");
return 0;
}

stock GetLowestMineID(vehicleid){
	for (new m=0; m<MAX_MINES; m++){
		if (MineExistTime[vehicleid][m] == 0){
		    return m;
		}
	}
	return -1;
}