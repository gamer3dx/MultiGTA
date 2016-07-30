//Taxi Job module
//Created by BetaMaster (SA~DMA+LG+PB+SR)
//Converted by MadCat

#include "base"

#define TAXI_REGFEE 11500
#define TAXI_PICKUPFEE 5
#define MAX_TAXI_METER_FEE 100
#define MIN_TAXI_METER_FEE 5
#define TAXI_BONUS_PASSENGER_COUNT 5
#define TAXI_BONUS_CASH 1000
#define TAXI_DRIVER 0
#define TAXI_PASSENGER 1

enum TAXI_MAIN{
	TAXI_STATUS,
	TAXI_VEHICLE,
	Float:TAXI_POS[CoordInfo],
	TAXI_METER,
	TAXI_METER_FEE,
	TAXI_PASSENGERS
};

new gPlayerTaxi[MAX_PLAYERS][TAXI_MAIN];

new TaxiOneSecondCount = 0;

new TimerTaxi;

forward Taxi_OnGameModeInit();
forward Taxi_OnPlayerCommandText(playerid,text[]);
forward TaxiKill(killerid, victimid, reason);
forward Taxi_OnPlayerStateChange(playerid, newstate, oldstate);
forward Taxi_OnPlayerDisconnect(playerid);
forward Taxi_OnPlayerDeath(playerid, killerid, reason);
forward Taxi_OnPlayerExitVehicle(playerid,vehicleid);
forward Taxi_OnPlayerSpawn(playerid);

public Taxi_OnGameModeInit()
{	Debug("jobs/taxi.inc > Taxi_OnGameModeInit - Start");
	for(new i=0; i<MAX_PLAYERS_EX; i++) {
		Taxi_Initialise(i);
	}
	TimerTaxi = SetTimer("TaxiTimer", 1000, 1);
	WriteLog("Taxi job Loaded");
	Debug("jobs/taxi.inc > Taxi_OnGameModeInit - Stop");
	return 0;
}

public Taxi_OnPlayerCommandText(playerid,text[])
{
	if (!IsPlayerConnected(playerid)) return 0;

	if (!IsPlayerRegistered(playerid)) return 0;

	dcmd(taxi,4,text);
	dcmd(taxifee,7,text);

	return 0;	
}

dcmd_taxi(playerid,params[]){
	#pragma unused params
	Debug("jobs/taxi.inc  > Command 'taxi' - Start");
	Taxi_Driver(playerid);
	Debug("jobs/taxi.inc  > Command 'taxi' - Stop");
	return 1;
}

dcmd_taxifee(playerid,params[]){
	#pragma unused params
	Debug("jobs/taxi.inc  > Command 'taxifee' - Start");
	Taxi_SetFee(playerid,params);
	Debug("jobs/taxi.inc  > Command 'taxifee' - Stop");
	return 1;
}

public Taxi_OnPlayerDisconnect(playerid) {
	if(gPlayerTaxi[playerid][TAXI_STATUS]!=-1)  Taxi_Leave(playerid);
	return 0;
}

public Taxi_OnPlayerDeath(playerid, killerid, reason) {
	if (IsPlayerInDuel(playerid)) return 0;
	#pragma unused killerid
	#pragma unused reason
	if(gPlayerTaxi[playerid][TAXI_STATUS]!=-1) {
		Taxi_Leave(playerid);
	}
	if ((Player[playerid][Money] - 100) > 0)
	{
		if (Drop_Money_On_Death == 1) DropDeathMoney(playerid,100);
		oGivePlayerMoney(playerid,-100,0);
	}
	else
	{
		oSetPlayerMoney(playerid,0);
	}
	if (Player[playerid][XP] - (Player[playerid][XP] /200) > 0)
	{
		GivePlayerXP(playerid,-(Player[playerid][XP] /200),1); // take death penalty xp
	}
	else
	{
		Player[playerid][XP] = 0;
		SetPlayerScore(playerid,Player[playerid][XP]);
	}
	PlayerSpawn[playerid][Coord_X] = 0.0;
	PlayerSpawn[playerid][Coord_Y] = 0.0;
	PlayerSpawn[playerid][Coord_Z] = 0.0;
	if (GetPlayerRace(playerid) != 0){
		LeaveRace(GetPlayerRace(playerid),playerid);
	}	
	return 0;
}

