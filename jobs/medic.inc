//Medic Job module
//Created by MadCat

#include "base"

new medicjobinfoshowed[MAX_PLAYERS] = 0;

#define MedicJobPlaces 8

new MedicJobPlace[MedicJobPlaces][CoordInfo] = {
	{-2696.7810,636.3222,14.4545},
	{1607.8134,1817.1027,10.8203},
	{2033.6117,-1406.0784,17.2177},
	{1177.2189,-1323.8665,14.0599},
	{1241.6986,327.5570,19.7555},
	{-319.8885,1049.3755,20.3403},
	{-1514.9995,2519.9368,55.9958},
	{-2209.5378,-2288.1538,30.6250}
};

#define MedicSpawnsCount 8

new MedicSpawns[MedicSpawnsCount][CoordInfo] = {
	{-2654.7939,638.1785,14.4531},
	{1607.8134,1817.1027,10.8203},
	{2033.6117,-1406.0784,17.2177},
	{1177.2189,-1323.8665,14.0599},
	{1241.6986,327.5570,19.7555},
	{-319.8885,1049.3755,20.3403},
	{-1514.9995,2519.9368,55.9958},
	{-2209.5378,-2288.1538,30.6250}
};

new PlayerWantMedic[MAX_PLAYERS];

forward Medic_OnGameModeInit();
forward IsPlayerAtMedicJobPlace(playerid);
forward MedicJobPickupCheck(playerid);
forward Medic_OnPlayerCommandText(playerid,text[]);
forward MedicKill(killerid, victimid, reason);
forward Medic_OnPlayerDeath(playerid,killerid,reason);
forward Medic_OnPlayerStateChange(playerid, newstate, oldstate);
forward Medic_OnPlayerSpawn(playerid);

public Medic_OnGameModeInit()
{	Debug("jobs/medic.inc > Medic_OnGameModeInit - Start");
	for (new id=0;id<MedicJobPlaces;id++)
	{
		CreateStreamPickup(1248,1,MedicJobPlace[id][Coord_X],MedicJobPlace[id][Coord_Y],MedicJobPlace[id][Coord_Z]);
		CreateStreamPickup(1240,1,MedicJobPlace[id][Coord_X],MedicJobPlace[id][Coord_Y],MedicJobPlace[id][Coord_Z]+0.5);
		CreateStreamMapIcon(22,MedicJobPlace[id][Coord_X],MedicJobPlace[id][Coord_Y],MedicJobPlace[id][Coord_Z]);
		CreateDMapIcon(22,MedicJobPlace[id][Coord_X],MedicJobPlace[id][Coord_Y],MedicJobPlace[id][Coord_Z],gettext(1759),gettext(1760));
	}
	WriteLog("Medic Job checkpoints Loaded");
	Debug("jobs/medic.inc > Medic_OnGameModeInit - Stop");
}

public IsPlayerAtMedicJobPlace(playerid)
{
	Debug("jobs/medic.inc > IsPlayerAtMedicJobPlace - Start");
	for (new id=0;id<MedicJobPlaces;id++)
	{
		if (IsPlayerInRangeOfPoint(playerid,1.0,MedicJobPlace[id][Coord_X],MedicJobPlace[id][Coord_Y],MedicJobPlace[id][Coord_Z]))
		{
			Debug("jobs/medic.inc > IsPlayerAtMedicJobPlace - Stop");
			return 1;
		}
	}
	Debug("jobs/medic.inc > IsPlayerAtMedicJobPlace - Stop");
	return 0;
}

public MedicJobPickupCheck(playerid)
{
	Debug("jobs/medic.inc > MedicJobPickupCheck - Start");
	if (playerid == INVALID_PLAYER_ID) return;
	if (!IsPlayerConnected(playerid)) return;
	if (SpawnTime[playerid] < 5) return;
	if (!IsPlayerAtMedicJobPlace(playerid)) {medicjobinfoshowed[playerid] = 0; return;}
	if (medicjobinfoshowed[playerid] == 1) return;
	medicjobinfoshowed[playerid] = 1;
		if (Player[playerid][GotJob] == JOB_MEDIC){
			SystemMsg(playerid,COLOUR_MEDIC,gettext(634));
		} else {
			SystemMsg(playerid,COLOUR_MEDIC,gettext(635));
		}
	Debug("jobs/medic.inc > MedicJobPickupCheck - Stop");
}

