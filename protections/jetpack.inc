//Anti-Jetpack Module
//Created by MadCat

forward AntiJetpack_OnGameModeInit();
forward JetpackCheckTimer();

new AntiJetpackTimer;

new Anti_Jetpack_Enabled = 1;
new Action_On_Jetpack=2;
new Jetpack_Ban_Hours=720;

public AntiJetpack_OnGameModeInit(){
	AntiJetpackConfig();
	if (Anti_Jetpack_Enabled == 1){
		AntiJetpackTimer = SetTimer("JetpackCheckTimer",5000,1);
		WriteLog("Anti Jetpack Loaded");
	}
	return 0;
}

AntiJetpackConfig()
{
	Debug("protections/jetpack.inc > AntiJetpackConfig - Start");
	new temp[MAX_STRING];
	if (!db_Exists(SecurityDB)) db_Create(SecurityDB);
	
	set(temp,db_Get(SecurityDB,"Anti_Jetpack_Enabled"));
	if (strlen(temp) > 0) Anti_Jetpack_Enabled = strval(temp);
	else { valstr(temp,Anti_Jetpack_Enabled); db_Set(SecurityDB,"Anti_Jetpack_Enabled",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Action_On_Jetpack"));
	if (strlen(temp) > 0) Action_On_Jetpack = strval(temp);
	else { valstr(temp,Action_On_Jetpack); db_Set(SecurityDB,"Action_On_Jetpack",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Jetpack_Ban_Hours"));
	if (strlen(temp) > 0) Jetpack_Ban_Hours = strval(temp);
	else { valstr(temp,Jetpack_Ban_Hours); db_Set(SecurityDB,"Jetpack_Ban_Hours",temp);  }
	set(temp,nullstr);


	Debug("protections/jetpack.inc > AntiJetpackConfig - Stop");
	return;
}

public JetpackCheckTimer()
{
	Debug("protections/jetpack.inc > JetpackCheckTimer - Start");
	for (new playerid=0;playerid<MAX_PLAYERS_EX;playerid++)
	{
		if (IsPlayerConnected(playerid) && PlayerPriv[playerid][IsModerator] == 0 && !IsPlayerAdmin(playerid))
		{
			if (GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_USEJETPACK){
				if (Action_On_Jetpack == 0) continue;
				if (Action_On_Jetpack == 1) Report(playerid,gettext(1024));
				if (Action_On_Jetpack == 2) KickPlayer(playerid,-1,gettext(1024));
				if (Action_On_Jetpack == 3) BanPlayer(playerid,Jetpack_Ban_Hours,gettext(1024));
			}
		}
	}
	Debug("protections/jetpack.inc > JetpackCheckTimer - Stop");
}