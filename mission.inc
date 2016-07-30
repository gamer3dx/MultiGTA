//Simple mission engine
//Created by MadCat

forward Mission_Timer();
forward Mission_Register(name[]);
forward Mission_StartMission(playerid,missionid);
forward Mission_FinishMission(playerid,missionid,bool:success);
forward Mission_PlayerGetThere(playerid,Float:x,Float:y,Float:z);
forward Timer_PlayerOutOfCar(playerid);
//Main callbacks
forward Missions_OnGameModeInit();
forward Missions_OnPlayerEnterCP(playerid);
forward Missions_OnPlayerDeath(playerid, killerid, reason);
forward Missions_OnPlayerCommandText(playerid,text[]);
forward Missions_OnPlayerDisconnect(playerid,reason);
forward Missions_OnPlayerConnect(playerid);
forward Missions_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger);
forward Missions_OnPlayerExitVehicle(playerid, vehicleid);

new PlayerMissionsDB[MAX_STRING] = "MultiGTA/Players/Missions/";

new Mission_PlayerState[MAX_PLAYERS];
new Mission_PlayerTime[MAX_PLAYERS];
new Mission_PlayerCarOutTime[MAX_PLAYERS];
new LastMissionPlayerTime[MAX_PLAYERS];
new Mission_PlayerGetTo[MAX_PLAYERS][CoordInfo];

new OutOfCarTimer[MAX_PLAYERS];

#define MAX_MISSIONS 10
#define MAX_MISSION_CP 20

#define INVALID_MISSION_ID 0

new MissionsCount;

enum MissionInfo{
	mission_name[MAX_NAME],
	mission_questid,
	mission_reward_money,
	mission_reward_xp
}

new Float:MissionCheckpoints[MAX_MISSIONS][MAX_MISSION_CP][CoordInfo];

new MissionText[MAX_MISSIONS][MAX_MISSION_CP][MAX_STRING];

new MissionNeedVehicle[MAX_MISSIONS][MAX_MISSION_CP];
new MissionTime[MAX_MISSIONS][MAX_MISSION_CP];
new MissionMaxOutOfCarTime[MAX_MISSIONS];
new Mission[MAX_MISSIONS][MissionInfo];

new MissionSize[MAX_MISSIONS];

new Text:MissionTimeHud[MAX_PLAYERS];
new Mission_TextDrawExist[MAX_PLAYERS];

public Mission_Timer(){
	for(new playerid=0; playerid<MAX_PLAYERS_EX; playerid++)
	{
		if(IsPlayerConnected(playerid)){
			if (Mission_PlayerState[playerid] != 999){
				if (Mission_PlayerTime[playerid] != 999){
					Mission_PlayerTime[playerid] = Mission_PlayerTime[playerid]-1;
					if (!Mission_TextDrawExist[playerid]) ShowMissionTime(playerid);
					UpdateMissionTime(playerid);
					if (Mission_PlayerTime[playerid] <= 0) Mission_FinishMission(playerid,GetPlayerMission(playerid),false);
				} else {
					StopShowingMissionTime(playerid);
				}
			}
		}	
	}
}

public Mission_Register(name[])
{
	Debug("mission.inc > Mission_Register - Start");
	for (new i=0;i<MAX_MISSIONS;i++)
	{
		if (strcomp(name,Mission[i][mission_name],true)==1)
		{
			new logstring[MAX_STRING];
			format(logstring,MAX_STRING,"Error: Mission with name '%s' already exist!",name);
			WriteLog(logstring);
			return INVALID_MISSION_ID;
		}
	}

	MissionsCount++;
	if (MissionsCount >= MAX_MISSIONS) return INVALID_MISSION_ID;
	new missionid = MissionsCount;
	Mission[missionid][mission_questid] = RegisterQuest(name);
	if (Mission[missionid][mission_questid] == INVALID_MISSION_ID)
	{
		new logstring[MAX_STRING];
		format(logstring,MAX_STRING,"Error: Can't register mission '%s'!",name);
		WriteLog(logstring);
		MissionsCount--;
		return INVALID_MISSION_ID;
	}
	set(Mission[missionid][mission_name],name);
	Debug("mission.inc > Mission_Register - Stop");
	return missionid;
}

