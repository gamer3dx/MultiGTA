//Derby module
//Created by Novocaine Dev Team
//Converted by MadCat

#include "stadium"

//Defines
#define DERBY_MAX_PLAYERS 18
#define DERBY_BARRELS 45
#define DERBY_MIN_MOVE 3
#define DERBY_MAX_LACK_OF_MOVE 4
#define DERBY_MAX_LACK_OF_MOVE2 4

//Settings
new Derby_Wait_Time = 30;
new Derby_Entry_Fee = 1000;
new Derby_Forced_Length = 480;
new Derby_Pot_Increase_Per_Player = 2000;
new Derby_Give_Weapon = 0;
new Derby_Min_Players = 2;

//Vars
new gStartDerbyCount;
new gStartDerbyTimer;
new gDerbyInProgress;
new gDerbyPrizePot;
new gDerbyPlayerCar[MAX_PLAYERS];
new gDerbyPlayerCarSpot[DERBY_MAX_PLAYERS];
new gForceEndDerby = -1;
new Float:gDerbyCarHealth[MAX_PLAYERS];
new gWaitingDerby[MAX_PLAYERS];

enum DerbyMoveEnum
{
    Float:PrevX,
    Float:PrevY,
    Float:PrevZ
}
new gDerbyMovePrevPos[MAX_PLAYERS][DerbyMoveEnum];
new gDerbyLackOfMove[MAX_PLAYERS];
new gDerbyLackOfMove2[MAX_PLAYERS];

enum DerbyCarCoords {
	Float: X,
	Float: Y,
	Float: Z,
	Float: ROT
}

enum WeaponsForDerby {
	Weaponid,
	Bullets
}

new gDerbyCars[DERBY_MAX_PLAYERS][DerbyCarCoords] = {
	{-1362.1300,934.0107,1036.1425,8.2077},
        {-1281.4504,993.7186,1037.0098,88.5409},
	{-1357.7025,1056.9972,1038.1967,171.7115},
	{-1471.5518,1048.7573,1038.2665,202.9111},
	{-1503.1484,964.0452,1036.9065,313.1971},
	{-1334.5221,938.7358,1036.1893,20.2244},
	{-1287.5679,1018.6424,1037.4434,121.2998},
	{-1386.3435,1058.6985,1038.2795,181.3318},
	{-1494.9187,1035.0526,1038.0681,219.8876},
	{-1485.0924,948.7413,1036.6074,328.7740},
	{-1310.0051,951.3401,1036.3535,33.6543},
	{-1306.0643,1036.9724,1037.7799,144.8096},
	{-1416.3500,1059.1179,1038.3304,179.4746},
	{-1512.4734,1012.2025,1037.7173,248.3608},
	{-1290.5635,969.8209,1036.6274,57.2449},
 	{-1330.4025,1050.7218,1038.0485,159.3552},
	{-1444.3807,1056.4791,1038.3386,190.0766},
	{-1514.8184,986.4584,1037.3016,282.9504}};

new gDerbyBarrels[DERBY_BARRELS][CoordInfo] = {	
	{-1370.5059,930.7913,1040.8313},
	{-1374.4453,930.7526,1040.8313},
	{-1379.2421,930.6647,1040.8313},
	{-1383.8975,930.6188,1040.8313},
	{-1389.1924,930.6469,1040.8313},
	{-1394.3082,930.6138,1040.8313},
	{-1399.2671,930.5474,1040.8313},
	{-1404.3220,930.5677,1040.8313},
	{-1409.6389,930.4827,1040.8313},
	{-1414.1080,930.4657,1040.8313},
	{-1418.9791,930.4529,1040.8313},
	{-1423.0762,930.5989,1040.8313},
	{-1428.5613,930.9061,1040.8313},
	{-1433.6459,931.2767,1040.8313},
	{-1438.9602,931.8196,1040.8313},
	{-1444.2720,932.6198,1040.8313},
	{-1449.3865,933.5886,1040.8313},
	{-1453.9938,934.5941,1040.8313},
	{-1458.8187,935.7296,1040.8239},
	{-1463.1307,937.0088,1040.8313},
	{-1468.2520,938.8137,1040.8313},
	{-1473.4585,940.7531,1040.8239},
	{-1477.2518,942.3598,1040.8313},
	{-1481.2731,944.2432,1040.8239},
	{-1347.2617,935.1491,1035.7586},
	{-1319.2505,944.7104,1035.8716},
	{-1296.7683,959.9625,1036.0871},
	{-1282.1489,981.1239,1036.4061},
	{-1281.0913,1006.6852,1036.8309},
	{-1294.5475,1030.9448,1037.2617},
	{-1318.6968,1047.0227,1037.5680},
	{-1345.1302,1055.6395,1037.7599},
	{-1372.4952,1059.8375,1037.8823},
	{-1402.6470,1059.7668,1037.9327},
	{-1430.8793,1058.9067,1037.9619},
	{-1458.6072,1054.4073,1037.9364},
	{-1485.7925,1042.7750,1037.7926},
	{-1508.0482,1024.7008,1037.5282},
	{-1518.3781,996.8094,1037.0857},
	{-1511.2568,972.5646,1036.6748},
	{-1494.2396,954.2856,1036.6341},
	{-1483.3000,939.8836,1037.2135},
	{-1478.1072,937.3965,1037.4195},
	{-1369.4181,926.0510,1037.3500},
	{-1373.5752,926.0424,1037.3471}
};

