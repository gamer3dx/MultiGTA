//Fuel Business module.
//Created by MadCat
//Based on Original Business Handler from GTO, created by Iain Gilbert

#include "base"
#include "vehicles_extras"

forward GetFuelStationID(playerid);
forward GetCanisterID(playerid);
forward FuelStations_OnGameModeInit();
forward IsPlayerAtFuelPickup(playerid);
forward BuyCanister(playerid);
forward BuyFuelstation(playerid);
forward SellFuelstation(playerid);
forward CollectFuelstation(playerid);
forward SetFuelPrice(playerid,value);
forward SetFuelCanisterPrice(playerid,value);
forward CanisterPickupCheck(playerid);

new canisterinfoshowed[MAX_PLAYERS] = 0;

new FuelBusinessDB[MAX_STRING] = "MultiGTA/Businesses/";

enum FuelStationInfo {
    Name[MAX_STRING],
    Float:Coord_X,
    Float:Coord_Y,
    Float:Coord_Z,
    Level,
    Cost,
    OneFuelPrice,
    MaxOneFuelPrice,
    CanisterPrice,
    MaxCanisterPrice,
    MoneyNow,
    Float:Canister_X,
    Float:Canister_Y,
    Float:Canister_Z,
    ItemsInStock,
    Components,
    Owner[MAX_NAME]
}

#define MAX_GSTATIONS 16

new Float:fuelstations[MAX_GSTATIONS][FuelStationInfo] = {
	{"Fuelstation - LV 1",2109.2126,917.5845,10.8203,5,50000,2,6,10,30,0,2116.4980,899.1451,11.1796,100,0,"Server"},
	{"Fuelstation - LV 2",2640.1831,1103.9224,10.8203,5,50000,2,6,10,30,0,2637.2790,1127.5390,11.1796,100,0,"Server"},
	{"Fuelstation - Bone County",611.8934,1694.7921,6.7193,5,50000,2,6,10,30,0,662.2729,1717.5540,7.1875,100,0,"Server"},
	{"Fuelstation - Tierra Robada 1",-1327.5398,2682.9771,49.7896,5,50000,2,6,10,30,0,-1273.4980,2711.7540,50.2662,100,0,"Server"},
	{"Fuelstation - (SF)Juniper Hollow",-2413.7427,975.9317,45.0031,5,50000,2,6,10,30,0,-2418.7190,969.7847,45.2968,100,0,"Server"},
	{"Fuelstation - (SF)Easter Basin",-1672.3597,414.2950,6.8866,5,50000,2,6,10,30,0,-1675.4680,431.6342,7.1796,100,0,"Server"},
	{"Fuelstation - Angel Pine",-2244.1365,-2560.6294,31.6276,5,50000,2,6,10,30,0,-2232.2380,-2558.0580,31.9218,100,0,"Server"},
	{"Fuelstation - Whetstone",-1603.0166,-2709.3589,48.2419,5,50000,2,6,10,30,0,-1624.0370,-2694.1710,48.7426,100,0,"Server"},
	{"Fuelstation - (LS)Idlewood",1939.3275,-1767.6813,13.2787,5,50000,2,6,10,30,0,1929.6920,-1776.1480,13.5468,100,0,"Server"},
	{"Fuelstation - Flint County",-94.7651,-1174.8079,1.9979,5,50000,2,6,10,30,0,-78.7764,-1169.7150,2.1463,100,0,"Server"},
	{"Fuelstation - Montgomery",1381.6699,462.6467,19.8540,5,50000,2,6,10,30,0,1374.2070,475.3847,20.0544,100,0,"Server"},
	{"Fuelstation - Dillimore",657.8167,-559.6507,16.0630,5,50000,2,6,10,30,0,660.9887,-573.3342,16.3359,100,0,"Server"},
	{"Fuelstation - Tierra Robada 2",-1478.2916,1862.8318,32.3617,5,50000,2,6,10,30,0,-1465.7350,1872.8400,32.6328,100,0,"Server"},
	{"Fuelstation - (LV)Spinybed",2147.3054,2744.9377,10.5263,5,50000,2,6,10,30,0,2150.6530,2734.0930,11.1763,100,0,"Server"},
	{"Fuelstation - (LV)The Emerald Isle",2204.9602,2480.3494,10.5278,5,50000,2,6,10,30,0,2188.6380,2469.8820,11.2421,100,0,"Server"},
	{"Fuelstation - (LV)Redsands West",1590.9493,2202.2637,10.5247,5,50000,2,6,10,30,0,1599.8910,2221.0370,11.06250,100,0,"Server"}
};

