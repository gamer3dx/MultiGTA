//Hay module.
//Created by MadCat
//Based on Deathmatch module
//Part of code from GTAT Hay

#include "base"
#include "player"
#include "world"

forward Hay_OnGameModeInit();
forward Hay_OnPlayerDeath(playerid,killerid);
forward Hay_OnPlayerDisconnect(playerid);
forward Hay_OnPlayerEnterCheckpoint(playerid);
forward Hay_OnKeyStateChange(playerid, newkeys, oldkeys);

forward CheckHay();
forward RegisterHay(name[]);

#define MAX_HAY 10
#define MAX_HAY_OBJECTS 200
#define MAX_HAY_LEVELS 50
#define HAY_BLOCKHIGHT 4
#define HAY_LEVELHIGHT 3 // 2.5

#define HAY_STATE_DISABLED 0
#define HAY_STATE_SLEEPING 1
#define HAY_STATE_LINEUP 2
#define HAY_STATE_COUNTDOWN 3
#define HAY_STATE_ACTIVE 4
#define INVALID_HAY_ID 0

enum HayInfo{
	hay_name[MAX_NAME],
	hay_minlevel,
	hay_minplayers,
	hay_maxtime,
	hay_frequency,
	hay_lineupdelay,
	hay_cashprizeperlevel,
	hay_cashentry,
	hay_xpprizeperlevel,
	hay_levels,
	hay_objects,
	hay_movespersecond,
	hay_maxblocksperlevel,
	hay_minblocksperlevel
}
new Hay[MAX_HAY][HayInfo];

enum HayStateInfo{
	hay_state,             // hay state
	hay_timer,
	hay_playercount,
	hay_questid,
	hay_activecpid
}
new HayStats[MAX_HAY][HayStateInfo];

enum HayPlayerInfo{
	bool:hay_player_active, // is the player active and spawned in the hay zone
	hay_player_level
}
new HayPlayerStats[MAX_PLAYERS][HayPlayerInfo];

enum HayScoreInfo {
	hay_score_player[MAX_NAME],
	hay_score_level
};
new HayBestScore[MAX_HAY][HayScoreInfo];

new HayCount; // count of active hays
new HayCPSize=9; // size of checkpoints
new HayStartCP[MAX_HAY][CoordInfo];

new HAY_BlockLevel[MAX_HAY][MAX_HAY_OBJECTS];
new HAY_BlockPos[MAX_HAY][MAX_HAY_OBJECTS];
new Float:HAY_BlockPosX[MAX_HAY][25];
new Float:HAY_BlockPosY[MAX_HAY][25];
new HAY_HayBlocks[MAX_HAY][MAX_HAY_LEVELS];
new HAY_BlockInUse[MAX_HAY][MAX_HAY_LEVELS][MAX_HAY_OBJECTS];
new HAY_ObjectCount[MAX_HAY];
new HAY_ObjectID[MAX_HAY][MAX_HAY_OBJECTS];

new Text:HAY_TextDraw;

Float:GetHayCPX(hayid)
{
	return HayStartCP[hayid][Coord_X];
}
Float:GetHayCPY(hayid)
{
	return HayStartCP[hayid][Coord_Y];
}
Float:GetHayCPZ(hayid)
{
	return HayStartCP[hayid][Coord_Z];
}

//--------------------------------------------------------------

new HayDB[MAX_STRING] = "MultiGTA/Hays/Hays.list";
new HayBaseDB[MAX_STRING] = "MultiGTA/Hays/";
new HayRecordDB[MAX_STRING] = "MultiGTA/Hays/Records/";

new TimerCheckHay;

public Hay_OnGameModeInit(){
	HAY_TextDraw = TextDrawCreate(634.0, 420.0, "not started");
	TextDrawSetShadow(HAY_TextDraw, 0);
	TextDrawAlignment(HAY_TextDraw,3);
	TextDrawSetOutline(HAY_TextDraw, 1);
	TextDrawLetterSize(HAY_TextDraw, 0.4, 1.4);
	TextDrawFont(HAY_TextDraw, 2);
	HayLoadAll();
	TimerCheckHay = SetTimer("CheckHay", 1000, 1);
	new logstring[MAX_STRING];
	format(logstring,MAX_STRING,"Hays System Loaded. %d hays loaded.",HayCount);
	WriteLog(logstring);
	return 0;
}

