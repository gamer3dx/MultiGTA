//Anti-HeliKill Module
//Created by Iain Gilbert
//Modder: MadCat

forward AntiHeliKill_OnGameModeInit();
forward AntiHeliKill_OnPlayerDeath(playerid, killerid, reason);

new Anti_Helikill_Enabled = 1;
new Action_On_HeliKills=2;
new HeliKill_Ban_Hours=24;


public AntiHeliKill_OnGameModeInit(){
	AntiHeliKillConfig();
	if (Anti_Helikill_Enabled == 1)	WriteLog("Anti Heli-Kill Loaded");
	return 0;
}

AntiHeliKillConfig()
{
	Debug("protections/helikill.inc > AntiHeliKillConfig - Start");
	new temp[MAX_STRING];
	if (!db_Exists(SecurityDB)) db_Create(SecurityDB);
	
	set(temp,db_Get(SecurityDB,"Anti_Helikill_Enabled"));
	if (strlen(temp) > 0) Anti_Helikill_Enabled = strval(temp);
	else { valstr(temp,Anti_Helikill_Enabled); db_Set(SecurityDB,"Anti_Helikill_Enabled",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Action_On_HeliKills"));
	if (strlen(temp) > 0) Action_On_HeliKills = strval(temp);
	else { valstr(temp,Action_On_HeliKills); db_Set(SecurityDB,"Action_On_HeliKills",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"HeliKill_Ban_Hours"));
	if (strlen(temp) > 0) HeliKill_Ban_Hours = strval(temp);
	else { valstr(temp,HeliKill_Ban_Hours); db_Set(SecurityDB,"HeliKill_Ban_Hours",temp);  }
	set(temp,nullstr);

	Debug("protections/helikill.inc > AntiHeliKillConfig - Stop");
	return;
}

public AntiHeliKill_OnPlayerDeath(playerid, killerid, reason)
{
	if (Anti_Helikill_Enabled == 0) return 0;
	Debug("protections/helikill.inc > AntiHeliKill_OnPlayerDeath - Start");
	if (!IsPlayerConnected(playerid)) {Debug("protections/helikill.inc > AntiHeliKill_OnPlayerDeath - Stop"); return 0;}
	if (!IsPlayerConnected(killerid)) {Debug("protections/helikill.inc > AntiHeliKill_OnPlayerDeath - Stop"); return 0;}
	if (IsPlayerAdmin(killerid) || PlayerPriv[killerid][IsModerator] == 1) {Debug("protections/helikill.inc > AntiHeliKill_OnPlayerDeath - Stop"); return 0;}
	if(IsPlayerInAnyVehicle(killerid) && (!IsPlayerInAnyVehicle(playerid)) && (GetPlayerState(killerid) == PLAYER_STATE_DRIVER) && (reason == 50))
	{
		if (Action_On_HeliKills == 0) return 0;
		if (Action_On_HeliKills == 1) Report(killerid,gettext(178));
		if (Action_On_HeliKills == 2) KickPlayer(killerid,-1,gettext(178));
		if (Action_On_HeliKills == 3) BanPlayer(killerid,HeliKill_Ban_Hours,gettext(178));
	}
	Debug("protections/helikill.inc > AntiHeliKill_OnPlayerDeath - Stop");
	return 0;
}

