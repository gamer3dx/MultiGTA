//Bank module.
//Created by Iain Gilbert
//Modder: MadCat

#include "base"
#include "player"

forward bankconfig();
forward Bank_OnGameModeInit();
forward IsPlayerAtBank(playerid);
forward BankPickupCheck(playerid);
forward Bank_OnPlayerCommandText(playerid,text[]);

new bankinfoshowed[MAX_PLAYERS] = 0;

new Max_Money_At_Bank=8000000;

#define BanksCount 3

new Banks[BanksCount][CoordInfo] = {
	{1481.000000,-1769.500000,18.700000}, // LS - Commerce
	{-1494.400024,920.099975,7.099997}, // SF - Esplanade East
	{2127.574462,2378.429443,10.820300} // LV - The Emerald Isle
}; 

public bankconfig()
{
	Debug("bank.inc > bankconfig - Start");
	if (!db_Exists(ConfigDB)) db_Create(ConfigDB);

	new temp[MAX_STRING];
	
	set(temp,db_Get(ConfigDB,"Max_Money_At_Bank"));
	if (strlen(temp) > 0) Max_Money_At_Bank = strval(temp); 
	else { valstr(temp,Max_Money_At_Bank); db_Set(ConfigDB,"Max_Money_At_Bank",temp); }
	set(temp,nullstr);
	Debug("bank.inc > bankconfig - Stop");
}

public Bank_OnGameModeInit()
{
	Debug("bank.inc > Bank_OnGameModeInit - Start");
	bankconfig();
	for (new bankid=0;bankid<BanksCount;bankid++)
	{
		CreateStreamPickup(1274,1,Banks[bankid][Coord_X],Banks[bankid][Coord_Y],Banks[bankid][Coord_Z]);
		CreateStreamPickup(1274,1,Banks[bankid][Coord_X],Banks[bankid][Coord_Y],Banks[bankid][Coord_Z]);
		CreateStreamMapIcon(52,Banks[bankid][Coord_X],Banks[bankid][Coord_Y],Banks[bankid][Coord_Z]);
		CreateDMapIcon(52,Banks[bankid][Coord_X],Banks[bankid][Coord_Y],Banks[bankid][Coord_Z],gettext(1743),gettext(1744));
	}
	WriteLog("Banking system Loaded");
	Debug("bank.inc > Bank_OnGameModeInit - Stop");
}

public IsPlayerAtBank(playerid)
{
	Debug("bank.inc > IsPlayerAtBank- Start");
	for (new bankid=0;bankid<BanksCount;bankid++)
	{
		if (IsPlayerInRangeOfPoint(playerid,1.0,Banks[bankid][Coord_X],Banks[bankid][Coord_Y],Banks[bankid][Coord_Z]))
		{
			Debug("bank.inc > IsPlayerAtBank- Stop");
			return 1;
		}
	}
	Debug("bank.inc > IsPlayerAtBank- Stop");
	return 0;
}

public BankPickupCheck(playerid)
{
	Debug("bank.inc > BankPickupCheck - Start");
	if (playerid == INVALID_PLAYER_ID) return;
	if (!IsPlayerConnected(playerid)) return;
	if (!IsPlayerAtBank(playerid)) {bankinfoshowed[playerid] = 0; return;}
	if (bankinfoshowed[playerid] == 1) return;
	bankinfoshowed[playerid] = 1;
	SystemMsg(playerid,COLOUR_INFO,gettext(7));
	SystemMsgFormat(playerid,COLOUR_INFO,gettext(8), Player[playerid][BankMoney]);
	SystemMsg(playerid,COLOUR_INFO,gettext(1590));
	SystemMsg(playerid,COLOUR_INFO,gettext(1591));
	if (PlayerGangid[playerid] != 0){
		new gangid = PlayerGangid[playerid];
		SystemMsg(playerid,COLOUR_INFO,gettext(9));
		SystemMsg(playerid,COLOUR_INFO,gettext(10));
		SystemMsgFormat(playerid,COLOUR_INFO,gettext(11),Gangs[gangid][gang_bank]);
	}
	Debug("bank.inc > BankPickupCheck - Stop");
}