public Taxi_OnPlayerSpawn(playerid){
	Taxi_Reset(playerid);
}

public Taxi_OnPlayerStateChange(playerid, newstate, oldstate) {
	if(newstate==PLAYER_STATE_DRIVER){
		if (Player[playerid][GotJob] == JOB_TAXIDRIVER){
			new vehicle=GetPlayerVehicleID(playerid);
			if (GetVehicleModel(vehicle) == V_TAXI || GetVehicleModel(vehicle) == V_CABBIE){
				gPlayerTaxi[playerid][TAXI_STATUS] = TAXI_DRIVER;
				gPlayerTaxi[playerid][TAXI_VEHICLE] = vehicle;
				gPlayerTaxi[playerid][TAXI_METER] = 0;
				gPlayerTaxi[playerid][TAXI_PASSENGERS] = 0;
				SetSpecialColorForPlayer(playerid);
				new string[MAX_STRING];
				format(string,MAX_STRING,gettext(1414),oGetPlayerName(playerid));
				SystemMsgToRegistered(COLOUR_TAXI,string);
			}
		}
	}
	if(newstate==PLAYER_STATE_PASSENGER) {
		new vehicle = GetPlayerVehicleID(playerid);
		for(new i=0; i<MAX_PLAYERS_EX; i++) {
			if(gPlayerTaxi[i][TAXI_VEHICLE]==vehicle) {
				Taxi_Enter(playerid, i);
				break;
			}
		}
	}
	return 0;
}

public Taxi_OnPlayerExitVehicle(playerid, vehicleid) {
	#pragma unused vehicleid
	if(gPlayerTaxi[playerid][TAXI_STATUS]!=-1) Taxi_Leave(playerid);
	return 1;
}


public TaxiKill(killerid, victimid, reason){
Debug("jobs/taxi.inc  > TaxiKill - Start");
if (IsPlayerInAnyVehicle(killerid)){
	SystemMsg(killerid,COLOUR_ERROR,gettext(1375));
	Taxi_Leave(killerid);
	Taxi_Reset(killerid);
	Player[killerid][GotJob] = JOB_NOJOB;
	new string[MAX_STRING];
	SetSpecialColorForPlayer(killerid);
	format(string,MAX_STRING,gettext(1376),oGetPlayerName(killerid));
	SystemMsgToRegistered(COLOUR_TAXI,string);
	new logstring[256];
	format(logstring, sizeof (logstring), "player: %d:  %s: civilian kill as a taxi driver",killerid,oGetPlayerName(killerid));
	WriteLog(logstring);
}
Debug("jobs/taxi.inc  > TaxiKill - Stop");
return 1;
}

stock SendMsgToTaxi(msg[]){
	Debug("jobs/taxi.inc  > SendMsgToTaxi - Start");
	for(new i = 0; i <= MAX_PLAYERS_EX; i++)
	{
		if (IsPlayerConnected(i)){
			if (Player[i][GotJob] == JOB_TAXIDRIVER){
				SystemMsg(i,COLOUR_TAXI,msg);
			}
		}
	}
	Debug("jobs/taxi.inc  > SendMsgToTaxi - Stop");
}

forward Taxi_Initialise(playerid);
public Taxi_Initialise(playerid) {
	Taxi_Reset(playerid);
	gPlayerTaxi[playerid][TAXI_METER_FEE] = MIN_TAXI_METER_FEE;
}

forward Taxi_Reset(playerid);

public Taxi_Reset(playerid) {
	gPlayerTaxi[playerid][TAXI_STATUS] = -1;
	gPlayerTaxi[playerid][TAXI_VEHICLE] = -1;
	gPlayerTaxi[playerid][TAXI_METER] = 0;
	gPlayerTaxi[playerid][TAXI_PASSENGERS] = 0;
}

forward Taxi_Driver(playerid);

