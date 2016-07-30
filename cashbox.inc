//Cashbox Minigame Module
//Created by MadCat
//Idea from PEN1
//Coords from SF Fugitive, PENLS, PENLV

#include "jobs\cop"

new Float:cashbox_startx,Float:cashbox_starty,Float:cashbox_startz;
new Float:cashbox_finishx,Float:cashbox_finishy,Float:cashbox_finishz;
new cashbox_finname[MAX_STRING];
new cashbox_object;
new cashbox_reward = 0;
new cashbox_cpid;

new Float:CashBoxFinish[][CoordInfo] = {
{2217.2,-1179.7,29.7}, //LS
{1098.1,-1428.0,22.7},
{2484.2,-1961.8,16.7},
{733.2,-1357.9,23.5},
{1411.6,-1455.1,20.4},
{1080.6,-1605.0,20.4},
{382.0,-2058.7,10.7},
{656.1,-1646.6,15.4},
{816.2,-1380.5,23.5},
{853.3,-1388.2,-0.5},
{915.4,-1235.1,17.2},
{1153.7,-1180.4,32.8},
{1183.9,-1257.3,18.8},
{1441.3,-926.8,39.6},
{1527.5,-893.5,57.6},
{1342.1,-1624.3,17.7},
{1292.1,-770.7,95.9},
{1907.9,-1318.6,14.1},
{1971.7,-1284.5,28.4},
{2687.1,-1144.6,71.3},
{2654.1,-1317.5,58.9},
{2771.0,-1388.5,27.1},
{2793.3,-1945.0,17.3},
{197.9,-1206.0,78.3},
{2447.5,-2209.8,26.6},
{2512.4,-2072.9,26.5},
{2171.6,-1732.1,17.2},
{2175.4,-1836.1,16.1},
{2399.5,-1550.7,28.0},
{2109.2,-1010.2,63.8},
{1409.9,-1274.5,33.4},
{1279.8,-1439.0,20.4},
{2234.0,-2269.3,17.8},
{2025.8,-2133.1,19.8},
{811.2,-1098.2,25.9},
{818.2,-944.2,55.2},
{1976.8,-1779.4,17.3},
{-1531.62,687.50,133.05}, //SF
{-2116.69,925.64,86.08},
{-2178.19,714.87,53.89},
{-2079.53,1066.02,65.57},
{-2708.04,378.05,11.98},
{-1474.37,152.15,18.77},
{-1320.62,485.88,11.19},
{-2659.46,1530.08,54.94},
{-2657.78,1414.17,27.97},
{-2807.55,1162.87,20.31},
{-2387.21,2215.94,4.98},
{-2119.53,224.59,35.10},
{-2027.70,-42.67,38.80},
{-1948.38,111.97,27.69},
{-1808.22,560.85,35.16},
{-1940.65,486.94,31.97},
{-1571.32,703.71,26.46},
{-1599.16,683.78,17.16},
{-1610.15,-493.28,22.09},
{-1429.51,-964.76,200.91},
{-1685.92,1034.85,56.91},
{-1643.64,1206.49,32.91},
{-1505.59,1374.12,3.83},
{-2187.07,607.10,35.16},
{-2222.01,615.84,44.24},
{-2435.86,544.59,28.36},
{-2525.08,310.48,35.12},
{-1812.34,1226.70,23.98},
{-1864.90,1281.93,62.48},
{-2483.35,164.50,32.06},
{-1650.97,1422.88,9.22},
{-2582.19,893.12,74.48},
{-2661.18,874.05,84.38},
{-1465.60,408.74,7.19},
{-2734.98,769.54,65.91},
{-2402.25,-597.65,132.65},
{-2432.76,-1620.05,526.86},
{2498.9,1152.0,25.3}, //LV
{2360.0,1681.2,14.2},
{2000.6,1521.9,17.0},
{2270.9,1388.7,42.8},
{2308.6,1303.6,68.4},
{2155.6,1285.7,7.5},
{1953.2,1343.0,15.3},
{2265.7,2034.4,14.2},
{2225.9,2354.5,14.9},
{2058.3,2183.5,13.5},
{1722.6,1816.0,10.8},
{1679.9,1760.7,10.8},
{1961.6,2324.1,16.3},
{1595.6,2312.4,18.8},
{1969.5,1343.0,20.6},
{2323.7,1283.3,97.5},
{2534.8,1386.1,10.8},
{2550.8,1045.4,13.8},
{2518.0,918.4,20.8},
{2220.4,1048.5,10.8},
{2007.8,1079.8,15.5}
};

