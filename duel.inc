//Duel module
//Created by GTAT team (http://gtat.org)
//Coords from GTAT (modules duell and tourna)
//Converted by MadCat

forward duel_OnPlayerCommandText(playerid,text[]);
forward duel_Timer();
forward duel_OnPlayerDeath(playerid,killerid,reason);
forward duel_OnPlayerSpawn(playerid);
forward duel_OnPlayerDisconnect(playerid,reason);
forward duel_OnGameModeInit();

forward EndDuel(playerid);

#define DUELS_COUNT 17

new DuelTime[DUELS_COUNT];
new DuelLocation[DUELS_COUNT];
new WeaponForDuel[DUELS_COUNT];
new DuelPlaceName[DUELS_COUNT][MAX_NAME];
new PlayerDuel1[DUELS_COUNT];
new PlayerDuel2[DUELS_COUNT];
new StarterDuel[DUELS_COUNT];
new DUEL_PlayerWasAt[MAX_PLAYERS][CoordInfo];

new TimerDuel;

enum DuelInfo {
	Name[MAX_STRING],
	Float:Player1X,
	Float:Player1Y,
	Float:Player1Z,
	Float:Player1R,
	Float:Player2X,
	Float:Player2Y,
	Float:Player2Z,
	Float:Player2R,
	Float:StarterX,
	Float:StarterY,
	Float:StarterZ,
	Float:StarterR
}

new Duels[DUELS_COUNT][DuelInfo] = {
	{"LS - Ganton",2510.6409,-1671.477,13.4273,80.9132,2463.1638,-1658.8440,13.3047,269.8316,2482.4797,-1686.4331,13.5078,358.8190},
	{"Desert - Verdant Meadows",290.7083,2536.3652,16.8204,178.3376,326.4536,2536.6804,16.8071,176.8270,307.8979,2536.3572,16.8162,184.0337},
	{"Desert - Hill",-772.7994,2414.0583,157.0781,80.6326,-815.8213,2411.5559,156.3741,277.0945,-793.9265,2393.4988,159.8219,346.6551},
	{"SF - Driving School",-2052.8047,-115.3339,35.2881,178.4402,-2059.9133,-271.4485,35.3203,355.7652,-2025.8654,-190.1775,35.3203,92.2728},
	{"LV - Baseball Stadium",1305.4500,2107.0125,11.0156,316.2849,1393.1835,2194.0259,11.0234,128.9331,1393.6890,2159.1616,14.2495,100.4195},
	{"LS - Hill",1133.5670,-2036.7322,69.0078,270.7176,1175.9695,-2007.0253,69.0078,175.1734,1175.3529,-2064.5171,69.0006,1.5851},
	{"LS - Verona Mall",1086.2744,-1491.2257,22.7561,273.6738,1129.8911,-1426.7748,15.7969,175.5995,1129.0818,-1533.1527,15.3292,1.4079},
	{"LS - Beach",245.4832,-1870.4946,3.3176,358.7360,208.2975,-1829.5797,3.9461,259.4319,276.1742,-1832.1699,3.7577,90.8569},
	{"SF - Airport Entrance",-1491.7223,-377.6141,15.5542,326.1787,-1491.4132,-317.1754,14.8857,220.8975,-1440.0625,-375.1543,15.0066,36.0291},
	{"LV - Caligula Rooftop",2187.0315,1686.4764,20.3906,82.9465,2177.9038,1651.6649,20.3837,4.6124,2176.7627,1713.9994,20.3906,191.9875},
	{"LV - Skyscrapper",2449.7778,2378.5486,71.0496,270.0,2489.5415,2378.6658,71.0496,90.0,2460.1892,2336.0178,82.7734,180.0},
	{"Desert - Wild West",-375.2620,2203.2485,42.0938,13.6295,-389.0793,2257.3638,42.0292,194.4245,-392.8202,2228.0439,47.9601,285.3154},
	{"Desert - Dam",-830.2930,1980.8583,9.2500,189.1446,-800.5883,1823.9233,7.0000,10.8563,-807.5495,1829.2427,23.9245,253.3552},
	{"SF - Big Building",-1775.9803,578.0722,234.8906,90.5372,-1836.2024,578.3348,234.8906,270.0555,-1812.2483,541.1913,234.8874,347.4495},
	{"LV - Pier",2391.5447,598.6744,7.7802,108.9577,2238.2742,546.4180,7.7813,289.4160,2317.1006,604.6239,10.8203,178.8083},
	{"LV - Pyramide",2258.1990,1217.8845,11.8514,315.6061,2389.2546,1348.6991,11.5756,136.4011,2312.7979,1161.1974,85.9375,355.5409},
	{"SF - Train Station",-1939.3143,111.8237,26.2734,358.5566,-1940.0365,168.8738,26.2813,179.9783,-1939.6541,164.4411,41.0000,179.6650}
};

