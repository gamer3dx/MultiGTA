//Hotwire module
//Created by MadCat
//Idea from PEN1
//Thanks to GTAT Team for great Hotwire InfoTextDraw

forward Hotwire_OnGameModeInit();
forward Hotwire_OnPlayerStateChange(playerid, newstate, oldstate);
forward Hotwire_OnPlayerEnterCheckpoint(playerid);
forward Hotwire_OnVehicleDeath(vehicleid,killerid);
forward Hotwire_OnPlayerExitVehicle(playerid,vehicleid);
forward Hotwire_OnPlayerConnect(playerid);
forward FinishHotwire(playerid);
forward FinishHotwireTimeLeft();
forward hotwireconfig();
forward HotwireTimer();
forward HotwireTickTimer();
forward StartHotWire();

new Text:Hotwire_TextDraw[MAX_PLAYERS];
new Hotwire_TextDrawExists[MAX_PLAYERS];

new htimeleft=0;

new Float:cwx;
new Float:cwy;
new Float:cwz;

new quest_hotwire = INVALID_QUEST_ID;

new htimelefttimer;

//5 mins before hotwire will be finished
#define HWTIME 30000

new stealcar = INVALID_VEHICLE_ID;
new stealcardest = INVALID_VEHICLE_ID;

new hreward = 0;

new Players_To_Hotwire = 6;

public Hotwire_OnGameModeInit(){
	hotwireconfig();
	quest_hotwire = RegisterQuest("Hotwire");
	SetTimer("HotwireTimer", 720000, 1);
	WriteLog("Hotwire Minigame Loaded");
	return 0;
}

public hotwireconfig()
{
	Debug("hotwire.inc > hotwireconfig - Start");
	if (!db_Exists(ConfigDB)) db_Create(ConfigDB);

	new temp[MAX_STRING];
	
	set(temp,db_Get(ConfigDB,"Players_To_Hotwire"));
	if (strlen(temp) > 0) Players_To_Hotwire = strval(temp); 
	else { valstr(temp,Players_To_Hotwire); db_Set(ConfigDB,"Players_To_Hotwire",temp); }
	set(temp,nullstr);
	Debug("hotwire.inc > hotwireconfig - Stop");
}

public HotwireTimer()
{
	Debug("hotwire.inc > HotWireTimer - Start");
	if (event_now == false){
		if (PlayerCount() >= Players_To_Hotwire){
			StartHotWire();
		}
	}
	Debug("hotwire.inc > HotWireTimer - Start");

}

public StartHotWire(){
Debug("hotwire.inc > StartHotWire - Start");
stealcar = random(ActiveVehiclesCount);
while (GetVehicleModel(stealcar) == V_SPEEDER || GetVehicleModel(stealcar) == V_JETMAX || GetVehicleModel(stealcar) == V_SQUALO || GetVehicleModel(stealcar) == V_RUSTLER || GetVehicleModel(stealcar) == V_BEAGLE || GetVehicleModel(stealcar) == V_AMBULAN || GetVehicleModel(stealcar) == V_FIRETRUK || GetVehicleModel(stealcar) == V_POLMAV || GetVehicleModel(stealcar) == V_VCNMAV){
stealcar = random(ActiveVehiclesCount);
}
new Float:carx,Float:cary,Float:carz;
GetVehiclePos(stealcar,carx,cary,carz);
stealcardest=random(ActiveVehiclesCount);
while (stealcardest == stealcar)
{
	stealcardest = random(ActiveVehiclesCount);
}
while (GetVehicleModel(stealcardest) == V_SPEEDER || GetVehicleModel(stealcardest) == V_JETMAX || GetVehicleModel(stealcardest) == V_SQUALO || GetVehicleModel(stealcardest) == V_COPBIKE || GetVehicleModel(stealcardest) == V_COPCARLA || GetVehicleModel(stealcardest) == V_COPCARRU || GetVehicleModel(stealcardest) == V_COPCARSF || GetVehicleModel(stealcardest) == V_COPCARVG){
	stealcardest = random(ActiveVehiclesCount);
}
event_now = true;
new time = HWTIME;
if (GetDistanceBetweenVehicles(stealcar,stealcardest) > 5000){
	time = time * 6;
} else if (GetDistanceBetweenVehicles(stealcar,stealcardest) > 4000){
	time = time * 5;
} else if (GetDistanceBetweenVehicles(stealcar,stealcardest) > 3000){
	time = time * 4;
} else if (GetDistanceBetweenVehicles(stealcar,stealcardest) > 2000){
	time = time * 3;
} else if (GetDistanceBetweenVehicles(stealcar,stealcardest) > 1000){
	time = time * 2;
}
htimeleft = time/1000;
htimelefttimer=SetTimer("HotwireTickTimer",1000,1);
for(new i = 0; i <= MAX_PLAYERS_EX; i++)
			{
				if(IsPlayerConnected(i))
				{

					while (GetPlayerVehicleID(i) == stealcar)
					{
						stealcar = random(ActiveVehiclesCount);
					}
					if (Player[i][Cop] == 1){
					SystemMsg(i,gettext(82));
					ShowTextForPlayer(i,gettext(81), 5000, 1);
					oSetVehicleParamsForPlayer(stealcar,i,1,0);
					PlayerQuest[i] = quest_hotwire;
					ShowHotwireInfo(i);
					} else if (Player[i][Medic] == 1){
						// do nothing...
					} else {
					ShowTextForPlayer(i,gettext(81), 5000, 1);
					oSetVehicleParamsForPlayer(stealcar,i,1,0);
					new string[256];
					format(string,sizeof(string),gettext(83),GetXYZZoneName(carx,cary,carz));
					SystemMsg(i,string);
					PlayerQuest[i] = quest_hotwire;
					ShowHotwireInfo(i);
					}
				}
			}
Debug("hotwire.inc > StartHotWire - Stop");
}

