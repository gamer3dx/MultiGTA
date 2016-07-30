//PayDay module.
//Created by Iain Gilbert
//Modder: MadCat

#include "base"
#include "player"

new PlayTimeForStats[MAX_PLAYERS] = 0;
new PlayTimeForPayDay[MAX_PLAYERS] = 0;

forward PayDay_OnGameModeInit();
forward PayDay();

new BasePay = 200;
new PayDayDelay = 1440; // One SA Day

new TimerPayDay;

public PayDay_OnGameModeInit(){
	PaydayConfig();
	TimerPayDay = SetTimer("PayDay",1000, 1);
	WriteLog("PayDay System Loaded");
	return 0;
}

// timer needs to run this.
// gives players money.
public PayDay()
{
	Debug("payday.inc > PayDay - Start");
	for (new playerid = 0; playerid < MAX_PLAYERS_EX; playerid++)
	{
		 if (IsPlayerConnected(playerid))
		 {
			if (PlayTimeForPayDay[playerid] >= PayDayDelay){
				new string[MAX_STRING];
				new moneys = 0;
				if (Player[playerid][GotJob] == JOB_COP){
					moneys=BasePay+(1000*Player[playerid][CopLevel])+(BasePay*Player[playerid][Level]);
					format(string, sizeof(string),gettext(1),moneys);
				} else if (Player[playerid][GotJob] == JOB_MEDIC){
					moneys=BasePay+(10*Player[playerid][Heals])+(BasePay*Player[playerid][Level]);
					format(string, sizeof(string),gettext(2),moneys);
				} else {
					moneys=BasePay+(BasePay*Player[playerid][Level]);
					format(string, sizeof(string),gettext(3),moneys);
				}
				oGivePlayerMoney(playerid,moneys,0);
				SystemMsg(playerid,COLOUR_MONEY_GOOD,string);
				PlayTimeForPayDay[playerid] = 0;
			} else {
				PlayTimeForPayDay[playerid]++;
			}
			PlayTimeForStats[playerid]++;
			Player[playerid][TotalPlaytime]++;
		}
	}
	Debug("payday.inc > PayDay - Stop");
}

PaydayConfig()
{
	Debug("payday.inc > PaydayConfig - Start");
	
	if (!db_Exists(ConfigDB)) db_Create(ConfigDB);
	new temp[MAX_STRING];
	set(temp,db_Get(ConfigDB,"Payday_Base"));
	if (strlen(temp) > 0) BasePay = strval(temp);
	else { valstr(temp,BasePay); db_Set(ConfigDB,"Payday_Base",temp); }
	set(temp,nullstr);
	set(temp,db_Get(ConfigDB,"Payday_Delay"));
	if (strlen(temp) > 0) PayDayDelay = strval(temp);
	else { valstr(temp,PayDayDelay); db_Set(ConfigDB,"Payday_Delay",temp); }
	set(temp,nullstr);
	Debug("payday.inc > PaydayConfig - Stop");
	return;
}
