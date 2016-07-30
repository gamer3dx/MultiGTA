//MOTD Message module
//Created by MadCat
//Idea from FreeRoam 2.4c by ProRail

forward MOTD_OnGameModeInit();
forward MOTD_OnPlayerConnect(playerid);
forward MOTDConfig();
forward LoadMOTD();

#define MAX_MOTD_LINES 4

new MOTD_Text[MAX_MOTD_LINES][MAX_STRING];
new MOTD_Color[MAX_MOTD_LINES][MAX_STRING];
new MOTD_DB[MAX_STRING] = "MultiGTA/Main/MOTD.txt";
new TotalMOTDLines = 0;

public MOTD_OnGameModeInit(){
	MOTDConfig();
	LoadMOTD();
	WriteLog("MOTD loaded");
	return 0;
}

public MOTD_OnPlayerConnect(playerid)
{

	for (new motdline=0;motdline<TotalMOTDLines;motdline++){
		SystemMsg(playerid,HexToInt(MOTD_Color[motdline]),MOTD_Text[motdline]);
	}
	return 0;
}

public MOTDConfig()
{
	Debug("motd.inc > MOTDConfig - Start");
	
	if (!db_Exists(DatabaseDB)) db_Create(DatabaseDB);
	new temp[MAX_STRING];
	set(temp,db_Get(DatabaseDB,"MOTD_DB"));
	if (strlen(temp) > 0) set(MOTD_DB,temp); 
	else { set(temp,MOTD_DB); db_Set(DatabaseDB,"MOTD_DB",temp);  }
	set(temp,nullstr);
	if (!db_Exists(MOTD_DB)) db_Create(MOTD_DB);
	Debug("motd.inc > MOTDConfig - Stop");
	return;
}

public LoadMOTD()
{
	Debug("motd.inc > LoadMOTD - Start");
	new temp[MAX_STRING];
	for (new i=0;i<MAX_MOTD_LINES;i++)
	{
		new tempname[MAX_STRING];
	    	format(tempname,sizeof(tempname),"MOTD_Text%d",i);
		set(temp,db_Get(MOTD_DB,tempname));
		if (strlen(temp) > 0) set(MOTD_Text[i],temp);
		else break; //No text? Stop
		set(temp,nullstr);
	    	format(tempname,sizeof(tempname),"MOTD_Color%d",i);
		set(temp,db_Get(MOTD_DB,tempname));
		if (strlen(temp) > 0) set(MOTD_Color[i],temp);
		else db_Set(MOTD_DB,tempname,MOTD_Color[i]);
		set(temp,nullstr);
		TotalMOTDLines++;		
	}
	Debug("motd.inc > LoadMOTD - Stop");
	return;
}