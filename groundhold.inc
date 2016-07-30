//
// Created:     	13.09.06
// Aurthor:    		Iain Gilbert
// Modified by:		MadCat
//

#if defined _groundhold_included
#endinput
#endif

#define _groundhold_included
#pragma library groundhold
#include "base"
#include "player"
#include "world"

new Groundhold_Enabled=0;
new Show_Groundhold_Zones_On_Map=0;

#define GROUNDHOLD_DELAY 5000 // how often in seconds players are paid

enum gh_info {
	gh_name[MAX_NAME], // name of ground
	gh_money, // base amount of money recieved for holding this ground
	gh_xp, // base amount of xp recieved for holding this ground
	gh_multiplyer, // max multiplyer of base
	gh_distance, // distance from checkpoint player can be to still receive pay
	gh_maxdistance,
	Float:gh_x,
	Float:gh_y,
	Float:gh_z
}

#define GROUNDHOLD_SIZE 4
new PlayerGroundHoldTime[GROUNDHOLD_SIZE][MAX_PLAYERS];
new PlayerHoldingGround[GROUNDHOLD_SIZE][MAX_PLAYERS];
new groundholdZones[GROUNDHOLD_SIZE][gh_info] = {
	{"Mount Chilliad",0,10,14,25,50,-2231.3235,-1739.4625,481.4385},
	{"Containers",10,0,8,10,20,-558.1875,-541.8652,25.5234},
	{"Catalina House",5,5,5,20,30,870.1729,-25.5642,63.9384},
	{"Ship",0,10,8,10,20,-1534.5357,112.4920,17.0078}
};

new GroundholdDB[MAX_STRING] = "MultiGTA/Groundhold/";

new TimerGroundhold;

forward Groundhold_OnGameModeInit();
forward CheckGround(ghid);
forward CountPlayers(players[]);
forward GroundholdTimer();

GroundholdZonesLoadAll()
{
	Debug("groundhold.inc > GroundholdZonesLoadAll - Start");
	
	new temp[MAX_STRING];
	if (!db_Exists(DatabaseDB)) db_Create(DatabaseDB);
	if (!db_Isset(DatabaseDB,"Groundhold_DB")) db_Set(DatabaseDB,"Groundhold_DB",GroundholdDB);
	set(temp,db_Get(DatabaseDB,"Groundhold_DB"));
	if (strlen(temp) > 0) set(GroundholdDB,temp);
	if (!db_Exists(GroundholdDB))
	{

		GroundHoldZonesSaveAll();

		return;
	}
	for (new i=0;i<GROUNDHOLD_SIZE;i++)
	{
		new GroundholdDBName[MAX_STRING];
		format(GroundholdDBName,sizeof(GroundholdDBName),"%s%s.txt",GroundholdDB,groundholdZones[i][gh_name]);
		if (!db_Exists(GroundholdDBName))
		{
			GroundHoldZonesSaveAll();
		}
		set(temp,db_Get(GroundholdDBName,"Name"));
		if (strlen(temp) > 0) set(groundholdZones[i][gh_name],temp);
		set(temp,nullstr);
		set(temp,db_Get(GroundholdDBName,"Money"));
		if (strlen(temp) > 0) groundholdZones[i][gh_money] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(GroundholdDBName,"XP"));
		if (strlen(temp) > 0) groundholdZones[i][gh_xp] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(GroundholdDBName,"Multiplyer"));
		if (strlen(temp) > 0) groundholdZones[i][gh_multiplyer] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(GroundholdDBName,"Distance"));
		if (strlen(temp) > 0) groundholdZones[i][gh_distance] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(GroundholdDBName,"MaxDistance"));
		if (strlen(temp) > 0) groundholdZones[i][gh_maxdistance] = strval(temp);
		set(temp,nullstr);
		if (Show_Groundhold_Zones_On_Map){
			CreateStreamMapIcon(25,groundholdZones[i][gh_x],groundholdZones[i][gh_y],groundholdZones[i][gh_z]);
			CreateDMapIcon(25,groundholdZones[i][gh_x],groundholdZones[i][gh_y],groundholdZones[i][gh_z],gettext(1783),gettext(1784));
		}
	}
	Debug("groundhold.inc > GroundholdZonesLoadAll - Stop");
	return;
}

