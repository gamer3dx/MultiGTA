//DM and Race bets module
//Created by Denver (Astro) (PEN1 Gamemode)
//Convert for MultiGTA by MadCat

#include "base"

forward BetWinner (playerid,contest);

forward Bets_OnPlayerConnect(playerid);
forward Bets_OnPlayerDisconnect(playerid,reason);
forward Bets_OnPlayerCommandText(playerid,text[]);

new BetRace[MAX_PLAYERS][2];
new BetDM[MAX_PLAYERS][2];
new BetHay[MAX_PLAYERS][2];

#define MAX_DM_BET 100000
#define MAX_RACE_BET 100000
#define MAX_HAY_BET 100000

public Bets_OnPlayerConnect(playerid)
{
	BetRace[playerid][0] = INVALID_PLAYER_ID;
	BetRace[playerid][1] = 0;
	BetDM[playerid][0] = INVALID_PLAYER_ID;
	BetDM[playerid][1] = 0;
	BetHay[playerid][0] = INVALID_PLAYER_ID;
	BetHay[playerid][1] = 0;
	return 0;
}

public Bets_OnPlayerDisconnect(playerid,reason){
	if (BetRace[playerid][1] > 0){
		for (new i=0;i<MAX_PLAYERS_EX;i++)
		{
			if (IsPlayerConnected(i)){
				if(BetRace[i][0] == playerid)
				{
					new string[MAX_STRING];
					format(string,MAX_STRING,gettext(1392),BetRace[i][1]);
					SystemMsg(i,COLOUR_MONEY_GOOD,string);
					oGivePlayerMoney(i,BetRace[i][1],false);
					break;
				}
			}
		}
	}
	if (BetDM[playerid][1] > 0){
		for (new i=0;i<MAX_PLAYERS_EX;i++)
		{
			if (IsPlayerConnected(i)){
				if(BetDM[i][0] == playerid)
				{
					new string[MAX_STRING];
					format(string,MAX_STRING,gettext(1393),BetDM[i][1]);
					SystemMsg(i,COLOUR_MONEY_GOOD,string);
					oGivePlayerMoney(i,BetDM[i][1],false);
					break;
				}
			}
		}
	}
	if (BetHay[playerid][1] > 0){
		for (new i=0;i<MAX_PLAYERS_EX;i++)
		{
			if (IsPlayerConnected(i)){
				if(BetHay[i][0] == playerid)
				{
					new string[MAX_STRING];
					format(string,MAX_STRING,gettext(1724),BetHay[i][1]);
					SystemMsg(i,COLOUR_MONEY_GOOD,string);
					oGivePlayerMoney(i,BetHay[i][1],false);
					break;
				}
			}
		}
	}
	BetRace[playerid][0] = INVALID_PLAYER_ID;
	BetRace[playerid][1] = 0;
	BetDM[playerid][0] = INVALID_PLAYER_ID;
	BetDM[playerid][1] = 0;
	BetHay[playerid][0] = INVALID_PLAYER_ID;
	BetHay[playerid][1] = 0;
}

