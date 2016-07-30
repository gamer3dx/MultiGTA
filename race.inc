//Race module.
//Created by Iain Gilbert
//Modder: MadCat
//Thanks to switch for TextDraw (from gamemode 'Adrenaline 1.06')

#include "base"
#include "player"
#include "world"
#include "vehicles"

forward Race_OnGameModeInit();
forward racesconfig();
forward CheckRace();
forward Race_OnPlayerCommandText(playerid,text[]);
forward RegisterRace(name[]);
forward Race_OnPlayerEnterRaceCP(playerid);
forward Race_OnPlayerStateChange(playerid,newstate,oldstate);
forward Race_OnPlayerDisconnect(playerid);

#define MAX_CP 100
#define MAX_RACES 150

#define MAX_RACE_VEHICLES 15

#define RACE_STATE_DISABLED 0
#define RACE_STATE_SLEEPING 1
#define RACE_STATE_LINEUP 2
#define RACE_STATE_COUNTDOWN 3
#define RACE_STATE_RACING 4

#define INVALID_RACE_ID 0

new Text:Ttime;
new Text:Tplayercps[MAX_PLAYERS];

new RacesCount;

enum RaceInfo{
	race_name[MAX_NAME],    // name of race
	race_minracers,         // minimum racers needed to race
	race_maxracetime,       // max time player can take to complete a race
	race_frequency,        // frquency that race runs
	race_lineupdelay,       // time to wait for players to linup
	race_cashprize,   // cash earned for 1st position
	race_cashentry,   // Cash required to enter (earned by winner)
	race_xpprize,    // xp earned for 1st position
	race_vehicles[MAX_RACE_VEHICLES], // vehicles allowed in race
	Float:race_startheading // startline heading
}
new Race[MAX_RACES][RaceInfo];

enum RaceStatsInfo{
	race_state,             // race state
	race_questid,           // quest id race is assigned
	race_activecpid,        // active cp id race is assigned
	race_timer,       // race timer
	race_racercount,    // count of racers
	race_position           // how many ppl have completed race alrready
}
new RaceStats[MAX_RACES][RaceStatsInfo];


enum RacePlayerInfo{
	race_player_time,   // time player has been in race
	race_player_cp
}
new RacePlayerStats[MAX_PLAYERS][RacePlayerInfo];

enum RaceScoreInfo {
	race_score_player[MAX_NAME],
	race_score_time,
	race_score_vehicle,
};
new BestScore[MAX_RACES][RaceScoreInfo];

new RaceSize[MAX_RACES]; // number of checkpoints, including start and finish
new CPSize=12; // size of checkpoints
new Float:RaceCheckpoints[MAX_RACES][MAX_CP][CoordInfo]; //  someone tell me... why does this work? i thought pawn only suppors 2d arrays

new LastRaceVehicle[MAX_PLAYERS];

//--------------------------------------------------------------

new RaceDB[MAX_STRING] = "MultiGTA/Races/Races.list";
new RaceBaseDB[MAX_STRING] = "MultiGTA/Races/";
new RaceRecordDB[MAX_STRING] = "MultiGTA/Races/Records/";

new Set_Virtual_World_For_Races = 1;

new TimerRace;

public Race_OnGameModeInit(){
	racesconfig();
	RaceLoadAll();
	TimerRace = SetTimer("CheckRace", 1000, 1);
	Ttime = TextDrawCreate(518.0, 310.0, "00:00");
	TextDrawLetterSize(Ttime, 1.2, 6.2);
	new logstring[MAX_STRING];
	format(logstring,MAX_STRING,"Races System Loaded. %d races loaded.",RacesCount);
	WriteLog(logstring);
	return 0;
}

public racesconfig()
{
	Debug("race.inc > racesconfig - Start");
	if (!db_Exists(ConfigDB)) db_Create(ConfigDB);

	new temp[MAX_STRING];
	
	set(temp,db_Get(ConfigDB,"Set_Virtual_World_For_Races"));
	if (strlen(temp) > 0) Set_Virtual_World_For_Races = strval(temp); 
	else { valstr(temp,Set_Virtual_World_For_Races); db_Set(ConfigDB,"Set_Virtual_World_For_Races",temp); }
	set(temp,nullstr);
	
	Debug("race.inc > racesconfig - Stop");

}

Float:GetRaceCPX(raceid,cpid)
{
	return RaceCheckpoints[raceid][cpid][Coord_X];
}
Float:GetRaceCPY(raceid,cpid)
{
	return RaceCheckpoints[raceid][cpid][Coord_Y];
}
Float:GetRaceCPZ(raceid,cpid)
{
	return RaceCheckpoints[raceid][cpid][Coord_Z];
}

RaceLoadAll()
{
	Debug("race.inc > RaceLoadAll - Start");
	
	new temp[MAX_STRING];
	if (!db_Exists(DatabaseDB)) db_Create(DatabaseDB);
	set(temp,db_Get(DatabaseDB,"Race_DB"));
	if (strlen(temp) > 0) set(RaceDB,temp);
	set(temp,db_Get(DatabaseDB,"Race_Base_DB"));
	if (strlen(temp) > 0) set(RaceBaseDB,temp);
	set(temp,db_Get(DatabaseDB,"Race_Record_DB"));
	if (strlen(temp) > 0) set(RaceRecordDB,temp);
	if (!db_Exists(RaceDB))
	{
		db_Create(RaceDB);
	}

	for (new racedbid=0;racedbid<MAX_RACES;racedbid++)
	{ // load all our races from db
		new cellname[MAX_STRING];
		format(cellname,sizeof(cellname),"Race%d",racedbid);
		set(temp,db_Get(RaceDB,cellname));
		if (strlen(temp) == 0) continue;
		if (!RaceBaseDBExists(temp)) continue;
		new raceid = RegisterRace(temp);
		if (raceid == INVALID_RACE_ID) continue;
		RaceLoadBaseDB(raceid);
		RaceLoadRecordDB(raceid);
	}
	Debug("race.inc > RaceLoadAll - Stop");
	return;
}

RaceBaseDBExists(racename[MAX_STRING])
{
	Debug("race.inc > RaceBaseDBExists - Start");
	new rdbname[MAX_STRING];
	format(rdbname,sizeof(rdbname),"%s%s.txt",RaceBaseDB,racename);
	if (!db_Exists(rdbname))
	{
		Debug("race.inc > RaceBaseDBExists - Stop");
		return 0;
	}
	Debug("race.inc > RaceBaseDBExists - Stop");
	return 1;
}