GroundHoldZonesSaveAll()
{
	Debug("groundhold.inc > GroundHoldZonesSaveAll - Start");
	new temp[MAX_STRING];
	if (db_Exists(DatabaseDB))
	{
		db_Set(DatabaseDB,"Groundhold_DB",GroundholdDB);
	}

	for (new i=0;i<GROUNDHOLD_SIZE;i++)
	{
		new GroundholdDBName[MAX_STRING];
		format(GroundholdDBName,sizeof(GroundholdDBName),"%s%s.txt",GroundholdDB,groundholdZones[i][gh_name]);
		if (db_Exists(GroundholdDBName)) {
			continue;
		}
		db_Create(GroundholdDBName);
		db_BeforeBigSaving(GroundholdDBName);
		db_Set(GroundholdDBName,"Name",groundholdZones[i][gh_name]);
		valstr(temp,groundholdZones[i][gh_money]);
		db_Set(GroundholdDBName,"Money",temp);
		set(temp,nullstr);
		valstr(temp,groundholdZones[i][gh_xp]);
		db_Set(GroundholdDBName,"XP",temp);
		set(temp,nullstr);
		valstr(temp,groundholdZones[i][gh_multiplyer]);
		db_Set(GroundholdDBName,"Multiplyer",temp);
		set(temp,nullstr);
		valstr(temp,groundholdZones[i][gh_distance]);
		db_Set(GroundholdDBName,"Distance",temp);
		set(temp,nullstr);
		valstr(temp,groundholdZones[i][gh_maxdistance]);
		db_Set(GroundholdDBName,"MaxDistance",temp);
		db_AfterBigSaving(GroundholdDBName);
		set(temp,nullstr);
	}
	Debug("groundhold.inc > GroundHoldZonesSaveAll - Stop");
	return;
}

