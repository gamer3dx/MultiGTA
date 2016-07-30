//Anti-Money Cheat
//Created by Lukor
//Converted from Moderntopia for MultiGTA by MadCat

forward AntiMoneyCheat_OnGameModeInit();
forward MoneyCheatTimer();

new TimerAntiMoney;

new Anti_Money_Cheat_Enabled=1;
new Action_On_Money_Cheat=2;
new Money_Cheat_Ban_Hours=720;

public AntiMoneyCheat_OnGameModeInit(){
	AntiMoneyCheatConfig();
	if (Anti_Money_Cheat_Enabled == 1){
		TimerAntiMoney = SetTimer("MoneyCheatTimer",5000,1);
		WriteLog("Anti Money-Cheat Loaded");
	}
	return 0;
}

AntiMoneyCheatConfig()
{
	Debug("protections/money.inc > AntiMoneyCheatConfig - Start");
	new temp[MAX_STRING];
	if (!db_Exists(SecurityDB)) db_Create(SecurityDB);
	
	set(temp,db_Get(SecurityDB,"Anti_Money_Cheat_Enabled"));
	if (strlen(temp) > 0) Anti_Money_Cheat_Enabled = strval(temp);
	else { valstr(temp,Anti_Money_Cheat_Enabled); db_Set(SecurityDB,"Anti_Money_Cheat_Enabled",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Action_On_Money_Cheat"));
	if (strlen(temp) > 0) Action_On_Money_Cheat = strval(temp);
	else { valstr(temp,Action_On_Money_Cheat); db_Set(SecurityDB,"Action_On_Money_Cheat",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Money_Cheat_Ban_Hours"));
	if (strlen(temp) > 0) Money_Cheat_Ban_Hours = strval(temp);
	else { valstr(temp,Money_Cheat_Ban_Hours); db_Set(SecurityDB,"Money_Cheat_Ban_Hours",temp);  }
	set(temp,nullstr);


	Debug("protections/money.inc > AntiMoneyCheatConfig - Stop");
	return;
}

public MoneyCheatTimer()
{
	Debug("protections/money.inc > MoneyCheatTimer - Start");
	new curHour, curMinute, curSecond;
	gettime(curHour, curMinute, curSecond); //now here :)
	for (new playerid=0;playerid<MAX_PLAYERS_EX;playerid++)
	{
		if (IsPlayerConnected(playerid) && PlayerPriv[playerid][IsModerator] == 0 && !IsPlayerAdmin(playerid))
		{
			if (ScriptMoneyUpdated[playerid]+2 < curSecond)
			{
				new plactualmoney = GetPlayerMoney(playerid);
				if (plactualmoney > Player[playerid][Money] && plactualmoney-499 > Player[playerid][Money])
				{
					if (Action_On_Money_Cheat == 0) continue;
					if (Action_On_Money_Cheat == 1) Report(playerid,gettext(971));
					if (Action_On_Money_Cheat == 2) KickPlayer(playerid,-1,gettext(971));
					if (Action_On_Money_Cheat == 3) BanPlayer(playerid,Money_Cheat_Ban_Hours,gettext(971));
				}
				else
				{
					ScriptMoneyUpdated[playerid] = 0;
				}
			}
		}
	}
	Debug("protections/money.inc > MoneyCheatTimer - Stop");
}