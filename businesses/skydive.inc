//Skydive Business module.
//Created by MadCat
//Based on Original Business Handler from GTO, created by Iain Gilbert
//Idea from PEN1

#include "base"
#include "player"

forward Skydive_OnGameModeInit();
forward IsPlayerAtSkydivePlace(playerid);
forward SkydivePlacePickupCheck(playerid);
forward GetSkydivePlaceID(playerid);
forward DoSkydive(playerid);
forward SellSkydivePlace(playerid);
forward CollectSkydivePlace(playerid);
forward BuySkydivePlace(playerid);
forward SetSkydivePrice(playerid,value);
forward SkyDiveTimer();

new skydiveplaceinfoshowed[MAX_PLAYERS] = 0;
new PlayerSkydive[MAX_PLAYERS];

new skydivecount = 45;

new SkydiveBusinessDB[MAX_STRING] = "MultiGTA/Businesses/";

#define MAX_SKYDIVEPLACES 8

new Float:SkyPosX,Float:SkyPosY,Float:SkyPosZ;

new TimerSkyDive;

enum SkydivePlaceInfo {
    Name[MAX_STRING],
    Float:Coord_X,
    Float:Coord_Y,
    Float:Coord_Z,
    Level,
    Cost,
    SkydivePrice,
    MaxPrice,
    MoneyNow,
    ItemsInStock,
    Components,
    Owner[MAX_NAME]
}

new SkydivePlaces[MAX_SKYDIVEPLACES][SkydivePlaceInfo] = {
	{"SkyDive place in (LS)El Corona",1954.099975,-2177.699951,13.500000,4,100000,1000,3000,0,100,0,"Server"},
	{"SkyDive place in (SF)Easter Basin",-1545.400024,-439.899963,6.000000,4,100000,1000,3000,0,100,0,"Server"},
	{"SkyDive place in (LV)Las Venturas Airport",1714.908325,1615.993652,10.112895,4,100000,1000,3000,0,100,0,"Server"},
	{"SkyDive place in (LV)The Emerald Isle",2057.0,2434.0,166.0,4,100000,1000,3000,0,100,0,"Server"},
	{"SkyDive place in Mount Chilliad",-2350.0,-1586.0,485.0,4,100000,1000,3000,0,100,0,"Server"},
	{"SkyDive place in Big Ear",-225.6758,1394.256,172.0143,4,100000,1000,3000,0,100,0,"Server"},
	{"SkyDive place in Arco Del Oestre",-773.0379,2423.499,157.0856,4,100000,1000,3000,0,100,0,"Server"},
	{"SkyDive place in (SF)Ocean Flats",-2677.726,-192.3469,6.8518,4,100000,1000,3000,0,100,0,"Server"}
}; 

SkydivePlacesLoadAll()
{
	Debug("businesses/skydive.inc > SkydivePlacesLoadAll - Start");
	
	new temp[MAX_STRING];
	if (!db_Exists(DatabaseDB)) db_Create(DatabaseDB);
	if (!db_Isset(DatabaseDB,"Skydive_Business_DB")) db_Set(DatabaseDB,"Skydive_Business_DB",SkydiveBusinessDB);
	set(temp,db_Get(DatabaseDB,"Skydive_Business_DB"));
	if (strlen(temp) > 0) set(SkydiveBusinessDB,temp);
	if (!db_Exists(SkydiveBusinessDB))
	{

		SkydivePlacesSaveAll();

		return;
	}
	for (new i=0;i<MAX_SKYDIVEPLACES;i++)
	{
		new SkydiveBusinessDBName[MAX_STRING];
		format(SkydiveBusinessDBName,sizeof(SkydiveBusinessDBName),"%s%s.txt",SkydiveBusinessDB,SkydivePlaces[i][Name]);
		if (!db_Exists(SkydiveBusinessDBName))
		{
			SkydivePlacesSaveAll();
		}
		set(temp,db_Get(SkydiveBusinessDBName,"Name"));
		if (strlen(temp) > 0) set(SkydivePlaces[i][Name],temp);
		set(temp,nullstr);
		set(temp,db_Get(SkydiveBusinessDBName,"Level"));
		if (strlen(temp) > 0) SkydivePlaces[i][Level] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(SkydiveBusinessDBName,"Cost"));
		if (strlen(temp) > 0) SkydivePlaces[i][Cost] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(SkydiveBusinessDBName,"MoneyNow"));
		if (strlen(temp) > 0) SkydivePlaces[i][MoneyNow] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(SkydiveBusinessDBName,"ItemsInStock"));
		if (strlen(temp) > 0) SkydivePlaces[i][ItemsInStock] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(SkydiveBusinessDBName,"Components"));
		if (strlen(temp) > 0) SkydivePlaces[i][Components] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(SkydiveBusinessDBName,"SkydivePrice"));
		if (strlen(temp) > 0) SkydivePlaces[i][SkydivePrice] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(SkydiveBusinessDBName,"MaxPrice"));
		if (strlen(temp) > 0) SkydivePlaces[i][MaxPrice] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(SkydiveBusinessDBName,"Owner"));
		if (strlen(temp) > 0) set(SkydivePlaces[i][Owner],temp);
		set(temp,nullstr);
	}
	Debug("businesses/skydive.inc > SkydivePlacesLoadAll - Stop");
	return;

}

