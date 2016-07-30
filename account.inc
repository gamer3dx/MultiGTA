//Account module.
//Created by Iain Gilbert
//Modder: MadCat

#include "base"
#include "privilegies"

forward Account_OnGameModeInit();
forward Account_OnPlayerCommandText(playerid,text[]);
forward Account_OnPlayerConnect(playerid);
forward Account_OnPlayerDisconnect(playerid,reason);
forward AccountConfig();
forward AccountLogin(playerid,hash_pass);
forward KickTimer(playerid);

new Account_DB[MAX_STRING] = "MultiGTA/Accounts/";
new PlayerEmail[MAX_PLAYERS][MAX_NAME];
new PlayerRegistered[MAX_PLAYERS];
new AutoLogin[MAX_PLAYERS];
new Max_Failed_Logins = 3;
new Auto_Login_Enabled = 1;
new Not_Logged_Kick_Time = 30; //If 0 - turned off
new Need_Login_Before_Spawn = 0;
new Min_Pass_Length = 6;

new FailedLogins[MAX_PLAYERS] = 0;

public Account_OnGameModeInit(){
	AccountConfig();
	return 0;
}

public AccountConfig()
{
	Debug("account.inc > AccountConfig - Start");
	
	if (!db_Exists(DatabaseDB)) db_Create(DatabaseDB);
	if (!db_Exists(ConfigDB)) db_Create(ConfigDB);
	new temp[MAX_STRING];
	set(temp,db_Get(DatabaseDB,"Account_DB"));
	if (strlen(temp) > 0) set(Account_DB,temp);
	else { set(temp,Account_DB), db_Set(DatabaseDB,"Account_DB",temp); }
	set(temp,nullstr);
	set(temp,db_Get(ConfigDB,"Max_Failed_Logins"));
	if (strlen(temp) > 0) Max_Failed_Logins = strval(temp); 
	else { valstr(temp,Max_Failed_Logins); db_Set(ConfigDB,"Max_Failed_Logins",temp); }
	set(temp,nullstr);
	set(temp,db_Get(ConfigDB,"Auto_Login_Enabled"));
	if (strlen(temp) > 0) Auto_Login_Enabled = strval(temp); 
	else { valstr(temp,Auto_Login_Enabled); db_Set(ConfigDB,"Auto_Login_Enabled",temp); }
	set(temp,nullstr);
	set(temp,db_Get(ConfigDB,"Not_Logged_Kick_Time"));
	if (strlen(temp) > 0) Not_Logged_Kick_Time = strval(temp); 
	else { valstr(temp,Not_Logged_Kick_Time); db_Set(ConfigDB,"Not_Logged_Kick_Time",temp); }
	set(temp,nullstr);
	set(temp,db_Get(ConfigDB,"Need_Login_Before_Spawn"));
	if (strlen(temp) > 0) Need_Login_Before_Spawn = strval(temp); 
	else { valstr(temp,Need_Login_Before_Spawn); db_Set(ConfigDB,"Need_Login_Before_Spawn",temp); }
	set(temp,nullstr);
	set(temp,db_Get(ConfigDB,"Min_Pass_Length"));
	if (strlen(temp) > 0) Min_Pass_Length = strval(temp); 
	else { valstr(temp,Min_Pass_Length); db_Set(ConfigDB,"Min_Pass_Length",temp); }
	set(temp,nullstr);
	Debug("account.inc > AccountConfig - Stop");
	return;
}

