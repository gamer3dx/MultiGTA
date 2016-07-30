//Deathmatch module.
//Created by Iain Gilbert
//Modder: MadCat
//Thanks to GTAT Team for idea for InfoTextDraw
//Idea for Sudden Explosion from GTAT Arena

#include "base"
#include "player"
#include "world"
#include "weapons"

forward DM_OnGameModeInit();
forward dmconfig();
forward CheckDM();
forward RegisterDM(name[]);
forward OnPlayerDMKill(playerid,victimid,weaponid);
forward DM_OnPlayerDeath(playerid,killerid);
forward DM_OnPlayerEnterCheckpoint(playerid);
forward DM_OnPlayerCommandText(playerid,text[]);

#define MAX_DMWEAPONS 10
#define MAX_DM 85
#define MAX_DM_SP 30
#define MAX_DM_OBJECTS 250

#define DM_STATE_DISABLED 0
#define DM_STATE_SLEEPING 1
#define DM_STATE_LINEUP 2
#define DM_STATE_COUNTDOWN 3
#define DM_STATE_ACTIVE 4
#define INVALID_DM_ID 0

enum DMInfo{
	dm_name[MAX_NAME],    // name of dm
	dm_minlevel,         // minimum level required to enter dm
	dm_minplayers,         // minimum dmplayers needed to dm
	dm_maxtime,       // max time player can take to complete a dm
	dm_frequency,        // frquency that dm runs
	dm_lineupdelay,       // time to wait for players to linup
	dm_cashprize,   // cash earned for 1st position
	dm_cashentry,   // Cash required to enter (earned by winner)
	dm_xpprize,    // xp earned for 1st position
	dm_xpbonus,   // xp earned per player still in dm
	dm_xpkill,          // xp gained for killing someone in dm
	dm_xpdeath,         // xp lost for dieing in dm
	dm_activedmjoin,         // can a player join this dm while it is currently running
	dm_interior,		// interior
	dm_suddenexplosiontime,	// time in seconds before sudden death. 0 - disabled
	dm_weapons[MAX_DMWEAPONS]   //array of dm weaponid's
}
new Deathmatch[MAX_DM][DMInfo];

enum DMStateInfo{
	dm_state,             // dm state
	dm_timer,
	dm_playercount,
	dm_questid,
	dm_activecpid
}
new DeathmatchStats[MAX_DM][DMStateInfo];

enum DMPlayerInfo{
	bool:dm_player_active, // is the player active and spawned in the deathmatch zone
	dm_player_kills,
	dm_player_deaths
}
new DMPlayerStats[MAX_PLAYERS][DMPlayerInfo];

enum DMScoreInfo {
	dm_score_player[MAX_NAME],
	dm_score_record,
	dm_score_kills,
	dm_score_deaths
};
new DMBestScore[MAX_DM][DMScoreInfo];

new DMCount; // count of active deathmatches
new DMCPSize=9; // size of checkpoints
new Float:DMSpawnPoints[MAX_DM][MAX_CP][CoordInfo]; // moo
new DMSpawnsSize[MAX_DM]; // number of spawnpoints
new DMStartCP[MAX_DM][CoordInfo];
new DMObjectsSize[MAX_DM];

new TimeBeforeExplosion[MAX_DM];

enum DMWBInfo{
	Float:x_max,
	Float:x_min,
	Float:y_max,
	Float:y_min
}

new DMWorldBounds[MAX_DM][DMWBInfo];

enum DMOInfo{
	dmomodel,
	Float:dmox,
	Float:dmoy,
	Float:dmoz,
	Float:dmorx,
	Float:dmory,
	Float:dmorz
}

new DMObjectInfo[MAX_DM][MAX_DM_OBJECTS][DMOInfo];

new DMObjectID[MAX_DM][MAX_DM_OBJECTS];

new Text:DM_TextDraw[MAX_PLAYERS];
new DM_TextDrawExists[MAX_PLAYERS];

Float:GetDMCPX(dmid)
{
	return DMStartCP[dmid][Coord_X];
}
Float:GetDMCPY(dmid)
{
	return DMStartCP[dmid][Coord_Y];
}
Float:GetDMCPZ(dmid)
{
	return DMStartCP[dmid][Coord_Z];
}

//--------------------------------------------------------------

new DeathmatchDB[MAX_STRING] = "MultiGTA/DMS/Deathmatches.list";
new DMBaseDB[MAX_STRING] = "MultiGTA/DMS/";
new DMRecordDB[MAX_STRING] = "MultiGTA/DMS/Records/";

new Hide_DM_Players_On_Map = 1;

new TimerCheckDM;

public DM_OnGameModeInit(){
	dmconfig();
	DeathmatchLoadAll();
	TimerCheckDM = SetTimer("CheckDM", 1000, 1);
	new logstring[MAX_STRING];
	format(logstring,MAX_STRING,"Deathmatches System Loaded. %d deathmatches loaded.",DMCount);
	WriteLog(logstring);
	return 0;
}

public dmconfig()
{
	Debug("deathmatch.inc > dmconfig - Start");
	if (!db_Exists(ConfigDB)) db_Create(ConfigDB);

	new temp[MAX_STRING];
	
	set(temp,db_Get(ConfigDB,"Hide_DM_Players_On_Map"));
	if (strlen(temp) > 0) Hide_DM_Players_On_Map = strval(temp); 
	else { valstr(temp,Hide_DM_Players_On_Map); db_Set(ConfigDB,"Hide_DM_Players_On_Map",temp); }
	
	Debug("deathmatch.inc > dmconfig - Stop");

}

public CheckDM() // must be ran by timer every second
{
 	Debug("deathmatch.inc > CheckDM - Start");
	new string [MAX_STRING];
	for (new dmid=1; dmid<=DMCount;dmid++)
	{

		if (DeathmatchStats[dmid][dm_state] == DM_STATE_DISABLED) continue;

		if (DeathmatchStats[dmid][dm_state] == DM_STATE_SLEEPING)
		{
			if (DeathmatchStats[dmid][dm_timer] <= 0) // if it is time to run
			{
				if (event_now == false){
					DeathmatchStats[dmid][dm_timer] = 0;
					StartDMLineup(dmid);
					continue;
				} else {
					DeathmatchStats[dmid][dm_timer] = MakeDMSleepTime(dmid);
				}
			}
			DeathmatchStats[dmid][dm_timer]--;
			continue;
		}

		DeathmatchStats[dmid][dm_timer]++;
		if (DeathmatchStats[dmid][dm_state] == DM_STATE_LINEUP)
		{
			if (DeathmatchStats[dmid][dm_timer] >= Deathmatch[dmid][dm_lineupdelay]) // if it is time to run
			{
				DeathmatchStats[dmid][dm_timer] = 0;
				StartDMCountdown(dmid);
			}
			else
			{
				if (DeathmatchStats[dmid][dm_playercount] > 0)
				{
					new dmcountdown = Deathmatch[dmid][dm_lineupdelay] - DeathmatchStats[dmid][dm_timer];
					if ((dmcountdown == 5) || (dmcountdown == 10) || (dmcountdown == 20) || (dmcountdown == 30) || (dmcountdown == 45) || (dmcountdown == 60) || (dmcountdown == 90))
					{
						format(string, sizeof(string),gettext(974),(Deathmatch[dmid][dm_lineupdelay] - DeathmatchStats[dmid][dm_timer]));
						SendMessageToDMPlayers(dmid,string);
					}
				}
			}
		}
		else if (DeathmatchStats[dmid][dm_state] == DM_STATE_COUNTDOWN)
		{
			for (new playerid=0; playerid<MAX_PLAYERS_EX;playerid++)
			{
				if (IsPlayerConnected(playerid))
				{
					if (PlayerQuest[playerid] == GetDMQuestID(dmid)) // if player is in this dm
					{
						if (DMPlayerStats[playerid][dm_player_active] == true)
						{
							new Countdown = MAX_COUNTDOWN - DeathmatchStats[dmid][dm_timer];
							if (Countdown >= 1)
							{
								format(string, sizeof(string), "%d",Countdown);
								PlayerPlaySoundOnPlayer(playerid,1056);
							}
							else
							{
								format(string, sizeof(string),gettext(975));
								PlayerPlaySoundOnPlayer(playerid,1057);
							}
							ShowTextForPlayer(playerid, string,1000,6);
						}
					}
				}
			}
			if (DeathmatchStats[dmid][dm_timer] >= MAX_COUNTDOWN)
			{
				StartDM(dmid);
			}
		}
		else if (DeathmatchStats[dmid][dm_state] == DM_STATE_ACTIVE)
		{
			if (TimeBeforeExplosion[dmid] != 0){
				TimeBeforeExplosion[dmid]--;
				if (TimeBeforeExplosion[dmid] <= 0){
					new placeid = random(DMSpawnsSize[dmid]);
					CreateExplosion(DMSpawnPoints[dmid][placeid][Coord_X]+random(10)-5,DMSpawnPoints[dmid][placeid][Coord_Y]+random(10)-5,DMSpawnPoints[dmid][placeid][Coord_Z],1,random(20));
					TimeBeforeExplosion[dmid] = Deathmatch[dmid][dm_suddenexplosiontime];
				}
			}
			if (DeathmatchStats[dmid][dm_timer] >= Deathmatch[dmid][dm_maxtime]) // if time limit reached
			{
				EndDM(dmid);
			}
		}
	}
	Debug("deathmatch.inc > CheckDM - Stop");
}