public Bank_OnPlayerCommandText(playerid,text[])
{
	if (!IsPlayerConnected(playerid)) return 0;

	dcmd(get,3,text);
	dcmd(put,3,text);
	dcmd(send,4,text);

	return 0;
}

dcmd_get(playerid,params[])
{
	Debug("bank.inc > Command 'get' - Start");
	if (!IsPlayerAtBank(playerid))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(12));
		Debug("bank.inc > Command 'get' - Stop");
		return 1;
	}

	if (!IsPlayerRegistered(playerid))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(561));
		Debug("bank.inc > Command 'get' - Stop");
		return 1;
	}

	if (strlen(params) == 0)
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(13));
		Debug("bank.inc > Command 'get' - Stop");
		return 1;
	}

	new amount = strval(params);
	if (amount <= 0)
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(13));
		Debug("bank.inc > Command 'get' - Stop");
		return 1;
	}
	if (amount > Player[playerid][BankMoney])
	{
		amount = Player[playerid][BankMoney];
	}

	oGivePlayerMoney(playerid,amount,0);
	Player[playerid][BankMoney] = Player[playerid][BankMoney] - amount;
	new string[MAX_STRING];
	format(string,sizeof(string),gettext(14),amount,Player[playerid][BankMoney]);
	SystemMsg(playerid,COLOUR_OK,string);
	PlayerPlaySoundOnPlayer(playerid,1084);
	new logstring[256];
	format(logstring, sizeof (logstring), "player: %d:  %s: withdrawed $%d from the bank. Current balance: $%d ",playerid,oGetPlayerName(playerid),amount,Player[playerid][BankMoney]);
	WriteLog(logstring);
	Debug("bank.inc > Command 'get' - Stop");
	return 1;
}

dcmd_put(playerid,params[])
{
	Debug("bank.inc > Command 'put'- Start");
	if (!IsPlayerAtBank(playerid))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(12));
		Debug("bank.inc > Command 'put'- Stop");
		return 1;
	}

	if (!IsPlayerRegistered(playerid))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(561));
		Debug("bank.inc > Command 'put' - Stop");
		return 1;
	}

	if (strlen(params) == 0)
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(15));
		Debug("bank.inc > Command 'put'- Stop");
		return 1;
	}
	new amount = strval(params);
	if (amount <= 0)
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(15));
		Debug("bank.inc > Command 'put'- Stop");
		return 1;
	}
	if (oGetPlayerMoney(playerid) < amount)
	{
		amount = oGetPlayerMoney(playerid);
	}
	if (Player[playerid][BankMoney] + amount > Max_Money_At_Bank)
	{
		amount = Max_Money_At_Bank - Player[playerid][BankMoney];
	}
	oGivePlayerMoney(playerid,0 - amount,0);
	Player[playerid][BankMoney] = Player[playerid][BankMoney] + amount;
	new string[MAX_STRING];
	format(string,sizeof(string),gettext(16),amount,Player[playerid][BankMoney]);
	SystemMsg(playerid,COLOUR_OK,string);
	PlayerPlaySoundOnPlayer(playerid,1083);

	new logstring[256];
	format(logstring, sizeof (logstring), "player: %d:  %s: deposited $%d at the bank. Current balance: $%d ",playerid,oGetPlayerName(playerid),amount,Player[playerid][BankMoney]);
	WriteLog(logstring);
	if (Player[playerid][BankMoney] == Max_Money_At_Bank)
	{
		SystemMsg(playerid,COLOUR_INFO,gettext(17));
		set(logstring,nullstr);
		format(logstring, sizeof (logstring), "player: %d:  %s: His bank is full",playerid,oGetPlayerName(playerid));
		WriteLog(logstring);
	}
	Debug("bank.inc > Command 'put'- Stop");
	return 1;
}