FuelBusinessLoadAll()
{
	Debug("businesses/fuel.inc > FuelBusinessLoadAll - Start");
	
	new temp[MAX_STRING];
	if (!db_Exists(DatabaseDB)) db_Create(DatabaseDB);
	if (!db_Isset(DatabaseDB,"Fuel_Business_DB")) db_Set(DatabaseDB,"Fuel_Business_DB",FuelBusinessDB);
	set(temp,db_Get(DatabaseDB,"Fuel_Business_DB"));
	if (strlen(temp) > 0) set(FuelBusinessDB,temp);
	if (!db_Exists(FuelBusinessDB))
	{

		FuelBusinessSaveAll();

		return;
	}
	for (new i=0;i<MAX_GSTATIONS;i++)
	{
		new FuelBusinessDBname[MAX_STRING];
		format(FuelBusinessDBname,sizeof(FuelBusinessDBname),"%s%s.txt",FuelBusinessDB,fuelstations[i][Name]);
		if (!db_Exists(FuelBusinessDBname))
		{
			FuelBusinessSaveAll();
		}
		set(temp,db_Get(FuelBusinessDBname,"Name"));
		if (strlen(temp) > 0) set(fuelstations[i][Name],temp);
		set(temp,nullstr);
		set(temp,db_Get(FuelBusinessDBname,"Level"));
		if (strlen(temp) > 0) fuelstations[i][Level] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(FuelBusinessDBname,"Cost"));
		if (strlen(temp) > 0) fuelstations[i][Cost] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(FuelBusinessDBname,"OneFuelPrice"));
		if (strlen(temp) > 0) fuelstations[i][OneFuelPrice] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(FuelBusinessDBname,"MaxOneFuelPrice"));
		if (strlen(temp) > 0) fuelstations[i][MaxOneFuelPrice] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(FuelBusinessDBname,"CanisterPrice"));
		if (strlen(temp) > 0) fuelstations[i][CanisterPrice] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(FuelBusinessDBname,"MaxCanisterPrice"));
		if (strlen(temp) > 0) fuelstations[i][MaxCanisterPrice] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(FuelBusinessDBname,"MoneyNow"));
		if (strlen(temp) > 0) fuelstations[i][MoneyNow] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(FuelBusinessDBname,"ItemsInStock"));
		if (strlen(temp) > 0) fuelstations[i][ItemsInStock] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(FuelBusinessDBname,"Components"));
		if (strlen(temp) > 0) fuelstations[i][Components] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(FuelBusinessDBname,"Owner"));
		if (strlen(temp) > 0) set(fuelstations[i][Owner],temp);
		set(temp,nullstr);
	}
	Debug("businesses/fuel.inc > FuelBusinessLoadAll - Stop");
	return;
}

FuelBusinessSaveAll()
{
	Debug("businesses/fuel.inc > FuelBusinessSaveAll - Start");
	new temp[MAX_STRING];
	if (db_Exists(DatabaseDB))
	{
		db_Set(DatabaseDB,"Fuel_Business_DB",FuelBusinessDB);
	}

	for (new i=0;i<MAX_GSTATIONS;i++)
	{
		new FuelBusinessDBname[MAX_STRING];
		format(FuelBusinessDBname,sizeof(FuelBusinessDBname),"%s%s.txt",FuelBusinessDB,fuelstations[i][Name]);
		if (db_Exists(FuelBusinessDBname)) {
			continue;
		}
		db_Create(FuelBusinessDBname);
		db_BeforeBigSaving(FuelBusinessDBname);
		db_Set(FuelBusinessDBname,"Name",fuelstations[i][Name]);
		valstr(temp,fuelstations[i][Level]);
		db_Set(FuelBusinessDBname,"Level",temp);
		set(temp,nullstr);
		valstr(temp,fuelstations[i][Cost]);
		db_Set(FuelBusinessDBname,"Cost",temp);
		set(temp,nullstr);
		valstr(temp,fuelstations[i][OneFuelPrice]);
		db_Set(FuelBusinessDBname,"OneFuelPrice",temp);
		set(temp,nullstr);
		valstr(temp,fuelstations[i][MaxOneFuelPrice]);
		db_Set(FuelBusinessDBname,"MaxOneFuelPrice",temp);
		set(temp,nullstr);
		valstr(temp,fuelstations[i][CanisterPrice]);
		db_Set(FuelBusinessDBname,"CanisterPrice",temp);
		set(temp,nullstr);
		valstr(temp,fuelstations[i][MaxCanisterPrice]);
		db_Set(FuelBusinessDBname,"MaxCanisterPrice",temp);
		set(temp,nullstr);
		valstr(temp,fuelstations[i][MoneyNow]);
		db_Set(FuelBusinessDBname,"MoneyNow",temp);
		set(temp,nullstr);
		valstr(temp,fuelstations[i][ItemsInStock]);
		db_Set(FuelBusinessDBname,"ItemsInStock",temp);
		set(temp,nullstr);
		valstr(temp,fuelstations[i][Components]);
		db_Set(FuelBusinessDBname,"Components",temp);
		set(temp,nullstr);
		db_Set(FuelBusinessDBname,"Owner",fuelstations[i][Owner]);
		db_AfterBigSaving(FuelBusinessDBname);

	}
	Debug("businesses/fuel.inc > FuelBusinessSaveAll - Stop");
	return;
}

