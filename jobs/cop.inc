//Cop Job module
//Created by MadCat
//Spawns and places by Nitro's (Nitro's Freeroam RPG);

#include "businesses\skydive"

new copjobinfoshowed[MAX_PLAYERS] = 0;

#define CopSpawnsCount 9

new CopSpawns[CopSpawnsCount][CoordInfo] = {
	{2297.1064,2452.0115,10.8203},
	{2297.0452,2468.6743,10.8203},
	{1553.4371,-1675.5244,16.1953},
	{611.0424,-583.5018,18.2109},
	{-2149.1824,-2395.7847,30.6250},
	{-2161.8364,-2385.3284,30.6250},
	{-1616.5706,683.7604,7.1875},
	{-1389.8243,2636.7017,55.9844},
	{-217.3357,979.5353,19.5020}
};

#define CopJobPlaces 7

new CopJobPlace[CopJobPlaces][CoordInfo] = {
	{2340.3408,2456.9556,14.9688},
	{1553.4371,-1675.5244,16.1953},
	{628.4794,-571.6782,17.2009},
	{-2161.8364,-2385.3284,30.6250},
	{-1605.7537,712.8554,13.8089},
	{-1389.8243,2636.7017,55.9844},
	{-217.3357,979.5353,19.5020}
};


new WasInCopCar[MAX_PLAYERS] = 0;

forward Cop_OnGameModeInit();
forward IsPlayerAtCopJobPlace(playerid);
forward CopJobPickupCheck(playerid);
forward Cop_OnPlayerCommandText(playerid,text[]);
forward CopKill(killerid, victimid, reason);
forward Cop_OnPlayerDeath(playerid,killerid,reason);
forward Cop_OnPlayerStateChange(playerid, newstate, oldstate);
forward Cop_OnPlayerSpawn(playerid);
forward CheckCopSkill(playerid);

public Cop_OnGameModeInit()
{	Debug("jobs/cop.inc > Cop_OnGameModeInit - Start");
	for (new id=0;id<CopJobPlaces;id++)
	{
		CreateStreamPickup(1248,1,CopJobPlace[id][Coord_X],CopJobPlace[id][Coord_Y],CopJobPlace[id][Coord_Z]);
		CreateStreamPickup(1247,1,CopJobPlace[id][Coord_X],CopJobPlace[id][Coord_Y],CopJobPlace[id][Coord_Z]+0.5);
		CreateStreamMapIcon(30,CopJobPlace[id][Coord_X],CopJobPlace[id][Coord_Y],CopJobPlace[id][Coord_Z]);
		CreateDMapIcon(30,CopJobPlace[id][Coord_X],CopJobPlace[id][Coord_Y],CopJobPlace[id][Coord_Z],gettext(1751),gettext(1752));
	}
	WriteLog("Cop Job checkpoints Loaded");
	Debug("jobs/cop.inc > Cop_OnGameModeInit - Stop");
}

public IsPlayerAtCopJobPlace(playerid)
{
	Debug("jobs/cop.inc > IsPlayerAtCopJobPlace - Start");
	for (new id=0;id<CopJobPlaces;id++)
	{
		if (IsPlayerInRangeOfPoint(playerid,1.0,CopJobPlace[id][Coord_X],CopJobPlace[id][Coord_Y],CopJobPlace[id][Coord_Z]))
		{
			Debug("jobs/cop.inc > IsPlayerAtCopJobPlace - Stop");
			return 1;
		}
	}
	Debug("jobs/cop.inc > IsPlayerAtCopJobPlace - Stop");
	return 0;
}

public CopJobPickupCheck(playerid)
{
	Debug("jobs/cop.inc > CopJobPickupCheck - Start");
	if (playerid == INVALID_PLAYER_ID) return;
	if (!IsPlayerConnected(playerid)) return;
	if (SpawnTime[playerid] < 5) return;
	if (!IsPlayerAtCopJobPlace(playerid)) {copjobinfoshowed[playerid] = 0; return;}
	if (copjobinfoshowed[playerid] == 1) return;
	copjobinfoshowed[playerid] = 1;
		if (Player[playerid][GotJob] == JOB_COP){
			SystemMsg(playerid,COLOUR_POLICE,gettext(562));
		} else {
			SystemMsg(playerid,COLOUR_POLICE,gettext(563));
		}
	Debug("jobs/cop.inc > CopJobPickupCheck - Stop");
}

