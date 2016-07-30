//Sync Time module
//Created by MadCat
//Part of Real Time code by Kye

forward SyncTime_OnGameModeInit();
forward SyncTime_OnPlayerDisconnect(playerid,reason);
forward SyncTime_OnPlayerDeath(playerid, killerid, reason);
forward SyncTime_OnPlayerSpawn(playerid);

forward SyncTimeConfig();
forward SyncRealTime();
forward SyncTime();

new Time_String[32];
new Text:TimeText;

new WorldTime;

new synctime_hour, synctime_minute;

new Real_Time = 1;

new TimerSyncTime;

public SyncTime_OnGameModeInit()
{
	Debug("synctime.inc > SyncTime_OnGameModeInit - Start");
	SyncTimeConfig();
	if (Real_Time == 1){
		TimeText = TextDrawCreate(605.0,25.0,"00:00");
		TextDrawUseBox(TimeText, 0);
		TextDrawFont(TimeText, 3);
		TextDrawSetShadow(TimeText,0); // no shadow
    		TextDrawSetOutline(TimeText,2); // thickness 1
    		TextDrawBackgroundColor(TimeText,0x000000FF);
    		TextDrawColor(TimeText,0xFFFFFFFF);
    		TextDrawAlignment(TimeText,3);
		TextDrawLetterSize(TimeText,0.5,1.5);
		SyncRealTime();
		TimerSyncTime = SetTimer("SyncRealTime",30000,1);
		WriteLog("Real time sync loaded");
	} else {
		WorldTime = 12;
		TimerSyncTime = SetTimer("SyncTime", 120000, 1);
		WriteLog("Time sync loaded");
	}
	Debug("synctime.inc > SyncTime_OnGameModeInit - Stop");
	return 0;
}

public SyncTime_OnPlayerDisconnect(playerid,reason)
{
	#pragma unused reason
	Debug("synctime.inc > SyncTime_OnPlayerDisconnect - Start");
	if (Real_Time == 1){
		gettime(synctime_hour, synctime_minute);
    		SetPlayerTime(playerid,synctime_hour,synctime_minute);
	}
	Debug("synctime.inc > SyncTime_OnPlayerDisconnect - Stop");
	return 0;
}

public SyncTime_OnPlayerSpawn(playerid)
{
	Debug("synctime.inc > SyncTime_OnPlayerSpawn - Start");
	if (Real_Time == 1){
		TextDrawShowForPlayer(playerid,TimeText);
		gettime(synctime_hour, synctime_minute);
		SetPlayerTime(playerid,synctime_hour,synctime_minute);
	}
	Debug("synctime.inc > SyncTime_OnPlayerSpawn - Stop");
	return 0;
}


public SyncTime_OnPlayerDeath(playerid, killerid, reason)
{
	#pragma unused killerid
	#pragma unused reason
    	TextDrawHideForPlayer(playerid,TimeText);
 	return 1;
}

public SyncTimeConfig()
{
	Debug("synctime.inc > SyncTimeConfig - Start");
	if (!db_Exists(ConfigDB)) db_Create(ConfigDB);

	new temp[MAX_STRING];
	
	set(temp,db_Get(ConfigDB,"Real_Time"));
	if (strlen(temp) > 0) Real_Time = strval(temp); 
	else { valstr(temp,Real_Time); db_Set(ConfigDB,"Real_Time",temp); }
	set(temp,nullstr);
	Debug("synctime.inc > SyncTimeConfig - Stop");
}

public SyncRealTime()
{
	gettime(synctime_hour, synctime_minute);
   	format(Time_String,32,"%02d:%02d",synctime_hour,synctime_minute);
   	TextDrawSetString(TimeText,Time_String);
   	
   	SetWorldTime(synctime_hour);
   	
	for (new playerid = 0; playerid < MAX_PLAYERS_EX; playerid++)
	{
	    	if(IsPlayerConnected(playerid) && GetPlayerState(playerid) != PLAYER_STATE_NONE) SetPlayerTime(playerid,synctime_hour,synctime_minute);
	}
}

public SyncTime()
{
	Debug("synctime.inc > SyncTime - Start");
	if (WorldTime >= 23)
	{
		WorldTime = 0;
	} else {
		WorldTime++;
	}
	SetWorldTime(WorldTime);
	Debug("synctime.inc > SyncTime - Stop");
}