DM_OnPlayerDisconnect(playerid)
{
	Debug("deathmatch.inc > DM_OnPlayerDisconnect - Start");
	new dmid = GetPlayerDM(playerid);
	if (dmid != INVALID_DM_ID) PlayerLeaveDM(playerid,dmid);
	Debug("deathmatch.inc > DM_OnPlayerDisconnect - Stop");
	return 0;
}

public RegisterDM(name[])
{
	Debug("deathmatch.inc > RegisterDM - Start");
	for (new i=0;i<MAX_DM;i++)
	{
		if (DeathmatchStats[i][dm_state] == DM_STATE_DISABLED) continue;
		if (strcomp(name,Deathmatch[i][dm_name],true)==1)
		{
			Debug("deathmatch.inc > RegisterDM - Stop");
			return INVALID_DM_ID;
		}
	}
	if (DMCount >= MAX_DM) return INVALID_DM_ID;
	DMCount++;
	new dmid = DMCount;
	DeathmatchStats[dmid][dm_questid] = RegisterQuest(name);
	if (DeathmatchStats[dmid][dm_questid] == INVALID_QUEST_ID)
	{
		DMCount--;
		Debug("deathmatch.inc > RegisterDM - Stop");
		return INVALID_DM_ID;
	}
	set(Deathmatch[dmid][dm_name],name);
	Debug("deathmatch.inc > RegisterDM - Stop");
	return dmid;
}


StartDMLineup(dmid)
{
	Debug("deathmatch.inc > StartDMLineup - Start");
	DeathmatchStats[dmid][dm_state] = DM_STATE_LINEUP;
	DeathmatchStats[dmid][dm_timer] = 0;
	DeathmatchStats[dmid][dm_playercount] = 0;
	new string1[MAX_STRING];
	new string2[MAX_STRING];
	event_now=true;
	format(string1, sizeof(string1),gettext(978),Deathmatch[dmid][dm_name],dmid,Deathmatch[dmid][dm_lineupdelay],GetDMStartZoneName(dmid));
	if (Deathmatch[dmid][dm_minplayers] > 1)
	{
		format(string2, sizeof(string2),gettext(979),Deathmatch[dmid][dm_cashentry],Deathmatch[dmid][dm_minlevel],Deathmatch[dmid][dm_minplayers]);
	}
	else
	{
		format(string2, sizeof(string2),gettext(980),Deathmatch[dmid][dm_cashentry],Deathmatch[dmid][dm_minlevel]);
	}
	for (new playerid=0; playerid<MAX_PLAYERS_EX;playerid++)
	{
		if (!IsPlayerConnected(playerid)) continue;
		SystemMsg(playerid,COLOUR_DM,string1);
		SystemMsg(playerid,COLOUR_DM,string2);
		ShowTextForPlayer(playerid,gettext(981), 5000, 1);
		if ((PlayerQuest[playerid] == 0) && (Player[playerid][GotJob] != JOB_COP)) // if player not on a quest
		{
			oSetPlayerCheckpoint(playerid,GetDMCPX(dmid),GetDMCPY(dmid),GetDMCPZ(dmid),DMCPSize);
			DMResetPlayerStats(playerid);
		} else {
			SystemMsg(playerid,COLOUR_DM_BAD,gettext(982));
			oDisablePlayerCheckpoint(playerid);
		}
	}
	DeathmatchStats[dmid][dm_activecpid] = AddActiveCheckpoint(GetDMCPX(dmid),GetDMCPY(dmid),GetDMCPZ(dmid),10000,8);
	Debug("deathmatch.inc > StartDMLineup - Stop");
}

StartDMCountdown(dmid)
{
	Debug("deathmatch.inc > StartDMCountdown - Start");
	DeathmatchStats[dmid][dm_timer]=0;
	//if (DeathmatchStats[dmid][dm_activecpid] != 0)
	//{
	// RemoveActiveCheckpoint(DeathmatchStats[dmid][dm_activecpid]);
	// DeathmatchStats[dmid][dm_activecpid] = 0;
	//}
	new string[MAX_STRING];


	for (new playerid=0; playerid<MAX_PLAYERS_EX;playerid++)
	{
		if (!IsPlayerConnected(playerid)) continue;
		if (PlayerQuest[playerid] == GetDMQuestID(dmid)) // if player is in this dm
		{
			if (!oIsPlayerInCheckpoint(playerid,GetDMCPX(dmid),GetDMCPY(dmid),GetDMCPZ(dmid),50))
			{
				//             if (Deathmatch[dmid][dm_activedmjoin] == 0)
				//             {
				PlayerLeaveDM(playerid,dmid);
				SystemMsg(playerid,COLOUR_DM_BAD,gettext(983));
				/*    }
	else
	{
	SystemMsg(playerid,COLOUR_DM,gettext(993));
	} */
			}
		}
	}

	for(new i=0; i<DMObjectsSize[dmid]; i++){
		DMObjectID[dmid][i] = CreateStreamObject(DMObjectInfo[dmid][i][dmomodel],DMObjectInfo[dmid][i][dmox],DMObjectInfo[dmid][i][dmoy],DMObjectInfo[dmid][i][dmoz],DMObjectInfo[dmid][i][dmorx],DMObjectInfo[dmid][i][dmory],DMObjectInfo[dmid][i][dmorz],WORLD_DM);
	}

	if (DeathmatchStats[dmid][dm_playercount] < Deathmatch[dmid][dm_minplayers])
	{
		format(string, sizeof(string),gettext(984),Deathmatch[dmid][dm_name]);
		SystemMsgToRegistered(COLOUR_DM_BAD,string);

		new logstring[MAX_STRING];
		format(logstring, MAX_STRING, "Deathmatch '%s' did not run. Not enough players.",Deathmatch[dmid][dm_name]);
		WriteLog(logstring);

		CleanupDM(dmid);
		Debug("deathmatch.inc > StartDMCountdown - Stop");
		return;
	}

	format(string, sizeof(string),gettext(985),Deathmatch[dmid][dm_name]);
	SystemMsgToRegistered(COLOUR_DM,string);
	new logstring[MAX_STRING];
	format(logstring, MAX_STRING, "Deathmatch '%s' starting.",Deathmatch[dmid][dm_name]);
	WriteLog(logstring);
	DeathmatchStats[dmid][dm_state] = DM_STATE_COUNTDOWN;

	for (new playerid=0; playerid<MAX_PLAYERS_EX;playerid++)
	{
		if (!IsPlayerConnected(playerid)) continue;
		if (PlayerQuest[playerid] == GetDMQuestID(dmid)) // if player is in this dm
		{
			if (oIsPlayerInCheckpoint(playerid,GetDMCPX(dmid),GetDMCPY(dmid),GetDMCPZ(dmid),50))
			{
				ShowDMInfo(playerid);
				TogglePlayerControllable(playerid, 0);
				SetCameraBehindPlayer(playerid);
				DM_OnPlayerSpawn(playerid,dmid);
			}
		}
	}
	Debug("deathmatch.inc > StartDMCountdown - Stop");
}

StartDM(dmid)
{
	Debug("deathmatch.inc > StartDM - Start");
	DeathmatchStats[dmid][dm_timer]=0;
	DeathmatchStats[dmid][dm_state] = DM_STATE_ACTIVE;
	TimeBeforeExplosion[dmid] = Deathmatch[dmid][dm_suddenexplosiontime];
	for (new playerid=0; playerid<MAX_PLAYERS_EX;playerid++)
	{
		if (IsPlayerConnected(playerid))
		{
			if (PlayerQuest[playerid] == GetDMQuestID(dmid)) // if player is in this dm
			{
				Player[playerid][DMsPlayed]++;
				TogglePlayerControllable(playerid, 1);
			}
		}
	}
	if (Deathmatch[dmid][dm_activedmjoin] == 0 && DeathmatchStats[dmid][dm_activecpid] != 0){
		RemoveActiveCheckpoint(DeathmatchStats[dmid][dm_activecpid]);
	}
	Debug("deathmatch.inc > StartDM - Stop");
}