public Cop_OnPlayerCommandText(playerid,text[])
{
	if (!IsPlayerConnected(playerid)) return 0;

	if (!IsPlayerRegistered(playerid)) return 0;

	dcmd(wanteds,7,text);
	dcmd(nocop,5,text);
	dcmd(beacop,6,text);
	dcmd(ar,2,text);

	return 0;	
}

dcmd_wanteds(playerid,params[]){
	#pragma unused params
	Debug("jobs/cop.inc  > Command 'wanteds' - Start");
	new temp[MAX_STRING];
	new count = 0;
	for (new i=0;i<MAX_PLAYERS_EX;i++)
	{
		if (IsPlayerConnected(i)){
			if(Player[i][WantedLevel]>0 || Player[i][Crimes]>0)
			{
				count++;
				format(temp,sizeof(temp),gettext(564),count,oGetPlayerName(i),Player[i][Crimes],Player[i][WantedLevel]);
				SystemMsgScrolling(playerid,temp);
				set(temp,nullstr);
			}
		}
	}
	if(count < 1)
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(565));
	}
	Debug("jobs/cop.inc  > Command 'wanteds' - Stop");
	return 1;
}

dcmd_nocop(playerid,params[]){
	#pragma unused params
	Debug("jobs/cop.inc  > Command 'nocop' - Start");
	if(Player[playerid][GotJob] != JOB_COP){
		SystemMsg(playerid,COLOUR_ERROR,gettext(566));
		Debug("jobs/cop.inc  > Command 'nocop' - Stop");
		return 1;
	}

	if (!IsPlayerAtCopJobPlace(playerid)){
		SystemMsg(playerid,COLOUR_ERROR,gettext(567));
		Debug("jobs/cop.inc  > Command 'nocop' - Stop");
		return 1;	
	}

	SetPlayerSkin(playerid,SKIN_STANDART);
	Player[playerid][GotJob]=JOB_NOJOB;
	SystemMsg(playerid,COLOUR_POLICE,gettext(568));
	new string[MAX_STRING];
	format(string,sizeof(string),gettext(569),oGetPlayerName(playerid));
	SystemMsgToRegistered(COLOUR_POLICE,string);
	if (PlayerGangid[playerid] == 0){
		SetPlayerColor(playerid,COLOUR_PLAYER);		
	} else {
		SetPlayerColor(playerid,PlayerGangColour(playerid));
	}
	new logstring[256];
	format(logstring, sizeof (logstring), "player: %d:  %s: not a cop now",playerid,oGetPlayerName(playerid));
	WriteLog(logstring);
	Debug("jobs/cop.inc  > Command 'nocop' - Stop");
	return 1;
}

