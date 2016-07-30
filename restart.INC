forward RestartConfig();
forward RestartCheck();
forward restart_OnGameModeInit();
forward restart_OnPlayerConnect(playerid);

new Autorestart_Enabled=0;

new TimerRestart;

new bool:RestartNeeded;

public RestartConfig()
{
	Debug("restart.inc > RestartConfig - Start");
	
	if (!db_Exists(ConfigDB)) db_Create(ConfigDB);
	new temp[MAX_STRING];
	set(temp,db_Get(ConfigDB,"Autorestart_Enabled"));
	if (strlen(temp) > 0) Autorestart_Enabled = strval(temp); 
	else { valstr(temp,Autorestart_Enabled); db_Set(ConfigDB,"Autorestart_Enabled",temp); }
	set(temp,nullstr);
	Debug("restart.inc > RestartConfig - Stop");
	return;
}

public RestartCheck()
{
    Debug("restart.inc > RestartCheck - Stop");
    if ((PlayerCount() == 0) && (RestartNeeded == true)) GameModeExit();
    Debug("restart.inc > RestartCheck - Stop");
}

public restart_OnGameModeInit()
{
	Debug("restart.inc > restart_OnGameModeInit - Stop");
	RestartConfig();
	if (Autorestart_Enabled == 1){
    		TimerRestart = SetTimer("RestartCheck",300000, 1);
		RestartNeeded = false;
		WriteLog("Restarter Loaded!");
	}
	Debug("restart.inc > restart_OnGameModeInit - Stop");
	return 0;
}

public restart_OnPlayerConnect(playerid)
{
	Debug("restart.inc > restart_OnPlayerConnect - Stop");
	if (Autorestart_Enabled){
		RestartNeeded = true;
	}
	Debug("restart.inc > restart_OnPlayerConnect - Stop");
	return 0;
}