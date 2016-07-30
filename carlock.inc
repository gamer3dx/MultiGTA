//Car Lock module
//Created by GTAT Dev Team. Credits for this!
//Converted by MadCat

forward carlock_OnGameModeInit();
forward carlock_OnPlayerCommandText(playerid,text[]);
forward carlock_OnPlayerExitVehicle(playerid,vehicleid);
forward carlock_OnPlayerDisconnect(playerid,reason);
forward carlock_OnPlayerConnect(playerid);
forward carlock_OnVehicleSpawn(vehicleid);

new CARLOCK_VehicleLockedForPlayer[MAX_VEHICLES][MAX_PLAYERS];
new CARLOCK_VehicleLockedForAll[MAX_VEHICLES];
new CARLOCK_PlayerLockedVehicle[MAX_PLAYERS];

new CarLock_Enabled = 1;

public carlock_OnGameModeInit(){
	CarLockConfig();
	return 0;
}

CarLockConfig()
{
	Debug("carlock.inc > CarLockConfig - Start");
	new temp[MAX_STRING];
	if (!db_Exists(ConfigDB)) db_Create(ConfigDB);
	
	set(temp,db_Get(ConfigDB,"CarLock_Enabled"));
	if (strlen(temp) > 0) CarLock_Enabled = strval(temp);
	else { valstr(temp,CarLock_Enabled); db_Set(ConfigDB,"CarLock_Enabled",temp);  }
	set(temp,nullstr);

	Debug("carlock.inc > CarLockConfig - Stop");
	return;
}

public carlock_OnPlayerCommandText(playerid,text[])
{
	if (CarLock_Enabled == 0) return 0;
	if (!IsPlayerConnected(playerid)) return 0;
	dcmd(lock,4,text);
	dcmd(unlock,6,text);

	return 0;
}

public carlock_OnPlayerConnect(playerid){
	if (CarLock_Enabled == 0) return 0;
	for(new v=0; v<MAX_VEHICLES; v++){
	    if(CARLOCK_VehicleLockedForAll[v] == 1){
	        LockVehicleForPlayer(v,playerid);
	    }
	}
	CARLOCK_PlayerLockedVehicle[playerid] = -1;
	return 0;
}

public carlock_OnPlayerDisconnect(playerid,reason){
	if (CarLock_Enabled == 0) return 0;
	if(CARLOCK_PlayerLockedVehicle[playerid] != -1){
	    UnlockVehicleForAll(CARLOCK_PlayerLockedVehicle[playerid]);
	}
	return 0;
}

public carlock_OnPlayerExitVehicle(playerid,vehicleid){
	if (CarLock_Enabled == 0) return 0;
	CARLOCK_PlayerLockedVehicle[playerid] = -1;
	UnlockVehicleForAll(vehicleid);
	return 0;
}

public carlock_OnVehicleSpawn(vehicleid){
	if (CarLock_Enabled == 0) return 0;
	UnlockVehicleForAll(vehicleid);
	return 0;
}

stock LockVehicleForAll(vehicleid){
	for(new playerid=0; playerid<MAX_PLAYERS_EX; playerid++){
	    if(IsPlayerConnected(playerid)){
	        oSetVehicleParamsForPlayer(vehicleid,playerid,0,1);
	        CARLOCK_VehicleLockedForPlayer[vehicleid][playerid] = 1;
	    }
		CARLOCK_VehicleLockedForAll[vehicleid] = 1;
	}
}

stock UnlockVehicleForAll(vehicleid){
	for(new playerid=0; playerid<MAX_PLAYERS_EX; playerid++){
	    if(IsPlayerConnected(playerid)){
	        oSetVehicleParamsForPlayer(vehicleid,playerid,0,0);
	        CARLOCK_VehicleLockedForPlayer[vehicleid][playerid] = 0;
	    }
		CARLOCK_VehicleLockedForAll[vehicleid] = 0;
	}
}

stock LockVehicleForPlayer(vehicleid,playerid){
	oSetVehicleParamsForPlayer(vehicleid,playerid,0,1);
	CARLOCK_VehicleLockedForPlayer[vehicleid][playerid] = 1;
}

stock UnlockVehicleForPlayer(vehicleid,playerid){
	oSetVehicleParamsForPlayer(vehicleid,playerid,0,0);
	CARLOCK_VehicleLockedForPlayer[vehicleid][playerid] = 0;
}

dcmd_lock(playerid,params[])
{
	#pragma unused params
	if(IsPlayerInAnyVehicle(playerid)) {
		new State=GetPlayerState(playerid);
		if(State!=PLAYER_STATE_DRIVER) {
			SystemMsg(playerid,COLOUR_ERROR,gettext(1017));
			return 1;
		}
		new vid = GetPlayerVehicleID(playerid);
		new model = GetVehicleModel(vid);
		if (IsVehicleMotoBike(model) || model == V_BIKE || model == V_BMX || model == V_MTBIKE){
			SystemMsg(playerid,COLOUR_ERROR,gettext(171));
			return 1;
		}
   		CARLOCK_PlayerLockedVehicle[playerid] = vid;
   		LockVehicleForAll(vid);
		SystemMsg(playerid,COLOUR_OK,gettext(1023));
	}
	else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(673));
	}
return 1;
}

dcmd_unlock(playerid,params[])
{
	#pragma unused params
	if(IsPlayerInAnyVehicle(playerid)) {
		new State = GetPlayerState(playerid);
		if(State!=PLAYER_STATE_DRIVER) {
			SystemMsg(playerid,COLOUR_ERROR,gettext(1017));
			return 1;
		}
		new vid = GetPlayerVehicleID(playerid);
		new model = GetVehicleModel(vid);
		if (IsVehicleMotoBike(model) || model == V_BIKE || model == V_BMX || model == V_MTBIKE){
			SystemMsg(playerid,COLOUR_ERROR,gettext(180));
			return 1;
		}
		UnlockVehicleForAll(vid);
		SystemMsg(playerid,COLOUR_OK,gettext(1328));
	}
	else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(673));
	}
return 1;
}