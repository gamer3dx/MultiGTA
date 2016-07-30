//Temporaty bans module
//Created by MadCat

#define MAX_TEMP_BANS 200

forward tempban_OnGameModeInit();
forward tempban_OnPlayerConnect(playerid);
forward tempban_OnGameModeExit();

forward tempbanCheck();
forward TempBan(playerid,hours,reason[]);
forward LoadTempBans();

enum TempBanEnum
{
	BannedName[MAX_NAME],
	BannedIP[IP_LEN],
	UnbanTime
}
new TempBanInfo[MAX_TEMP_BANS][TempBanEnum];

new BansDB[MAX_STRING] = "MultiGTA/Main/Bans.txt";

new TimerTempBan;

public tempban_OnGameModeInit(){
	new temp[MAX_STRING];
	set(temp,db_Get(DatabaseDB,"BansDB"));
	if (strlen(temp) > 0) set(BansDB,temp); 
	else { set(temp,BansDB); db_Set(DatabaseDB,"BansDB",temp);  }
	set(temp,nullstr);
	if (!db_Exists(BansDB)) db_Create(BansDB);
	LoadTempBans();
	tempbanCheck();
	TimerTempBan = SetTimer("tempbanCheck",3600000,true);
	WriteLog("Temp ban loaded. One check in hour.");
	return 0;
}

public tempban_OnPlayerConnect(playerid)
{
	new ip[IP_LEN];
	ip=ReturnPlayerIp(playerid);
	for(new i=0;i<sizeof(TempBanInfo);i++)
	{
        	if(strcomp(TempBanInfo[i][BannedName],oGetPlayerName(playerid),false))
		{
			TempBanInfo[i][BannedIP]=ip;
			KickPlayer(playerid,-1,gettext(1772));
			return 1;
		}
	}
	return 0;
}

public tempban_OnGameModeExit(){
 	fremove(BansDB);
	new File:f=fopen(BansDB, io_append),string[64];
	for(new i=0;i<sizeof(TempBanInfo);i++)
	{
		if(strlen(TempBanInfo[i][BannedName]))
		{
		    format(string,sizeof(string),"%d;%s;%s\r\n",TempBanInfo[i][UnbanTime],TempBanInfo[i][BannedIP],TempBanInfo[i][BannedName]);
		    fwrite(f,string);
		}
	}
	fclose(f);
	return 0;
}

public tempbanCheck()
{
    	new string[32];
	new curtime = Now();
    	for(new i=0;i<sizeof(TempBanInfo);i++)
	{
		if((TempBanInfo[i][UnbanTime] < curtime) && (strlen(TempBanInfo[i][BannedName])))
		{
			format(string,sizeof(string),"unbanip %s",TempBanInfo[i][BannedIP]);
			SendRconCommand(string);
			new logstring[MAX_STRING];
			format(logstring,MAX_STRING,"Player '%s' (%s) has been unbanned.",TempBanInfo[i][BannedName],TempBanInfo[i][BannedIP]);
			WriteLog(logstring);
			set(TempBanInfo[i][BannedName],nullstr);
		}
	}
}

public TempBan(playerid,hours,reason[])
{
	if(hours)
	{
		new ip[IP_LEN];
		ip=ReturnPlayerIp(playerid);
	    	for(new i=0;i<sizeof(TempBanInfo);i++)
		{
			if(!TempBanInfo[i][UnbanTime])
			{
			    	TempBanInfo[i][UnbanTime]=Now()+(hours*3600);
			    	TempBanInfo[i][BannedIP]=ip;
			    	set(TempBanInfo[i][BannedName],oGetPlayerName(playerid));
				return 1;
			}
		}
	}
	return 0;
}

public LoadTempBans()
{
	new File:f=fopen(BansDB, io_read);
	if(f)
	{
	    	new count,string[MAX_STRING],idx;
	    	while(fread(f,string,sizeof(string)) && count<sizeof(TempBanInfo))
	    	{	
		        StripNewLine(string);
	        	idx=0;
			if(strlen(string))
			{
			    	TempBanInfo[count][UnbanTime]=strval(strcharsplit(string,idx,strchar(";")));
			    	set(TempBanInfo[count][BannedIP],strcharsplit(string,idx,strchar(";")));
			    	set(TempBanInfo[count][BannedName],strcharsplit(string,idx,strchar(";")));
				count++;
			}
	    	}
	    	fclose(f);
		new logstring[MAX_STRING];
		format(logstring,MAX_STRING,"%d temp bans loaded.",count);
		WriteLog(logstring);
	    	return 1;
	}
	WriteLog("Could not open temp bans file!");
	return 0;
}