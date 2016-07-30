//Drink Vendors module.
//Created by MadCat
//Based on Original Business Handler from GTO, created by Iain Gilbert
//Coords from GTAT

#include "base"
#include "player"

forward Drinks_OnGameModeInit();
forward IsPlayerAtDrinkVendor(playerid);
forward DrinkPickupCheck(playerid);
forward Drink_OnPlayerCommandText(playerid,text[]);
forward GetDrinkVendorID(playerid);
forward BuyDrink(playerid);
forward SellDrinkVendor(playerid);
forward CollectDrinkVendor(playerid);
forward BuyDrinkVendor(playerid);
forward SetDrinkPrice(playerid,value);

new dvendorinfoshowed[MAX_PLAYERS] = 0;

new PlayerDrinkTick[MAX_PLAYERS];

new DrinkBusinessDB[MAX_STRING] = "MultiGTA/Businesses/";

#define MAX_DRINKBUSINESSES 12

enum DrinkVendorInfo {
    Name[MAX_STRING],
    Float:Coord_X,
    Float:Coord_Y,
    Float:Coord_Z,
    Level,
    Cost,
    DrinkPrice,
    MaxPrice,
    ItemsInStock,
    Components,
    MoneyNow,
    Owner[MAX_NAME]
}

new DrinkVendors[MAX_DRINKBUSINESSES][DrinkVendorInfo] = {
	{"24-7 - (LV)Starfish Casino 1",2549.1382,1970.4800,10.8203,10,100000,50,150,1500,0,0,"Server"},
	{"24-7 - (LV)Starfish Casino 2",2192.8645,1992.9717,12.2969,10,100000,50,150,1500,0,0,"Server"},
	{"24-7 - (LS)Commerce",1350.1237,-1757.7849,13.5078,10,100000,50,150,1500,0,0,"Server"},
	{"24-7 - (LS)Mullholland",999.1581,-922.7105,42.1797,10,100000,50,150,1500,0,0,"Server"},
	{"24-7 - (LS)Temple",1318.1299,-900.3708,39.5781,10,100000,50,150,1500,0,0,"Server"},
	{"24-7 - Whetstone",-1566.1554,-2734.4534,48.5448,10,100000,50,150,1500,0,0,"Server"},
	{"24-7 - (LV)Creek",2888.3052,2454.5820,11.0690,10,100000,50,150,1500,0,0,"Server"},
	{"24-7 - (LS)Idlewood",1832.0914,-1846.5950,13.5781,10,100000,50,150,1500,0,0,"Server"},
	{"24-7 - (LV)The Emerald Isle",2099.7461,2223.4846,11.0234,10,100000,50,150,1500,0,0,"Server"},
	{"24-7 - (LV)Redsands East",1936.0497,2305.1279,10.8203,10,100000,50,150,1500,0,0,"Server"},
	{"24-7 - (LV)Old Venturas Strip",2455.2944,2062.8630,10.8203,10,100000,50,150,1500,0,0,"Server"},
	{"24-7 - (LV)Roca Escalante",2245.9651,2398.4990,10.8203,10,100000,50,150,1500,0,0,"Server"}
	
}; 