public Taxi_Driver(playerid) {
	if(Player[playerid][GotJob] == JOB_TAXIDRIVER) {
		Taxi_Leave(playerid);
		Player[playerid][GotJob] = JOB_NOJOB;
	} else if (Player[playerid][GotJob] != JOB_NOJOB){
		SystemMsg(playerid,COLOUR_ERROR,gettext(1415));
	} else if(GetPlayerState(playerid)==PLAYER_STATE_DRIVER) {
		new vehicle = GetPlayerVehicleID(playerid), string[MAX_STRING];
		for(new i=0; i<MAX_PLAYERS_EX; i++) {
			if(i!=playerid && gPlayerTaxi[i][TAXI_VEHICLE]==vehicle) {
				SystemMsg(playerid,COLOUR_ERROR,gettext(1416));
				return 1;
			}
		}
		if(GetVehicleModel(vehicle) == V_CABBIE || GetVehicleModel(vehicle) == V_TAXI) {
			if(oGetPlayerMoney(playerid)>=TAXI_REGFEE) {
				format(string, MAX_STRING,gettext(1417), TAXI_REGFEE);
				SystemMsg(playerid,COLOUR_TAXI,string);
				oGivePlayerMoney(playerid, -TAXI_REGFEE,false);
			} else {
				SystemMsg(playerid,COLOUR_ERROR,gettext(1418));
				SystemMsg(playerid,COLOUR_ERROR,gettext(1419));
				format(string, MAX_STRING, gettext(1420), TAXI_REGFEE);
				SystemMsg(playerid,COLOUR_ERROR,string);
				return 1;
			}
		}
		SystemMsg(playerid,COLOUR_TAXI,gettext(1421));
		SystemMsg(playerid,COLOUR_TAXI,gettext(1422));
		SystemMsg(playerid,COLOUR_TAXI,gettext(1423));
		format(string, MAX_STRING,gettext(1424), TAXI_BONUS_PASSENGER_COUNT, TAXI_BONUS_CASH);
		SystemMsg(playerid,COLOUR_TAXI,string);
		gPlayerTaxi[playerid][TAXI_STATUS] = TAXI_DRIVER;
		gPlayerTaxi[playerid][TAXI_VEHICLE] = vehicle;
		gPlayerTaxi[playerid][TAXI_METER] = 0;
		gPlayerTaxi[playerid][TAXI_PASSENGERS] = 0;
		Player[playerid][GotJob] = JOB_TAXIDRIVER;
		SetSpecialColorForPlayer(playerid);
		format(string,MAX_STRING,gettext(1425),oGetPlayerName(playerid));
		SystemMsgToRegistered(COLOUR_TAXI,string);
	} else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(1426));
	}
	return 1;
}

forward Taxi_Enter(playerid, taxidriver);

public Taxi_Enter(playerid, taxidriver) {
	new string[MAX_STRING];
	gPlayerTaxi[playerid][TAXI_STATUS] = TAXI_PASSENGER;
	gPlayerTaxi[playerid][TAXI_VEHICLE] = gPlayerTaxi[taxidriver][TAXI_VEHICLE];
	SetSpecialColorForPlayer(playerid);
	gPlayerTaxi[taxidriver][TAXI_PASSENGERS]++;
	if(gPlayerTaxi[taxidriver][TAXI_PASSENGERS]>1) {
		format(string, MAX_STRING, gettext(1427), gPlayerTaxi[taxidriver][TAXI_METER]);
		SystemMsg(playerid,COLOUR_TAXI,string);
		SystemMsg(playerid,COLOUR_TAXI,gettext(1428));
	} else {
		gPlayerTaxi[taxidriver][TAXI_METER] = TAXI_PICKUPFEE;
		gPlayerTaxi[taxidriver][TAXI_POS][Coord_X] = PlayerPos[playerid][Coord_X];
		gPlayerTaxi[taxidriver][TAXI_POS][Coord_Y] = PlayerPos[playerid][Coord_Y];
		gPlayerTaxi[taxidriver][TAXI_POS][Coord_Z] = PlayerPos[playerid][Coord_Z];
		format(string, MAX_STRING, gettext(1429), TAXI_PICKUPFEE);
		SystemMsg(playerid,COLOUR_TAXI,string);
		ShowTextForPlayer(playerid, gettext(1430), 3000, 6);
	}
	Player[playerid][TaxiRides]++;
	ShowTextForPlayer(taxidriver, gettext(1431), 4000, 4);
	PlayerPlaySoundOnPlayer(taxidriver, 1056);
}

