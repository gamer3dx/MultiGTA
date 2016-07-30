//Chatsystem module
//Created by: MadCat

#include "base"

forward Chat_OnPlayerCommandText(playerid,text[]);

#define COLOUR_EMOTE 0xFF981FAA

public Chat_OnPlayerCommandText(playerid,text[])
{
	if (!IsPlayerRegistered(playerid)) return 0;
	dcmd(me,2,text);
	return 0;
}

dcmd_me(playerid,params[])
{
	Debug("chatsystem.inc > Command 'me' - Start");
   	new i;
   	new tmp[MAX_STRING];

  	set(tmp,oGetPlayerName(playerid));
   	strcat(tmp," ");
   	strcat(tmp,params);

   	for (i=0;i<MAX_PLAYERS_EX;i++) {
	   	if (IsPlayerConnected(i)) {
			if (Real_Chat == 1){
				if (IsPlayerInRangeOfPoint(i,RADIUS_SAY,PlayerPos[playerid][Coord_X],PlayerPos[playerid][Coord_Y],PlayerPos[playerid][Coord_Z])){
   					SystemMsg(i,COLOUR_EMOTE,tmp);
				}
			} else {
				SystemMsg(i,COLOUR_EMOTE,tmp);
			}
	   	}
   	}
   	WriteChatLog(playerid,params,"emote");
       	Debug("chatsystem.inc > Command 'me' - Stop");
       	return 1;
}