dcmd_beacop(playerid,params[]){
	#pragma unused params
	Debug("jobs/cop.inc  > Command 'beacop' - Start");

	if (!IsPlayerAtCopJobPlace(playerid)){
			SystemMsg(playerid,COLOUR_ERROR,gettext(567));
			Debug("jobs/cop.inc  > Command 'beacop' - Stop");
			return 1;	
	}

	if(PlayerGangid[playerid] != 0)
		{
		SystemMsg(playerid,COLOUR_ERROR,gettext(570));
		Debug("jobs/cop.inc  > Command 'beacop' - Stop");
		return 1;
	}

	if(Player[playerid][GotJob]==JOB_COP)
		{
		SystemMsg(playerid,COLOUR_ERROR,gettext(571));
		Debug("jobs/cop.inc  > Command 'beacop' - Stop");
		return 1;
	}

	if(Player[playerid][GotJob] != JOB_NOJOB || GetPlayerDM(playerid) || GetPlayerRace(playerid))
		{
		SystemMsg(playerid,COLOUR_ERROR,gettext(572));
		Debug("jobs/cop.inc  > Command 'beacop' - Stop");
		return 1;
	}
	if(Player[playerid][WantedLevel]>0 || Player[playerid][Crimes]>0)
		{
			SystemMsg(playerid,COLOUR_ERROR,gettext(573));
			Debug("jobs/cop.inc  > Command 'beacop' - Stop");
			return 1;
		}

	Player[playerid][GotJob]=JOB_COP;	
	if (Player[playerid][CopLevel] == 0){
		SetPlayerSkin(playerid,SKIN_COP);
		SystemMsg(playerid,COLOUR_POLICE,gettext(574));
	} else if (Player[playerid][CopLevel] == 1){
		SetPlayerSkin(playerid,SKIN_GOODCOP);
		SystemMsg(playerid,COLOUR_POLICE,gettext(575));
	} else if (Player[playerid][CopLevel] == 2){
		SetPlayerSkin(playerid,SKIN_STREETPATROL);
		SystemMsg(playerid,COLOUR_POLICE,gettext(576));
	} else if (Player[playerid][CopLevel] == 3){
		SetPlayerSkin(playerid,SKIN_POLICEOFFICER);
		SystemMsg(playerid,COLOUR_POLICE,gettext(577));
	} else if (Player[playerid][CopLevel] == 4){
		SetPlayerSkin(playerid,SKIN_SWATMEMBER);
		SystemMsg(playerid,COLOUR_POLICE,gettext(578));
	} else if (Player[playerid][CopLevel] == 5){
		SetPlayerSkin(playerid,SKIN_FBIAGENT);
		SystemMsg(playerid,COLOUR_POLICE,gettext(579));
	}
	SetPlayerColor(playerid,COLOUR_BLUE);
	ShowTextForPlayer(playerid,gettext(580),2500,6);
	SystemMsg(playerid,COLOUR_POLICE,gettext(581));
	SystemMsg(playerid,COLOUR_POLICE,gettext(582));
	new string[MAX_STRING];
	format(string,sizeof(string),gettext(583),oGetPlayerName(playerid));
	SystemMsgToRegistered(COLOUR_POLICE,string);
	new logstring[256];
	format(logstring, sizeof (logstring), "player: %d:  %s: is a cop now",playerid,oGetPlayerName(playerid));
	WriteLog(logstring);
	Debug("jobs/cop.inc  > Command 'beacop' - Stop");
	return 1;
}

