//Logging module.
//Created by GTO Dev Team
//Modder: MadCat

#define DEVELOPER

#include "base"

forward Logging_OnGameModeInit();
forward WriteLog(string[]);
forward WriteModerLog(string[]);
forward WriteChatLog(playerid,text[],type[]);
forward WriteCMDLog(string[]);
forward WriteSecurityLog(string[]);
forward Debug(string[]);

new LogsDir[MAX_STRING] = "MultiGTA/Logs/";
new File:LogFile;

new Logging_On = 1;
new Debug_On = 0;
new ChatLog_On = 1;
new CMDLog_On = 1;
new ModerLog_On = 1;
new SecurityLog_On = 1;

LoggingConfig()
{
	
	if (!db_Exists(ConfigDB)) db_Create(ConfigDB);
	if (!db_Exists(DatabaseDB)) db_Create(DatabaseDB);
	new temp[MAX_STRING];
	set(temp,db_Get(ConfigDB,"Logging_On"));
	if (strlen(temp) > 0) Logging_On = strval(temp);
	else { valstr(temp,Logging_On); db_Set(ConfigDB,"Logging_On",temp); }
	set(temp,nullstr);

	set(temp,db_Get(ConfigDB,"Debug_On"));
	if (strlen(temp) > 0) Debug_On = strval(temp);
	else { valstr(temp,Debug_On); db_Set(ConfigDB,"Debug_On",temp); }
    	set(temp,nullstr);

    	set(temp,db_Get(ConfigDB,"ChatLog_On"));
	if (strlen(temp) > 0) ChatLog_On = strval(temp);
	else { valstr(temp,ChatLog_On); db_Set(ConfigDB,"ChatLog_On",temp); }
    	set(temp,nullstr);

    	set(temp,db_Get(ConfigDB,"CMDLog_On"));
	if (strlen(temp) > 0) CMDLog_On = strval(temp);
	else { valstr(temp,CMDLog_On); db_Set(ConfigDB,"CMDLog_On",temp); }
    	set(temp,nullstr);

    	set(temp,db_Get(ConfigDB,"ModerLog_On"));
	if (strlen(temp) > 0) ModerLog_On = strval(temp);
	else { valstr(temp,ModerLog_On); db_Set(ConfigDB,"ModerLog_On",temp); }
    	set(temp,nullstr);

    	set(temp,db_Get(ConfigDB,"SecurityLog_On"));
	if (strlen(temp) > 0) SecurityLog_On = strval(temp);
	else { valstr(temp,SecurityLog_On); db_Set(ConfigDB,"SecurityLog_On",temp); }
    	set(temp,nullstr);

    	set(temp,db_Get(DatabaseDB,"Logs_DB"));
	if (strlen(temp) > 0) set(LogsDir,temp);
	else { set(temp,LogsDir); db_Set(DatabaseDB,"Logs_DB",temp); }
    	set(temp,nullstr);
	return;
}

public Logging_OnGameModeInit()
{
	LoggingConfig();
	WriteLog("Logging Loaded");
}

FormatFigures(start[], f1, sep1[], f2, sep2[], f3, end[])
{
	new string[256];
	if (f1 < 10)
	{
	    format(string, sizeof (string), "%s0%d%s", start, f1, sep1);
	}
	else
	{
	    format(string, sizeof (string), "%s%d%s", start, f1, sep1);
	}
	if (f2 < 10)
	{
	    format(string, sizeof (string), "%s0%d%s", string, f2, sep2);
	}
	else
	{
	    format(string, sizeof (string), "%s%d%s", string, f2, sep2);
	}
	if (f3 < 10)
	{
	    format(string, sizeof (string), "%s0%d%s", string, f3, end);
	}
	else
	{
	    format(string, sizeof (string), "%s%d%s", string, f3, end);
	}
	return string;
}

MakeFilename(name[])
{
	new d, m, y;
	getdate(y, m, d);
	new string[MAX_STRING];
	set(string,FormatFigures(name, d, "-", m, "-", y, ".log"));
	return string;
}

MakeFormattedTime()
{
	new h, m, s;
	gettime(h, m, s);
	new string[MAX_STRING];
	set(string,FormatFigures("[", h, ":", m, ":", s, "]"));
	return string;
}