DrinkBusinessLoadAll()
{
	Debug("businesses/drinks.inc > DrinkBusinessLoadAll - Start");
	
	new temp[MAX_STRING];
	if (!db_Exists(DatabaseDB)) db_Create(DatabaseDB);
	if (!db_Isset(DatabaseDB,"Drink_Business_DB")) db_Set(DatabaseDB,"Drink_Business_DB",DrinkBusinessDB);
	set(temp,db_Get(DatabaseDB,"Drink_Business_DB"));
	if (strlen(temp) > 0) set(DrinkBusinessDB,temp);
	if (!db_Exists(DrinkBusinessDB))
	{

		DrinkBusinessSaveAll();

		return;
	}
	for (new i=0;i<MAX_DRINKBUSINESSES;i++)
	{
		new DrinkBusinessDBname[MAX_STRING];
		format(DrinkBusinessDBname,sizeof(DrinkBusinessDBname),"%s%s.txt",DrinkBusinessDB,DrinkVendors[i][Name]);
		if (!db_Exists(DrinkBusinessDBname))
		{
			DrinkBusinessSaveAll();
		}
		set(temp,db_Get(DrinkBusinessDBname,"Name"));
		if (strlen(temp) > 0) set(DrinkVendors[i][Name],temp);
		set(temp,nullstr);
		set(temp,db_Get(DrinkBusinessDBname,"Level"));
		if (strlen(temp) > 0) DrinkVendors[i][Level] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(DrinkBusinessDBname,"Cost"));
		if (strlen(temp) > 0) DrinkVendors[i][Cost] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(DrinkBusinessDBname,"DrinkPrice"));
		if (strlen(temp) > 0) DrinkVendors[i][DrinkPrice] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(DrinkBusinessDBname,"MaxPrice"));
		if (strlen(temp) > 0) DrinkVendors[i][MaxPrice] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(DrinkBusinessDBname,"ItemsInStock"));
		if (strlen(temp) > 0) DrinkVendors[i][ItemsInStock] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(DrinkBusinessDBname,"MoneyNow"));
		if (strlen(temp) > 0) DrinkVendors[i][MoneyNow] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(DrinkBusinessDBname,"Components"));
		if (strlen(temp) > 0) DrinkVendors[i][Components] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(DrinkBusinessDBname,"Owner"));
		if (strlen(temp) > 0) set(DrinkVendors[i][Owner],temp);
		set(temp,nullstr);
	}
	Debug("businesses/drinks.inc > DrinkBusinessLoadAll - Stop");
	return;

}

DrinkBusinessSaveAll()
{
	Debug("businesses/drinks.inc > DrinkBusinessSaveAll - Start");
	new temp[MAX_STRING];
	if (db_Exists(DatabaseDB))
	{
		db_Set(DatabaseDB,"Drink_Business_DB",DrinkBusinessDB);
	}

	for (new i=0;i<MAX_DRINKBUSINESSES;i++)
	{
		new DrinkBusinessDBname[MAX_STRING];
		format(DrinkBusinessDBname,sizeof(DrinkBusinessDBname),"%s%s.txt",DrinkBusinessDB,DrinkVendors[i][Name]);
		if (db_Exists(DrinkBusinessDBname)) {
			continue;
		}
		db_Create(DrinkBusinessDBname);
		db_BeforeBigSaving(DrinkBusinessDBname);
		db_Set(DrinkBusinessDBname,"Name",DrinkVendors[i][Name]);
		valstr(temp,DrinkVendors[i][Level]);
		db_Set(DrinkBusinessDBname,"Level",temp);
		set(temp,nullstr);
		valstr(temp,DrinkVendors[i][Cost]);
		db_Set(DrinkBusinessDBname,"Cost",temp);
		set(temp,nullstr);
		valstr(temp,DrinkVendors[i][DrinkPrice]);
		db_Set(DrinkBusinessDBname,"DrinkPrice",temp);
		set(temp,nullstr);
		valstr(temp,DrinkVendors[i][MaxPrice]);
		db_Set(DrinkBusinessDBname,"MaxPrice",temp);
		set(temp,nullstr);
		valstr(temp,DrinkVendors[i][MoneyNow]);
		db_Set(DrinkBusinessDBname,"MoneyNow",temp);
		set(temp,nullstr);
		valstr(temp,DrinkVendors[i][ItemsInStock]);
		db_Set(DrinkBusinessDBname,"ItemsInStock",temp);
		set(temp,nullstr);
		valstr(temp,DrinkVendors[i][Components]);
		db_Set(DrinkBusinessDBname,"Components",temp);
		set(temp,nullstr);
		db_Set(DrinkBusinessDBname,"Owner",DrinkVendors[i][Owner]);
		db_AfterBigSaving(DrinkBusinessDBname);
	}
	Debug("businesses/drinks.inc > DrinkBusinessSaveAll - Stop");
	return;
}