new gDerbyWeapons[][WeaponsForDerby] = {
	{WEAPON_UZI,500},
	{WEAPON_MP5,250},
	{WEAPON_TEC9,500}
};

new gDerbyBarrelsInventory[DERBY_BARRELS];

forward StartDerby();
forward JoinDerby(playerid);
forward CreateDerbyBarrels();
forward ForceEndDerby();
forward PutPlayerInDerbyCar(playerid);
forward DerbyMoveCheck();
forward CheckPlayerDerbyPause();

forward Derby_OnGameModeInit();
forward Derby_OnPlayerConnect(playerid);
forward Derby_OnPlayerDisconnect(playerid,reason);
forward Derby_OnPlayerDeath(playerid,killerid,reason);
forward Derby_OnVehicleDeath(vehicleid,killerid);
forward Derby_OnPlayerStateChange(playerid, newstate, oldstate);
forward Derby_OnPlayerCommandText(playerid, text[]);
forward Derby_OnPlayerEnterCheckpoint(playerid);

new TimerDerbyBarrels;
new TimerDerbyForceEnd;
new TimerDerbyMoveCheck;

public Derby_OnGameModeInit()
{
	DerbyConfig();
   	CreateStreamObject(13604,-1396.62,988.36,1024.10,0.00,0.00,-3.00,-1); // Ramp in bloodbowl derby
	CreateStreamObject(971,-1460.10,939.70,1035.68,0.00,0.00,-17.59,-1); // Gate at bloodbowl derby
	CreateStreamObject(971,-1441.57,935.68,1035.77,0.00,0.00,-10.00,-1); // Gate at bloodbowl derby
	CreateStreamObject(971,-1422.45,934.06,1035.64,0.00,0.00,-1.88,-1); // Gate at bloodbowl derby

	for(new x=0; x<DERBY_BARRELS; x++) {
		gDerbyBarrelsInventory[x] = CreateStreamObject(1225,gDerbyBarrels[x][Coord_X],gDerbyBarrels[x][Coord_Y],gDerbyBarrels[x][Coord_Z],0.0,0.0,0.0,-1);
	}

	TimerDerbyBarrels = SetTimer("CreateDerbyBarrels",5000,1);
	TimerDerbyForceEnd = SetTimer("ForceEndDerby",60000,1);
	TimerDerbyMoveCheck = SetTimer("DerbyMoveCheck",1500,1);
	
	for(new i=0; i<DERBY_MAX_PLAYERS; i++) {
		gDerbyPlayerCarSpot[i] = -1;
	}

	if (Derby_Give_Weapon == 1) SetInteriorWeapons(INTERIOR_DERBY,true);
	WriteLog("Derby system loaded!");

	return 0;
}

