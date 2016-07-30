//Anti-Reconndect module
//Created by GTAT Dev Team
//Converted for MultiGTA by MadCat

forward Reconnect_OnGameModeInit();
forward Reconnect_OnPlayerConnect(playerid);
forward Reconnect_OnPlayerDisconnect(playerid,reason);

new PlayerIP[MAX_PLAYERS][MAX_STRING];

new AntiReconnect_Enabled=0;
new Reconnect_DB[MAX_STRING]="MultiGTA/OtherDB/Reconnects.txt";

ReconnectConfig()
{
	Debug("protections/reconnect.inc > ReconnectConfig - Start");
	

	if (!db_Exists(SecurityDB)) db_Create(SecurityDB);

	new temp[MAX_STRING];
	set(temp,db_Get(DatabaseDB,"Reconnect_DB"));
	if (strlen(temp) > 0) Reconnect_DB = temp; 
	else { set(temp,Reconnect_DB); db_Set(DatabaseDB,"Reconnect_DB",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"AntiReconnect_Enabled"));
	if (strlen(temp) > 0) AntiReconnect_Enabled = strval(temp); 
	else { valstr(temp,AntiReconnect_Enabled); db_Set(SecurityDB,"AntiReconnect_Enabled",temp);  }
	set(temp,nullstr);

	if (!db_Exists(Reconnect_DB)) db_Create(Reconnect_DB);

	Debug("protections/reconnect.inc > ReconnectConfig - Stop");
	return;
}

public Reconnect_OnGameModeInit()
{
	Debug("protections/reconnect.inc > Reconnect_OnGameModeInit - Start");
	ReconnectConfig();
	if (AntiReconnect_Enabled == 1) WriteLog("Anti-Reconnect Loaded");
	Debug("protections/reconnect.inc > Reconnect_OnGameModeInit - Stop");
	return;
}

public Reconnect_OnPlayerConnect(playerid){
	if (AntiReconnect_Enabled == 0) return 0;
	Debug("protections/reconnect.inc > Reconnect_OnPlayerConnect - Start"); 
	new ip[IP_LEN]; ip = ReturnPlayerIp(playerid);
	new timeval=strval(db_Get(Reconnect_DB,ip));
	format(PlayerIP[playerid],MAX_STRING,"%s",ip);
	if(timeval != 0){
	    	if(Now() < timeval){
			KickPlayer(playerid,-1,gettext(186));
			Debug("protections/reconnect.inc > Reconnect_OnPlayerConnect - Stop"); 
			return 1;
	    	} else {
			db_Unset(Reconnect_DB,ip);
            	}
	}
	Debug("protections/reconnect.inc > Reconnect_OnPlayerConnect - Stop"); 
	return 0;
}

public Reconnect_OnPlayerDisconnect(playerid,reason){
	if (AntiReconnect_Enabled == 0) return 0;
	Debug("protections/reconnect.inc > Reconnect_OnPlayerDisconnect - Start"); 
	new newtime = (Now()+120);
	if(reason == 0) newtime = (Now()+60);
	db_Set(Reconnect_DB,PlayerIP[playerid],ovalstr(newtime));
	Debug("protections/reconnect.inc > Reconnect_OnPlayerDisconnect - Stop"); 
	return 0;
}