public Medic_OnPlayerCommandText(playerid,text[])
{
	if (!IsPlayerConnected(playerid)) return 0;

	if (!IsPlayerRegistered(playerid)) return 0;

	dcmd(wantmedics,10,text);
	dcmd(nomedic,7,text);
	dcmd(callmedics,10,text);
	dcmd(beamedic,8,text);
	dcmd(heal,4,text);

	return 0;	
}

dcmd_wantmedics(playerid,params[]){
	#pragma unused params
	Debug("jobs/medic.inc  > Command 'wantmedics' - Start");
	if (Player[playerid][GotJob] != JOB_MEDIC){
		SystemMsg(playerid,COLOUR_ERROR,gettext(636));
		Debug("jobs/medic.inc  > Command 'wantmedics' - Stop");
		return 1;
	}
	new temp[MAX_STRING];
	new count = 0;
	for (new i=0;i<MAX_PLAYERS_EX;i++)
	{
		if(IsPlayerConnected(i)){
			if(PlayerWantMedic[i]==1)
			{
				count++;
				format(temp,sizeof(temp),gettext(637),count,oGetPlayerName(i),GetXYZZoneName(PlayerPos[i][Coord_X],PlayerPos[i][Coord_Y],PlayerPos[i][Coord_Z]));
				SystemMsgScrolling(playerid,temp);
				set(temp,nullstr);
			}
		}
	}
	if(count < 1)
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(638));
	}
	Debug("jobs/medic.inc  > Command 'wantmedics' - Stop");
	return 1;
}

dcmd_nomedic(playerid,params[]){
	#pragma unused params
	Debug("jobs/medic.inc  > Command 'nomedic' - Start");
	if(Player[playerid][GotJob] != JOB_MEDIC){
		SystemMsg(playerid,COLOUR_ERROR,gettext(636));
		Debug("jobs/medic.inc  > Command 'nomedic' - Stop");
		return 1;
	}

	if (!IsPlayerAtMedicJobPlace(playerid)){
		SystemMsg(playerid,COLOUR_ERROR,gettext(639));
		Debug("jobs/medic.inc  > Command 'nomedic' - Stop");
		return 1;	
	}
	Player[playerid][GotJob] = JOB_NOJOB;
	SetPlayerSkin(playerid,SKIN_STANDART);
	SystemMsg(playerid,COLOUR_MEDIC,gettext(640));
	new string[MAX_STRING];
	format(string,sizeof(string),gettext(641),oGetPlayerName(playerid));
	SystemMsgToRegistered(COLOUR_MEDIC,string);
	SetSpecialColorForPlayer(playerid);		
	new logstring[256];
	format(logstring, sizeof (logstring), "player: %d:  %s: not a medic now",playerid,oGetPlayerName(playerid));
	WriteLog(logstring);
	Debug("jobs/medic.inc  > Command 'nomedic' - Stop");
	return 1;
}

dcmd_callmedics(playerid,params[]){
	#pragma unused params
	Debug("jobs/medic.inc  > Command 'callmedics' - Start");
	if(Player[playerid][GotJob] == JOB_MEDIC){
		SystemMsg(playerid,COLOUR_ERROR,gettext(642));
		Debug("jobs/medic.inc  > Command 'callmedics' - Stop");
		return 1;
	}

	if (PlayerWantMedic[playerid] == 1){
		SystemMsg(playerid,COLOUR_ERROR,gettext(643));
		Debug("jobs/medic.inc  > Command 'callmedics' - Stop");
		return 1;
	}

	if (oGetPlayerHealth(playerid) >= GetMaxHealth(playerid)){
		SystemMsg(playerid,COLOUR_ERROR,gettext(644));
		Debug("jobs/medic.inc  > Command 'callmedics' - Stop");
		return 1;
	}

	PlayerWantMedic[playerid] = 1;
	SystemMsg(playerid,COLOUR_MEDIC,gettext(645));
	new string[MAX_STRING];
	format(string,sizeof(string),gettext(646),oGetPlayerName(playerid),GetXYZZoneName(PlayerPos[playerid][Coord_X],PlayerPos[playerid][Coord_Y],PlayerPos[playerid][Coord_Z]));
	SendMsgToMedics(string);
	new logstring[256];
	format(logstring, sizeof (logstring), "player: %d:  %s: want medics help!",playerid,oGetPlayerName(playerid));
	WriteLog(logstring);
	Debug("jobs/medic.inc  > Command 'callmedics' - Stop");
	return 1;
}

