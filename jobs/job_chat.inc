//Job chat. Simple. No comments.

#include "base"

forward JobChat_OnPlayerText(playerid, text[]);

public JobChat_OnPlayerText(playerid, text[]) {
	if(text[0]=='@') {
		new string[MAX_STRING], chat[MAX_STRING];
		if(Player[playerid][GotJob] == JOB_COP) {
			strmid(chat,text,1,strlen(text));			
			format(string,sizeof(string),gettext(584),oGetPlayerName(playerid),chat);
			SendMsgToCops(string);
			WriteChatLog(playerid,text,"job");
			return 1;
		} else if(Player[playerid][GotJob] == JOB_MEDIC) {
			strmid(chat,text,1,strlen(text));			
			format(string,sizeof(string),gettext(654),oGetPlayerName(playerid),chat);
			SendMsgToMedics(string);
			WriteChatLog(playerid,text,"job");
			return 1;
		} else if(Player[playerid][GotJob] == JOB_TAXIDRIVER) {
			strmid(chat,text,1,strlen(text));			
			format(string,sizeof(string),gettext(279),oGetPlayerName(playerid),chat);
			SendMsgToTaxi(string);
			WriteChatLog(playerid,text,"job");
			return 1;
		}
		return 0;
	}
	return 0;
}