stock GetLowestFightID(){
	for(new i=0; i<DUELS_COUNT; i++){
	    if(DuelTime[i]==0) return i;
	}
	return -1;
}

public duel_OnGameModeInit(){
	for(new fightid=0; fightid<DUELS_COUNT; fightid++){
		DuelTime[fightid]=0;
	}
	QUEST_DUEL = RegisterQuest("Duel");
	TimerDuel = SetTimer("duel_Timer",1000,1);
	WriteLog("Duel system Loaded.");
	return 0;
}

public duel_OnPlayerDisconnect(playerid,reason){
	for(new fightid=0; fightid<DUELS_COUNT; fightid++)
		if (DuelTime[fightid] > 0 && (playerid == PlayerDuel1[fightid] || playerid == PlayerDuel2[fightid]))	{
			DUEL_PlayerWasAt[playerid][Coord_X] = DUEL_PlayerWasAt[playerid][Coord_Y] = DUEL_PlayerWasAt[playerid][Coord_Z] = 0.0; //Reset vars
			new string[MAX_STRING];
			if(reason == 0) string = gettext(1539);
			if(reason == 1) string = gettext(1540);
			if(reason == 2) string = gettext(1541);
	        	format(string,MAX_STRING,gettext(1542),oGetPlayerName(playerid),string);
		        SystemMsg(StarterDuel[fightid],COLOUR_DUEL,string);
			for(new i=0; i<MAX_PLAYERS_EX; i++){
			    	if((i!=playerid) && (IsPlayerConnected(i)) && (i == PlayerDuel1[fightid] || i == PlayerDuel2[fightid])) {
			        	SystemMsg(i,COLOUR_DUEL,gettext(1543));
					EndDuel(i);
			    	}
			}				
		}
	return 0;
}

public EndDuel(playerid){
	for(new fightid=0; fightid<DUELS_COUNT; fightid++)
		if (DuelTime[fightid] > 0 && (playerid == PlayerDuel1[fightid] || playerid == PlayerDuel2[fightid])) {
				PlayerQuest[PlayerDuel1[fightid]] = QUEST_NOQUEST;
				PlayerQuest[PlayerDuel2[fightid]] = QUEST_NOQUEST;
				PlayerQuest[StarterDuel[fightid]] = QUEST_NOQUEST;
				new name[MAX_NAME];
				set(name,oGetPlayerName(playerid));
				SystemMsgFormatStr(StarterDuel[fightid],COLOUR_DUEL,gettext(1544),name);
				SystemMsgFormatStr(PlayerDuel1[fightid],COLOUR_DUEL,gettext(1544),name);
				SystemMsgFormatStr(PlayerDuel2[fightid],COLOUR_DUEL,gettext(1544),name);
				SetPlayerVirtualWorld(PlayerDuel1[fightid],WORLD_DEFAULT);
				SetPlayerVirtualWorld(PlayerDuel2[fightid],WORLD_DEFAULT);
				SetPlayerVirtualWorld(StarterDuel[fightid],WORLD_DEFAULT);
				oSetPlayerPos(PlayerDuel1[fightid],DUEL_PlayerWasAt[PlayerDuel1[fightid]][Coord_X],DUEL_PlayerWasAt[PlayerDuel1[fightid]][Coord_Y],DUEL_PlayerWasAt[PlayerDuel1[fightid]][Coord_Z]);
				oSetPlayerPos(PlayerDuel2[fightid],DUEL_PlayerWasAt[PlayerDuel2[fightid]][Coord_X],DUEL_PlayerWasAt[PlayerDuel2[fightid]][Coord_Y],DUEL_PlayerWasAt[PlayerDuel2[fightid]][Coord_Z]);
				ResetPlayerWeapons(PlayerDuel1[fightid]);
				ResetPlayerWeapons(PlayerDuel2[fightid]);
				GivePlayerOwnedWeapons(PlayerDuel1[fightid]);
				GivePlayerOwnedWeapons(PlayerDuel2[fightid]);
				oSetPlayerHealth(PlayerDuel1[fightid],GetMaxHealth(PlayerDuel1[fightid]));
				oSetPlayerHealth(PlayerDuel2[fightid],GetMaxHealth(PlayerDuel2[fightid]));
				oSetPlayerHealth(StarterDuel[fightid],GetMaxHealth(StarterDuel[fightid]));
				PlayerDuel1[fightid] = INVALID_PLAYER_ID;
				PlayerDuel2[fightid] = INVALID_PLAYER_ID;
				StarterDuel[fightid] = INVALID_PLAYER_ID;
				DuelTime[fightid]=0;
			}
}
public duel_OnPlayerSpawn(playerid) {
	for(new fightid=0; fightid<DUELS_COUNT; fightid++)
		if (DuelTime[fightid] > 0 && (playerid == StarterDuel[fightid])) {
			SetPlayerInterior(StarterDuel[fightid],INTERIOR_DEFAULT);
			oSetPlayerPos(StarterDuel[fightid],Duels[DuelLocation[fightid]][StarterX],Duels[DuelLocation[fightid]][StarterY],Duels[DuelLocation[fightid]][StarterZ]);
			SetPlayerFacingAngle(StarterDuel[fightid],Duels[DuelLocation[fightid]][StarterR]);
			SetPlayerVirtualWorld(StarterDuel[fightid],PrivateWorld(StarterDuel[fightid]));
			return 1;
		}
	return 0;
}