dcmd_beamedic(playerid,params[]){
	#pragma unused params
	Debug("jobs/medic.inc  > Command 'beamedic' - Start");

	if (!IsPlayerAtMedicJobPlace(playerid)){
			SystemMsg(playerid,COLOUR_ERROR,gettext(639));
			Debug("jobs/medic.inc  > Command 'beamedic' - Stop");
			return 1;	
	}

	if(Player[playerid][GotJob]==JOB_MEDIC)
		{
		SystemMsg(playerid,COLOUR_ERROR,gettext(647));
		Debug("jobs/medic.inc  > Command 'beamedic' - Stop");
		return 1;
	}

	if(Player[playerid][GotJob] != JOB_NOJOB || GetPlayerDM(playerid) || GetPlayerRace(playerid))
		{
		SystemMsg(playerid,COLOUR_ERROR,gettext(648));
		Debug("jobs/medic.inc  > Command 'beamedic' - Stop");
		return 1;
	}
	if(Player[playerid][WantedLevel]>0 || Player[playerid][Crimes]>0)
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(649));
		Debug("jobs/medic.inc  > Command 'beamedic' - Stop");
		return 1;
	}
	Player[playerid][GotJob] = JOB_MEDIC;
	SetPlayerSkin(playerid,SKIN_MEDIC);
	SetPlayerColor(playerid,COLOUR_MEDIC);
	ShowTextForPlayer(playerid,gettext(650),2500,6);
	SystemMsg(playerid,COLOUR_MEDIC,gettext(651));
	SystemMsg(playerid,COLOUR_MEDIC,gettext(652));
	new string[MAX_STRING];
	format(string,sizeof(string),gettext(653),oGetPlayerName(playerid));
	SystemMsgToRegistered(COLOUR_MEDIC,string);
	new logstring[256];
	format(logstring, sizeof (logstring), "player: %d:  %s: is a medic now",playerid,oGetPlayerName(playerid));
	WriteLog(logstring);
	Debug("jobs/medic.inc  > Command 'beamedic' - Stop");
	return 1;
}

dcmd_heal(playerid,params[]){
	#pragma unused params
	Debug("jobs/medic.inc  > Command 'heal' - Start");
	if(Player[playerid][GotJob] != JOB_MEDIC){
		SystemMsg(playerid,COLOUR_ERROR,gettext(636));
		Debug("jobs/medic.inc  > Command 'heal' - Stop");
		return 1;
	}

	if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT){
		SystemMsg(playerid,COLOUR_ERROR,gettext(655));
		Debug("jobs/medic.inc  > Command 'heal' - Stop");
		return 1;
	}

	new closplayer;
	closplayer=GetClosestPlayer(playerid);
	if(closplayer == -1) {
		SystemMsg(playerid,COLOUR_ERROR,gettext(656));
		Debug("jobs/medic.inc  > Command 'heal' - Stop");
		return 1;
	}

	new Float:d = GetDistanceBetweenPlayers(playerid, closplayer);
	if (d > 7) {
		SystemMsg(playerid,COLOUR_MEDIC,gettext(657));
		oSetPlayerHealth(playerid,100);
		new logstring[256];
		format(logstring, sizeof (logstring), "player: %d:  %s: heal self",playerid,oGetPlayerName(playerid));
		WriteLog(logstring);
		Debug("jobs/medic.inc  > Command 'heal' - Stop");
		return 1;
	}

	if (GetPlayerState(closplayer) == PLAYER_STATE_WASTED){
		SystemMsg(playerid,COLOUR_ERROR,gettext(658));
		Debug("jobs/medic.inc  > Command 'heal' - Stop");
		return 1;
	}

	if (GetPlayerDM(closplayer) != INVALID_DM_ID){
		SystemMsg(playerid,COLOUR_ERROR,gettext(659));
		Debug("jobs/medic.inc  > Command 'heal' - Stop");
		return 1;
	}

	if (MedicHealTime[playerid] > 0){
		SystemMsg(playerid,COLOUR_ERROR,gettext(660));
		Debug("jobs/medic.inc  > Command 'heal' - Stop");
		return 1;
	}

	if (PlayerWantMedic[closplayer] == 1){
		oSetPlayerHealth(closplayer,100);
		PlayerWantMedic[closplayer]=0;
		Player[playerid][Heals]++;
		GivePlayerXP(playerid,(Player[playerid][Heals]*5),1);
		new string[MAX_STRING];
		format(string,MAX_STRING,gettext(661),oGetPlayerName(playerid));
		SystemMsg(closplayer,COLOUR_MEDIC,string);
		format(string,MAX_STRING,gettext(662),oGetPlayerName(closplayer),Player[playerid][Heals]);
		SystemMsg(playerid,COLOUR_MEDIC,string);
		MedicHealTime[playerid]=30;
		new logstring[256];
		format(logstring, sizeof (logstring), "player: %d:  %s: heal player %s",playerid,oGetPlayerName(playerid),oGetPlayerName(closplayer));
		WriteLog(logstring);
	} else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(663)); 
	}
	Debug("jobs/medic.inc  > Command 'heal' - Stop");
	return 1;
}