public AccountLogin(playerid,hash_pass)
{
	Debug("account.inc > AccountLogin - Start");
	if (PlayerRegistered[playerid])
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(39));
		Debug("account.inc > AccountLogin - Stop");
		return false;
	}

	if (account_login_db_dini(playerid,hash_pass) == 0) return 0;

	PlayerRegistered[playerid] = 1;

	PlayerLoadStats(playerid);
	if (PlayerPriv[playerid][IsModerator] == 1){
		ModMsg(playerid,gettext(40));
	} else {
		SystemMsg(playerid,COLOUR_OK,gettext(41));
	}
	GivePlayerOwnedWeapons(playerid);

	new playerip[IP_LEN]; playerip = ReturnPlayerIp(playerid);

	new dinifilename[MAX_STRING];
	format(dinifilename,sizeof(dinifilename),"%s%s.txt",Account_DB,EncodeName(oGetPlayerName(playerid)));
	db_Set(dinifilename,"Last_Login",GetNowAsString());
	db_Set(dinifilename,"Last_IP",playerip);
	AutoLogin[playerid] = strval(db_Get(dinifilename,"AutoLogin"));

	new logstring[256];
	format(logstring, sizeof (logstring), "player: %d:  %s: logged in successfully ",playerid,oGetPlayerName(playerid));
	WriteLog(logstring);
	Debug("account.inc > AccountLogin - Stop");
	return true;

}

account_login_db_dini(playerid,hash_pass)
{
	Debug("account.inc > account_login_db_dini - Start");
	new dinifilename[MAX_STRING];
	format(dinifilename,sizeof(dinifilename),"%s%s.txt",Account_DB,EncodeName(oGetPlayerName(playerid)));

	if (!db_Exists(dinifilename))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(42));
		SystemMsg(playerid,COLOUR_INFO,gettext(43));
		Debug("account.inc > account_login_db_dini - Stop");
		return 0;
	}

	if (hash_pass != strval(db_Get(dinifilename,"Password")))
	{
		new logstring[256];
		format(logstring, sizeof (logstring), "player_login: failed: incorrect password ID:%d NAME:%s :",playerid,oGetPlayerName(playerid));
		WriteLog(logstring);
		SystemMsg(playerid,COLOUR_ERROR,gettext(44));
		FailedLogins[playerid]++;
		if (FailedLogins[playerid] > Max_Failed_Logins){
			SystemMsg(playerid,COLOUR_ERROR,gettext(45));
			KickPlayer(playerid,-1,gettext(46)); // kick them
		}
		Debug("account.inc > account_login_db_dini - Stop");
		return false;
	}
	if (strval(db_Get(dinifilename,"Locked")) == 1){
		SystemMsg(playerid,COLOUR_ERROR,gettext(47));
		KickPlayer(playerid,-1,gettext(909));
		Debug("account.inc > account_login_db_dini - Stop");
		return false;
	}
	Debug("account.inc > account_login_db_dini - Stop");
	set(PlayerEmail[playerid],db_Get(dinifilename,"Email"));
	return true;
}

public Account_OnPlayerCommandText(playerid,text[]) // process account commands
{
	if (!IsPlayerConnected(playerid)) return 0;
	dcmd(register,8,text);
	dcmd(login,5,text);
	dcmd(changepass,10,text);
	dcmd(savechar,8,text);
	dcmd(autologin,9,text);
	return 0;
}

