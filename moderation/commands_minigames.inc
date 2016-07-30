//Minigames Moderator Commands Module
//Created by MadCat

#include "base"
//#include "hotwire"
#include "cashbox"

forward ModMG_OnPlayerCommandText(playerid,text[]) ;

public ModMG_OnPlayerCommandText(playerid,text[])
{
	if (!IsPlayerConnected(playerid)) return 0;
	if (PlayerPriv[playerid][IsModerator] == 0) return 0;

	new cmd[20];
	new idx;
	set(cmd,strcharsplit(text, idx,strchar(" ")));

	if(strcomp(cmd, "/start", true) == 1)
	{
	set(cmd,strcharsplit(text, idx,strchar(" ")));

//	if(strcomp(cmd, "hotwire", true) == 1)
//	{
//		if (PlayerPriv[playerid][CanStartHotwire] == 0) return 0;
//		Debug("moderation/commands_minigames.inc > Command 'start hotwire' - Start");
//		if (event_now == 1)
//		{
//			ModMsg(playerid,gettext(1114));
//			Debug("moderation/commands_minigames.inc > Command 'start hotwire' - Stop");
//			return 1;
//		}
//		StartHotWire();
//
//		new logstring[256];
//		format(logstring, sizeof (logstring), "Moderator %s start hotwire.",oGetPlayerName(playerid));
//		WriteModerLog(logstring);
//		Debug("moderation/commands_minigames.inc > Command 'start hotwire' - Stop");
//		return 1;
//	}

	if(strcomp(cmd, "cashbox", true) == 1)
	{
		if (PlayerPriv[playerid][CanStartCashBox] == 0) return 0;
		Debug("moderation/commands_minigames.inc > Command 'start cashbox' - Start");
		if (event_now == true)
		{
			ModMsg(playerid,gettext(1114));
			Debug("moderation/commands_minigames.inc > Command 'start cashbox' - Stop");
			return 1;
		}
		StartCashBox();

		new logstring[256];
		format(logstring, sizeof (logstring), "Moderator %s start cashbox.",oGetPlayerName(playerid));
		WriteModerLog(logstring);
		Debug("moderation/commands_minigames.inc > Command 'hotwire start' - Stop");
		return 1;
	}

return 1;}

return 0;
}