public MedicKill(killerid, victimid, reason){
Debug("jobs/medic.inc  > MedicKill - Start");
SystemMsg(killerid,COLOUR_MEDIC,gettext(664));
Player[killerid][Heals] = 0;
Player[killerid][GotJob]=JOB_NOJOB;
SetPlayerSkin(killerid,SKIN_STANDART);
SetSpecialColorForPlayer(killerid);
new string[256];
format(string,MAX_STRING,gettext(665),oGetPlayerName(killerid));
SystemMsgToRegistered(COLOUR_MEDIC,string);
new logstring[256];
format(logstring, sizeof (logstring), "player: %d:  %s: civilian kill as a medic",killerid,oGetPlayerName(killerid));
WriteLog(logstring);
Debug("jobs/medic.inc  > MedicKill - Stop");
return 1;
}

public Medic_OnPlayerDeath(playerid,killerid,reason){
if (IsPlayerInDuel(playerid)) return 0;
Debug("jobs/medic.inc  > Medic_OnPlayerDeath - Start");
if (killerid == INVALID_PLAYER_ID) { 
	new string[256];
	format(string,MAX_STRING,gettext(666),oGetPlayerName(playerid));
	SystemMsgToRegistered(COLOUR_MEDIC,string);
	return 0;
}
SetPlayerCriminal(killerid,gettext(667),10);
new logstring[256];
format(logstring, sizeof (logstring), "player: %d:  %s: kill medic %s, wanted level and crimes added",killerid,oGetPlayerName(killerid),oGetPlayerName(playerid));
WriteLog(logstring);
Debug("jobs/medic.inc  > Medic_OnPlayerDeath - Stop");
return 0;
}

public Medic_OnPlayerStateChange(playerid, newstate, oldstate)
{
	Debug("jobs/medic.inc  > Medic_OnPlayerStateChange - Start");
	if (newstate == PLAYER_STATE_DRIVER){
		new pvehmodel = GetVehicleModel(GetPlayerVehicleID(playerid));
		if (pvehmodel == V_AMBULAN){
			if (Player[playerid][GotJob] != JOB_MEDIC && Player[playerid][GotJob] != JOB_COP){
				if (WasInAmbulance[playerid] == 0){
					SystemMsg(playerid,COLOUR_MEDIC,gettext(668));
					SetPlayerCriminal(playerid,gettext(669),2);
					WasInAmbulance[playerid] = 1;
				}
			}
		}
	}
	Debug("jobs/medic.inc  > Medic_OnPlayerStateChange - Stop");
}

public Medic_OnPlayerSpawn(playerid)
{
	Debug("jobs/Medic.inc  > Medic_OnPlayerSpawn - Start");
	if (Player[playerid][GotJob] == JOB_MEDIC && Player[playerid][SpawnPlace] == SPAWN_JOB){
		new rand = random(MedicSpawnsCount);
		SetPlayerPos(playerid, MedicSpawns[rand][Coord_X], MedicSpawns[rand][Coord_Y], MedicSpawns[rand][Coord_Z]);
	}
	Debug("jobs/cop.inc  > Medic_OnPlayerSpawn - Stop");
	return 0;
}

stock SendMsgToMedics(msg[]){
	Debug("jobs/medic.inc  > SendMsgToMedics - Start");
	for(new i = 0; i <= MAX_PLAYERS_EX; i++)
	{
		if (IsPlayerConnected(i)){
			if (Player[i][GotJob] == JOB_MEDIC){
				SystemMsg(i,COLOUR_MEDIC,msg);
			}
		}
	}
	Debug("jobs/medic.inc  > SendMsgToMedics - Stop");
}