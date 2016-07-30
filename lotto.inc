//Lotto
//Created by Fear
//Converted for MultiGTA by MadCat

forward Lotto();
forward LottoConfig();

forward Lotto_OnGameModeInit();
forward Lotto_OnPlayerCommandText(playerid,text[]);

new Lottery_Enabled = 1;
new Ticket_Cost = 1500;
new Jackpot = 10000;

new TimerLotto;

public LottoConfig()
{
	Debug("lotto.inc > LottoConfig - Start");
	if (!db_Exists(ConfigDB)) db_Create(ConfigDB);

	new temp[MAX_STRING];

	set(temp,db_Get(ConfigDB,"Lottery_Enabled"));
	if (strlen(temp) > 0) Lottery_Enabled = strval(temp); 
	else { valstr(temp,Lottery_Enabled); db_Set(ConfigDB,"Lottery_Enabled",temp); }
	set(temp,nullstr);
	
	set(temp,db_Get(ConfigDB,"Ticket_Cost"));
	if (strlen(temp) > 0) Ticket_Cost = strval(temp); 
	else { valstr(temp,Ticket_Cost); db_Set(ConfigDB,"Ticket_Cost",temp); }
	set(temp,nullstr);

	set(temp,db_Get(ConfigDB,"Jackpot"));
	if (strlen(temp) > 0) Jackpot = strval(temp); 
	else { valstr(temp,Jackpot); db_Set(ConfigDB,"Jackpot",temp); }
	set(temp,nullstr);
	Debug("lotto.inc > LottoConfig - Stop");
}

public Lotto_OnGameModeInit(){
	LottoConfig();
	if (Lottery_Enabled == 1){
		TimerLotto = SetTimer("Lotto",ONE_SA_DAY,1);
		WriteLog("Lotto System Loaded");
	}
	return 0;
}

public Lotto()
{
	new JackpotFallen = 0;
	new string[256];
	new number = random(80);
	if(number < 77) number += 3;
	format(string, sizeof(string), gettext(1479), number);
   	SystemMsgToRegistered(COLOUR_INFO, string);
    	for(new i = 0; i < MAX_PLAYERS_EX; i++)
	{
		if(IsPlayerConnected(i))
		{
		    if(Player[i][LottoNumber] > 0)
		    {
			    if(Player[i][LottoNumber] == number)
			    {
			        	JackpotFallen = 1;
					format(string, sizeof(string), gettext(1480), oGetPlayerName(i), Jackpot);
					SystemMsgToRegistered(COLOUR_INFO, string);
					format(string, sizeof(string), gettext(1481), Jackpot);
					SystemMsg(i, COLOUR_MONEY_GOOD, string);
					oGivePlayerMoney(i, Jackpot, false);
			    }
			    else
			    {
			        SystemMsg(i, COLOUR_MONEY_BAD, gettext(1482));
			    }
			}
		     Player[i][LottoNumber] = 0;
		}
	}
	new temp[16];
	if(JackpotFallen)
	{
	    	new rand = random(10000); rand += 5789;
	    	Jackpot = rand;
	    	valstr(temp,Jackpot); 
		db_Set(ConfigDB,"Jackpot",temp);
	    	format(string, sizeof(string), gettext(1483), Jackpot);
		SystemMsgToRegistered(COLOUR_INFO, string);
	}
	else
	{
	    	new rand = random(2000); rand += 1258;
	    	Jackpot += rand;
	    	valstr(temp,Jackpot); 
		db_Set(ConfigDB,"Jackpot",temp);
	    	format(string, sizeof(string), gettext(1484), Jackpot);
		SystemMsgToRegistered(COLOUR_INFO, string);
	}
	return 1;
}

public Lotto_OnPlayerCommandText(playerid,text[])
{
	if (Lottery_Enabled == 0) return 0;
	if (!IsPlayerRegistered(playerid)) return 0;
	dcmd(lotto,5,text);
	return 0;
}

dcmd_lotto(playerid,params[])
{
    if(Player[playerid][LottoNumber] > 0)
    {
    	SystemMsg(playerid,COLOUR_ERROR,gettext(1485));
    	return 1;
    }
    if(oGetPlayerMoney(playerid) < Ticket_Cost)
    {
        SystemMsgFormat(playerid,COLOUR_ERROR,gettext(1486),Ticket_Cost);
        return 1;
    }
    if(isempty(params)) {
   		SystemMsg(playerid, COLOUR_ERROR,gettext(1487));
		return 1;
    }
    new lottonr = strval(params);
    new string[MAX_STRING];
    if(lottonr < 1 || lottonr > 80) return SystemMsg(playerid, COLOUR_ERROR,gettext(1488));
    format(string, sizeof(string), gettext(1489), lottonr);
    SystemMsg(playerid, COLOUR_OK, string);
    oGivePlayerMoney(playerid, -Ticket_Cost,false);
    Player[playerid][LottoNumber] = lottonr;
    return 1;
}