public duel_OnPlayerDeath(playerid,killerid,reason){
	for(new fightid=0; fightid<DUELS_COUNT; fightid++)
		if (DuelTime[fightid] > 0 && (playerid == PlayerDuel1[fightid] || playerid == PlayerDuel2[fightid])) {
			new string[MAX_STRING];
			if(playerid == PlayerDuel1[fightid]) {
				SetTimerEx("EndDuel",1000,0,"i",PlayerDuel2[fightid]);
				if (reason != 0 && reason != WeaponForDuel[fightid] && reason < 47){ //not fist, not weapon for duel and... weapon! :)
					format(string,MAX_STRING,gettext(1575),oGetPlayerName(PlayerDuel1[fightid]),oGetPlayerName(PlayerDuel2[fightid]),oGetWeaponName(WeaponForDuel[fightid]),oGetWeaponName(reason));
				}
			} else {
				SetTimerEx("EndDuel",1000,0,"i",PlayerDuel1[fightid]);
				if (reason != 0 && reason != WeaponForDuel[fightid] && reason < 47){ //not fist, not weapon for duel and... weapon! :)
					format(string,MAX_STRING,gettext(1575),oGetPlayerName(PlayerDuel2[fightid]),oGetPlayerName(PlayerDuel1[fightid]),oGetWeaponName(WeaponForDuel[fightid]),oGetWeaponName(reason));
				}
			}
			return SystemMsg(StarterDuel[fightid],COLOUR_INFO,string);
		}
	return 0;
}

public duel_Timer() {
	for(new fightid=0; fightid<DUELS_COUNT; fightid++) if (DuelTime[fightid] > 0) {
	
		new string[MAX_STRING];
		
		if (DuelTime[fightid] < MAX_COUNTDOWN){
			format(string,MAX_STRING,"%d",MAX_COUNTDOWN - DuelTime[fightid]);
			ShowTextForPlayer(PlayerDuel1[fightid],string,2000,6);
			ShowTextForPlayer(PlayerDuel2[fightid],string,2000,6);
			ShowTextForPlayer(StarterDuel[fightid],string,2000,6);
		}
		else if(DuelTime[fightid] == MAX_COUNTDOWN){
			ShowTextForPlayer(PlayerDuel1[fightid],gettext(975),1000,6);
			ShowTextForPlayer(PlayerDuel2[fightid],gettext(975),1000,6);
			ShowTextForPlayer(StarterDuel[fightid],gettext(975),1000,6);
			oSetPlayerHealth(PlayerDuel1[fightid],100);
			oSetPlayerHealth(PlayerDuel2[fightid],100);
			TogglePlayerControllable(PlayerDuel1[fightid],1);
			TogglePlayerControllable(PlayerDuel2[fightid],1);
		}
		DuelTime[fightid]++;
	}
}