public BetWinner(playerid,contest)
{
	new string[256];
	for(new i = 0; i <= MAX_PLAYERS_EX; i++)
	{
		if(IsPlayerConnected(i))
		{
			if(contest == 1)
			{
				if(BetRace[i][0] == playerid)
				{
					oGivePlayerMoney(i,BetRace[i][1]*2,false);
					format(string, MAX_STRING,gettext(1394), BetRace[i][1]*2, oGetPlayerName(playerid));
					SystemMsg(i,COLOUR_MONEY_GOOD,string);
					PlayerPlaySoundOnPlayer(i, 1052);
					format(string, MAX_STRING,gettext(1395), oGetPlayerName(i), BetRace[i][1]*2, oGetPlayerName(playerid));
					WriteLog(string);
					SystemMsgToRegistered(COLOUR_INFO,string);
				}
				BetRace[i][0] = INVALID_PLAYER_ID;
				BetRace[i][1] = 0;
			}
			if(contest == 2)
			{
				if(BetDM[i][0] == playerid)
				{
					oGivePlayerMoney(i,BetDM[i][1]*2,false);
					format(string, MAX_STRING,gettext(1396), BetDM[i][1]*2, oGetPlayerName(playerid));
					SystemMsg(i,COLOUR_MONEY_GOOD,string);
					PlayerPlaySoundOnPlayer(i, 1052);
					format(string, MAX_STRING,gettext(1395), oGetPlayerName(i), BetDM[i][1]*2, oGetPlayerName(playerid));
					WriteLog(string);
					SystemMsgToRegistered(COLOUR_INFO,string);
				}
				BetDM[i][0] = INVALID_PLAYER_ID;
				BetDM[i][1] = 0;
			}
			if(contest == 3)
			{
				if(BetHay[i][0] == playerid)
				{
					oGivePlayerMoney(i,BetHay[i][1]*2,false);
					format(string, MAX_STRING,gettext(1726), BetHay[i][1]*2, oGetPlayerName(playerid));
					SystemMsg(i,COLOUR_MONEY_GOOD,string);
					PlayerPlaySoundOnPlayer(i, 1052);
					format(string, MAX_STRING,gettext(1725), oGetPlayerName(i), BetHay[i][1]*2, oGetPlayerName(playerid));
					WriteLog(string);
					SystemMsgToRegistered(COLOUR_INFO,string);
				}
				BetHay[i][0] = INVALID_PLAYER_ID;
				BetHay[i][1] = 0;
			}
		}
	}
	return 1;
}

public Bets_OnPlayerCommandText(playerid,text[])
{
	if (!IsPlayerConnected(playerid)) return 0;
	dcmd(betdm,5,text);
	dcmd(betrace,7,text);
	dcmd(bethay,6,text);
	dcmd(mybets,6,text);
	return 0;
}

dcmd_betdm(playerid,params[])
{
	if (!IsPlayerRegistered(playerid))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(561));
		return 1;
	}

	new tmp[MAX_STRING];
	new giveplayerid,moneys,idx;
	
	if (BetDM[playerid][1] > 0){
		SystemMsg(playerid,COLOUR_ERROR,gettext(1397));
		return 1;
	}

	tmp = strcharsplit(params, idx,strchar(" "));
	if(isempty(tmp))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(1398));
		return 1;
	}

	giveplayerid = FindPlayerID(tmp);
	if (giveplayerid==-1) return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1571),tmp);
	if (giveplayerid==-2) return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1572),tmp);
		
	if (giveplayerid == playerid){
		SystemMsg(playerid,COLOUR_ERROR,gettext(1400));
		return 1;
	}
		
	tmp = strcharsplit(params, idx,strchar(" "));
	if(isempty(tmp))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(1398));
		return 1;
	}
	moneys = strval(tmp);
	if (moneys > MAX_DM_BET)
	{
		SystemMsgFormat(playerid,COLOUR_ERROR,gettext(1399),MAX_DM_BET);
		return 1;
	}
	new dmid = GetPlayerDM(playerid);

	if (dmid == INVALID_DM_ID){
		SystemMsg(playerid,COLOUR_ERROR,gettext(1401));
		return 1;		
	}

	if ((DeathmatchStats[dmid][dm_state] == DM_STATE_DISABLED) || (DeathmatchStats[dmid][dm_state] == DM_STATE_SLEEPING) || (DeathmatchStats[dmid][dm_state] == DM_STATE_ACTIVE)){
		SystemMsg(playerid,COLOUR_ERROR,gettext(1402));
		return 1;
	} 

	if (IsPlayerConnected(giveplayerid))
	{
		new playermoney = oGetPlayerMoney(playerid);
		if (moneys > 0 && playermoney >= moneys)
		{
			oGivePlayerMoney(playerid,-moneys,false);
			BetDM[playerid][0] = giveplayerid;
			BetDM[playerid][1] = moneys;
			new string[MAX_STRING];
			format(string, MAX_STRING,gettext(1403), moneys, oGetPlayerName(giveplayerid));
			SystemMsg(playerid,COLOUR_OK,string);
			PlayerPlaySoundOnPlayer(playerid, 1052);
			format(string, MAX_STRING,gettext(1404), moneys, oGetPlayerName(playerid));
			SystemMsg(giveplayerid,COLOUR_OK,string);
			format(string, MAX_STRING,gettext(1405), oGetPlayerName(playerid), moneys, oGetPlayerName(giveplayerid));
			WriteLog(string);
			SystemMsgToRegistered(COLOUR_INFO,string);
			PlayerPlaySoundOnPlayer(giveplayerid, 1052);
		}
		else
		{
			SystemMsg(playerid,COLOUR_ERROR,gettext(388));
		}
	}
	else
	{
			SystemMsg(playerid,COLOUR_ERROR,gettext(1295));
	}
	return 1;
}

