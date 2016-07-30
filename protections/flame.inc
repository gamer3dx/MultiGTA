//Anti-flame Module
//Created by [NB]Boylett (FilterScript name: xAntiSwear)
//Converted and modified by: MadCat

#define MAX_BADWORDS 200
#define STR 100

forward AntiFlame_OnGameModeInit();
forward FlameConfig();
forward AntiFlame_OnPlayerText(playerid,text[]);

new BadWords[MAX_BADWORDS][STR];
new BadWordsCount = 0;
new PlayerFlameCount[MAX_PLAYERS];

new AntiFlame_Enabled=0;
new FlameDB[MAX_STRING] = "MultiGTA/OtherDB/Badwords.txt";
new Max_Allowed_Badwords=5;
new Action_On_MaxBadwords=2;
new Flame_Ban_Hours=1;

public AntiFlame_OnGameModeInit(){
	FlameConfig();
	if (AntiFlame_Enabled == 1){
		LoadBadWords();
		new logstring[256];
		format(logstring, sizeof (logstring), "Anti Flame Loaded! Words: %d",BadWordsCount);
		WriteLog(logstring);
	}
	return 0;
}

public FlameConfig()
{
	Debug("protections/flame.inc > FlameConfig - Start");
	
	if (!db_Exists(SecurityDB)) db_Create(SecurityDB);
	new temp[MAX_STRING];
	set(temp,db_Get(DatabaseDB,"AntiFlame_DB"));
	if (strlen(temp) > 0) FlameDB = temp; 
	else { set(temp,FlameDB); db_Set(DatabaseDB,"AntiFlame_DB",temp);  }
	set(temp,nullstr);
	set(temp,db_Get(SecurityDB,"AntiFlame_Enabled"));
	if (strlen(temp) > 0) AntiFlame_Enabled = strval(temp); 
	else { valstr(temp,AntiFlame_Enabled); db_Set(SecurityDB,"AntiFlame_Enabled",temp);  }
	set(temp,nullstr);
	set(temp,db_Get(SecurityDB,"Max_Allowed_Badwords"));
	if (strlen(temp) > 0) Max_Allowed_Badwords = strval(temp); 
	else { valstr(temp,Max_Allowed_Badwords); db_Set(SecurityDB,"Max_Allowed_Badwords",temp);  }
	set(temp,nullstr);
	set(temp,db_Get(SecurityDB,"Action_On_MaxBadwords"));
	if (strlen(temp) > 0) Action_On_MaxBadwords = strval(temp); 
	else { valstr(temp,Action_On_MaxBadwords); db_Set(SecurityDB,"Action_On_MaxBadwords",temp);  }
	set(temp,nullstr);
	set(temp,db_Get(SecurityDB,"Flame_Ban_Hours"));
	if (strlen(temp) > 0) Flame_Ban_Hours = strval(temp); 
	else { valstr(temp,Flame_Ban_Hours); db_Set(SecurityDB,"Flame_Ban_Hours",temp);  }
	set(temp,nullstr);
	if (!db_Exists(FlameDB)) db_Create(FlameDB);
	Debug("protections/flame.inc > FlameConfig - Stop");
	return;
}

LoadBadWords()
{
	Debug("protections/flame.inc > LoadBadWords - Start");
	if (!db_Exists(FlameDB)){ db_Create(FlameDB); Debug("protections/flame.inc > LoadBadWords - Stop"); return 0;}
	new string[STR], File:file = fopen(FlameDB,io_read);
	if(file)
	{
		while(fread(file,string))
		{
			if (BadWordsCount >= MAX_BADWORDS) continue;
		   	
			StripNewLine(string);
            		set(BadWords[BadWordsCount],string);
            		BadWordsCount++;
		}
		fclose(file);
	}
	Debug("protections/flame.inc > LoadBadWords - Stop");
	return 0;
}

public AntiFlame_OnPlayerText(playerid,text[])
{
	if (AntiFlame_Enabled == 0) return 0;
	if (!IsPlayerConnected(playerid)) return 0;
	if (IsPlayerAdmin(playerid) || PlayerPriv[playerid][IsModerator] == 1) return 0;
   	for(new s = 0; s < BadWordsCount; s++)
    	{
		if(strfind(text,BadWords[s],true) != -1){
			PlayerFlameCount[playerid]++;
			SystemMsg(playerid,COLOUR_ERROR,gettext(502));
		}
		new pos;
		while((pos = strfind(text,BadWords[s],true)) != -1) for(new i = pos, j = pos + strlen(BadWords[s]); i < j; i++) text[i] = '*';
    	}
    	if(PlayerFlameCount[playerid] >= Max_Allowed_Badwords){
	PlayerFlameCount[playerid]=0;
	if (Action_On_MaxBadwords == 0) PlayerFlameCount[playerid]=0;
	if (Action_On_MaxBadwords == 1)	Report(playerid,gettext(503));
	if (Action_On_MaxBadwords == 2)	KickPlayer(playerid,-1,gettext(503));
	if (Action_On_MaxBadwords == 3) BanPlayer(playerid,Flame_Ban_Hours,gettext(503));
	return 0;
    }
	return 1;
}