public duel_OnPlayerCommandText(playerid,text[]){
	if (!IsPlayerConnected(playerid)) return 0;
	if (PlayerPriv[playerid][CanStartDuels] == 0) return 0;
	dcmd(duel,4,text);
	dcmd(duelplaces,10,text);
	
	return 0;
}

StartDuel(fightid){
	// Players are on a mission now:
	PlayerQuest[PlayerDuel1[fightid]] = QUEST_DUEL;
	PlayerQuest[PlayerDuel2[fightid]] = QUEST_DUEL;
	PlayerQuest[StarterDuel[fightid]] = QUEST_DUEL;

	// Remember positions
	DUEL_PlayerWasAt[PlayerDuel1[fightid]][Coord_X] = PlayerPos[PlayerDuel1[fightid]][Coord_X];
	DUEL_PlayerWasAt[PlayerDuel1[fightid]][Coord_Y] = PlayerPos[PlayerDuel1[fightid]][Coord_Y];
	DUEL_PlayerWasAt[PlayerDuel1[fightid]][Coord_Z] = PlayerPos[PlayerDuel1[fightid]][Coord_Z];

	DUEL_PlayerWasAt[PlayerDuel2[fightid]][Coord_X] = PlayerPos[PlayerDuel2[fightid]][Coord_X];
	DUEL_PlayerWasAt[PlayerDuel2[fightid]][Coord_Y] = PlayerPos[PlayerDuel2[fightid]][Coord_Y];
	DUEL_PlayerWasAt[PlayerDuel2[fightid]][Coord_Z] = PlayerPos[PlayerDuel2[fightid]][Coord_Z];
	
	// Players should be all at the same place now:
	// #1
	SetPlayerInterior(PlayerDuel1[fightid],INTERIOR_DEFAULT);
	oSetPlayerPos(PlayerDuel1[fightid],Duels[DuelLocation[fightid]][Player1X],Duels[DuelLocation[fightid]][Player1Y],Duels[DuelLocation[fightid]][Player1Z]);
	SetPlayerFacingAngle(PlayerDuel1[fightid],Duels[DuelLocation[fightid]][Player1R]);
	SetCameraBehindPlayer(PlayerDuel1[fightid]);
	SetPlayerVirtualWorld(PlayerDuel1[fightid],PrivateWorld(StarterDuel[fightid]));
	// #2
	SetPlayerInterior(PlayerDuel2[fightid],INTERIOR_DEFAULT);
	oSetPlayerPos(PlayerDuel2[fightid],Duels[DuelLocation[fightid]][Player2X],Duels[DuelLocation[fightid]][Player2Y],Duels[DuelLocation[fightid]][Player2Z]);
	SetPlayerFacingAngle(PlayerDuel2[fightid],Duels[DuelLocation[fightid]][Player2R]);
	SetCameraBehindPlayer(PlayerDuel2[fightid]);
	SetPlayerVirtualWorld(PlayerDuel2[fightid],PrivateWorld(StarterDuel[fightid]));
	// initialiser
	SetPlayerInterior(StarterDuel[fightid],INTERIOR_DEFAULT);
	oSetPlayerPos(StarterDuel[fightid],Duels[DuelLocation[fightid]][StarterX],Duels[DuelLocation[fightid]][StarterY],Duels[DuelLocation[fightid]][StarterZ]);
	SetPlayerFacingAngle(StarterDuel[fightid],Duels[DuelLocation[fightid]][StarterR]);
	SetCameraBehindPlayer(StarterDuel[fightid]);
	SetPlayerVirtualWorld(StarterDuel[fightid],PrivateWorld(StarterDuel[fightid]));

	// Not able to move ...
	TogglePlayerControllable(PlayerDuel1[fightid],0);
	TogglePlayerControllable(PlayerDuel2[fightid],0);

	// Players should have 100 health :-)
	oSetPlayerHealth(PlayerDuel1[fightid],100);
	oSetPlayerHealth(PlayerDuel2[fightid],100);
	// And also the initaliser ... He is immortal :)
	oSetPlayerHealth(StarterDuel[fightid],INFINITY);

	// With new weapons ...
	ResetPlayerWeapons(PlayerDuel1[fightid]);
	GivePlayerWeapon(PlayerDuel1[fightid],WeaponForDuel[fightid],GetWeaponMaxAmmo(WeaponForDuel[fightid]));
	ResetPlayerWeapons(PlayerDuel2[fightid]);
	GivePlayerWeapon(PlayerDuel2[fightid],WeaponForDuel[fightid],GetWeaponMaxAmmo(WeaponForDuel[fightid]));

	new string[MAX_STRING];
	format(string,MAX_STRING,gettext(1545),MAX_COUNTDOWN,oGetPlayerName(PlayerDuel2[fightid]),DuelPlaceName[fightid]);
	SystemMsg(PlayerDuel1[fightid],COLOUR_DUEL,string);
	format(string,MAX_STRING,gettext(1545),MAX_COUNTDOWN,oGetPlayerName(PlayerDuel1[fightid]),DuelPlaceName[fightid]);
	SystemMsg(PlayerDuel2[fightid],COLOUR_DUEL,string);
	set(DuelPlaceName[fightid],nullstr);
	
	DuelTime[fightid] = 1;
}