public Hotwire_OnPlayerExitVehicle(playerid, vehicleid)
{
	Debug("hotwire.inc > Hotwire_OnPlayerExitVehicle - Start");
	if ((vehicleid == stealcar) && (!oIsPlayerInCheckpoint(playerid,cwx,cwy,cwz,10))){
		ShowTextForPlayer(playerid,gettext(85), 5000, 1);
		new string[256];
		format(string,sizeof(string),gettext(86),oGetPlayerName(playerid));
		for(new i = 0; i <= MAX_PLAYERS_EX; i++)
		{
			if(IsPlayerConnected(i) && Player[i][Medic] == 0)
			{
				oSetVehicleParamsForPlayer(stealcar,i,1,0);
				SystemMsg(i,string);
			}
		}
	}
	if ((vehicleid == stealcar) && (oIsPlayerInCheckpoint(playerid,cwx,cwy,cwz,10))){
	FinishHotwire(playerid);
	}

	Debug("hotwire.inc > Hotwire_OnPlayerExitVehicle - Stop");
	return 0;
}

public Hotwire_OnPlayerStateChange(playerid, newstate, oldstate){
Debug("hotwire.inc > Hotwire_OnPlayerStateChange - Start");
if(newstate == PLAYER_STATE_DRIVER)
{
	if (GetPlayerVehicleID(playerid) == stealcar){
		if (Player[playerid][Cop] == 1){
			new string[256];
			ShowTextForPlayer(playerid,gettext(87), 5000, 1);
			if (GameCity == 0){
				cwx=1540.5975;
				cwy=-1667.5183;
				cwz=13.5509;
			} else if (GameCity == 1){
				cwx=-1628.6217;
				cwy=651.3723;
				cwz=7.1875;
			} else if (GameCity == 2){
				cwx=2277.8303;
				cwy=2426.3796;
				cwz=10.8203;
			} else {
				new rand2 = random(3);
				if (rand2 == 1){
				cwx=2277.8303;
				cwy=2426.3796;
				cwz=10.8203;
				} else if (rand2 == 2){
				cwx=1540.5975;
				cwy=-1667.5183;
				cwz=13.5509;
				} else {
				cwx=-1628.6217;
				cwy=651.3723;
				cwz=7.1875;
				}
			}
			if (hreward == 0){
				hreward = GetVehicleCost(stealcar)+random(20000);
				new dist = floatround(PlayerDistanceTo(playerid,cwx,cwy,cwz,false));
				if (dist > 4000){
					hreward = hreward * 5;
				} else if (dist > 3000){
					hreward = hreward * 4;
				} else if (dist > 2000){
					hreward = hreward * 3;
				} else if (dist > 1000){
					hreward = hreward * 2;
				}
			}
			format(string,sizeof(string),gettext(88),oGetPlayerName(playerid),hreward);
			for (new pid=0; pid<MAX_PLAYERS_EX;pid++)
			{
				if (IsPlayerConnected(pid)){
					if (Player[pid][Medic] == 0){
						SystemMsg(pid, string);
						oSetPlayerCheckpoint(pid,cwx, cwy, cwz, 8.0);
						PlayerQuest[pid] = quest_hotwire;
					}
				}

			}
		} else {
			new string[256];
			GetVehiclePos(stealcardest, cwx, cwy, cwz);
			ShowTextForPlayer(playerid,gettext(89), 5000, 1);
			if (hreward == 0){
				hreward = GetVehicleCost(stealcar)+random(20000);
				new dist = floatround(PlayerDistanceTo(playerid,cwx,cwy,cwz,false));
				if (dist > 4000){
					hreward = hreward * 5;
				} else if (dist > 3000){
					hreward = hreward * 4;
				} else if (dist > 2000){
					hreward = hreward * 3;
				} else if (dist > 1000){
					hreward = hreward * 2;
				}
			}
			format(string, sizeof(string),gettext(90), oGetPlayerName(playerid),hreward);
			for (new pid=0; pid<MAX_PLAYERS_EX;pid++)
			{
				if (IsPlayerConnected(pid)){
					if (Player[pid][Medic] == 0){
						SystemMsg(pid, string);
						oSetPlayerCheckpoint(pid,cwx, cwy, cwz, 16.0);
						PlayerQuest[pid] = quest_hotwire;
					}
				}
			}
		}
	}
}
Debug("hotwire.inc > Hotwire_OnPlayerStateChange - Stop");
return 0;
}

