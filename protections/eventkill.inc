//Anti-EventKill Module
//Created by MadCat

forward AntiEventKill_OnGameModeInit();
forward AntiEventKill_OnPlayerDeath(playerid, killerid, reason);

new Anti_EventKill_Enabled=0;
new Action_On_EventKill=2;
new EventKill_Ban_Hours=24;

public AntiEventKill_OnGameModeInit(){
	AntiEventKillConfig();
	if (Anti_EventKill_Enabled == 1) WriteLog("Anti Event-Kill Loaded");
	return 0;
}

AntiEventKillConfig()
{
	Debug("protections/eventkill.inc > AntiEventKillConfig - Start");
	new temp[MAX_STRING];
	if (!db_Exists(SecurityDB)) db_Create(SecurityDB);
	
	set(temp,db_Get(SecurityDB,"Anti_EventKill_Enabled"));
	if (strlen(temp) > 0) Anti_EventKill_Enabled = strval(temp);
	else { valstr(temp,Anti_EventKill_Enabled); db_Set(SecurityDB,"Anti_EventKill_Enabled",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Action_On_EventKill"));
	if (strlen(temp) > 0) Action_On_EventKill = strval(temp);
	else { valstr(temp,Action_On_EventKill); db_Set(SecurityDB,"Action_On_EventKill",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"EventKill_Ban_Hours"));
	if (strlen(temp) > 0) EventKill_Ban_Hours = strval(temp);
	else { valstr(temp,EventKill_Ban_Hours); db_Set(SecurityDB,"EventKill_Ban_Hours",temp);  }
	set(temp,nullstr);

	Debug("protections/eventkill.inc > AntiEventKillConfig - Stop");
	return;
}

public AntiEventKill_OnPlayerDeath(playerid, killerid, reason)
{
	if (Anti_EventKill_Enabled == 0) return 0;
	Debug("protections/eventkill.inc > AntiEventKill_OnPlayerDeath - Start");
	if (!IsPlayerConnected(playerid)) {Debug("protections/eventkill.inc > AntiEventKill_OnPlayerDeath - Stop"); return 0;}
	if (!IsPlayerConnected(killerid)) {Debug("protections/eventkill.inc > AntiEventKill_OnPlayerDeath - Stop"); return 0;}
	if (IsPlayerAdmin(killerid) || PlayerPriv[killerid][IsModerator] == 1) {Debug("protections/eventkill.inc > AntiEventKill_OnPlayerDeath - Stop"); return 0;}
	for (new dmid=1; dmid<=DMCount;dmid++)
	{
		if (DeathmatchStats[dmid][dm_state] == DM_STATE_LINEUP)
		{
			if (PlayerQuest[playerid] == GetDMQuestID(dmid) && Player[playerid][GotJob] != JOB_COP)
			{
				if (oIsPlayerInCheckpoint(playerid,GetDMCPX(dmid),GetDMCPY(dmid),GetDMCPZ(dmid),DMCPSize*3))
				{
					if (Action_On_EventKill == 0) return 0;
					if (Action_On_EventKill == 1) Report(killerid,gettext(537));
					if (Action_On_EventKill == 2) KickPlayer(killerid,-1,gettext(537));
					if (Action_On_EventKill == 3) BanPlayer(killerid,EventKill_Ban_Hours,gettext(537));
				}
			}
		}
	}
	for (new hayid=1; hayid<=HayCount;hayid++)
	{
		if (HayStats[hayid][hay_state] == HAY_STATE_LINEUP)
		{
			if (PlayerQuest[playerid] == GetHayQuestID(hayid) && Player[playerid][GotJob] != JOB_COP)
			{
				if (oIsPlayerInCheckpoint(playerid,GetHayCPX(hayid),GetHayCPY(hayid),GetHayCPZ(hayid),HayCPSize*3))
				{
					if (Action_On_EventKill == 0) return 0;
					if (Action_On_EventKill == 1) Report(killerid,gettext(537));
					if (Action_On_EventKill == 2) KickPlayer(killerid,-1,gettext(537));
					if (Action_On_EventKill == 3) BanPlayer(killerid,EventKill_Ban_Hours,gettext(537));
				}
			}
		}
	}
	for (new raceid=1; raceid<=RacesCount;raceid++)
	{
		if (RaceStats[raceid][race_state] == RACE_STATE_LINEUP)
		{
			if (PlayerQuest[playerid] == GetRaceQuestID(raceid) && Player[playerid][GotJob] != JOB_COP && IsPlayerInAnyVehicle(playerid))
			{
				if (oIsPlayerInRaceCheckpoint(playerid,GetRaceCPX(raceid,0),GetRaceCPY(raceid,0),GetRaceCPZ(raceid,0),CPSize*2))
				{
					if (Action_On_EventKill == 0) return 0;
					if (Action_On_EventKill == 1) Report(killerid,gettext(537));
					if (Action_On_EventKill == 2) KickPlayer(killerid,-1,gettext(537));
					if (Action_On_EventKill == 3) BanPlayer(killerid,EventKill_Ban_Hours,gettext(537));		
				}
			}
		}
	}
	Debug("protections/eventkill.inc > AntiEventKill_OnPlayerDeath - Stop");
	return 0;
}

