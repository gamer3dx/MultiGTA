//Hotspot module.
//Idea by DracoBlue
//Created by MadCat

new Hotspots_Enabled = 1;
new Hotspot_Multiplier = 2;

new hotspot_zonename[MAX_STRING];

forward Hotspot_OnGameModeInit();
forward Hotspot_Timer();

new TimerHotspot;

HotspotsConfig()
{
	Debug("hotspot.inc > HotspotsConfig - Start");
	new temp[MAX_STRING];
	if (!db_Exists(ConfigDB)) db_Create(ConfigDB);
	
	set(temp,db_Get(ConfigDB,"Hotspots_Enabled"));
	if (strlen(temp) > 0) Hotspots_Enabled = strval(temp);
	else { valstr(temp,Hotspots_Enabled); db_Set(ConfigDB,"Hotspots_Enabled",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(ConfigDB,"Hotspot_Multiplier"));
	if (strlen(temp) > 0) Hotspot_Multiplier = strval(temp);
	else { valstr(temp,Hotspot_Multiplier); db_Set(ConfigDB,"Hotspot_Multiplier",temp);  }
	set(temp,nullstr);

	if (Hotspot_Multiplier < 1) Hotspot_Multiplier = 1;

	Debug("hotspot.inc > HotspotsConfig - Stop");
	return;
}

public Hotspot_OnGameModeInit(){
	HotspotsConfig();
	if (Hotspots_Enabled == 1){
		TimerHotspot = SetTimer("Hotspot_Timer",600000,1);
		WriteLog("Hotspots Loaded");
	}
	return 0;
}

public Hotspot_Timer(){
	new zone = random(GetMaxZones());
	while (zone == 0 || zone == 1 || zone == 2) zone = random(GetMaxZones()); //Casinos
	set(hotspot_zonename,nullstr); // reset variable :)
	set(hotspot_zonename,GetZoneNameByID(zone));
	new string[MAX_STRING];
	format(string,MAX_STRING,gettext(413),hotspot_zonename);
	for(new i=0;i<MAX_PLAYERS_EX;i++)
	{
		SystemMsg(i,COLOUR_INFO,string);	
	}
}

IsPlayerKilledInHotspotZone(playerid,killerid){
	if (Hotspots_Enabled == 0) return 0;
	if (IsPlayerInAnyDM(playerid)) return 0;
	if (killerid == INVALID_PLAYER_ID) return 0;
	if (!IsPlayerConnected(killerid)) return 0;
	if (strcomp(GetXYZZoneName(PlayerPos[playerid][Coord_X],PlayerPos[playerid][Coord_Y],PlayerPos[playerid][Coord_Z]),hotspot_zonename,true) == 1) return 1;
	return 0;
}