dcmd_ar(playerid,params[]){
	#pragma unused params
	Debug("jobs/cop.inc  > Command 'ar' - Start");
	if(Player[playerid][GotJob] != JOB_COP){
		SystemMsg(playerid,COLOUR_ERROR,gettext(566));
		Debug("jobs/cop.inc  > Command 'ar' - Stop");
		return 1;
	}

	if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT){
		SystemMsg(playerid,COLOUR_ERROR,gettext(585));
		Debug("jobs/cop.inc  > Command 'ar' - Stop");
		return 1;
	}

	new closplayer;
	closplayer=GetClosestPlayer(playerid);
	if(closplayer == -1) {
		SystemMsg(playerid,COLOUR_ERROR,gettext(586));
		Debug("jobs/cop.inc  > Command 'ar' - Stop");
		return 1;
	}

	new Float:d = GetDistanceBetweenPlayers(playerid, closplayer);
	if (d > 7) {
		SystemMsg(playerid,COLOUR_ERROR,gettext(587));
		Debug("jobs/cop.inc  > Command 'ar' - Stop");
		return 1;
	}
	if (Player[closplayer][WantedLevel] == 0 && Player[closplayer][Crimes] == 0){
		SystemMsg(playerid,COLOUR_ERROR,gettext(588));
		Debug("jobs/cop.inc  > Command 'ar' - Stop");
		return 1;
	}

	if (IsPlayerInAnyVehicle(closplayer)){
		SystemMsg(playerid,COLOUR_ERROR,gettext(589));
		Debug("jobs/cop.inc  > Command 'ar' - Stop");
		return 1;
	}

	if (GetPlayerState(closplayer) == PLAYER_STATE_WASTED){
		SystemMsg(playerid,COLOUR_ERROR,gettext(590));
		Debug("jobs/cop.inc  > Command 'ar' - Stop");
		return 1;
	}

	if(PlayerSkydive[closplayer] == 1){
		SystemMsg(playerid,COLOUR_ERROR,gettext(591));
		Debug("jobs/cop.inc  > Command 'ar' - Stop");
		return 1;
	}

	if(closplayer == playerid){
		SystemMsg(playerid,COLOUR_ERROR,gettext(592));
		Debug("jobs/cop.inc  > Command 'ar' - Stop");
		return 1;
	}
	if (oGetPlayerHealth(closplayer) < 50){
		oGivePlayerMoney(closplayer,-oGetPlayerMoney(closplayer),false);
		SetWantedLevel(closplayer,0);
		SystemMsg(closplayer,COLOUR_POLICE,gettext(593));
		ShowTextForPlayer(closplayer,gettext(594),2500,5);
		Player[closplayer][GotBusted]++;
		ResetPlayerWeapons(closplayer);
		JailPlayer(closplayer,60,gettext(595));
		oGivePlayerMoney(playerid,(1000*Player[closplayer][WantedLevel])+(Player[closplayer][Crimes]*50),1);
		Player[playerid][CopSkill]=Player[playerid][CopSkill]+Player[closplayer][WantedLevel]+3;
		GivePlayerXP(playerid,(500*Player[closplayer][WantedLevel])+(Player[closplayer][Crimes]*25),1);
		Player[closplayer][WantedLevel]=0;
		Player[closplayer][Crimes]=0;
		WasInCopCar[closplayer]=0;
		WasInAmbulance[closplayer]=0;
		SystemMsg(playerid,COLOUR_POLICE,gettext(596));
		ShowTextForPlayer(playerid,gettext(597),2500,5);
		CheckCopSkill(playerid);
		new string[256];
		if (Player[playerid][CopLevel] == 0){
			format(string,sizeof(string),gettext(598),oGetPlayerName(playerid),oGetPlayerName(closplayer));
		} else if (Player[playerid][CopLevel] == 1){
			format(string,sizeof(string),gettext(599),oGetPlayerName(playerid),oGetPlayerName(closplayer));
		} else if (Player[playerid][CopLevel] == 2){
			format(string,sizeof(string),gettext(600),oGetPlayerName(playerid),oGetPlayerName(closplayer));
		} else if (Player[playerid][CopLevel] == 3){
			format(string,sizeof(string),gettext(601),oGetPlayerName(playerid),oGetPlayerName(closplayer));
		} else if (Player[playerid][CopLevel] == 4){
			format(string,sizeof(string),gettext(602),oGetPlayerName(playerid),oGetPlayerName(closplayer));
		} else if (Player[playerid][CopLevel] == 5){
			format(string,sizeof(string),gettext(603),oGetPlayerName(playerid),oGetPlayerName(closplayer));
		}
		SystemMsgToRegistered(COLOUR_POLICE,string);
		new logstring[256];
		format(logstring, sizeof (logstring), "player: %d:  %s: arrest criminal %s",playerid,oGetPlayerName(playerid),oGetPlayerName(closplayer));
		WriteLog(logstring);
	} else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(604)); 
	}
	Debug("jobs/cop.inc  > Command 'ar' - Stop");
	return 1;
}

public CopKill(killerid, victimid, reason){
Debug("jobs/cop.inc  > CopKill - Start");
if (Player[victimid][WantedLevel]>0){
	oGivePlayerMoney(killerid,(300*Player[victimid][WantedLevel])+(Player[victimid][Crimes]*20),1);
	GivePlayerXP(killerid,(150*Player[victimid][WantedLevel])+(Player[victimid][Crimes]*10),1);
	Player[killerid][CopSkill]=Player[killerid][CopSkill]+Player[victimid][WantedLevel];
	CheckCopSkill(killerid);
	new string[256];
	if (Player[killerid][CopLevel] == 0){
	format(string,sizeof(string),gettext(605),oGetPlayerName(killerid),oGetPlayerName(victimid));
	} else if (Player[killerid][CopLevel] == 1){
	format(string,sizeof(string),gettext(606),oGetPlayerName(killerid),oGetPlayerName(victimid));
	} else if (Player[killerid][CopLevel] == 2){
	format(string,sizeof(string),gettext(607),oGetPlayerName(killerid),oGetPlayerName(victimid));
	} else if (Player[killerid][CopLevel] == 3){
	format(string,sizeof(string),gettext(608),oGetPlayerName(killerid),oGetPlayerName(victimid));
	} else if (Player[killerid][CopLevel] == 4){
	format(string,sizeof(string),gettext(609),oGetPlayerName(killerid),oGetPlayerName(victimid));
	} else if (Player[killerid][CopLevel] == 5){
	format(string,sizeof(string),gettext(610),oGetPlayerName(killerid),oGetPlayerName(victimid));
	}
	SystemMsgToRegistered(COLOUR_POLICE,string);
}else{
	SystemMsg(killerid,COLOUR_POLICE,gettext(611));
	oGivePlayerMoney(killerid,-(500*Player[victimid][Level]),1);
	GivePlayerXP(killerid,-(200*Player[victimid][Level]),1);
	new string[MAX_STRING];
	format(string,sizeof(string),gettext(612),oGetPlayerName(killerid));
	SystemMsgToRegistered(COLOUR_POLICE,string);
	new logstring[256];
	format(logstring, sizeof (logstring), "player: %d:  %s: player without crimes kill as a cop",killerid,oGetPlayerName(killerid));
	WriteLog(logstring);
}
Debug("jobs/cop.inc  > CopKill - Stop");
return 1;
}