forward Taxi_Leave(playerid);

public Taxi_Leave(playerid) {
	new string[MAX_STRING], taxidriver = -1;
	if(gPlayerTaxi[playerid][TAXI_STATUS]==TAXI_DRIVER) {
		if(gPlayerTaxi[playerid][TAXI_PASSENGERS]>0) {
			for(new i=0; i<MAX_PLAYERS_EX; i++) {
				if(IsPlayerConnected(i) && gPlayerTaxi[i][TAXI_STATUS]==TAXI_PASSENGER && gPlayerTaxi[i][TAXI_VEHICLE]==gPlayerTaxi[playerid][TAXI_VEHICLE]) {
					SystemMsg(i,COLOUR_ERROR,gettext(1432));
					gPlayerTaxi[playerid][TAXI_PASSENGERS]--;
					gPlayerTaxi[i][TAXI_STATUS] = -1;
					gPlayerTaxi[i][TAXI_VEHICLE] = -1;
				}
			}
		}
		SystemMsg(playerid,COLOUR_ERROR,gettext(1433));
		SetSpecialColorForPlayer(playerid);
		format(string,MAX_STRING,gettext(1434),oGetPlayerName(playerid));
		SystemMsgToRegistered(COLOUR_TAXI,string);
	} else if(gPlayerTaxi[playerid][TAXI_STATUS]==TAXI_PASSENGER) {
		for(new i=0; i<MAX_PLAYERS_EX; i++) {
			if(IsPlayerConnected(i) && gPlayerTaxi[i][TAXI_STATUS]==TAXI_DRIVER && gPlayerTaxi[i][TAXI_VEHICLE]==gPlayerTaxi[playerid][TAXI_VEHICLE]) {
				taxidriver = i;
				break;
			}
		}
		if(taxidriver!=-1) {
			gPlayerTaxi[taxidriver][TAXI_PASSENGERS]--;
			if(gPlayerTaxi[taxidriver][TAXI_PASSENGERS]==0) {
				if(oGetPlayerMoney(playerid)<gPlayerTaxi[taxidriver][TAXI_METER]) {
					format(string, MAX_STRING, gettext(1435), gPlayerTaxi[taxidriver][TAXI_METER]);
				} else {
					format(string, MAX_STRING, gettext(1436), gPlayerTaxi[taxidriver][TAXI_METER]);
				}
				SystemMsg(playerid,COLOUR_TAXI,string);
				oGivePlayerMoney(playerid, -gPlayerTaxi[taxidriver][TAXI_METER],false);
				format(string, MAX_STRING, gettext(1437), gPlayerTaxi[taxidriver][TAXI_METER]);
				SystemMsg(taxidriver,COLOUR_TAXI,string);
				oGivePlayerMoney(taxidriver, gPlayerTaxi[taxidriver][TAXI_METER],false);
				Player[taxidriver][TaxiPassengers]++;
			}			
		}
		if(!(Player[taxidriver][TaxiPassengers]%TAXI_BONUS_PASSENGER_COUNT)) {
			format(string, MAX_STRING, gettext(1438), TAXI_BONUS_CASH);
			ShowTextForPlayer(taxidriver, string, 4000, 4);
			oGivePlayerMoney(taxidriver, TAXI_BONUS_CASH,false);
			PlayerPlaySoundOnPlayer(taxidriver, 1185);
		}
	}
	Taxi_Reset(playerid);
}

forward Taxi_ShowFee(playerid);

public Taxi_ShowFee(playerid) {
	if(gPlayerTaxi[playerid][TAXI_STATUS]!=TAXI_DRIVER) {
		SystemMsg(playerid,COLOUR_ERROR,gettext(1439));
	} else {
		new string[MAX_STRING];
		format(string, MAX_STRING, gettext(1440), gPlayerTaxi[playerid][TAXI_METER_FEE]);
		SystemMsg(playerid,COLOUR_TAXI,string);
	}
}