public WriteLog(string[])
{
	if (Logging_On > 0)
	{
		#if defined DEVELOPER
		#else
		new filestring[256];
	    	format(filestring,sizeof(filestring),"%s%s",LogsDir,MakeFilename("game_"));

		if (fexist(filestring))
		{
	        	LogFile = fopen(filestring, io_append);
		}
		else
		{
	        	LogFile = fopen(filestring, io_write);
		}

		fwrite(LogFile, MakeFormattedTime());
		fwrite(LogFile, "  ");
		fwrite(LogFile, string);
		fwrite(LogFile, "\r\n");

		fclose(LogFile);
		#endif
		printf(string);
	}
}

public WriteModerLog(string[])
{
	if (ModerLog_On > 0)
	{
		#if defined DEVELOPER
		#else
		new filestring[256];
	   	format(filestring,sizeof(filestring),"%s%s",LogsDir,MakeFilename("moderator_"));

		if (fexist(filestring))
		{
	        LogFile = fopen(filestring, io_append);
		}
		else
		{
	        LogFile = fopen(filestring, io_write);
		}

		fwrite(LogFile, MakeFormattedTime());
		fwrite(LogFile, "  ");
		fwrite(LogFile, string);
		fwrite(LogFile, "\r\n");

		fclose(LogFile);
		#endif
		printf(string);
	}
}

public WriteChatLog(playerid,text[],type[])
{
	if (ChatLog_On > 0)
	{
		#if defined DEVELOPER
		#else
		new filestring[256];
	    	format(filestring,sizeof(filestring),"%s%s",LogsDir,MakeFilename("chat_"));

		if (fexist(filestring))
		{
	        LogFile = fopen(filestring, io_append);
		}
		else
		{
	        LogFile = fopen(filestring, io_write);
		}

		new string[MAX_STRING];
		if(strlen(type) > 0){
			format(string,MAX_STRING,"(%s) %s: %s",type,oGetPlayerName(playerid),text);
		} else {
			format(string,MAX_STRING,"%s: %s",oGetPlayerName(playerid),text);
		}

		fwrite(LogFile, MakeFormattedTime());
		fwrite(LogFile, "  ");
		fwrite(LogFile, string);
		fwrite(LogFile, "\r\n");

		fclose(LogFile);
		printf(string);
		#endif
	}
}

public WriteCMDLog(string[])
{
	if (CMDLog_On > 0)
	{
		#if defined DEVELOPER
		#else
	   	new filestring[256];
		format(filestring,sizeof(filestring),"%s%s",LogsDir,MakeFilename("commands_"));

		if (fexist(filestring))
		{
	        LogFile = fopen(filestring, io_append);
		}
		else
		{
	        LogFile = fopen(filestring, io_write);
		}

		fwrite(LogFile, MakeFormattedTime());
		fwrite(LogFile, "  ");
		fwrite(LogFile, string);
		fwrite(LogFile, "\r\n");

		fclose(LogFile);
		#endif
		printf(string);
	}
}

public WriteSecurityLog(string[])
{
	if (SecurityLog_On > 0)
	{
		#if defined DEVELOPER
		#else
		new filestring[256];
	   	format(filestring,sizeof(filestring),"%s%s",LogsDir,MakeFilename("security_"));

		if (fexist(filestring))
		{
	        LogFile = fopen(filestring, io_append);
		}
		else
		{
	        LogFile = fopen(filestring, io_write);
		}

		fwrite(LogFile, MakeFormattedTime());
		fwrite(LogFile, "  ");
		fwrite(LogFile, string);
		fwrite(LogFile, "\r\n");

		fclose(LogFile);
		#endif
		printf(string);
	}
}

public Debug(string[])
{
	if (Debug_On > 0)
	{
		#if defined DEVELOPER
		#else
		new filestring[256];
	    	format(filestring,sizeof(filestring),"%s%s",LogsDir,MakeFilename("debug_"));

		if (fexist(filestring))
		{
	        	LogFile = fopen(filestring, io_append);
		}
		else
		{
	        	LogFile = fopen(filestring, io_write);
		}

		fwrite(LogFile, MakeFormattedTime());
		fwrite(LogFile, "  ");
		fwrite(LogFile, string);
		fwrite(LogFile, "\r\n");

		fclose(LogFile);
		#endif
		printf(string);
	}
}

#if defined DEVELOPER
#pragma unused FormatFigures
#pragma unused LogFile
#pragma unused LogsDir
#pragma unused MakeFilename
#pragma unused MakeFormattedTime
#endif