public Cop_OnPlayerDeath(playerid,killerid,reason){
if (IsPlayerInDuel(playerid)) return 0;
Debug("jobs/cop.inc  > Cop_OnPlayerDeath - Start");
new string[256];
if (killerid == INVALID_PLAYER_ID){
	if (Player[playerid][CopLevel] == 0){
		format(string,sizeof(string),gettext(613),oGetPlayerName(playerid));
	} else if (Player[playerid][CopLevel] == 1){
		format(string,sizeof(string),gettext(614),oGetPlayerName(playerid));
	} else if (Player[playerid][CopLevel] == 2){
		format(string,sizeof(string),gettext(615),oGetPlayerName(playerid));
	} else if (Player[playerid][CopLevel] == 3){
		format(string,sizeof(string),gettext(616),oGetPlayerName(playerid));
	} else if (Player[playerid][CopLevel] == 4){
		format(string,sizeof(string),gettext(617),oGetPlayerName(playerid));
	} else if (Player[playerid][CopLevel] == 5){
		format(string,sizeof(string),gettext(618),oGetPlayerName(playerid));
	}
	SystemMsgToRegistered(COLOUR_POLICE,string);
} else {
	SystemMsg(killerid,COLOUR_POLICE,gettext(619));
	if (Player[playerid][CopLevel] == 0){
		format(string,sizeof(string),gettext(620),oGetPlayerName(playerid));
		SetPlayerCriminal(killerid,string,2);
	} else if (Player[playerid][CopLevel] == 1){
		format(string,sizeof(string),gettext(621),oGetPlayerName(playerid));
		SetPlayerCriminal(killerid,string,3);
	} else if (Player[playerid][CopLevel] == 2){
		format(string,sizeof(string),gettext(622),oGetPlayerName(playerid));
		SetPlayerCriminal(killerid,string,4);
	} else if (Player[playerid][CopLevel] == 3){
		format(string,sizeof(string),gettext(623),oGetPlayerName(playerid));
		SetPlayerCriminal(killerid,string,5);
	} else if (Player[playerid][CopLevel] == 4){
		format(string,sizeof(string),gettext(624),oGetPlayerName(playerid));
		SetPlayerCriminal(killerid,string,6);
	} else if (Player[playerid][CopLevel] == 5){
		format(string,sizeof(string),gettext(625),oGetPlayerName(playerid));
		SetPlayerCriminal(killerid,string,7);
	}
}
if ((Player[playerid][Money] - 50) > 0)
{
	if (Drop_Money_On_Death == 1) DropDeathMoney(playerid,50);
	oGivePlayerMoney(playerid,-50,0);
}
else
{
	oSetPlayerMoney(playerid,0);
}
if (Player[playerid][XP] - (Player[playerid][XP] /400) > 0)
{
	GivePlayerXP(playerid,-(Player[playerid][XP] /400),1); // take death penalty xp
}
else
{
	Player[playerid][XP] = 0;
	SetPlayerScore(playerid,Player[playerid][XP]);
}
new logstring[256];
format(logstring, sizeof (logstring), "player: %d:  %s: kill cop, wanted level and crimes added",killerid,oGetPlayerName(killerid));
WriteLog(logstring);
Debug("jobs/cop.inc  > Cop_OnPlayerDeath - Stop");
return 0;
}

