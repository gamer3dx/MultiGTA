//Anti-Idle module
//Created by Iain Gilbert
//Modder: MadCat

forward AntiIdle_OnGameModeInit();
forward AntiIdle();

new TimerAntiIdle;

new Anti_Idle_Enabled=1;
new Action_On_MaxIdleTime=2;
new Idle_Ban_Hours=24;
new Idle_Minutes_To_Punish=5;

new playerIdle[MAX_PLAYERS];
new Float:playerIdlePos[MAX_PLAYERS][CoordInfo];

public AntiIdle_OnGameModeInit(){
	AntiIdleConfig();
	if (Anti_Idle_Enabled == 1){
		TimerAntiIdle = SetTimer("AntiIdle",10000,1);
		WriteLog("Anti Idle Loaded");
	}
	return 0;
}

AntiIdleConfig()
{
	Debug("protections/idle.inc > AntiIdleConfig - Start");
	new temp[MAX_STRING];
	if (!db_Exists(SecurityDB)) db_Create(SecurityDB);

	set(temp,db_Get(SecurityDB,"Anti_Idle_Enabled"));
	if (strlen(temp) > 0) Anti_Idle_Enabled = strval(temp);
	else { valstr(temp,Anti_Idle_Enabled); db_Set(SecurityDB,"Anti_Idle_Enabled",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Action_On_MaxIdleTime"));
	if (strlen(temp) > 0) Action_On_MaxIdleTime = strval(temp);
	else { valstr(temp,Action_On_MaxIdleTime); db_Set(SecurityDB,"Action_On_MaxIdleTime",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Idle_Ban_Hours"));
	if (strlen(temp) > 0) Idle_Ban_Hours = strval(temp);
	else { valstr(temp,Idle_Ban_Hours); db_Set(SecurityDB,"Idle_Ban_Hours",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Idle_Minutes_To_Punish"));
	if (strlen(temp) > 0) Idle_Minutes_To_Punish = strval(temp);
	else { valstr(temp,Idle_Minutes_To_Punish); db_Set(SecurityDB,"Idle_Minutes_To_Punish",temp);  }
	set(temp,nullstr);
	
	Debug("protections/idle.inc > AntiIdleConfig - Stop");
	return;
}

public AntiIdle()
{
Debug("protections/idle.inc > AntiIdle - Start");
for(new playerid=0; playerid<MAX_PLAYERS_EX; playerid++)
{
  	if(IsPlayerConnected(playerid) && PlayerPriv[playerid][IsModerator] == 0 && !IsPlayerAdmin(playerid))
	{
        if(PlayerPos[playerid][Coord_X] == playerIdlePos[playerid][Coord_X] && PlayerPos[playerid][Coord_Y] == playerIdlePos[playerid][Coord_Y] && PlayerPos[playerid][Coord_Z] == playerIdlePos[playerid][Coord_Z])
		{
			playerIdle[playerid]++;
			if(playerIdle[playerid] > (Idle_Minutes_To_Punish * 6) - 1)
			{
				if (Action_On_MaxIdleTime == 1){
					SystemMsg(playerid,COLOUR_ERROR,gettext(505));
				} else if (Action_On_MaxIdleTime == 2){
               				SystemMsg(playerid,COLOUR_ERROR,gettext(505));
					SystemMsg(playerid,COLOUR_ERROR,gettext(506));
				} else if (Action_On_MaxIdleTime == 3){
               				SystemMsg(playerid,COLOUR_ERROR,gettext(505));
					SystemMsg(playerid,COLOUR_ERROR,gettext(507));
				}
			}        
			if(playerIdle[playerid] > (Idle_Minutes_To_Punish * 6))
			{
				playerIdle[playerid] = 0;
				if (Action_On_MaxIdleTime == 0) continue;
				if (Action_On_MaxIdleTime == 1) Report(playerid,gettext(508));
				if (Action_On_MaxIdleTime == 2) KickPlayer(playerid,-1,gettext(508));
				if (Action_On_MaxIdleTime == 3)	BanPlayer(playerid,Idle_Ban_Hours,gettext(508));
			}
		}
        else
		{
               		playerIdle[playerid] = 0;
			playerIdlePos[playerid][Coord_X] = PlayerPos[playerid][Coord_X];
			playerIdlePos[playerid][Coord_Y] = PlayerPos[playerid][Coord_Y];
			playerIdlePos[playerid][Coord_Z] = PlayerPos[playerid][Coord_Z];
       		}
	}
}
Debug("protections/idle.inc > AntiIdle - Stop");
}


