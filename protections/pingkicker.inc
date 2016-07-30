//Anti-HighPing Module
//Created by MadCat

forward PingKicker_OnGameModeInit();
forward PingKickerConfig();
forward pinglimit_timer();

new TimerAntiPing;

new PingKicker_Enabled=1;
new Max_Ping=1000;
new Max_HighPingTime=10;

new HighPing[MAX_PLAYERS];

public PingKicker_OnGameModeInit(){
	PingKickerConfig();
	if (PingKicker_Enabled == 1){
		TimerAntiPing = SetTimer("pinglimit_timer",1000,1);
		WriteLog("Anti High-Ping Loaded");
	}
	return 0;
}

public PingKickerConfig()
{
	Debug("protections/pingkicker.inc > PingKickerConfig - Start");
	if (!db_Exists(SecurityDB)) db_Create(SecurityDB);

	new temp[MAX_STRING];
	
	set(temp,db_Get(SecurityDB,"PingKicker_Enabled"));
	if (strlen(temp) > 0) PingKicker_Enabled = strval(temp); 
	else { valstr(temp,PingKicker_Enabled); db_Set(SecurityDB,"PingKicker_Enabled",temp); }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Max_Ping"));
	if (strlen(temp) > 0) Max_Ping = strval(temp); 
	else { valstr(temp,Max_Ping); db_Set(SecurityDB,"Max_Ping",temp); }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Max_HighPingTime"));
	if (strlen(temp) > 0) Max_HighPingTime = strval(temp); 
	else { valstr(temp,Max_HighPingTime); db_Set(SecurityDB,"Max_HighPingTime",temp);  }
	set(temp,nullstr);

	Debug("protections/pingkicker.inc > PingKickerConfig - Stop");
}

public pinglimit_timer(){
	Debug("protections/pingkicker.inc > pinglimit_timer - Start");
	for(new i=0;i<MAX_PLAYERS_EX;i++){
		if(IsPlayerConnected(i) && PlayerPriv[i][IsModerator] == 0 && !IsPlayerAdmin(i)){
			if(GetPlayerPing(i) > Max_Ping){
				HighPing[i]++;
			} else {
				HighPing[i]=0;
			}
			if(HighPing[i] >= Max_HighPingTime){
				SystemMsg(i,COLOUR_ERROR,gettext(509));
				HighPing[i]=0;
				KickPlayer(i,-1,gettext(510));
			}
		}
	}
	Debug("protections/pingkicker.inc > pinglimit_timer - Stop");
}