new CashBoxStart[][CoordInfo] = {
{997.7,-921.1,42.1}, //LS
{2421.5,-1221.7,25.3},
{2069.7,-1780.1,13.5},
{1352.4,-1756.7,13.5},
{1831.1,-1842.8,13.5},
{2398.2,-1897.9,13.5},
{2400.5,-1979.7,13.5},
{2073.9,-1828.3,13.5},
{2073.3,-1794.5,13.5},
{925.2,-1353.2,13.3},
{813.3,-1616.3,13.5},
{492.2,-1733.7,11.2},
{361.0,-2033.5,7.8},
{1199.0,-920.7,43.0},
{1968.6,-1177.9,20.0},
{-2418.83,969.51,45.30}, //SF
{-1498.02,925.16,7.19},
{-2444.78,753.17,35.17},
{-2453.94,504.07,30.08},
{-2494.48,-29.11,25.77},
{-2625.85,209.14,4.62},
{-1689.99,432.77,7.19},
{-1422.02,-288.76,14.15},
{-1055.1,-696.30,32.35},
{-1816.50,614.57,35.17},
{-1911.46,829.30,35.17},
{-2356.83,1005.78,50.89},
{-2626.80,1408.64,7.09},
{-1968.74,295.80,35.17},
{-2100.48,-961.34,32.17},
{-2522.09,-623.25,132.76},
{-2720.67,-318.12,7.84},
{-2672.49,261.17,4.63},
{-2627.56,208.75,4.57},
{2080.4,2120.6,10.8}, //LV
{2083.5,2222.4,10.8},
{1517.3,1066.9,10.8},
{2156.4,943.2,10.8},
{2470.9,2034.9,11.0},
{1674.7,1448.0,10.7},
{2193.4,1676.4,12.3},
{2637.4,1127.2,11.1},
{2415.5,1124.0,10.8},
{1458.2,2773.5,10.8}
};

forward cashboxconfig();
forward CashBoxTimer();
forward StartCashBox();
forward CashboxEndNoWinner();
forward CashBox_OnGameModeInit();
forward CashBox_OnPlayerEnterCheckpoint(playerid);
forward CashBox_OnPlayerDeath(playerid);
forward CashBox_OnPlayerDisconnect(playerid);

new Players_To_CashBox = 6;

new TimerCashbox;

public CashBox_OnGameModeInit(){
	Debug("cashbox.inc > CashBox_OnGameModeInit - Start");
	cashboxconfig();
    	TimerCashbox = SetTimer("CashBoxTimer", 840000, 1);
	WriteLog("CashBox Minigame Loaded");
	Debug("cashbox.inc > CashBox_OnGameModeInit - Stop");
	return 0;
}

public cashboxconfig()
{
	Debug("cashbox.inc > cashboxconfig - Start");
	if (!db_Exists(ConfigDB)) db_Create(ConfigDB);

	new temp[MAX_STRING];
	
	set(temp,db_Get(ConfigDB,"Players_To_CashBox"));
	if (strlen(temp) > 0) Players_To_CashBox = strval(temp); 
	else { valstr(temp,Players_To_CashBox); db_Set(ConfigDB,"Players_To_CashBox",temp); }
	set(temp,nullstr);
	Debug("cashbox.inc > cashboxconfig - Stop");
}

public CashBoxTimer() // must be ran by timer every.... as you wish :)
{
	Debug("cashbox.inc > CashBoxTimer - Start");
	if (event_now == false){
		if (PlayerCount() >= Players_To_CashBox){
			StartCashBox();
		}
	}
	Debug("cashbox.inc > CashBoxTimer - Start");

}

public StartCashBox(){
	Debug("cashbox.inc > StartCashBox - Start");
	new zonename[MAX_STRING];
	new rnd=random(sizeof(CashBoxStart));
	cashbox_startx=CashBoxStart[rnd][Coord_X];
	cashbox_starty=CashBoxStart[rnd][Coord_Y];
	cashbox_startz=CashBoxStart[rnd][Coord_Z];
	rnd=random(sizeof(CashBoxFinish));
	cashbox_finishx=CashBoxFinish[rnd][Coord_X];
	cashbox_finishy=CashBoxFinish[rnd][Coord_Y];
	cashbox_finishz=CashBoxFinish[rnd][Coord_Z];
	format(zonename,MAX_STRING,"%s (%s)",GetXYZZoneName(cashbox_startx,cashbox_starty,cashbox_startz),GetXYZCityName(cashbox_startx,cashbox_starty,cashbox_startz));
	set(cashbox_finname,zonename);
	event_now = true;
	for(new i = 0; i <= MAX_PLAYERS_EX; i++)
	{
		if(IsPlayerConnected(i))
		{
			if (Player[i][GotJob] == JOB_COP){
				ShowTextForPlayer(i,gettext(96), 5000, 1);
				new string[256];
				format(string,sizeof(string),gettext(97),cashbox_finname,ConvertSeconds(ONE_SA_DAY/1000));
				SystemMsg(i,COLOUR_INFO,string);
			} else if (Player[i][GotJob] == JOB_MEDIC){
				// do nothing...
			} else {
				ShowTextForPlayer(i, gettext(96), 5000, 1);
				new string[256];
				format(string,sizeof(string),gettext(98),cashbox_finname,ConvertSeconds(ONE_SA_DAY/1000));
				SystemMsg(i,COLOUR_INFO,string);
			}
		}
	}
	cashbox_cpid = AddActiveCheckpoint(cashbox_startx,cashbox_starty,cashbox_startz,10000,2);
	cashbox_object = CreateStreamObject(3013,cashbox_startx,cashbox_starty,cashbox_startz,0,0,0,-1);
	SetTimer("CashboxEndNoWinner",ONE_SA_DAY,0);
	Debug("cashbox.inc > StartCashBox - Stop");
}