public Cop_OnPlayerStateChange(playerid, newstate, oldstate)
{
	Debug("jobs/cop.inc  > Cop_OnPlayerStateChange - Start");
	if (newstate == PLAYER_STATE_DRIVER){
		if (IsACopCar(GetPlayerVehicleID(playerid))){
			if (Player[playerid][GotJob] != JOB_COP){
				if (Player[playerid][GotJob] == JOB_MEDIC){
					RemovePlayerFromVehicle(playerid);
					SystemMsg(playerid,COLOUR_POLICE,gettext(626));
				} else {
					if (WasInCopCar[playerid] == 0){
						SetPlayerCriminal(playerid,gettext(627),2);
						WasInCopCar[playerid] = 1;
					}
				}
			}
		}
	}
	Debug("jobs/cop.inc  > Cop_OnPlayerStateChange - Stop");
}

public Cop_OnPlayerSpawn(playerid)
{
	Debug("jobs/cop.inc  > Cop_OnPlayerSpawn - Start");
	if (Player[playerid][GotJob] == JOB_COP && Player[playerid][SpawnPlace] == SPAWN_JOB){
		new rand = random(CopSpawnsCount);
		SetPlayerPos(playerid, CopSpawns[rand][Coord_X], CopSpawns[rand][Coord_Y], CopSpawns[rand][Coord_Z]);
	}
	Debug("jobs/cop.inc  > Cop_OnPlayerSpawn - Stop");
	return 0;
}

stock IsACopCar(vehicleid){
new model = GetVehicleModel(vehicleid);
if (model == V_COPBIKE || model == V_COPCARRU || model == V_COPCARVG || model == V_COPCARSF || model == V_COPCARLA || model == V_BARRACKS || model == V_ENFORCER|| model == V_FBITRUCK|| model == V_FBIRANCH|| model == V_SWATVAN) return 1;
return 0;
}

public CheckCopSkill(playerid){
	Debug("jobs/cop.inc  > CheckCopSkill - Start");
	if (Player[playerid][CopSkill] > 24 && Player[playerid][CopSkill] < 50){
		if (Player[playerid][CopLevel] != 1){
			SystemMsg(playerid,COLOUR_POLICE,gettext(629)); Player[playerid][CopLevel] = 1;	
		}
	}
	else if(Player[playerid][CopSkill] > 49 && Player[playerid][CopSkill] < 100){
		if (Player[playerid][CopLevel] != 2){
			SystemMsg(playerid,COLOUR_POLICE,gettext(630)); Player[playerid][CopLevel] = 2;
		}
	}
	else if(Player[playerid][CopSkill] > 99 && Player[playerid][CopSkill] < 200){
		if (Player[playerid][CopLevel] != 3){
			SystemMsg(playerid,COLOUR_POLICE,gettext(631)); Player[playerid][CopLevel] = 3;
		}
	}
	else if(Player[playerid][CopSkill] > 199 && Player[playerid][CopSkill] < 300){
		if (Player[playerid][CopLevel] != 4){
			SystemMsg(playerid,COLOUR_POLICE,gettext(632)); Player[playerid][CopLevel] = 4;
		}
	}
	else if(Player[playerid][CopSkill] > 399){
		if (Player[playerid][CopLevel] != 5){
			SystemMsg(playerid,COLOUR_POLICE,gettext(633)); Player[playerid][CopLevel] = 5;
		}
	}
	Debug("jobs/cop.inc  > CheckCopSkill - Stop");
}

stock SendMsgToCops(msg[]){
	Debug("jobs/cop.inc  > SendMsgToCops - Start");
	for(new i = 0; i <= MAX_PLAYERS_EX; i++)
	{
		if (IsPlayerConnected(i)){
			if (Player[i][GotJob] == JOB_COP){
				SystemMsg(i,COLOUR_POLICE,msg);
			}
		}
	}
	Debug("jobs/cop.inc  > SendMsgToCops - Stop");
}