public Hotwire_OnPlayerEnterCheckpoint(playerid)
{
	Debug("hotwire.inc > Hotwire_OnPlayerEnterCheckpoint - Start");
	if (oIsPlayerInCheckpoint(playerid,cwx,cwy,cwz,50) && IsPlayerInAnyVehicle(playerid) && GetPlayerVehicleID(playerid) == stealcar){
		ShowTextForPlayer(playerid,gettext(91),5000, 1);
	}
	Debug("hotwire.inc > Hotwire_OnPlayerEnterCheckpoint - Stop");
}

public HotwireTickTimer()
{
Debug("hotwire.inc > HotwireTickTimer - Start");
htimeleft--;
for (new playerid=0; playerid<MAX_PLAYERS_EX;playerid++)
{
	if (IsPlayerConnected(playerid)){
		if (Player[playerid][Medic] == 0){
			UpdateHotwireInfo(playerid);	
		}
	}
}
if (htimeleft <= 0){
	FinishHotwireTimeLeft();
}
Debug("hotwire.inc > HotwireTickTimer - Stop");
}

public FinishHotwire(playerid){
	Debug("hotwire.inc > FinishHotwire - Start");
/*	if (playerid==INVALID_PLAYER_ID){
		event_now = false; 
		KillTimer(htimelefttimer); 
		for(new i = 0; i <= MAX_PLAYERS_EX; i++){
			if(IsPlayerConnected(i)){
				if (Player[i][Medic] == 0){
					oSetVehicleParamsForPlayer(stealcar,i,0,0);
					ResetQuest(i); 
					StopShowingHotwireInfo(playerid);
					SystemMsg(i,gettext(192));
				}
			}
		}
		SetVehicleToRespawn(stealcar),
		stealcar=INVALID_VEHICLE_ID;
		stealcardest=INVALID_VEHICLE_ID;
		cwx=0.0;cwy=0.0;cwz=0.0;
		hreward=0;
		htimeleft = 0;
		Debug("hotwire.inc > FinishHotwire - Stop");
		return 0;
	}*/
	oGivePlayerMoney(playerid,hreward,0);
	ResetQuest(playerid);
	new string[256];
	if (Player[playerid][Cop] == 1){
	Player[playerid][HotwiresCop]++;
	format(string,sizeof(string),gettext(92),oGetPlayerName(playerid));
	Player[playerid][CopSkill]=Player[playerid][CopSkill]+10;
	CheckCopSkill(playerid);
	} else {
	Player[playerid][Hotwires]++;
	format(string, sizeof(string),gettext(93), oGetPlayerName(playerid),hreward);
	Player[playerid][Crimes]=Player[playerid][Crimes]+13;
	CheckCurrentCrimes(playerid);
	new copstring[MAX_STRING];
	format(copstring,sizeof(copstring),gettext(94),oGetPlayerName(playerid));
	SendMsgToCops(copstring);
	}
	for(new i = 0; i <= MAX_PLAYERS_EX; i++)
	{
		if(IsPlayerConnected(i))
		{
			if (Player[playerid][Medic] == 0){
				oSetVehicleParamsForPlayer(stealcar,i,0,0);
				ResetQuest(i);
				SystemMsg(i,string);
			}
		}
	}
	
	SetVehicleToRespawn(stealcar);
	htimeleft = 0;
	event_now = false;
	stealcar = INVALID_VEHICLE_ID;
	stealcardest = INVALID_VEHICLE_ID;
	cwx = 0.0;
	cwy = 0.0;
	cwz = 0.0;
	hreward = 0;
	KillTimer(htimelefttimer);
	StopShowingHotwireInfo(playerid);
	Debug("hotwire.inc > FinishHotwire - Stop");
	return 0;
}

