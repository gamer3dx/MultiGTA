//Anti-Bot module
//Created by Pixels^. Part of code from AntiBot Script by DarkByte. Another part from Forbidden Names module. Rewritten.
//Modder: MadCat

forward AntiBot_OnGameModeInit();
forward AntiBot_OnPlayerConnect(playerid);
forward BotCheck();

new TimerAntiBot;

new Anti_Bot_Enabled=0;
new Max_Player_Per_IP=15;
new Bot_Names_DB[MAX_STRING]="MultiGTA/OtherDB/Bot_Names.txt";

#define MAX_BOTNAMES 50
new BotNames[MAX_BOTNAMES][MAX_NAME];
new BotNamesCount = 0;

public AntiBot_OnGameModeInit(){
	AntiBotsConfig();
	if (Anti_Bot_Enabled == 1){
		LoadBotNames();
		TimerAntiBot = SetTimer("BotCheck",5000,1);
		WriteLog("Anti Bot Loaded");
	}
	return 0;
}

AntiBotsConfig()
{
	Debug("protections/bots.inc > AntiBotsConfig - Start");
	new temp[MAX_STRING];
	if (!db_Exists(SecurityDB)) db_Create(SecurityDB);

	set(temp,db_Get(SecurityDB,"Anti_Bot_Enabled"));
	if (strlen(temp) > 0) Anti_Bot_Enabled = strval(temp);
	else { valstr(temp,Anti_Bot_Enabled); db_Set(SecurityDB,"Anti_Bot_Enabled",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(SecurityDB,"Max_Player_Per_IP"));
	if (strlen(temp) > 0) Max_Player_Per_IP = strval(temp);
	else { valstr(temp,Max_Player_Per_IP); db_Set(SecurityDB,"Max_Player_Per_IP",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(DatabaseDB,"Bot_Names_DB"));
	if (strlen(temp) > 0) set(Bot_Names_DB,temp); 
	else { set(temp,Bot_Names_DB); db_Set(DatabaseDB,"Bot_Names_DB",temp);  }
	set(temp,nullstr);

	Debug("protections/bots.inc > AntiBotsConfig - Stop");
	return;
}

public BotCheck()
{
Debug("protections/bots.inc > BotCheck - Start");
for(new playerid=0; playerid<MAX_PLAYERS_EX; playerid++)
{
	if(IsPlayerConnected(playerid) && PlayerPriv[playerid][IsModerator] == 0 && !IsPlayerAdmin(playerid)){
		new ping = GetPlayerPing(playerid);
  		if(ping <= 0 || ping == 65535)
		{
			KickPlayer(playerid,-1,gettext(1116));
		}
	}
}
Debug("protections/bots.inc > BotCheck - Stop");
}

public AntiBot_OnPlayerConnect(playerid)
{
	if (Anti_Bot_Enabled == 0) return 0;
	Debug("protections/bots.inc > AntiBot_OnPlayerConnect - Start");
	if (IsBotName(playerid)){ Debug("protections/bots.inc > AntiBot_OnPlayerConnect - Stop"); KickPlayer(playerid,-1,gettext(1116)); return 1;}
	new ip[IP_LEN];
	new ip2[IP_LEN];
	new cnt = 0;
	ip2 = ReturnPlayerIp(playerid);
	for (new i=0; i<MAX_PLAYERS_EX; i++){
		if ((IsPlayerConnected(i))&&(i!=playerid)) {
			ip2=ReturnPlayerIp(playerid);
			if (strcomp(ip,ip2,true)==1) cnt++;
		}
		if (cnt>=Max_Player_Per_IP) {
			Debug("protections/bots.inc > AntiBot_OnPlayerConnect - Stop");
		    	KickPlayer(playerid,-1,gettext(1116));
			return 1;
		}
	}
	Debug("protections/bots.inc > AntiBot_OnPlayerConnect - Stop");
	return 0;
}

LoadBotNames()
{
	Debug("protections/bots.inc > LoadBotNames - Start");
	if (!db_Exists(Bot_Names_DB)){ db_Create(Bot_Names_DB); Debug("protections/bots.inc > LoadBotNames - Stop"); return 0;}
	new string[MAX_NAME+10], File:file = fopen(Bot_Names_DB,io_read);
	if(file)
	{
		while(fread(file,string))
		{
			if (BotNamesCount >= MAX_BOTNAMES) continue;

		   	StripNewLine(string);
            		set(BotNames[BotNamesCount],string);
            		BotNamesCount++;
		}
		fclose(file);
	}
	new logstring[MAX_STRING];
	format(logstring,MAX_STRING,"Loaded %d bot names from DB",BotNamesCount);
	WriteLog(logstring);
	Debug("protections/bots.inc > LoadBotNames - Stop");
	return 0;
}

stock IsBotName(playerid)
{
	Debug("protections/bots.inc > IsBotName - Start");
	new pName[MAX_NAME];
	set(pName,oGetPlayerName(playerid));
	for(new i = 0; i < BotNamesCount; i++)
	{
        	if(strcomp(pName,BotNames[i],false) == 1) return 1;
	
		if(strcomp(pName,BotNames[i],false) == 1) return 1;
	}
	Debug("protections/bots.inc > IsBotName - Stop");
	return 0;
}