public Drinks_OnGameModeInit()
{
	Debug("businesses/drinks.inc > Drinks_OnGameModeInit - Start");
	new count = 0;
	for (new dbid=0;dbid<MAX_DRINKBUSINESSES;dbid++)
	{
		CreateStreamPickup(1241,1,DrinkVendors[dbid][Coord_X],DrinkVendors[dbid][Coord_Y],DrinkVendors[dbid][Coord_Z]);
		CreateStreamMapIcon(21,DrinkVendors[dbid][Coord_X],DrinkVendors[dbid][Coord_Y],DrinkVendors[dbid][Coord_Z]);
		CreateDMapIcon(21,DrinkVendors[dbid][Coord_X],DrinkVendors[dbid][Coord_Y],DrinkVendors[dbid][Coord_Z],gettext(1753),gettext(1754));
		count++;
	}
	new logstring[256];
	format(logstring,sizeof(logstring),"Drinks System Loaded. Drink Businesses: %d",count);
	WriteLog(logstring);
	Debug("businesses/drinks.inc > Drinks_OnGameModeInit - Stop");
}

public IsPlayerAtDrinkVendor(playerid)
{
	for (new dbid=0;dbid<MAX_DRINKBUSINESSES;dbid++)
	{
		if (IsPlayerInRangeOfPoint(playerid,1.0,DrinkVendors[dbid][Coord_X],DrinkVendors[dbid][Coord_Y],DrinkVendors[dbid][Coord_Z]))
		{
			return 1;
		}
	}
	return 0;
}

public GetDrinkVendorID(playerid)
{
	new lookupid=-1;
	for (new dbid=0;dbid<MAX_DRINKBUSINESSES;dbid++)
	{
		if (IsPlayerInRangeOfPoint(playerid,1.0,DrinkVendors[dbid][Coord_X],DrinkVendors[dbid][Coord_Y],DrinkVendors[dbid][Coord_Z])) lookupid = dbid;
	}
	return lookupid;
}

public DrinkPickupCheck(playerid)
{
	if (playerid == INVALID_PLAYER_ID) return;
	if (!IsPlayerConnected(playerid)) return;
	if (!IsPlayerAtDrinkVendor(playerid)) {dvendorinfoshowed[playerid] = 0; return;}
	if (dvendorinfoshowed[playerid] == 1) return;
	dvendorinfoshowed[playerid] = 1;
	new id=GetDrinkVendorID(playerid);
	SystemMsgFormat(playerid,COLOUR_INFO,gettext(739),DrinkVendors[id][DrinkPrice]);
	if (strcomp(DrinkVendors[id][Owner],oGetPlayerName(playerid),false) == 1){
		SystemMsg(playerid,COLOUR_INFO,gettext(740));
		if (DrinkVendors[id][MoneyNow] > 0){
			SystemMsgFormat(playerid,COLOUR_INFO,gettext(741),DrinkVendors[id][MoneyNow]);
		}
	}
	if (strcomp(DrinkVendors[id][Owner],"Server",false) == 1){
		new string[MAX_STRING];
		format(string,sizeof(string),gettext(742),DrinkVendors[id][Cost]+DrinkVendors[id][MoneyNow],DrinkVendors[id][Level]);
		SystemMsg(playerid,COLOUR_INFO,string);
	}
}



public Drink_OnPlayerCommandText(playerid,text[])
{
	if (!IsPlayerConnected(playerid)) return 0;

	dcmd(drink,5,text);

	return 0;
}

dcmd_drink(playerid,params[])
{
	#pragma unused params
	Debug("businesses/drinks.inc > Command 'drink' - Start");
	if (Player[playerid][Drinks] == 0)
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(743));
		Debug("businesses/drinks.inc > Command 'drink' - Stop");
		return 1;
	}

	if(GetTickCount() > PlayerDrinkTick[playerid] + DRINK_TIME) {
	    Debug("businesses/drinks.inc > Command 'drink' - Stop");
	    return SystemMsgFormat(playerid,COLOUR_ERROR,gettext(744),(DRINK_TIME/1000));
	}


	if (IsPlayerInAnyVehicle(playerid))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(745));
		Debug("businesses/drinks.inc > Command 'drink' - Stop");
		return 1;
	}

	if (oGetPlayerHealth(playerid) >= GetMaxHealth(playerid))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(746));
		Debug("businesses/drinks.inc > Command 'drink' - Stop");
		return 1;
	}

	Player[playerid][Drinks]--;
	Player[playerid][DrinksUsed]++;
	oSetPlayerHealth(playerid, GetMaxHealth(playerid));
	SystemMsg(playerid,COLOUR_OK,gettext(747));
	PlayerDrinkTick[playerid] = GetTickCount();

	new logstring[256];
	format(logstring,sizeof(logstring), "player: %d:  %s: used Energy Drink. Drinks left: %d",playerid,oGetPlayerName(playerid),Player[playerid][Drinks]);
	WriteLog(logstring);
	Debug("businesses/drinks.inc > Command 'drink' - Stop");
	return 1;
}