DerbyConfig()
{
	if (!db_Exists(ConfigDB)) db_Create(ConfigDB);

	new temp[MAX_STRING];

	set(temp,db_Get(ConfigDB,"Derby_Wait_Time"));
	if (strlen(temp) > 0) Derby_Wait_Time = strval(temp); 
	else { valstr(temp,Derby_Wait_Time); db_Set(ConfigDB,"Derby_Wait_Time",temp); }
	set(temp,nullstr);

	set(temp,db_Get(ConfigDB,"Derby_Entry_Fee"));
	if (strlen(temp) > 0) Derby_Entry_Fee = strval(temp); 
	else { valstr(temp,Derby_Entry_Fee); db_Set(ConfigDB,"Derby_Entry_Fee",temp); }
	set(temp,nullstr);

	set(temp,db_Get(ConfigDB,"Derby_Forced_Length"));
	if (strlen(temp) > 0) Derby_Forced_Length = strval(temp); 
	else { valstr(temp,Derby_Forced_Length); db_Set(ConfigDB,"Derby_Forced_Length",temp); }
	set(temp,nullstr);

	set(temp,db_Get(ConfigDB,"Derby_Pot_Increase_Per_Player"));
	if (strlen(temp) > 0) Derby_Pot_Increase_Per_Player = strval(temp); 
	else { valstr(temp,Derby_Pot_Increase_Per_Player); db_Set(ConfigDB,"Derby_Pot_Increase_Per_Player",temp); }
	set(temp,nullstr);

	set(temp,db_Get(ConfigDB,"Derby_Give_Weapon"));
	if (strlen(temp) > 0) Derby_Give_Weapon = strval(temp); 
	else { valstr(temp,Derby_Give_Weapon); db_Set(ConfigDB,"Derby_Give_Weapon",temp); }
	set(temp,nullstr);

	set(temp,db_Get(ConfigDB,"Derby_Min_Players"));
	if (strlen(temp) > 0) Derby_Min_Players = strval(temp); 
	else { valstr(temp,Derby_Min_Players); db_Set(ConfigDB,"Derby_Min_Players",temp); }
	set(temp,nullstr);

	if (Derby_Min_Players < 2) Derby_Min_Players = 2;
}

public Derby_OnPlayerConnect(playerid)
{
	gDerbyPlayerCar[playerid] = -1;
	gDerbyLackOfMove[playerid] = 0;
	gDerbyLackOfMove2[playerid] = 0;
	gDerbyCarHealth[playerid] = 0;
	gWaitingDerby[playerid] = 0;
	
	return 0;
}
public Derby_OnPlayerDisconnect(playerid,reason)
{
	#pragma unused reason
	if(gDerbyPlayerCar[playerid] != -1) {
		LeaveDerby(playerid);
	}
	return 0;
}

public Derby_OnPlayerDeath(playerid,killerid,reason)
{
	#pragma unused killerid
	#pragma unused reason
	if(gDerbyPlayerCar[playerid] != -1) {
		LeaveDerby(playerid);
	}
	return 0;
}

public Derby_OnVehicleDeath(vehicleid,killerid)
{
	#pragma unused killerid
	for(new x=0; x<MAX_PLAYERS_EX; x++) {
	    	if(IsPlayerConnected(x)){
	    		if(gDerbyPlayerCar[x] == vehicleid && gDerbyInProgress) {
				LeaveDerby(x);
			}
		}
	}
	return 0;
}

public Derby_OnPlayerStateChange(playerid, newstate, oldstate)
{
	if(newstate == PLAYER_STATE_ONFOOT && oldstate == PLAYER_STATE_DRIVER) {
		if(gDerbyPlayerCar[playerid] != -1) {
			oPutPlayerInVehicle(playerid,gDerbyPlayerCar[playerid],0);
	    		SystemMsg(playerid,COLOUR_DERBY,gettext(1614));
   		}
	}
	return 0;
}

public Derby_OnPlayerEnterCheckpoint(playerid)
{
	if (playerid == INVALID_PLAYER_ID) return 1;
	if (!IsPlayerRegistered(playerid)) return 0;
	if (PlayerQuest[playerid] != 0) return 0;
	if (IsPlayerAtStadiumEntrance(playerid)){
		SystemMsg(playerid,COLOUR_INFO,gettext(1613));
	}
	return 0;
}

public Derby_OnPlayerCommandText(playerid, text[])
{
	if (!IsPlayerRegistered(playerid)) return 0;
	
    	dcmd(derby,5,text);
	dcmd(derbyplayers,12,text);
	
	return 0;
}

