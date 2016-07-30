//Multikill module.
//Based on module "Multikill" from GTAT SVN
//Created by DracoBlue
//Converted by MadCat

new MULTIKILL_SessionKills[MAX_PLAYERS];
new MULTIKILL_Time[MAX_PLAYERS];

new Multikill_Enabled=0;
new Multikill_End_Time=60;

new TimerMultikill;

MultikillConfig()
{
	Debug("multikill.inc > MultikillConfig - Start");
	new temp[MAX_STRING];
	if (!db_Exists(ConfigDB)) db_Create(ConfigDB);
	
	set(temp,db_Get(ConfigDB,"Multikill_Enabled"));
	if (strlen(temp) > 0) Multikill_Enabled = strval(temp);
	else { valstr(temp,Multikill_Enabled); db_Set(ConfigDB,"Multikill_Enabled",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(ConfigDB,"Multikill_End_Time"));
	if (strlen(temp) > 0) Multikill_End_Time = strval(temp);
	else { valstr(temp,Multikill_End_Time); db_Set(ConfigDB,"Multikill_End_Time",temp);  }
	set(temp,nullstr);


	Debug("multikill.inc > MultikillConfig - Stop");
	return;
}

forward multikill_OnGameModeInit();

public multikill_OnGameModeInit(){
	MultikillConfig();
	if (Multikill_Enabled == 1){
		TimerMultikill = SetTimer("multikill_Timer",1000,1);
		WriteLog("Multikill Loaded");
	}
	return 0;
}

forward multikill_Timer();
public multikill_Timer(){
	for(new playerid;playerid<MAX_PLAYERS_EX;playerid++){
		if(IsPlayerConnected(playerid) && MULTIKILL_Time[playerid]>0){
			MULTIKILL_Time[playerid]++;
			if(MULTIKILL_Time[playerid] >= Multikill_End_Time){
				MULTIKILL_Time[playerid] = 0;
				ShowTextForPlayer(playerid,gettext(1118),4,6);
				SystemMsgFormat(playerid,COLOUR_INFO,gettext(1119),MULTIKILL_SessionKills[playerid]);
				MULTIKILL_SessionKills[playerid] = 0;
			}
		}
	}
}

forward multikill_OnPlayerConnect(playerid);
public multikill_OnPlayerConnect(playerid){
	if (Multikill_Enabled == 0) return 0;
	Debug("multikill.inc > multikill_OnPlayerConnect - Start");
	MULTIKILL_Time[playerid]=0;
	MULTIKILL_SessionKills[playerid]=0;
	Debug("multikill.inc > multikill_OnPlayerConnect - Stop");
	return 0;
}

forward multikill_OnPlayerDisconnect(playerid);
public multikill_OnPlayerDisconnect(playerid){
	if (Multikill_Enabled == 0) return 0;
	Debug("multikill.inc > multikill_OnPlayerDisconnect - Start");
	MULTIKILL_Time[playerid]=0;
	MULTIKILL_SessionKills[playerid]=0;
	Debug("multikill.inc > multikill_OnPlayerDisconnect - Stop");
	return 0;
}

forward multikill_OnPlayerDeath(playerid,killerid,reason);
public multikill_OnPlayerDeath(playerid,killerid,reason){
	#pragma unused reason
	if (Multikill_Enabled == 0) return 0;
	if (IsPlayerInAnyDM(playerid)) return 0;
	Debug("multikill.inc > multikill_OnPlayerDeath - Start");
	if (MULTIKILL_SessionKills[killerid]==0) {
		MULTIKILL_Time[killerid]=1;
	}
	MULTIKILL_SessionKills[killerid]++;
	
	new tmp[MAX_STRING];
	if(MULTIKILL_SessionKills[killerid] == 2) {
		tmp=gettext(1120);
	} else if(MULTIKILL_SessionKills[killerid] == 5) {
		tmp=gettext(1121);
	} else if(MULTIKILL_SessionKills[killerid] == 8) {
		tmp=gettext(1122);
	} else if(MULTIKILL_SessionKills[killerid] == 12) {
		tmp=gettext(1123);
	} else if(MULTIKILL_SessionKills[killerid] == 17) {
		tmp=gettext(1124);
	} else if(MULTIKILL_SessionKills[killerid] == 22) {
		tmp=gettext(1125);
	} else if(MULTIKILL_SessionKills[killerid] == 30) {
		tmp=gettext(1126);
	} else if(MULTIKILL_SessionKills[killerid] == 38) {
		tmp=gettext(1127);
	} else if(MULTIKILL_SessionKills[killerid] == 50) {
		tmp=gettext(1128);
	}

	new tmp2[MAX_STRING];
	format(tmp2,sizeof(tmp2),gettext(1129),tmp);
	ShowTextForPlayer(killerid,tmp2,4000,6);
	GivePlayerXP(killerid,MULTIKILL_SessionKills[killerid]*50,0);
	oGivePlayerMoney(killerid,MULTIKILL_SessionKills[killerid]*100,0);

	Debug("multikill.inc > multikill_OnPlayerDeath - Stop");
	return 0;
}