SkydivePlacesSaveAll()
{
	Debug("businesses/skydive.inc > SkydivePlacesSaveAll - Start");
	new temp[MAX_STRING];
	if (db_Exists(DatabaseDB))
	{
		db_Set(DatabaseDB,"Skydive_Business_DB",SkydiveBusinessDB);
	}

	for (new i=0;i<MAX_SKYDIVEPLACES;i++)
	{
		new SkydiveBusinessDBName[MAX_STRING];
		format(SkydiveBusinessDBName,sizeof(SkydiveBusinessDBName),"%s%s.txt",SkydiveBusinessDB,SkydivePlaces[i][Name]);
		if (db_Exists(SkydiveBusinessDBName)){
			continue;
		}
		db_Create(SkydiveBusinessDBName);
		db_BeforeBigSaving(SkydiveBusinessDBName);
		db_Set(SkydiveBusinessDBName,"Name",SkydivePlaces[i][Name]);
		valstr(temp,SkydivePlaces[i][Level]);
		db_Set(SkydiveBusinessDBName,"Level",temp);
		set(temp,nullstr);
		valstr(temp,SkydivePlaces[i][Cost]);
		db_Set(SkydiveBusinessDBName,"Cost",temp);
		set(temp,nullstr);
		valstr(temp,SkydivePlaces[i][MoneyNow]);
		db_Set(SkydiveBusinessDBName,"MoneyNow",temp);
		set(temp,nullstr);
		valstr(temp,SkydivePlaces[i][ItemsInStock]);
		db_Set(SkydiveBusinessDBName,"ItemsInStock",temp);
		set(temp,nullstr);
		valstr(temp,SkydivePlaces[i][Components]);
		db_Set(SkydiveBusinessDBName,"Components",temp);
		set(temp,nullstr);
		valstr(temp,SkydivePlaces[i][SkydivePrice]);
		db_Set(SkydiveBusinessDBName,"SkydivePrice",temp);
		set(temp,nullstr);
		valstr(temp,SkydivePlaces[i][MaxPrice]);
		db_Set(SkydiveBusinessDBName,"MaxPrice",temp);
		set(temp,nullstr);
		db_Set(SkydiveBusinessDBName,"Owner",SkydivePlaces[i][Owner]);
		db_AfterBigSaving(SkydiveBusinessDBName);

	}
	Debug("businesses/skydive.inc > SkydivePlacesSaveAll - Stop");
	return;
}