dcmd_register(playerid,params[])
{
	Debug("account.inc > Command 'register' - Start");

	if (PlayerRegistered[playerid])
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(39));
		Debug("account.inc > Command 'register' - Stop");
		return 1;
	}
	new idx;
	new password[MAX_NAME];
	new email[MAX_NAME];
	set(password,strcharsplit(params, idx,strchar(" "))); 
	set(email,strcharsplit(params, idx,strchar(" ")));
	if (strfind(email,charstr(DBSeperator),false) != -1)
	{
		new logstring[256];
		format(logstring, sizeof (logstring), "%s: register_account: fail: DB seperator found in email",oGetPlayerName(playerid));
		WriteLog(logstring);
		
		SystemMsg(playerid,COLOUR_ERROR,gettext(48));
		Debug("account.inc > Command 'register' - Stop");
		return 1;
	}

	if(isempty(password))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(49));
		Debug("account.inc > Command 'register' - Stop");
		return 1;
	}

	if(isempty(email))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(1115));
		Debug("account.inc > Command 'register' - Stop");
		return 1;
	}

	if (strlen(password) < Min_Pass_Length){
		SystemMsgFormat(playerid,COLOUR_ERROR,gettext(1559),Min_Pass_Length);
		Debug("account.inc > Command 'register' - Stop");
		return 1;
	}

	new dinifilename[MAX_STRING];
	format(dinifilename,sizeof(dinifilename),"%s%s.txt",Account_DB,EncodeName(oGetPlayerName(playerid)));

	if (db_Exists(dinifilename))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(50));
		Debug("account.inc > Command 'register' - Stop");
		return 1;
	}

	db_Create(dinifilename);
	new playerip[IP_LEN]; playerip = ReturnPlayerIp(playerid);
	#if DB_TYPE == 1 && DINI_OPTIMIZE == 1
	new File:file = fopen(dinifilename,io_write);
	if (file){
		new writeit[MAX_STRING];
		format(writeit,MAX_STRING,"Name=%s\r\n",oGetPlayerName(playerid)); fwrite(file,writeit);
		format(writeit,MAX_STRING,"Password=%d\r\n",num_hash(password)); fwrite(file,writeit);
		format(writeit,MAX_STRING,"Email=%s\r\n",email); fwrite(file,writeit);
		format(writeit,MAX_STRING,"Creation_Date=%s\r\n",GetNowAsString()); fwrite(file,writeit);
		format(writeit,MAX_STRING,"Last_Login=%s\r\n",GetNowAsString()); fwrite(file,writeit);
		format(writeit,MAX_STRING,"Last_IP=%s\r\n",playerip); fwrite(file,writeit);
		format(writeit,MAX_STRING,"AutoLogin=0\r\n",0); fwrite(file,writeit);
	}
	#else
	db_BeforeBigSaving(dinifilename);
	db_Set(dinifilename,"Name",oGetPlayerName(playerid));
	db_Set(dinifilename,"Password",ovalstr(num_hash(password)));
	db_Set(dinifilename,"Email",email);
	db_Set(dinifilename,"Creation_Date",GetNowAsString());
	db_Set(dinifilename,"Last_Login",GetNowAsString());
	db_Set(dinifilename,"Last_IP",playerip);
	db_Set(dinifilename,"AutoLogin",ovalstr(0));
	db_AfterBigSaving(dinifilename);
	#endif

	CreatePlayer(playerid);
	SystemMsg(playerid,COLOUR_OK,gettext(51));
	AccountLogin(playerid,num_hash(password));

	new logstring[256];
	format(logstring, sizeof (logstring), "create_account: success %d  %s",playerid,oGetPlayerName(playerid));
	WriteLog(logstring);
	Debug("account.inc > Command 'register' - Stop");
	return 1;
}

dcmd_login(playerid,params[])
{
	Debug("account.inc > Command 'login' - Start");
	if(isempty(params))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(55));
		return 1;
	}
	AccountLogin(playerid,num_hash(params));
	Debug("account.inc > Command 'login' - Stop");
	return 1;
}

dcmd_changepass(playerid,params[])
{
	Debug("account.inc > Command 'changepass' - Start");
	
	if (!IsPlayerRegistered(playerid))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(53));
		Debug("account.inc > Command 'changepass' - Stop");
		return 1;
	}

	if(isempty(params))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(52));
		Debug("account.inc > Command 'changepass' - Stop");
		return 1;
	}

	if (strlen(params) < Min_Pass_Length){
		SystemMsgFormat(playerid,COLOUR_ERROR,gettext(1559),Min_Pass_Length);
		Debug("account.inc > Command 'register' - Stop");
		return 1;
	}

	new dinifilename[MAX_STRING];
	format(dinifilename,sizeof(dinifilename),"%s%s.txt",Account_DB,EncodeName(oGetPlayerName(playerid)));
	db_Set(dinifilename,"Password",ovalstr(num_hash(params)));
	SystemMsg(playerid,COLOUR_OK,gettext(54));
	new logstring[256];
	format(logstring, sizeof (logstring), "change_password: success %d  %s",playerid,oGetPlayerName(playerid));
	WriteLog(logstring);
	Debug("account.inc > Command 'changepass' - Stop");
	return 1;
}