GroundholdConfig()
{
	Debug("groundhold.inc > GroundholdConfig - Start");
	new temp[MAX_STRING];
	if (!db_Exists(ConfigDB)) db_Create(ConfigDB);
	
	set(temp,db_Get(ConfigDB,"Groundhold_Enabled"));
	if (strlen(temp) > 0) Groundhold_Enabled = strval(temp);
	else { valstr(temp,Groundhold_Enabled); db_Set(ConfigDB,"Groundhold_Enabled",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(ConfigDB,"Show_Groundhold_Zones_On_Map"));
	if (strlen(temp) > 0) Show_Groundhold_Zones_On_Map = strval(temp);
	else { valstr(temp,Show_Groundhold_Zones_On_Map); db_Set(ConfigDB,"Show_Groundhold_Zones_On_Map",temp);  }
	set(temp,nullstr);

	Debug("groundhold.inc > GroundholdConfig - Stop");
	return;
}

public Groundhold_OnGameModeInit()
{
	GroundholdConfig();
	GroundholdZonesLoadAll();
	if (Groundhold_Enabled == 0) return 0;
	Debug("groundhold.inc > Groundhold_OnGameModeInit - Start");
	for (new i=0;i<GROUNDHOLD_SIZE;i++)
	{
		AddActiveCheckpoint(groundholdZones[i][gh_x],groundholdZones[i][gh_y],groundholdZones[i][gh_z],groundholdZones[i][gh_maxdistance],2);
	}
	TimerGroundhold = SetTimer("GroundholdTimer", GROUNDHOLD_DELAY, 1);
	WriteLog("Groundhold System Loaded");
	Debug("groundhold.inc > Groundhold_OnGameModeInit - Stop");
	return 0;
}

public CheckGround(ghid)
{
	Debug("groundhold.inc > CheckGround - Start");
	new playergroundcount;
	playergroundcount = CountPlayers(PlayerGroundHoldTime[ghid]);

	for (new playerid=0;playerid<MAX_PLAYERS_EX;playerid++)
	{
		if (IsPlayerConnected(playerid))
		{
			
			if ((oIsPlayerInCheckpoint(playerid,groundholdZones[ghid][gh_x],groundholdZones[ghid][gh_y],groundholdZones[ghid][gh_z],groundholdZones[ghid][gh_distance]))
					|| (GetDistancePlayertoXYZ(playerid,groundholdZones[ghid][gh_x],groundholdZones[ghid][gh_y],groundholdZones[ghid][gh_z]) < groundholdZones[ghid][gh_distance]))
			{

				new groundenemycount;
				for (new i = 0;i<MAX_PLAYERS_EX;i++)
				{
					if (i == playerid) continue;
					if (i == INVALID_PLAYER_ID) continue;
					if (PlayerGroundHoldTime[ghid][i] > 0)
					{
						if ((PlayerGangid[i] == 0) || (PlayerGangid[i] != PlayerGangid[playerid]))
						{
							groundenemycount++;
						}
					}
				}

				if (PlayerGroundHoldTime[ghid][playerid] < groundholdZones[ghid][gh_multiplyer])
				{
					if ((PlayerGroundHoldTime[ghid][playerid] == 0) && (PlayerHoldingGround[ghid][playerid] == 0))
					{
						new string[MAX_STRING];
						format(string,sizeof(string),gettext(1319),groundholdZones[ghid][gh_name]);
						SystemMsg(playerid,COLOUR_INFO,string);
						new maxmoney=(groundholdZones[ghid][gh_money]*groundholdZones[ghid][gh_multiplyer]);
						new maxxp=(groundholdZones[ghid][gh_xp]*groundholdZones[ghid][gh_multiplyer]);
						format(string,sizeof(string),gettext(1320),maxmoney,maxxp,(GROUNDHOLD_DELAY/1000));
						SystemMsg(playerid,COLOUR_INFO,string);
					}
					if (PlayerGroundHoldTime[ghid][playerid] == 0)
					{
						PlayerGroundHoldTime[ghid][playerid]++;
					}
					else if (groundenemycount == 0)
					{
						PlayerGroundHoldTime[ghid][playerid]++;
					}
				}
				PlayerHoldingGround[ghid][playerid] = 1;
				
				if (PlayerGroundHoldTime[ghid][playerid] > 0)
				{
					if (groundenemycount > 0) // if more that 0 enimies on ground
					{

						if (PlayerGroundHoldTime[ghid][playerid] > 2) // if we have been holding this ground
						{
							new string[MAX_STRING];
							format(string,sizeof(string),gettext(1321),groundholdZones[ghid][gh_name]);
							SystemMsg(playerid,COLOUR_ERROR,string);
						}
						if (PlayerGroundHoldTime[ghid][playerid] > 1)
						{
							PlayerGroundHoldTime[ghid][playerid]--;
						}
					}

					new moneyreward;
					new xpreward;
					xpreward = (groundholdZones[ghid][gh_xp] * PlayerGroundHoldTime[ghid][playerid]);
					if (playergroundcount <= 5)
					{
						moneyreward = (groundholdZones[ghid][gh_money] * PlayerGroundHoldTime[ghid][playerid]) / (1+(playergroundcount / 5));
					}
					else
					{
						moneyreward = (groundholdZones[ghid][gh_money] * PlayerGroundHoldTime[ghid][playerid]) / (1+(5 / 5));
					}

					if (moneyreward > 0) oGivePlayerMoney(playerid,moneyreward,0);
					if (xpreward > 0) GivePlayerXP(playerid,xpreward,0);
				}
				
			}
			else 
			{
				if (PlayerGroundHoldTime[ghid][playerid] > 0)
				{
					if (PlayerHoldingGround[ghid][playerid] == 1)
					{
						new string[MAX_STRING];
						format(string,sizeof(string),gettext(1322));
						SystemMsg(playerid,COLOUR_INFO,string);
					}
					
					if (PlayerGroundHoldTime[ghid][playerid] == 1)
					{
						new string[MAX_STRING];
						format(string,sizeof(string),gettext(1323),groundholdZones[ghid][gh_name]);
						SystemMsg(playerid,COLOUR_ERROR,string);
					}
					PlayerGroundHoldTime[ghid][playerid]--;
					if (PlayerGroundHoldTime[ghid][playerid] > 1)
					{
						PlayerGroundHoldTime[ghid][playerid]--;
					}
					if (PlayerGroundHoldTime[ghid][playerid] > 1)
					{
						PlayerGroundHoldTime[ghid][playerid]--;
					}
				}
				PlayerHoldingGround[ghid][playerid] = 0;
			}

		}
		else
		{
			PlayerGroundHoldTime[ghid][playerid] = 0;
			PlayerHoldingGround[ghid][playerid] = 0;
		}
	}
	Debug("groundhold.inc > CheckGround - Stop");
	return;
}

public CountPlayers(players[])
{
	Debug("groundhold.inc > CountPlayers - Start");
	new count;
	for (new i = 0;i<MAX_PLAYERS_EX;i++)
	{
		if (players[i] > 0) count++;
	}
	Debug("groundhold.inc > CountPlayers - Stop");
	return count;

}

public GroundholdTimer()
{
	Debug("groundhold.inc > GroundholdTimer - Start");
	for (new ghid = 0;ghid<GROUNDHOLD_SIZE;ghid++)
	{
		CheckGround(ghid);
	}
	Debug("groundhold.inc > GroundholdTimer - Stop");
}