RaceLoadBaseDB(raceid)
{ // load race from db
	Debug("race.inc > RaceLoadBaseDB - Start");
	new temp[MAX_STRING];
	new rdbname[MAX_STRING];
	format(rdbname,sizeof(rdbname),"%s%s.txt",RaceBaseDB,Race[raceid][race_name]);
	if (!db_Exists(rdbname))
	{
		return INVALID_RACE_ID;
	}
	set(temp,db_Get(rdbname,"Name"));
	if (strlen(temp) > 0) set(Race[raceid][race_name],temp);
	set(temp,nullstr);

	if (raceid == INVALID_RACE_ID)
	{
		new logstring[256];
		format(logstring, sizeof (logstring), "Race (DB): %s Failed to load", Race[raceid][race_name]);
		WriteLog(logstring);
		Debug("race.inc > RaceLoadBaseDB - Stop");
		return INVALID_RACE_ID;
	}

	set(temp,db_Get(rdbname,"Race_Frequency"));
	if (strlen(temp) > 0) Race[raceid][race_frequency] = strval(temp);
	set(temp,nullstr);

	set(temp,db_Get(rdbname,"Lineup_Delay"));
	if (strlen(temp) > 0) Race[raceid][race_lineupdelay] = strval(temp);
	set(temp,nullstr);

	set(temp,db_Get(rdbname,"Min_Racers"));
	if (strlen(temp) > 0) Race[raceid][race_minracers] = strval(temp);
	set(temp,nullstr);

	if (Race[raceid][race_minracers] < 2) Race[raceid][race_minracers] = 2;

	set(temp,db_Get(rdbname,"Cash_Prize"));
	if (strlen(temp) > 0) Race[raceid][race_cashprize] = strval(temp);
	set(temp,nullstr);

	set(temp,db_Get(rdbname,"Cash_Entry"));
	if (strlen(temp) > 0) Race[raceid][race_cashentry] = strval(temp);
	set(temp,nullstr);

	set(temp,db_Get(rdbname,"XP_Prize"));
	if (strlen(temp) > 0) Race[raceid][race_xpprize] = strval(temp);
	set(temp,nullstr);

	set(temp,db_Get(rdbname,"Race_Time"));
	if (strlen(temp) > 0) Race[raceid][race_maxracetime] = strval(temp);
	set(temp,nullstr);

	set(temp,db_Get(rdbname,"Allowed_Vehicles"));
	if (strlen(temp) > 0) set(Race[raceid][race_vehicles],RaceDecodeVehiclesDB(temp));
	set(temp,nullstr);

	set(temp,db_Get(rdbname,"Startline_Heading"));
	if (strlen(temp) > 0) Race[raceid][race_startheading] = floatstr(temp);
	set(temp,nullstr);

	new cpcount;
	for (new cpid=0;cpid<MAX_CP;cpid++)
	{
		new cellname[MAX_STRING];
		format(cellname,sizeof(cellname),"CP%d",cpid);
		set(temp,db_Get(rdbname,cellname));
		if (strlen(temp) == 0) break;

		new Float:X;
		new Float:Y;
		new Float:Z;
		new idx=0;
		X= floatstr(strcharsplit(temp,idx,strchar(",")));
		Y = floatstr(strcharsplit(temp,idx,strchar(",")));
		Z = floatstr(strcharsplit(temp,idx,strchar(",")));
		set(temp,nullstr);
		if (( X== 0.0) && (Y == 0.0)) break;
		RaceCheckpoints[raceid][cpid][Coord_X] = X;
		RaceCheckpoints[raceid][cpid][Coord_Y] = Y;
		RaceCheckpoints[raceid][cpid][Coord_Z] = Z;
		cpcount++;
	}

	RaceSize[raceid] = cpcount;
	new logstring[256];
	RaceStats[raceid][race_timer] = MakeRaceSleepTime(raceid);
	RaceStats[raceid][race_state] = RACE_STATE_SLEEPING;
	format(logstring, sizeof (logstring), "Race (DB): %s - Loaded", Race[raceid][race_name]);
	WriteLog(logstring);
	Debug("race.inc > RaceLoadBaseDB - Stop");
	return raceid;
}

MakeRaceSleepTime(raceid)
{
	Debug("race.inc > MakeRaceSleepTime - Start");
	new sleeptime;
	if (Race[raceid][race_frequency] == 0) Race[raceid][race_frequency] = 5;
	sleeptime = ((Race[raceid][race_frequency] * RacesCount) * 150);
	Debug("race.inc > MakeRaceSleepTime - Stop");
	return sleeptime;
}
RaceLoadRecordDB(raceid)
{
	Debug("race.inc > RaceLoadRecordDB - Start");
	new temp[MAX_STRING];
	new rdbname[MAX_STRING];
	format(rdbname,sizeof(rdbname),"%s%s.txt",RaceRecordDB,Race[raceid][race_name]);
	if (!db_Exists(rdbname)) return;
	set(temp,db_Get(rdbname,"Best_Time_Record"));
	if (strlen(temp) > 0) BestScore[raceid][race_score_time] = strval(temp);
	set(temp,nullstr);
	set(temp,db_Get(rdbname,"Best_Time_Player"));
	if (strlen(temp) > 0) set(BestScore[raceid][race_score_player],temp);
	set(temp,nullstr);
	set(temp,db_Get(rdbname,"Best_Time_Vehicle"));
	if (strlen(temp) > 0) BestScore[raceid][race_score_vehicle] = strval(temp);
	set(temp,nullstr);
	Debug("race.inc > RaceLoadRecordDB - Stop");
}

RaceSaveAll()
{
	Debug("race.inc > RaceSaveAll - Start");
	if (!db_Exists(DatabaseDB)) db_Create(DatabaseDB);
	db_Set(DatabaseDB,"Race_Base_DB",RaceBaseDB);

	if (!db_Exists(RaceDB)) db_Create(RaceDB);
	for (new raceid=1;raceid<=RacesCount;raceid++)
	{ // load all our races from db
		if (RaceStats[raceid][race_state] == RACE_STATE_DISABLED) continue;
		new cellname[MAX_STRING];
		format(cellname,sizeof(cellname),"Race%d",raceid);
		db_Set(RaceDB,cellname,Race[raceid][race_name]);

		RaceSaveBaseDB(raceid);
		RaceSaveRecordDB(raceid);
	}
	Debug("race.inc > RaceSaveAll - Stop");
}

RaceSaveScores()
{
	Debug("race.inc > RaceSaveScores - Start");
	if (!db_Exists(DatabaseDB)) db_Create(DatabaseDB);
	db_Set(DatabaseDB,"Race_Base_DB",RaceBaseDB);

	if (!db_Exists(RaceDB)) db_Create(RaceDB);
	for (new raceid=1;raceid<=RacesCount;raceid++)
	{ // load all our races from db
		if (RaceStats[raceid][race_state] == RACE_STATE_DISABLED) continue;
		new cellname[MAX_STRING];
		format(cellname,sizeof(cellname),"Race%d",raceid);
		db_Set(RaceDB,cellname,Race[raceid][race_name]);
		RaceSaveRecordDB(raceid);
	}
	Debug("race.inc > RaceSaveScores - Stop");
}

RaceSaveBaseDB(raceid)
{ // save race to db
	Debug("race.inc > RaceSaveBaseDB - Start");
	if (RaceStats[raceid][race_state] == RACE_STATE_DISABLED) return;
	new temp[MAX_STRING];
	new rdbname[MAX_STRING];
	format(rdbname,sizeof(rdbname),"%s%s.txt",RaceBaseDB,Race[raceid][race_name]);
	if (db_Exists(rdbname)) {
		return;
	}

	db_Create(rdbname);

	db_BeforeBigSaving(rdbname);

	db_Set(rdbname,"Name",Race[raceid][race_name]);

	valstr(temp,Race[raceid][race_frequency]);
	db_Set(rdbname,"Race_Frequency",temp);
	set(temp,nullstr);

	valstr(temp,Race[raceid][race_lineupdelay]);
	db_Set(rdbname,"Lineup_Delay",temp);
	set(temp,nullstr);

	valstr(temp,Race[raceid][race_minracers]);
	db_Set(rdbname,"Min_Racers",temp);
	set(temp,nullstr);

	valstr(temp,Race[raceid][race_cashprize]);
	db_Set(rdbname,"Cash_Prize",temp);
	set(temp,nullstr);

	valstr(temp,Race[raceid][race_cashentry]);
	db_Set(rdbname,"Cash_Entry",temp);
	set(temp,nullstr);

	valstr(temp,Race[raceid][race_xpprize]);
	db_Set(rdbname,"XP_Prize",temp);
	set(temp,nullstr);

	valstr(temp,Race[raceid][race_maxracetime]);
	db_Set(rdbname,"Race_Time",temp);
	set(temp,nullstr);

	set(temp,RaceEncodeVehiclesDB(raceid));
	db_Set(rdbname,"Allowed_Vehicles",temp);
	set(temp,nullstr);

	format(temp,sizeof(temp),"%f",Race[raceid][race_startheading]);
	db_Set(rdbname,"Startline_Heading",temp);
	set(temp,nullstr);

	for (new cpid=0;cpid<RaceSize[raceid];cpid++)
	{
		new cellname[MAX_STRING];
		format(cellname,sizeof(cellname),"CP%d",cpid);
		format(temp,sizeof(temp),"%f,%f,%f,",RaceCheckpoints[raceid][cpid][Coord_X],RaceCheckpoints[raceid][cpid][Coord_Y],RaceCheckpoints[raceid][cpid][Coord_Z]);
		db_Set(rdbname,cellname,temp);
	}

	db_AfterBigSaving(rdbname);

	new logstring[256];
	format(logstring, sizeof (logstring), "Race base saved: %s",Race[raceid][race_name]);
	WriteLog(logstring);
	Debug("race.inc > RaceSaveBaseDB - Stop");
}