public FinishHotwireTimeLeft(){
	Debug("hotwire.inc > FinishHotwireTimeLeft - Start");
	KillTimer(htimelefttimer);
	for(new i = 0; i <= MAX_PLAYERS_EX; i++)
	{
		if(IsPlayerConnected(i))
		{
			if (Player[i][Medic] == 0){
				oSetVehicleParamsForPlayer(stealcar,i,0,0);
				ResetQuest(i);
				SystemMsg(i,gettext(192));
				StopShowingHotwireInfo(i);
			}
		}
	}
	
	SetVehicleToRespawn(stealcar);
	htimeleft = 0;
	event_now = false;
	stealcar = INVALID_VEHICLE_ID;
	stealcardest = INVALID_VEHICLE_ID;
	cwx = 0.0;
	cwy = 0.0;
	cwz = 0.0;
	hreward = 0;
	Debug("hotwire.inc > FinishHotwireTimeLeft - Stop");
	return 0;
}

public Hotwire_OnVehicleDeath(vehicleid,killerid){
	Debug("hotwire.inc > Hotwire_OnVehicleDeath - Start");
	if (vehicleid == stealcar){
		new string[256];
		format(string,sizeof(string),gettext(95));
		for(new i = 0; i <= MAX_PLAYERS_EX; i++)
		{
			if(IsPlayerConnected(i))
			{
				if (Player[i][Medic] == 0){
					oSetVehicleParamsForPlayer(stealcar,i,0,0);
					SystemMsg(i,string);
					ResetQuest(i);
					StopShowingHotwireInfo(i);
				}
			}
		}
		SetVehicleToRespawn(stealcar);
		stealcar = INVALID_VEHICLE_ID;
		stealcardest = INVALID_VEHICLE_ID;
		cwx = 0.0;
		cwy = 0.0;
		cwz = 0.0;
		hreward = 0;
		event_now = false;
		htimeleft = 0;
		KillTimer(htimelefttimer);
	}
	Debug("hotwire.inc > Hotwire_OnVehicleDeath - Stop");
	return 0;
}

public Hotwire_OnPlayerConnect(playerid){
	Debug("hotwire.inc > Hotwire_OnPlayerConnect - Start");
	if (stealcar != INVALID_VEHICLE_ID){
		PlayerQuest[playerid]=quest_hotwire;
		oSetVehicleParamsForPlayer(stealcar,playerid,1,0);
	}
	if (stealcardest != INVALID_VEHICLE_ID){
		PlayerQuest[playerid]=quest_hotwire;
		oSetPlayerCheckpoint(playerid,cwx, cwy, cwz, 8.0);
	}
	if (stealcardest != INVALID_VEHICLE_ID && stealcar != INVALID_VEHICLE_ID) ShowHotwireInfo(playerid);
	Debug("hotwire.inc > Hotwire_OnPlayerConnect - Stop");
}

stock StopShowingHotwireInfo(playerid){
	if(Hotwire_TextDrawExists[playerid]){
	    TextDrawDestroy(Hotwire_TextDraw[playerid]);
	    Hotwire_TextDrawExists[playerid] = 0;
	}
}

stock ShowHotwireInfo(playerid){
	new string[MAX_STRING];
	format(string,MAX_STRING,gettext(197),ConvertSeconds(htimeleft));
	Hotwire_TextDraw[playerid] = TextDrawCreate(634.0, 360.0, string);
	TextDrawSetShadow(Hotwire_TextDraw[playerid], 0);
	TextDrawAlignment(Hotwire_TextDraw[playerid],3);
	TextDrawSetOutline(Hotwire_TextDraw[playerid], 1);
	TextDrawLetterSize(Hotwire_TextDraw[playerid], 0.4, 1.4);
	TextDrawFont(Hotwire_TextDraw[playerid], 2);
	Hotwire_TextDrawExists[playerid] = 1;
	TextDrawShowForPlayer(playerid, Hotwire_TextDraw[playerid]);
}

stock UpdateHotwireInfo(playerid){
	new string[MAX_STRING];
	format(string,MAX_STRING,gettext(197),ConvertSeconds(htimeleft));
	TextDrawSetString(Hotwire_TextDraw[playerid],string);
}