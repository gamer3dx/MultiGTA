//Anti-CarJacking module
//Created by Double-O-Seven (JunkBuster Anti-Cheat)
//Converted and modified by MadCat

forward AntiCJ_OnGameModeInit();
forward AntiCJ_OnPlayerConnect(playerid);
forward AntiCJ_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger);
forward AntiCJ_OnPlayerStateChange(playerid, newstate, oldstate);

new Anti_CarJack_Enabled = 0;
new Action_On_CarJacks=2;
new Carjack_Ban_Hours=24;

new CurrentVehicle[MAX_PLAYERS];

AntiCarJackingConfig()
{
	new temp[MAX_STRING];
	if (!db_Exists(SecurityDB)) db_Create(SecurityDB);
	
	set(temp,db_Get(SecurityDB,"Anti_CarJack_Enabled"));
	if (strlen(temp) > 0) Anti_CarJack_Enabled = strval(temp);
	else { valstr(temp,Anti_CarJack_Enabled); db_Set(SecurityDB,"Anti_CarJack_Enabled",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Action_On_CarJacks"));
	if (strlen(temp) > 0) Action_On_CarJacks = strval(temp);
	else { valstr(temp,Action_On_CarJacks); db_Set(SecurityDB,"Action_On_CarJacks",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Carjack_Ban_Hours"));
	if (strlen(temp) > 0) Carjack_Ban_Hours = strval(temp);
	else { valstr(temp,Carjack_Ban_Hours); db_Set(SecurityDB,"Carjack_Ban_Hours",temp);  }
	set(temp,nullstr);

	return;
}


public AntiCJ_OnGameModeInit()
{
	AntiCarJackingConfig();
	if (Anti_CarJack_Enabled == 1)	WriteLog("Anti car-jacking Loaded");
	return 0;
}

public AntiCJ_OnPlayerConnect(playerid)
{
	CurrentVehicle[playerid]=INVALID_VEHICLE_ID;
	return 0;
}

public AntiCJ_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	CurrentVehicle[playerid]=vehicleid;
	return 0;
}

public AntiCJ_OnPlayerStateChange(playerid, newstate, oldstate)
{
	if (Anti_CarJack_Enabled == 0) return 0;
	#pragma unused oldstate
	new vehicleid=GetPlayerVehicleID(playerid);
	if(newstate==PLAYER_STATE_DRIVER && CurrentVehicle[playerid]!=vehicleid)
	{
		new bool:used = false;
		for(new i=0;i<MAX_PLAYERS_EX;i++)
		{
		    if(IsPlayerConnected(i) && i!=playerid)
		    {
		        if(GetPlayerState(i)==PLAYER_STATE_DRIVER)
		        {
		            if(GetPlayerVehicleID(i)==vehicleid)
		            {
		                used=true;
		                break;
		            }
		        }
			}
		}
		
		if(used)
		{
			if (Action_On_CarJacks == 0) return 0;
			if (Action_On_CarJacks == 1) Report(playerid,gettext(1773));
			if (Action_On_CarJacks == 2) KickPlayer(playerid,-1,gettext(1773));
			if (Action_On_CarJacks == 3) BanPlayer(playerid,Carjack_Ban_Hours,gettext(1773));
		}
	}

	CurrentVehicle[playerid]=INVALID_VEHICLE_ID;
	return 1;
}

oPutPlayerInVehicle(playerid,vehicleid,seatid)
{
	if(IsPlayerConnected(playerid))
	{
		if(PutPlayerInVehicle(playerid,vehicleid,seatid))
		{
		    	CurrentVehicle[playerid]=vehicleid;
			PlayerJustTeleported[playerid]+=3;
		    	return 1;
		}
	}
	return 0;
}