public FuelStations_OnGameModeInit()
{
	Debug("businesses/fuel.inc > FuelStations_OnGameModeInit - Start");
	new count=0;
	for (new id; id < MAX_GSTATIONS; id++) 
	{
		AddActiveCheckpoint(fuelstations[id][Coord_X],fuelstations[id][Coord_Y],fuelstations[id][Coord_Z],15,2);
		CreateStreamPickup(1262,1,fuelstations[id][Canister_X],fuelstations[id][Canister_Y],fuelstations[id][Canister_Z]);
		CreateStreamMapIcon(55,fuelstations[id][Coord_X],fuelstations[id][Coord_Y],fuelstations[id][Coord_Z]);
		CreateDMapIcon(55,fuelstations[id][Coord_X],fuelstations[id][Coord_Y],fuelstations[id][Coord_Z],gettext(1755),gettext(1756));
		count++;
	}
	new logstring[256];
	format(logstring,sizeof(logstring),"Fuel Stations System Loaded. Fuel stations: %d",count);
	WriteLog(logstring);
	Debug("businesses/fuel.inc > FuelStations_OnGameModeInit - Stop");
}

public IsPlayerAtFuelPickup(playerid)
{
	for (new id=0;id<MAX_GSTATIONS;id++)
	{
		if (IsPlayerInRangeOfPoint(playerid,1.0,fuelstations[id][Canister_X],fuelstations[id][Canister_Y],fuelstations[id][Canister_Z]))
		{
			return 1;
		}
	}
	return 0;
}

public GetCanisterID(playerid)
{
	new lookupid=-1;
	for (new db=0;db<MAX_GSTATIONS;db++)
	{
		if (IsPlayerInRangeOfPoint(playerid,1.0,fuelstations[db][Canister_X],fuelstations[db][Canister_Y],fuelstations[db][Canister_Z])) lookupid = db;
	}
	return lookupid;
}

public GetFuelStationID(playerid)
{
	new lookupid=-1;
	for (new db=0;db<MAX_GSTATIONS;db++)
	{
		if (oIsPlayerInCheckpoint(playerid,fuelstations[db][Coord_X],fuelstations[db][Coord_Y],fuelstations[db][Coord_Z],10)) lookupid = db;
	}
	return lookupid;
}

public CanisterPickupCheck(playerid)
{
	if (playerid == INVALID_PLAYER_ID) return;
	if (!IsPlayerConnected(playerid)) return;
	if (!IsPlayerAtFuelPickup(playerid)) {canisterinfoshowed[playerid] = 0; return;}
	if (canisterinfoshowed[playerid] == 1) return;
	canisterinfoshowed[playerid] = 1;
	new id=GetCanisterID(playerid);
	SystemMsgFormat(playerid,COLOUR_INFO,gettext(789),fuelstations[id][CanisterPrice]);
	if (strcomp(fuelstations[id][Owner],oGetPlayerName(playerid),false) == 1){
	SystemMsg(playerid,COLOUR_INFO,gettext(790));
	if (fuelstations[id][MoneyNow] > 0){
	SystemMsgFormat(playerid,COLOUR_INFO,gettext(791),fuelstations[id][MoneyNow]);
	}
	}
}