dcmd_bethay(playerid,params[])
{
	if (!IsPlayerRegistered(playerid))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(561));
		return 1;
	}

	new tmp[MAX_STRING];
	new giveplayerid,moneys,idx;
	
	if (BetHay[playerid][1] > 0){
		SystemMsg(playerid,COLOUR_ERROR,gettext(1397));
		return 1;
	}

	tmp = strcharsplit(params, idx,strchar(" "));
	if(isempty(tmp))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(1727));
		return 1;
	}

	giveplayerid = FindPlayerID(tmp);
	if (giveplayerid==-1) return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1571),tmp);
	if (giveplayerid==-2) return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1572),tmp);
		
	if (giveplayerid == playerid){
		SystemMsg(playerid,COLOUR_ERROR,gettext(1728));
		return 1;
	}
		
	tmp = strcharsplit(params, idx,strchar(" "));
	if(isempty(tmp))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(1727));
		return 1;
	}
	moneys = strval(tmp);
	if (moneys > MAX_HAY_BET)
	{
		SystemMsgFormat(playerid,COLOUR_ERROR,gettext(1399),MAX_HAY_BET);
		return 1;
	}
	new hayid = GetPlayerHay(playerid);

	if (hayid == INVALID_HAY_ID){
		SystemMsg(playerid,COLOUR_ERROR,gettext(1729));
		return 1;		
	}

	if ((HayStats[hayid][hay_state] == HAY_STATE_DISABLED) || (HayStats[hayid][hay_state] == HAY_STATE_SLEEPING) || (HayStats[hayid][hay_state] == HAY_STATE_ACTIVE)){
		SystemMsg(playerid,COLOUR_ERROR,gettext(1730));
		return 1;
	} 

	if (IsPlayerConnected(giveplayerid))
	{
		new playermoney = oGetPlayerMoney(playerid);
		if (moneys > 0 && playermoney >= moneys)
		{
			oGivePlayerMoney(playerid,-moneys,false);
			BetHay[playerid][0] = giveplayerid;
			BetHay[playerid][1] = moneys;
			new string[MAX_STRING];
			format(string, MAX_STRING,gettext(1731), moneys, oGetPlayerName(giveplayerid));
			SystemMsg(playerid,COLOUR_OK,string);
			PlayerPlaySoundOnPlayer(playerid, 1052);
			format(string, MAX_STRING,gettext(1732), moneys, oGetPlayerName(playerid));
			SystemMsg(giveplayerid,COLOUR_OK,string);
			format(string, MAX_STRING,gettext(1733), oGetPlayerName(playerid), moneys, oGetPlayerName(giveplayerid));
			WriteLog(string);
			SystemMsgToRegistered(COLOUR_INFO,string);
			PlayerPlaySoundOnPlayer(giveplayerid, 1052);
		}
		else
		{
			SystemMsg(playerid,COLOUR_ERROR,gettext(388));
		}
	}
	else
	{
			SystemMsg(playerid,COLOUR_ERROR,gettext(1295));
	}
	return 1;
}

