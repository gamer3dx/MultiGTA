//Hay Moderator Commands Module
//Created by MadCat

#include "base"
#include "hay"

forward ModHay_OnPlayerCommandText(playerid,text[]);

public ModHay_OnPlayerCommandText(playerid,text[])
{
	if (!IsPlayerConnected(playerid)) return 0;
	if (PlayerPriv[playerid][IsModerator] == 0) return 0;
	

	new cmd[20];
	new idx;
	set(cmd,strcharsplit(text, idx,strchar(" ")));

	if(strcomp(cmd, "/hay", true) == 1)
	{
		set(cmd,strcharsplit(text, idx,strchar(" ")));

		if(strcomp(cmd, "start", true) == 1)
		{
			Debug("moderation/commands_hay.inc > Command 'hay start' - Start");

			new hayid;
			hayid = strval(strcharsplit(text, idx,strchar(" ")));

			if ((hayid == 0) || (hayid > HayCount))
			{
				new string[256];
				format(string,sizeof(string),gettext(1713), HayCount+1);
				ModMsg(playerid,string);
				Debug("moderation/commands_hay.inc > Command 'hay start' - Stop");
				return 1;
			}
			if (HayStats[hayid][hay_state] == HAY_STATE_DISABLED)
			{
				ModMsg(playerid,gettext(1714));
				Debug("moderation/commands_hay.inc > Command 'hay start' - Stop");
				return 1;
			}
			if (HayStats[hayid][hay_state] == HAY_STATE_ACTIVE)
			{
				ModMsg(playerid,gettext(1715));
				Debug("moderation/commands_hay.inc > Command 'hay start' - Stop");
				return 1;
			}
			if (event_now == true)
			{
				ModMsg(playerid,gettext(1716));
				Debug("moderation/commands_hay.inc > Command 'hay start' - Stop");
				return 1;
			}
			if (HayStats[hayid][hay_state] == HAY_STATE_LINEUP)
			{
				HayStats[hayid][hay_timer] = 0;
				StartHayCountdown(hayid);
				new logstring[256];
				format(logstring, sizeof (logstring), "Moderator %s start hay countdown. Hay ID: %d",oGetPlayerName(playerid),hayid);
				WriteModerLog(logstring);
				Debug("moderation/commands_hay.inc > Command 'hay start' - Stop");
				return 1;
			}
			if (HayStats[hayid][hay_state] == HAY_STATE_SLEEPING)
			{
				HayStats[hayid][hay_timer] = 0;
				StartHayLineup(hayid);
				new logstring[256];
				format(logstring, sizeof (logstring), "Moderator %s start hay lineup. Hay ID: %d",oGetPlayerName(playerid),hayid);
				WriteModerLog(logstring);
				Debug("moderation/commands_hay.inc > Command 'hay start' - Stop");
				return 1;
			}
			return 1;
		}

		if(strcomp(cmd, "list", true) == 1)
		{
			Debug("moderation/commands_hay.inc > Command 'hay list' - Start");
			new string[MAX_STRING];
			SystemMsgScrolling(playerid,gettext(1717));
			for (new hayid=1; hayid<=HayCount;hayid++)
			{
				if (HayStats[hayid][hay_state] != HAY_STATE_DISABLED)
				{
					format(string, sizeof(string),gettext(1718),Hay[hayid][hay_name],hayid,Hay[hayid][hay_minplayers],GetHayStartZoneName(hayid),Hay[hayid][hay_minlevel],Hay[hayid][hay_levels],Hay[hayid][hay_objects]);
					SystemMsgScrolling(playerid,string);
				}
			}
			Debug("moderation/commands_hay.inc > Command 'hay list' - Stop");
			return 1;
		}

		if(strcomp(cmd, "find", true) == 1)
		{
			Debug("moderation/commands_hay.inc > Command 'hay find' - Start");
			new string[MAX_STRING];
			new name[MAX_STRING];
			name = strcharsplit(text, idx,strchar(" "));
			if (isempty(name)){ ModMsg(playerid,gettext(1719)); Debug("moderation/commands_hay.inc > Command 'hay find' - Stop"); return 1;}
			new count = 0;
			for (new hayid=1; hayid<=HayCount;hayid++)
			{
				if (strfind(Hay[hayid][hay_name],name) != -1)
				{
					format(string, sizeof(string),gettext(1718),Hay[hayid][hay_name],hayid,Hay[hayid][hay_minplayers],GetHayStartZoneName(hayid),Hay[hayid][hay_minlevel],Hay[hayid][hay_levels],Hay[hayid][hay_objects]);
					SystemMsgScrolling(playerid,string);
					count++;
				}
			}
			if (count == 0) ModMsg(playerid,gettext(1720));
			Debug("moderation/commands_hay.inc > Command 'hay find' - Stop");
			return 1;
		}

	}
	return 0;
}