//Anti-SpoofKillCheat Modules
//Created by MadCat
//Ideas from SAMP DMA+LG+PB+SR 1.0.1 by BetaMaster, FcukIt 3.0

forward AntiSpoofKill_OnGameModeInit();
forward AntiSpoofKill_OnPlayerDeath(playerid, killerid, reason);

new Anti_Spoof_Kill_Enabled = 1;
new Action_On_Spoof_Cheat=2;
new Spoof_Kill_Ban_Hours=8760;

public AntiSpoofKill_OnGameModeInit(){
	AntiSpoofKillConfig();
	if (Anti_Spoof_Kill_Enabled == 1) WriteLog("Anti Spoof-Kill Loaded");
	return 0;
}

AntiSpoofKillConfig()
{
	Debug("protections/spoofkill.inc > AntiSpoofKillConfig - Start");
	new temp[MAX_STRING];
	if (!db_Exists(SecurityDB)) db_Create(SecurityDB);
	
	set(temp,db_Get(SecurityDB,"Anti_Spoof_Kill_Enabled"));
	if (strlen(temp) > 0) Anti_Spoof_Kill_Enabled = strval(temp);
	else { valstr(temp,Anti_Spoof_Kill_Enabled); db_Set(SecurityDB,"Anti_Spoof_Kill_Enabled",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Action_On_Spoof_Cheat"));
	if (strlen(temp) > 0) Action_On_Spoof_Cheat = strval(temp);
	else { valstr(temp,Action_On_Spoof_Cheat); db_Set(SecurityDB,"Action_On_Spoof_Cheat",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Spoof_Kill_Ban_Hours"));
	if (strlen(temp) > 0) Spoof_Kill_Ban_Hours = strval(temp);
	else { valstr(temp,Spoof_Kill_Ban_Hours); db_Set(SecurityDB,"Spoof_Kill_Ban_Hours",temp);  }
	set(temp,nullstr);

	Debug("protections/spoofkill.inc > AntiSpoofKillConfig - Stop");
	return;
}

public AntiSpoofKill_OnPlayerDeath(playerid, killerid, reason)
{
	if (Anti_Spoof_Kill_Enabled == 0) return 0;
	Debug("protections/spoofkill.inc > AntiSpoofKill_OnPlayerDeath - Start"); 
	if (!IsPlayerConnected(playerid)) {Debug("protections/spoofkill.inc > AntiSpoofKill_OnPlayerDeath - Stop"); return 0;}
	if (!IsPlayerConnected(killerid)) {Debug("protections/spoofkill.inc > AntiSpoofKill_OnPlayerDeath - Stop"); return 0;}
	if (IsPlayerAdmin(killerid) || PlayerPriv[killerid][IsModerator] == 1) {Debug("protections/spoofkill.inc > AntiSpoofKill_OnPlayerDeath - Stop"); return 0;}
	if (IsWeapon(reason)){
		if((IsWeaponHandToHand(reason) == 0) && ((oGetPlayerHealth(playerid) > 70.0 && reason != INVALID_PLAYER_ID) || (GetDistanceBetweenPlayers(playerid, killerid)>250.0 && reason!=WEAPON_FLAMETHROWER)))
		{ //Upper check - This is not hand weapon
			if (Action_On_Spoof_Cheat == 0){Debug("protections/spoofkill.inc > AntiSpoofKill_OnPlayerDeath - Stop"); return 0;}
			if (Action_On_Spoof_Cheat == 1){
				new string[MAX_STRING];
				format(string,sizeof(string),gettext(1305),oGetWeaponName(reason));
				Report(killerid,string);
			}
			if (Action_On_Spoof_Cheat == 2){
				new string[MAX_STRING];
				format(string,sizeof(string),gettext(1305),oGetWeaponName(reason));
				KickPlayer(killerid,-1,string);
			}
			if (Action_On_Spoof_Cheat == 3){ 
				new string[MAX_STRING];
				format(string,sizeof(string),gettext(1305),oGetWeaponName(reason));
				BanPlayer(killerid,Spoof_Kill_Ban_Hours,string);
			}
		}
	}
	Debug("protections/spoofkill.inc > AntiSpoofKill_OnPlayerDeath - Stop");
	return 0;
}