JoinDM(playerid,dmid)
{
	Debug("deathmatch.inc > JoinDM - Start");
	new string[MAX_STRING];
	if (oGetPlayerMoney(playerid) < Deathmatch[dmid][dm_cashentry])
	{
		format(string, sizeof(string),gettext(986), Deathmatch[dmid][dm_cashentry]);
		SystemMsg(playerid,COLOUR_DM_BAD,string);
		Debug("deathmatch.inc > JoinDM - Stop");
		return;
	}
	if (GetPlayerLevel(playerid) < Deathmatch[dmid][dm_minlevel])
	{
		format(string, sizeof(string),gettext(987), Deathmatch[dmid][dm_minlevel]);
		SystemMsg(playerid,COLOUR_DM_BAD,string);
		Debug("deathmatch.inc > JoinDM - Stop");
		return;
	}
	DMResetPlayerStats(playerid);
	DeathmatchStats[dmid][dm_playercount]++;
	if (Deathmatch[dmid][dm_minplayers] > 1)
	{
		format(string, sizeof(string),gettext(988), oGetPlayerName(playerid),DeathmatchStats[dmid][dm_playercount],Deathmatch[dmid][dm_minplayers]);
		new logstring[MAX_STRING];
		format(logstring, MAX_STRING, "player: %d:  %s: has joined the deathmatch. (Players:%d/%d)",playerid,oGetPlayerName(playerid),DeathmatchStats[dmid][dm_playercount],Deathmatch[dmid][dm_minplayers]);
		WriteLog(logstring);
	}
	else
	{
		format(string, sizeof(string),gettext(989), oGetPlayerName(playerid),DeathmatchStats[dmid][dm_playercount]);
		new logstring[MAX_STRING];
		format(logstring, MAX_STRING, "player: %d:  %s: has joined the deathmatch. (Players:%d)",playerid,oGetPlayerName(playerid),DeathmatchStats[dmid][dm_playercount]);
		WriteLog(logstring);
	}
	SendMessageToDMPlayers(dmid,string);

	oGivePlayerMoney(playerid,0-Deathmatch[dmid][dm_cashentry],1);
	ShowTextForPlayer(playerid,gettext(990),3000,1);
	PlayerQuest[playerid] = GetDMQuestID(dmid);

	if (DeathmatchStats[dmid][dm_state] == DM_STATE_LINEUP)
	{
		format(string, sizeof(string),gettext(991),(Deathmatch[dmid][dm_lineupdelay] - DeathmatchStats[dmid][dm_timer]));
		SystemMsg(playerid,COLOUR_DM,string);
		oSetPlayerCheckpoint(playerid,GetDMCPX(dmid),GetDMCPY(dmid),GetDMCPZ(dmid),DMCPSize);
	}
	else if ((DeathmatchStats[dmid][dm_state] == DM_STATE_ACTIVE) && (Deathmatch[dmid][dm_activedmjoin] == 1))
	{
		SystemMsg(playerid,COLOUR_DM,gettext(992));
		if (oIsPlayerInCheckpoint(playerid,GetDMCPX(dmid),GetDMCPY(dmid),GetDMCPZ(dmid),50))
		{
			DM_OnPlayerSpawn(playerid,dmid);
			PlayerPlaySoundOnPlayer(playerid,1057);
			ShowTextForPlayer(playerid, gettext(975),1000,1);

		}
		else
		{
			SystemMsg(playerid,COLOUR_DM,gettext(994));
			oSetPlayerCheckpoint(playerid,GetDMCPX(dmid),GetDMCPY(dmid),GetDMCPZ(dmid),DMCPSize);
		}
	}
	else
	{
		SystemMsg(playerid,COLOUR_DM_BAD,gettext(995));
		Debug("deathmatch.inc > JoinDM - Stop");
		return;
	}
	Debug("deathmatch.inc > JoinDM - Stop");
}

DM_OnPlayerSpawn(playerid,dmid)
{
	Debug("deathmatch.inc > DM_OnPlayerSpawn - Start");
//	if (!IsPlayerInDM(playerid,dmid))
//	{
//		DMResetPlayerStats(playerid);
//		Debug("deathmatch.inc > DM_OnPlayerSpawn - Stop");
//		return;
//	}

	SetPlayerInterior(playerid,Deathmatch[dmid][dm_interior]);
	new spid = random(DMSpawnsSize[dmid]);
	oSetPlayerPos(playerid,DMSpawnPoints[dmid][spid][Coord_X], DMSpawnPoints[dmid][spid][Coord_Y], DMSpawnPoints[dmid][spid][Coord_Z]); // Warp the player

	oSetPlayerHealth(playerid,GetMaxHealth(playerid)); // set health based on level
	oSetPlayerMoney(playerid,Player[playerid][Money]);
	GivePlayerDMWeapons(playerid,dmid);
	DMPlayerStats[playerid][dm_player_active] = true;
	oDisablePlayerCheckpoint(playerid);
	SetPlayerVirtualWorld(playerid,WORLD_DM);
	SetPlayerWorldBounds(playerid,DMWorldBounds[dmid][x_max],DMWorldBounds[dmid][x_min],DMWorldBounds[dmid][y_max],DMWorldBounds[dmid][y_min]);
	if (Hide_DM_Players_On_Map == 1){
		SetPlayerColor(playerid,COLOUR_INVIS);
	}
	Debug("deathmatch.inc > DM_OnPlayerSpawn - Stop");
	//oDisablePlayerCheckpoint(playerid);
}


GetPlayerDMPosition(playerid,dmid)
{
	if (!IsPlayerInDM(playerid,dmid)) { Debug("deathmatch.inc > GetPlayerDMPosition - Stop"); return 0;}
	new position=1;
	for (new i=0;i<MAX_PLAYERS_EX;i++)
	{
		if (!IsPlayerInDM(i,dmid)) continue;
		new iscore = DMPlayerStats[i][dm_player_kills] - DMPlayerStats[i][dm_player_deaths];
		new playerscore = DMPlayerStats[playerid][dm_player_kills] - DMPlayerStats[playerid][dm_player_deaths];
		if (iscore > playerscore)
		{
			position++;
		}
		else if (iscore == playerscore)
		{
			if (DMPlayerStats[i][dm_player_kills] > DMPlayerStats[playerid][dm_player_kills])
			{
				position++;
			}
		}
	}
	Debug("deathmatch.inc > GetPlayerDMPosition - Stop");
	return position;
}

EndDM(dmid)
{
	Debug("deathmatch.inc > EndDM - Start");
	for (new playerid=0; playerid<MAX_PLAYERS_EX;playerid++)
	{
		if (!IsPlayerConnected(playerid)) continue;
		if (PlayerQuest[playerid] == GetDMQuestID(dmid)) // if player is in this dm
		{
			new playerpos = GetPlayerDMPosition(playerid,dmid);
			PlayerFinishDM(playerid,dmid,playerpos);
		}
	}
	CleanupDM(dmid);
	Debug("deathmatch.inc > EndDM - Stop");
}

CleanupDM(dmid)
{
	Debug("deathmatch.inc > CleanupDM - Start");
	for (new playerid=0; playerid<MAX_PLAYERS_EX;playerid++)
	{
		if (!IsPlayerConnected(playerid)) continue;
		if (PlayerQuest[playerid] == GetDMQuestID(dmid)) // if player is in this dm
		{
			PlayerLeaveDM(playerid,dmid);
		}
		if (IsPlayerConnected(playerid))
		{
			oDisablePlayerCheckpoint(playerid);
		}
	}
	DeathmatchStats[dmid][dm_playercount] = 0;
	DeathmatchStats[dmid][dm_timer] = MakeDMSleepTime(dmid);
	DeathmatchStats[dmid][dm_state] = DM_STATE_SLEEPING;
	event_now=false;
	for(new i=0; i<DMObjectsSize[dmid]; i++){
		DestroyStreamObject(DMObjectID[dmid][i]);
	}
	if (DeathmatchStats[dmid][dm_activecpid] != 0)
	{
		RemoveActiveCheckpoint(DeathmatchStats[dmid][dm_activecpid]);
		DeathmatchStats[dmid][dm_activecpid] = 0;
	}
	Debug("deathmatch.inc > CleanupDM - Stop");
}

