//Anti-Idle module
//Created by _Sneaky
//Modder: MadCat

#define MAX_FORBIDDENNAMES 50

new ForbiddenNames[MAX_FORBIDDENNAMES][MAX_NAME];

new ForbidNamesCount = 0;

forward ForbiddenNames_OnGameModeInit();

new Forbidden_Names_Enabled = 1;
new Forbidden_Names_DB[MAX_STRING] = "MultiGTA/OtherDB/Forbidden_Names.txt";

public ForbiddenNames_OnGameModeInit(){
	AntiForbidNamesConfig();
	if (Forbidden_Names_Enabled == 1){
		LoadForbiddenNames();
		new logstring[256];
		format(logstring, sizeof (logstring), "%d forbidden names loaded from DB.",ForbidNamesCount);
		WriteLog(logstring);
	}
	return 0;
}

AntiForbidNamesConfig()
{
	Debug("protections/namecrash.inc > AntiForbidNamesConfig - Start");
	new temp[MAX_STRING];
	if (!db_Exists(SecurityDB)) db_Create(SecurityDB);

	set(temp,db_Get(SecurityDB,"Forbidden_Names_Enabled"));
	if (strlen(temp) > 0) Forbidden_Names_Enabled = strval(temp);
	else { valstr(temp,Forbidden_Names_Enabled); db_Set(SecurityDB,"Forbidden_Names_Enabled",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(DatabaseDB,"Forbidden_Names_DB"));
	if (strlen(temp) > 0) set(Forbidden_Names_DB,temp); 
	else { set(temp,Forbidden_Names_DB); db_Set(DatabaseDB,"Forbidden_Names_DB",temp);  }
	set(temp,nullstr);

	Debug("protections/namecrash.inc > AntiForbidNamesConfig - Stop");
	return;
}

LoadForbiddenNames()
{
	Debug("protections/forbidnames.inc > LoadForbiddenNames - Start");
	if (!db_Exists(Forbidden_Names_DB)){ db_Create(Forbidden_Names_DB); Debug("protections/forbidnames.inc > LoadForbiddenNames - Stop"); return 0;}
	new string[MAX_NAME+10], File:file = fopen(Forbidden_Names_DB,io_read);
	if(file)
	{
		while(fread(file,string))
		{
			if (ForbidNamesCount >= MAX_FORBIDDENNAMES) continue;

		   	StripNewLine(string);
            		set(ForbiddenNames[ForbidNamesCount],string);
            		ForbidNamesCount++;
		}
		fclose(file);
	}
	new logstring[MAX_STRING];
	format(logstring,MAX_STRING,"Loaded %d forbidden names from DB",ForbidNamesCount);
	WriteLog(logstring);
	Debug("protections/forbidnames.inc > LoadForbiddenNames - Stop");
	return 0;
}

stock IsNameForbidden(name[])
{
	Debug("protections/forbidnames.inc > IsNameForbidden - Start");
	for(new i = 0; i < ForbidNamesCount; i++)
	{
        	if(strcomp(name,ForbiddenNames[i],false) == 1) return 1;
	
		if(strcomp(name,ForbiddenNames[i],false) == 1) return 1;
	}
	Debug("protections/forbidnames.inc > IsNameForbidden - Stop");
	return 0;
}