public BuyCanister(playerid){
	Debug("businesses/fuel.inc > BuyCanister - Start");
	new id=GetCanisterID(playerid);
	if (oGetPlayerMoney(playerid) < fuelstations[id][CanisterPrice])
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(792));
		Debug("businesses/fuel.inc > BuyCanister - Stop");
		return 1;
	}
	if (fuelstations[id][ItemsInStock] == 0){
		ShowTextForPlayer(playerid, gettext(712), 5000, 1);
		Debug("businesses/fuel.inc > BuyCanister - Stop");
		return 1;
	}
	new string[MAX_STRING];
	if (strcomp(fuelstations[id][Owner],"Server",false) == 1){
	oGivePlayerMoney(playerid,-fuelstations[id][CanisterPrice],0);
	fuelstations[id][MoneyNow]=fuelstations[id][MoneyNow]+fuelstations[id][CanisterPrice];
	fuelstations[id][ItemsInStock]--;
	new dinifilename[MAX_STRING];
	new temp[MAX_STRING];
	format(dinifilename,sizeof(dinifilename),"%s%s.txt",FuelBusinessDB,fuelstations[id][Name]);
	if (db_Exists(dinifilename))
	{
		valstr(temp,fuelstations[id][MoneyNow]);
		db_Set(dinifilename,"MoneyNow",temp);
		set(temp,nullstr);
		valstr(temp,fuelstations[id][ItemsInStock]);
		db_Set(dinifilename,"ItemsInStock",temp);
		set(temp,nullstr);
	}
	Player[playerid][FuelCans]++;
	format(string,sizeof(string),gettext(793),Player[playerid][FuelCans]);
	} else if (strcomp(fuelstations[id][Owner],oGetPlayerName(playerid),false) == 1){
	Player[playerid][FuelCans]++;
	fuelstations[id][ItemsInStock]--;
	new dinifilename[MAX_STRING];
	new temp[MAX_STRING];
	format(dinifilename,sizeof(dinifilename),"%s%s.txt",FuelBusinessDB,fuelstations[id][Name]);
	if (db_Exists(dinifilename))
	{
		valstr(temp,fuelstations[id][ItemsInStock]);
		db_Set(dinifilename,"ItemsInStock",temp);
		set(temp,nullstr);
	}
	format(string,sizeof(string),gettext(794),Player[playerid][FuelCans]);
	}
	else
	{
	oGivePlayerMoney(playerid,-fuelstations[id][CanisterPrice],0);
	fuelstations[id][MoneyNow]=fuelstations[id][MoneyNow]+fuelstations[id][CanisterPrice];
	fuelstations[id][ItemsInStock]--;
	new dinifilename[MAX_STRING];
	new temp[MAX_STRING];
	format(dinifilename,sizeof(dinifilename),"%s%s.txt",FuelBusinessDB,fuelstations[id][Name]);
	if (db_Exists(dinifilename))
	{
		valstr(temp,fuelstations[id][MoneyNow]);
		db_Set(dinifilename,"MoneyNow",temp);
		set(temp,nullstr);
		valstr(temp,fuelstations[id][ItemsInStock]);
		db_Set(dinifilename,"ItemsInStock",temp);
		set(temp,nullstr);
	}
	Player[playerid][FuelCans]++;
	fuelstations[id][MoneyNow]=fuelstations[id][MoneyNow]+fuelstations[id][CanisterPrice];
	format(string,sizeof(string),gettext(793),Player[playerid][FuelCans]);
	}
	SystemMsg(playerid,COLOUR_OK,string);
	new logstring[256];
	format(logstring,sizeof(logstring), "player: %d:  %s: buy fuel canister. Total canisters: %d.",playerid,oGetPlayerName(playerid),Player[playerid][FuelCans]);
	WriteLog(logstring);
	Debug("businesses/fuel.inc > BuyCanister - Stop");
	return 0;
}