forward Taxi_SetFee(playerid, text[]);

public Taxi_SetFee(playerid, text[]) {
	if(!text[0]) {
		Taxi_ShowFee(playerid);
	} else if(gPlayerTaxi[playerid][TAXI_STATUS]!=TAXI_DRIVER) {
		SystemMsg(playerid,COLOUR_ERROR,gettext(1439));
	} else if(gPlayerTaxi[playerid][TAXI_PASSENGERS]>0) {
		SystemMsg(playerid,COLOUR_ERROR,gettext(1441));
	} else {
		new string[MAX_STRING], fee = strval(text);
		if(fee<MIN_TAXI_METER_FEE || fee>MAX_TAXI_METER_FEE) {
			format(string, MAX_STRING, gettext(1442), fee, MIN_TAXI_METER_FEE, MAX_TAXI_METER_FEE);
			SystemMsg(playerid,COLOUR_ERROR,string);
			return 1;
		}
		format(string, MAX_STRING, gettext(1443), oGetPlayerName(playerid), fee);
		SystemMsgToRegistered(COLOUR_TAXI,string);
		gPlayerTaxi[playerid][TAXI_METER_FEE] = fee;
	}
	return 1;
}

forward Taxi_Update(playerid);

public Taxi_Update(playerid) {
	if(gPlayerTaxi[playerid][TAXI_STATUS]!=TAXI_DRIVER || gPlayerTaxi[playerid][TAXI_PASSENGERS]==0) return;
	new updated = 0, Float:distance, Float:pos1[3],string[MAX_STRING];
	pos1[0] = gPlayerTaxi[playerid][TAXI_POS][Coord_X];
	pos1[1] = gPlayerTaxi[playerid][TAXI_POS][Coord_Y];
	pos1[2] = gPlayerTaxi[playerid][TAXI_POS][Coord_Z];
	for(new i=0; i<MAX_PLAYERS_EX; i++) {
		if(IsPlayerConnected(i) && gPlayerTaxi[i][TAXI_STATUS]==TAXI_PASSENGER && gPlayerTaxi[i][TAXI_VEHICLE]==gPlayerTaxi[playerid][TAXI_VEHICLE]) {
			if(!updated) {
				distance = GetPointDistanceToPointEx(gPlayerTaxi[playerid][TAXI_POS][Coord_X],gPlayerTaxi[playerid][TAXI_POS][Coord_Y],gPlayerTaxi[playerid][TAXI_POS][Coord_Z],PlayerPos[i][Coord_X],PlayerPos[i][Coord_Y],PlayerPos[i][Coord_Z]);
				if(distance<250.0) {
					gPlayerTaxi[playerid][TAXI_METER] += gPlayerTaxi[playerid][TAXI_METER_FEE];
				} else {
					gPlayerTaxi[playerid][TAXI_METER] += floatround((gPlayerTaxi[playerid][TAXI_METER_FEE]/4.0)*(distance/250.0));
					gPlayerTaxi[playerid][TAXI_POS][Coord_X] = PlayerPos[i][Coord_X];
					gPlayerTaxi[playerid][TAXI_POS][Coord_Y] = PlayerPos[i][Coord_Y];
					gPlayerTaxi[playerid][TAXI_POS][Coord_Z] = PlayerPos[i][Coord_Z];
				}
				updated = 1;
			}
			format(string, MAX_STRING, gettext(1444), gPlayerTaxi[playerid][TAXI_METER]);
			ShowTextForPlayer(i, string, 3000, 1);
		}
	}
	if(updated) ShowTextForPlayer(playerid, string, 3000, 1);
}

forward TaxiTimer();
public TaxiTimer() {
	TaxiOneSecondCount++, TaxiOneSecondCount%=60;
	for(new i=0; i<MAX_PLAYERS_EX; i++) {
		if(IsPlayerConnected(i)) {
			if(!(TaxiOneSecondCount%20)) Taxi_Update(i);
		}
	}
}

GetPlayerTaxiStatus(playerid){
if (IsPlayerConnected(playerid)){
	return gPlayerTaxi[playerid][TAXI_STATUS];
}
return -1;
}