dcmd_derby(playerid,params[]){
	#pragma unused params
    	if(gDerbyPlayerCar[playerid] == -1) {
		if(gDerbyInProgress) {
			SystemMsg(playerid,COLOUR_ERROR,gettext(1615));
			TogglePlayerControllable(playerid,1);
			return 1;
		}

		else if(oGetPlayerMoney(playerid) < Derby_Entry_Fee) {
		    	new string[MAX_STRING];
			format(string,MAX_STRING,gettext(1616),Derby_Entry_Fee);
		    	SystemMsg(playerid,COLOUR_ERROR,string);
		    	TogglePlayerControllable(playerid,1);
			return 1;
		}

		else if(IsPlayerInAnyVehicle(playerid)) {
		    	SystemMsg(playerid,COLOUR_ERROR,gettext(1617));
		    	TogglePlayerControllable(playerid,1);
		    	return 1;
		}

		else if(gWaitingDerby[playerid]) {
			SystemMsg(playerid,COLOUR_ERROR,gettext(1618));
			return 1;
		}

		else if(IsPlayerAtStadiumEntrance(playerid)) {
			JoinDerby(playerid);
		}

		else if(GetDerbyCount() > 0) {
		    	if(Derby_Wait_Time-gStartDerbyCount <= 10) {
	       	    		SystemMsg(playerid, COLOUR_ERROR,gettext(1619));
	               		return 1;
			}
			SystemMsg(playerid, COLOUR_DERBY,gettext(1620));
			SetTimerEx("JoinDerby",10000,0,"i",playerid);
			gWaitingDerby[playerid] = 1;
		} else {
			SystemMsg(playerid,COLOUR_DERBY,gettext(157));
  		}
		
	} else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(1623));
	}
	return 1;
}
	
dcmd_derbyplayers(playerid,params[]){
	#pragma unused params
	new string[MAX_STRING], Float:vhp, thisline = 0, derbyers = 0;
	string = gettext(1624);

	for(new i=0; i<MAX_PLAYERS_EX; i++) {
		if(IsPlayerConnected(i)) {
			if(gDerbyPlayerCar[i] != -1) {
				GetVehicleHealth(gDerbyPlayerCar[i], vhp);
    				format(string,MAX_STRING,gettext(1625),string,oGetPlayerName(i),vhp);
     				derbyers++;
   				thisline++;

				if(thisline == 5) {
					SystemMsg(playerid, COLOUR_DERBY,string);
					string = gettext(1626);
      					thisline = 0;
     				}
    			}
		}
	}
	if(thisline >= 1) { SystemMsg(playerid, COLOUR_DERBY,string); }
  	else if(derbyers == 0) { SystemMsg(playerid, COLOUR_DERBY,gettext(1627)); }

	return 1;
}

public CreateDerbyBarrels()
{
	for(new x=0; x<DERBY_BARRELS; x++) {
		DestroyStreamObject(gDerbyBarrelsInventory[x]);
 		gDerbyBarrelsInventory[x] = CreateStreamObject(1225,gDerbyBarrels[x][Coord_X],gDerbyBarrels[x][Coord_Y],gDerbyBarrels[x][Coord_Z],0.0,0.0,0.0,-1);
	}
}

public JoinDerby(playerid)
{
	gWaitingDerby[playerid] = 0;

	new count = GetDerbyCount();

	WhereHeEnter[playerid][Coord_X] = PlayerPos[playerid][Coord_X];
	WhereHeEnter[playerid][Coord_Y] = PlayerPos[playerid][Coord_Y];
	WhereHeEnter[playerid][Coord_Z] = PlayerPos[playerid][Coord_Z];
		
	new spot;
	for(new x=0; x<DERBY_MAX_PLAYERS; x++) {
		if(gDerbyPlayerCarSpot[x] == -1) {
			spot = x;
			gDerbyPlayerCarSpot[x] = playerid;
			break;
		}
	}
	gDerbyPlayerCar[playerid] = CreateVehicle(504,gDerbyCars[spot][X],gDerbyCars[spot][Y],gDerbyCars[spot][Z],gDerbyCars[spot][ROT],-1,-1,9999);
	LinkVehicleToInterior(gDerbyPlayerCar[playerid],INTERIOR_DERBY);
  	//SetPlayerInterior(playerid,INTERIOR_DERBY);
	//oPutPlayerInVehicle(playerid,gDerbyPlayerCar[playerid],0);
	SetTimerEx("PutPlayerInDerbyCar",500,0,"i",playerid);
	TogglePlayerControllable(playerid,0);
   	//SetCameraBehindPlayer(playerid);

    	oGivePlayerMoney(playerid,0-Derby_Entry_Fee,true);
    	gDerbyPrizePot += Derby_Pot_Increase_Per_Player;
     	
    	SystemMsg(playerid,COLOUR_DERBY,gettext(1628));
    	SystemMsg(playerid,COLOUR_DERBY,gettext(1629));

	new logstring[MAX_STRING];
	format(logstring,MAX_STRING,"%s joined the derby",oGetPlayerName(playerid));
	WriteLog(logstring);
     	
	if(count == 0) {
		gStartDerbyCount = 0;
	   	StartDerby();
 	}
	return 1;
}