PlayerFinishDM(playerid,dmid,position)
{
	Debug("deathmatch.inc > PlayerFinishDM - Start");
	new string[MAX_STRING];

	set(string,FormatPosition(position));
	ShowTextForPlayer(playerid,string,5000,1);
	PlayerPlaySoundOnPlayer(playerid,1057);

	if (position == 1)
	{
		format(string, sizeof(string),gettext(999),Deathmatch[dmid][dm_name],FormatPosition(position),GetPlayerDMScore(playerid,dmid));
		SystemMsg(playerid,COLOUR_DM,string);
		Player[playerid][DMsWon]++;
		format(string, sizeof(string),gettext(996),oGetPlayerName(playerid),Deathmatch[dmid][dm_name],GetPlayerDMScore(playerid,dmid));
		SystemMsgToRegistered(COLOUR_DM,string);
		new logstring[MAX_STRING];
		format(logstring, MAX_STRING, "player: %d:  %s: has won deathmatch '%s'! Score: %d",playerid,oGetPlayerName(playerid),Deathmatch[dmid][dm_name],GetPlayerDMScore(playerid,dmid));
		WriteLog(logstring);
		oGivePlayerMoney(playerid,Deathmatch[dmid][dm_cashprize],1);
		GivePlayerXP(playerid,Deathmatch[dmid][dm_xpprize] + (Deathmatch[dmid][dm_xpbonus]),1);
		SetPlayerCriminal(playerid,gettext(1388),13);
		BetWinner(playerid,2);

	}
	else if (position == 2)
	{
		format(string, sizeof(string),gettext(999),Deathmatch[dmid][dm_name],FormatPosition(position),GetPlayerDMScore(playerid,dmid));
		SystemMsg(playerid,COLOUR_DM,string);

		//format(string, sizeof(string),gettext(997),oGetPlayerName(playerid),FormatPosition(position),Deathmatch[dmid][dm_name],GetPlayerDMScore(playerid));
		//SystemMsgToRegistered(COLOUR_DM,string);
		if (DeathmatchStats[dmid][dm_playercount] > 2)
		{
			oGivePlayerMoney(playerid,(Deathmatch[dmid][dm_cashprize]/2),1);
			GivePlayerXP(playerid,(Deathmatch[dmid][dm_xpprize]/2) + (Deathmatch[dmid][dm_xpbonus]),1);

		}
		else
		{
			GivePlayerXP(playerid,Deathmatch[dmid][dm_xpbonus] * DeathmatchStats[dmid][dm_playercount],1);
		}
		SetPlayerCriminal(playerid,gettext(1388),10);
	}
	else if ((position == 3) && (DeathmatchStats[dmid][dm_playercount] > 3))
	{
		format(string, sizeof(string),gettext(999),Deathmatch[dmid][dm_name],FormatPosition(position),GetPlayerDMScore(playerid,dmid));
		SystemMsg(playerid,COLOUR_DM,string);

		//format(string, sizeof(string),gettext(998),oGetPlayerName(playerid),FormatPosition(Position[dmid]),Deathmatch[dmid][dm_name],GetPlayerDMScore(playerid));
		//SystemMsgToRegistered(COLOUR_DM,string);

		if (DeathmatchStats[dmid][dm_playercount] > 3)
		{
			oGivePlayerMoney(playerid,(Deathmatch[dmid][dm_cashprize]/3),1);
			GivePlayerXP(playerid,(Deathmatch[dmid][dm_xpprize]/3) + (Deathmatch[dmid][dm_xpbonus]),1);

		}
		else
		{
			GivePlayerXP(playerid,Deathmatch[dmid][dm_xpbonus] * DeathmatchStats[dmid][dm_playercount],1);
		}
		SetPlayerCriminal(playerid,gettext(1388),7);
	}
	else
	{
		format(string, sizeof(string),gettext(999),Deathmatch[dmid][dm_name],FormatPosition(position),GetPlayerDMScore(playerid,dmid));
		SystemMsg(playerid,COLOUR_DM,string);
		GivePlayerXP(playerid,Deathmatch[dmid][dm_xpbonus] * DeathmatchStats[dmid][dm_playercount],1);
		SetPlayerCriminal(playerid,gettext(1388),5);
	}

	if ((GetPlayerDMScore(playerid,dmid) > DMBestScore[dmid][dm_score_record]) || (DMBestScore[dmid][dm_score_record] == 0))
	{
		format(string, sizeof(string),gettext(1000),oGetPlayerName(playerid),Deathmatch[dmid][dm_name],GetPlayerDMScore(playerid,dmid),DMBestScore[dmid][dm_score_record]);
		SystemMsgToRegistered(COLOUR_DM,string);
		new logstring[MAX_STRING];
		format(logstring, MAX_STRING, "player: %d:  %s: has set a new record for deathmatch '%s'! Score: %d. Old record: %d.",playerid,oGetPlayerName(playerid),Deathmatch[dmid][dm_name],GetPlayerDMScore(playerid,dmid),DMBestScore[dmid][dm_score_record]);
		WriteLog(logstring);
		oGivePlayerMoney(playerid,(Deathmatch[dmid][dm_cashprize]*3),1);
		GivePlayerXP(playerid,(Deathmatch[dmid][dm_xpprize]),1);
		DMBestScore[dmid][dm_score_record] = GetPlayerDMScore(playerid,dmid);
		DMBestScore[dmid][dm_score_kills] = DMPlayerStats[playerid][dm_player_kills];
		DMBestScore[dmid][dm_score_deaths] = DMPlayerStats[playerid][dm_player_deaths];
		set(DMBestScore[dmid][dm_score_player],oGetPlayerName(playerid));

	}

	PlayerLeaveDM(playerid,dmid);

	DeathmatchSaveRecordDB(dmid);

	Debug("deathmatch.inc > PlayerFinishDM - Stop");
}

PlayerLeaveDM(playerid,dmid)
{
	Debug("deathmatch.inc > PlayerLeaveDM - Start");
	new string[MAX_STRING];
	ResetQuest(playerid);  // reset checkpoints
	DeathmatchStats[dmid][dm_playercount]--;

	if (!IsPlayerConnected(playerid))
	{
		DMResetPlayerStats(playerid);
		Debug("deathmatch.inc > PlayerLeaveDM - Stop");
		return;
	}
	if (DeathmatchStats[dmid][dm_state] == DM_STATE_LINEUP)
	{
		if (Deathmatch[dmid][dm_minplayers] > 1)
		{
			format(string, sizeof(string),gettext(1001), oGetPlayerName(playerid),DeathmatchStats[dmid][dm_playercount],Deathmatch[dmid][dm_minplayers]);
			new logstring[MAX_STRING];
			format(logstring, MAX_STRING, "player: %d:  %s: has left the dm. (Players:%d/%d)",playerid,oGetPlayerName(playerid),DeathmatchStats[dmid][dm_playercount],Deathmatch[dmid][dm_minplayers]);
			WriteLog(logstring);
		}
		else
		{
			format(string, sizeof(string),gettext(1002), oGetPlayerName(playerid),DeathmatchStats[dmid][dm_playercount]);
			new logstring[MAX_STRING];
			format(logstring, MAX_STRING, "player: %d:  %s: has left the dm. (Players:%d)",playerid,oGetPlayerName(playerid),DeathmatchStats[dmid][dm_playercount]);
			WriteLog(logstring);
		}
		SendMessageToDMPlayers(dmid,string);
		oGivePlayerMoney(playerid,Deathmatch[dmid][dm_cashentry],1); // give player back thier cash entry
		ShowTextForPlayer(playerid, gettext(1003),5000,1);
		SystemMsg(playerid,COLOUR_DM,gettext(1475));
	}
	else
	{
		format(string, sizeof(string),gettext(1004), oGetPlayerName(playerid),DeathmatchStats[dmid][dm_playercount]);
		SendMessageToDMPlayers(dmid,string);
		new logstring[MAX_STRING];
		format(logstring, MAX_STRING, "player: %d:  %s: has left the dm. (Players:%d)",playerid,oGetPlayerName(playerid),DeathmatchStats[dmid][dm_playercount]);
		WriteLog(logstring);
		SystemMsg(playerid,COLOUR_DM,gettext(1005));
	}
	StopShowingDMInfo(playerid);

	if (DMPlayerStats[playerid][dm_player_active])
	{
		ResetPlayerDMWeapons(playerid);
		oSetPlayerPos(playerid,DMStartCP[dmid][Coord_X], DMStartCP[dmid][Coord_Y], DMStartCP[dmid][Coord_Z]);
		SetPlayerInterior(playerid,INTERIOR_DEFAULT);
		oSetPlayerHealth(playerid,GetMaxHealth(playerid)); // set health based on level
		oDisablePlayerCheckpoint(playerid);
		GivePlayerOwnedWeapons(playerid);
	}
	if (DeathmatchStats[dmid][dm_playercount] <= 0){
		CleanupDM(dmid);
	}
	DMResetPlayerStats(playerid);
	SetPlayerServerWorldBounds(playerid);
	TogglePlayerControllable(playerid,1);
	SetPlayerVirtualWorld(playerid,WORLD_DEFAULT);
	SetSpecialColorForPlayer(playerid);
	Debug("deathmatch.inc > PlayerLeaveDM - Stop");
}
/*
SendPlayerDMWeapons(playerid,dmid)
{
ResetPlayerWeapons(playerid);
new string[MAX_STRING];
new weapons[MAX_DMWEAPONS+1];
set(weapons,Deathmatch[dmid][dm_weapons]);
if (strlen(weapons) <= 7)
{
set(string,"Deathmatch weapons: ");
for (new i=0;i<strlen(weapons);i++)
{
if (weapons[i] == 0)  break;
if (i>0) strins(string, ", ", strlen(string));
strins(string, GetWeaponName(weapons[i]), strlen(string));
}
SystemMsg(playerid,string);
}
else
{
SystemMsgScrolling(playerid, "(deathmatch) Deathmatch weapons:");
for (new i=0;i<strlen(weapons);i++)
{
		if (weapons[i] == 0)  break;
		if (i>0) strins(string, ", ", strlen(string));

strins(string, GetWeaponName(weapons[i]), strlen(string));
SystemMsgScrolling(playerid,string);
}
}
}
*/
GivePlayerDMWeapons(playerid,dmid)
{
	Debug("deathmatch.inc > GivePlayerDMWeapons - Start");
	new weapons[MAX_DMWEAPONS+1];
	set(weapons,Deathmatch[dmid][dm_weapons]);
	ResetPlayerWeapons(playerid);
	for (new i=0;i<strlen(weapons);i++)
	{
		if (weapons[i] == 0)  break;
		new slot = GetWeaponSlot(weapons[i]);
		new ammo = GetWeaponMaxAmmo(weapons[i]);
		GivePlayerWeapon(playerid,weapons[i],ammo);
		PlayerTempWeapons[playerid][slot][bullets] = 1;
	}
	Debug("deathmatch.inc > GivePlayerDMWeapons - Stop");
}

ResetPlayerDMWeapons(playerid)
{
	Debug("deathmatch.inc > ResetPlayerDMWeapons - Start");
	ResetPlayerWeapons(playerid);
	for (new i=0;i<PLAYER_WEAPON_SLOTS;i++)
	{
		PlayerTempWeapons[playerid][i][wid] = 0;
		PlayerTempWeapons[playerid][i][bullets] = 0;	
	}
	Debug("deathmatch.inc > ResetPlayerDMWeapons - Stop");
}