public Skydive_OnGameModeInit()
{
	Debug("businesses/skydive.inc > Skydive_OnGameModeInit - Start");
	new count = 0;
	for (new dbid=0;dbid<MAX_SKYDIVEPLACES;dbid++)
	{
		CreateStreamPickup(371,1,SkydivePlaces[dbid][Coord_X],SkydivePlaces[dbid][Coord_Y],SkydivePlaces[dbid][Coord_Z]);
		CreateStreamMapIcon(5,SkydivePlaces[dbid][Coord_X],SkydivePlaces[dbid][Coord_Y],SkydivePlaces[dbid][Coord_Z]);
		CreateDMapIcon(5,SkydivePlaces[dbid][Coord_X],SkydivePlaces[dbid][Coord_Y],SkydivePlaces[dbid][Coord_Z],gettext(1769),gettext(1770));
		count++;
	}
	TimerSkyDive = SetTimer("SkyDiveTimer", 1000, 1);
	SetInteriorWeapons(INTERIOR_JET,false);
	new logstring[256];
	format(logstring,sizeof(logstring),"Skydive system Loaded. Places: %d",count);
	WriteLog(logstring);
	Debug("businesses/skydive.inc > Skydive_OnGameModeInit - Stop");
}

public IsPlayerAtSkydivePlace(playerid)
{
	for (new dbid=0;dbid<MAX_SKYDIVEPLACES;dbid++)
	{
		if (IsPlayerInRangeOfPoint(playerid,1.0,SkydivePlaces[dbid][Coord_X],SkydivePlaces[dbid][Coord_Y],SkydivePlaces[dbid][Coord_Z]))
		{
			return 1;
		}
	}
	return 0;
}

public GetSkydivePlaceID(playerid)
{
	new lookupid=-1;
	for (new dbid=0;dbid<MAX_SKYDIVEPLACES;dbid++)
	{
		if (IsPlayerInRangeOfPoint(playerid,1.0,SkydivePlaces[dbid][Coord_X],SkydivePlaces[dbid][Coord_Y],SkydivePlaces[dbid][Coord_Z])) lookupid = dbid;
	}
	return lookupid;
}

public SkydivePlacePickupCheck(playerid)
{
	if (playerid == INVALID_PLAYER_ID) return;
	if (!IsPlayerConnected(playerid)) return;
	if (!IsPlayerAtSkydivePlace(playerid)) {skydiveplaceinfoshowed[playerid] = 0; return;}
	if (skydiveplaceinfoshowed[playerid] == 1) return;
	skydiveplaceinfoshowed[playerid] = 1;
	new id=GetSkydivePlaceID(playerid);
	SystemMsgFormat(playerid,COLOUR_INFO,gettext(871),SkydivePlaces[id][SkydivePrice]);
	if (strcomp(SkydivePlaces[id][Owner],oGetPlayerName(playerid),false) == 1){
		SystemMsg(playerid,COLOUR_INFO,gettext(872));
		if (SkydivePlaces[id][MoneyNow] > 0){
			SystemMsgFormat(playerid,COLOUR_INFO,gettext(873),SkydivePlaces[id][MoneyNow]);
		}
	}
	if (strcomp(SkydivePlaces[id][Owner],"Server",false) == 1){
		new string[MAX_STRING];
		format(string,sizeof(string),gettext(874),SkydivePlaces[id][Cost]+SkydivePlaces[id][MoneyNow],SkydivePlaces[id][Level]);
		SystemMsg(playerid,COLOUR_INFO,string);
	}
}

