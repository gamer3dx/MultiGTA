//Deathmatch Moderator Commands Module
//Created by MadCat

#include "base"
#include "deathmatch"

forward ModDM_OnPlayerCommandText(playerid,text[]);

public ModDM_OnPlayerCommandText(playerid,text[])
{
	if (!IsPlayerConnected(playerid)) return 0;
	if (PlayerPriv[playerid][IsModerator] == 0) return 0;
	

	new cmd[20];
	new idx;
	set(cmd,strcharsplit(text, idx,strchar(" ")));

	if(strcomp(cmd, "/dm", true) == 1)
	{
		set(cmd,strcharsplit(text, idx,strchar(" ")));

		if(strcomp(cmd, "start", true) == 1)
		{
			Debug("moderation/commands_dm.inc > Command 'dm start' - Start");

			new dmid;
			dmid = strval(strcharsplit(text, idx,strchar(" ")));

			if ((dmid == 0) || (dmid > DMCount))
			{
				new string[256];
				format(string,sizeof(string),gettext(1130), DMCount+1);
				ModMsg(playerid,string);
				Debug("moderation/commands_dm.inc > Command 'dm start' - Stop");
				return 1;
			}
			if (DeathmatchStats[dmid][dm_state] == DM_STATE_DISABLED)
			{
				ModMsg(playerid,gettext(1131));
				Debug("moderation/commands_dm.inc > Command 'dm start' - Stop");
				return 1;
			}
			if (DeathmatchStats[dmid][dm_state] == DM_STATE_ACTIVE)
			{
				ModMsg(playerid,gettext(1132));
				Debug("moderation/commands_dm.inc > Command 'dm start' - Stop");
				return 1;
			}
			if (event_now == true)
			{
				ModMsg(playerid,gettext(1114));
				Debug("moderation/commands_dm.inc > Command 'dm start' - Stop");
				return 1;
			}
			if (DeathmatchStats[dmid][dm_state] == DM_STATE_LINEUP)
			{
				DeathmatchStats[dmid][dm_timer] = 0;
				StartDMCountdown(dmid);
				new logstring[256];
				format(logstring, sizeof (logstring), "Moderator %s start dm countdown. DM ID: %d",oGetPlayerName(playerid),dmid);
				WriteModerLog(logstring);
				Debug("moderation/commands_dm.inc > Command 'dm start' - Stop");
				return 1;
			}
			if (DeathmatchStats[dmid][dm_state] == DM_STATE_SLEEPING)
			{
				DeathmatchStats[dmid][dm_timer] = 0;
				StartDMLineup(dmid);
				new logstring[256];
				format(logstring, sizeof (logstring), "Moderator %s start dm lineup. DM ID: %d",oGetPlayerName(playerid),dmid);
				WriteModerLog(logstring);
				Debug("moderation/commands_dm.inc > Command 'dm start' - Stop");
				return 1;
			}
			return 1;
		}

		if(strcomp(cmd, "list", true) == 1)
		{
			Debug("moderation/commands_dm.inc > Command 'dm list' - Start");
			new string[MAX_STRING];
			SystemMsgScrolling(playerid,gettext(1006));
			for (new dmid=1; dmid<=DMCount;dmid++)
			{
				if (DeathmatchStats[dmid][dm_state] != DM_STATE_DISABLED)
				{
					format(string, sizeof(string),gettext(1009),Deathmatch[dmid][dm_name],dmid,Deathmatch[dmid][dm_minplayers],GetDMStartZoneName(dmid),Deathmatch[dmid][dm_minlevel]);
					SystemMsgScrolling(playerid,string);
				}
			}
			Debug("moderation/commands_dm.inc > Command 'dm list' - Stop");
			return 1;
		}

		if(strcomp(cmd, "find", true) == 1)
		{
			Debug("moderation/commands_dm.inc > Command 'dm find' - Start");
			new string[MAX_STRING];
			new name[MAX_STRING];
			name = strcharsplit(text, idx,strchar(" "));
			if (isempty(name)){ ModMsg(playerid,gettext(1673)); Debug("moderation/commands_dm.inc > Command 'dm find' - Stop"); return 1;}
			new count = 0;
			for (new dmid=1; dmid<=DMCount;dmid++)
			{
				if (strfind(Deathmatch[dmid][dm_name],name) != -1)
				{
					format(string, sizeof(string),gettext(1009),Deathmatch[dmid][dm_name],dmid,Deathmatch[dmid][dm_minplayers],GetDMStartZoneName(dmid),Deathmatch[dmid][dm_minlevel]);
					SystemMsgScrolling(playerid,string);
					count++;
				}
			}
			if (count == 0) ModMsg(playerid,gettext(1674));
			Debug("moderation/commands_dm.inc > Command 'dm find' - Stop");
			return 1;
		}

	}
	return 0;
}