public StartDerby()
{
	new countdown, string[MAX_STRING];
	if(gStartDerbyCount == 0) {
	    	gStartDerbyTimer = SetTimer("StartDerby",1000,1);
	    	for(new x=0; x<MAX_PLAYERS_EX; x++) {
	    		if(IsPlayerConnected(x)){
	    			if(gDerbyPlayerCar[x] == -1) {
					format(string,MAX_STRING,gettext(1630),Derby_Wait_Time);
	    				ShowTextForPlayer(x,string,5000,3);
	    				SystemMsg(x,COLOUR_DERBY,gettext(1631));
				}
			}
		}
	}
	gStartDerbyCount++;
	
	if(gStartDerbyCount != Derby_Wait_Time) {
		for(new x=0; x<MAX_PLAYERS_EX; x++) {
		    	if(IsPlayerConnected(x)){
		    		if(gDerbyPlayerCar[x] != -1) {
					countdown = (Derby_Wait_Time-gStartDerbyCount);
					format(string, sizeof(string), "%d",countdown);
					PlayerPlaySoundOnPlayer(x,1056);
					ShowTextForPlayer(x, string,1000,6);
 				}
				else if(Derby_Wait_Time-gStartDerbyCount == 30) {
					ShowTextForPlayer(x,gettext(1633),5000,3);
				}
			}
		}
	}
	else if(gStartDerbyCount == Derby_Wait_Time) {
 		if(GetDerbyCount() < Derby_Min_Players) {
		     	gDerbyPrizePot = 0;
		    	for(new x=0; x<MAX_PLAYERS_EX; x++) {
				if(IsPlayerConnected(x)){
		        			if(gDerbyPlayerCar[x] != -1) {
						SetTimerEx("SpawnDerbyPlayer",4000,0,"i",x);
	           				SetTimerEx("DestroyDerbyVehicle",5000,0,"i",gDerbyPlayerCar[x]);
	           		
	        	    			gDerbyPlayerCar[x] = -1;
	              				for(new i=0; i<DERBY_MAX_PLAYERS; i++) {
							if(gDerbyPlayerCarSpot[i] == x) {
								gDerbyPlayerCarSpot[i] = -1;
								break;
							}
						}
	        	    			oGivePlayerMoney(x,Derby_Entry_Fee,true);
	        	    			TogglePlayerControllable(x,1);
						RemovePlayerFromVehicle(x);
						SetPlayerInterior(x,INTERIOR_DEFAULT);
						oSetPlayerPos(x,WhereHeEnter[x][Coord_X],WhereHeEnter[x][Coord_Y],WhereHeEnter[x][Coord_Z]);
	           				SystemMsg(x,COLOUR_DERBY,gettext(1634));
						GivePlayerOwnedWeapons(x);
					}
				}
			}
			WriteLog("No players joined - derby ended");
		} else {
	    	for(new x=0; x<MAX_PLAYERS_EX; x++) {
			if(IsPlayerConnected(x)){
	        		if(gDerbyPlayerCar[x] != -1) {
	        	    		if(!IsPlayerInVehicle(x,gDerbyPlayerCar[x])) {
						LeaveDerby(x);
						SystemMsg(x,COLOUR_ERROR,gettext(1635));
						oGivePlayerMoney(x,Derby_Entry_Fee,true);
						continue;
					}
		    			TogglePlayerControllable(x,1);
		    			format(string,MAX_STRING,gettext(1636),GetDerbyCount(),gDerbyPrizePot);
		    			SystemMsg(x,COLOUR_DERBY,string);
		    			ShowTextForPlayer(x,gettext(516),3000,3);
		    			gForceEndDerby = 0;
		    			gDerbyInProgress = 1;

		    			
					}
				}
			}
			WriteLog("Derby started");
			new logstring[MAX_STRING];
			format(logstring,MAX_STRING,"Derby info: players: %d - money pot: $%d",GetDerbyCount(),gDerbyPrizePot);
			WriteLog(logstring);
		}
		KillTimer(gStartDerbyTimer);
	}
	return 1;
}