SendMessageToDMPlayers(dmid,string[])
{
	for (new dmplayerid=0; dmplayerid<MAX_PLAYERS_EX;dmplayerid++)
	{
		if (!IsPlayerConnected(dmplayerid)) continue;
		if (PlayerQuest[dmplayerid] == GetDMQuestID(dmid)) // if player is in this dm
		{
			SystemMsg(dmplayerid,COLOUR_DM,string);
		}
	}
}

IsPlayerInDM(playerid,dmid)
{
	if (dmid == INVALID_DM_ID) return 0;
	if (!IsPlayerConnected(playerid)) return 0;
	if (PlayerQuest[playerid] != GetDMQuestID(dmid)) return 0; 
	return 1;
}

IsPlayerInAnyDM(playerid)
{
	new dmid = GetPlayerDM(playerid);
	if (dmid == INVALID_DM_ID) return 0;
	if (DMPlayerStats[playerid][dm_player_active] == false) return 0; // if we are not spawned in dm zone then we are not actualy active in dm
	return 1;
}

DMResetPlayerStats(playerid)
{
	Debug("deathmatch.inc > DMResetPlayerStats - Start");
	DMPlayerStats[playerid][dm_player_active] = false;
	DMPlayerStats[playerid][dm_player_kills] = 0;
	DMPlayerStats[playerid][dm_player_deaths] = 0;
	Debug("deathmatch.inc > DMResetPlayerStats - Stop");
}

GetPlayerDMScore(playerid,dmid)
{
	Debug("deathmatch.inc > GetPlayerDMScore - Start");
	if (!IsPlayerInDM(playerid,dmid)) { Debug("deathmatch.inc > GetPlayerDMScore - Stop"); return 0; }
	new score;
	score = DMPlayerStats[playerid][dm_player_kills] - DMPlayerStats[playerid][dm_player_deaths];
	if (score < 0) score = 0;
	Debug("deathmatch.inc > GetPlayerDMScore - Stop");
	return score;
}

GetPlayerDM(playerid)
{
	Debug("deathmatch.inc > GetPlayerDM - Start");
	if (!IsPlayerConnected(playerid)) {Debug("deathmatch.inc > GetPlayerDM - Stop"); return 0;}
	for (new dmid=1; dmid<=DMCount;dmid++)
	{
		if (DeathmatchStats[dmid][dm_state] == DM_STATE_DISABLED) continue;
		if (PlayerQuest[playerid] == GetDMQuestID(dmid)) // if player is in this dm
		{
			Debug("deathmatch.inc > GetPlayerDM - Stop");
			return dmid;
		}
	}
	Debug("deathmatch.inc > GetPlayerDM - Stop");
	return 0;
}

GetDMQuestID(dmid)
{
	return DeathmatchStats[dmid][dm_questid];
}


//----------------------------------------

GetDMStartZoneName(dmid)
{
	new zone[MAX_STRING];
	zone = GetXYZZoneName(GetDMCPX(dmid),GetDMCPY(dmid),GetDMCPZ(dmid));
	return zone;
}

GetDMWinner(dmid)
{
	Debug("deathmatch.inc > GetDMWinner - Start");
	new winnerid=INVALID_PLAYER_ID;
	new winnerscore;
	for (new playerid=0; playerid<MAX_PLAYERS_EX;playerid++)
	{
		if (!IsPlayerConnected(playerid)) continue;
		if (PlayerQuest[playerid] == GetDMQuestID(dmid)) // if player is in this dm
		{
			if (DMPlayerStats[playerid][dm_player_kills] - DMPlayerStats[playerid][dm_player_deaths] > winnerscore)
			{
				winnerid = playerid;
				winnerscore = DMPlayerStats[playerid][dm_player_kills] - DMPlayerStats[playerid][dm_player_deaths];
			}
		}
	}
	Debug("deathmatch.inc > GetDMWinner - Stop");
	return winnerid;
}


UpdateDMScore(dmid)
{
	Debug("deathmatch.inc > UpdateDMScore - Start");
	new winnerid = GetDMWinner(dmid);
	if (winnerid != INVALID_PLAYER_ID)
	{
		new string[MAX_STRING];
		format(string, sizeof(string),gettext(1011),oGetPlayerName(winnerid),DMPlayerStats[winnerid][dm_player_kills],DMPlayerStats[winnerid][dm_player_deaths]);
		SendMessageToDMPlayers(dmid,string);
	}
	Debug("deathmatch.inc > UpdateDMScore - Stop");
}

MakeDMSleepTime(dmid)
{
	Debug("deathmatch.inc > MakeDMSleepTime - Start");
	new sleeptime;
	if (Deathmatch[dmid][dm_frequency] == 0) Deathmatch[dmid][dm_frequency] = 5;
	sleeptime = ((Deathmatch[dmid][dm_frequency] * DMCount) * 150);
	Debug("deathmatch.inc > MakeDMSleepTime - Stop");
	return sleeptime;
}

IsWeaponAllowedInDM(weaponid,dmid)
{
	Debug("deathmatch.inc > IsWeaponAllowedInDM - Start");
	if (weaponid == 0) return 1; //unarmed always allowed
	if (weaponid == WEAPON_BRASSKNUCKLE) return 1; //brassknuckle too
	new weapons[MAX_DMWEAPONS+1];
	set(weapons,Deathmatch[dmid][dm_weapons]);
	for (new i=0;i<strlen(weapons);i++)
	{
		if (weapons[i] == weaponid) return 1;
	}
	Debug("deathmatch.inc > IsWeaponAllowedInDM - Stop");
	return 0;
}

public OnPlayerDMKill(playerid,victimid,weaponid)
{
	Debug("deathmatch.inc > OnPlayerDMKill - Start");

	new dmid = GetPlayerDM(playerid);
//	if ((dmid == INVALID_DM_ID) || (!IsPlayerInDM(playerid,dmid)))
//	{
//		DMResetPlayerStats(playerid);
//		Debug("deathmatch.inc > OnPlayerDMKill - Stop");
//		return; // player not in a dm
//	}
	DMPlayerStats[playerid][dm_player_kills]++;
	GivePlayerXP(playerid,Deathmatch[dmid][dm_xpkill],0);

	if (GetDMWinner(dmid) == playerid)
	{
		UpdateDMScore(dmid);
	}
	PlayerPlaySoundOnPlayer(playerid,1056);
	UpdateDMInfo(playerid);
	Debug("deathmatch.inc > OnPlayerDMKill - Stop");
}

public DM_OnPlayerDeath(playerid,killerid)
{
	Debug("deathmatch.inc > DM_OnPlayerDeath - Start");

	new dmid = GetPlayerDM(playerid);
//	if ((dmid == INVALID_DM_ID) || (!IsPlayerInDM(playerid,dmid)))
//	{
//		DMResetPlayerStats(playerid);
//		Debug("deathmatch.inc > DM_OnPlayerDeath - Stop");
//		return; // player not in a dm
//	}
	DMPlayerStats[playerid][dm_player_deaths]++;

	GivePlayerXP(playerid,0-Deathmatch[dmid][dm_xpdeath],1);
	DM_OnPlayerSpawn(playerid,dmid);
	PlayerPlaySoundOnPlayer(playerid,1057);
	UpdateDMInfo(playerid);	
	Debug("deathmatch.inc > DM_OnPlayerDeath - Stop");
	return 0;
}


public DM_OnPlayerEnterCheckpoint(playerid)
{
	new playerdmid = GetPlayerDM(playerid);
	new string[MAX_STRING];
	for (new dmid=1; dmid<=DMCount;dmid++) //  for each dm
	{
		//if (DeathmatchStats[dmid][dm_state] == DM_STATE_DISABLED) break;

		if (DeathmatchStats[dmid][dm_state] == DM_STATE_LINEUP)
		{
			if (PlayerQuest[playerid] == 0 && Player[playerid][GotJob] != JOB_COP) // if player not on a quest
			{
				// if player at startline
				if (oIsPlayerInCheckpoint(playerid,GetDMCPX(dmid),GetDMCPY(dmid),GetDMCPZ(dmid),DMCPSize*3))
				{
					JoinDM(playerid,dmid);
				}
			}
			else if (playerdmid == dmid)
			{
				if (oIsPlayerInCheckpoint(playerid,GetDMCPX(dmid),GetDMCPY(dmid),GetDMCPZ(dmid),DMCPSize*3))
				{
					format(string,sizeof(string),gettext(1014),(Deathmatch[dmid][dm_lineupdelay] - DeathmatchStats[dmid][dm_timer]));
					SystemMsg(playerid,COLOUR_DM,string);
				}
			}
		}
		else if (DeathmatchStats[dmid][dm_state] == DM_STATE_ACTIVE)
		{
			if (PlayerQuest[playerid] == 0) // if player not on a quest
			{
				// if player at startline
				if (oIsPlayerInCheckpoint(playerid,GetDMCPX(dmid),GetDMCPY(dmid),GetDMCPZ(dmid),DMCPSize*3))
				{
					//JoinDM(playerid,dmid);
					if (Deathmatch[dmid][dm_activedmjoin] == 1){
						format(string,sizeof(string),gettext(1015),Deathmatch[dmid][dm_name],dmid);
						SystemMsg(playerid,COLOUR_DM,string);
					} else {
						SystemMsg(playerid,COLOUR_DM_BAD,gettext(1016));
					}
				}
			}
			else if (playerdmid == dmid)
			{
				if (oIsPlayerInCheckpoint(playerid,GetDMCPX(dmid),GetDMCPY(dmid),GetDMCPZ(dmid),DMCPSize*3))
				{
					DM_OnPlayerSpawn(playerid,dmid);
					format(string, sizeof(string), gettext(975));
					PlayerPlaySoundOnPlayer(playerid,1057);
					ShowTextForPlayer(playerid, string,1000,6);
				}
			}
		}
	}
}