public BuyFuelstation(playerid){
new id=GetFuelStationID(playerid);
			new temp[MAX_STRING];
			if (id > -1)
			{
				if (IsPlayerRegistered(playerid))
				{
					if (Player[playerid][Level] >= fuelstations[id][Level])
					{
						new price;
						price = fuelstations[id][Cost] + fuelstations[id][MoneyNow];
						if(Player[playerid][Money] >= price)
						{
							if(strcomp(fuelstations[id][Owner], oGetPlayerName(playerid), true) == 1)
							{
								SystemMsg(playerid,COLOUR_ERROR,gettext(795));
							} else {

								oGivePlayerMoney(playerid,0-price,1);
								set(fuelstations[id][Owner],oGetPlayerName(playerid));
								new dinifilename[MAX_STRING];
								format(dinifilename,sizeof(dinifilename),"%s%s.txt",FuelBusinessDB,fuelstations[id][Name]);
								if (db_Exists(dinifilename))
								{
									db_Set(dinifilename,"Owner",fuelstations[id][Owner]);
								}
								format(temp,sizeof(temp),gettext(796),fuelstations[id][Name]);
								SystemMsg(playerid,COLOUR_OK,temp);
								SystemMsg(playerid,COLOUR_INFO,gettext(797));
								SystemMsg(playerid,COLOUR_INFO,gettext(798));
								SystemMsg(playerid,COLOUR_INFO,gettext(799));
								set(temp,nullstr);
								new logstring[256];
								format(logstring, sizeof (logstring), "player: %d:  %s: bought the '%s' (Fuel Station)",playerid,oGetPlayerName(playerid),fuelstations[id][Name]);
								WriteLog(logstring);

							}
						} else {
							SystemMsg(playerid,COLOUR_ERROR,gettext(800));
						}
					} else {
						format(temp,sizeof(temp),gettext(801),fuelstations[id][Level]);
						SystemMsg(playerid,COLOUR_ERROR,temp);
						set(temp,nullstr);
					}
				} else {
					SystemMsg(playerid,COLOUR_ERROR,gettext(561));
				}
			} else {
				SystemMsg(playerid,COLOUR_ERROR,gettext(802));
			}
			Debug("businesses/fuel.inc > BuyFuelstation - Stop");
			return 0;
}

public SellFuelstation(playerid){
			Debug("businesses/fuel.inc > SellFuelstation - Start");
			new id=GetFuelStationID(playerid);
			new temp[MAX_STRING];
			if (id > -1)
			{
				if (IsPlayerRegistered(playerid))
				{
					if(strcomp(fuelstations[id][Owner], oGetPlayerName(playerid), true) == 0)
					{
						SystemMsg(playerid,COLOUR_ERROR,gettext(803));
					} else {
						new price;
						price = fuelstations[id][Cost] + fuelstations[id][MoneyNow];
						price = (price * 85) / 100;
						oGivePlayerMoney(playerid,0+price,1);
						set(fuelstations[id][Owner],"Server");
						new dinifilename[MAX_STRING];
						format(dinifilename,sizeof(dinifilename),"%s%s.txt",FuelBusinessDB,fuelstations[id][Name]);
						if (db_Exists(dinifilename))
						{
							db_Set(dinifilename,"Owner","Server");
						}
						format(temp,sizeof(temp),gettext(804),fuelstations[id][Name]);
						SystemMsg(playerid,COLOUR_OK,temp);
						set(temp,nullstr);
						new logstring[256];
						format(logstring, sizeof (logstring), "player: %d:  %s: sold the '%s' (Fuel Station)",playerid,oGetPlayerName(playerid),fuelstations[id][Name]);
						WriteLog(logstring);
					}
				} else {
					SystemMsgFormat(playerid,COLOUR_ERROR,gettext(561),0);
				}
			} else {
				SystemMsg(playerid,COLOUR_ERROR,gettext(802));
			}
			Debug("businesses/fuel.inc > SellFuelstation - Stop");
			return 0;
}

public CollectFuelstation(playerid){
			Debug("businesses/fuel.inc > CollectFuelstation - Start");
			new id=GetFuelStationID(playerid);
			new temp[MAX_STRING];
			if (id > -1)
			{
				if (IsPlayerRegistered(playerid))
				{
					if(strcomp(fuelstations[id][Owner], oGetPlayerName(playerid), true) == 0)
					{
						SystemMsg(playerid,COLOUR_ERROR,gettext(803));
					} else {
						if(fuelstations[id][MoneyNow] > 0)
						{
							oGivePlayerMoney(playerid,0+fuelstations[id][MoneyNow],1);
							SystemMsg(playerid,COLOUR_OK,gettext(805));
							fuelstations[id][MoneyNow] = 0;
							new dinifilename[MAX_STRING];
							format(dinifilename,sizeof(dinifilename),"%s%s.txt",FuelBusinessDB,fuelstations[id][Name]);
							if (db_Exists(dinifilename))
							{
								valstr(temp,fuelstations[id][MoneyNow]);
								db_Set(dinifilename,"MoneyNow",temp);
								set(temp,nullstr);
							}
						} else {
							SystemMsg(playerid,COLOUR_ERROR,gettext(806));
						}
					}
				} else {
					SystemMsg(playerid,COLOUR_ERROR,gettext(561));
				}
			} else {
				SystemMsg(playerid,COLOUR_ERROR,gettext(802));
			}
			Debug("businesses/fuel.inc > CollectFuelstation - Stop");
			return 0;
}

