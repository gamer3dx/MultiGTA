//Race Moderator Commands Module
//Created by Iain Gilbert

#include "base"
#include "race"

forward ModRace_OnPlayerCommandText(playerid,text[]);

new PlayerRaceEdit[MAX_PLAYERS]=INVALID_RACE_ID; // will hold id of race a player is currently creating

public ModRace_OnPlayerCommandText(playerid,text[])
{
	if (!IsPlayerConnected(playerid)) return 0;
	if (PlayerPriv[playerid][IsModerator] == 0) return 0;

	new cmd[20];
	new idx;
	set(cmd,strcharsplit(text, idx,strchar(" ")));

	if(strcomp(cmd, "/race", true) == 1)
	{
		set(cmd,strcharsplit(text, idx,strchar(" ")));

		if(strcomp(cmd, "create", true) == 1)
		{
			if (PlayerPriv[playerid][CanEditRace] == 0) return 0;

			Debug("moderation/commands_race.inc > Command 'race create' - Start");

			new racename[MAX_NAME];
			set(racename,text);
			strdel(racename, 0, idx);
			if (strlen(racename) <3)
			{
				ModMsg(playerid,gettext(1133));
				Debug("moderation/commands_race.inc > Command 'race create' - Stop");
				return 1;
			}
			if (strlen(racename) > MAX_NAME)
			{
				new string[256];
				format(string,sizeof(string),gettext(1134), MAX_NAME);
				ModMsg(playerid,string);
				Debug("moderation/commands_race.inc > Command 'race create' - Stop");
				return 1;
			}
			new tempname[MAX_STRING];
			set(tempname,racename);
			if (!StringCharCheck(tempname))
			{
				ModMsg(playerid,gettext(1135));
				Debug("moderation/commands_race.inc > Command 'race create' - Stop");
				return 1;
			}

			new raceid = RegisterRace(racename);
			if (raceid == INVALID_RACE_ID)
			{
				ModMsg(playerid,gettext(1136));
				Debug("moderation/commands_race.inc > Command 'race create' - Stop");
				return 1;
			}

			// ok we good to go

			PlayerRaceEdit[playerid] = raceid;
			new Float:angle;
			if (IsPlayerInAnyVehicle(playerid)){
				GetVehicleZAngle(GetPlayerVehicleID(playerid),angle);
				Race[raceid][race_startheading] = angle;
			} else {
				GetPlayerFacingAngle(playerid,angle);
				Race[raceid][race_startheading] = angle;
			}
			RaceSize[raceid] = 0;
			//StartHeading[raceid] = GetPlayerHeading(playerid);
			AddRaceCP(raceid,PlayerPos[playerid][Coord_X],PlayerPos[playerid][Coord_Y],PlayerPos[playerid][Coord_Z]);
			ModMsg(playerid,gettext(1137));
			ModMsg(playerid,gettext(1138));
			ModMsg(playerid,gettext(1139));
			ModMsg(playerid,gettext(1140));

			new logstring[256];
			format(logstring, sizeof (logstring), "Moderator %s start creation of race %s (ID: %d)",oGetPlayerName(playerid),racename,raceid);
			WriteModerLog(logstring);
			Debug("moderation/commands_race.inc > Command 'race create' - Stop");
			return 1;
		}

		if(strcomp(cmd, "cp", true) == 1)
		{
			if (PlayerPriv[playerid][CanEditRace] == 0) return 0;
			Debug("moderation/commands_race.inc > Command 'race cp' - Start");
			if (PlayerRaceEdit[playerid] == INVALID_RACE_ID)
			{
				ModMsg(playerid,gettext(1141));
				Debug("moderation/commands_race.inc > Command 'race cp' - Stop");
				return 1;
			}
	
			new raceid = PlayerRaceEdit[playerid];
			if (RaceStats[raceid][race_state] != RACE_STATE_DISABLED)
			{
				ModMsg(playerid,gettext(1142));
				Debug("moderation/commands_race.inc > Command 'race cp' - Stop");
				return 1;
			}
	
			// ok, all good
			AddRaceCP(raceid,PlayerPos[playerid][Coord_X],PlayerPos[playerid][Coord_Y],PlayerPos[playerid][Coord_Z]);
			ModMsg(playerid,gettext(1143));
	
			new logstring[256];
			format(logstring, sizeof (logstring), "Moderator %s add checkpoint to race (ID: %d)",oGetPlayerName(playerid),raceid);
			WriteModerLog(logstring);
			Debug("moderation/commands_race.inc > Command 'race cp' - Stop");
			return 1;
		}

		if(strcomp(cmd, "cpremove", true) == 1)
		{
			Debug("moderation/commands_race.inc > Command 'race cpremove' - Start");
			if (PlayerPriv[playerid][CanEditRace] == 0) return 0;
			if (PlayerRaceEdit[playerid] == INVALID_RACE_ID)
			{
				ModMsg(playerid,gettext(1141));
				Debug("moderation/commands_race.inc > Command 'race cpremove' - Stop");
				return 1;
			}
			new raceid = PlayerRaceEdit[playerid];
			if (RaceStats[raceid][race_state] != RACE_STATE_DISABLED)
			{
				ModMsg(playerid,gettext(1142));
				Debug("moderation/commands_race.inc > Command 'race cpremove' - Stop");
				return 1;
			}
	
			// ok, all good
			RemoveLastRaceCP(raceid);
			ModMsg(playerid,gettext(1144));
	
			new logstring[256];
			format(logstring, sizeof (logstring), "Moderator %s remove last checkpoint in race (ID: %d)",oGetPlayerName(playerid),raceid);
			WriteModerLog(logstring);
			Debug("moderation/commands_race.inc > Command 'race cpremove' - Stop");
			return 1;
		}

		if(strcomp(cmd, "save", true) == 1)
		{
			if (PlayerPriv[playerid][CanEditRace] == 0) return 0;
			Debug("moderation/commands_race.inc > Command 'race save' - Start");
			if (PlayerRaceEdit[playerid] == INVALID_RACE_ID)
			{
				ModMsg(playerid,gettext(1141));
				Debug("moderation/commands_race.inc > Command 'race save' - Stop");
				return 1;
			}
			new raceid = PlayerRaceEdit[playerid];
			if (RaceStats[raceid][race_state] != RACE_STATE_DISABLED)
			{
				ModMsg(playerid,gettext(1142));
				Debug("moderation/commands_race.inc > Command 'race save' - Stop");
				return 1;
			}
	
			// ok, all good
	
			// set default values
			Race[raceid][race_frequency] = 5;
			Race[raceid][race_lineupdelay] = 240;
			Race[raceid][race_minracers] = 3;
			Race[raceid][race_cashprize] = CalculateRewardMoney(raceid);
			Race[raceid][race_cashentry] = CalculateEntryFee(raceid);
			Race[raceid][race_xpprize] = CalculateRewardXP(raceid);
			Race[raceid][race_maxracetime] = CalculateMaxRaceTime(raceid);
			RaceStats[raceid][race_state] = RACE_STATE_SLEEPING;
			RaceSaveBaseDB(raceid);
			PlayerRaceEdit[playerid] = INVALID_RACE_ID;
			RaceStats[raceid][race_timer] = 5;
	
			ModMsg(playerid,gettext(1145));
	
			new logstring[256];
			format(logstring, sizeof (logstring), "Moderator %s save race (ID: %d)",oGetPlayerName(playerid),raceid);
			WriteModerLog(logstring);
			Debug("moderation/commands_race.inc > Command 'race save' - Stop");
			return 1;
		}

		if(strcomp(cmd, "start", true) == 1)
		{
			Debug("moderation/commands_race.inc > Command 'race start' - Start");
			if (PlayerPriv[playerid][CanStartRace] == 0) return 0;
			new raceid;
			raceid = strval(strcharsplit(text, idx,strchar(" ")));
	
			if ((raceid == 0) || (raceid > RacesCount))
			{
				new string[256];
				format(string,sizeof(string),gettext(1146),RacesCount+1);
				ModMsg(playerid,string);
				Debug("moderation/commands_race.inc > Command 'race start' - Stop");
				return 1;
			}
			if (RaceStats[raceid][race_state] == RACE_STATE_DISABLED)
			{
				ModMsg(playerid,gettext(1147));
				Debug("moderation/commands_race.inc > Command 'race start' - Stop");
				return 1;
			}
			if (RaceStats[raceid][race_state] == RACE_STATE_RACING)
			{
				ModMsg(playerid,gettext(1148));
				Debug("moderation/commands_race.inc > Command 'race start' - Stop");
				return 1;
			}
			if (event_now == true)
			{
				ModMsg(playerid,gettext(1114));
				Debug("moderation/commands_race.inc > Command 'race start' - Stop");
				return 1;
			}
			if (IsServerGotVehiclesForRace(raceid) == 0)
			{
				ModMsg(playerid,gettext(538));
				Debug("moderation/commands_race.inc > Command 'race start' - Stop");
				return 1;
			}
			if (RaceStats[raceid][race_state] == RACE_STATE_LINEUP)
			{
				RaceStats[raceid][race_timer] = 0;
				StartRaceCountdown(raceid);
				Debug("moderation/commands_race.inc > Command 'race start' - Stop");
				return 1;
			}
			if (RaceStats[raceid][race_state] == RACE_STATE_SLEEPING)
			{		
				RaceStats[raceid][race_timer] = 0;
				StartRaceLineup(raceid);

				new logstring[256];
				format(logstring, sizeof (logstring), "Moderator %s start race (ID: %d)",oGetPlayerName(playerid),raceid);
				WriteModerLog(logstring);
				Debug("moderation/commands_race.inc > Command 'race start' - Stop");
				return 1;
			}
			return 1;
		}

		if(strcomp(cmd, "list", true) == 1)
		{
			Debug("moderation/commands_race.inc > Command 'race list' - Start");
			if (PlayerPriv[playerid][CanStartRace] == 0) {Debug("moderation/commands_race.inc > Command 'race list' - Stop"); return 0;}
			new string[MAX_STRING];
			SystemMsgScrolling(playerid,gettext(535));
			for (new raceid=1; raceid<=RacesCount;raceid++)
			{
				if (RaceStats[raceid][race_state] != RACE_STATE_DISABLED)
				{
					format(string, sizeof(string),gettext(536),Race[raceid][race_name],raceid,RaceStats[raceid][race_racercount],GetRaceCPZoneName(raceid,0),FormatTimeForEvent(Race[raceid][race_maxracetime]));
					SystemMsgScrolling(playerid, string);
				}
			}
			Debug("moderation/commands_race.inc > Command 'race list' - Stop");
			return 1;
		}

		if(strcomp(cmd, "find", true) == 1)
		{
			Debug("moderation/commands_race.inc > Command 'race find' - Start");
			if (PlayerPriv[playerid][CanStartRace] == 0) {Debug("moderation/commands_race.inc > Command 'race find' - Stop"); return 0;}
			new string[MAX_STRING];
			new name[MAX_STRING];
			name = strcharsplit(text, idx,strchar(" "));
			if (isempty(name)){ModMsg(playerid,gettext(1672)); Debug("moderation/commands_race.inc > Command 'race find' - Stop"); return 1;}
			new count = 0;
			for (new raceid=1; raceid<=RacesCount;raceid++)
			{
				if (strfind(Race[raceid][race_name],name) != -1)
				{
					format(string, sizeof(string),gettext(536),Race[raceid][race_name],raceid,RaceStats[raceid][race_racercount],GetRaceCPZoneName(raceid,0),FormatTimeForEvent(Race[raceid][race_maxracetime]));
					SystemMsgScrolling(playerid, string);
					count++;
				}
			}
			if (count == 0) ModMsg(playerid,gettext(1670));
			Debug("moderation/commands_race.inc > Command 'race find' - Stop");
			return 1;
		}
	}
	return 0;
}