public DM_OnPlayerCommandText(playerid,text[])
{
	if (!IsPlayerRegistered(playerid)) return 0;

	new cmd[20];
	new idx;
	//new string[MAX_STRING];

	set(cmd,strcharsplit(text, idx,strchar(" ")));
	if (strlen(cmd) == 0) return 0;

	if(strcomp(cmd, "/dm", true) == 1)
	{
		set(cmd,strcharsplit(text, idx,strchar(" ")));

		if(strcomp(cmd, "join", true) == 1)
		{
			Debug("deathmatch.inc > Command 'dm join' - Start");
			if (PlayerQuest[playerid] != 0)
			{
				SystemMsg(playerid,COLOUR_DM_BAD,gettext(1018));
				Debug("deathmatch.inc > Command 'dm join' - Stop");
				return 1;
			}
			new dmid = GetPlayerDMByCheckpoint(playerid);
			if (dmid == 0)
			{
				SystemMsg(playerid,COLOUR_DM_BAD,gettext(1019));
				Debug("deathmatch.inc > Command 'dm join' - Stop");
				return 1;
			}
			if (DeathmatchStats[dmid][dm_state] == DM_STATE_DISABLED)
			{
				SystemMsg(playerid,COLOUR_DM_BAD,gettext(1019));
				Debug("deathmatch.inc > Command 'dm join' - Stop");
				return 1;
			}
			if ((DeathmatchStats[dmid][dm_state] != DM_STATE_LINEUP) && (DeathmatchStats[dmid][dm_state] != DM_STATE_ACTIVE))
			{
				SystemMsg(playerid,COLOUR_DM_BAD,gettext(1020));
				Debug("deathmatch.inc > Command 'dm join' - Stop");
				return 1;
			}

			if ((Deathmatch[dmid][dm_activedmjoin] == 0) && (DeathmatchStats[dmid][dm_state] == DM_STATE_ACTIVE))
			{
				SystemMsg(playerid,COLOUR_DM_BAD,gettext(1021));
				Debug("deathmatch.inc > Command 'dm join' - Stop");
				return 1;
			}
			if (Player[playerid][GotJob] != JOB_COP){
			JoinDM(playerid,dmid);
			} else {
			SystemMsg(playerid,COLOUR_DM_BAD,gettext(1022));
			}
			Debug("deathmatch.inc > Command 'dm join' - Stop");
			return 1;
		}

		return 0;
	}
	return 0;
}

GetPlayerDMByCheckpoint(playerid){
	new lookupdmid;
	for (new dmid=1; dmid<=DMCount;dmid++)
	{
		if (oIsPlayerInCheckpoint(playerid,GetDMCPX(dmid),GetDMCPY(dmid),GetDMCPZ(dmid),50)) lookupdmid = dmid;
	}
	return lookupdmid;
}


DeathmatchLoadAll()
{
	Debug("deathmatch.inc > DeathmatchLoadAll - Start");
	new temp[MAX_STRING];
	if (!db_Exists(DatabaseDB)) db_Create(DatabaseDB);
	set(temp,db_Get(DatabaseDB,"Deathmatch_DB"));
	if (strlen(temp) > 0) set(DeathmatchDB,temp);
	set(temp,db_Get(DatabaseDB,"Deathmatch_Base_DB"));
	if (strlen(temp) > 0) set(DMBaseDB,temp);
	set(temp,db_Get(DatabaseDB,"Deathmatch_Record_DB"));
	if (strlen(temp) > 0) set(DMRecordDB,temp);

	if (!db_Exists(DeathmatchDB))
	{
		db_Create(DeathmatchDB);
	}

	for (new dmdbid=0;dmdbid<MAX_DM;dmdbid++)
	{ // load all our dms from db
		new cellname[MAX_STRING];
		format(cellname,sizeof(cellname),"Deathmatch%d",dmdbid);
		set(temp,db_Get(DeathmatchDB,cellname));
		if (strlen(temp) == 0) continue;
		if (!DeathmatchBaseDBExists(temp)) continue;
		new dmid = RegisterDM(temp);
		if (dmid == INVALID_DM_ID) continue;
		DeathmatchLoadBaseDB(dmid);
		DeathmatchLoadRecordDB(dmid);
	}
	Debug("deathmatch.inc > DeathmatchLoadAll - Stop");
	return;
}

DeathmatchBaseDBExists(dmname[MAX_STRING])
{ 
	new rdbname[MAX_STRING];
	format(rdbname,sizeof(rdbname),"%s%s.txt",DMBaseDB,dmname);
	if (!db_Exists(rdbname))
	{
		return 0;
	}
	return 1;
}