dcmd_betrace(playerid,params[])
{
	if (!IsPlayerRegistered(playerid))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(561));
		return 1;
	}

	new tmp[MAX_STRING];
	new giveplayerid,moneys,idx;
	if (BetRace[playerid][1] > 0){
		SystemMsg(playerid,COLOUR_ERROR,gettext(1397));
		return 1;
	}

	tmp = strcharsplit(params, idx,strchar(" "));
	if(isempty(tmp))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(1406));
		return 1;
	}

	giveplayerid = FindPlayerID(tmp);
	if (giveplayerid==-1) return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1571),tmp);
	if (giveplayerid==-2) return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1572),tmp);

	if (giveplayerid == playerid){
		SystemMsg(playerid,COLOUR_ERROR,gettext(1400));
		return 1;
	}

	tmp = strcharsplit(params, idx,strchar(" "));
	if(isempty(tmp))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(1406));
		return 1;
	}
	moneys = strval(tmp);
	if (moneys > MAX_RACE_BET)
	{
		SystemMsgFormat(playerid,COLOUR_ERROR,gettext(1399),MAX_RACE_BET);
		return 1;
	}

		new raceid = GetPlayerRace(playerid);

	if (raceid == INVALID_RACE_ID){
		SystemMsg(playerid,COLOUR_ERROR,gettext(1407));
		return 1;		
	}

	if ((RaceStats[raceid][race_state] == RACE_STATE_DISABLED) || (RaceStats[raceid][race_state] == RACE_STATE_SLEEPING) || (RaceStats[raceid][race_state] == RACE_STATE_RACING)){
		SystemMsg(playerid,COLOUR_ERROR,gettext(1408));
		return 1;
	} 

	if (IsPlayerConnected(giveplayerid))
	{
		new playermoney = oGetPlayerMoney(playerid);
		if (moneys > 0 && playermoney >= moneys)
		{
			oGivePlayerMoney(playerid,-moneys,false);
			BetRace[playerid][0] = giveplayerid;
			BetRace[playerid][1] = moneys;
			new string[MAX_STRING];
			format(string, MAX_STRING,gettext(1409), moneys, oGetPlayerName(giveplayerid));
			SystemMsg(playerid,COLOUR_OK,string);
			PlayerPlaySoundOnPlayer(playerid, 1052);
			format(string, MAX_STRING,gettext(1410), moneys, oGetPlayerName(playerid));
			SystemMsg(giveplayerid,COLOUR_OK,string);
			format(string, MAX_STRING,gettext(1405), oGetPlayerName(playerid), moneys, oGetPlayerName(giveplayerid));
			WriteLog(string);
			SystemMsgToRegistered(COLOUR_INFO,string);
			PlayerPlaySoundOnPlayer(giveplayerid, 1052);
		}
		else
		{
			SystemMsg(playerid,COLOUR_ERROR,gettext(388));
		}
	}
	else
	{
			SystemMsg(playerid,COLOUR_ERROR,gettext(1295));
	}
return 1;
}

dcmd_mybets(playerid,params[])
{
	#pragma unused params
	new giveplayer[MAX_NAME],string[MAX_STRING];

	if(BetRace[playerid][0] == INVALID_PLAYER_ID)
	{
		format(giveplayer,MAX_NAME,gettext(1411));
	} else {
		format(giveplayer,MAX_NAME,oGetPlayerName(BetRace[playerid][0]));
	}
	format(string, MAX_STRING,gettext(1412),giveplayer, BetRace[playerid][1]);
	SystemMsg(playerid,COLOUR_INFO,string);

	if(BetDM[playerid][0] == INVALID_PLAYER_ID)
	{
		format(giveplayer,MAX_NAME,gettext(1411));
	} else {
		format(giveplayer,MAX_NAME,oGetPlayerName(BetDM[playerid][0]));
	}
	format(string, MAX_STRING,gettext(1413),giveplayer, BetDM[playerid][1]);
	SystemMsg(playerid,COLOUR_INFO,string);

	if(BetHay[playerid][0] == INVALID_PLAYER_ID)
	{
		format(giveplayer,MAX_NAME,gettext(1411));
	} else {
		format(giveplayer,MAX_NAME,oGetPlayerName(BetHay[playerid][0]));
	}
	format(string, MAX_STRING,gettext(1734),giveplayer, BetHay[playerid][1]);
	SystemMsg(playerid,COLOUR_INFO,string);

	return 1;
}