RaceSaveRecordDB(raceid)
{
	Debug("race.inc > RaceSaveRecordDB - Start");
	new temp[MAX_STRING];
	new rdbname[MAX_STRING];
	format(rdbname,sizeof(rdbname),"%s%s.txt",RaceRecordDB,Race[raceid][race_name]);
	if (!db_Exists(rdbname)) db_Create(rdbname);
	db_BeforeBigSaving(rdbname);
	db_Set(rdbname,"Name",Race[raceid][race_name]);
	set(temp,nullstr);
	valstr(temp,BestScore[raceid][race_score_time]);
	db_Set(rdbname,"Best_Time_Record",temp);
	set(temp,nullstr);
	set(temp,BestScore[raceid][race_score_player]);
	db_Set(rdbname,"Best_Time_Player",temp);
	set(temp,nullstr);
	valstr(temp,BestScore[raceid][race_score_vehicle]);
	db_Set(rdbname,"Best_Time_Vehicle",temp);
	set(temp,nullstr);
	db_AfterBigSaving(rdbname);
	Debug("race.inc > RaceSaveRecordDB - Stop");
	return;
}

public CheckRace() // must be ran by timer every second
{
	Debug("race.inc > CheckRace - Start");
	new string [MAX_STRING];
	for (new raceid=1; raceid<=RacesCount;raceid++)
	{
		if (RaceStats[raceid][race_state] == RACE_STATE_DISABLED) continue;

		if (RaceStats[raceid][race_state] == RACE_STATE_SLEEPING)
		{
			if (RaceStats[raceid][race_timer] <= 0) // if it is time to run
			{
				if (event_now == false){
					if (IsServerGotVehiclesForRace(raceid) == 1){
						RaceStats[raceid][race_timer] = 0;
						StartRaceLineup(raceid);
						continue;
					} else {
						RaceStats[raceid][race_timer] = MakeRaceSleepTime(raceid) * 500; //Block for a loooong time :)
					}
				} else {
					RaceStats[raceid][race_timer] = MakeRaceSleepTime(raceid);
				}
			}
			RaceStats[raceid][race_timer]--;
			continue;
		}

		TextDrawSetString(Ttime, FormatTimeForEvent(RaceStats[raceid][race_timer]));
		RaceStats[raceid][race_timer]++;
		if (RaceStats[raceid][race_state] == RACE_STATE_LINEUP)
		{
			if (RaceStats[raceid][race_timer] >= Race[raceid][race_lineupdelay]) // if it is time to run
			{
				RaceStats[raceid][race_timer] = 0;
				StartRaceCountdown(raceid);
			}
			else
			{
				if (RaceStats[raceid][race_racercount] > 0)
				{
					new racecountdown = Race[raceid][race_lineupdelay] - RaceStats[raceid][race_timer];
					if ((racecountdown == 5) || (racecountdown == 10) || (racecountdown == 20) || (racecountdown == 30) || (racecountdown == 45) || (racecountdown == 60) || (racecountdown == 90))
					{
						format(string,sizeof(string),gettext(515),Race[raceid][race_name],(Race[raceid][race_lineupdelay] - RaceStats[raceid][race_timer]));
						SendMessageToRacers(raceid,string);
						
					}
				}
			}
		}
		else if (RaceStats[raceid][race_state] == RACE_STATE_COUNTDOWN)
		{
			for (new playerid=0; playerid<MAX_PLAYERS_EX;playerid++)
			{
				if (IsPlayerConnected(playerid))
				{
					if (PlayerQuest[playerid] == GetRaceQuestID(raceid)) // if player is in this race
					{
						new Countdown = MAX_COUNTDOWN - RaceStats[raceid][race_timer];
						if (Countdown >= 1)
						{
							format(string, sizeof(string), "%d",Countdown);
							PlayerPlaySoundOnPlayer(playerid,1056);
						}
						else
						{
							format(string, sizeof(string), gettext(516));
							PlayerPlaySoundOnPlayer(playerid,1057);
						}
						ShowTextForPlayer(playerid, string,1000,6);
					}
				}
			}
			if (RaceStats[raceid][race_timer] >= MAX_COUNTDOWN)
			{
				StartRace(raceid);
				RaceStats[raceid][race_timer] = 0;
			}
		}
		else if (RaceStats[raceid][race_state] == RACE_STATE_RACING)
		{
			if (RaceStats[raceid][race_timer] >= Race[raceid][race_maxracetime]) // if time limit reached
			{
				EndRace(raceid);
				RaceStats[raceid][race_timer] = 0;
			}
		}
	}
	Debug("race.inc > CheckRace - Stop");
}

public Race_OnPlayerCommandText(playerid,text[])
{
	if (!IsPlayerRegistered(playerid)) return 0;

	new cmd[20];
	new idx;
	//new string[MAX_STRING];

	set(cmd,strcharsplit(text, idx,strchar(" ")));
	if (strlen(cmd) == 0) return 0;

	if(strcomp(cmd, "/race", true) == 1)
	{
		set(cmd,strcharsplit(text, idx,strchar(" ")));
		if(strcomp(cmd, "help", true) == 1)
		{
			Debug("race.inc > Command 'race help' - Start");
			SystemMsgScrolling(playerid,gettext(169));
			SystemMsgScrolling(playerid,gettext(170));
			SystemMsgScrolling(playerid,gettext(171));
			SystemMsgScrolling(playerid,gettext(172));
			SystemMsgScrolling(playerid,gettext(173));
			SystemMsgScrolling(playerid,gettext(174));
			SystemMsgScrolling(playerid,gettext(175));
			SystemMsgScrolling(playerid,gettext(176));
			SystemMsgScrolling(playerid,gettext(177));
			Debug("race.inc > Command 'race help' - Stop");
			return 1;
		}

		if(strcomp(cmd, "join", true) == 1)
		{
			Debug("race.inc > Command 'race join' - Start");
			new raceid;
			raceid = strval(strcharsplit(text, idx,strchar(" ")));
			if (PlayerQuest[playerid] != 0)
			{
				SystemMsg(playerid,COLOUR_RACE_BAD,gettext(518));
				Debug("race.inc > Command 'race join' - Stop");
				return 1;
			}
			if ((raceid == INVALID_RACE_ID) || (raceid >= MAX_RACES))
			{
				SystemMsg(playerid,COLOUR_RACE_BAD,gettext(519));
				Debug("race.inc > Command 'race join' - Stop");
				return 1;
			}
			if (RaceStats[raceid][race_state] == RACE_STATE_DISABLED)
			{
				SystemMsg(playerid,COLOUR_RACE_BAD,gettext(519));
				Debug("race.inc > Command 'race join' - Stop");
				return 1;
			}
			if (RaceStats[raceid][race_state] == RACE_STATE_RACING)
			{
				SystemMsg(playerid,COLOUR_RACE_BAD,gettext(520));
				Debug("race.inc > Command 'race join' - Stop");
				return 1;
			}
			if (RaceStats[raceid][race_state] != RACE_STATE_LINEUP)
			{
				SystemMsg(playerid,COLOUR_RACE_BAD,gettext(521));
				Debug("race.inc > Command 'race join' - Stop");
				return 1;
			}
			if (Player[playerid][GotJob] != JOB_COP){
			JoinRace(raceid,playerid);
			} else {
			SystemMsg(playerid,COLOUR_RACE_BAD,gettext(522));
			}
			Debug("race.inc > Command 'race join' - Stop");
			return 1;
		}
		if((strcomp(cmd, "leave", true) == 1) || (strcomp(cmd, "quit", true) == 1))
		{
			Debug("race.inc > Command 'race leave' - Start");
			if (PlayerQuest[playerid] == 0)
			{
				SystemMsg(playerid,COLOUR_RACE_BAD,gettext(523));
				Debug("race.inc > Command 'race leave' - Stop");
				return 1;
			}
			new raceid = GetPlayerRace(playerid);
			if (raceid == INVALID_RACE_ID)
			{
				SystemMsg(playerid,COLOUR_RACE_BAD,gettext(523));
				Debug("race.inc > Command 'race leave' - Stop");
				return 1;
			}
			LeaveRace(raceid,playerid);
			Debug("race.inc > Command 'race leave' - Stop");
			return 1;
		}
		return 0;
	}
	return 0;
}

