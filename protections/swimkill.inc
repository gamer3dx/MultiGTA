//Anti-Swim kill Module
//Created by MadCat

forward AntiSwimKill_OnGameModeInit();
forward AntiSwimKill_OnPlayerDeath(playerid, killerid, reason);

new Anti_SwimKill_Enabled=0;
new Action_On_SwimKill=2;
new SwimKill_Ban_Hours=24;

public AntiSwimKill_OnGameModeInit(){
	AntiSwimKillConfig();
	if (Anti_SwimKill_Enabled == 1) WriteLog("Anti Floater-kill Loaded");
	return 0;
}

AntiSwimKillConfig()
{
	Debug("protections/swimkill.inc > AntiSwimKillConfig - Start");
	new temp[MAX_STRING];
	if (!db_Exists(SecurityDB)) db_Create(SecurityDB);
	
	set(temp,db_Get(SecurityDB,"Anti_SwimKill_Enabled"));
	if (strlen(temp) > 0) Anti_SwimKill_Enabled = strval(temp);
	else { valstr(temp,Anti_SwimKill_Enabled); db_Set(SecurityDB,"Anti_SwimKill_Enabled",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Action_On_SwimKill"));
	if (strlen(temp) > 0) Action_On_SwimKill = strval(temp);
	else { valstr(temp,Action_On_SwimKill); db_Set(SecurityDB,"Action_On_SwimKill",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"SwimKill_Ban_Hours"));
	if (strlen(temp) > 0) SwimKill_Ban_Hours = strval(temp);
	else { valstr(temp,SwimKill_Ban_Hours); db_Set(SecurityDB,"SwimKill_Ban_Hours",temp);  }
	set(temp,nullstr);

	Debug("protections/swimkill.inc > AntiSwimKillConfig - Stop");
	return;
}

public AntiSwimKill_OnPlayerDeath(playerid, killerid, reason)
{
	if (Anti_SwimKill_Enabled == 0) return 0;
	Debug("protections/swimkill.inc > AntiSwimKill_OnPlayerDeath - Start");
	if (!IsPlayerConnected(playerid)) {Debug("protections/swimkill.inc > AntiSwimKill_OnPlayerDeath - Stop"); return 0;}
	if (!IsPlayerConnected(killerid)) {Debug("protections/swimkill.inc > AntiSwimKill_OnPlayerDeath - Stop"); return 0;}
	if (IsPlayerAdmin(killerid) || PlayerPriv[killerid][IsModerator] == 1) {Debug("protections/swimkill.inc > AntiSwimKill_OnPlayerDeath - Stop"); return 0;}
	if (IsWeapon(reason) && !IsPlayerInAnyDM(playerid)){
		if(PlayerPos[playerid][Coord_Z] < 0.0)
		{
			if (Action_On_SwimKill == 0) return 0;
			if (Action_On_SwimKill == 1) Report(killerid,gettext(1740));
			if (Action_On_SwimKill == 2) KickPlayer(killerid,-1,gettext(1740));
			if (Action_On_SwimKill == 3) BanPlayer(killerid,SwimKill_Ban_Hours,gettext(1740));
		}
	}
	Debug("protections/swimkill.inc > AntiSwimKill_OnPlayerDeath - Stop");
	return 0;
}