public CashBox_OnPlayerEnterCheckpoint(playerid)
{
	if (Player[playerid][GotJob] == JOB_MEDIC) return 0;
	Debug("cashbox.inc > CashBox_OnPlayerEnterCheckpoint - Start");
	if (oIsPlayerInCheckpoint(playerid,cashbox_startx,cashbox_starty,cashbox_startz,10)){
		if (cashbox_reward == 0){
			cashbox_reward = random(35000);
			while (cashbox_reward < 15000){
				cashbox_reward = random(35000);
			}
			new distance = GetPlayerDistanceToPointEx(playerid,cashbox_finishx,cashbox_finishy,cashbox_finishz);
			if (distance > 4000){
				cashbox_reward = cashbox_reward * 5;
			} else if (distance > 3000){
				cashbox_reward = cashbox_reward * 4;
			} else if (distance > 2000){
				cashbox_reward = cashbox_reward * 3;
			} else if (distance > 1000){
				cashbox_reward = cashbox_reward * 2;
			}
		}
		DestroyStreamObject(cashbox_object);
		cashbox_object = 0;
		new string[MAX_STRING];
		new rnd;
		if (Player[playerid][GotJob] == JOB_COP){
			rnd = random(CopJobPlaces);
			cashbox_finishx=CopJobPlace[rnd][Coord_X];
			cashbox_finishy=CopJobPlace[rnd][Coord_Y];
			cashbox_finishz=CopJobPlace[rnd][Coord_Z];
			format(string,sizeof(string),gettext(100),oGetPlayerName(playerid),cashbox_reward);
			ShowTextForPlayer(playerid,gettext(101), 5000, 1);
			SetSpecialColorForPlayer(playerid);
			PlayerWithCashBox = playerid;
		} else {
			rnd=random(sizeof(CashBoxFinish));
			cashbox_finishx=CashBoxFinish[rnd][Coord_X];
			cashbox_finishy=CashBoxFinish[rnd][Coord_Y];
			cashbox_finishz=CashBoxFinish[rnd][Coord_Z];
			format(string,sizeof(string),gettext(102),oGetPlayerName(playerid),cashbox_reward);
			ShowTextForPlayer(playerid,gettext(103), 5000, 1);
			SetSpecialColorForPlayer(playerid);
			PlayerWithCashBox = playerid;
			SetPlayerCriminal(playerid,gettext(498),3);
		}
		for(new i = 0; i <= MAX_PLAYERS_EX; i++)
		{
			if (IsPlayerConnected(i)){
				if (Player[i][GotJob] != JOB_MEDIC){
					SystemMsg(i,COLOUR_INFO,string);
				}
			}
		}
		RemoveActiveCheckpoint(cashbox_cpid);
		cashbox_cpid = AddActiveCheckpoint(cashbox_finishx,cashbox_finishy,cashbox_finishz,10000,2);
	} 
	if (oIsPlayerInCheckpoint(playerid,cashbox_finishx,cashbox_finishy,cashbox_finishz,10) && PlayerWithCashBox == playerid){
		new string[256];
		if (Player[playerid][GotJob] == JOB_COP){
			format(string,sizeof(string),gettext(104),oGetPlayerName(playerid),cashbox_reward);
			Player[playerid][CopSkill]=Player[playerid][CopSkill]+10;
			Player[playerid][CashBoxCop]++;
		} else {
			format(string,sizeof(string),gettext(105),oGetPlayerName(playerid),cashbox_reward);
			SetPlayerCriminal(playerid,gettext(628),13);
			Player[playerid][CashBox]++;
			new copstring[MAX_STRING];
			format(copstring,sizeof(copstring),gettext(106),oGetPlayerName(playerid));
			SendMsgToCops(copstring);
		}
		oGivePlayerMoney(playerid,cashbox_reward,0);
		cashbox_startx=0.0,cashbox_starty=0.0,cashbox_startz=0.0,cashbox_finishx=0.0,cashbox_finishy=0.0,cashbox_finishz=0.0;
		set(cashbox_finname,nullstr);
		cashbox_reward = 0;
		PlayerWithCashBox = INVALID_PLAYER_ID;
		SetSpecialColorForPlayer(playerid);
		event_now = false;
		DestroyStreamObject(cashbox_object);
		cashbox_object = 0;
		RemoveActiveCheckpoint(cashbox_cpid);
		for(new i = 0; i <= MAX_PLAYERS_EX; i++)
		{
			if (IsPlayerConnected(i)){
				if (Player[i][GotJob] != JOB_MEDIC){
					SystemMsg(i,COLOUR_INFO,string);
				}
			}
		}
	}
	Debug("cashbox.inc > CashBox_OnPlayerEnterCheckpoint - Stop");
	return 0;
}

