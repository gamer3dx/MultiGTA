//Mobile phone module.
//Created by MadCat
//Idea from PEN1

#include "moderation\commands"

forward PhoneConfig();
forward Phone_OnGameModeInit();
forward Phone_OnPlayerConnect(playerid);
forward Phone_OnPlayerDisconnect(playerid);
forward Phone_OnPlayerDeath(playerid,killerid,reason);
forward PhoneTimer();
forward Phone_OnPlayerCommandText(playerid,text[]);
forward Phone_OnPlayerText(playerid,text[]);

new TimerPhone;

new PlayerCalling[MAX_PLAYERS];
new PlayerCalled[MAX_PLAYERS];
new PlayerChatPhone[MAX_PLAYERS];
new PlayerWaiting[MAX_PLAYERS];
new PlayerWaitingTime[MAX_PLAYERS];

new SMS_Cost = 1;

public Phone_OnGameModeInit(){
	PhoneConfig();
	TimerPhone = SetTimer("PhoneTimer",1000,1);
	WriteLog("Phone System Loaded");
	return 0;
}

public PhoneConfig()
{
	Debug("phone.inc > PhoneConfig - Start");
	if (!db_Exists(ConfigDB)) db_Create(ConfigDB);

	new temp[MAX_STRING];
	
	set(temp,db_Get(ConfigDB,"SMS_Cost"));
	if (strlen(temp) > 0) SMS_Cost = strval(temp); 
	else { valstr(temp,SMS_Cost); db_Set(ConfigDB,"SMS_Cost",temp); }
	set(temp,nullstr);
	Debug("phone.inc > PhoneConfig - Stop");
}

public Phone_OnPlayerDeath(playerid,killerid,reason){
	Debug("phone.inc > Phone_OnPlayerDeath - Start");
	if(PlayerChatPhone[playerid] != -1){
	    	SystemMsg(playerid,COLOUR_PHONE,gettext(18));
		SystemMsg(PlayerChatPhone[playerid],COLOUR_PHONE,gettext(19));
		PlayerCalling[PlayerChatPhone[playerid]] = -1;
		PlayerCalled[PlayerChatPhone[playerid]] = -1;
		PlayerChatPhone[PlayerChatPhone[playerid]] = -1;
		PlayerCalling[playerid] = -1;
		PlayerCalled[playerid] = -1;
		PlayerChatPhone[playerid] = -1;
	}
	Debug("phone.inc > Phone_OnPlayerDeath - Stop");
	return 0;
}

public Phone_OnPlayerConnect(playerid){
	Debug("phone.inc > Phone_OnPlayerConnect - Start");
	PlayerCalling[playerid] = -1;
	PlayerCalled[playerid] = -1;
	PlayerChatPhone[playerid] = -1;
	Debug("phone.inc > Phone_OnPlayerConnect - Stop");
	return 0;
}

public Phone_OnPlayerDisconnect(playerid){
	Debug("phone.inc > Phone_OnPlayerDisconnect - Start");
	if(PlayerChatPhone[playerid] != -1){
	    	SystemMsg(PlayerChatPhone[playerid],COLOUR_PHONE,gettext(20));
	}
	PlayerCalling[playerid] = -1;
	PlayerCalled[playerid] = -1;
	PlayerChatPhone[playerid] = -1;
	PlayerWaiting[playerid] = 0;
	PlayerWaitingTime[playerid] = 0;
	Debug("phone.inc > Phone_OnPlayerDisconnect - Stop");
	return 0;
}