dcmd_send(playerid,params[])
{
	Debug("bank.inc > Command 'send' - Start");
	new string[256];
	new giveplayerid, moneys,idx;
	new tmp[256];
	tmp = strcharsplit(params, idx,strchar(" "));

	if(isempty(tmp))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(1578));
		Debug("bank.inc > Command 'send' - Stop");
		return 1;
	}

	if (!IsPlayerAtBank(playerid))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(12));
		Debug("bank.inc > Command 'send' - Stop");
		return 1;
	}

	if (!IsPlayerRegistered(playerid))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(561));
		Debug("bank.inc > Command 'send' - Stop");
		return 1;
	}
		
	giveplayerid = FindPlayerID(tmp);
	if (giveplayerid==-1 ||giveplayerid==-2) { //No such player online.
		new giveplayername[MAX_STRING];
		set(giveplayername,tmp);
		if (strcomp(giveplayername,oGetPlayerName(playerid),true) == 1){
			SystemMsg(playerid,COLOUR_ERROR,gettext(1579));
			Debug("bank.inc > Command 'send' - Stop");
			return 1;
		}

		tmp = strcharsplit(params, idx,strchar(" "));
		if(isempty(tmp))
		{
			SystemMsg(playerid,COLOUR_ERROR,gettext(1578));
			Debug("bank.inc > Command 'send' - Stop");
			return 1;
		}
		moneys = strval(tmp);

		new dinifilename[MAX_STRING];
		format(dinifilename,sizeof(dinifilename),"%s%s.txt",Player_DB,EncodeName(giveplayername));
		if (db_Exists(dinifilename)){

			if (moneys > 0 && Player[playerid][BankMoney] >= moneys)
			{
				Player[playerid][BankMoney] = Player[playerid][BankMoney] - moneys;
				db_Set(dinifilename,"BankMoney",ovalstr(strval(db_Get(dinifilename,"BankMoney"))+moneys));
				format(string,MAX_STRING,gettext(1580), moneys, giveplayername);
				SystemMsg(playerid,COLOUR_MONEY_GOOD,string);
	
				new logstring[256];
				format(logstring, sizeof (logstring), "%s(playerid:%d) has transfered %d to %s",oGetPlayerName(playerid), playerid, moneys, giveplayername);
				WriteLog(logstring);
			}
			else
			{
				SystemMsg(playerid,COLOUR_ERROR,gettext(1581));
			}
		} else {
			SystemMsg(playerid,COLOUR_ERROR,gettext(1577));
		}

	} else {

		if (giveplayerid == playerid){
			SystemMsg(playerid,COLOUR_ERROR,gettext(1579));
			Debug("bank.inc > Command 'send' - Stop");
			return 1;
		}
	
		tmp = strcharsplit(params, idx,strchar(" "));
		if(isempty(tmp))
		{
			SystemMsg(playerid,COLOUR_ERROR,gettext(1578));
			Debug("bank.inc > Command 'send' - Stop");
			return 1;
		}
		moneys = strval(tmp);

		if (IsPlayerConnected(giveplayerid))
		{
			if (moneys > 0 && Player[playerid][BankMoney] >= moneys)
			{
				Player[playerid][BankMoney] = Player[playerid][BankMoney] - moneys;
				Player[giveplayerid][BankMoney] = Player[giveplayerid][BankMoney] + moneys;
				format(string,MAX_STRING,gettext(1582), moneys, oGetPlayerName(giveplayerid),giveplayerid);
				SystemMsg(playerid,COLOUR_MONEY_GOOD,string);
				format(string,MAX_STRING,gettext(1583), moneys, oGetPlayerName(playerid), playerid);
				SystemMsg(giveplayerid,COLOUR_MONEY_GOOD,string);
	
				new logstring[256];
				format(logstring, sizeof (logstring), "%s(playerid:%d) has transfered %d to %s(playerid:%d)",oGetPlayerName(playerid), playerid, moneys, oGetPlayerName(giveplayerid), giveplayerid);
				WriteLog(logstring);
			}
			else
			{
				SystemMsg(playerid,COLOUR_ERROR,gettext(1581));
			}
		}
		else
		{
			format(string,MAX_STRING,gettext(1584),giveplayerid);
			SystemMsg(playerid,COLOUR_ERROR,string);
		}
	}
	Debug("bank.inc > Command 'send' - Stop");
	return 1;
}