public CashboxEndNoWinner(){
	if (event_now == true && cashbox_object != 0){
		cashbox_startx=0.0,cashbox_starty=0.0,cashbox_startz=0.0,cashbox_finishx=0.0,cashbox_finishy=0.0,cashbox_finishz=0.0;
		set(cashbox_finname,nullstr);
		cashbox_reward = 0;
		PlayerWithCashBox = INVALID_PLAYER_ID;
		event_now = false;
		DestroyStreamObject(cashbox_object);
		cashbox_object = 0;
		RemoveActiveCheckpoint(cashbox_cpid);
		for(new i = 0; i <= MAX_PLAYERS_EX; i++)
		{
			if (IsPlayerConnected(i)){
				if (Player[i][GotJob] != JOB_MEDIC){
					SystemMsg(i,COLOUR_INFO,gettext(1741));
				}
			}
		}
	}
}

public CashBox_OnPlayerDeath(playerid){
	if (PlayerWithCashBox != playerid) return 0;
	Debug("cashbox.inc > CashBox_OnPlayerDeath - Start");
	PlayerWithCashBox = INVALID_PLAYER_ID;
	new string[MAX_STRING];
	format(string,sizeof(string),gettext(107),oGetPlayerName(playerid));
	cashbox_startx = PlayerPos[playerid][Coord_X];
	cashbox_starty = PlayerPos[playerid][Coord_Y];
	cashbox_startz = PlayerPos[playerid][Coord_Z];
	cashbox_object = CreateStreamObject(3013,cashbox_startx,cashbox_starty,cashbox_startz,0,0,0,-1);
	for(new i = 0; i <= MAX_PLAYERS_EX; i++)
	{
		if (IsPlayerConnected(i)){
			if (Player[i][GotJob] != JOB_MEDIC){
				SystemMsg(i,COLOUR_INFO,string);
			}
		}
	}
	RemoveActiveCheckpoint(cashbox_cpid);
	cashbox_cpid = AddActiveCheckpoint(cashbox_startx,cashbox_starty,cashbox_startz,10000,2);
	Debug("cashbox.inc > CashBox_OnPlayerDeath - Stop");
	return 0;
}

public CashBox_OnPlayerDisconnect(playerid){
	if (PlayerWithCashBox != playerid) return 0;
	Debug("cashbox.inc > CashBox_OnPlayerDisconnect - Start");
	PlayerWithCashBox = INVALID_PLAYER_ID;
	new string[MAX_STRING];
	format(string,sizeof(string),gettext(108),oGetPlayerName(playerid));
	cashbox_startx = PlayerPos[playerid][Coord_X];
	cashbox_starty = PlayerPos[playerid][Coord_Y];
	cashbox_startz = PlayerPos[playerid][Coord_Z];
	cashbox_object = CreateStreamObject(3013,cashbox_startx,cashbox_starty,cashbox_startz,0,0,0,-1);
	for(new i = 0; i <= MAX_PLAYERS_EX; i++)
	{
		if (IsPlayerConnected(i)){
			if (Player[i][GotJob] != JOB_MEDIC){
				SystemMsg(i,COLOUR_INFO,string);
			}
		}
	}
	RemoveActiveCheckpoint(cashbox_cpid);
	cashbox_cpid = AddActiveCheckpoint(cashbox_startx,cashbox_starty,cashbox_startz,10000,2);
	Debug("cashbox.inc > CashBox_OnPlayerDisconnect - Stop");
	return 0;
}