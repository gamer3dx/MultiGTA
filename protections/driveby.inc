//Anti-DriveBy Module
//Created by Iain Gilbert
//Modder: MadCat

forward AntiDriveBy_OnGameModeInit();
forward AntiDriveBy_OnPlayerDeath(playerid, killerid, reason);

new Max_DriveBy=2;
new Anti_Driveby_Enabled=0;
new Action_On_Max_DriveBy=2;
new Money_For_DriveBy=10000;
new XP_For_DriveBy=1000;
new Driveby_Ban_Hours=24;

new DriveBy[MAX_PLAYERS];

public AntiDriveBy_OnGameModeInit(){
	AntiDrivebyConfig();
	if (Anti_Driveby_Enabled == 1) WriteLog("Anti Drive-By Loaded");
	return 0;
}

AntiDrivebyConfig()
{
	Debug("protections/driveby.inc > AntiDrivebyConfig - Start");
	new temp[MAX_STRING];
	if (!db_Exists(SecurityDB)) db_Create(SecurityDB);
	
	set(temp,db_Get(SecurityDB,"Anti_Driveby_Enabled"));
	if (strlen(temp) > 0) Anti_Driveby_Enabled = strval(temp);
	else { valstr(temp,Anti_Driveby_Enabled); db_Set(SecurityDB,"Anti_Driveby_Enabled",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Max_DriveBy"));
	if (strlen(temp) > 0) Max_DriveBy = strval(temp);
	else { valstr(temp,Max_DriveBy); db_Set(SecurityDB,"Max_DriveBy",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Money_For_DriveBy"));
	if (strlen(temp) > 0) Money_For_DriveBy = strval(temp);
	else { valstr(temp,Money_For_DriveBy); db_Set(SecurityDB,"Money_For_DriveBy",temp); }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"XP_For_DriveBy"));
	if (strlen(temp) > 0) XP_For_DriveBy = strval(temp);
	else { valstr(temp,XP_For_DriveBy); db_Set(SecurityDB,"XP_For_DriveBy",temp); }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Action_On_Max_DriveBy"));
	if (strlen(temp) > 0) Action_On_Max_DriveBy = strval(temp);
	else { valstr(temp,Action_On_Max_DriveBy); db_Set(SecurityDB,"Action_On_Max_DriveBy",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Driveby_Ban_Hours"));
	if (strlen(temp) > 0) Driveby_Ban_Hours = strval(temp);
	else { valstr(temp,Driveby_Ban_Hours); db_Set(SecurityDB,"Driveby_Ban_Hours",temp);  }
	set(temp,nullstr);

	Debug("protections/driveby.inc > AntiDrivebyConfig - Stop");
	return;
}

public AntiDriveBy_OnPlayerDeath(playerid, killerid, reason)
{
	if (Anti_Driveby_Enabled == 0) return 0;
	Debug("protections/driveby.inc > AntiDriveBy_OnPlayerDeath - Start");
	if (!IsPlayerConnected(playerid)) {Debug("protections/driveby.inc > AntiDriveBy_OnPlayerDeath - Stop"); return 0;}
	if (!IsPlayerConnected(killerid)) {Debug("protections/driveby.inc > AntiDriveBy_OnPlayerDeath - Stop"); return 0;}
	if (IsPlayerAdmin(killerid) || PlayerPriv[killerid][IsModerator] == 1) {Debug("protections/driveby.inc > AntiDriveBy_OnPlayerDeath - Stop"); return 0;}
	if (DriveBy[killerid] < 0) DriveBy[killerid] = 0;
	if(IsPlayerInAnyVehicle(killerid) && (!IsPlayerInAnyVehicle(playerid)) && (GetPlayerState(killerid) == PLAYER_STATE_DRIVER) && (reason != WEAPON_VEHICLE))
	{
		DriveBy[killerid]++;
		GivePlayerXP(killerid,-XP_For_DriveBy,0);
		oGivePlayerMoney(killerid,-Money_For_DriveBy,0);
		GivePlayerXP(playerid,XP_For_DriveBy,0);
		oGivePlayerMoney(playerid,Money_For_DriveBy,0);
  		if(DriveBy[killerid] < Max_DriveBy)
		{
    		SystemMsg(killerid,COLOUR_ERROR,gettext(500));
		}
		if(DriveBy[killerid] >= Max_DriveBy)
		{
			DriveBy[killerid]=0;
			if (Action_On_Max_DriveBy == 0) return 0;
			if (Action_On_Max_DriveBy == 1) Report(killerid,gettext(501));
			if (Action_On_Max_DriveBy == 2) KickPlayer(killerid,-1,gettext(501));
			if (Action_On_Max_DriveBy == 3) BanPlayer(killerid,Driveby_Ban_Hours,gettext(501));
		}
	}
	else
	{
		if(DriveBy[killerid] > 0)
		{
			DriveBy[killerid]--;
		}
	}
	Debug("protections/driveby.inc > AntiDriveBy_OnPlayerDeath - Stop");
	return 0;
}

