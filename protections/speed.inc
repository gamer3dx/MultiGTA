//Anti-Speed Module
//Created by Iain Gilbert
//Additions, fixes, changes by MadCat

forward AntiSpeed_OnGameModeInit();
forward AntiSpeedCheatTimer();

new TimerAntiSpeed;

new Anti_SpeedCheat_Enabled=0;
new Speed_Cheat_Tolerance=3;
new Action_On_SpeedCheat_Foot=2;
new SpeedCheat_Foot_Ban_Hours=720;
new Action_On_SuperJump=2;
new SuperJump_Ban_Hours=720;
new Action_On_FlyingVehicle=2;
new FlyingVehicle_Ban_Hours=720;
new Action_On_SpeedCheat_Vehicle=2;
new SpeedCheat_Vehicle_Ban_Hours=720;
new Action_On_InfiniteNOS=2;
new InfiniteNOS_Ban_Hours=720;

#define MAX_FOOTSPEED 45.0 // max player footspeed per second

new PlayerOldSpeed[MAX_PLAYERS];
new PlayerSpeedCheatFound[MAX_PLAYERS];
new PlayerNOS[MAX_PLAYERS];

public AntiSpeed_OnGameModeInit(){
	AntiSpeedCheatConfig();
	if (Anti_SpeedCheat_Enabled == 1){
		TimerAntiSpeed = SetTimer("AntiSpeedCheatTimer",5000,1);
		WriteLog("Anti Speed Cheat system Loaded");
	}
	return 0;
}