AddRaceCP(raceid,Float:x,Float:y,Float:z)
{
	Debug("race.inc > AddRaceCP - Start");
	if (RaceSize[raceid] > MAX_CP) return 0;
	new cpid = RaceSize[raceid];
	RaceCheckpoints[raceid][cpid][Coord_X] = x;
	RaceCheckpoints[raceid][cpid][Coord_Y] = y;
	RaceCheckpoints[raceid][cpid][Coord_Z] = z;
	RaceSize[raceid]++;
	Debug("race.inc > AddRaceCP - Stop");
	return 1;
}

RemoveLastRaceCP(raceid)
{
	Debug("race.inc > RemoveLastRaceCP - Start");
	if (RaceSize[raceid] < 1) return 0;
	new cpid = RaceSize[raceid]-1;
	RaceCheckpoints[raceid][cpid][Coord_X] = 0.0;
	RaceCheckpoints[raceid][cpid][Coord_Y] = 0.0;
	RaceCheckpoints[raceid][cpid][Coord_Z] = 0.0;
	RaceSize[raceid]--;
	Debug("race.inc > RemoveLastRaceCP - Stop");
	return 1;
}

public RegisterRace(name[])
{
	Debug("race.inc > RegisterRace - Start");
	for (new i=0;i<MAX_RACES;i++)
	{
		if (RaceStats[i][race_state] == RACE_STATE_DISABLED) continue;
		if (strcomp(name,Race[i][race_name],true)==1)
		{
			return INVALID_RACE_ID;
		}
	}

	RacesCount++;
	if (RacesCount >= MAX_RACES) return INVALID_RACE_ID;
	new raceid = RacesCount;
	RaceStats[raceid][race_questid] = RegisterQuest(name);
	if (RaceStats[raceid][race_questid] == INVALID_RACE_ID)
	{
		RacesCount--;
		return INVALID_RACE_ID;
	}
	set(Race[raceid][race_name],name);
	Debug("race.inc > RegisterRace - Stop");
	return raceid;
}

StartRace(raceid)
{
	Debug("race.inc > StartRace - Start");
	RaceStats[raceid][race_state] = RACE_STATE_RACING;
	RaceStats[raceid][race_timer]=0;

	for (new playerid=0; playerid<MAX_PLAYERS_EX;playerid++)
	{
		if (IsPlayerConnected(playerid))
		{
			if (PlayerQuest[playerid] == GetRaceQuestID(raceid)) // if player is in this race
			{
				new tmp[MAX_STRING];
				format(tmp,MAX_STRING,"%d/%d",RacePlayerStats[playerid][race_player_cp],RaceSize[raceid]);
				Tplayercps[playerid] = TextDrawCreate(575.0, 370.0, tmp);
				TextDrawLetterSize(Tplayercps[playerid], 0.6, 3);
				TextDrawSetString(Ttime,"00:00");
				TextDrawShowForPlayer(playerid, Ttime);
				TextDrawShowForPlayer(playerid, Tplayercps[playerid]);

				Player[playerid][RacesPlayed]++;
				TogglePlayerControllable(playerid, 1);
				NextRaceCP(raceid,playerid);
			}
		}
	}
	Debug("race.inc > StartRace - Stop");
}