public BuyDrink(playerid){
		Debug("businesses/drinks.inc > BuyDrink - Start");
		new id=GetDrinkVendorID(playerid);
		if (oGetPlayerMoney(playerid) < DrinkVendors[id][DrinkPrice])
		{
			SystemMsg(playerid,COLOUR_ERROR,gettext(748));
			Debug("businesses/drinks.inc > BuyDrink - Stop");
			return 1;
		}
		if (DrinkVendors[id][ItemsInStock] == 0){
			ShowTextForPlayer(playerid, gettext(712), 5000, 1);
			Debug("businesses/drinks.inc > BuyDrink - Stop");
			return 1;
		}
		new string[MAX_STRING];
		if (strcomp(DrinkVendors[id][Owner],"Server",false) == 1){
		oGivePlayerMoney(playerid,-DrinkVendors[id][DrinkPrice],0);
		DrinkVendors[id][MoneyNow]=DrinkVendors[id][MoneyNow]+DrinkVendors[id][DrinkPrice];
		DrinkVendors[id][ItemsInStock]--;
		new dinifilename[MAX_STRING];
		new temp[256];
		format(dinifilename,sizeof(dinifilename),"%s%s.txt",DrinkBusinessDB,DrinkVendors[id][Name]);
		if (db_Exists(dinifilename))
		{
			valstr(temp,DrinkVendors[id][MoneyNow]);
			db_Set(dinifilename,"MoneyNow",temp);
			set(temp,nullstr);
			valstr(temp,DrinkVendors[id][ItemsInStock]);
			db_Set(dinifilename,"ItemsInStock",temp);
			set(temp,nullstr);
		}
		Player[playerid][Drinks]++;
		format(string,sizeof(string), gettext(749),Player[playerid][Drinks]);
		} else if (strcomp(DrinkVendors[id][Owner],oGetPlayerName(playerid),false) == 1){
		Player[playerid][Drinks]++;
		DrinkVendors[id][ItemsInStock]--;
		new dinifilename[MAX_STRING];
		new temp[256];
		format(dinifilename,sizeof(dinifilename),"%s%s.txt",DrinkBusinessDB,DrinkVendors[id][Name]);
		if (db_Exists(dinifilename))
		{
			valstr(temp,DrinkVendors[id][MoneyNow]);
			db_Set(dinifilename,"MoneyNow",temp);
			set(temp,nullstr);
			valstr(temp,DrinkVendors[id][ItemsInStock]);
			db_Set(dinifilename,"ItemsInStock",temp);
			set(temp,nullstr);
		}
		format(string,sizeof(string),gettext(750),Player[playerid][Drinks]);
		}
		else
		{
		oGivePlayerMoney(playerid,-DrinkVendors[id][DrinkPrice],0);
		Player[playerid][Drinks]++;
		DrinkVendors[id][ItemsInStock]--;
		DrinkVendors[id][MoneyNow]=DrinkVendors[id][MoneyNow]+DrinkVendors[id][DrinkPrice];
		new dinifilename[MAX_STRING];
		new temp[256];
		format(dinifilename,sizeof(dinifilename),"%s%s.txt",DrinkBusinessDB,DrinkVendors[id][Name]);
		if (db_Exists(dinifilename))
		{
			valstr(temp,DrinkVendors[id][MoneyNow]);
			db_Set(dinifilename,"MoneyNow",temp);
			set(temp,nullstr);
		}
		format(string,sizeof(string),gettext(749),Player[playerid][Drinks]);
		}
		SystemMsg(playerid,COLOUR_OK,string);
		new logstring[256];
		format(logstring,sizeof(logstring), "player: %d:  %s: buy energy drink. Total drinks: %d.",playerid,oGetPlayerName(playerid),Player[playerid][Drinks]);
		WriteLog(logstring);
		Debug("businesses/drinks.inc > BuyDrink - Stop");
		return 1;
}

