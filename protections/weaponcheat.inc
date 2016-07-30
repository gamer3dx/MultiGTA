//Anti-WeaponCheat Modules
//Created by MadCat

forward AntiWeapon_OnGameModeInit();
forward AntiWeapon_OnPlayerDeath(playerid, killerid, reason);

new Anti_Weapon_Cheat_Enabled=1;
new Action_On_Weapon_Cheat=2;
new WeaponCheat_Ban_Hours=8760;

public AntiWeapon_OnGameModeInit(){
	AntiWeaponCheatConfig();
	if (Anti_Weapon_Cheat_Enabled == 1) WriteLog("Anti-Weapon-Cheat Loaded");
	return 0;
}

AntiWeaponCheatConfig()
{
	Debug("protections/weaponcheat.inc > AntiWeaponCheatConfig - Start");
	new temp[MAX_STRING];
	if (!db_Exists(SecurityDB)) db_Create(SecurityDB);
	
	set(temp,db_Get(SecurityDB,"Anti_Weapon_Cheat_Enabled"));
	if (strlen(temp) > 0) Anti_Weapon_Cheat_Enabled = strval(temp);
	else { valstr(temp,Anti_Weapon_Cheat_Enabled); db_Set(SecurityDB,"Anti_Weapon_Cheat_Enabled",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Action_On_Weapon_Cheat"));
	if (strlen(temp) > 0) Action_On_Weapon_Cheat = strval(temp);
	else { valstr(temp,Action_On_Weapon_Cheat); db_Set(SecurityDB,"Action_On_Weapon_Cheat",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"WeaponCheat_Ban_Hours"));
	if (strlen(temp) > 0) WeaponCheat_Ban_Hours = strval(temp);
	else { valstr(temp,WeaponCheat_Ban_Hours); db_Set(SecurityDB,"WeaponCheat_Ban_Hours",temp);  }
	set(temp,nullstr);

	Debug("protections/weaponcheat.inc > AntiWeaponCheatConfig - Stop");
	return;
}

public AntiWeapon_OnPlayerDeath(playerid, killerid, reason)
{
	if (Anti_Weapon_Cheat_Enabled == 0) return 0;
	Debug("protections/weaponcheat.inc > AntiWeapon_OnPlayerDeath - Start"); 
	if (!IsWeapon(reason)) {Debug("protections/weaponcheat.inc > AntiWeapon_OnPlayerDeath - Stop"); return 0;}
	if (!IsPlayerConnected(playerid)) {Debug("protections/weaponcheat.inc > AntiWeapon_OnPlayerDeath - Stop"); return 0;}
	if (!IsPlayerConnected(killerid)) {Debug("protections/weaponcheat.inc > AntiWeapon_OnPlayerDeath - Stop"); return 0;}
	if (IsPlayerAdmin(killerid) || PlayerPriv[killerid][IsModerator] == 1) {Debug("protections/weaponcheat.inc > AntiWeapon_OnPlayerDeath - Stop"); return 0;}
	if (IsPlayerInDuel(killerid)) {Debug("protections/weaponcheat.inc > AntiWeapon_OnPlayerDeath - Stop"); return 0;}
	if (gDerbyPlayerCar[killerid] != -1 && Derby_Give_Weapon == 1) {Debug("protections/weaponcheat.inc > AntiWeapon_OnPlayerDeath - Stop"); return 0;}
	new killermodel = GetVehicleModel(GetPlayerVehicleID(killerid));
	if ((IsPlayerInAnyVehicle(killerid)) && (killermodel == V_RUSTLER || killermodel == V_SEASPAR) && (reason == WEAPON_M4)) {Debug("protections/weaponcheat.inc > AntiWeapon_OnPlayerDeath - Stop"); return 0;}
	if (reason == WEAPON_FLAMETHROWER) {Debug("protections/weaponcheat.inc > AntiWeapon_OnPlayerDeath - Stop"); return 0;} // needed because SA-MP still incorrectly detects this weapon
	if (IsPlayerInAnyDM(killerid)) {
		new dmid = GetPlayerDM(playerid);
		if (!IsWeaponAllowedInDM(reason,dmid))
		{
			new punishreason[MAX_STRING];
			format(punishreason,MAX_STRING,gettext(1012),oGetWeaponName(reason),Deathmatch[dmid][dm_name]);
			if (Action_On_Weapon_Cheat == 0) return 0;
			if (Action_On_Weapon_Cheat == 1){
				Report(killerid,punishreason);
				return 1;
			}
			if (Action_On_Weapon_Cheat == 2){
				KickPlayer(killerid,-1,punishreason);
				return 1;
			}
			if (Action_On_Weapon_Cheat == 3){ 
				BanPlayer(killerid,WeaponCheat_Ban_Hours,punishreason);
				return 1;
			}
			return 1;
		}
	} else if (IsPlayerInAnyHay(killerid)){
		if (reason != 0){
			new punishreason[MAX_STRING];
			format(punishreason,sizeof(punishreason),gettext(1792),oGetWeaponName(reason));
			if (Action_On_Weapon_Cheat == 0) return 0;
			if (Action_On_Weapon_Cheat == 1){
				Report(killerid,punishreason);
				return 1;
			}
			if (Action_On_Weapon_Cheat == 2){
				KickPlayer(killerid,-1,punishreason);
				return 1;
			}
			if (Action_On_Weapon_Cheat == 3){ 
				BanPlayer(killerid,WeaponCheat_Ban_Hours,punishreason);
				return 1;
			}
		}
	} else {
		if (!IsPlayerAllowedWeapon(killerid,reason)){
			new punishreason[MAX_STRING];
			format(punishreason,sizeof(punishreason),gettext(514),oGetWeaponName(reason));
			if (Action_On_Weapon_Cheat == 0) return 0;
			if (Action_On_Weapon_Cheat == 1){
				Report(killerid,punishreason);
				return 1;
			}
			if (Action_On_Weapon_Cheat == 2){
				KickPlayer(killerid,-1,punishreason);
				return 1;
			}
			if (Action_On_Weapon_Cheat == 3){ 
				BanPlayer(killerid,WeaponCheat_Ban_Hours,punishreason);
				return 1;
			}
		}
	}
	Debug("protections/weaponcheat.inc > AntiWeapon_OnPlayerDeath - Stop");
	return 0;
}