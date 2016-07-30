//Anti-Freeze Cheats Module
//Ripped from LACY anticheat (by LucifeR)

forward AntiFreeze_OnGameModeInit();
forward AntiFreeze_OnPlayerSpawn(playerid);

new Anti_Freeze_Cheat_Enabled=0;
new Action_On_Freeze=1;
new Freeze_Ban_Hours=720;

new Float:FreezePos[MAX_PLAYERS][CoordInfo];

forward CheckPlayerForAntiFreeze(playerid, num);

public AntiFreeze_OnGameModeInit(){
	AntiFreezeConfig();
	if (Anti_Freeze_Cheat_Enabled == 1) WriteLog("Anti-Freeze Loaded");
	return 0;
}

AntiFreezeConfig()
{
	Debug("protections/freeze.inc > AntiFreezeConfig - Start");
	new temp[MAX_STRING];
	if (!db_Exists(SecurityDB)) db_Create(SecurityDB);
	
	set(temp,db_Get(SecurityDB,"Anti_Freeze_Cheat_Enabled"));
	if (strlen(temp) > 0) Anti_Freeze_Cheat_Enabled = strval(temp);
	else { valstr(temp,Anti_Freeze_Cheat_Enabled); db_Set(SecurityDB,"Anti_Freeze_Cheat_Enabled",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Action_On_Freeze"));
	if (strlen(temp) > 0) Action_On_Freeze = strval(temp);
	else { valstr(temp,Action_On_Freeze); db_Set(SecurityDB,"Action_On_Freeze",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Freeze_Ban_Hours"));
	if (strlen(temp) > 0) Freeze_Ban_Hours = strval(temp);
	else { valstr(temp,Freeze_Ban_Hours); db_Set(SecurityDB,"Freeze_Ban_Hours",temp);  }
	set(temp,nullstr);

	Debug("protections/freeze.inc > AntiFreezeConfig - Stop");
	return;
}

public AntiFreeze_OnPlayerSpawn(playerid){
	if (Anti_Freeze_Cheat_Enabled == 1){
		Debug("protections/freeze.inc > AntiFreeze_OnPlayerSpawn - Start");
		if (PlayerPriv[playerid][IsModerator] == 0 && !IsPlayerAdmin(playerid)){
			SetTimerEx("CheckPlayerForAntiFreeze", 60000, 0, "d d", playerid, 1);
		}
		Debug("protections/freeze.inc > AntiFreeze_OnPlayerSpawn - Stop");
	}
}

public CheckPlayerForAntiFreeze(playerid, num)
{
	if (PlayerFreezed[playerid] == true) SetTimerEx("CheckPlayerForAntiFreeze", 60000, 0, "d d", playerid, 1);
	if (IsPlayerInAnyVehicle(playerid)){
		SetTimerEx("CheckPlayerForAntiFreeze", 60000, 0, "d d", playerid, 1);
		return;
	}
	if(num == 1)
	{
		TogglePlayerControllable(playerid, 0);
		GetPlayerPos(playerid, FreezePos[playerid][Coord_X], FreezePos[playerid][Coord_Y], FreezePos[playerid][Coord_Z]);
		oSetPlayerPos(playerid, FreezePos[playerid][Coord_X]+1, FreezePos[playerid][Coord_Y]+1, FreezePos[playerid][Coord_Z]+1);
		SetTimerEx("CheckPlayerForAntiFreeze", 250, 0, "d d", playerid, 2);
	}
	if(num == 2)
	{
	    	new Float:x,Float:y,Float:z;
	    	GetPlayerPos(playerid, x, y, z);
	    
		if(x != FreezePos[playerid][Coord_X]+1 || y != FreezePos[playerid][Coord_Y]+1 || z != FreezePos[playerid][Coord_Z]+1)
	     	{
			if (Action_On_Freeze == 0) return;
			if (Action_On_Freeze == 1) {Report(playerid,gettext(1345)); return;}
			if (Action_On_Freeze == 2) {KickPlayer(playerid,-1,gettext(1345)); return;}
			if (Action_On_Freeze == 3) {BanPlayer(playerid,Freeze_Ban_Hours,gettext(1345)); return;}
		}
	        
	    	oSetPlayerPos(playerid, FreezePos[playerid][Coord_X], FreezePos[playerid][Coord_Y], FreezePos[playerid][Coord_Z]);
		TogglePlayerControllable(playerid, 1);
	}
}