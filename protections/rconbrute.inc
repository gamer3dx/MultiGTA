//Anti-HighPing Module
//Created by MadCat

forward rconbrute_OnGameModeInit();
forward rconbruteConfig();
forward rconbrute_OnRconLoginAttempt(ip[], password[], success);

new Anti_Rcon_Brute_Enabled = 1;
new Max_Rcon_Brute_Attempts = 3;
new Action_On_Max_Rcon_Brute = 2;
new Rcon_Brute_Ban_Hours = 168;

new RconBruteAttempts[MAX_PLAYERS];

public rconbrute_OnGameModeInit(){
	rconbruteConfig();
	if (Anti_Rcon_Brute_Enabled == 1) WriteLog("Anti Rcon Bruteforce Loaded");
	return 0;
}

public rconbruteConfig()
{
	Debug("protections/rconbrute.inc > rconbruteConfig - Start");
	if (!db_Exists(SecurityDB)) db_Create(SecurityDB);

	new temp[MAX_STRING];
	
	set(temp,db_Get(SecurityDB,"Anti_Rcon_Brute_Enabled"));
	if (strlen(temp) > 0) Anti_Rcon_Brute_Enabled = strval(temp); 
	else { valstr(temp,Anti_Rcon_Brute_Enabled); db_Set(SecurityDB,"Anti_Rcon_Brute_Enabled",temp); }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Max_Rcon_Brute_Attempts"));
	if (strlen(temp) > 0) Max_Rcon_Brute_Attempts = strval(temp); 
	else { valstr(temp,Max_Rcon_Brute_Attempts); db_Set(SecurityDB,"Max_Rcon_Brute_Attempts",temp); }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Action_On_Max_Rcon_Brute"));
	if (strlen(temp) > 0) Action_On_Max_Rcon_Brute = strval(temp);
	else { valstr(temp,Action_On_Max_Rcon_Brute); db_Set(SecurityDB,"Action_On_Max_Rcon_Brute",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Rcon_Brute_Ban_Hours"));
	if (strlen(temp) > 0) Rcon_Brute_Ban_Hours = strval(temp);
	else { valstr(temp,Rcon_Brute_Ban_Hours); db_Set(SecurityDB,"Rcon_Brute_Ban_Hours",temp);  }
	set(temp,nullstr);

	if (Max_Rcon_Brute_Attempts == 0) Max_Rcon_Brute_Attempts = 1;

	Debug("protections/rconbrute.inc > rconbruteConfig - Stop");
}

public rconbrute_OnRconLoginAttempt(ip[], password[], success){
	Debug("protections/rconbrute.inc > rconbrute_OnRconLoginAttempt - Start");
	if (Anti_Rcon_Brute_Enabled == 1){
    		if(!success) 
    		{
        		for(new playerid=0; playerid<MAX_PLAYERS_EX; playerid++)
        		{
            			if(strcomp(ip, ReturnPlayerIp(playerid), true))
            			{
					if (PlayerPriv[playerid][IsModerator] == 0){
						RconBruteAttempts[playerid]++;
						if(RconBruteAttempts[playerid] >= Max_Rcon_Brute_Attempts)
						{
							RconBruteAttempts[playerid]=0;
							if (Action_On_Max_Rcon_Brute == 0) return 0;
							if (Action_On_Max_Rcon_Brute == 1) Report(playerid,gettext(1782));
							if (Action_On_Max_Rcon_Brute == 2) KickPlayer(playerid,-1,gettext(1782));
							if (Action_On_Max_Rcon_Brute == 3) BanPlayer(playerid,Rcon_Brute_Ban_Hours,gettext(1782));
            					}
					}
        			}
    			}
		}
	}
	Debug("protections/rconbrute.inc > rconbrute_OnRconLoginAttempt - Stop");
	return 0;
}