public SetFuelPrice(playerid,value){
		Debug("businesses/fuel.inc > SetFuelPrice - Start");
		new id=GetFuelStationID(playerid);
		if (value <= 0){
			SystemMsg(playerid,COLOUR_ERROR,gettext(807));
			Debug("businesses/fuel.inc > SetFuelPrice - Stop");			
			return 1;
		}
		if (id > -1)
		{
			if (IsPlayerRegistered(playerid))
			{
				if(strcomp(fuelstations[id][Owner], oGetPlayerName(playerid), true) == 0)
				{
					SystemMsg(playerid,COLOUR_ERROR,gettext(803));
				} else {
					if (value > fuelstations[id][MaxOneFuelPrice]){
						SystemMsgFormat(playerid,COLOUR_ERROR,gettext(808),fuelstations[id][MaxOneFuelPrice]);
						return 1;
					}
					new dinifilename[MAX_STRING];
					format(dinifilename,sizeof(dinifilename),"%s%s.txt",FuelBusinessDB,fuelstations[id][Name]);
					if (db_Exists(dinifilename))
					{
						fuelstations[id][OneFuelPrice] = value;
						new temp[MAX_STRING];
						valstr(temp,fuelstations[id][OneFuelPrice]);
						db_Set(dinifilename,"OneFuelPrice",temp);
						set(temp,nullstr);
					}
					new string[MAX_STRING];
					format(string,sizeof(string),gettext(809),fuelstations[id][Name],fuelstations[id][OneFuelPrice]);
					SystemMsg(playerid,COLOUR_OK,string);
					new logstring[256];
					format(logstring, sizeof (logstring), "player: %d:  %s: set price of '%s' (Fuel station) to $%d",playerid,oGetPlayerName(playerid),fuelstations[id][Name],fuelstations[id][OneFuelPrice]);
					WriteLog(logstring);
				}
			} else {
				SystemMsg(playerid,COLOUR_ERROR,gettext(561));
			}
		} else {
			SystemMsg(playerid,COLOUR_ERROR,gettext(802));
		}
		Debug("businesses/fuel.inc > SetFuelPrice - Stop");
		return 0;
}

public SetFuelCanisterPrice(playerid,value){
			Debug("businesses/fuel.inc > SetFuelCanisterPrice - Start");
			new id=GetCanisterID(playerid);
			if (value <= 0){
				SystemMsg(playerid,COLOUR_ERROR,gettext(807));
				Debug("businesses/fuel.inc > SetFuelCanisterPrice - Stop");			
				return 1;
			}
			if (id > -1)
			{
				if (IsPlayerRegistered(playerid))
				{
					if(strcomp(fuelstations[id][Owner], oGetPlayerName(playerid), true) == 0)
					{
						SystemMsg(playerid,COLOUR_ERROR,gettext(803));
					} else {
						if (value > fuelstations[id][MaxCanisterPrice]){
							SystemMsgFormat(playerid,COLOUR_ERROR,gettext(808),fuelstations[id][MaxCanisterPrice]);
							return 1;
						}
						new dinifilename[MAX_STRING];
						format(dinifilename,sizeof(dinifilename),"%s%s.txt",FuelBusinessDB,fuelstations[id][Name]);
						if (db_Exists(dinifilename))
						{
							fuelstations[id][CanisterPrice] = value;
							new temp[MAX_STRING];
							valstr(temp,fuelstations[id][CanisterPrice]);
							db_Set(dinifilename,"CanisterPrice",temp);
							set(temp,nullstr);
						}
						new string[MAX_STRING];
						format(string,sizeof(string),gettext(810),fuelstations[id][Name],fuelstations[id][CanisterPrice]);
						SystemMsg(playerid,COLOUR_OK,string);
						new logstring[256];
						format(logstring, sizeof (logstring), "player: %d:  %s: set price of '%s' (Fuel station) to $%d",playerid,oGetPlayerName(playerid),fuelstations[id][Name],fuelstations[id][CanisterPrice]);
						WriteLog(logstring);
					}
				} else {
					SystemMsg(playerid,COLOUR_ERROR,gettext(561));
				}
			} else {
				SystemMsg(playerid,COLOUR_ERROR,gettext(802));
			}
			Debug("businesses/fuel.inc > SetFuelCanisterPrice - Stop");
			return 0;
}