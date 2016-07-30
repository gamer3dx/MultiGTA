//Anti-SpawnKill Module
//Created by... Sorry, forgot
//Converted by: MadCat

#include "a_samp" 			
#include <core> 			
#include "base"
#include "player"

#define MIN_SPAWNTIME 10
new SpawnKill[MAX_PLAYERS];
new Anti_Spawnkill_Enabled=0;
new Max_Spawn_Kills=2;
new Money_For_Spawn_Kill=1000;
new XP_For_Spawn_Kill=350;
new Action_On_Max_Spawn_Kills=2;
new Spawnkills_Ban_Hours=24;

forward SpawnKill_OnGameModeInit();

forward AntiSpawnKill_OnPlayerDeath(playerid, killerid, reason);

public SpawnKill_OnGameModeInit(){
	AntiSpawnkillConfig();
	if (Anti_Spawnkill_Enabled == 1) WriteLog("Anti Spawn-Kill Loaded");
	return 0;
}

AntiSpawnkillConfig()
{
	Debug("protections/spawnkill.inc > AntiSpawnKillConfig - Start");
	new temp[MAX_STRING];
	
	if (!db_Exists(SecurityDB)) db_Create(SecurityDB);

	set(temp,db_Get(SecurityDB,"Anti_Spawnkill_Enabled"));
	if (strlen(temp) > 0) Anti_Spawnkill_Enabled = strval(temp);
	else { valstr(temp,Anti_Spawnkill_Enabled); db_Set(SecurityDB,"Anti_Spawnkill_Enabled",temp); }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Anti_Spawnkill_Max_Spawnkill"));
	if (strlen(temp) > 0) Max_Spawn_Kills = strval(temp);
	else { valstr(temp,Max_Spawn_Kills); db_Set(SecurityDB,"Anti_Spawnkill_Max_Spawnkill",temp); }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Money_For_Spawn_Kill"));
	if (strlen(temp) > 0) Money_For_Spawn_Kill = strval(temp);
	else { valstr(temp,Money_For_Spawn_Kill); db_Set(SecurityDB,"Money_For_Spawn_Kill",temp); }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"XP_For_Spawn_Kill"));
	if (strlen(temp) > 0) XP_For_Spawn_Kill = strval(temp);
	else { valstr(temp,XP_For_Spawn_Kill); db_Set(SecurityDB,"XP_For_Spawn_Kill",temp); }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Action_On_Max_Spawn_Kills"));
	if (strlen(temp) > 0) Action_On_Max_Spawn_Kills = strval(temp);
	else { valstr(temp,Action_On_Max_Spawn_Kills); db_Set(SecurityDB,"Action_On_Max_Spawn_Kills",temp); }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Spawnkills_Ban_Hours"));
	if (strlen(temp) > 0) Spawnkills_Ban_Hours = strval(temp);
	else { valstr(temp,Spawnkills_Ban_Hours); db_Set(SecurityDB,"Spawnkills_Ban_Hours",temp); }
	set(temp,nullstr);

	Debug("protections/spawnkill.inc > AntiSpawnKillConfig - Stop");
	return;
}

// anti spawn kill
public AntiSpawnKill_OnPlayerDeath(playerid, killerid, reason)
{
	if (Anti_Spawnkill_Enabled == 0) return 0;
	Debug("protections/spawnkill.inc > AntiSpawnKill_OnPlayerDeath - Start");
	if (!IsPlayerConnected(playerid)) {Debug("protections/spawnkill.inc > AntiSpawnKill_OnPlayerDeath - Stop"); return 0;}
	if (!IsPlayerConnected(killerid)) {Debug("protections/spawnkill.inc > AntiSpawnKill_OnPlayerDeath - Stop"); return 0;}
	if (IsPlayerAdmin(killerid) || PlayerPriv[killerid][IsModerator] == 1) {Debug("protections/spawnkill.inc > AntiSpawnKill_OnPlayerDeath - Stop"); return 0;}
	if (SpawnKill[playerid] > 0) SpawnKill[playerid] = 0;

	if (GetPlayerDM(playerid) != INVALID_DM_ID) {Debug("protections/spawnkill.inc > AntiSpawnKill_OnPlayerDeath - Stop"); return 0;}

	if(SpawnTime[playerid] < MIN_SPAWNTIME)
	{
		SpawnKill[killerid]++;
		GivePlayerXP(killerid,-XP_For_Spawn_Kill,0);
		oGivePlayerMoney(killerid,-Money_For_Spawn_Kill,0);
		GivePlayerXP(playerid,XP_For_Spawn_Kill,0);
		oGivePlayerMoney(playerid,Money_For_Spawn_Kill,0);
		Player[killerid][SpawnKills]++;
		Player[playerid][SpawnDeaths]++;
  		if((SpawnKill[killerid] < Max_Spawn_Kills) && (SpawnKill[killerid] > 0))
		{
    		SystemMsg(killerid,COLOUR_ERROR,gettext(511));
		}
		if(SpawnKill[killerid] >= Max_Spawn_Kills)
		{
			SpawnKill[killerid]=0;
			if (Action_On_Max_Spawn_Kills == 0) {Debug("protections/spawnkill.inc > AntiSpawnKill_OnPlayerDeath - Stop"); return 0;}
			if (Action_On_Max_Spawn_Kills == 1) Report(killerid,gettext(512));
			if (Action_On_Max_Spawn_Kills == 2) KickPlayer(killerid,-1,gettext(512));
			if (Action_On_Max_Spawn_Kills == 3) BanPlayer(killerid,Spawnkills_Ban_Hours,gettext(512));
		}
	}
	else
	{
		if(SpawnKill[killerid] > 0)
		{
			SpawnKill[killerid]--;
		}
	}
	Debug("protections/spawnkill.inc > AntiSpawnKill_OnPlayerDeath - Stop");
	return 0;
}