public SellDrinkVendor(playerid){
Debug("businesses/drinks.inc > SellDrinkVendor - Start");
new id=GetDrinkVendorID(playerid);
new temp[MAX_STRING];
	if (id > -1)
	{
		if (IsPlayerRegistered(playerid))
		{
			if(strcomp(DrinkVendors[id][Owner], oGetPlayerName(playerid), true) == 0)
			{
				SystemMsg(playerid,COLOUR_ERROR,gettext(752));
			} else {
				new price;
				price = (DrinkVendors[id][Cost] + DrinkVendors[id][MoneyNow])*85 / 100;
				oGivePlayerMoney(playerid,0+price,1);
				set(DrinkVendors[id][Owner],"Server");
				new dinifilename[MAX_STRING];
				format(dinifilename,sizeof(dinifilename),"%s%s.txt",DrinkBusinessDB,DrinkVendors[id][Name]);
				if (db_Exists(dinifilename))
				{
					db_Set(dinifilename,"Owner","Server");
				}
				format(temp,sizeof(temp),gettext(762),DrinkVendors[id][Name]);
				SystemMsg(playerid,COLOUR_OK,temp);
				set(temp,nullstr);
				new logstring[256];
				format(logstring, sizeof (logstring), "player: %d:  %s: sold the '%s' (Energy Drink Vendor)",playerid,oGetPlayerName(playerid),DrinkVendors[id][Name]);
				WriteLog(logstring);
			}
		} else {
			SystemMsg(playerid,COLOUR_ERROR,gettext(561));
		}
	} else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(759));
	}
	Debug("businesses/drinks.inc > SellDrinkVendor - Stop");
	return 0;
}

public CollectDrinkVendor(playerid){
Debug("businesses/drinks.inc > CollectDrinkVendor - Start");
new id=GetDrinkVendorID(playerid);
			new temp[MAX_STRING];
			if (id > -1)
			{
				if (IsPlayerRegistered(playerid))
				{
					if(strcomp(DrinkVendors[id][Owner], oGetPlayerName(playerid), true) == 1)
					{
						if(DrinkVendors[id][MoneyNow] > 0)
						{
							oGivePlayerMoney(playerid,0+DrinkVendors[id][MoneyNow],1);
							SystemMsg(playerid,COLOUR_OK,gettext(761));
							DrinkVendors[id][MoneyNow] = 0;
							new dinifilename[MAX_STRING];
							format(dinifilename,sizeof(dinifilename),"%s%s.txt",DrinkBusinessDB,DrinkVendors[id][Name]);
							if (db_Exists(dinifilename))
							{
								valstr(temp,DrinkVendors[id][MoneyNow]);
								db_Set(dinifilename,"MoneyNow",temp);
								set(temp,nullstr);
							}
						} else {
							SystemMsg(playerid,COLOUR_ERROR,gettext(760));
						}
					} else {
						SystemMsg(playerid,COLOUR_ERROR,gettext(752));
					}
				} else {
					SystemMsg(playerid,COLOUR_ERROR,gettext(561));
				}
			} else {
				SystemMsg(playerid,COLOUR_ERROR,gettext(759));
			}
			Debug("businesses/drinks.inc > CollectDrinkVendor - Stop");
			return 0;
}