JoinRace(raceid,playerid)
{
	Debug("race.inc > JoinRace - Start");
	new PlayerState = GetPlayerState(playerid);
	new string[MAX_STRING];
	if (oGetPlayerMoney(playerid) < Race[raceid][race_cashentry])
	{
		format(string, sizeof(string),gettext(524),Race[raceid][race_cashentry]);
		SystemMsg(playerid,COLOUR_RACE_BAD,string);
		Debug("race.inc > JoinRace - Stop");
		return;
	}

	RaceStats[raceid][race_racercount]++;
	if (Race[raceid][race_minracers] > 1)
	{
		format(string, sizeof(string),"%s has joined the race. (Racers: %d/%d)", oGetPlayerName(playerid),RaceStats[raceid][race_racercount],Race[raceid][race_minracers]);
		new logstring[256];
		format(logstring, sizeof (logstring), "player: %d:  %s: has joined the race. (Racers:%d/%d)",playerid,oGetPlayerName(playerid),RaceStats[raceid][race_racercount],Race[raceid][race_minracers]);
		WriteLog(logstring);
	}
	else
	{
		format(string, sizeof(string),"%s has joined the race. (Racers: %d)", oGetPlayerName(playerid),RaceStats[raceid][race_racercount]);
		new logstring[256];
		format(logstring, sizeof (logstring), "player: %d:  %s: has joined the race. (Racers:%d)",playerid,oGetPlayerName(playerid),RaceStats[raceid][race_racercount]);
		WriteLog(logstring);
	}
	format(string,sizeof(string),gettext(525),oGetPlayerName(playerid),Race[raceid][race_name],RaceStats[raceid][race_racercount]);
	SendMessageToRacers(raceid,string);
	oGivePlayerMoney(playerid,0-Race[raceid][race_cashentry],1);
	ShowTextForPlayer(playerid,gettext(526),3000,1);
	PlayerQuest[playerid] = GetRaceQuestID(raceid);
	RacePlayerStats[playerid][race_player_cp] = 0;
	oSetPlayerRaceCheckpoint(playerid,2,GetRaceCPX(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPY(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPZ(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPX(raceid,RacePlayerStats[playerid][race_player_cp]+1),GetRaceCPY(raceid,RacePlayerStats[playerid][race_player_cp]+1),GetRaceCPZ(raceid,RacePlayerStats[playerid][race_player_cp]+1),CPSize);

	if (RaceStats[raceid][race_state] == RACE_STATE_LINEUP)
	{
		format(string, sizeof(string),gettext(527),(Race[raceid][race_lineupdelay] - RaceStats[raceid][race_timer]));
		SystemMsg(playerid,COLOUR_RACE,string);
	}

	if(PlayerState != PLAYER_STATE_DRIVER && PlayerState != PLAYER_STATE_PASSENGER)
	{
		SystemMsg(playerid,COLOUR_RACE_BAD,gettext(528));
	}
	else if(PlayerState == PLAYER_STATE_PASSENGER)
	{
		SystemMsg(playerid,COLOUR_RACE_BAD,gettext(529));
	}

	new playervehiclemodel = GetVehicleModel(GetPlayerVehicleID(playerid));
	if (playervehiclemodel >= 400)
	{
		if (IsVehicleAllowedInRace(raceid,playervehiclemodel) == 0)
		{
			SystemMsg(playerid,COLOUR_RACE_BAD,gettext(530));
			SendPlayerAllowedRaceVehicles(playerid,raceid);
		}
	}
	Debug("race.inc > JoinRace - Stop");
}

SendPlayerAllowedRaceVehicles(playerid,raceid)
{
	Debug("race.inc > SendPlayerAllowedRaceVehicles - Start");
	new string[MAX_STRING];
	if (Race[raceid][race_vehicles][0] == 0) return;
	if (strlen(Race[raceid][race_vehicles]) <= 10)
	{
		set(string,gettext(531));
		for (new i=0;i<strlen(Race[raceid][race_vehicles]);i++)
		{
			if (Race[raceid][race_vehicles][i] == 0)  break;
			new carmodel = Race[raceid][race_vehicles][i];
			if (i>0) strins(string, ", ", strlen(string));
			strins(string, GetVehicleName(carmodel), strlen(string));
		}
		SystemMsg(playerid,COLOUR_RACE,string);
	}
	else
	{
		SystemMsgScrolling(playerid,gettext(531));
		for (new i=0;i<strlen(Race[raceid][race_vehicles]);i++)
		{
			if (Race[raceid][race_vehicles][i] == 0)  break;
			new carmodel = Race[raceid][race_vehicles][i];
			format(string,sizeof(string),"%s",GetVehicleName(carmodel));
			SystemMsgScrolling(playerid,string);
		}
	}
	Debug("race.inc > SendPlayerAllowedRaceVehicles - Stop");
}

LeaveRace(raceid,playerid)
{
	Debug("race.inc > LeaveRace - Start");
	new string[MAX_STRING];
	ResetQuest(playerid);  // reset checkpoints
	RaceStats[raceid][race_racercount]--;
	RacePlayerStats[playerid][race_player_cp] = 0;
	RacePlayerStats[playerid][race_player_time] = 0;

	if (Set_Virtual_World_For_Races == 1 && LastRaceVehicle[playerid] != INVALID_VEHICLE_ID){
		SetVehicleVirtualWorld(LastRaceVehicle[playerid],WORLD_DEFAULT);
		SetPlayerVirtualWorld(playerid,WORLD_DEFAULT);
	}
	LastRaceVehicle[playerid] = INVALID_VEHICLE_ID;

	if (!IsPlayerConnected(playerid)) {Debug("race.inc > LeaveRace - Stop"); return;}
	if (RaceStats[raceid][race_state] == RACE_STATE_LINEUP)
	{
		if (Race[raceid][race_minracers] > 1)
		{
			format(string, sizeof(string),"(race) %s has left the race. (Racers:%d/%d)", oGetPlayerName(playerid),RaceStats[raceid][race_racercount],Race[raceid][race_minracers]);
			new logstring[256];
			format(logstring, sizeof (logstring), "player: %d:  %s: has left the race. (Racers:%d/%d)",playerid,oGetPlayerName(playerid),RaceStats[raceid][race_racercount],Race[raceid][race_minracers]);
			WriteLog(logstring);
		}
		else
		{
			format(string, sizeof(string),"(race) %s has left the race. (Racers:%d)", oGetPlayerName(playerid),RaceStats[raceid][race_racercount]);
			new logstring[256];
			format(logstring, sizeof (logstring), "player: %d:  %s: has left the race. (Racers:%d)",playerid,oGetPlayerName(playerid),RaceStats[raceid][race_racercount]);
			WriteLog(logstring);
		}
		format(string, sizeof(string),gettext(532), oGetPlayerName(playerid),Race[raceid][race_name],RaceStats[raceid][race_racercount]);
		SendMessageToRacers(raceid,string);
		oGivePlayerMoney(playerid,Race[raceid][race_cashentry],1); // give player back thier cash entry
		ShowTextForPlayer(playerid,gettext(533),5000,1);
		SystemMsg(playerid,COLOUR_RACE_BAD,gettext(1474));
		TogglePlayerControllable(playerid,1);
	}
	else
	{
		format(string, sizeof(string),"(race) %s has left the race. (Racers:%d)", oGetPlayerName(playerid),RaceStats[raceid][race_racercount]);
		SendMessageToRacers(raceid,string);
		SystemMsg(playerid,COLOUR_RACE_BAD,gettext(534));
	}
	TextDrawHideForPlayer(playerid, Ttime);
	TextDrawHideForPlayer(playerid, Tplayercps[playerid]);
	TextDrawDestroy(Tplayercps[playerid]);

	if (RaceStats[raceid][race_racercount] <= 0){
		CleanupRace(raceid);
	}
	Debug("race.inc > LeaveRace - Stop");
}

SendMessageToRacers(raceid,string[])
{
	Debug("race.inc > SendMessageToRacers - Start");
	for (new racerid=0; racerid<MAX_PLAYERS_EX;racerid++)
	{
		if (!IsPlayerConnected(racerid)) continue;
		if (PlayerQuest[racerid] == GetRaceQuestID(raceid)) // if player is in this race
		{
			SystemMsg(racerid,COLOUR_RACE,string);
		}
	}
	Debug("race.inc > SendMessageToRacers - Stop");
}

IsPlayerInRace(playerid,raceid)
{
	Debug("race.inc > IsPlayerInRace - Start");
	if (PlayerQuest[playerid] == GetRaceQuestID(raceid)) {
	Debug("race.inc > IsPlayerInRace - Stop");
	return 1;
	}
	Debug("race.inc > IsPlayerInRace - Stop");
	return 0;
}


GetPlayerRace(playerid)
{
	Debug("race.inc > GetPlayerRace - Start");
	if (!IsPlayerConnected(playerid)) return 0;
	for (new raceid=1; raceid<=RacesCount;raceid++)
	{
		if (RaceStats[raceid][race_state] == RACE_STATE_DISABLED) continue;
		if (PlayerQuest[playerid] == GetRaceQuestID(raceid)) // if player is in this race
		{
			return raceid;
		}
	}
	Debug("race.inc > GetPlayerRace - Stop");
	return 0;
}

GetRaceQuestID(raceid)
{
	return RaceStats[raceid][race_questid];
}

CleanupRace(raceid)
{
	Debug("race.inc > CleanupRace - Start");
	for (new playerid=0; playerid<MAX_PLAYERS_EX;playerid++)
	{
		if (!IsPlayerConnected(playerid)) continue;
		if (PlayerQuest[playerid] == GetRaceQuestID(raceid)) // if player is in this race
		{
			LeaveRace(raceid,playerid);
		}
		if (IsPlayerConnected(playerid))
		{
			oDisablePlayerRaceCheckpoint(playerid);
		}
		if (Set_Virtual_World_For_Races == 1 && LastRaceVehicle[playerid] != INVALID_VEHICLE_ID ){
			SetVehicleVirtualWorld(LastRaceVehicle[playerid],WORLD_DEFAULT);
			SetPlayerVirtualWorld(playerid,WORLD_DEFAULT);
		}
		LastRaceVehicle[playerid] = INVALID_VEHICLE_ID;
	}
	RaceStats[raceid][race_position] = 0;
	RaceStats[raceid][race_racercount] = 0;
	RaceStats[raceid][race_timer] = MakeRaceSleepTime(raceid);
	RaceStats[raceid][race_state] = RACE_STATE_SLEEPING;
	event_now=false;
	if (RaceStats[raceid][race_activecpid] != 0)
	{
		RemoveActiveRaceCheckpoint(RaceStats[raceid][race_activecpid]);
		RaceStats[raceid][race_activecpid] = 0;
	}
	Debug("race.inc > CleanupRace - Stop");
}

NextRaceCP(raceid,playerid)
{
	Debug("race.inc > NextRaceCP - Start");
	if (RacePlayerStats[playerid][race_player_cp] == RaceSize[raceid]-1) // if finish line
	{
		FinishRace(raceid,playerid);
		Debug("race.inc > NextRaceCP - Stop");
		return;
	}

	PlayerPlaySoundOnPlayer(playerid,1058);
	RacePlayerStats[playerid][race_player_cp]++;
	new tmp[MAX_STRING];
	if (RacePlayerStats[playerid][race_player_cp] < 10){
		format(tmp,MAX_STRING,"0%d/%d",RacePlayerStats[playerid][race_player_cp],RaceSize[raceid]);
	} else {
		format(tmp,MAX_STRING,"%d/%d",RacePlayerStats[playerid][race_player_cp],RaceSize[raceid]);
	}
	TextDrawSetString(Tplayercps[playerid],tmp);
	if (RacePlayerStats[playerid][race_player_cp] == RaceSize[raceid]-1){ //Almost finish
		oSetPlayerRaceCheckpoint(playerid,1,GetRaceCPX(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPY(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPZ(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPX(raceid,RacePlayerStats[playerid][race_player_cp]+1),GetRaceCPY(raceid,RacePlayerStats[playerid][race_player_cp]+1),GetRaceCPZ(raceid,RacePlayerStats[playerid][race_player_cp]+1),CPSize);
	} else {
		oSetPlayerRaceCheckpoint(playerid,0,GetRaceCPX(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPY(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPZ(raceid,RacePlayerStats[playerid][race_player_cp]),GetRaceCPX(raceid,RacePlayerStats[playerid][race_player_cp]+1),GetRaceCPY(raceid,RacePlayerStats[playerid][race_player_cp]+1),GetRaceCPZ(raceid,RacePlayerStats[playerid][race_player_cp]+1),CPSize);
	}
	Debug("race.inc > NextRaceCP - Stop");
}

StartRaceCountdown(raceid)
{
	Debug("race.inc > StartRaceCountdown - Start");
	RaceStats[raceid][race_timer]=0;
	if (RaceStats[raceid][race_activecpid] != 0)
	{
		RemoveActiveRaceCheckpoint(RaceStats[raceid][race_activecpid]);
		RaceStats[raceid][race_activecpid] = 0;
	}
	new string[MAX_STRING];
	for (new playerid=0; playerid<MAX_PLAYERS_EX;playerid++)
	{
		if (!IsPlayerConnected(playerid)) continue;
		if (PlayerQuest[playerid] == GetRaceQuestID(raceid)) // if player is in this race
		{
			if (!oIsPlayerInRaceCheckpoint(playerid,GetRaceCPX(raceid,0),GetRaceCPY(raceid,0),GetRaceCPZ(raceid,0),CPSize*2))
			{
				LeaveRace(raceid,playerid);
				SystemMsg(playerid,COLOUR_RACE_BAD,gettext(540));
			}
		}
	}

	if (RaceStats[raceid][race_racercount] < Race[raceid][race_minracers])
	{
		format(string, sizeof(string),gettext(541),Race[raceid][race_name]);
		if (PlayerCount() > 0)
		{
			new logstring[256];
			format(logstring, sizeof (logstring), "Race '%s' did not run. Not enough players.",Race[raceid][race_name]);
			WriteLog(logstring);
		}
		SystemMsgToRegistered(COLOUR_RACE_BAD,string);
		CleanupRace(raceid);
		return;
	}

	format(string, sizeof(string),gettext(542),Race[raceid][race_name]);
	SystemMsgToRegistered(COLOUR_RACE_BAD,string);
	new logstring[256];
	format(logstring, sizeof (logstring), "Race '%s' starting.",Race[raceid][race_name]);
	WriteLog(logstring);
	RaceStats[raceid][race_state] = RACE_STATE_COUNTDOWN;

	for (new playerid=0; playerid<MAX_PLAYERS_EX;playerid++)
	{
		if (!IsPlayerConnected(playerid)) continue;
		if (PlayerQuest[playerid] == GetRaceQuestID(raceid)) // if player is in this race
		{
			TogglePlayerControllable(playerid, 0);
			if (!IsPlayerInAnyVehicle(playerid)){
				LeaveRace(raceid,playerid);
				SystemMsg(playerid,COLOUR_RACE_BAD,gettext(543));
				TogglePlayerControllable(playerid, 1);
				
			}
			if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER)
			{
				LeaveRace(raceid,playerid);
				SystemMsg(playerid,COLOUR_RACE_BAD,gettext(544));
				TogglePlayerControllable(playerid, 1);
			}		
			// Still in race. Additional check.
			if (GetPlayerRace(playerid) == raceid){
				LastRaceVehicle[playerid] = GetPlayerVehicleID(playerid);
				SetVehicleZAngle(LastRaceVehicle[playerid], Race[raceid][race_startheading]);
				SetCameraBehindPlayer(playerid);
				if (Set_Virtual_World_For_Races == 1){
					SetPlayerVirtualWorld(playerid,WORLD_RACE);
					SetVehicleVirtualWorld(LastRaceVehicle[playerid],WORLD_RACE);
				}
				NosCar(GetPlayerVehicleID(playerid));
			}
		} else {
			oDisablePlayerRaceCheckpoint(playerid);
		}
	}
	Debug("race.inc > StartRaceCountdown - Stop");
}

EndRace(raceid)
{
	Debug("race.inc > EndRace - Start");
	for (new playerid=0; playerid<MAX_PLAYERS_EX;playerid++)
	{
		if (!IsPlayerConnected(playerid)) continue;
		if (PlayerQuest[playerid] == GetRaceQuestID(raceid)) // if player is in this race
		{
			ShowTextForPlayer(playerid,gettext(545),5000,1);
			SystemMsg(playerid,COLOUR_RACE_BAD,gettext(546));
			new logstring[256];
			format(logstring, sizeof (logstring), "player: %d:  %s: his race ended. He failed to quilify.",playerid,oGetPlayerName(playerid));
			WriteLog(logstring);
			GivePlayerXP(playerid,0-(Race[raceid][race_xpprize]/3),1);
		}
	}
	event_now=false;
	CleanupRace(raceid);
	Debug("race.inc > EndRace - Stop");
}


FinishRace(raceid,playerid)
{
	Debug("race.inc > FinishRace - Start");
	RaceStats[raceid][race_position]++;
	new string[MAX_STRING];
	RacePlayerStats[playerid][race_player_time] = RaceStats[raceid][race_timer];
	
	if (RaceStats[raceid][race_position] == 1)
	{
		format(string, sizeof(string),gettext(547),Race[raceid][race_name],FormatPosition(RaceStats[raceid][race_position]),ConvertSeconds(RacePlayerStats[playerid][race_player_time]));
		SystemMsg(playerid,COLOUR_RACE,string);
		Player[playerid][RacesWon]++;
		format(string, sizeof(string),gettext(548),oGetPlayerName(playerid),Race[raceid][race_name],ConvertSeconds(RacePlayerStats[playerid][race_player_time]));
		SystemMsgToRegistered(COLOUR_RACE,string);
		new logstring[256];
		format(logstring, sizeof (logstring), "player: %d:  %s: has won race '%s'! Time: %s",playerid,oGetPlayerName(playerid),Race[raceid][race_name],ConvertSeconds(RacePlayerStats[playerid][race_player_time]));
		WriteLog(logstring);
		oGivePlayerMoney(playerid,Race[raceid][race_cashprize],1);
		GivePlayerXP(playerid,Race[raceid][race_xpprize],1);
		SetPlayerCriminal(playerid,gettext(1387),13);
		BetWinner(playerid,1);

	}
	else if (RaceStats[raceid][race_position] == 2)
	{
		format(string, sizeof(string),gettext(549),Race[raceid][race_name],FormatPosition(RaceStats[raceid][race_position]),ConvertSeconds(RacePlayerStats[playerid][race_player_time]));
		SystemMsg(playerid,COLOUR_RACE,string);

		format(string, sizeof(string),gettext(550),oGetPlayerName(playerid),FormatPosition(RaceStats[raceid][race_position]),Race[raceid][race_name],ConvertSeconds(RacePlayerStats[playerid][race_player_time]));
		SystemMsgToRegistered(COLOUR_RACE,string);
		oGivePlayerMoney(playerid,(Race[raceid][race_cashprize]/2),1);
		GivePlayerXP(playerid,(Race[raceid][race_xpprize]/2),1);
		SetPlayerCriminal(playerid,gettext(1387),10);
	}
	else if ((RaceStats[raceid][race_position] == 3) && (RaceStats[raceid][race_racercount] > 3))
	{
		format(string, sizeof(string),gettext(551),Race[raceid][race_name],FormatPosition(RaceStats[raceid][race_position]),ConvertSeconds(RacePlayerStats[playerid][race_player_time]));
		SystemMsg(playerid,COLOUR_RACE,string);
		format(string, sizeof(string),gettext(552),oGetPlayerName(playerid),FormatPosition(RaceStats[raceid][race_position]),Race[raceid][race_name],ConvertSeconds(RacePlayerStats[playerid][race_player_time]));
		SystemMsgToRegistered(COLOUR_RACE,string);
		oGivePlayerMoney(playerid,(Race[raceid][race_cashprize]/3),1);
		GivePlayerXP(playerid,(Race[raceid][race_xpprize]/3),1);
		SetPlayerCriminal(playerid,gettext(1387),7);
	}
	else
	{
		format(string, sizeof(string),gettext(553),Race[raceid][race_name],FormatPosition(RaceStats[raceid][race_position]),ConvertSeconds(RacePlayerStats[playerid][race_player_time]));
		SystemMsg(playerid,COLOUR_RACE,string);
		GivePlayerXP(playerid,((Race[raceid][race_xpprize]/3)/4),1);
		SetPlayerCriminal(playerid,gettext(1387),5);
	}

	string = FormatPosition(RaceStats[raceid][race_position]);
	ShowTextForPlayer(playerid,string,5000,1);
	PlayerPlaySoundOnPlayer(playerid,1057);

	if ((RaceStats[raceid][race_timer] < BestScore[raceid][race_score_time]) || (BestScore[raceid][race_score_time] == 0))
	{
		format(string, sizeof(string),gettext(554),oGetPlayerName(playerid),Race[raceid][race_name],ConvertSeconds(RacePlayerStats[playerid][race_player_time]),ConvertSeconds(BestScore[raceid][race_score_time]));
		SystemMsgToRegistered(COLOUR_RACE,string);
		new logstring[256];
		format(logstring, sizeof (logstring), "player: %d:  %s: has set a new record for race '%s'! Time: %s. Old record: %s.",playerid,oGetPlayerName(playerid),Race[raceid][race_name],ConvertSeconds(RacePlayerStats[playerid][race_player_time]),ConvertSeconds(BestScore[raceid][race_score_time]));
		WriteLog(logstring);
		oGivePlayerMoney(playerid,(Race[raceid][race_cashprize]),1);
		GivePlayerXP(playerid,(Race[raceid][race_xpprize]),1);
		BestScore[raceid][race_score_time] = RaceStats[raceid][race_timer];
		set(BestScore[raceid][race_score_player],oGetPlayerName(playerid));
		new playervehiclemodel = GetVehicleModel(GetPlayerVehicleID(playerid));
		BestScore[raceid][race_score_vehicle]  = GetVehicleModel(playervehiclemodel);
	}

	LeaveRace(raceid,playerid);
	RaceSaveRecordDB(raceid);
	Debug("race.inc > FinishRace - Stop");
}

StartRaceLineup(raceid)
{
	Debug("race.inc > StartRaceLineup - Start");
	RaceStats[raceid][race_state] = RACE_STATE_LINEUP;
	new string1[MAX_STRING];
	new string2[MAX_STRING];
	event_now=true;
	format(string1, sizeof(string1),gettext(555),Race[raceid][race_name],raceid,ConvertSeconds(Race[raceid][race_lineupdelay]),GetRaceCPZoneName(raceid,0));
	if (Race[raceid][race_minracers] > 1)
	{
		format(string2, sizeof(string2),gettext(556),Race[raceid][race_cashentry],Race[raceid][race_minracers],FormatTimeForEvent(Race[raceid][race_maxracetime]));
	}
	else
	{
		format(string2, sizeof(string2),gettext(557),Race[raceid][race_cashentry],FormatTimeForEvent(Race[raceid][race_maxracetime]));
	}
	for (new playerid=0; playerid<MAX_PLAYERS_EX;playerid++)
	{
		if (!IsPlayerConnected(playerid)) continue;
		if (Player[playerid][GotJob] == JOB_COP) continue;
		SystemMsg(playerid,COLOUR_RACE,string1);
		SystemMsg(playerid,COLOUR_RACE,string2);
		SendPlayerAllowedRaceVehicles(playerid,raceid);
		ShowTextForPlayer(playerid,gettext(558), 5000, 1);
		oSetPlayerRaceCheckpoint(playerid,2,GetRaceCPX(raceid,0),GetRaceCPY(raceid,0),GetRaceCPZ(raceid,0),GetRaceCPX(raceid,1),GetRaceCPY(raceid,1),GetRaceCPZ(raceid,1),CPSize);
	}
	RaceStats[raceid][race_activecpid] = AddActiveRaceCheckpoint(2,GetRaceCPX(raceid,0),GetRaceCPY(raceid,0),GetRaceCPZ(raceid,0),GetRaceCPX(raceid,1),GetRaceCPY(raceid,1),GetRaceCPZ(raceid,1),10000,10);
	Debug("race.inc > StartRaceLineup - Stop");

}

GetRaceCPZoneName(raceid,cpid)
{
	new zone[MAX_STRING];
	zone = GetXYZZoneName(GetRaceCPX(raceid,cpid),GetRaceCPY(raceid,cpid),GetRaceCPZ(raceid,cpid));
	return zone;
}

public Race_OnPlayerEnterRaceCP(playerid) //Fuck the cycle... I use this checkpoint type only in races
{
	for (new raceid=1; raceid<=RacesCount;raceid++) //  for each race
	{
		//if (RaceStats[raceid][race_state] == RACE_STATE_DISABLED) break;
		
		if (RaceStats[raceid][race_state] == RACE_STATE_LINEUP)
		{
			if (PlayerQuest[playerid] == 0 && Player[playerid][GotJob] != JOB_COP) // if player not on a quest
			{
				// if player at startline
				if (oIsPlayerInRaceCheckpoint(playerid,GetRaceCPX(raceid,0),GetRaceCPY(raceid,0),GetRaceCPZ(raceid,0),CPSize*2))
				{
					if (!IsPlayerInRace(playerid,raceid))
					{
						JoinRace(raceid,playerid);
					}
					else
					{
						new playervehiclemodel = GetVehicleModel(GetPlayerVehicleID(playerid));
						if (IsVehicleAllowedInRace(raceid,playervehiclemodel) == 0)
						{
							SystemMsg(playerid,COLOUR_RACE_BAD,gettext(559));
							SendPlayerAllowedRaceVehicles(playerid,raceid);
						}
						else
						{
							SystemMsg(playerid,COLOUR_RACE,gettext(560));
						}
					}
				}
			}
		}
	}
	new raceid = GetPlayerRace(playerid);
	if (RaceStats[raceid][race_state] == RACE_STATE_RACING)
	{
		//if (oIsPlayerInRaceCheckpoint(playerid,GetRaceCPX(raceid,RacePlayerStats[playerid][race_player_cp]-1),GetRaceCPY(raceid,RacePlayerStats[playerid][race_player_cp]-1),GetRaceCPZ(raceid,RacePlayerStats[playerid][race_player_cp]-1),CPSize*2))
		//{ Temp fix... Still
				NextRaceCP(raceid,playerid);
		//}
	}
}

IsVehicleAllowedInRace(raceid,vehiclemodel)
{
	Debug("race.inc > IsVehicleAllowedInRace - Start");
	if (Race[raceid][race_vehicles][0] == 0){ Debug("race.inc > IsVehicleAllowedInRace - Stop"); return 1;}
	
	for (new i=0;i<strlen(Race[raceid][race_vehicles]);i++)
	{
		if (vehiclemodel == Race[raceid][race_vehicles][i])
		{
			Debug("race.inc > IsVehicleAllowedInRace - Stop");
			return 1;
		}
	}
	Debug("race.inc > IsVehicleAllowedInRace - Stop");
	return 0;
}

stock NosCar(carid)
{
	Debug("race.inc > NosCar - Start");
	RepairVehicle(carid);
	new model;
	model = GetVehicleModel(carid);
	switch(model)
	{
		case 407,578,579,580,582,583,584,585,587,588,589,596,
		597,400,401,420,534,535,536,556,557,558,559,560,561,
		598,599,600,601,602,603,604,605,609,568,572,574,402,
		403,404,405,408,409,410,411,412,413,414,415,416,418,
		419,421,422,423,424,426,427,428,429,431,432,433,434,
		436,437,438,439,440,441,442,443,444,445,451,455,456,
		457,458,459,466,467,470,474,475,477,478,479,480,482,
		483,485,486,489,492,494,495,496,498,499,500,502,503,
		504,505,506,507,508,514,515,516,517,518,524,525,526,
		527,528,529,530,531,532,533,540,541,542,543,566,544,
		545,546,547,549,550,551,552,554,555,562,565,567,575,
		576:
		{
			AddVehicleComponent(carid,1010);//NOS
			CarHaveNOS[carid] = true;
		}

	}
	Debug("race.inc > NosCar - Stop");
}

public Race_OnPlayerStateChange(playerid,newstate,oldstate)
{
	Debug("race.inc > Race_OnPlayerStateChange - Start");
	if (oldstate == PLAYER_STATE_DRIVER && (newstate == PLAYER_STATE_ONFOOT || newstate == PLAYER_STATE_WASTED)){
		new raceid = GetPlayerRace(playerid);
		if (raceid != INVALID_RACE_ID && event_now == true){
			if (RaceStats[raceid][race_state] == RACE_STATE_RACING){
				LeaveRace(raceid,playerid);
			}
		}
	}
	Debug("race.inc > Race_OnPlayerStateChange - Stop");
	return 0;

}

public Race_OnPlayerDisconnect(playerid){
	Debug("race.inc > Race_OnPlayerDisconnect - Start");
	if (GetPlayerRace(playerid) != INVALID_RACE_ID){
	    LeaveRace(GetPlayerRace(playerid),playerid);
	}
	LastRaceVehicle[playerid] = INVALID_VEHICLE_ID;
	Debug("race.inc > Race_OnPlayerDisconnect - Stop");
}

stock CalculateRaceDistance(raceid){
	new Float:oldx,Float:oldy,Float:oldz;
	new totaldist = 0;
	for (new cpid=0;cpid<RaceSize[raceid];cpid++)
	{
		totaldist = totaldist + GetPointDistanceToPointEx(RaceCheckpoints[raceid][cpid][Coord_X],RaceCheckpoints[raceid][cpid][Coord_Y],RaceCheckpoints[raceid][cpid][Coord_Z],oldx,oldy,oldz);
		oldx = RaceCheckpoints[raceid][cpid][Coord_X];
		oldy = RaceCheckpoints[raceid][cpid][Coord_Y];
		oldz = RaceCheckpoints[raceid][cpid][Coord_Z];
	}
	return totaldist;
}

stock CalculateRewardXP(raceid){
	new distance = CalculateRaceDistance(raceid);
	new reward = 0;
	if (distance < 1000){
		reward = 500;
	} else if (distance < 2000){
		reward = 1000;
	} else if (distance < 3000){
		reward = 1500;
	} else if (distance < 4000){
		reward = 2000;
	} else if (distance < 5000){
		reward = 2500;
	} else if (distance < 6000){
		reward = 3000;
	} else if (distance < 7000){
		reward = 3500;
	} else if (distance < 8000){
		reward = 4000;
	} else if (distance < 9000){
		reward = 4500;
	} else if (distance < 10000){
		reward = 5000;
	} else if (distance < 11000){
		reward = 5500;
	} else if (distance < 12000){
		reward = 6000;
	} else if (distance < 13000){
		reward = 6500;
	} else if (distance < 14000){
		reward = 7000;
	} else if (distance < 15000){
		reward = 7500;
	} else if (distance < 30000){
		reward = 15000;
	} else {
		reward = 20000;
	}
	return reward;
}

stock CalculateRewardMoney(raceid){
	new distance = CalculateRaceDistance(raceid);
	new reward = 0;
	if (distance < 1000){
		reward = 1000;
	} else if (distance < 2000){
		reward = 2000;
	} else if (distance < 3000){
		reward = 3000;
	} else if (distance < 4000){
		reward = 4000;
	} else if (distance < 5000){
		reward = 5000;
	} else if (distance < 6000){
		reward = 6000;
	} else if (distance < 7000){
		reward = 7000;
	} else if (distance < 8000){
		reward = 8000;
	} else if (distance < 9000){
		reward = 9000;
	} else if (distance < 10000){
		reward = 10000;
	} else if (distance < 11000){
		reward = 11000;
	} else if (distance < 12000){
		reward = 12000;
	} else if (distance < 13000){
		reward = 13000;
	} else if (distance < 14000){
		reward = 14000;
	} else if (distance < 15000){
		reward = 15000;
	} else if (distance < 30000){
		reward = 50000;
	} else {
		reward = 75000;
	}
	return reward;
}

stock CalculateEntryFee(raceid){
	new distance = CalculateRaceDistance(raceid);
	new efee = 0;
	if (distance < 1000){
		efee = 100;
	} else if (distance < 2000){
		efee = 200;
	} else if (distance < 3000){
		efee = 300;
	} else if (distance < 4000){
		efee = 400;
	} else if (distance < 5000){
		efee = 500;
	} else if (distance < 6000){
		efee = 600;
	} else if (distance < 7000){
		efee = 700;
	} else if (distance < 8000){
		efee = 800;
	} else if (distance < 9000){
		efee = 900;
	} else if (distance < 10000){
		efee = 1000;
	} else if (distance < 11000){
		efee = 1100;
	} else if (distance < 12000){
		efee = 1200;
	} else if (distance < 13000){
		efee = 1300;
	} else if (distance < 14000){
		efee = 1400;
	} else if (distance < 15000){
		efee = 1500;
	} else if (distance < 30000){
		efee = 4500;
	} else {
		efee = 7500;
	}
	return efee;
}

stock CalculateMaxRaceTime(raceid){
	new curtime = 10;
	new time = 0;
	new distance = CalculateRaceDistance(raceid);
	if (Race[raceid][race_vehicles][0] == 0){
		curtime = floatround(((float(distance) / (1000 * 200)) * 3600));
	} else {
		for (new i=0;i<strlen(Race[raceid][race_vehicles]);i++)
		{
			time = floatround((float(distance) / (1000 * GetVehicleSpeed(Race[raceid][race_vehicles][i]))) * 3600);
			if (time > curtime) curtime = time;
			
		}
	}
	return curtime+60;
}

IsServerGotVehiclesForRace(raceid)
{
	Debug("race.inc > IsServerGotVehiclesForRace - Start");
	if (Race[raceid][race_vehicles][0] == 0){Debug("race.inc > IsServerGotVehiclesForRace - Stop");return 1;}
	
	new vehicles = 0;
	for (new i=0;i<strlen(Race[raceid][race_vehicles]);i++)
	{
		vehicles = vehicles + ActiveVehicleModelCount(Race[raceid][race_vehicles][i]);
	}
	if (vehicles < (Race[raceid][race_minracers] * 4)){
		Debug("race.inc > IsServerGotVehiclesForRace - Stop");
		return 0;
	} else {
		Debug("race.inc > IsServerGotVehiclesForRace - Stop");
		return 1;
	}
}

RaceEncodeVehiclesDB(raceid)
{
	Debug("race.inc > RaceEncodeVehiclesDB - Start");
	new vehs[MAX_RACE_VEHICLES+1];
	new retstr[MAX_STRING];
	set(vehs,Race[raceid][race_vehicles]);
	for (new i=0;i<strlen(vehs);i++)
	{
		if (vehs[i] == 0) break;
		new tempstr[10];
		valstr(tempstr,vehs[i]);
		strins(retstr,tempstr,strlen(retstr));
		retstr[strlen(retstr)] = '/';
	}
	Debug("race.inc > RaceEncodeVehiclesDB - Stop");
	return retstr;
}

RaceDecodeVehiclesDB(vehstr[])
{
	Debug("race.inc > RaceDecodeVehiclesDB - Start");
	new vehs[MAX_RACE_VEHICLES];
	new tmpvehstr[20];
	new idx;

	for (new i=0;((i<strlen(vehstr)) && (i<MAX_RACE_VEHICLES));i++)
	{
		set(tmpvehstr,strcharsplit(vehstr, idx,'/'));
		if (strlen(tmpvehstr) == 0) break;
		vehs[i] = strval(tmpvehstr);
		if (vehs[i] == 0) break;
	}
	//set(Race[raceid][race_vehicles],vehs);
	Debug("race.inc > RaceDecodeVehiclesDB - Stop");
	return vehs;
}