Float:GetMissionCPX(missionid,cpid)
{
	return MissionCheckpoints[missionid][cpid][Coord_X];
}
Float:GetMissionCPY(missionid,cpid)
{
	return MissionCheckpoints[missionid][cpid][Coord_Y];
}
Float:GetMissionCPZ(missionid,cpid)
{
	return MissionCheckpoints[missionid][cpid][Coord_Z];
}

public Mission_StartMission(playerid,missionid){
	new tmp[MAX_STRING];
  	format(tmp,sizeof(tmp),gettext(1329),Mission[missionid][mission_name]);
  	ShowTextForPlayer(playerid, tmp, 3000, 3);
	PlayerQuest[playerid] = GetMissionQuestID(missionid);
	Mission_PlayerState[playerid] = 0;
	Mission_PlayerTime[playerid] = MissionTime[missionid][0];
	Mission_PlayerCarOutTime[playerid] = MissionMaxOutOfCarTime[missionid];
	new logstring[MAX_STRING];
	format(logstring,MAX_STRING,"player %s (%d) start mission %s (%d)",oGetPlayerName(playerid),playerid,Mission[missionid][mission_name],missionid);
	WriteLog(logstring);
	NextMissionCP(playerid,missionid);
}

public Mission_FinishMission(playerid,missionid,bool:success) {
	Mission_PlayerState[playerid] = 999;
	Mission_PlayerTime[playerid] = 999;
	Mission_PlayerCarOutTime[playerid] = 0;
	LastMissionPlayerTime[playerid] = 999;
  	new tmp[MAX_STRING];
	new logstring[MAX_STRING];
  	if (success == true){
  		format(tmp,sizeof(tmp),gettext(1330),Mission[missionid][mission_name]);
		oGivePlayerMoney(playerid,Mission[missionid][mission_reward_money],1);
		GivePlayerXP(playerid,Mission[missionid][mission_reward_xp],1);
		PlayerPlaySoundOnPlayer(playerid,1057);
		WritePlayerMissionToFile(playerid,missionid);
		format(logstring,MAX_STRING,"player %s (%d) complete mission %s (%d)",oGetPlayerName(playerid),playerid,Mission[missionid][mission_name],missionid);
  	} else {
		format(tmp,sizeof(tmp),gettext(1331),Mission[missionid][mission_name]);
		oGivePlayerMoney(playerid,-Mission[missionid][mission_reward_money],1);
		GivePlayerXP(playerid,-Mission[missionid][mission_reward_xp],1);
		format(logstring,MAX_STRING,"player %s (%d) fail mission %s (%d)",oGetPlayerName(playerid),playerid,Mission[missionid][mission_name],missionid);
	  }
  	ShowTextForPlayer(playerid, tmp, 3000, 3);
	StopShowingMissionTime(playerid);
	ResetQuest(playerid);
	WriteLog(logstring);
}

public Mission_PlayerGetThere(playerid,Float:x,Float:y,Float:z){
	oSetPlayerCheckpoint(playerid,x,y,z,2);
	Mission_PlayerGetTo[playerid][Coord_X] = x;
	Mission_PlayerGetTo[playerid][Coord_Y] = y;
	Mission_PlayerGetTo[playerid][Coord_Z] = z;
}

GetMissionQuestID(missionid)
{
	return Mission[missionid][mission_questid];
}

GetPlayerMission(playerid)
{
	if (!IsPlayerConnected(playerid)) return 0;
	for (new missionid=1; missionid<=MissionsCount;missionid++)
	{
		if (PlayerQuest[playerid] == GetMissionQuestID(missionid)) // if player is in this mission
		{
			return missionid;
		}
	}
	return 0;
}