AntiSpeedCheatConfig()
{
	Debug("protections/speed.inc > AntiSpeedCheatConfig - Start");
	new temp[MAX_STRING];
	if (!db_Exists(SecurityDB)) db_Create(SecurityDB);
	
	set(temp,db_Get(SecurityDB,"Anti_SpeedCheat_Enabled"));
	if (strlen(temp) > 0) Anti_SpeedCheat_Enabled = strval(temp);
	else { valstr(temp,Anti_SpeedCheat_Enabled); db_Set(SecurityDB,"Anti_SpeedCheat_Enabled",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Speed_Cheat_Tolerance"));
	if (strlen(temp) > 0) Speed_Cheat_Tolerance = strval(temp);
	else { valstr(temp,Speed_Cheat_Tolerance); db_Set(SecurityDB,"Speed_Cheat_Tolerance",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Action_On_SpeedCheat_Foot"));
	if (strlen(temp) > 0) Action_On_SpeedCheat_Foot = strval(temp);
	else { valstr(temp,Action_On_SpeedCheat_Foot); db_Set(SecurityDB,"Action_On_SpeedCheat_Foot",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"SpeedCheat_Foot_Ban_Hours"));
	if (strlen(temp) > 0) SpeedCheat_Foot_Ban_Hours = strval(temp);
	else { valstr(temp,SpeedCheat_Foot_Ban_Hours); db_Set(SecurityDB,"SpeedCheat_Foot_Ban_Hours",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Action_On_SuperJump"));
	if (strlen(temp) > 0) Action_On_SuperJump = strval(temp);
	else { valstr(temp,Action_On_SuperJump); db_Set(SecurityDB,"Action_On_SuperJump",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"SuperJump_Ban_Hours"));
	if (strlen(temp) > 0) SuperJump_Ban_Hours = strval(temp);
	else { valstr(temp,SuperJump_Ban_Hours); db_Set(SecurityDB,"SuperJump_Ban_Hours",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Action_On_FlyingVehicle"));
	if (strlen(temp) > 0) Action_On_FlyingVehicle = strval(temp);
	else { valstr(temp,Action_On_FlyingVehicle); db_Set(SecurityDB,"Action_On_FlyingVehicle",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"FlyingVehicle_Ban_Hours"));
	if (strlen(temp) > 0) FlyingVehicle_Ban_Hours = strval(temp);
	else { valstr(temp,FlyingVehicle_Ban_Hours); db_Set(SecurityDB,"FlyingVehicle_Ban_Hours",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Action_On_SpeedCheat_Vehicle"));
	if (strlen(temp) > 0) Action_On_SpeedCheat_Vehicle = strval(temp);
	else { valstr(temp,Action_On_SpeedCheat_Vehicle); db_Set(SecurityDB,"Action_On_SpeedCheat_Vehicle",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"SpeedCheat_Vehicle_Ban_Hours"));
	if (strlen(temp) > 0) SpeedCheat_Vehicle_Ban_Hours = strval(temp);
	else { valstr(temp,SpeedCheat_Vehicle_Ban_Hours); db_Set(SecurityDB,"SpeedCheat_Vehicle_Ban_Hours",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Action_On_InfiniteNOS"));
	if (strlen(temp) > 0) Action_On_InfiniteNOS = strval(temp);
	else { valstr(temp,Action_On_InfiniteNOS); db_Set(SecurityDB,"Action_On_InfiniteNOS",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"InfiniteNOS_Ban_Hours"));
	if (strlen(temp) > 0) InfiniteNOS_Ban_Hours = strval(temp);
	else { valstr(temp,InfiniteNOS_Ban_Hours); db_Set(SecurityDB,"InfiniteNOS_Ban_Hours",temp);  }
	set(temp,nullstr);

	Debug("protections/speed.inc > AntiSpeedCheatConfig - Stop");
	return;
}

public AntiSpeedCheatTimer()
{
	Debug("protections/speed.inc > AntiSpeedCheatTimer - Start");
	for (new playerid=0;playerid<MAX_PLAYERS_EX;playerid++)
	{
		if (IsPlayerConnected(playerid) && PlayerPriv[playerid][CanMapTeleport] == 0 && PlayerPriv[playerid][IsModerator] == 0 && !IsPlayerAdmin(playerid))
		{
            		new Float:zchange = PlayerPos[playerid][Coord_Z] - PlayerOldPos[playerid][Coord_Z];
            		new string[MAX_STRING];
			if (PlayerJustTeleported[playerid] == 0){
				new PlayerState = GetPlayerState(playerid);
				if (PlayerState == PLAYER_STATE_ONFOOT){
					if (PlayerSpeed[playerid] > MAX_FOOTSPEED){
                       				if (zchange < 0){
							// do nothing, player fall
						}
                        			else if (PlayerSpeedCheatFound[playerid] >= Speed_Cheat_Tolerance) //ok, i can kick player for speed cheat on foot
                        			{
							ResetPlayerSpeedCheatStats(playerid);
							SetPlayerOldPosAsCurrent(playerid);
                        				format(string,sizeof(string),gettext(1350),PlayerSpeed[playerid]);
							if (Action_On_SpeedCheat_Foot == 0) {continue;}
							if (Action_On_SpeedCheat_Foot == 1) {Report(playerid,string); continue;}
							if (Action_On_SpeedCheat_Foot == 2) {KickPlayer(playerid,-1,string); continue;}
							if (Action_On_SpeedCheat_Foot == 3) {BanPlayer(playerid,SpeedCheat_Foot_Ban_Hours,string); continue;}
                        				continue;
                        			}
                    				else
                    				{
                    					PlayerSpeedCheatFound[playerid]++; //just increase meter
                    				}
					}
					else
					{
						if (zchange > 5.0){ // too high jump? i don't think so
							ResetPlayerSpeedCheatStats(playerid);
							SetPlayerOldPosAsCurrent(playerid);
							format(string,sizeof(string),gettext(1351),zchange);
							if (Action_On_SuperJump == 0) {continue;}
							if (Action_On_SuperJump == 1) {Report(playerid,string); continue;}
							if (Action_On_SuperJump == 2) {KickPlayer(playerid,-1,string); continue;}
							if (Action_On_SuperJump == 3) {BanPlayer(playerid,SuperJump_Ban_Hours,string); continue;}
                       					continue;
						} else {
							PlayerSpeedCheatFound[playerid] = 0; // everything ok? meter = 0...
						}
					}

				} else if (PlayerState == PLAYER_STATE_DRIVER) {
					new vehid = GetPlayerVehicleID(playerid);
					new playervehiclemodel = GetVehicleModel(vehid);
                       			if (zchange < 0){
						// do nothing, player fall in vehicle :)
					}
					else if (zchange > 20.0 || (GetPlayerInterior(playerid) == 0 && PlayerPos[playerid][Coord_Z] > 500)){ // too high vehicle jump or vehicle flying? O_O
							if (!CanVehicleFly(playervehiclemodel)){ //this is not plane? cheater!!!
								ResetPlayerSpeedCheatStats(playerid);
								SetPlayerOldPosAsCurrent(playerid);
								format(string,sizeof(string),gettext(1352),zchange);
								if (Action_On_FlyingVehicle == 0) {continue;}
								if (Action_On_FlyingVehicle == 1) {Report(playerid,string); continue;}
								if (Action_On_FlyingVehicle == 2) {KickPlayer(playerid,-1,string); continue;}
								if (Action_On_FlyingVehicle == 3) {BanPlayer(playerid,FlyingVehicle_Ban_Hours,string); continue;}
                        					continue;
							}
					}
					new playervehiclespeed = GetVehicleSpeed(playervehiclemodel);
					if (PlayerSpeed[playerid] > playervehiclespeed){// too high speed
						if (CarHaveNOS[vehid]){
							PlayerNOS[playerid]++;
							if (PlayerNOS[playerid] >= 40){
								ResetPlayerSpeedCheatStats(playerid);
								SetPlayerOldPosAsCurrent(playerid);
								format(string,sizeof(string),gettext(1354),0);
                        					if (Action_On_InfiniteNOS == 0) {continue;}
								if (Action_On_InfiniteNOS == 1) {Report(playerid,string); continue;}
								if (Action_On_InfiniteNOS == 2) {KickPlayer(playerid,-1,string); continue;}
								if (Action_On_InfiniteNOS == 3) {BanPlayer(playerid,InfiniteNOS_Ban_Hours,string); continue;}
								continue;
							}
						} else {
							if (PlayerSpeedCheatFound[playerid] >= Speed_Cheat_Tolerance)
                       					{
								ResetPlayerSpeedCheatStats(playerid);
								SetPlayerOldPosAsCurrent(playerid);
                        					format(string,sizeof(string),gettext(1353),GetVehicleName(playervehiclemodel),PlayerSpeed[playerid],playervehiclespeed);
                        					if (Action_On_SpeedCheat_Vehicle == 0) {continue;}
								if (Action_On_SpeedCheat_Vehicle == 1) {Report(playerid,string); continue;}
								if (Action_On_SpeedCheat_Vehicle == 2) {KickPlayer(playerid,-1,string); continue;}
								if (Action_On_SpeedCheat_Vehicle == 3) {BanPlayer(playerid,SpeedCheat_Vehicle_Ban_Hours,string); continue;}
                        					continue;
                        				} else {
                    						PlayerSpeedCheatFound[playerid]++;
                    					}
						}
					} else {
						PlayerSpeedCheatFound[playerid] = 0; // everything ok? meter = 0...
						PlayerNOS[playerid] = 0;
					}
						
				}
			}
			SetPlayerOldPosAsCurrent(playerid);
			PlayerOldSpeed[playerid] = PlayerSpeed[playerid];
		}
	}
	Debug("protections/speed.inc > AntiSpeedCheatTimer - Stop");
}

SetPlayerOldPosAsCurrent(playerid){
	PlayerOldPos[playerid][Coord_X] = PlayerPos[playerid][Coord_X];
	PlayerOldPos[playerid][Coord_Y] = PlayerPos[playerid][Coord_Y];
	PlayerOldPos[playerid][Coord_Z] = PlayerPos[playerid][Coord_Z];
}

ResetPlayerSpeedCheatStats(playerid)
{
	PlayerOldPos[playerid][Coord_X] = 0.0;
	PlayerOldPos[playerid][Coord_Y] = 0.0;
	PlayerOldPos[playerid][Coord_Z] = 0.0;
	PlayerOldSpeed[playerid] = 0;
	PlayerNOS[playerid] = 0;
	PlayerSpeedCheatFound[playerid] = 0;
}