public DoSkydive(playerid){
		Debug("businesses/skydive.inc > DoSkydive - Start");
		new id=GetSkydivePlaceID(playerid);
		if (oGetPlayerMoney(playerid) < SkydivePlaces[id][SkydivePrice])
		{
			SystemMsg(playerid,COLOUR_ERROR,gettext(875));
			Debug("businesses/skydive.inc > DoSkydive - Stop");
			return 1;
		}
		if (SkydivePlaces[id][ItemsInStock] == 0){
			ShowTextForPlayer(playerid, gettext(712), 5000, 1);
			Debug("businesses/skydive.inc > DoSkydive - Stop");
			return 1;
		}
		new string[MAX_STRING];
		if (strcomp(SkydivePlaces[id][Owner],"Server",false) == 1){
		oGivePlayerMoney(playerid,-SkydivePlaces[id][SkydivePrice],0);
		SkydivePlaces[id][MoneyNow]=SkydivePlaces[id][MoneyNow]+SkydivePlaces[id][SkydivePrice];
		SkydivePlaces[id][ItemsInStock]--;
		new dinifilename[MAX_STRING];
		new temp[256];
		format(dinifilename,sizeof(dinifilename),"%s%s.txt",SkydiveBusinessDB,SkydivePlaces[id][Name]);
		if (db_Exists(dinifilename))
		{
			valstr(temp,SkydivePlaces[id][MoneyNow]);
			db_Set(dinifilename,"MoneyNow",temp);
			set(temp,nullstr);
			valstr(temp,SkydivePlaces[id][ItemsInStock]);
			db_Set(dinifilename,"ItemsInStock",temp);
			set(temp,nullstr);
		}
		SkyPosX = PlayerPos[playerid][Coord_X];
		SkyPosY = PlayerPos[playerid][Coord_Y];
		SkyPosZ = PlayerPos[playerid][Coord_Z];
		SetPlayerInterior(playerid,INTERIOR_JET);
		oSetPlayerPos(playerid,1.71875, 30.4062, 1200.34);
		format(string,sizeof(string),gettext(876));
		} else if (strcomp(SkydivePlaces[id][Owner],oGetPlayerName(playerid),false) == 1){
		SkyPosX = PlayerPos[playerid][Coord_X];
		SkyPosY = PlayerPos[playerid][Coord_Y];
		SkyPosZ = PlayerPos[playerid][Coord_Z];
		SetPlayerInterior(playerid,INTERIOR_JET);
		oSetPlayerPos(playerid,1.71875, 30.4062, 1200.34);
		SkydivePlaces[id][ItemsInStock]--;
		new dinifilename[MAX_STRING];
		new temp[256];
		format(dinifilename,sizeof(dinifilename),"%s%s.txt",SkydiveBusinessDB,SkydivePlaces[id][Name]);
		if (db_Exists(dinifilename))
		{
			valstr(temp,SkydivePlaces[id][ItemsInStock]);
			db_Set(dinifilename,"ItemsInStock",temp);
			set(temp,nullstr);
		}
		format(string,sizeof(string),gettext(876));
		}
		else
		{
		oGivePlayerMoney(playerid,-SkydivePlaces[id][SkydivePrice],0);
		SkydivePlaces[id][MoneyNow]=SkydivePlaces[id][MoneyNow]+SkydivePlaces[id][SkydivePrice];
		SkydivePlaces[id][ItemsInStock]--;
		new dinifilename[MAX_STRING];
		new temp[256];
		format(dinifilename,sizeof(dinifilename),"%s%s.txt",SkydiveBusinessDB,SkydivePlaces[id][Name]);
		if (db_Exists(dinifilename))
		{
			valstr(temp,SkydivePlaces[id][MoneyNow]);
			db_Set(dinifilename,"MoneyNow",temp);
			set(temp,nullstr);
			valstr(temp,SkydivePlaces[id][ItemsInStock]);
			db_Set(dinifilename,"ItemsInStock",temp);
			set(temp,nullstr);
		}
		SkyPosX = PlayerPos[playerid][Coord_X];
		SkyPosY = PlayerPos[playerid][Coord_Y];
		SkyPosZ = PlayerPos[playerid][Coord_Z];
		SetPlayerInterior(playerid,INTERIOR_JET);
		oSetPlayerPos(playerid,1.71875, 30.4062, 1200.34);
		format(string,sizeof(string),gettext(876));
		}
		SystemMsg(playerid,COLOUR_OK,string);
		PlayerSkydive[playerid] = 1;
		new logstring[256];
		format(logstring,sizeof(logstring), "player: %d:  %s: do skydive.",playerid,oGetPlayerName(playerid));
		WriteLog(logstring);
		Debug("businesses/skydive.inc > DoSkydive - Stop");
		return 1;
}