public PhoneTimer(){
	Debug("phone.inc > PhoneTimer - Start");
	new string[MAX_STRING];
	for(new playerid;playerid<MAX_PLAYERS_EX;playerid++){
		if(IsPlayerConnected(playerid)){
		    	if(PlayerWaiting[playerid]){
		       		if(PlayerWaitingTime[playerid] == 15){
		            		SystemMsg(playerid,COLOUR_PHONE,gettext(21));
					format(string,MAX_STRING,gettext(22));
		            		SystemMsg(PlayerCalled[playerid],COLOUR_PHONE,string);
		            		PlayerWaiting[playerid] = 0;
		            		PlayerCalling[PlayerCalled[playerid]] = -1;
		            		PlayerCalled[playerid] = -1;
		        	}
		        	if(PlayerWaitingTime[playerid] == 3 || PlayerWaitingTime[playerid] == 6 || PlayerWaitingTime[playerid] == 9 || PlayerWaitingTime[playerid] == 12){
					SystemMsg(playerid,COLOUR_PHONE,gettext(23));
		        	}
		        	PlayerWaitingTime[playerid]++;
		    	}
		
		}
	}
	Debug("phone.inc > PhoneTimer - Stop");
}

public Phone_OnPlayerCommandText(playerid,text[])
{
	if (!IsPlayerRegistered(playerid)) return 0;
	dcmd(accept,6,text);
	dcmd(hangup,6,text);
	dcmd(call,4,text);
	dcmd(sms,3,text);
	return 0;
}

dcmd_accept(playerid,params[])
{
	#pragma unused params
	Debug("phone.inc > Command 'accept' - Start");
	if(PlayerCalling[playerid] == -1) {
		SystemMsg(playerid,COLOUR_ERROR,gettext(24));
		Debug("phone.inc > Command 'accept' - Stop");
		return 1;
	}
	new string[MAX_STRING];
	format(string,MAX_STRING,gettext(25),oGetPlayerName(PlayerCalling[playerid]));
	SystemMsg(playerid,COLOUR_PHONE,string);
	format(string,MAX_STRING,gettext(26),oGetPlayerName(playerid));
	SystemMsg(PlayerCalling[playerid],COLOUR_PHONE,string);
	PlayerChatPhone[playerid] = PlayerCalling[playerid];
	PlayerChatPhone[PlayerCalling[playerid]] = playerid;
	PlayerWaiting[PlayerCalling[playerid]] = 0;
	PlayerWaitingTime[PlayerCalling[playerid]] = 0;
	Debug("phone.inc > Command 'accept' - Stop");
	return 1;
}

dcmd_hangup(playerid,params[])
{
	#pragma unused params
	if(PlayerChatPhone[playerid] == -1) {
		SystemMsg(playerid,COLOUR_ERROR,gettext(27));
		Debug("phone.inc > Command 'hangup' - Stop");
		return 1;
	}
  	SystemMsg(playerid,COLOUR_PHONE,gettext(28));
	SystemMsg(PlayerChatPhone[playerid],COLOUR_PHONE,gettext(29));
	PlayerCalling[PlayerChatPhone[playerid]] = -1;
	PlayerCalled[PlayerChatPhone[playerid]] = -1;
	PlayerChatPhone[PlayerChatPhone[playerid]] = -1;
	PlayerCalling[playerid] = -1;
	PlayerCalled[playerid] = -1;
	PlayerChatPhone[playerid] = -1;
	PlayerWaiting[PlayerChatPhone[playerid]] = -1;
	PlayerWaitingTime[PlayerChatPhone[playerid]] = -1;
	Debug("phone.inc > Command 'hangup' - Stop");
	return 1;
}