DeathmatchLoadBaseDB(dmid)
{ // load dm from db
	Debug("deathmatch.inc > DeathmatchLoadBaseDB - Start");
	new temp[MAX_STRING];
	new rdbname[MAX_STRING];
	format(rdbname,sizeof(rdbname),"%s%s.txt",DMBaseDB,Deathmatch[dmid][dm_name]);
	if (!db_Exists(rdbname))
	{
		Debug("deathmatch.inc > DeathmatchLoadBaseDB - Stop");
		return INVALID_DM_ID;
	}
	set(temp,db_Get(rdbname,"Name"));
	if (strlen(temp) > 0) set(Deathmatch[dmid][dm_name],temp);
	set(temp,nullstr);

	if (dmid <= 0)
	{
		new logstring[MAX_STRING];
		format(logstring, MAX_STRING, "Deathmatch (DB): %s Failed to load", Deathmatch[dmid][dm_name]);
		WriteLog(logstring);
		Debug("deathmatch.inc > DeathmatchLoadBaseDB - Stop");
		return INVALID_DM_ID;
	}

	set(temp,db_Get(rdbname,"Frequency"));
	if (strlen(temp) > 0) Deathmatch[dmid][dm_frequency] = strval(temp);
	set(temp,nullstr);

	set(temp,db_Get(rdbname,"Lineup_Delay"));
	if (strlen(temp) > 0) Deathmatch[dmid][dm_lineupdelay] = strval(temp);
	set(temp,nullstr);

	set(temp,db_Get(rdbname,"Min_Players"));
	if (strlen(temp) > 0) Deathmatch[dmid][dm_minplayers] = strval(temp);
	set(temp,nullstr);

	//if (Deathmatch[dmid][dm_minplayers] < 2) Deathmatch[dmid][dm_minplayers] = 2;

	set(temp,db_Get(rdbname,"Min_Level"));
	if (strlen(temp) > 0) Deathmatch[dmid][dm_minlevel] = strval(temp);
	set(temp,nullstr);

	set(temp,db_Get(rdbname,"Cash_Prize"));
	if (strlen(temp) > 0) Deathmatch[dmid][dm_cashprize] = strval(temp);
	set(temp,nullstr);

	set(temp,db_Get(rdbname,"Cash_Entry"));
	if (strlen(temp) > 0) Deathmatch[dmid][dm_cashentry] = strval(temp);
	set(temp,nullstr);

	set(temp,db_Get(rdbname,"XP_Prize"));
	if (strlen(temp) > 0) Deathmatch[dmid][dm_xpprize] = strval(temp);
	set(temp,nullstr);

	set(temp,db_Get(rdbname,"XP_Bonus"));
	if (strlen(temp) > 0) Deathmatch[dmid][dm_xpbonus] = strval(temp);
	set(temp,nullstr);

	set(temp,db_Get(rdbname,"Max_Time"));
	if (strlen(temp) > 0) Deathmatch[dmid][dm_maxtime] = strval(temp);
	set(temp,nullstr);

	set(temp,db_Get(rdbname,"XP_Kill"));
	if (strlen(temp) > 0) Deathmatch[dmid][dm_xpkill] = strval(temp);
	set(temp,nullstr);

	set(temp,db_Get(rdbname,"XP_Death"));
	if (strlen(temp) > 0) Deathmatch[dmid][dm_xpdeath] = strval(temp);
	set(temp,nullstr);

	set(temp,db_Get(rdbname,"Join_Active"));
	if (strlen(temp) > 0) Deathmatch[dmid][dm_activedmjoin] = strval(temp);
	set(temp,nullstr);

	set(temp,db_Get(rdbname,"Interior"));
	if (strlen(temp) > 0) Deathmatch[dmid][dm_interior] = strval(temp);
	set(temp,nullstr);

	SetInteriorWeapons(Deathmatch[dmid][dm_interior],true);

	set(temp,db_Get(rdbname,"Sudden_Explosion_Time"));
	if (strlen(temp) > 0) Deathmatch[dmid][dm_suddenexplosiontime] = strval(temp);
	set(temp,nullstr);

	set(temp,db_Get(rdbname,"Weapons"));
	if (strlen(temp) > 0) set(Deathmatch[dmid][dm_weapons],DMDecodeWeaponsDB(temp));
	set(temp,nullstr);

	new Float:X;
	new Float:Y;
	new Float:Z;

	set(temp,db_Get(rdbname,"Start_CP"));
	if (strlen(temp) != 0)
	{
		new idx=0;
		X= floatstr(strcharsplit(temp,idx,strchar(",")));
		Y = floatstr(strcharsplit(temp,idx,strchar(",")));
		Z = floatstr(strcharsplit(temp,idx,strchar(",")));
		set(temp,nullstr);
		if (( X!= 0.0) || (Y != 0.0))
		{
			DMStartCP[dmid][Coord_X] = X;
			DMStartCP[dmid][Coord_Y] = Y;
			DMStartCP[dmid][Coord_Z] = Z;
			X= 0.0;
			Y = 0.0;
			Z = 0.0;
		}
	}

	set(temp,db_Get(rdbname,"World_Bounds"));
	if (strlen(temp) != 0)
	{
		new idx=0;
		new Float:myx_max,Float:myx_min,Float:myy_max,Float:myy_min;
		myx_max = floatstr(strcharsplit(temp,idx,strchar(",")));
		myx_min = floatstr(strcharsplit(temp,idx,strchar(",")));
		myy_max = floatstr(strcharsplit(temp,idx,strchar(",")));
		myy_min = floatstr(strcharsplit(temp,idx,strchar(",")));
		set(temp,nullstr);
		if ((myx_max != 0.0) || (myx_min != 0.0) || (myy_max != 0.0) || (myy_min != 0.0))
		{
			DMWorldBounds[dmid][x_max] = myx_max;
			DMWorldBounds[dmid][x_min] = myx_min;
			DMWorldBounds[dmid][y_max] = myy_max;
			DMWorldBounds[dmid][y_min] = myy_min;
			myx_max = 0.0;
			myx_min = 0.0;
			myy_max = 0.0;
			myy_min = 0.0;
		}
	}

	new spsize;
	for (new spid=0;spid<MAX_DM_SP;spid++)
	{
		new cellname[MAX_STRING];
		format(cellname,sizeof(cellname),"SP%d",spid);
		set(temp,db_Get(rdbname,cellname));
		if (strlen(temp) == 0) break;
		new idx=0;
		X= floatstr(strcharsplit(temp,idx,strchar(",")));
		Y = floatstr(strcharsplit(temp,idx,strchar(",")));
		Z = floatstr(strcharsplit(temp,idx,strchar(",")));
		set(temp,nullstr);
		if (( X== 0.0) && (Y == 0.0)) break;
		DMSpawnPoints[dmid][spid][Coord_X] = X;
		DMSpawnPoints[dmid][spid][Coord_Y] = Y;
		DMSpawnPoints[dmid][spid][Coord_Z] = Z;
		spsize++;
	}

	new objectsize;
	for (new objid=0;objid<MAX_DM_OBJECTS;objid++)
	{
		new cellname[MAX_STRING];
		format(cellname,sizeof(cellname),"Object%d",objid);
		set(temp,db_Get(rdbname,cellname));
		if (strlen(temp) == 0) break;
		new idx=0;
		DMObjectInfo[dmid][objid][dmomodel] = strval(strcharsplit(temp,idx,strchar(",")));
		DMObjectInfo[dmid][objid][dmox] = floatstr(strcharsplit(temp,idx,strchar(",")));
		DMObjectInfo[dmid][objid][dmoy] = floatstr(strcharsplit(temp,idx,strchar(",")));
		DMObjectInfo[dmid][objid][dmoz] = floatstr(strcharsplit(temp,idx,strchar(",")));
		DMObjectInfo[dmid][objid][dmorx] = floatstr(strcharsplit(temp,idx,strchar(",")));
		DMObjectInfo[dmid][objid][dmory] = floatstr(strcharsplit(temp,idx,strchar(",")));
		DMObjectInfo[dmid][objid][dmorz] = floatstr(strcharsplit(temp,idx,strchar(",")));
		set(temp,nullstr);
		objectsize++;
	}

	DMObjectsSize[dmid] = objectsize;

	DMSpawnsSize[dmid] = spsize;
	DeathmatchStats[dmid][dm_state] = DM_STATE_SLEEPING;
	DeathmatchStats[dmid][dm_timer] = MakeDMSleepTime(dmid);
	new logstring[MAX_STRING];
	format(logstring, MAX_STRING, "Deathmatch (DB): %s - loaded", Deathmatch[dmid][dm_name]);
	WriteLog(logstring);
	Debug("deathmatch.inc > DeathmatchLoadBaseDB - Stop");
	return dmid;
}

DeathmatchLoadRecordDB(dmid)
{
	Debug("deathmatch.inc > DeathmatchLoadRecordDB - Start");
	new temp[MAX_STRING];
	new rdbname[MAX_STRING];
	format(rdbname,sizeof(rdbname),"%s%s.txt",DMRecordDB,Deathmatch[dmid][dm_name]);
	if (!db_Exists(rdbname)) { Debug("deathmatch.inc > DeathmatchLoadRecordDB - Stop"); return;}
	set(temp,db_Get(rdbname,"Best_Score_Record"));
	if (strlen(temp) > 0) DMBestScore[dmid][dm_score_record] = strval(temp);
	set(temp,nullstr);
	set(temp,db_Get(rdbname,"Best_Score_Player"));
	if (strlen(temp) > 0) set(DMBestScore[dmid][dm_score_player],temp);
	set(temp,nullstr);
	set(temp,db_Get(rdbname,"Best_Score_Kills"));
	if (strlen(temp) > 0) DMBestScore[dmid][dm_score_kills] = strval(temp);
	set(temp,nullstr);
	set(temp,db_Get(rdbname,"Best_Score_Deaths"));
	if (strlen(temp) > 0) DMBestScore[dmid][dm_score_deaths] = strval(temp);
	set(temp,nullstr);
	Debug("deathmatch.inc > DeathmatchLoadRecordDB - Stop");
}

DeathmatchSaveAll()
{
	Debug("deathmatch.inc > DeathmatchSaveAll - Start");
	if (!db_Exists(DatabaseDB)) db_Create(DatabaseDB);
	db_Set(DatabaseDB,"Deathmatch_Base_DB",DMBaseDB);
	db_Set(DatabaseDB,"Deathmatch_Record_DB",DMRecordDB);

	if (!db_Exists(DeathmatchDB)) db_Create(DeathmatchDB);
	for (new dmid=1;dmid<=DMCount;dmid++)
	{ // load all our dms from db
		if (DeathmatchStats[dmid][dm_state] == DM_STATE_DISABLED) continue;
		new cellname[MAX_STRING];
		format(cellname,sizeof(cellname),"Deathmatch%d",dmid);
		db_Set(DeathmatchDB,cellname,Deathmatch[dmid][dm_name]);

		DeathmatchSaveBaseDB(dmid);
		DeathmatchSaveRecordDB(dmid);
	}
	Debug("deathmatch.inc > DeathmatchSaveAll - Stop");
}

DeathmatchSaveScores()
{
	Debug("deathmatch.inc > DeathmatchSaveScores - Start");
	if (!db_Exists(DatabaseDB)) db_Create(DatabaseDB);
	db_Set(DatabaseDB,"Deathmatch_Base_DB",DMBaseDB);

	if (!db_Exists(DeathmatchDB)) db_Create(DeathmatchDB);
	for (new dmid=1;dmid<=DMCount;dmid++)
	{ // load all our dms from db
		if (DeathmatchStats[dmid][dm_state] == DM_STATE_DISABLED) continue;
		new cellname[MAX_STRING];
		format(cellname,sizeof(cellname),"Deathmatch%d",dmid);
		db_Set(DeathmatchDB,cellname,Deathmatch[dmid][dm_name]);
		DeathmatchSaveRecordDB(dmid);
	}
	Debug("deathmatch.inc > DeathmatchSaveScores - Stop");
}