NextMissionCP(playerid,missionid)
{
	Debug("mission.inc > NextMissionCP - Start");

	Mission_PlayerState[playerid]++;

	if (MissionNeedVehicle[missionid][Mission_PlayerState[playerid]] != 0){
		if (MissionNeedVehicle[missionid][Mission_PlayerState[playerid]] == 1){
			if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER){
				SystemMsg(playerid,COLOUR_ERROR,gettext(1332));
				Mission_PlayerState[playerid]--;
				return;
			}
		}
		else if (MissionNeedVehicle[missionid][Mission_PlayerState[playerid]] > 399 && MissionNeedVehicle[missionid][Mission_PlayerState[playerid]] < 612){
			if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER){
				new string[MAX_STRING];
				format(string,sizeof(string),gettext(1333),GetVehicleName(MissionNeedVehicle[missionid][Mission_PlayerState[playerid]]));
				SystemMsg(playerid,COLOUR_ERROR,string);
				Mission_PlayerState[playerid]--;
				return;
			} else if (GetVehicleModel(GetPlayerVehicleID(playerid)) != MissionNeedVehicle[missionid][Mission_PlayerState[playerid]]){
				new string[MAX_STRING];
				format(string,sizeof(string),gettext(1333),GetVehicleName(MissionNeedVehicle[missionid][Mission_PlayerState[playerid]]));
				SystemMsg(playerid,COLOUR_ERROR,string);
				Mission_PlayerState[playerid]--;
				return;
			}
		} 
	}

	if (MissionTime[missionid][Mission_PlayerState[playerid]] == 999){
		Mission_PlayerTime[playerid] = 999;
		LastMissionPlayerTime[playerid] = 999;
	} else {
		if (LastMissionPlayerTime[playerid] != 999){
			Mission_PlayerTime[playerid] = Mission_PlayerTime[playerid] + MissionTime[missionid][Mission_PlayerState[playerid]];
		} else {
			Mission_PlayerTime[playerid] = MissionTime[missionid][Mission_PlayerState[playerid]];
		}
	}

	if (Mission_PlayerState[playerid] == MissionSize[missionid]) // if finish CP
	{
		Mission_FinishMission(playerid,missionid,true);
		Debug("mission.inc > NextMissionCP - Stop");
		return;
	}
	
	if (strlen(MissionText[missionid][Mission_PlayerState[playerid]]) > 0) SystemMsg(playerid,COLOUR_INFO,MissionText[missionid][Mission_PlayerState[playerid]]);
	
	Mission_PlayerGetThere(playerid,GetMissionCPX(missionid,Mission_PlayerState[playerid]),GetMissionCPY(missionid,Mission_PlayerState[playerid]),GetMissionCPZ(missionid,Mission_PlayerState[playerid]));
	
	Debug("mission.inc > NextMissionCP - Stop");
}

public Missions_OnGameModeInit(){
	SetTimer("Mission_Timer",1000,1);
	WriteLog("Mission engine Loaded!");
}

public Missions_OnPlayerEnterCP(playerid)
{
	if (!IsPlayerRegistered(playerid)) return 0;
	for (new missionid=1; missionid<=MissionsCount;missionid++) //  for each mission
	{
		if (PlayerQuest[playerid] == 0) // if player not on a quest
		{
			// if player at startline
			if (oIsPlayerInCheckpoint(playerid,GetMissionCPX(missionid,0),GetMissionCPY(missionid,0),GetMissionCPZ(missionid,0),20))
			{
				new string[MAX_STRING];
				format(string,MAX_STRING,gettext(1334),Mission[missionid][mission_name]);
				SystemMsg(playerid,COLOUR_OK,string);
				return 1;
			}
		}


		if (PlayerQuest[playerid] == GetMissionQuestID(missionid)) // if player is in this mission
		{
			if (oIsPlayerInCheckpoint(playerid,GetMissionCPX(missionid,Mission_PlayerState[playerid]),GetMissionCPY(missionid,Mission_PlayerState[playerid]),GetMissionCPZ(missionid,Mission_PlayerState[playerid]),20))
			{ 
				NextMissionCP(playerid,missionid);
				return 1;
			}
		}
	}
	return 0;
}

public Missions_OnPlayerDeath(playerid, killerid, reason)
{
	#pragma unused killerid
	#pragma unused reason
	new mission = GetPlayerMission(playerid);
	if (mission != 0){
		Mission_FinishMission(playerid,mission,false);
	}
}