public SellSkydivePlace(playerid){
Debug("businesses/skydive.inc > SellSkydivePlace - Start");
new id=GetSkydivePlaceID(playerid);
new temp[MAX_STRING];
	if (id > -1)
	{
		if (IsPlayerRegistered(playerid))
		{
			if(strcomp(SkydivePlaces[id][Owner], oGetPlayerName(playerid), true) == 0)
			{
				SystemMsg(playerid,COLOUR_ERROR,gettext(877));
			} else {
				new price;
				price = (SkydivePlaces[id][Cost] + SkydivePlaces[id][MoneyNow])*85 / 100;
				oGivePlayerMoney(playerid,0+price,1);
				set(SkydivePlaces[id][Owner],"Server");
				new dinifilename[MAX_STRING];
				format(dinifilename,sizeof(dinifilename),"%s%s.txt",SkydiveBusinessDB,SkydivePlaces[id][Name]);
				if (db_Exists(dinifilename))
				{
					db_Set(dinifilename,"Owner","Server");
				}
				format(temp,sizeof(temp),gettext(878),SkydivePlaces[id][Name]);
				SystemMsg(playerid,COLOUR_OK,temp);
				set(temp,nullstr);
				new logstring[256];
				format(logstring, sizeof (logstring), "player: %d:  %s: sold the '%s' (Skydive Place)",playerid,oGetPlayerName(playerid),SkydivePlaces[id][Name]);
				WriteLog(logstring);
			}
		} else {
			SystemMsg(playerid,COLOUR_ERROR,gettext(561));
		}
	} else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(881));
	}
	Debug("businesses/skydive.inc > SellSkydivePlace - Stop");
	return 0;
}

public CollectSkydivePlace(playerid){
Debug("businesses/skydive.inc > CollectSkydivePlace - Start");
new id=GetSkydivePlaceID(playerid);
			new temp[MAX_STRING];
			if (id > -1)
			{
				if (IsPlayerRegistered(playerid))
				{
					if(strcomp(SkydivePlaces[id][Owner], oGetPlayerName(playerid), true) == 0)
					{
						SystemMsg(playerid,COLOUR_ERROR,gettext(877));
					} else {
						if(SkydivePlaces[id][MoneyNow] > 0)
						{
							oGivePlayerMoney(playerid,0+SkydivePlaces[id][MoneyNow],1);
							SystemMsg(playerid,COLOUR_OK,gettext(879));
							SkydivePlaces[id][MoneyNow] = 0;
							new dinifilename[MAX_STRING];
							format(dinifilename,sizeof(dinifilename),"%s%s.txt",SkydiveBusinessDB,SkydivePlaces[id][Name]);
							if (db_Exists(dinifilename))
							{
								valstr(temp,SkydivePlaces[id][MoneyNow]);
								db_Set(dinifilename,"MoneyNow",temp);
								set(temp,nullstr);
							}
						} else {
							SystemMsg(playerid,COLOUR_ERROR,gettext(880));
						}
					}
				} else {
					SystemMsg(playerid,COLOUR_ERROR,gettext(561));
				}
			} else {
				SystemMsg(playerid,COLOUR_ERROR,gettext(881));
			}
			Debug("businesses/skydive.inc > CollectSkydivePlace - Stop");
			return 0;
}

public BuySkydivePlace(playerid){
Debug("businesses/skydive.inc > BuySkydivePlace - Start");
			new id=GetSkydivePlaceID(playerid);
			new temp[MAX_STRING];
			if (id > -1)
			{
				if (IsPlayerRegistered(playerid))
				{
					if (Player[playerid][Level] >= SkydivePlaces[id][Level])
					{
						new price;
						price = SkydivePlaces[id][Cost] + SkydivePlaces[id][MoneyNow];
						if(Player[playerid][Money] >= price)
						{
							if(strcomp(SkydivePlaces[id][Owner], oGetPlayerName(playerid), true) == 1)
							{
								SystemMsg(playerid,COLOUR_ERROR,gettext(882));
							} else {

								oGivePlayerMoney(playerid,0-price,1);
								set(SkydivePlaces[id][Owner],oGetPlayerName(playerid));
								new dinifilename[MAX_STRING];
								format(dinifilename,sizeof(dinifilename),"%s%s.txt",SkydiveBusinessDB,SkydivePlaces[id][Name]);
								if (db_Exists(dinifilename))
								{
									db_Set(dinifilename,"Owner",SkydivePlaces[id][Owner]);
								}
								format(temp,sizeof(temp),gettext(883),SkydivePlaces[id][Name]);
								SystemMsg(playerid,COLOUR_OK,temp);
								SystemMsg(playerid,COLOUR_INFO,gettext(884));
								SystemMsg(playerid,COLOUR_INFO,gettext(885));
								SystemMsg(playerid,COLOUR_INFO,gettext(886));
								set(temp,nullstr);

								new logstring[256];
								format(logstring, sizeof (logstring), "player: %d:  %s: bought the '%s' (Skydive Place)",playerid,oGetPlayerName(playerid),SkydivePlaces[id][Name]);
								WriteLog(logstring);

							}
						} else {
							SystemMsg(playerid,COLOUR_ERROR,gettext(887));
						}
					} else {
						format(temp,sizeof(temp),gettext(888),SkydivePlaces[id][Level]);
						SystemMsg(playerid,COLOUR_ERROR,temp);
						set(temp,nullstr);
					}
				} else {
					SystemMsg(playerid,COLOUR_ERROR,gettext(561));
				}
			} else {
				SystemMsg(playerid,COLOUR_ERROR,gettext(881));
			}
			Debug("businesses/skydive.inc > BuySkydivePlace - Stop");
			return 0;
}