public BuyDrinkVendor(playerid){
Debug("businesses/drinks.inc > BuyDrinKVendor - Start");
			new id=GetDrinkVendorID(playerid);
			new temp[MAX_STRING];
			if (id > -1)
			{
				if (IsPlayerRegistered(playerid))
				{
					if (Player[playerid][Level] >= DrinkVendors[id][Level])
					{
						new price;
						price = DrinkVendors[id][Cost] + DrinkVendors[id][MoneyNow];
						if(Player[playerid][Money] >= price)
						{
							if(strcomp(DrinkVendors[id][Owner], oGetPlayerName(playerid), true) == 1)
							{
								SystemMsg(playerid,COLOUR_ERROR,gettext(751));
							} else {

								oGivePlayerMoney(playerid,0-price,1);
								set(DrinkVendors[id][Owner],oGetPlayerName(playerid));
								new dinifilename[MAX_STRING];
								format(dinifilename,sizeof(dinifilename),"%s%s.txt",DrinkBusinessDB,DrinkVendors[id][Name]);
								if (db_Exists(dinifilename))
								{
									db_Set(dinifilename,"Owner",DrinkVendors[id][Owner]);
								}
								format(temp,sizeof(temp),gettext(753),DrinkVendors[id][Name]);
								SystemMsg(playerid,COLOUR_OK,temp);
								SystemMsg(playerid,COLOUR_INFO,gettext(754));
								SystemMsg(playerid,COLOUR_INFO,gettext(755));
								SystemMsg(playerid,COLOUR_INFO,gettext(756));
								set(temp,nullstr);

								new logstring[256];
								format(logstring, sizeof (logstring), "player: %d:  %s: bought the '%s' (Energy Drink vendor)",playerid,oGetPlayerName(playerid),DrinkVendors[id][Name]);
								WriteLog(logstring);

							}
						} else {
							SystemMsg(playerid,COLOUR_ERROR,gettext(757));
						}
					} else {
						format(temp,sizeof(temp),gettext(758),DrinkVendors[id][Level]);
						SystemMsg(playerid,COLOUR_ERROR,temp);
						set(temp,nullstr);
					}
				} else {
					SystemMsg(playerid,COLOUR_ERROR,gettext(561));
				}
			} else {
				SystemMsg(playerid,COLOUR_ERROR,gettext(759));
			}
			Debug("businesses/drinks.inc > BuyDrinKVendor - Stop");
			return 0;
}

public SetDrinkPrice(playerid,value){
Debug("businesses/drinks.inc > SetDrinkPrice - Start");
new id=GetDrinkVendorID(playerid);
if (value <= 0){
	SystemMsg(playerid,COLOUR_ERROR,gettext(763));
	Debug("businesses/drinks.inc > SetDrinkPrice - Stop");
	return 1;
}
if (id > -1)
{
	if (IsPlayerRegistered(playerid))
	{
		if(strcomp(DrinkVendors[id][Owner], oGetPlayerName(playerid), true) == 0)
		{
			SystemMsg(playerid,COLOUR_ERROR,gettext(752));
		} else {
			if (value > DrinkVendors[id][MaxPrice]){
				SystemMsgFormat(playerid,COLOUR_ERROR,gettext(764),DrinkVendors[id][MaxPrice]);
				return 1;
			}
			new dinifilename[MAX_STRING];
			format(dinifilename,sizeof(dinifilename),"%s%s.txt",DrinkBusinessDB,DrinkVendors[id][Name]);
			if (db_Exists(dinifilename))
			{
				DrinkVendors[id][DrinkPrice] = value;
				new temp[MAX_STRING];
				valstr(temp,DrinkVendors[id][DrinkPrice]);
				db_Set(dinifilename,"DrinkPrice",temp);
				set(temp,nullstr);
			}
			new string[MAX_STRING];
			format(string,sizeof(string),gettext(765),DrinkVendors[id][Name],DrinkVendors[id][DrinkPrice]);
			SystemMsg(playerid,COLOUR_OK,string);
			new logstring[256];
			format(logstring, sizeof (logstring), "player: %d:  %s: set price of '%s' (Energy Drink Vendor) to $%d",playerid,oGetPlayerName(playerid),DrinkVendors[id][Name],DrinkVendors[id][DrinkPrice]);
			WriteLog(logstring);
		}
	} else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(561));
	}
} else {
	SystemMsg(playerid,COLOUR_ERROR,gettext(759));
}
Debug("businesses/drinks.inc > SetDrinkPrice - Stop");
return 0;
}