dcmd_duelplaces(playerid,params[]){
	#pragma unused params
   	new string[MAX_STRING];
	for(new i=0; i<DUELS_COUNT; i++){
		format(string,MAX_STRING,gettext(1546),i,Duels[i][Name]);
		SystemMsgScrolling(playerid,string);
	}
	return 1;
}

dcmd_duel(playerid,params[])
{
	new tmp[MAX_STRING], idx;
	tmp = strcharsplit(params, idx,strchar(" "));
	if (isempty(tmp)) return SystemMsg(playerid,COLOUR_ERROR,gettext(1547));
	new place = strval(tmp);
	if(place < 0 || place >= DUELS_COUNT) return SystemMsg(playerid,COLOUR_ERROR,gettext(1547));
	tmp = strcharsplit(params, idx,strchar(" "));
	if (isempty(tmp)) return SystemMsg(playerid,COLOUR_ERROR,gettext(1548));
	new weapon = GetWeaponID(tmp);
	if (weapon==-1) return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1573),tmp);
	if (weapon==-2) return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1574),tmp);
	tmp = strcharsplit(params, idx,strchar(" "));
	if (isempty(tmp)) return SystemMsg(playerid,COLOUR_ERROR,gettext(1549));
	new pid1 = FindPlayerID(tmp);
	if (pid1==-1) return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1571),tmp);
	if (pid1==-2) return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1572),tmp);
	tmp = strcharsplit(params, idx,strchar(" "));
	if (isempty(tmp)) return SystemMsg(playerid,COLOUR_ERROR,gettext(1550));
	new pid2 = FindPlayerID(tmp);
	if (pid2==-1) return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1571),tmp);
	if (pid2==-2) return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1572),tmp);
	if(pid1 == pid2) return SystemMsg(playerid,COLOUR_ERROR,gettext(1551));
	if(!IsPlayerConnected(pid1) || !IsPlayerConnected(pid2)) return SystemMsg(playerid,COLOUR_ERROR,gettext(1552));
	if(PlayerQuest[pid1]!=QUEST_NOQUEST) return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1553),oGetPlayerName(pid1));
	if(PlayerQuest[pid2]!=QUEST_NOQUEST) return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1553),oGetPlayerName(pid2));
	if(PlayerQuest[playerid]!=QUEST_NOQUEST) return SystemMsg(playerid,COLOUR_ERROR,gettext(1554));
	if(Player[pid1][JailTime] > 0) return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1555),oGetPlayerName(pid1));
	if(Player[pid2][JailTime] > 0) return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1555),oGetPlayerName(pid1));
	if((pid2 == playerid) || (pid1 == playerid)) return SystemMsg(playerid,COLOUR_ERROR,gettext(1556));
	new fightid = GetLowestFightID();
	if(fightid == -1) return SystemMsg(playerid,COLOUR_ERROR,gettext(1557));
	DuelLocation[fightid] = place;
	WeaponForDuel[fightid] = weapon;
	PlayerDuel1[fightid] = pid1;
	PlayerDuel2[fightid] = pid2;
	StarterDuel[fightid] = playerid;
	set(DuelPlaceName[fightid],Duels[place][Name]);
	StartDuel(fightid);
	SystemMsg(playerid,COLOUR_DUEL,gettext(1558));
	return 1;
}

stock IsPlayerInDuel(playerid){
	for(new fightid=0; fightid<DUELS_COUNT; fightid++){
		if ((playerid == PlayerDuel1[fightid] || playerid == PlayerDuel2[fightid]) && DuelTime[fightid] > 0 && PlayerQuest[playerid] == QUEST_DUEL){
			return true;
		}
	}
	return false;
}