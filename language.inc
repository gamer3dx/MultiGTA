//Language module
//Created by GTO Dev Team
//Modified by MadCat

#define gettext(%1) Langs[%1]

new Langs[1800][150];

forward LanguageConfig();
forward Language_OnGameModeInit();
forward lang_read(filename[]);

new Languages_DB[MAX_STRING] = "MultiGTA/Languages/";
new Language[MAX_STRING] = "English";

public LanguageConfig()
{
	Debug("language.inc > LanguageConfig - Start");
	
	if (!db_Exists(DatabaseDB)) return;
	new temp[MAX_STRING];
	set(temp,db_Get(DatabaseDB,"Languages_DB"));
	if (strlen(temp) > 0) set(Languages_DB,temp); 
	else { set(temp,Languages_DB); db_Set(DatabaseDB,"Languages_DB",temp);  }
	set(temp,nullstr);
	set(temp,db_Get(ConfigDB,"Language"));
	if (strlen(temp) > 0) set(Language,temp); 
	else { set(temp,Language); db_Set(ConfigDB,"Language",temp);  }
	set(temp,nullstr);
	Debug("language.inc > LanguageConfig - Stop");
	return;
}

public Language_OnGameModeInit(){
	LanguageConfig();
	new langfile[MAX_STRING];
	format(langfile,MAX_STRING,"%s%s.txt",Languages_DB,Language);
	lang_read(langfile);
	return 0;
}

public lang_read(filename[]) {
	new File:fohnd, tmpres[MAX_STRING], tmp1[MAX_STRING], tmp2[MAX_STRING];
	fohnd=fopen(filename,io_read);
	if (!fohnd){WriteLog("Cannot open language file!"); SendRconCommand("exit"); return;}
	while (fread(fohnd,tmpres,sizeof(tmpres))){
		if (strlen(tmpres) == 0) return;
		if (strfind(tmpres, "//",true) != -1) continue;
		if (strfind(tmpres, ";", true) != -1) 
		{
			strmid(tmp1, tmpres, 0, strfind(tmpres, ";", true));
			new idx;
			strcharsplit(tmpres, idx,strchar(";"));
			set(tmp2,strcharsplit(tmpres, idx,strchar(";")));
			StripNewLine(tmp2);
			set(Langs[strval(tmp1)],tmp2);
			set(tmp2,nullstr);
		} 
	}
	fclose(fohnd);
	WriteLog("Language Loaded");
}