stock LeaveDerby(playerid)
{
	SetTimerEx("DestroyDerbyVehicle",3000,0,"i",gDerbyPlayerCar[playerid]);
	gDerbyPlayerCar[playerid] = -1;
	gDerbyLackOfMove2[playerid] = 0;
	gDerbyLackOfMove[playerid] = 0;
	gDerbyCarHealth[playerid] = 0;

    	if(GetDerbyCount() == 0 && !gDerbyInProgress) { KillTimer(gStartDerbyTimer); }

	if(gDerbyInProgress) {
	    if(GetDerbyCount() == 1) {
			EndDerby();
		}
		else {
			ShowTextForPlayer(playerid,gettext(1638),5000,3);
		}
	}
	for(new x=0; x<DERBY_MAX_PLAYERS; x++) {
		if(gDerbyPlayerCarSpot[x] == playerid) {
			gDerbyPlayerCarSpot[x] = -1;
			break;
		}
	}
	SetPlayerInterior(playerid,INTERIOR_DEFAULT);
	oSetPlayerPos(playerid,WhereHeEnter[playerid][Coord_X],WhereHeEnter[playerid][Coord_Y],WhereHeEnter[playerid][Coord_Z]);
	GivePlayerOwnedWeapons(playerid);
	new string[MAX_STRING];
	format(string,MAX_STRING,gettext(1639),oGetPlayerName(playerid),GetDerbyCount());
 	for(new x=0; x<MAX_PLAYERS_EX; x++) {
	   	if(IsPlayerConnected(x)){
        		if(gDerbyPlayerCar[x] != -1) {
	        		SystemMsg(x,COLOUR_DERBY,string);
			}
		}
	}
	new logstring[MAX_STRING];
	format(logstring,MAX_STRING,"%s's car blew up in derby - %d players left",oGetPlayerName(playerid),GetDerbyCount());
	WriteLog(logstring);
	return 1;
}

stock EndDerby()
{
	gForceEndDerby = -1;

    	for(new x=0; x<MAX_PLAYERS_EX; x++) {
		if (IsPlayerConnected(x)){
        		if(gDerbyPlayerCar[x] != -1) {

            		new string[MAX_STRING];
            		format(string,MAX_STRING,gettext(1640),oGetPlayerName(x));
			for (new playerid=0;playerid<MAX_PLAYERS_EX;playerid++){
        			if (IsPlayerConnected(playerid)){
        	    			ShowTextForPlayer(playerid,string,5000,3);
        			}
			}
			RemovePlayerFromVehicle(x);
			SetPlayerInterior(x,INTERIOR_DEFAULT);
			oSetPlayerPos(x,WhereHeEnter[x][Coord_X],WhereHeEnter[x][Coord_Y],WhereHeEnter[x][Coord_Z]);
			GivePlayerOwnedWeapons(x);
			oSetVehicleParamsForPlayer(gDerbyPlayerCar[x],x,0,1);
            		SetTimerEx("SpawnDerbyPlayer",4000,0,"i",x);
            		SetTimerEx("DestroyDerbyVehicle",5000,0,"i",gDerbyPlayerCar[x]);
            		oGivePlayerMoney(x,gDerbyPrizePot,true);

			gDerbyPrizePot = 0;
   	        	gDerbyPlayerCar[x] = -1;
  			gDerbyLackOfMove2[x] = 0;
			gDerbyLackOfMove[x] = 0;
			gDerbyCarHealth[x] = 0;
   	        
           		for(new i=0; i<DERBY_MAX_PLAYERS; i++) {
				if(gDerbyPlayerCarSpot[i] == x) {
					gDerbyPlayerCarSpot[i] = -1;
					break;
				}
			}
		}
	}
	}
	gDerbyInProgress = 0;
	//KillTimer(gShowLeaderTimer);
	
	return 1;
}

