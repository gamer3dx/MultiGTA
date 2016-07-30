//Rules Message module
//Created by MadCat
//Based on MOTD Module

forward Rules_OnGameModeInit();
forward Rules_OnPlayerCommandText(playerid,text[]);
forward RulesConfig();
forward LoadRules();

#define MAX_RULES_LINES 10

new Rules_Text[MAX_RULES_LINES][MAX_STRING];
new Rules_Color[MAX_RULES_LINES][MAX_STRING];
new Rules_DB[MAX_STRING] = "MultiGTA/Main/Rules.txt";
new TotalRulesLines = 0;

public Rules_OnGameModeInit(){
	RulesConfig();
	LoadRules();
	WriteLog("Rules loaded");
	return 0;
}

dcmd_rules(playerid,params[])
{
	#pragma unused params
	for (new ruleline=0;ruleline<TotalRulesLines;ruleline++){
		SystemMsg(playerid,HexToInt(Rules_Color[ruleline]),Rules_Text[ruleline]);
	}
	return 1;
}

public Rules_OnPlayerCommandText(playerid,text[])
{
	dcmd(rules,5,text);
	return 0;
}

public RulesConfig()
{
	Debug("rules.inc > RulesConfig - Start");
	
	if (!db_Exists(DatabaseDB)) db_Create(DatabaseDB);
	new temp[MAX_STRING];
	set(temp,db_Get(DatabaseDB,"Rules_DB"));
	if (strlen(temp) > 0) set(Rules_DB,temp); 
	else { set(temp,Rules_DB); db_Set(DatabaseDB,"Rules_DB",temp);  }
	set(temp,nullstr);
	if (!db_Exists(Rules_DB)) db_Create(Rules_DB);
	Debug("rules.inc > RulesConfig - Stop");
	return;
}

public LoadRules()
{
	Debug("rules.inc > LoadRules - Start");
	new temp[MAX_STRING];
	for (new i=0;i<MAX_RULES_LINES;i++)
	{
		new tempname[MAX_STRING];
	    	format(tempname,sizeof(tempname),"Rules_Text%d",i);
		set(temp,db_Get(Rules_DB,tempname));
		if (strlen(temp) > 0) set(Rules_Text[i],temp);
		else break; //No text? Stop
		set(temp,nullstr);
	    	format(tempname,sizeof(tempname),"Rules_Color%d",i);
		set(temp,db_Get(Rules_DB,tempname));
		if (strlen(temp) > 0) set(Rules_Color[i],temp);
		else db_Set(Rules_DB,tempname,Rules_Color[i]);
		set(temp,nullstr);
		TotalRulesLines++;		
	}
	Debug("rules.inc > LoadRules - Stop");
	return;
}