DeathmatchSaveBaseDB(dmid)
{ // save dm to db
	Debug("deathmatch.inc > DeathmatchSaveBaseDB - Start");
	if (DeathmatchStats[dmid][dm_state] == DM_STATE_DISABLED) {Debug("deathmatch.inc > DeathmatchSaveBaseDB - Stop"); return;}
	new temp[MAX_STRING];
	new rdbname[MAX_STRING];
	format(rdbname,sizeof(rdbname),"%s%s.txt",DMBaseDB,Deathmatch[dmid][dm_name]);
	if (db_Exists(rdbname))
	{
		Debug("deathmatch.inc > DeathmatchSaveBaseDB - Stop");
		return;
	}

	db_Create(rdbname);
	db_BeforeBigSaving(rdbname);
	db_Set(rdbname,"Name",Deathmatch[dmid][dm_name]);

	valstr(temp,Deathmatch[dmid][dm_frequency]);
	db_Set(rdbname,"Frequency",temp);
	set(temp,nullstr);

	valstr(temp,Deathmatch[dmid][dm_lineupdelay]);
	db_Set(rdbname,"Lineup_Delay",temp);
	set(temp,nullstr);

	valstr(temp,Deathmatch[dmid][dm_minplayers]);
	db_Set(rdbname,"Min_Players",temp);
	set(temp,nullstr);

	valstr(temp,Deathmatch[dmid][dm_minlevel]);
	db_Set(rdbname,"Min_Level",temp);
	set(temp,nullstr);

	valstr(temp,Deathmatch[dmid][dm_cashprize]);
	db_Set(rdbname,"Cash_Prize",temp);
	set(temp,nullstr);

	valstr(temp,Deathmatch[dmid][dm_cashentry]);
	db_Set(rdbname,"Cash_Entry",temp);
	set(temp,nullstr);

	valstr(temp,Deathmatch[dmid][dm_xpprize]);
	db_Set(rdbname,"XP_Prize",temp);
	set(temp,nullstr);

	valstr(temp,Deathmatch[dmid][dm_xpbonus]);
	db_Set(rdbname,"XP_Bonus",temp);
	set(temp,nullstr);

	valstr(temp,Deathmatch[dmid][dm_maxtime]);
	db_Set(rdbname,"Max_Time",temp);
	set(temp,nullstr);

	valstr(temp,Deathmatch[dmid][dm_xpkill]);
	db_Set(rdbname,"XP_Kill",temp);
	set(temp,nullstr);

	valstr(temp,Deathmatch[dmid][dm_xpdeath]);
	db_Set(rdbname,"XP_Death",temp);
	set(temp,nullstr);

	valstr(temp,Deathmatch[dmid][dm_activedmjoin]);
	db_Set(rdbname,"Join_Active",temp);
	set(temp,nullstr);

	valstr(temp,Deathmatch[dmid][dm_interior]);
	db_Set(rdbname,"Interior",temp);
	set(temp,nullstr);

	valstr(temp,Deathmatch[dmid][dm_suddenexplosiontime]);
	db_Set(rdbname,"Sudden_Explosion_Time",temp);
	set(temp,nullstr);

	set(temp,DMEncodeWeaponsDB(dmid));
	db_Set(rdbname,"Weapons",temp);
	set(temp,nullstr);

	format(temp,sizeof(temp),"%f,%f,%f,",DMStartCP[dmid][Coord_X],DMStartCP[dmid][Coord_Y],DMStartCP[dmid][Coord_Z]);
	db_Set(rdbname,"Start_CP",temp);
	set(temp,nullstr);

	format(temp,sizeof(temp), "%f,%f,%f,%f,",DMWorldBounds[dmid][x_max],DMWorldBounds[dmid][x_min],DMWorldBounds[dmid][y_max],DMWorldBounds[dmid][y_min]);
	db_Set(rdbname,"World_Bounds",temp);
	set(temp,nullstr);

	for (new spid=0;spid<DMSpawnsSize[dmid];spid++)
	{
		new cellname[MAX_STRING];
		format(cellname,sizeof(cellname),"SP%d",spid);
		format(temp,sizeof(temp),"%f,%f,%f,",DMSpawnPoints[dmid][spid][Coord_X],DMSpawnPoints[dmid][spid][Coord_Y],DMSpawnPoints[dmid][spid][Coord_Z]);
		db_Set(rdbname,cellname,temp);
	}

	for (new objid=0;objid<DMObjectsSize[dmid];objid++)
	{
		new cellname[MAX_STRING];
		format(cellname,sizeof(cellname),"Object%d",objid);
		format(temp,sizeof(temp),"%d,%f,%f,%f,%f,%f,%f,",DMObjectInfo[dmid][objid][dmomodel],DMObjectInfo[dmid][objid][dmox],DMObjectInfo[dmid][objid][dmoy],DMObjectInfo[dmid][objid][dmoz],DMObjectInfo[dmid][objid][dmorx],DMObjectInfo[dmid][objid][dmory],DMObjectInfo[dmid][objid][dmorz]);
		db_Set(rdbname,cellname,temp);
	}

	db_AfterBigSaving(rdbname);

	new logstring[MAX_STRING];
	format(logstring, MAX_STRING, "Deathmatch base saved: %s",Deathmatch[dmid][dm_name]);
	WriteLog(logstring);

	Debug("deathmatch.inc > DeathmatchSaveBaseDB - Stop");
}

DeathmatchSaveRecordDB(dmid)
{
	Debug("deathmatch.inc > DeathmatchSaveRecordDB - Start");
	new temp[MAX_STRING];
	new rdbname[MAX_STRING];
	format(rdbname,sizeof(rdbname),"%s%s.txt",DMRecordDB,Deathmatch[dmid][dm_name]);
	if (!db_Exists(rdbname)) db_Create(rdbname);
	db_BeforeBigSaving(rdbname);
	db_Set(rdbname,"Name",Deathmatch[dmid][dm_name]);
	set(temp,nullstr);
	valstr(temp,DMBestScore[dmid][dm_score_record]);
	db_Set(rdbname,"Best_Score_Record",temp);
	set(temp,nullstr);
	set(temp,DMBestScore[dmid][dm_score_player]);
	db_Set(rdbname,"Best_Score_Player",temp);
	set(temp,nullstr);
	valstr(temp,DMBestScore[dmid][dm_score_kills]);
	db_Set(rdbname,"Best_Score_Kills",temp);
	set(temp,nullstr);
	valstr(temp,DMBestScore[dmid][dm_score_deaths]);
	db_Set(rdbname,"Best_Score_Deaths",temp);
	set(temp,nullstr);
	db_AfterBigSaving(rdbname);
	Debug("deathmatch.inc > DeathmatchSaveRecordDB - Stop");
	return;
}

DMEncodeWeaponsDB(dmid)
{
	Debug("deathmatch.inc > DMEncodeWeaponsDB - Start");
	new weps[MAX_DMWEAPONS+1];
	new retstr[MAX_STRING];
	set(weps,Deathmatch[dmid][dm_weapons]);
	for (new i=0;i<strlen(weps);i++)
	{
		if (weps[i] == 0) break;
		new tempstr[10];
		valstr(tempstr,weps[i]);
		strins(retstr,tempstr,strlen(retstr));
		retstr[strlen(retstr)] = '/';
	}
	Debug("deathmatch.inc > DMEncodeWeaponsDB - Stop");
	return retstr;
}

DMDecodeWeaponsDB(wepstr[])
{
	Debug("deathmatch.inc > DMDecodeWeaponsDB - Start");
	new weps[MAX_DMWEAPONS];
	new tmpwepstr[20];
	new idx;

	for (new i=0;((i<strlen(wepstr)) && (i<MAX_DMWEAPONS));i++)
	{
		set(tmpwepstr,strcharsplit(wepstr, idx,'/'));
		if (strlen(tmpwepstr) == 0) break;
		weps[i] = strval(tmpwepstr);
		if (weps[i] == 0) break;
	}
	//set(Deathmatch[dmid][dm_weapons],weps);
	Debug("deathmatch.inc > DMDecodeWeaponsDB - Stop");
	return weps;
}

stock StopShowingDMInfo(playerid){
	if(DM_TextDrawExists[playerid]){
	    TextDrawDestroy(DM_TextDraw[playerid]);
	    DM_TextDrawExists[playerid] = 0;
	}
}

stock ShowDMInfo(playerid){
	new string[MAX_STRING];
	new dmid=GetPlayerDM(playerid);
	format(string,MAX_STRING,gettext(1013),Deathmatch[dmid][dm_name],FormatPosition(GetPlayerDMPosition(playerid,dmid)),DMPlayerStats[playerid][dm_player_deaths],DMPlayerStats[playerid][dm_player_kills]);
	DM_TextDraw[playerid] = TextDrawCreate(634.0, 380.0, string);
	TextDrawSetShadow(DM_TextDraw[playerid], 0);
	TextDrawAlignment(DM_TextDraw[playerid],3);
	TextDrawSetOutline(DM_TextDraw[playerid], 1);
	TextDrawLetterSize(DM_TextDraw[playerid], 0.4, 1.4);
	TextDrawFont(DM_TextDraw[playerid], 2);
	DM_TextDrawExists[playerid] = 1;
	TextDrawShowForPlayer(playerid, DM_TextDraw[playerid]);
}

stock UpdateDMInfo(playerid){
	new string[MAX_STRING];
	new dmid=GetPlayerDM(playerid);
	format(string,MAX_STRING,gettext(1013),Deathmatch[dmid][dm_name],FormatPosition(GetPlayerDMPosition(playerid,dmid)),DMPlayerStats[playerid][dm_player_deaths],DMPlayerStats[playerid][dm_player_kills]);
	TextDrawSetString(DM_TextDraw[playerid],string);
}

CalculateCashPrize(level){
	if (level == 0) level = 1;
	return level * 1000;
}

CalculateCashEntry(level){
	if (level == 0) level = 1;
	return level * 100;
}

CalculateXPPrize(level){
	if (level == 0) level = 1;
	return level * 500;
}

CalculateXPBonus(level){
	if (level == 0) level = 1;
	return level * 100;
}

CalculateXPForDeath(level){
	if (level == 0) level = 1;
	return level * 5;
}

CalculateXPForKill(level){
	if (level == 0) level = 1;
	return level * 10;
}