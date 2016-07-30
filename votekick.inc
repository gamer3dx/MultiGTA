//Votekick
//Created by Denwer/Astro (PEN1)
//Converted for MultiGTA by MadCat

forward Votekick_OnGameModeInit();
forward Votekick_OnPlayerConnect(playerid);
forward Votekick_OnPlayerDisconnect(playerid,reason);
forward Votekick_OnPlayerCommandText(playerid,text[]);

forward VoteKickTimer();
forward ClearVote (kickedid);

new KickVote[MAX_PLAYERS];
new VoteKick[MAX_PLAYERS];
new LastVote[MAX_PLAYERS];

new TimerVoteKick;

public Votekick_OnGameModeInit(){
	TimerVoteKick = SetTimer("VoteKickTimer", 1000, 1);
	WriteLog("VoteKick loaded");
	return 0;
}

public Votekick_OnPlayerConnect(playerid){
	KickVote[playerid] = 0;
	VoteKick[playerid] = 0;
	LastVote[playerid] = INVALID_PLAYER_ID;
	return 0;
}

public Votekick_OnPlayerDisconnect(playerid,reason){
	#pragma unused reason
	KickVote[playerid] = 0;
	VoteKick[playerid] = 0;
	LastVote[playerid] = INVALID_PLAYER_ID;
	return 0;
}

public Votekick_OnPlayerCommandText(playerid,text[])
{
	if (!IsPlayerRegistered(playerid)) return 0;
	dcmd(votekick,8,text);
	dcmd(vote,4,text);
	return 0;
}

public VoteKickTimer()
{
	for(new i = 0; i <= MAX_PLAYERS_EX; i++)
	{
		if(IsPlayerConnected(i) == 1 && VoteKick[i] > 0)
		{
			VoteKick[i]++;
			if(IsPlayerConnected(i) == 1 && VoteKick[i] == 20)
			{
				if(KickVote[i] >= PlayerCount()/4)
				{
					new string[MAX_STRING];
					format(string, MAX_STRING,gettext(1454),oGetPlayerName(i));
					SystemMsgToRegistered(COLOUR_INFO,string);
					ClearVote(i);
					KickPlayer(i,-1,gettext(1476));
				}
				else
				{
					new string[MAX_STRING];
					format(string, MAX_STRING,gettext(1455),oGetPlayerName(i));
					SystemMsgToRegistered(COLOUR_INFO,string);
					ClearVote(i);
				}
			}
		}
	}
}

public ClearVote(kickedid)
{
	KickVote[kickedid] = 0;
	VoteKick[kickedid] = 0;
	for(new j = 0; j <= MAX_PLAYERS_EX; j++)
	{
		if(IsPlayerConnected(j) == 1 && LastVote[j] == kickedid)
		{
			LastVote[j] = INVALID_PLAYER_ID;
		}
	}
}

dcmd_votekick(playerid,params[])
{
	if(isempty(params))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(1447));
		return 1;

	}

	new giveplayerid = FindPlayerID(params);
	if (giveplayerid==-1) return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1571),params);
	if (giveplayerid==-2) return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1572),params);
	if(IsPlayerAdmin(giveplayerid) || PlayerPriv[giveplayerid][IsModerator] == 1)
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(1448));
		return 1;
	}
	if (IsPlayerConnected(giveplayerid))
	{
		VoteKick[giveplayerid] = 1;
		KickVote[giveplayerid] = 1;
		new string[MAX_STRING];
		format(string, MAX_STRING, gettext(1449),oGetPlayerName(playerid), oGetPlayerName(giveplayerid),giveplayerid);
		PlayerPlaySoundOnPlayer(playerid, 1052);
		SystemMsgToRegistered(COLOUR_INFO,string);
	}
	else
	{
			SystemMsg(playerid,COLOUR_ERROR,gettext(32));
	}
	return 1;
}

dcmd_vote(playerid,params[])
{
	if(isempty(params))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(1450));
		return 1;
	}

	new giveplayerid = FindPlayerID(params);
	if (giveplayerid==-1) return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1571),params);
	if (giveplayerid==-2) return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1572),params);

	if (VoteKick[giveplayerid] == 0)
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(1451));
		return 1;
	}
	if (LastVote[playerid] == giveplayerid)
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(1452));
		return 1;
	}
	if (IsPlayerConnected(giveplayerid))
	{
		KickVote[giveplayerid]++;
		LastVote[playerid] = giveplayerid;
		SystemMsg(playerid,COLOUR_OK,gettext(1453));
	}
	else
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(32));
	}
	return 1;
}