public Missions_OnPlayerCommandText(playerid,text[])
{
	if (!IsPlayerRegistered(playerid)) return 0;
	dcmd(start,5,text);
	return 0;
}

public Missions_OnPlayerDisconnect(playerid,reason) {
	#pragma unused reason
	Mission_PlayerState[playerid] = Mission_PlayerTime[playerid] 999;
	Mission_PlayerCarOutTime[playerid] = LastMissionPlayerTime[playerid] = OutOfCarTimer[playerid] = 0;
	Mission_PlayerGetTo[playerid][Coord_X] = Mission_PlayerGetTo[playerid][Coord_Y] = Mission_PlayerGetTo[playerid][Coord_Z] = 0.0;
	StopShowingMissionTime(playerid);
    	return 0;
}

public Missions_OnPlayerConnect(playerid) {
	Mission_PlayerState[playerid] = Mission_PlayerTime[playerid] 999;
	Mission_PlayerCarOutTime[playerid] = LastMissionPlayerTime[playerid] = OutOfCarTimer[playerid] = 0;
	Mission_PlayerGetTo[playerid][Coord_X] = Mission_PlayerGetTo[playerid][Coord_Y] = Mission_PlayerGetTo[playerid][Coord_Z] = 0.0;
    	return 0;
}

public Missions_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger){
	#pragma unused vehicleid
	#pragma unused ispassenger
	new mis = GetPlayerMission(playerid);
	if (mis != 0){
		if (MissionMaxOutOfCarTime[mis] > 0){
			if (Mission_PlayerCarOutTime[playerid] > 0){
				KillTimer(OutOfCarTimer[playerid]);
				Mission_PlayerCarOutTime[playerid] = MissionMaxOutOfCarTime[mis];
			}
		}
	}
	return 0;
}

public Missions_OnPlayerExitVehicle(playerid, vehicleid){
	#pragma unused vehicleid
	new mis = GetPlayerMission(playerid);
	if (mis != 0){
		if (MissionMaxOutOfCarTime[mis] > 0){
			OutOfCarTimer[playerid] = SetTimerEx("Timer_PlayerOutOfCar", 1000, 1, "p", playerid);
		}
	}
	return 0;
}

public Timer_PlayerOutOfCar(playerid){
	if (!IsPlayerConnected(playerid)){
		KillTimer(OutOfCarTimer[playerid]);
	} else {
		Mission_PlayerCarOutTime[playerid]=Mission_PlayerCarOutTime[playerid]-1;
		new string[MAX_STRING];
		if (Mission_PlayerCarOutTime[playerid] == 60){
			format(string,MAX_STRING,gettext(1338));
			SystemMsg(playerid,COLOUR_INFO,string);
		} else if (Mission_PlayerCarOutTime[playerid] == 45){
			format(string,MAX_STRING,gettext(1339));
			SystemMsg(playerid,COLOUR_INFO,string);
		} else if (Mission_PlayerCarOutTime[playerid] == 30){
			format(string,MAX_STRING,gettext(1340));
			SystemMsg(playerid,COLOUR_INFO,string);
		} else if (Mission_PlayerCarOutTime[playerid] == 15){
			format(string,MAX_STRING,gettext(1341));
			SystemMsg(playerid,COLOUR_INFO,string);
		} else if (Mission_PlayerCarOutTime[playerid] == 10){
			format(string,MAX_STRING,gettext(1342));
			SystemMsg(playerid,COLOUR_INFO,string);
		} else if (Mission_PlayerCarOutTime[playerid] == 5){
			format(string,MAX_STRING,gettext(1343));
			SystemMsg(playerid,COLOUR_INFO,string);
		} else if (Mission_PlayerCarOutTime[playerid] <= 0){
			SystemMsg(playerid,COLOUR_INFO,gettext(1344));
			Mission_FinishMission(playerid,GetPlayerMission(playerid),false);
			KillTimer(OutOfCarTimer[playerid]);
		}
	}
}

stock StopShowingMissionTime(playerid){
	if(Mission_TextDrawExist[playerid]){
	    TextDrawDestroy(MissionTimeHud[playerid]);
	    Mission_TextDrawExist[playerid] = 0;
	}
}