dcmd_call(playerid,params[])
{
	Debug("phone.inc > Command 'call' - Start");
	new string[256];
	new giveplayerid;

	if(isempty(params))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(30));
		Debug("phone.inc > Command 'call' - Stop");
		return 1;
	}
		
	giveplayerid = FindPlayerID(params);
	if (giveplayerid==-1) {Debug("phone.inc > Command 'call' - Stop"); return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1571),params); }
	if (giveplayerid==-2) {Debug("phone.inc > Command 'call' - Stop"); return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1572),params); }


	if (giveplayerid == playerid){
		SystemMsg(playerid,COLOUR_ERROR,gettext(31));
		Debug("phone.inc > Command 'call' - Stop");
		return 1;
	}

	if (!IsPlayerConnected(giveplayerid)){
		SystemMsg(playerid,COLOUR_ERROR,gettext(32));
		Debug("phone.inc > Command 'call' - Stop");
		return 1;
	}

	if(PlayerChatPhone[giveplayerid] != -1){
		SystemMsg(playerid,COLOUR_ERROR,gettext(33));
		Debug("phone.inc > Command 'call' - Stop");
		return 1;	
	}

	format(string,MAX_STRING,gettext(34),oGetPlayerName(playerid));
	SystemMsg(giveplayerid,COLOUR_PHONE,string);
	PlayerCalling[giveplayerid] = playerid;
	PlayerCalled[playerid] = giveplayerid;
	SystemMsg(playerid,COLOUR_PHONE,gettext(23));
	PlayerWaiting[playerid] = 1;
	PlayerWaitingTime[playerid] = 0;
	Debug("phone.inc > Command 'call' - Stop");
	return 1;
}

dcmd_sms(playerid,params[])
{
	Debug("phone.inc > Command 'sms' - Start");

	if(isempty(params))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(35));
		Debug("phone.inc > Command 'sms' - Stop");
		return 1;
	}

	new idx=0;
	new giveplayername[MAX_STRING];
	giveplayername = strcharsplit(params, idx,strchar(" "));
	new giveplayerid = FindPlayerID(giveplayername);
	if (giveplayerid==-1) { Debug("phone.inc > Command 'sms' - Stop"); return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1571),giveplayername); }
	if (giveplayerid==-2) { Debug("phone.inc > Command 'sms' - Stop"); return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1572),giveplayername); }
	if (strlen(giveplayername)+1>strlen(params)) { Debug("phone.inc > Command 'sms' - Stop"); return SystemMsg(playerid,COLOUR_ERROR,gettext(35)); }
	
	if (giveplayerid == playerid){
		SystemMsg(playerid,COLOUR_ERROR,gettext(36));
		Debug("phone.inc > Command 'sms' - Stop");
		return 1;
	}

	new message[MAX_STRING];
	format(message,MAX_STRING,"%s",params[strlen(giveplayername)]);

	if(isempty(message))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(35));
		Debug("phone.inc > Command 'sms' - Stop");
		return 1;
	}

	if (Player[playerid][BankMoney] < SMS_Cost){
		SystemMsg(playerid,COLOUR_ERROR,gettext(1689));
		Debug("phone.inc > Command 'sms' - Stop");
		return 1;
	}

	if (IsPlayerConnected(giveplayerid))
	{
		new string[MAX_STRING];
		format(string,MAX_STRING,gettext(37),oGetPlayerName(playerid),message);
		SystemMsg(giveplayerid,COLOUR_PHONE,string);
		format(string,MAX_STRING,gettext(38),oGetPlayerName(giveplayerid),message);
		SystemMsg(playerid,COLOUR_PHONE,string);
		Player[playerid][BankMoney] = Player[playerid][BankMoney] - SMS_Cost;
		format(string,MAX_STRING,gettext(1463),oGetPlayerName(playerid),oGetPlayerName(giveplayerid),message);
		for (new id=0;id<MAX_PLAYERS_EX;id++)
		{
   			if (IsPlayerConnected(id) && id != playerid)
   			{
				if (ModSpySMS[id] == true){
        				ModMsg(id,string);
				}
			}
		}
	}
	else
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(32));
	}
	Debug("phone.inc > Command 'sms' - Stop");
	return 1;
}

public Phone_OnPlayerText(playerid,text[]){
	if(PlayerChatPhone[playerid] != -1){
		new string[MAX_STRING];
		format(string,MAX_STRING,gettext(1315),oGetPlayerName(playerid),text);
		SystemMsg(PlayerChatPhone[playerid],COLOUR_PHONE,string);
		SystemMsg(playerid,COLOUR_PHONE,string);
		WriteChatLog(playerid,text,"phone");
	        return 1;
	}
	return 0;
}