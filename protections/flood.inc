//Anti-flood Module
//Created by MadCat

new AntiFlood_Enabled=0;
new AntiFlood_Max_Messages=8;
new Action_On_MaxFlood=2;
new Flood_Ban_Hours=24;

new TimeLastMsg[MAX_PLAYERS];
new FloodCount[MAX_PLAYERS];

forward AntiFlood_OnGameModeInit();
forward AntiFlood_OnPlayerText(playerid,text[]);

public AntiFlood_OnGameModeInit(){
	AntiFloodConfig();
	if (AntiFlood_Enabled == 1) WriteLog("AntiFlood Loaded");
	return 0;
}

AntiFloodConfig()
{
	Debug("protections/flood.inc > AntiFloodConfig - Start");
	new temp[MAX_STRING];
	if (!db_Exists(SecurityDB)) db_Create(SecurityDB);

	set(temp,db_Get(SecurityDB,"AntiFlood_Enabled"));
	if (strlen(temp) > 0) AntiFlood_Enabled = strval(temp);
	else { valstr(temp,AntiFlood_Enabled); db_Set(SecurityDB,"AntiFlood_Enabled",temp); }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"AntiFlood_Max_Messages"));
	if (strlen(temp) > 0) AntiFlood_Max_Messages = strval(temp);
	else { valstr(temp,AntiFlood_Max_Messages); db_Set(SecurityDB,"AntiFlood_Max_Messages",temp); }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Action_On_MaxFlood"));
	if (strlen(temp) > 0) Action_On_MaxFlood = strval(temp);
	else { valstr(temp,Action_On_MaxFlood); db_Set(SecurityDB,"Action_On_MaxFlood",temp); }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Flood_Ban_Hours"));
	if (strlen(temp) > 0) Flood_Ban_Hours = strval(temp);
	else { valstr(temp,Flood_Ban_Hours); db_Set(SecurityDB,"Flood_Ban_Hours",temp); }
	set(temp,nullstr);

	Debug("protections/flood.inc > AntiFloodConfig - Stop");
	return;
}

public AntiFlood_OnPlayerText(playerid,text[])
{
	#pragma unused text
	if (!IsPlayerConnected(playerid)) return;
	if (AntiFlood_Enabled == 0) return;
	if (IsPlayerAdmin(playerid) || PlayerPriv[playerid][IsModerator] == 1) return;
	Debug("protections/flood.inc > AntiFlood_OnPlayerText - Start");
	if((GetTickCount()/1000) - TimeLastMsg[playerid] > 8){
		FloodCount[playerid] = 0;
	}
	else{
		FloodCount[playerid]++;
	}
	if(FloodCount[playerid] >= AntiFlood_Max_Messages){
		FloodCount[playerid] = 0;
		if (Action_On_MaxFlood == 0) return;
		if (Action_On_MaxFlood == 1) Report(playerid,gettext(504));
		if (Action_On_MaxFlood == 2) KickPlayer(playerid,-1,gettext(504));
		if (Action_On_MaxFlood == 3) BanPlayer(playerid,Flood_Ban_Hours,gettext(504));
	}
	TimeLastMsg[playerid] = (GetTickCount()/1000);
	Debug("protections/flood.inc > AntiFlood_OnPlayerText - Stop");
}