public SetSkydivePrice(playerid,value){
Debug("businesses/skydive.inc > SetSkydivePrice - Start");
new id=GetSkydivePlaceID(playerid);
if (value <= 0){
	SystemMsg(playerid,COLOUR_ERROR,gettext(889));
	Debug("businesses/skydive.inc > SetSkydivePrice - Stop");
	return 1;
}
if (id > -1)
{
	if (IsPlayerRegistered(playerid))
	{
		if(strcomp(SkydivePlaces[id][Owner], oGetPlayerName(playerid), true) == 0)
		{
			SystemMsg(playerid,COLOUR_ERROR,gettext(877));
		} else {
			if (value > SkydivePlaces[id][MaxPrice]){
				SystemMsgFormat(playerid,COLOUR_ERROR,gettext(890),SkydivePlaces[id][MaxPrice]);
				return 1;
			}
			new dinifilename[MAX_STRING];
			format(dinifilename,sizeof(dinifilename),"%s%s.txt",SkydiveBusinessDB,SkydivePlaces[id][Name]);
			if (db_Exists(dinifilename))
			{
				SkydivePlaces[id][SkydivePrice] = value;
				new temp[MAX_STRING];
				valstr(temp,SkydivePlaces[id][SkydivePrice]);
				db_Set(dinifilename,"SkydivePrice",temp);
				set(temp,nullstr);
			}
			new string[MAX_STRING];
			format(string,sizeof(string),gettext(891),SkydivePlaces[id][Name],SkydivePlaces[id][SkydivePrice]);
			SystemMsg(playerid,COLOUR_OK,string);
			new logstring[256];
			format(logstring, sizeof (logstring), "player: %d:  %s: set cost of '%s' (Skydive Place) to $%d",playerid,oGetPlayerName(playerid),SkydivePlaces[id][Name],SkydivePlaces[id][SkydivePrice]);
			WriteLog(logstring);
		}
	} else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(561));
	}
} else {
	SystemMsg(playerid,COLOUR_ERROR,gettext(881));
}
Debug("businesses/skydive.inc > SetSkydivePrice - Stop");
return 0;
}

public SkyDiveTimer()
{
	skydivecount--;
	new string[256];
	for(new i = 0; i <= MAX_PLAYERS_EX; i++)
	{
		if(IsPlayerConnected(i) == 1 && PlayerSkydive[i] == 1)
		{
			if(skydivecount > 10)
			{
				format(string, sizeof(string),gettext(892),skydivecount);
			}
			else
			{
				format(string, sizeof(string),gettext(893),skydivecount);
			}
			ShowTextForPlayer(i, string, 2000, 6);
			if(skydivecount == 0)
			{
				GivePlayerWeapon(i, 46, 0);
				oSetPlayerPos(i,SkyPosX-40+random(80),SkyPosY-40+random(80),SkyPosZ+1000);
				SetPlayerInterior(i,INTERIOR_DEFAULT);
				SystemMsg(i,COLOUR_OK,gettext(894));
				Player[i][SkyDives]++;
				PlayerSkydive[i] = 0;
			}
		}
	}
	if(skydivecount == 0)
	{
		skydivecount = 45;
	}
}