dcmd_savechar(playerid,params[])
{
	#pragma unused params
	Debug("account.inc > Command 'savechar' - Start");
	if (IsPlayerRegistered(playerid))
	{
		PlayerSave(playerid);
	}
	Debug("account.inc > Command 'savechar' - Stop");
	return 1;
}

dcmd_autologin(playerid,params[])
{
	#pragma unused params
	if (Auto_Login_Enabled == 0) return 0;

	Debug("account.inc > Command 'autologin' - Start");

	if (!IsPlayerRegistered(playerid))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(53));
		Debug("account.inc > Command 'autologin' - Stop");
		return 1;
	}
	new logstring[256];
	new dinifilename[MAX_STRING];
	format(dinifilename,sizeof(dinifilename),"%s%s.txt",Account_DB,EncodeName(oGetPlayerName(playerid)));
	if (AutoLogin[playerid] == 0){
		db_Set(dinifilename,"AutoLogin",ovalstr(1));
		AutoLogin[playerid] = 1;
		SystemMsg(playerid,COLOUR_OK,gettext(1466));
		format(logstring, sizeof (logstring), "autologin turned on (%d - %s)",playerid,oGetPlayerName(playerid));
	} else {
		db_Set(dinifilename,"AutoLogin",ovalstr(0));
		AutoLogin[playerid] = 0;
		SystemMsg(playerid,COLOUR_OK,gettext(1467));
		format(logstring, sizeof (logstring), "autologin turned off (%d - %s)",playerid,oGetPlayerName(playerid));
	}

	WriteLog(logstring);
	Debug("account.inc > Command 'autologin' - Stop");
	return 1;
}

public Account_OnPlayerConnect(playerid){
	if (Auto_Login_Enabled == 1){
		new playerip[IP_LEN];  playerip = ReturnPlayerIp(playerid);
		new dinifilename[MAX_STRING];
		format(dinifilename,sizeof(dinifilename),"%s%s.txt",Account_DB,EncodeName(oGetPlayerName(playerid)));
		if (db_Exists(dinifilename)){
			if(strval(db_Get(dinifilename,"AutoLogin")) == 1){
				if(strcomp(playerip,db_Get(dinifilename,"Last_IP"),false) == 1){
					AccountLogin(playerid,strval(db_Get(dinifilename,"Password")));
				} else {
					SendLoginMessage(playerid);
				}
			} else {
				SendLoginMessage(playerid);
			}
		} else {
			SendLoginMessage(playerid);
		}
	} else {
		SendLoginMessage(playerid);
	}
	return 0;
}

public Account_OnPlayerDisconnect(playerid,reason){
	set(PlayerEmail[playerid],nullstr);
	PlayerRegistered[playerid] = false;
	AutoLogin[playerid] = FailedLogins[playerid] = 0;
	return 0;
}

public KickTimer(playerid)
{
	if(!IsPlayerRegistered(playerid)) KickPlayer(playerid,-1,gettext(353));
}

stock SendLoginMessage(playerid){
	new dinifilename[MAX_STRING];
	format(dinifilename,sizeof(dinifilename),"%s%s.txt",Account_DB,EncodeName(oGetPlayerName(playerid)));
	if (db_Exists(dinifilename)){
		SystemMsg(playerid,COLOUR_INFO,gettext(56));
		SystemMsg(playerid,COLOUR_INFO,gettext(57));
		if (Not_Logged_Kick_Time > 0){
			SetTimerEx("KickTimer",Not_Logged_Kick_Time*1000,0,"i",playerid);
			SystemMsgFormat(playerid,COLOUR_INFO,gettext(355),Not_Logged_Kick_Time);
		}
	} else {
		SystemMsg(playerid,COLOUR_INFO,gettext(58));
		SystemMsg(playerid,COLOUR_INFO,gettext(59));
	}
}

IsLoggedBeforeSpawn(playerid){
	if (Need_Login_Before_Spawn == 1){
		if (!IsPlayerRegistered(playerid)) return 1;
	}
	return 0;	
}