//Bribe for cops module
//Created by Zamaroht and jtg91 (in Compton City RPG)
//Convert for MultiGTA by MadCat

#include "base"

forward Bribe_OnPlayerConnect(playerid);
forward Bribe_OnPlayerDisconnect(playerid,reason);
forward Bribe_OnPlayerDeath(playerid,killerid,reason);
forward Bribe_OnPlayerCommandText(playerid,text[]);

new Bribe[MAX_PLAYERS];
new BribeTo[MAX_PLAYERS] = INVALID_PLAYER_ID;
new BribeFrom[MAX_PLAYERS] = INVALID_PLAYER_ID;

public Bribe_OnPlayerConnect(playerid)
{
	BribeTo[playerid] = INVALID_PLAYER_ID;
	Bribe[playerid] = 0;
	BribeFrom[playerid] = INVALID_PLAYER_ID;
	return 0;
}

public Bribe_OnPlayerDisconnect(playerid,reason)
{
	#pragma unused reason
	BribeTo[playerid] = INVALID_PLAYER_ID;
	Bribe[playerid] = 0;
	BribeFrom[playerid] = INVALID_PLAYER_ID;
	return 0;
}

public Bribe_OnPlayerDeath(playerid,killerid,reason)
{
	if (BribeTo[playerid] != INVALID_PLAYER_ID){
		Bribe[BribeTo[playerid]] = 0;
		BribeTo[playerid] = INVALID_PLAYER_ID;
		BribeFrom[BribeTo[playerid]] = INVALID_PLAYER_ID;
	}
	return 0;
}

public Bribe_OnPlayerCommandText(playerid,text[])
{
	if (!IsPlayerConnected(playerid)) return 0;
 	dcmd(bribecancel,11,text);
	dcmd(bribeacc,8,text);
	dcmd(bribedec,8,text);
	dcmd(bribe,5,text);
	return 0;
}

dcmd_bribecancel(playerid,params[])
{
	#pragma unused params
	if(BribeTo[playerid] == INVALID_PLAYER_ID)
	{
		SystemMsg(playerid, COLOUR_ERROR, gettext(1495));
	} else {
		SystemMsg(BribeTo[playerid], COLOUR_INFO,gettext(1497));
		Bribe[BribeTo[playerid]] = 0;
		BribeFrom[BribeTo[playerid]] = INVALID_PLAYER_ID;
		BribeTo[playerid] = INVALID_PLAYER_ID;
		SystemMsg(playerid, COLOUR_INFO, gettext(1497));
	}
	return 1;
}

dcmd_bribeacc(playerid,params[])
{
	#pragma unused params
	if(Player[playerid][GotJob] == JOB_COP)
		{
		if(Bribe[playerid] == 0)
		{
			SystemMsg(playerid, COLOUR_ERROR, gettext(1496));
		} else {
			if(Player[BribeFrom[playerid]][Crimes] > 0)
			{
				oGivePlayerMoney(playerid,Bribe[playerid],false);
				oGivePlayerMoney(BribeFrom[playerid], -Bribe[playerid],false);
				SystemMsg(BribeFrom[playerid], COLOUR_GREEN, gettext(1498));
				SystemMsg(playerid, COLOUR_ERROR, gettext(1499));
				Player[BribeFrom[playerid]][Crimes] = 0;
				CheckCurrentCrimes(BribeFrom[playerid]);
				BribeTo[BribeFrom[playerid]] = INVALID_PLAYER_ID;
				Bribe[playerid] = 0;
				BribeFrom[playerid] = INVALID_PLAYER_ID;
			} else {
				SystemMsg(playerid, COLOUR_ERROR, gettext(1500));
				BribeTo[BribeFrom[playerid]] = INVALID_PLAYER_ID;
				Bribe[playerid] = 0;
				BribeFrom[playerid] = INVALID_PLAYER_ID;
			}
		}
	} else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(566));
	}
	return 1;
}

dcmd_bribedec(playerid,params[])
{
	#pragma unused params
	if(Player[playerid][GotJob] == JOB_COP)
	{
		if(BribeFrom[playerid] == INVALID_PLAYER_ID)
		{
			SystemMsg(playerid, COLOUR_ERROR, gettext(1496));
		} else {
			SystemMsg(BribeFrom[playerid], COLOUR_ERROR, gettext(1501));
			SystemMsg(playerid, COLOUR_GREEN, gettext(1502));
			SetPlayerCriminal(BribeFrom[playerid],gettext(1503),10);
			BribeTo[BribeFrom[playerid]] = INVALID_PLAYER_ID;
			Bribe[playerid] = 0;
			BribeFrom[playerid] = INVALID_PLAYER_ID;
		}
	} else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(566));
	}
	return 1;
}

dcmd_bribe(playerid, params[])
{
	if (Player[playerid][GotJob] == JOB_COP) return SystemMsg(playerid, COLOUR_ERROR, gettext(1504));
	new giveplayerid, amount,idx, tmp[MAX_STRING];
	tmp = strcharsplit(params, idx,strchar(" "));
	if (isempty(tmp)){
		SystemMsg(playerid, COLOUR_ERROR, gettext(1505)); 
		return 1;
	}
	giveplayerid = FindPlayerID(tmp);
	if (giveplayerid==-1) return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1571),tmp); 
	if (giveplayerid==-2) return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1572),tmp); 

	tmp = strcharsplit(params, idx,strchar(" "));
	if (isempty(tmp)){
		SystemMsg(playerid, COLOUR_ERROR, gettext(1505)); 
		return 1;
	}
	amount = strval(tmp);
	if (!IsPlayerConnected(giveplayerid)) return SystemMsg(playerid, COLOUR_ERROR,gettext(395));
	if (Player[giveplayerid][GotJob] != JOB_COP) return SystemMsg(playerid,COLOUR_ERROR, gettext(1506));
	if (Player[playerid][Crimes] == 0) return SystemMsg(playerid, COLOUR_ERROR, gettext(1507));
	if (playerid == giveplayerid) return SystemMsg(playerid, COLOUR_ERROR, gettext(1508));
	if (BribeTo[playerid]!=INVALID_PLAYER_ID) return SystemMsg(playerid, COLOUR_ERROR, gettext(1509));
	if (amount < 1) return SystemMsg(playerid, COLOUR_ERROR, gettext(1510));
	if (amount > oGetPlayerMoney(playerid)) return SystemMsg(playerid, COLOUR_ERROR,gettext(868));
	if (BribeFrom[giveplayerid] != INVALID_PLAYER_ID) return SystemMsg(playerid, COLOUR_ERROR, gettext(1511));
	
	BribeTo[playerid] = giveplayerid;
	BribeFrom[giveplayerid] = playerid;
	Bribe[giveplayerid] = amount;
	format(tmp,MAX_STRING,gettext(1512),oGetPlayerName(playerid), amount);
	SystemMsg(giveplayerid, COLOUR_INFO, tmp);
	SystemMsg(giveplayerid, COLOUR_INFO, gettext(1513));
	SystemMsg(playerid, COLOUR_INFO, gettext(1514));
	return 1;
}