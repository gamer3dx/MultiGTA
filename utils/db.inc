//Database module
//Created by MadCat
#if defined _db_included
  #endinput
#endif

#define _db_included
#pragma library db

stock db_Exists(filename[]) {
	#if DB_TYPE == 2
		if (fexist(filename)) return true;
		return false;
	#else 
		#if DB_TYPE == 1
			if (fexist(filename)) return true;
			return false;
		#else
			#error Unknown DB Type, check base_defsettings.inc and recompile mode
		#endif
	#endif
}

stock db_Remove(filename[]) {
	#if DB_TYPE == 2
		return djRemoveFile(filename);
	#else 
		#if DB_TYPE == 1
			return dini_Remove(filename);
		#else
			#error Unknown DB Type, check base_defsettings.inc and recompile mode
		#endif
	#endif
}

stock db_Create(filename[]) {
	#if DB_TYPE == 2
		return djCreateFile(filename);
	#else 
		#if DB_TYPE == 1
			return dini_Create(filename);
		#else
			#error Unknown DB Type, check base_defsettings.inc and recompile mode
		#endif
	#endif
}

stock db_Set(filename[],key[],value[]) {
	#if DB_TYPE == 2
		return djSet(filename,key,value);
	#else 
		#if DB_TYPE == 1
			return dini_Set(filename,key,value);
		#else
			#error Unknown DB Type, check base_defsettings.inc and recompile mode
		#endif
	#endif
}


stock db_IntSet(filename[],key[],value) {
	#if DB_TYPE == 2
		return djSetInt(filename,key,value);
	#else 
		#if DB_TYPE == 1
			return dini_IntSet(filename,key,value);
		#else
			#error Unknown DB Type, check base_defsettings.inc and recompile mode
		#endif
	#endif
}

stock db_Int(filename[],key[]) {
	#if DB_TYPE == 2
		return djInt(filename,key);
	#else 
		#if DB_TYPE == 1
			return dini_Int(filename,key);
		#else
			#error Unknown DB Type, check base_defsettings.inc and recompile mode
		#endif
	#endif
}

stock db_FloatSet(filename[],key[],Float:value) {
	#if DB_TYPE == 2
		return djSetFloat(filename,key,value);
	#else 
		#if DB_TYPE == 1
			return dini_FloatSet(filename,key,value);
		#else
			#error Unknown DB Type, check base_defsettings.inc and recompile mode
		#endif
	#endif
}

stock Float:db_Float(filename[],key[]) {
	#if DB_TYPE == 2
		return djFloat(filename,key);
	#else 
		#if DB_TYPE == 1
			return dini_Float(filename,key);
		#else
			#error Unknown DB Type, check base_defsettings.inc and recompile mode
		#endif	
	#endif
}

stock db_Unset(filename[],key[]) {
	#if DB_TYPE == 2
		return djUnset(filename,key);
	#else 
		#if DB_TYPE == 1
			return dini_Unset(filename,key);
		#else
			#error Unknown DB Type, check base_defsettings.inc and recompile mode
		#endif
	#endif
}

stock db_Get(filename[],key[]) {
	#if DB_TYPE == 2
		new var[DJSON_MAX_STRING];
		set(var,dj(filename,key));
		return var;
	#else 
		#if DB_TYPE == 1
			new var[MAX_STRING];
			set(var,dini_Get(filename,key));
			return var;
		#else
			#error Unknown DB Type, check base_defsettings.inc and recompile mode
		#endif
	#endif
}


stock db_Isset(filename[],key[]) {
	#if DB_TYPE == 2
		return djIsSet(filename,key);
	#else 
		#if DB_TYPE == 1
			return dini_Isset(filename,key);
		#else
			#error Unknown DB Type, check base_defsettings.inc and recompile mode
		#endif
	#endif
}

stock db_BeforeBigSaving(filename[]) {
	#pragma unused filename
	#if DB_TYPE == 2
		djAutocommit(false);		
	#else 
		#if DB_TYPE == 1
			return;
		#else
			#error Unknown DB Type, check base_defsettings.inc and recompile mode
		#endif
	#endif
}

stock db_AfterBigSaving(filename[]) {
	#if DB_TYPE == 2
		djCommit(filename);
		djAutocommit(true);
	#else 
		#if DB_TYPE == 1
			#pragma unused filename	
		#else
			#error Unknown DB Type, check base_defsettings.inc and recompile mode
		#endif
	#endif
	return;
}


stock db_OnGameModeInit(){
	dmap_Config(); //Hack :)
	#if DB_TYPE == 2
		djson_GameModeInit();
		djAutocommit(false);
	#else
		if (Use_DMap == 1) djson_GameModeInit();
	#endif
	return 0;
}

stock db_OnGameModeExit(){
	#if DB_TYPE == 2
		djson_GameModeExit();
	#endif
	return 0;
}