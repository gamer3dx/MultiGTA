//Anti-caps Module
//Created by Double-O-Seven (JunkBuster Anti-Cheat)
//Converted and modified by MadCat

forward AntiCaps_OnGameModeInit();
forward AntiCapsConfig();

new AntiCaps_Enabled=0;

public AntiCaps_OnGameModeInit(){
	AntiCapsConfig();
	if (AntiCaps_Enabled == 1){
		WriteLog("Anti-caps loaded!");
	}
	return 0;
}

public AntiCapsConfig()
{
	if (!db_Exists(SecurityDB)) db_Create(SecurityDB);
	new temp[MAX_STRING];
	set(temp,db_Get(SecurityDB,"AntiCaps_Enabled"));
	if (strlen(temp) > 0) AntiCaps_Enabled = strval(temp); 
	else { valstr(temp,AntiCaps_Enabled); db_Set(SecurityDB,"AntiCaps_Enabled",temp);  }
	set(temp,nullstr);
	return;
}

stock CheckForCaps(text[])
{
	if (AntiCaps_Enabled == 0) return 0;
	new len=strlen(text);
	if(len>3)
	{
	    	new c;
	    	for(new i=0;i<len;i++) if((text[i]>='A' && text[i]<='Z') || (text[i]>='À' && text[i]<='ß')) c++;
		if(c) if(c>=len/4*3) return 1;
	}
	return 0;
}
