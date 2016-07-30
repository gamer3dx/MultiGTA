//Anti-CarKill Module
//Created by MadCat
//Based on DriveBy Module

forward AntiCarKill_OnGameModeInit();
forward AntiCarKill_OnPlayerDeath(playerid, killerid, reason);

new Max_CarKills=2;
new Anti_Carkill_Enabled=0;
new Action_On_Max_CarKills=2;
new Money_For_CarKill=10000;
new XP_For_CarKill=1000;
new CarKill_Ban_Hours=24;

new CarKill[MAX_PLAYERS];

public AntiCarKill_OnGameModeInit(){
	AntiCarKillConfig();
	if (Anti_Carkill_Enabled == 1) WriteLog("Anti Car-Kill Loaded");
	return 0;
}

AntiCarKillConfig()
{
	Debug("protections/carkill.inc > AntiCarKillConfig - Start");
	new temp[MAX_STRING];
	if (!db_Exists(SecurityDB)) db_Create(SecurityDB);
	
	set(temp,db_Get(SecurityDB,"Anti_Carkill_Enabled"));
	if (strlen(temp) > 0) Anti_Carkill_Enabled = strval(temp);
	else { valstr(temp,Anti_Carkill_Enabled); db_Set(SecurityDB,"Anti_Carkill_Enabled",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Max_CarKills"));
	if (strlen(temp) > 0) Max_CarKills = strval(temp);
	else { valstr(temp,Max_CarKills); db_Set(SecurityDB,"Max_CarKills",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Money_For_CarKill"));
	if (strlen(temp) > 0) Money_For_CarKill = strval(temp);
	else { valstr(temp,Money_For_CarKill); db_Set(SecurityDB,"Money_For_CarKill",temp); }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"XP_For_CarKill"));
	if (strlen(temp) > 0) XP_For_CarKill = strval(temp);
	else { valstr(temp,XP_For_CarKill); db_Set(SecurityDB,"XP_For_CarKill",temp); }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Action_On_Max_CarKills"));
	if (strlen(temp) > 0) Action_On_Max_CarKills = strval(temp);
	else { valstr(temp,Action_On_Max_CarKills); db_Set(SecurityDB,"Action_On_Max_CarKills",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"CarKill_Ban_Hours"));
	if (strlen(temp) > 0) CarKill_Ban_Hours = strval(temp);
	else { valstr(temp,CarKill_Ban_Hours); db_Set(SecurityDB,"CarKill_Ban_Hours",temp);  }
	set(temp,nullstr);

	Debug("protections/carkill.inc > AntiCarKillConfig - Stop");
	return;
}

public AntiCarKill_OnPlayerDeath(playerid, killerid, reason)
{
	if (Anti_Carkill_Enabled == 0) return 0;
	Debug("protections/carkill.inc > AntiCarKill_OnPlayerDeath - Start");
	if (!IsPlayerConnected(playerid)) {Debug("protections/carkill.inc > AntiCarKill_OnPlayerDeath - Stop"); return 0;}
	if (!IsPlayerConnected(killerid)) {Debug("protections/carkill.inc > AntiCarKill_OnPlayerDeath - Stop"); return 0;}
	if (IsPlayerAdmin(killerid) || PlayerPriv[killerid][IsModerator] == 1) {Debug("protections/.inc > _OnPlayerDeath - Stop"); return 0;}
	if (CarKill[killerid] < 0) CarKill[killerid] = 0;
	if(IsPlayerInAnyVehicle(killerid) && (!IsPlayerInAnyVehicle(playerid)) && (reason == WEAPON_VEHICLE))
	{
		CarKill[killerid]++;
		GivePlayerXP(killerid,-XP_For_CarKill,0);
		oGivePlayerMoney(killerid,-Money_For_CarKill,0);
		GivePlayerXP(playerid,XP_For_CarKill,0);
		oGivePlayerMoney(playerid,Money_For_CarKill,0);
  		if(CarKill[killerid] < Max_CarKills)
		{
    		SystemMsg(killerid,COLOUR_ERROR,gettext(969));
		}
		if(CarKill[killerid] >= Max_CarKills)
		{
			CarKill[killerid]=0;
			if (Action_On_Max_CarKills == 0) return 0;
			if (Action_On_Max_CarKills == 1) Report(killerid,gettext(970));
			if (Action_On_Max_CarKills == 2) KickPlayer(killerid,-1,gettext(970));
			if (Action_On_Max_CarKills == 3) BanPlayer(killerid,CarKill_Ban_Hours,gettext(970));
		}
	}
	else
	{
		if(CarKill[killerid] > 0)
		{
			CarKill[killerid]--;
		}
	}
	Debug("protections/carkill.inc > AntiCarKill_OnPlayerDeath - Stop");
	return 0;
}