public ForceEndDerby()
{
    if(gForceEndDerby == -1) { return 1; }

	gForceEndDerby += 60;
    	if(gForceEndDerby != Derby_Forced_Length) { return 1; }

	new Float:vhp, Float:DerbyWinner, DerbyWinnerID;

	for(new x=0; x<=MAX_PLAYERS_EX; x++) {
    		if(IsPlayerConnected(x)) {
        		if(gDerbyPlayerCar[x] != -1) {
				GetVehicleHealth(gDerbyPlayerCar[x], vhp);
   				if (vhp > DerbyWinner)
	    			{
	        			DerbyWinner = vhp;
	        			DerbyWinnerID = x;
				}
			}
		}
	}
	new string[MAX_STRING];
	format(string,MAX_STRING,gettext(1641),oGetPlayerName(DerbyWinnerID), DerbyWinnerID);

	for(new x=0; x<=MAX_PLAYERS_EX; x++) {
    		if(IsPlayerConnected(x)){
        		if(gDerbyPlayerCar[x] != -1 && DerbyWinnerID != x) {
				LeaveDerby(x);
				SystemMsg(x,COLOUR_DERBY, string);
			}
		}
	}
	new logstring[MAX_STRING];
	format(logstring,MAX_STRING,"Derby's time up. %s won with the least damaged car",oGetPlayerName(DerbyWinnerID), DerbyWinnerID);
	WriteLog(logstring);
	return 1;
}

public PutPlayerInDerbyCar(playerid)
{
	ResetPlayerWeapons(playerid);
	if (Derby_Give_Weapon == 1) GivePlayerRandomDerbyWeapon(playerid);
	new Float:x,Float:y,Float:z;
	GetVehiclePos(gDerbyPlayerCar[playerid],x,y,z);
	oSetPlayerPos(playerid,x,y,z+1.0);
    	SetPlayerInterior(playerid,INTERIOR_DERBY);
	oPutPlayerInVehicle(playerid,gDerbyPlayerCar[playerid],0);
	SetCameraBehindPlayer(playerid);
	return 1;
}

stock GetDerbyCount()
{
	new count;
	for(new x=0; x<MAX_PLAYERS_EX; x++) {
	    	if(IsPlayerConnected(x)){
	    		if(gDerbyPlayerCar[x] != -1) {
	        		count++;
			}
		}
	}
	return count;
}

public DerbyMoveCheck()
{
	if(!gDerbyInProgress) { return 1; }
	
	new Float:vhp;
	new Float:distance, string[MAX_STRING];
	
	for(new i=0; i<MAX_PLAYERS_EX; i++) {
	    	if(IsPlayerConnected(i) && gDerbyPlayerCar[i] != -1) {
	        	GetVehicleHealth(gDerbyPlayerCar[i],vhp);
	        	if(vhp > 300) {

           			distance = floatsqroot(floatpower(floatabs(floatsub(PlayerPos[i][Coord_X],gDerbyMovePrevPos[i][PrevX])),2) + floatpower(floatabs(floatsub(PlayerPos[i][Coord_Y],gDerbyMovePrevPos[i][PrevY])),2) + floatpower(floatabs(floatsub(PlayerPos[i][Coord_Z],gDerbyMovePrevPos[i][PrevZ])),2));

				gDerbyMovePrevPos[i][PrevX] = PlayerPos[i][Coord_X];
				gDerbyMovePrevPos[i][PrevY] = PlayerPos[i][Coord_Y];
				gDerbyMovePrevPos[i][PrevZ] = PlayerPos[i][Coord_Z];

				if(distance < DERBY_MIN_MOVE) {
       					gDerbyLackOfMove[i]++;
            				if(gDerbyLackOfMove[i] == DERBY_MAX_LACK_OF_MOVE) {
                				gDerbyLackOfMove[i] = 0;
                				gDerbyLackOfMove2[i]++;
                				format(string,MAX_STRING,gettext(1642),gDerbyLackOfMove2[i],DERBY_MAX_LACK_OF_MOVE2);
                				SystemMsg(i,COLOUR_DERBY,string);
                				if(gDerbyLackOfMove2[i] == DERBY_MAX_LACK_OF_MOVE2) {
                					SystemMsg(i,COLOUR_ERROR,gettext(1643));
                					gDerbyLackOfMove2[i] = 0;
                					LeaveDerby(i);
                				}
					}
				}
			}
		}
	}
	return 1;
}

forward SpawnDerbyPlayer(playerid);
public SpawnDerbyPlayer(playerid){
	if (PlayerSpawned[playerid] == false) SpawnPlayer(playerid);
	return 1;
}

forward DestroyDerbyVehicle(vehicleid);
public DestroyDerbyVehicle(vehicleid){
	DestroyVehicle(vehicleid);
	return 1;
}

GivePlayerRandomDerbyWeapon(playerid){
	new weapon = random(sizeof(gDerbyWeapons));
	GivePlayerWeapon(playerid,gDerbyWeapons[weapon][Weaponid],gDerbyWeapons[weapon][Bullets]);
}