stock ShowMissionTime(playerid){
	new string[MAX_STRING];
	format(string,MAX_STRING,"%s",FormatTimeForEvent(Mission_PlayerTime[playerid]));
	MissionTimeHud[playerid] = TextDrawCreate(547.0, 24.0, string);
	TextDrawSetShadow(MissionTimeHud[playerid], 0);
	TextDrawSetOutline(MissionTimeHud[playerid], 2);
	TextDrawLetterSize(MissionTimeHud[playerid], 0.6, 1.8);
	TextDrawFont(MissionTimeHud[playerid], 3);
	TextDrawShowForPlayer(playerid,MissionTimeHud[playerid]);
	Mission_TextDrawExist[playerid] = 1;
	TextDrawShowForPlayer(playerid, MissionTimeHud[playerid]);
}

stock UpdateMissionTime(playerid){
	new string[MAX_STRING];
	format(string,MAX_STRING,"%s",FormatTimeForEvent(Mission_PlayerTime[playerid]));
	TextDrawSetString(MissionTimeHud[playerid],"    ");
	TextDrawSetString(MissionTimeHud[playerid],string);
}

stock WritePlayerMissionToFile(playerid,missionid){
	new File:PlayerFile;
	new filestring[256];
	format(filestring,sizeof(filestring),"%s%s.txt",PlayerMissionsDB,oGetPlayerName(playerid));

	if (fexist(filestring))
	{
	        PlayerFile = fopen(filestring, io_append);
	}
		else
	{
	        PlayerFile = fopen(filestring, io_write);
	}
	if (PlayerFile){
		fwrite(PlayerFile, Mission[missionid][mission_name]);
		fwrite(PlayerFile, "\r\n");
		fclose(PlayerFile);
	}
	
}

stock FindMissionInPlayerFile(playerid,missionid){
	new File:PlayerFile, filestring[MAX_STRING];
	new filename[256];
	format(filename,sizeof(filename),"%s%s.txt",PlayerMissionsDB,oGetPlayerName(playerid));
	PlayerFile=fopen(filename,io_read);
	if (!PlayerFile) return 0;
	while (fread(PlayerFile,filestring,sizeof(filestring))) {
		StripNewLine(filestring);
		if (strfind(filestring, Mission[missionid][mission_name], true) != -1){
			fclose(PlayerFile);
			return 1; //found
		}
	}
	fclose(PlayerFile);
	return 0; // not found
}

dcmd_start(playerid,params[])
{
	#pragma unused params
	for (new missionid=1; missionid<=MissionsCount;missionid++) //  for each mission
		{
		if (PlayerQuest[playerid] == 0) // if player not on a quest
		{
			// if player at startline
			if (oIsPlayerInCheckpoint(playerid,GetMissionCPX(missionid,0),GetMissionCPY(missionid,0),GetMissionCPZ(missionid,0),20))
			{
				if (MissionNeedVehicle[missionid][0] == 1){
					if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER){
						SystemMsg(playerid,COLOUR_ERROR,gettext(1335));
						return 1;
					}
				}
				else if (MissionNeedVehicle[missionid][0] > 399 && MissionNeedVehicle[missionid][0] < 612){
					if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER){
						new string[MAX_STRING];
						format(string,sizeof(string),gettext(1336),GetVehicleName(MissionNeedVehicle[missionid][0]));
						SystemMsg(playerid,COLOUR_ERROR,string);
						return 1;
					} else if (GetVehicleModel(GetPlayerVehicleID(playerid)) != MissionNeedVehicle[missionid][0]){
						new string[MAX_STRING];
						format(string,sizeof(string),gettext(1336),GetVehicleName(MissionNeedVehicle[missionid][0]));
						SystemMsg(playerid,COLOUR_ERROR,string);
						return 1;
					}
				} 
				if (FindMissionInPlayerFile(playerid,missionid) == 1){
					SystemMsg(playerid,COLOUR_ERROR,gettext(1337));
					return 1;
				}
				Mission_StartMission(playerid,missionid);
				return 1;
			}
		}
	}
	return 0;
}