public CheckHay() // must be ran by timer every second
{
 	Debug("hay.inc > CheckHay - Start");
	new string [MAX_STRING];
	for (new hayid=1; hayid<=HayCount;hayid++)
	{

		if (HayStats[hayid][hay_state] == HAY_STATE_DISABLED) continue;

		if (HayStats[hayid][hay_state] == HAY_STATE_SLEEPING)
		{
			if (HayStats[hayid][hay_timer] <= 0) // if it is time to run
			{
				if (event_now == false){
					HayStats[hayid][hay_timer] = 0;
					StartHayLineup(hayid);
					continue;
				} else {
					HayStats[hayid][hay_timer] = MakeHaySleepTime(hayid);
				}
			}
			HayStats[hayid][hay_timer]--;
			continue;
		}

		HayStats[hayid][hay_timer]++;
		if (HayStats[hayid][hay_state] == HAY_STATE_LINEUP)
		{
			if (HayStats[hayid][hay_timer] >= Hay[hayid][hay_lineupdelay]) // if it is time to run
			{
				HayStats[hayid][hay_timer] = 0;
				StartHayCountdown(hayid);
			}
			else
			{
				if (HayStats[hayid][hay_playercount] > 0)
				{
					new haycountdown = Hay[hayid][hay_lineupdelay] - HayStats[hayid][hay_timer];
					if ((haycountdown == 5) || (haycountdown == 10) || (haycountdown == 20) || (haycountdown == 30) || (haycountdown == 45) || (haycountdown == 60) || (haycountdown == 90))
					{
						format(string, sizeof(string),gettext(1686),(Hay[hayid][hay_lineupdelay] - HayStats[hayid][hay_timer]));
						SystemMsgToHayPlayers(hayid,string);
					}
				}
			}
		}
		else if (HayStats[hayid][hay_state] == HAY_STATE_COUNTDOWN)
		{
			for (new playerid=0; playerid<MAX_PLAYERS_EX;playerid++)
			{
				if (IsPlayerConnected(playerid))
				{
					if (PlayerQuest[playerid] == GetHayQuestID(hayid)) // if player is in this hay
					{
						if (HayPlayerStats[playerid][hay_player_active] == true)
						{
							new Countdown = MAX_COUNTDOWN - HayStats[hayid][hay_timer];
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
			if (HayStats[hayid][hay_timer] >= MAX_COUNTDOWN)
			{
				StartHay(hayid);
			}
		}
		else if (HayStats[hayid][hay_state] == HAY_STATE_ACTIVE)
		{
		    	Hay_DoWhileActive(hayid);
			if (HayStats[hayid][hay_timer] >= Hay[hayid][hay_maxtime]) // if time limit reached
			{
				EndHay(hayid);
			}
		}
	}
	Debug("hay.inc > CheckHay - Stop");
}

public Hay_OnPlayerDisconnect(playerid)
{
	Debug("hay.inc > Hay_OnPlayerDisconnect - Start");
	new hayid = GetPlayerHay(playerid);
	if (hayid != INVALID_HAY_ID) PlayerLeaveHay(playerid,hayid);
	Debug("hay.inc > Hay_OnPlayerDisconnect - Stop");
	return 0;
}

public RegisterHay(name[])
{
	Debug("hay.inc > RegisterHay - Start");
	for (new i=0;i<MAX_HAY;i++)
	{
		if (HayStats[i][hay_state] == HAY_STATE_DISABLED) continue;
		if (strcomp(name,Hay[i][hay_name],true)==1)
		{
			Debug("hay.inc > RegisterHay - Stop");
			return INVALID_HAY_ID;
		}
	}
	if (HayCount >= MAX_HAY) return INVALID_HAY_ID;
	HayCount++;
	new hayid = HayCount;
	HayStats[hayid][hay_questid] = RegisterQuest(name);
	if (HayStats[hayid][hay_questid] == INVALID_QUEST_ID)
	{
		HayCount--;
		Debug("hay.inc > RegisterHay - Stop");
		return INVALID_HAY_ID;
	}
	set(Hay[hayid][hay_name],name);
	Debug("hay.inc > RegisterHay - Stop");
	return hayid;
}


StartHayLineup(hayid)
{
	Debug("hay.inc > StartHayLineup - Start");
	HayStats[hayid][hay_state] = HAY_STATE_LINEUP;
	HayStats[hayid][hay_timer] = 0;
	HayStats[hayid][hay_playercount] = 0;
	new string1[MAX_STRING];
	new string2[MAX_STRING];
	event_now=true;
	format(string1, sizeof(string1),gettext(1687),Hay[hayid][hay_name],hayid,Hay[hayid][hay_lineupdelay],GetHayStartZoneName(hayid),Hay[hayid][hay_levels]);
	format(string2, sizeof(string2),gettext(1688),Hay[hayid][hay_cashentry],Hay[hayid][hay_minlevel],Hay[hayid][hay_minplayers]);
	for (new playerid=0; playerid<MAX_PLAYERS_EX;playerid++)
	{
		if (!IsPlayerConnected(playerid)) continue;
		SystemMsg(playerid,COLOUR_HAY,string1);
		SystemMsg(playerid,COLOUR_HAY,string2);
		ShowTextForPlayer(playerid,gettext(1690), 5000, 1);
		if ((PlayerQuest[playerid] == 0) && (Player[playerid][GotJob] != JOB_COP)) // if player not on a quest
		{
			oSetPlayerCheckpoint(playerid,GetHayCPX(hayid),GetHayCPY(hayid),GetHayCPZ(hayid),HayCPSize);
			HAYResetPlayerStats(playerid);
		} else {
			SystemMsg(playerid,COLOUR_HAY_BAD,gettext(1691));
			oDisablePlayerCheckpoint(playerid);
		}
	}
	HayStats[hayid][hay_activecpid] = AddActiveCheckpoint(GetHayCPX(hayid),GetHayCPY(hayid),GetHayCPZ(hayid),10000,8);
	Debug("hay.inc > StartHayLineup - Stop");
}

StartHayCountdown(hayid)
{
	Debug("hay.inc > StartHayCountdown - Start");
	HayStats[hayid][hay_timer]=0;
	//if (HayStats[hayid][hay_activecpid] != 0)
	//{
	// RemoveActiveCheckpoint(HayStats[hayid][hay_activecpid]);
	// HayStats[hayid][hay_activecpid] = 0;
	//}
	new string[MAX_STRING];


	for (new playerid=0; playerid<MAX_PLAYERS_EX;playerid++)
	{
		if (!IsPlayerConnected(playerid)) continue;
		if (PlayerQuest[playerid] == GetHayQuestID(hayid)) // if player is in this hay
		{
			if (!oIsPlayerInCheckpoint(playerid,GetHayCPX(hayid),GetHayCPY(hayid),GetHayCPZ(hayid),50))
			{
				PlayerLeaveHay(playerid,hayid);
				SystemMsg(playerid,COLOUR_HAY_BAD,gettext(1692));
			}
		}
	}

	if (HayStats[hayid][hay_playercount] < Hay[hayid][hay_minplayers])
	{
		format(string, sizeof(string),gettext(1693),Hay[hayid][hay_name]);
		SystemMsgToRegistered(COLOUR_HAY_BAD,string);

		new logstring[MAX_STRING];
		format(logstring, MAX_STRING, "Hay '%s' did not run. Not enough players.",Hay[hayid][hay_name]);
		WriteLog(logstring);

		CleanupHay(hayid);
		Debug("hay.inc > StartHayCountdown - Stop");
		return;
	}

	format(string, sizeof(string),gettext(1694),Hay[hayid][hay_name]);
	SystemMsgToRegistered(COLOUR_HAY,string);
	new logstring[MAX_STRING];
	format(logstring, MAX_STRING, "Hay '%s' starting.",Hay[hayid][hay_name]);
	WriteLog(logstring);
	HayStats[hayid][hay_state] = HAY_STATE_COUNTDOWN;

	for (new playerid=0; playerid<MAX_PLAYERS_EX;playerid++)
	{
		if (!IsPlayerConnected(playerid)) continue;
		if (PlayerQuest[playerid] == GetHayQuestID(hayid)) // if player is in this hay
		{
			if (oIsPlayerInCheckpoint(playerid,GetHayCPX(hayid),GetHayCPY(hayid),GetHayCPZ(hayid),50))
			{
				ShowHayInfo(playerid);
				TogglePlayerControllable(playerid, 0);
				SetCameraBehindPlayer(playerid);
				Hay_OnPlayerSpawn(playerid,hayid);
			}
		}
	}
	PrepareHay(hayid);
	Debug("hay.inc > StartHayCountdown - Stop");
}

StartHay(hayid)
{
	Debug("hay.inc > StartHay - Start");
	HayStats[hayid][hay_timer]=0;
	HayStats[hayid][hay_state] = HAY_STATE_ACTIVE;
	for (new playerid=0; playerid<MAX_PLAYERS_EX;playerid++)
	{
		if (IsPlayerConnected(playerid))
		{
			if (PlayerQuest[playerid] == GetHayQuestID(hayid)) // if player is in this hay
			{
				Player[playerid][HaysPlayed]++;
				TogglePlayerControllable(playerid, 1);
				UpdateHayInfo(hayid);
			}
		}
	}
	Debug("hay.inc > StartHay - Stop");
}

JoinHay(playerid,hayid)
{
	Debug("hay.inc > JoinHay - Start");
	new string[MAX_STRING];
	if (oGetPlayerMoney(playerid) < Hay[hayid][hay_cashentry])
	{
		format(string, sizeof(string),gettext(1695), Hay[hayid][hay_cashentry]);
		SystemMsg(playerid,COLOUR_HAY_BAD,string);
		Debug("hay.inc > JoinHay - Stop");
		return;
	}
	if (GetPlayerLevel(playerid) < Hay[hayid][hay_minlevel])
	{
		format(string, sizeof(string),gettext(1696), Hay[hayid][hay_minlevel]);
		SystemMsg(playerid,COLOUR_HAY_BAD,string);
		Debug("hay.inc > JoinHay - Stop");
		return;
	}
	HAYResetPlayerStats(playerid);
	HayStats[hayid][hay_playercount]++;
	if (Hay[hayid][hay_minplayers] > 1)
	{
		format(string, sizeof(string),gettext(1697), oGetPlayerName(playerid),HayStats[hayid][hay_playercount],Hay[hayid][hay_minplayers]);
		new logstring[MAX_STRING];
		format(logstring, MAX_STRING, "player: %d:  %s: has joined the hay. (Players:%d/%d)",playerid,oGetPlayerName(playerid),HayStats[hayid][hay_playercount],Hay[hayid][hay_minplayers]);
		WriteLog(logstring);
	}
	else
	{
		format(string, sizeof(string),gettext(1698), oGetPlayerName(playerid),HayStats[hayid][hay_playercount]);
		new logstring[MAX_STRING];
		format(logstring, MAX_STRING, "player: %d:  %s: has joined the hay. (Players:%d)",playerid,oGetPlayerName(playerid),HayStats[hayid][hay_playercount]);
		WriteLog(logstring);
	}
	SystemMsgToHayPlayers(hayid,string);

	oGivePlayerMoney(playerid,0-Hay[hayid][hay_cashentry],1);
	ShowTextForPlayer(playerid,gettext(1699),3000,1);
	PlayerQuest[playerid] = GetHayQuestID(hayid);

	if (HayStats[hayid][hay_state] == HAY_STATE_LINEUP)
	{
		format(string, sizeof(string),gettext(1700),(Hay[hayid][hay_lineupdelay] - HayStats[hayid][hay_timer]));
		SystemMsg(playerid,COLOUR_HAY,string);
		oSetPlayerCheckpoint(playerid,GetHayCPX(hayid),GetHayCPY(hayid),GetHayCPZ(hayid),HayCPSize);
	}
	else
	{
		SystemMsg(playerid,COLOUR_HAY_BAD,gettext(1701));
		Debug("hay.inc > JoinHay - Stop");
		return;
	}
	Debug("hay.inc > JoinHay - Stop");
}

Hay_OnPlayerSpawn(playerid,hayid)
{
	Debug("hay.inc > Hay_OnPlayerSpawn - Start");
//	if (!IsPlayerInHay(playerid,hayid))
//	{
//		HAYResetPlayerStats(playerid);
//		Debug("hay.inc > Hay_OnPlayerSpawn - Stop");
//		return;
//	}

	oSetPlayerHealth(playerid,GetMaxHealth(playerid)); // set health based on level
	HayPlayerStats[playerid][hay_player_active] = true;
	HayPlayerStats[playerid][hay_player_level] = 0;
	oDisablePlayerCheckpoint(playerid);
	SetPlayerVirtualWorld(playerid,WORLD_HAY);
	SetPlayerWorldBounds(playerid,(HayStartCP[hayid][Coord_X]+50),(HayStartCP[hayid][Coord_X]-50),(HayStartCP[hayid][Coord_Y]+50),(HayStartCP[hayid][Coord_Y]-50));
	ResetPlayerWeapons(playerid);
	new Float:startx,Float:starty;
	new ran = random(2);
	if(ran == 0) startx=HayStartCP[hayid][Coord_X]+random(10)+10;
	if(ran == 1) startx=HayStartCP[hayid][Coord_X]-random(10)-10;
	ran = random(2);
	if(ran == 0) starty=HayStartCP[hayid][Coord_Y]+random(10)+10;
	if(ran == 1) starty=HayStartCP[hayid][Coord_Y]-random(10)-10;
	oSetPlayerPos(playerid,startx, starty, HayStartCP[hayid][Coord_Z]+3);
	SystemMsg(playerid,COLOUR_HAY,gettext(1683));
	Debug("hay.inc > Hay_OnPlayerSpawn - Stop");
	//oDisablePlayerCheckpoint(playerid);
}


EndHay(hayid)
{
	Debug("hay.inc > EndHay - Start");
	new winnerid = GetHayWinner(hayid);
	for (new playerid=0; playerid<MAX_PLAYERS_EX;playerid++)
	{
		if (!IsPlayerConnected(playerid)) continue;
		if (PlayerQuest[playerid] == GetHayQuestID(hayid)) // if player is in this hay
		{
			if (playerid == winnerid){
				PlayerWonHay(playerid,hayid);
			} else {
				PlayerLeaveHay(playerid,hayid);
			}
		}
	}
	CleanupHay(hayid);
	Debug("hay.inc > EndHay - Stop");
}

CleanupHay(hayid)
{
	Debug("hay.inc > CleanupHay - Start");
	for (new playerid=0; playerid<MAX_PLAYERS_EX;playerid++)
	{
		if (!IsPlayerConnected(playerid)) continue;
		if (PlayerQuest[playerid] == GetHayQuestID(hayid)) // if player is in this hay
		{
			PlayerLeaveHay(playerid,hayid);
		}
		if (IsPlayerConnected(playerid))
		{
			oDisablePlayerCheckpoint(playerid);
		}
	}
	HayStats[hayid][hay_playercount] = 0;
	HayStats[hayid][hay_timer] = MakeHaySleepTime(hayid);
	HayStats[hayid][hay_state] = HAY_STATE_SLEEPING;
	event_now=false;
	if (HayStats[hayid][hay_activecpid] != 0)
	{
		RemoveActiveCheckpoint(HayStats[hayid][hay_activecpid]);
		HayStats[hayid][hay_activecpid] = 0;
	}
	for(new i=0; i<HAY_ObjectCount[hayid]; i++){
	    DestroyStreamObject(HAY_ObjectID[hayid][i]);
	}
	for(new i=0; i<Hay[hayid][hay_levels]; i++){
		HAY_HayBlocks[hayid][i] = 0;
	    	for(new j=0; j<25; j++){
	        	HAY_BlockInUse[hayid][i][j] = 0;
	    	}
	}
	Debug("hay.inc > CleanupHay - Stop");
}

PlayerWonHay(playerid,hayid)
{
	Debug("hay.inc > PlayerWonHay - Start");

	PlayerPlaySoundOnPlayer(playerid,1057);

	new string[MAX_STRING];
	format(string, sizeof(string),gettext(1702),Hay[hayid][hay_name],HayPlayerStats[playerid][hay_player_level]);
	SystemMsg(playerid,COLOUR_HAY,string);
	Player[playerid][HaysWon]++;
	format(string, sizeof(string),gettext(1703),oGetPlayerName(playerid),Hay[hayid][hay_name],HayPlayerStats[playerid][hay_player_level]);
	SystemMsgToRegistered(COLOUR_HAY,string);
	new logstring[MAX_STRING];
	format(logstring, MAX_STRING, "player: %d:  %s: has won hay '%s'! Score: %d",playerid,oGetPlayerName(playerid),Hay[hayid][hay_name],HayPlayerStats[playerid][hay_player_level]);
	WriteLog(logstring);
	oGivePlayerMoney(playerid,Hay[hayid][hay_cashprizeperlevel]*HayPlayerStats[playerid][hay_player_level],1);
	GivePlayerXP(playerid,Hay[hayid][hay_xpprizeperlevel]*HayPlayerStats[playerid][hay_player_level],1);
	SetPlayerCriminal(playerid,gettext(1704),13);
	BetWinner(playerid,3);

	if ((HayPlayerStats[playerid][hay_player_level] > HayBestScore[hayid][hay_score_level]) || (HayBestScore[hayid][hay_score_level] == 0))
	{
		format(string, sizeof(string),gettext(1705),oGetPlayerName(playerid),Hay[hayid][hay_name],HayPlayerStats[playerid][hay_player_level],HayBestScore[hayid][hay_score_level]);
		SystemMsgToRegistered(COLOUR_HAY,string);
		format(logstring, MAX_STRING, "player: %d:  %s: has set a new record for hay '%s'! Score: %d. Old record: %d.",playerid,oGetPlayerName(playerid),Hay[hayid][hay_name],HayPlayerStats[playerid][hay_player_level],HayBestScore[hayid][hay_score_level]);
		WriteLog(logstring);
		oGivePlayerMoney(playerid,(Hay[hayid][hay_cashprizeperlevel]*HayPlayerStats[playerid][hay_player_level]*2),1);
		GivePlayerXP(playerid,(Hay[hayid][hay_xpprizeperlevel]*HayPlayerStats[playerid][hay_player_level]*2),1);
		HayBestScore[hayid][hay_score_level] = HayPlayerStats[playerid][hay_player_level];
		set(HayBestScore[hayid][hay_score_player],oGetPlayerName(playerid));

	}

	PlayerLeaveHay(playerid,hayid);

	HaySaveRecordDB(hayid);

	Debug("hay.inc > PlayerWonHay - Stop");
}

PlayerLeaveHay(playerid,hayid)
{
	Debug("hay.inc > PlayerLeaveHay - Start");
	new string[MAX_STRING];
	ResetQuest(playerid);  // reset checkpoints
	HayStats[hayid][hay_playercount]--;

	if (!IsPlayerConnected(playerid))
	{
		HAYResetPlayerStats(playerid);
		Debug("hay.inc > PlayerLeaveHay - Stop");
		return;
	}
	if (HayStats[hayid][hay_state] == HAY_STATE_LINEUP)
	{
		if (Hay[hayid][hay_minplayers] > 1)
		{
			format(string, sizeof(string),gettext(1706), oGetPlayerName(playerid),HayStats[hayid][hay_playercount],Hay[hayid][hay_minplayers]);
			new logstring[MAX_STRING];
			format(logstring, MAX_STRING, "player: %d:  %s: has left the hay. (Players:%d/%d)",playerid,oGetPlayerName(playerid),HayStats[hayid][hay_playercount],Hay[hayid][hay_minplayers]);
			WriteLog(logstring);
		}
		else
		{
			format(string, sizeof(string),gettext(1707), oGetPlayerName(playerid),HayStats[hayid][hay_playercount]);
			new logstring[MAX_STRING];
			format(logstring, MAX_STRING, "player: %d:  %s: has left the hay. (Players:%d)",playerid,oGetPlayerName(playerid),HayStats[hayid][hay_playercount]);
			WriteLog(logstring);
		}
		SystemMsgToHayPlayers(hayid,string);
		oGivePlayerMoney(playerid,Hay[hayid][hay_cashentry],1); // give player back thier cash entry
		ShowTextForPlayer(playerid, gettext(1708),5000,1);
		SystemMsg(playerid,COLOUR_HAY,gettext(1709));
	}
	else
	{
		format(string, sizeof(string),gettext(1710), oGetPlayerName(playerid),HayStats[hayid][hay_playercount]);
		SystemMsgToHayPlayers(hayid,string);
		new logstring[MAX_STRING];
		format(logstring, MAX_STRING, "player: %d:  %s: has left the hay. (Players:%d)",playerid,oGetPlayerName(playerid),HayStats[hayid][hay_playercount]);
		WriteLog(logstring);
		SystemMsg(playerid,COLOUR_HAY,gettext(1711));
	}
	StopShowingHayInfo(playerid);

	if (HayPlayerStats[playerid][hay_player_active])
	{
		oSetPlayerPos(playerid,HayStartCP[hayid][Coord_X], HayStartCP[hayid][Coord_Y], HayStartCP[hayid][Coord_Z]);
		oSetPlayerHealth(playerid,GetMaxHealth(playerid)); // set health based on level
		oDisablePlayerCheckpoint(playerid);
		GivePlayerOwnedWeapons(playerid);
		oGivePlayerMoney(playerid,Hay[hayid][hay_cashprizeperlevel]*HayPlayerStats[playerid][hay_player_level],1);
		GivePlayerXP(playerid,Hay[hayid][hay_xpprizeperlevel]*HayPlayerStats[playerid][hay_player_level],1);
	}

	if (HayStats[hayid][hay_playercount] == 1){
		PlayerWonHay(GetHayWinner(hayid),hayid);
	}

	if (HayStats[hayid][hay_playercount] <= 0){
		CleanupHay(hayid);
	}
	HAYResetPlayerStats(playerid);
	SetPlayerServerWorldBounds(playerid);
	TogglePlayerControllable(playerid,1);
	SetPlayerVirtualWorld(playerid,WORLD_DEFAULT);
	SetSpecialColorForPlayer(playerid);
	Debug("hay.inc > PlayerLeaveHay - Stop");
}

SystemMsgToHayPlayers(hayid,string[])
{
	for (new hayplayerid=0; hayplayerid<MAX_PLAYERS_EX;hayplayerid++)
	{
		if (!IsPlayerConnected(hayplayerid)) continue;
		if (PlayerQuest[hayplayerid] == GetHayQuestID(hayid)) // if player is in this hay
		{
			SystemMsg(hayplayerid,COLOUR_HAY,string);
		}
	}
}

IsPlayerInHay(playerid,hayid)
{
	if (hayid == INVALID_HAY_ID) return 0;
	if (!IsPlayerConnected(playerid)) return 0;
	if (PlayerQuest[playerid] != GetHayQuestID(hayid)) return 0; 
	return 1;
}

IsPlayerInAnyHay(playerid)
{
	new hayid = GetPlayerHay(playerid);
	if (hayid == INVALID_HAY_ID) return 0;
	if (HayPlayerStats[playerid][hay_player_active] == false) return 0; // if we are not spawned in hay zone then we are not actualy active in hay
	return 1;
}

HAYResetPlayerStats(playerid)
{
	Debug("hay.inc > HAYResetPlayerStats - Start");
	HayPlayerStats[playerid][hay_player_active] = false;
	HayPlayerStats[playerid][hay_player_level] = 0;
	Debug("hay.inc > HAYResetPlayerStats - Stop");
}

GetPlayerHay(playerid)
{
	Debug("hay.inc > GetPlayerHay - Start");
	if (!IsPlayerConnected(playerid)) {Debug("hay.inc > GetPlayerHay - Stop"); return 0;}
	for (new hayid=1; hayid<=HayCount;hayid++)
	{
		if (HayStats[hayid][hay_state] == HAY_STATE_DISABLED) continue;
		if (PlayerQuest[playerid] == GetHayQuestID(hayid)) // if player is in this hay
		{
			Debug("hay.inc > GetPlayerHay - Stop");
			return hayid;
		}
	}
	Debug("hay.inc > GetPlayerHay - Stop");
	return INVALID_HAY_ID;
}

GetHayQuestID(hayid)
{
	return HayStats[hayid][hay_questid];
}


//----------------------------------------

GetHayStartZoneName(hayid)
{
	new zone[MAX_STRING];
	zone = GetXYZZoneName(GetHayCPX(hayid),GetHayCPY(hayid),GetHayCPZ(hayid));
	return zone;
}

MakeHaySleepTime(hayid)
{
	Debug("hay.inc > MakeHaySleepTime - Start");
	new sleeptime;
	if (Hay[hayid][hay_frequency] == 0) Hay[hayid][hay_frequency] = 5;
	sleeptime = ((Hay[hayid][hay_frequency] * HayCount) * 150);
	Debug("hay.inc > MakeHaySleepTime - Stop");
	return sleeptime;
}

public Hay_OnPlayerDeath(playerid,killerid)
{
	#pragma unused killerid
	Debug("hay.inc > Hay_OnPlayerDeath - Start");

	new hayid = GetPlayerHay(playerid);
//	if ((hayid == INVALID_HAY_ID) || (!IsPlayerInHay(playerid,hayid)))
//	{
//		HAYResetPlayerStats(playerid);
//		Debug("hay.inc > Hay_OnPlayerDeath - Stop");
//		return; // player not in a hay
//	}

	PlayerLeaveHay(playerid,hayid);
	PlayerPlaySoundOnPlayer(playerid,1057);	
	Debug("hay.inc > Hay_OnPlayerDeath - Stop");
	return 0;
}


public Hay_OnPlayerEnterCheckpoint(playerid)
{
	new playerhayid = GetPlayerHay(playerid);
	new string[MAX_STRING];
	for (new hayid=1; hayid<=HayCount;hayid++) //  for each hay
	{
		//if (HayStats[hayid][hay_state] == HAY_STATE_DISABLED) break;

		if (HayStats[hayid][hay_state] == HAY_STATE_LINEUP)
		{
			if (PlayerQuest[playerid] == 0 && Player[playerid][GotJob] != JOB_COP) // if player not on a quest
			{
				// if player at startline
				if (oIsPlayerInCheckpoint(playerid,GetHayCPX(hayid),GetHayCPY(hayid),GetHayCPZ(hayid),HayCPSize*3))
				{
					JoinHay(playerid,hayid);
				}
			}
			else if (playerhayid == hayid)
			{
				if (oIsPlayerInCheckpoint(playerid,GetHayCPX(hayid),GetHayCPY(hayid),GetHayCPZ(hayid),HayCPSize*3))
				{
					format(string,sizeof(string),gettext(1712),(Hay[hayid][hay_lineupdelay] - HayStats[hayid][hay_timer]));
					SystemMsg(playerid,COLOUR_HAY,string);
				}
			}
		}
	}
}

HayLoadAll()
{
	Debug("hay.inc > HayLoadAll - Start");
	new temp[MAX_STRING];
	if (!db_Exists(DatabaseDB)) db_Create(DatabaseDB);
	set(temp,db_Get(DatabaseDB,"Hay_DB"));
	if (strlen(temp) > 0) set(HayDB,temp);
	set(temp,db_Get(DatabaseDB,"Hay_Base_DB"));
	if (strlen(temp) > 0) set(HayBaseDB,temp);
	set(temp,db_Get(DatabaseDB,"Hay_Record_DB"));
	if (strlen(temp) > 0) set(HayRecordDB,temp);

	if (!db_Exists(HayDB))
	{
		db_Create(HayDB);
	}

	for (new haydbid=0;haydbid<MAX_HAY;haydbid++)
	{ // load all our hays from db
		new cellname[MAX_STRING];
		format(cellname,sizeof(cellname),"Hay%d",haydbid);
		set(temp,db_Get(HayDB,cellname));
		if (strlen(temp) == 0) continue;
		if (!HayBaseDBExists(temp)) continue;
		new hayid = RegisterHay(temp);
		if (hayid == INVALID_HAY_ID) continue;
		HayLoadBaseDB(hayid);
		HayLoadRecordDB(hayid);
	}
	Debug("hay.inc > HayLoadAll - Stop");
	return;
}

HayBaseDBExists(hayname[MAX_STRING])
{ 
	new rdbname[MAX_STRING];
	format(rdbname,sizeof(rdbname),"%s%s.txt",HayBaseDB,hayname);
	if (!db_Exists(rdbname))
	{
		return 0;
	}
	return 1;
}

HayLoadBaseDB(hayid)
{ // load hay from db
	Debug("hay.inc > HayLoadBaseDB - Start");
	new temp[MAX_STRING];
	new rdbname[MAX_STRING];
	format(rdbname,sizeof(rdbname),"%s%s.txt",HayBaseDB,Hay[hayid][hay_name]);
	if (!db_Exists(rdbname))
	{
		Debug("hay.inc > HayLoadBaseDB - Stop");
		return INVALID_HAY_ID;
	}
	set(temp,db_Get(rdbname,"Name"));
	if (strlen(temp) > 0) set(Hay[hayid][hay_name],temp);
	set(temp,nullstr);

	if (hayid <= 0)
	{
		new logstring[MAX_STRING];
		format(logstring, MAX_STRING, "Hay (DB): %s Failed to load", Hay[hayid][hay_name]);
		WriteLog(logstring);
		Debug("hay.inc > HayLoadBaseDB - Stop");
		return INVALID_HAY_ID;
	}

	set(temp,db_Get(rdbname,"Frequency"));
	if (strlen(temp) > 0) Hay[hayid][hay_frequency] = strval(temp);
	set(temp,nullstr);

	set(temp,db_Get(rdbname,"Lineup_Delay"));
	if (strlen(temp) > 0) Hay[hayid][hay_lineupdelay] = strval(temp);
	set(temp,nullstr);

	set(temp,db_Get(rdbname,"Min_Players"));
	if (strlen(temp) > 0) Hay[hayid][hay_minplayers] = strval(temp);
	set(temp,nullstr);

	//if (Hay[hayid][hay_minplayers] < 2) Hay[hayid][hay_minplayers] = 2;

	set(temp,db_Get(rdbname,"Min_Level"));
	if (strlen(temp) > 0) Hay[hayid][hay_minlevel] = strval(temp);
	set(temp,nullstr);

	set(temp,db_Get(rdbname,"Cash_Prize_Per_Level"));
	if (strlen(temp) > 0) Hay[hayid][hay_cashprizeperlevel] = strval(temp);
	set(temp,nullstr);

	set(temp,db_Get(rdbname,"Cash_Entry"));
	if (strlen(temp) > 0) Hay[hayid][hay_cashentry] = strval(temp);
	set(temp,nullstr);

	set(temp,db_Get(rdbname,"XP_Prize_Per_Level"));
	if (strlen(temp) > 0) Hay[hayid][hay_xpprizeperlevel] = strval(temp);
	set(temp,nullstr);

	set(temp,db_Get(rdbname,"Max_Time"));
	if (strlen(temp) > 0) Hay[hayid][hay_maxtime] = strval(temp);
	set(temp,nullstr);

	set(temp,db_Get(rdbname,"Levels"));
	if (strlen(temp) > 0) Hay[hayid][hay_levels] = strval(temp);
	set(temp,nullstr);

	set(temp,db_Get(rdbname,"Objects"));
	if (strlen(temp) > 0) Hay[hayid][hay_objects] = strval(temp);
	set(temp,nullstr);

	set(temp,db_Get(rdbname,"Moves_Per_second"));
	if (strlen(temp) > 0) Hay[hayid][hay_movespersecond] = strval(temp);
	set(temp,nullstr);

	set(temp,db_Get(rdbname,"Max_Blocks_Per_Level"));
	if (strlen(temp) > 0) Hay[hayid][hay_maxblocksperlevel] = strval(temp);
	set(temp,nullstr);

	set(temp,db_Get(rdbname,"Min_Blocks_Per_Level"));
	if (strlen(temp) > 0) Hay[hayid][hay_minblocksperlevel] = strval(temp);
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
			HayStartCP[hayid][Coord_X] = X;
			HayStartCP[hayid][Coord_Y] = Y;
			HayStartCP[hayid][Coord_Z] = Z;
			X= 0.0;
			Y = 0.0;
			Z = 0.0;
		}
	}

	HayStats[hayid][hay_state] = HAY_STATE_SLEEPING;
	HayStats[hayid][hay_timer] = MakeHaySleepTime(hayid);
	new logstring[MAX_STRING];
	format(logstring, MAX_STRING, "Hay (DB): %s - loaded", Hay[hayid][hay_name]);
	WriteLog(logstring);
	Debug("hay.inc > HayLoadBaseDB - Stop");
	return hayid;
}

HayLoadRecordDB(hayid)
{
	Debug("hay.inc > HayLoadRecordDB - Start");
	new temp[MAX_STRING];
	new rdbname[MAX_STRING];
	format(rdbname,sizeof(rdbname),"%s%s.txt",HayRecordDB,Hay[hayid][hay_name]);
	if (!db_Exists(rdbname)) { Debug("hay.inc > HayLoadRecordDB - Stop"); return;}
	set(temp,db_Get(rdbname,"Best_Score_Level"));
	if (strlen(temp) > 0) HayBestScore[hayid][hay_score_level] = strval(temp);
	set(temp,nullstr);
	set(temp,db_Get(rdbname,"Best_Score_Player"));
	if (strlen(temp) > 0) set(HayBestScore[hayid][hay_score_player],temp);
	set(temp,nullstr);
	Debug("hay.inc > HayLoadRecordDB - Stop");
}

HaySaveAll()
{
	Debug("hay.inc > HaySaveAll - Start");
	if (!db_Exists(DatabaseDB)) db_Create(DatabaseDB);
	db_Set(DatabaseDB,"Hay_Base_DB",HayBaseDB);
	db_Set(DatabaseDB,"Hay_Record_DB",HayRecordDB);

	if (!db_Exists(HayDB)) db_Create(HayDB);
	for (new hayid=1;hayid<=HayCount;hayid++)
	{ // load all our hays from db
		if (HayStats[hayid][hay_state] == HAY_STATE_DISABLED) continue;
		new cellname[MAX_STRING];
		format(cellname,sizeof(cellname),"Hay%d",hayid);
		db_Set(HayDB,cellname,Hay[hayid][hay_name]);

		HaySaveBaseDB(hayid);
		HaySaveRecordDB(hayid);
	}
	Debug("hay.inc > HaySaveAll - Stop");
}

HaySaveScores()
{
	Debug("hay.inc > HaySaveScores - Start");
	if (!db_Exists(DatabaseDB)) db_Create(DatabaseDB);
	db_Set(DatabaseDB,"Hay_Base_DB",HayBaseDB);

	if (!db_Exists(HayDB)) db_Create(HayDB);
	for (new hayid=1;hayid<=HayCount;hayid++)
	{ // load all our hays from db
		if (HayStats[hayid][hay_state] == HAY_STATE_DISABLED) continue;
		new cellname[MAX_STRING];
		format(cellname,sizeof(cellname),"Hay%d",hayid);

		db_Set(HayDB,cellname,Hay[hayid][hay_name]);
		HaySaveRecordDB(hayid);
	}
	Debug("hay.inc > HaySaveScores - Stop");
}

HaySaveBaseDB(hayid)
{ // save hay to db
	Debug("hay.inc > HaySaveBaseDB - Start");
	if (HayStats[hayid][hay_state] == HAY_STATE_DISABLED) {Debug("hay.inc > HaySaveBaseDB - Stop"); return;}
	new temp[MAX_STRING];
	new rdbname[MAX_STRING];
	format(rdbname,sizeof(rdbname),"%s%s.txt",HayBaseDB,Hay[hayid][hay_name]);
	if (db_Exists(rdbname))
	{
		Debug("hay.inc > HaySaveBaseDB - Stop");
		return;
	}

	db_Create(rdbname);

	db_BeforeBigSaving(rdbname);
	db_Set(rdbname,"Name",Hay[hayid][hay_name]);

	valstr(temp,Hay[hayid][hay_frequency]);
	db_Set(rdbname,"Frequency",temp);
	set(temp,nullstr);

	valstr(temp,Hay[hayid][hay_lineupdelay]);
	db_Set(rdbname,"Lineup_Delay",temp);
	set(temp,nullstr);

	valstr(temp,Hay[hayid][hay_minplayers]);
	db_Set(rdbname,"Min_Players",temp);
	set(temp,nullstr);

	valstr(temp,Hay[hayid][hay_minlevel]);
	db_Set(rdbname,"Min_Level",temp);
	set(temp,nullstr);

	valstr(temp,Hay[hayid][hay_cashprizeperlevel]);
	db_Set(rdbname,"Cash_Prize_Per_Level",temp);
	set(temp,nullstr);

	valstr(temp,Hay[hayid][hay_cashentry]);
	db_Set(rdbname,"Cash_Entry",temp);
	set(temp,nullstr);

	valstr(temp,Hay[hayid][hay_xpprizeperlevel]);
	db_Set(rdbname,"XP_Prize_Per_Level",temp);
	set(temp,nullstr);

	valstr(temp,Hay[hayid][hay_maxtime]);
	db_Set(rdbname,"Max_Time",temp);
	set(temp,nullstr);

	valstr(temp,Hay[hayid][hay_levels]);
	db_Set(rdbname,"Levels",temp);
	set(temp,nullstr);

	valstr(temp,Hay[hayid][hay_objects]);
	db_Set(rdbname,"Objects",temp);
	set(temp,nullstr);

	valstr(temp,Hay[hayid][hay_movespersecond]);
	db_Set(rdbname,"Moves_Per_Second",temp);
	set(temp,nullstr);

	valstr(temp,Hay[hayid][hay_maxblocksperlevel]);
	db_Set(rdbname,"Max_Blocks_Per_Level",temp);
	set(temp,nullstr);

	valstr(temp,Hay[hayid][hay_minblocksperlevel]);
	db_Set(rdbname,"Min_Blocks_Per_Level",temp);
	set(temp,nullstr);

	format(temp,sizeof(temp),"%f,%f,%f,",HayStartCP[hayid][Coord_X],HayStartCP[hayid][Coord_Y],HayStartCP[hayid][Coord_Z]);
	db_Set(rdbname,"Start_CP",temp);
	set(temp,nullstr);

	db_AfterBigSaving(rdbname);

	new logstring[MAX_STRING];
	format(logstring, MAX_STRING, "Hay base saved: %s",Hay[hayid][hay_name]);
	WriteLog(logstring);

	Debug("hay.inc > HaySaveBaseDB - Stop");
}

HaySaveRecordDB(hayid)
{
	Debug("hay.inc > HaySaveRecordDB - Start");
	new temp[MAX_STRING];
	new rdbname[MAX_STRING];
	format(rdbname,sizeof(rdbname),"%s%s.txt",HayRecordDB,Hay[hayid][hay_name]);
	if (!db_Exists(rdbname)) db_Create(rdbname);
	db_BeforeBigSaving(rdbname);
	db_Set(rdbname,"Name",Hay[hayid][hay_name]);
	set(temp,nullstr);
	valstr(temp,HayBestScore[hayid][hay_score_level]);
	db_Set(rdbname,"Best_Score_Level",temp);
	set(temp,nullstr);
	set(temp,HayBestScore[hayid][hay_score_player]);
	db_Set(rdbname,"Best_Score_Player",temp);
	set(temp,nullstr);
	db_AfterBigSaving(rdbname);
	Debug("hay.inc > HaySaveRecordDB - Stop");
	return;
}

stock StopShowingHayInfo(playerid){
	TextDrawHideForPlayer(playerid,HAY_TextDraw);
}

stock ShowHayInfo(playerid){
	TextDrawShowForPlayer(playerid,HAY_TextDraw);
}

stock UpdateHayInfo(hayid){
	new string[MAX_STRING];
	format(string,MAX_STRING,"~r~Hay: ~n~~w~%s",Hay[hayid][hay_name]);
	TextDrawSetString(HAY_TextDraw,string);
}

stock PrepareHay(hayid){
    	HAY_ObjectCount[hayid] = 0;

	HAY_BlockPosX[hayid][0] = HayStartCP[hayid][Coord_X]-(2*HAY_BLOCKHIGHT);
	HAY_BlockPosY[hayid][0] = HayStartCP[hayid][Coord_Y]-(2*HAY_BLOCKHIGHT);
	HAY_BlockPosX[hayid][1] = HayStartCP[hayid][Coord_X]-(HAY_BLOCKHIGHT);
	HAY_BlockPosY[hayid][1] = HAY_BlockPosY[hayid][0];
	HAY_BlockPosX[hayid][2] = HayStartCP[hayid][Coord_X];
	HAY_BlockPosY[hayid][2] = HAY_BlockPosY[hayid][0];
	HAY_BlockPosX[hayid][3] = HayStartCP[hayid][Coord_X]+(HAY_BLOCKHIGHT);
	HAY_BlockPosY[hayid][3] = HAY_BlockPosY[hayid][0];
	HAY_BlockPosX[hayid][4] = HayStartCP[hayid][Coord_X]+(2*HAY_BLOCKHIGHT);
	HAY_BlockPosY[hayid][4] = HAY_BlockPosY[hayid][0];

	HAY_BlockPosX[hayid][5] = HAY_BlockPosX[hayid][0];
	HAY_BlockPosY[hayid][5] = HayStartCP[hayid][Coord_Y]-(HAY_BLOCKHIGHT);
	HAY_BlockPosX[hayid][6] = HAY_BlockPosX[hayid][1];
	HAY_BlockPosY[hayid][6] = HAY_BlockPosY[hayid][5];
	HAY_BlockPosX[hayid][7] = HAY_BlockPosX[hayid][2];
	HAY_BlockPosY[hayid][7] = HAY_BlockPosY[hayid][5];
	HAY_BlockPosX[hayid][8] = HAY_BlockPosX[hayid][3];
	HAY_BlockPosY[hayid][8] = HAY_BlockPosY[hayid][5];
	HAY_BlockPosX[hayid][9] = HAY_BlockPosX[hayid][4];
	HAY_BlockPosY[hayid][9] = HAY_BlockPosY[hayid][5];

	HAY_BlockPosX[hayid][10] = HAY_BlockPosX[hayid][0];
	HAY_BlockPosY[hayid][10] = HayStartCP[hayid][Coord_Y];
	HAY_BlockPosX[hayid][11] = HAY_BlockPosX[hayid][1];
	HAY_BlockPosY[hayid][11] = HAY_BlockPosY[hayid][10];
	HAY_BlockPosX[hayid][12] = HAY_BlockPosX[hayid][2];
	HAY_BlockPosY[hayid][12] = HAY_BlockPosY[hayid][10];
	HAY_BlockPosX[hayid][13] = HAY_BlockPosX[hayid][3];
	HAY_BlockPosY[hayid][13] = HAY_BlockPosY[hayid][10];
	HAY_BlockPosX[hayid][14] = HAY_BlockPosX[hayid][4];
	HAY_BlockPosY[hayid][14] = HAY_BlockPosY[hayid][10];

	HAY_BlockPosX[hayid][15] = HAY_BlockPosX[hayid][0];
	HAY_BlockPosY[hayid][15] = HayStartCP[hayid][Coord_Y]+(HAY_BLOCKHIGHT);
	HAY_BlockPosX[hayid][16] = HAY_BlockPosX[hayid][1];
	HAY_BlockPosY[hayid][16] = HAY_BlockPosY[hayid][15];
	HAY_BlockPosX[hayid][17] = HAY_BlockPosX[hayid][2];
	HAY_BlockPosY[hayid][17] = HAY_BlockPosY[hayid][15];
	HAY_BlockPosX[hayid][18] = HAY_BlockPosX[hayid][3];
	HAY_BlockPosY[hayid][18] = HAY_BlockPosY[hayid][15];
	HAY_BlockPosX[hayid][19] = HAY_BlockPosX[hayid][4];
	HAY_BlockPosY[hayid][19] = HAY_BlockPosY[hayid][15];

	HAY_BlockPosX[hayid][20] = HAY_BlockPosX[hayid][0];
	HAY_BlockPosY[hayid][20] = HayStartCP[hayid][Coord_Y]+(2*HAY_BLOCKHIGHT);
	HAY_BlockPosX[hayid][21] = HAY_BlockPosX[hayid][1];
	HAY_BlockPosY[hayid][21] = HAY_BlockPosY[hayid][20];
	HAY_BlockPosX[hayid][22] = HAY_BlockPosX[hayid][2];
	HAY_BlockPosY[hayid][22] = HAY_BlockPosY[hayid][20];
	HAY_BlockPosX[hayid][23] = HAY_BlockPosX[hayid][3];
	HAY_BlockPosY[hayid][23] = HAY_BlockPosY[hayid][20];
	HAY_BlockPosX[hayid][24] = HAY_BlockPosX[hayid][4];
	HAY_BlockPosY[hayid][24] = HAY_BlockPosY[hayid][20];

	for(new i=0; i<Hay[hayid][hay_levels]; i++){
	    AddHay(hayid,i);
	    AddHay(hayid,i);
	    AddHay(hayid,i);
	}
	for(new i=(Hay[hayid][hay_levels]*2); i<Hay[hayid][hay_objects]; i++){
	    AddRandomHay(hayid);
	}
}

stock AddRandomHay(hayid){
	new ran = random(Hay[hayid][hay_levels]);
	if(HAY_HayBlocks[hayid][ran] >= Hay[hayid][hay_maxblocksperlevel]) AddRandomHay(hayid);
	if(HAY_HayBlocks[hayid][ran] < Hay[hayid][hay_maxblocksperlevel]) AddHay(hayid,ran);
}

stock AddHay(hayid,level){
	new ran = GetRandomFreeHayPos(hayid,level);
	HAY_ObjectID[hayid][HAY_ObjectCount[hayid]] = CreateStreamObject(3374,HAY_BlockPosX[hayid][ran],HAY_BlockPosY[hayid][ran],(level*HAY_LEVELHIGHT)+HAY_LEVELHIGHT+HayStartCP[hayid][Coord_Z],0,0,0,WORLD_HAY);
//	HAY_ObjectID[hayid][HAY_ObjectCount[hayid]] = CreateStreamObject(3374,HAY_BlockPosX[hayid][ran],HAY_BlockPosY[hayid][ran],(level*HAY_LEVELHIGHT)+HAY_LEVELHIGHT,0,0,0,-1);
	HAY_BlockLevel[hayid][HAY_ObjectCount[hayid]] = level;
	HAY_BlockPos[hayid][HAY_ObjectCount[hayid]] = ran;
	HAY_BlockInUse[hayid][level][ran] = 1;
	HAY_HayBlocks[hayid][level]++;
	HAY_ObjectCount[hayid]++;
}

stock GetRandomFreeHayPos(hayid,level){
	new ran = random(25);
	if(HAY_BlockInUse[hayid][level][ran] == 0) return ran;
	return GetRandomFreeHayPos(hayid,level);
}

stock GetFreePlaceAroundHay(hayid,pos,level){
	new PossibilitiesToMove;
	new PossibilityToMove[6];
	if((level > 0) && (!HAY_BlockInUse[hayid][pos][(level-1)]) && (HAY_HayBlocks[hayid][(level-1)] < Hay[hayid][hay_maxblocksperlevel]) && (HAY_HayBlocks[hayid][level] > Hay[hayid][hay_minblocksperlevel])){
	    PossibilityToMove[PossibilitiesToMove] = 1;
	    PossibilitiesToMove++;
	}
	if((level < (Hay[hayid][hay_levels]-1)) && (!HAY_BlockInUse[hayid][pos][(level+1)]) && (HAY_HayBlocks[hayid][(level+1)] < Hay[hayid][hay_maxblocksperlevel]) && (HAY_HayBlocks[hayid][level] > Hay[hayid][hay_minblocksperlevel])){
	    PossibilityToMove[PossibilitiesToMove] = 2;
	    PossibilitiesToMove++;
	}
	if((pos < 20) && (!HAY_BlockInUse[hayid][(pos+5)][level])){
	    PossibilityToMove[PossibilitiesToMove] = 3;
	    PossibilitiesToMove++;
	}
	if((pos > 4) && (!HAY_BlockInUse[hayid][(pos-5)][level])){
	    PossibilityToMove[PossibilitiesToMove] = 4;
	    PossibilitiesToMove++;
	}
	if(((pos+5)%5 != 0) && (!HAY_BlockInUse[hayid][(pos-1)][level])){
	    PossibilityToMove[PossibilitiesToMove] = 5;
	    PossibilitiesToMove++;
	}
	if(((pos+1)%5 != 0) && (!HAY_BlockInUse[hayid][(pos+1)][level])){
	    PossibilityToMove[PossibilitiesToMove] = 6;
	    PossibilitiesToMove++;
	}
	new ran2 = random(PossibilitiesToMove);
	return (PossibilityToMove[ran2]-1);
}

stock MoveHay(hayid,hay_objectid){
	new ran;
	new level;
    	HAY_GetNewBlockAndLevel(hayid,HAY_BlockPos[hayid][hay_objectid],HAY_BlockLevel[hayid][hay_objectid],ran,level);
    	HAY_BlockInUse[hayid][level][ran] = 1;
	MoveStreamObject(HAY_ObjectID[hayid][hay_objectid],HAY_BlockPosX[hayid][ran],HAY_BlockPosY[hayid][ran],(float((level*HAY_LEVELHIGHT)+HAY_LEVELHIGHT)+HayStartCP[hayid][Coord_Z]),1.0);
	HAY_BlockInUse[hayid][HAY_BlockLevel[hayid][hay_objectid]][HAY_BlockPos[hayid][hay_objectid]] = 0;
	HAY_HayBlocks[hayid][HAY_BlockLevel[hayid][hay_objectid]]--;
	HAY_BlockPos[hayid][hay_objectid] = ran;
	HAY_BlockLevel[hayid][hay_objectid] = level;
	HAY_HayBlocks[hayid][level]++;
}

stock HAY_GetNewBlockAndLevel(hayid,pos,level,&newpos,&newlevel){
	newpos = pos;
	newlevel = level;
	new ran = GetFreePlaceAroundHay(hayid,pos,level);
	if(ran == 0) newlevel--;
	if(ran == 1) newlevel++;
	if(ran == 2) newpos+=5;
	if(ran == 3) newpos-=5;
	if(ran == 4) newpos--;
	if(ran == 5) newpos++;
}

public Hay_OnKeyStateChange(playerid, newkeys, oldkeys){
	if (IsPlayerInAnyHay(playerid)){
		if((newkeys == KEY_FIRE) || (newkeys == KEY_SECONDARY_ATTACK) || (newkeys == KEY_ACTION)){
        		SystemMsg(playerid,COLOUR_HAY_BAD,gettext(1684));
        		oSetPlayerHealth(playerid,oGetPlayerHealth(playerid)-10);
		}
	}
	return 0;
}

Hay_DoWhileActive(hayid){
	for(new playerid=0; playerid<MAX_PLAYERS_EX; playerid++){
	       	if((IsPlayerConnected(playerid))){
			if (IsPlayerInHay(playerid,hayid)){
				new level = trunc(((PlayerPos[playerid][Coord_Z]-HayStartCP[hayid][Coord_Z]+1)/HAY_LEVELHIGHT)-1);
				if(level > HayPlayerStats[playerid][hay_player_level]){
			    		HayPlayerStats[playerid][hay_player_level] = level;
			    		SystemMsgFormat(playerid,COLOUR_HAY,gettext(1685),level);
				}
				if(level >= (Hay[hayid][hay_levels])){
			    		for(new i=0; i<MAX_PLAYERS_EX; i++){
			        		if(i != playerid){
							if((IsPlayerConnected(i))){
								new hayid2 = GetPlayerHay(i);
						    		if (hayid2 == hayid) PlayerLeaveHay(playerid,hayid2);
							}
			        		}
			    		}
		        	}
			}
		}
	 }
	new ran;
	for(new i=0;i<Hay[hayid][hay_movespersecond];i++){
		ran = random(Hay[hayid][hay_objects]);
		MoveHay(hayid,ran);
	}
}

GetHayWinner(hayid)
{
	Debug("hay.inc > GetHayWinner - Start");
	new winnerid=INVALID_PLAYER_ID;
	new winnerlevel;
	for (new playerid=0; playerid<MAX_PLAYERS_EX;playerid++)
	{
		if (!IsPlayerConnected(playerid)) continue;
		if (PlayerQuest[playerid] == GetHayQuestID(hayid)) // if player is in this hay
		{
			if (HayPlayerStats[playerid][hay_player_level] > winnerlevel)
			{
				winnerid = playerid;
				winnerlevel = HayPlayerStats[playerid][hay_player_level];
			}
		}
	}
	Debug("hay.inc > GetHayWinner - Stop");
	return winnerid;
}