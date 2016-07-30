//Anti-Teleport Module
//Created by MadCat

forward AntiTele_OnGameModeInit();
forward AntiTele_Timer();
forward AntiTele_OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid);
forward AntiTele_OnPlayerStateChange(playerid, newstate, oldstate);

new TimerAntiTele;

new Anti_Teleport_Enabled=0;
new Action_On_Teleport=2;
new Teleport_Ban_Hours=720;

public AntiTele_OnGameModeInit(){
	AntiTeleportConfig();
	if (Anti_Teleport_Enabled == 1){
		TimerAntiTele = SetTimer("AntiTele_Timer",1000,1);
		WriteLog("Anti Teleport Cheat Loaded");
	}
	return 0;
}

AntiTeleportConfig()
{
	Debug("protections/teleport.inc > AntiTeleportConfig - Start");
	new temp[MAX_STRING];
	if (!db_Exists(SecurityDB)) db_Create(SecurityDB);
	
	set(temp,db_Get(SecurityDB,"Anti_Teleport_Enabled"));
	if (strlen(temp) > 0) Anti_Teleport_Enabled = strval(temp);
	else { valstr(temp,Anti_Teleport_Enabled); db_Set(SecurityDB,"Anti_Teleport_Enabled",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Action_On_Teleport"));
	if (strlen(temp) > 0) Action_On_Teleport = strval(temp);
	else { valstr(temp,Action_On_Teleport); db_Set(SecurityDB,"Action_On_Teleport",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Teleport_Ban_Hours"));
	if (strlen(temp) > 0) Teleport_Ban_Hours = strval(temp);
	else { valstr(temp,Teleport_Ban_Hours); db_Set(SecurityDB,"Teleport_Ban_Hours",temp);  }
	set(temp,nullstr);


	Debug("protections/teleport.inc > AntiTeleportConfig - Stop");
	return;
}

public AntiTele_Timer(){
	Debug("protections/teleport.inc > AntiTele_Timer - Start");
	for(new i;i<MAX_PLAYERS_EX;i++){
 	    	if(IsPlayerConnected(i) && (!IsPlayerAdmin(i) && GetPlayerState(i) != PLAYER_STATE_SPECTATING && PlayerPriv[i][CanMapTeleport] == 0 && PlayerPriv[i][IsModerator] == 0)) { //admin can teleport. spectators detected like teleporters. player with CanMapTeleport not checked, he does not use oSetPlayerPos
			if(PlayerJustTeleported[i] == 0 && IsPlayerInRangeOfPoint(i,100.0,AntiTeleLastPos[i][Coord_X],AntiTeleLastPos[i][Coord_Y],AntiTeleLastPos[i][Coord_Z]) ){
				if (Action_On_Teleport == 0) continue;
				if (Action_On_Teleport == 1) Report(i,gettext(1306));
				if (Action_On_Teleport == 2) KickPlayer(i,-1,gettext(1306));
				if (Action_On_Teleport == 3) BanPlayer(i,Teleport_Ban_Hours,gettext(1306));
			}
			if (IsPlayerConnected(i)){
				AntiTeleLastPos[i][Coord_X] = PlayerPos[i][Coord_X];
				AntiTeleLastPos[i][Coord_Y] = PlayerPos[i][Coord_Y];
				AntiTeleLastPos[i][Coord_Z] = PlayerPos[i][Coord_Z];
				//PlayerLastInterior[i] = GetPlayerInterior(i); //Now in SyncPlayres
			}
	    	}
	}
	Debug("protections/teleport.inc > AntiTele_Timer - Stop");
}

public AntiTele_OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	#pragma unused newinteriorid
	#pragma unused oldinteriorid
	PlayerJustTeleported[playerid]+=3;
}

public AntiTele_OnPlayerStateChange(playerid, newstate, oldstate)
{
	#pragma unused newstate
	if(oldstate==PLAYER_STATE_PASSENGER) PlayerJustTeleported[playerid]+=3;
}