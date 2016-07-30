//Pay'n'Spray Businesses module.
//Created by MadCat
//Based on Original Business Handler from GTO, created by Iain Gilbert
//Coords and some part of code from GTAT

#include "base"
#include "player"

forward PayNSpray_OnGameModeInit();
forward IsPlayerAtPayNSpray(playerid);
forward PayNSprayPickupCheck(playerid);
forward GetPayNSprayID(playerid);
forward ResprayVehicle(playerid);
forward SellPayNSpray(playerid);
forward CollectPayNSpray(playerid);
forward BuyPayNSpray(playerid);
forward SetResprayPrice(playerid,value);

new paynsprayinfoshowed[MAX_PLAYERS] = 0;

new PayNSprayBusinessDB[MAX_STRING] = "MultiGTA/Businesses/";

#define MAX_PAYNSPRAYS 10

enum PayNSprayInfo {
    Name[MAX_STRING],
    Float:Coord_X,
    Float:Coord_Y,
    Float:Coord_Z,
    Level,
    Cost,
    ResprayPrice,
    MaxPrice,
    ItemsInStock,
    Components,
    MoneyNow,
    Owner[MAX_NAME]
}

new PayNSprays[MAX_PAYNSPRAYS][PayNSprayInfo] = {

	{"PayNSpray - (LS) Idlewood",2074.78,-1827.48,13.55,10,100000,100,300,1500,0,0,"Server"},
	{"PayNSpray - (LS) Temple",1028.89,-1032.34,31.92,10,100000,100,300,1500,0,0,"Server"},
	{"PayNSpray - (LS) Santa Maria Beach",484.20,-1731.86,11.09,10,100000,100,300,1500,0,0,"Server"},
	{"PayNSpray - (SF) Downtown",-1899.1246,274.1285,41.0391,10,100000,100,300,1500,0,0,"Server"},
	{"PayNSpray - (SF) Juniper Hollow",-2431.2671,1030.6918,50.3906,10,100000,100,300,1500,0,0,"Server"},
	{"PayNSpray - Dillimore",723.3140,-466.2574,16.3359,10,100000,100,300,1500,0,0,"Server"},
	{"PayNSpray - El Quebrados",-1425.7402,2593.4377,55.7693,10,100000,100,300,1500,0,0,"Server"},
	{"PayNSpray - Fort Carson",-95.5815,1107.9210,19.7422,10,100000,100,300,1500,0,0,"Server"},
	{"PayNSpray - (LV) Royal Casino",2389.3884,1480.8855,10.8125,10,100000,100,300,1500,0,0,"Server"},
	{"PayNSpray - (LV) Redsands East",1965.1838,2158.5244,10.8203,10,100000,100,300,1500,0,0,"Server"}		
}; 

PayNSpraysLoadAll()
{
	Debug("businesses/paynspray.inc > PayNSpraysLoadAll - Start");
	
	new temp[MAX_STRING];
	if (!db_Exists(DatabaseDB)) db_Create(DatabaseDB);
	if (!db_Isset(DatabaseDB,"PayNSpray_Business_DB")) db_Set(DatabaseDB,"PayNSpray_Business_DB",PayNSprayBusinessDB);
	set(temp,db_Get(DatabaseDB,"PayNSpray_Business_DB"));
	if (strlen(temp) > 0) set(PayNSprayBusinessDB,temp);
	if (!db_Exists(PayNSprayBusinessDB))
	{

		PayNSpraysSaveAll();

		return;
	}
	for (new i=0;i<MAX_PAYNSPRAYS;i++)
	{
		new PayNSprayBusinessDBname[MAX_STRING];
		format(PayNSprayBusinessDBname,sizeof(PayNSprayBusinessDBname),"%s%s.txt",PayNSprayBusinessDB,PayNSprays[i][Name]);
		if (!db_Exists(PayNSprayBusinessDBname))
		{
			PayNSpraysSaveAll();
		}
		set(temp,db_Get(PayNSprayBusinessDBname,"Name"));
		if (strlen(temp) > 0) set(PayNSprays[i][Name],temp);
		set(temp,nullstr);
		set(temp,db_Get(PayNSprayBusinessDBname,"Level"));
		if (strlen(temp) > 0) PayNSprays[i][Level] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(PayNSprayBusinessDBname,"Cost"));
		if (strlen(temp) > 0) PayNSprays[i][Cost] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(PayNSprayBusinessDBname,"ResprayPrice"));
		if (strlen(temp) > 0) PayNSprays[i][ResprayPrice] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(PayNSprayBusinessDBname,"MaxPrice"));
		if (strlen(temp) > 0) PayNSprays[i][MaxPrice] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(PayNSprayBusinessDBname,"ItemsInStock"));
		if (strlen(temp) > 0) PayNSprays[i][ItemsInStock] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(PayNSprayBusinessDBname,"MoneyNow"));
		if (strlen(temp) > 0) PayNSprays[i][MoneyNow] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(PayNSprayBusinessDBname,"Components"));
		if (strlen(temp) > 0) PayNSprays[i][Components] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(PayNSprayBusinessDBname,"Owner"));
		if (strlen(temp) > 0) set(PayNSprays[i][Owner],temp);
		set(temp,nullstr);
	}
	Debug("businesses/paynspray.inc > PayNSpraysLoadAll - Stop");
	return;

}

PayNSpraysSaveAll()
{
	Debug("businesses/paynspray.inc > PayNSpraysSaveAll - Start");
	new temp[MAX_STRING];
	if (db_Exists(DatabaseDB))
	{
		db_Set(DatabaseDB,"PayNSpray_Business_DB",PayNSprayBusinessDB);
	}

	for (new i=0;i<MAX_PAYNSPRAYS;i++)
	{
		new PayNSprayBusinessDBname[MAX_STRING];
		format(PayNSprayBusinessDBname,sizeof(PayNSprayBusinessDBname),"%s%s.txt",PayNSprayBusinessDB,PayNSprays[i][Name]);
		if (db_Exists(PayNSprayBusinessDBname)) {
			continue;
		}
		db_Create(PayNSprayBusinessDBname);
		db_BeforeBigSaving(PayNSprayBusinessDBname);
		db_Set(PayNSprayBusinessDBname,"Name",PayNSprays[i][Name]);
		valstr(temp,PayNSprays[i][Level]);
		db_Set(PayNSprayBusinessDBname,"Level",temp);
		set(temp,nullstr);
		valstr(temp,PayNSprays[i][Cost]);
		db_Set(PayNSprayBusinessDBname,"Cost",temp);
		set(temp,nullstr);
		valstr(temp,PayNSprays[i][ResprayPrice]);
		db_Set(PayNSprayBusinessDBname,"ResprayPrice",temp);
		set(temp,nullstr);
		valstr(temp,PayNSprays[i][MaxPrice]);
		db_Set(PayNSprayBusinessDBname,"MaxPrice",temp);
		set(temp,nullstr);
		valstr(temp,PayNSprays[i][MoneyNow]);
		db_Set(PayNSprayBusinessDBname,"MoneyNow",temp);
		set(temp,nullstr);
		valstr(temp,PayNSprays[i][ItemsInStock]);
		db_Set(PayNSprayBusinessDBname,"ItemsInStock",temp);
		set(temp,nullstr);
		valstr(temp,PayNSprays[i][Components]);
		db_Set(PayNSprayBusinessDBname,"Components",temp);
		set(temp,nullstr);
		db_Set(PayNSprayBusinessDBname,"Owner",PayNSprays[i][Owner]);
		db_AfterBigSaving(PayNSprayBusinessDBname);

	}
	Debug("businesses/paynspray.inc > PayNSpraysSaveAll - Stop");
	return;
}

public PayNSpray_OnGameModeInit()
{
	Debug("businesses/paynspray.inc > PayNSpray_OnGameModeInit - Start");
	new count = 0;
	for (new dbid=0;dbid<MAX_PAYNSPRAYS;dbid++)
	{
		CreateStreamPickup(1272,1,PayNSprays[dbid][Coord_X],PayNSprays[dbid][Coord_Y],PayNSprays[dbid][Coord_Z]);
		count++;
	}
	new logstring[256];
	format(logstring,sizeof(logstring),"Pay'n'Sprays Loaded. Pay'n'Sprays: %d",count);
	WriteLog(logstring);
	Debug("businesses/paynspray.inc > PayNSpray_OnGameModeInit - Stop");
}

public IsPlayerAtPayNSpray(playerid)
{
	for (new dbid=0;dbid<MAX_PAYNSPRAYS;dbid++)
	{
		if (IsPlayerInRangeOfPoint(playerid,4.0,PayNSprays[dbid][Coord_X],PayNSprays[dbid][Coord_Y],PayNSprays[dbid][Coord_Z]))
		{
			return 1;
		}
	}
	return 0;
}

public GetPayNSprayID(playerid)
{
	new lookupid=-1;
	for (new dbid=0;dbid<MAX_PAYNSPRAYS;dbid++)
	{
		if (IsPlayerInRangeOfPoint(playerid,4.0,PayNSprays[dbid][Coord_X],PayNSprays[dbid][Coord_Y],PayNSprays[dbid][Coord_Z])) lookupid = dbid;
	}
	return lookupid;
}

public PayNSprayPickupCheck(playerid)
{
	if (playerid == INVALID_PLAYER_ID) return;
	if (!IsPlayerConnected(playerid)) return;
	if (!IsPlayerAtPayNSpray(playerid)) {paynsprayinfoshowed[playerid] = 0; return;}
	if (paynsprayinfoshowed[playerid] == 1) return;
	paynsprayinfoshowed[playerid] = 1;
	new id=GetPayNSprayID(playerid);
	SystemMsgFormat(playerid,COLOUR_INFO,gettext(1515),PayNSprays[id][ResprayPrice]);
	if (strcomp(PayNSprays[id][Owner],oGetPlayerName(playerid),false) == 1){
		SystemMsg(playerid,COLOUR_INFO,gettext(1516));
		if (PayNSprays[id][MoneyNow] > 0){
			SystemMsgFormat(playerid,COLOUR_INFO,gettext(1517),PayNSprays[id][MoneyNow]);
		}
	}
	if (strcomp(PayNSprays[id][Owner],"Server",false) == 1){
		new string[MAX_STRING];
		format(string,sizeof(string),gettext(1518),PayNSprays[id][Cost]+PayNSprays[id][MoneyNow],PayNSprays[id][Level]);
		SystemMsg(playerid,COLOUR_INFO,string);
	}
}



public ResprayVehicle(playerid){
		Debug("businesses/paynspray.inc > ResprayVehicle - Start");
		new id=GetPayNSprayID(playerid);
		if (!IsPlayerInAnyVehicle(playerid))
		{
			SystemMsg(playerid,COLOUR_ERROR,gettext(815));
			Debug("businesses/paynspray.inc > ResprayVehicle - Stop");
			return 1;
		}
		if (oGetPlayerMoney(playerid) < PayNSprays[id][ResprayPrice])
		{
			SystemMsg(playerid,COLOUR_ERROR,gettext(1519));
			Debug("businesses/paynspray.inc > ResprayVehicle - Stop");
			return 1;
		}
		if (PayNSprays[id][ItemsInStock] == 0){
			ShowTextForPlayer(playerid, gettext(712), 5000, 1);
			Debug("businesses/paynspray.inc > ResprayVehicle - Stop");
			return 1;
		}
            	if(GetPlayerState(playerid) == PLAYER_STATE_PASSENGER){
	            	SystemMsg(playerid,COLOUR_ERROR,gettext(819));
			Debug("businesses/paynspray.inc > ResprayVehicle - Stop");
			return 1;
            	}

		new vid = GetPlayerVehicleID(playerid);
			
            	if(PAYNSPRAY_IsInvalidVehicle(GetVehicleModel(vid))){
  	            	SystemMsg(playerid,COLOUR_ERROR,gettext(1520));
			Debug("businesses/paynspray.inc > ResprayVehicle - Stop");
	            	return 1;
            	}

		new string[MAX_STRING];
		if (strcomp(PayNSprays[id][Owner],"Server",false) == 1){
		oGivePlayerMoney(playerid,-PayNSprays[id][ResprayPrice],0);
		PayNSprays[id][MoneyNow]=PayNSprays[id][MoneyNow]+PayNSprays[id][ResprayPrice];
		PayNSprays[id][ItemsInStock]--;
		new dinifilename[MAX_STRING];
		new temp[256];
		format(dinifilename,sizeof(dinifilename),"%s%s.txt",PayNSprayBusinessDB,PayNSprays[id][Name]);
		if (db_Exists(dinifilename))
		{
			valstr(temp,PayNSprays[id][MoneyNow]);
			db_Set(dinifilename,"MoneyNow",temp);
			set(temp,nullstr);
			valstr(temp,PayNSprays[id][ItemsInStock]);
			db_Set(dinifilename,"ItemsInStock",temp);
			set(temp,nullstr);
		}
		RepairVehicle(vid);
            	ChangeVehicleColor(vid,random(17),random(17));
		SetPlayerCriminal(playerid,"Respray",-20);
		format(string,sizeof(string),gettext(1521),0);
		} else if (strcomp(PayNSprays[id][Owner],oGetPlayerName(playerid),false) == 1){
		RepairVehicle(vid);
            	ChangeVehicleColor(vid,random(17),random(17));
		SetPlayerCriminal(playerid,"Respray",-20);
		PayNSprays[id][ItemsInStock]--;
		new dinifilename[MAX_STRING];
		new temp[256];
		format(dinifilename,sizeof(dinifilename),"%s%s.txt",PayNSprayBusinessDB,PayNSprays[id][Name]);
		if (db_Exists(dinifilename))
		{
			valstr(temp,PayNSprays[id][MoneyNow]);
			db_Set(dinifilename,"MoneyNow",temp);
			set(temp,nullstr);
			valstr(temp,PayNSprays[id][ItemsInStock]);
			db_Set(dinifilename,"ItemsInStock",temp);
			set(temp,nullstr);
		}
		format(string,sizeof(string),gettext(1522),0);
		}
		else
		{
		oGivePlayerMoney(playerid,-PayNSprays[id][ResprayPrice],0);
		RepairVehicle(vid);
            	ChangeVehicleColor(vid,random(17),random(17));
		SetPlayerCriminal(playerid,"Respray",-20);
		PayNSprays[id][ItemsInStock]--;
		PayNSprays[id][MoneyNow]=PayNSprays[id][MoneyNow]+PayNSprays[id][ResprayPrice];
		new dinifilename[MAX_STRING];
		new temp[256];
		format(dinifilename,sizeof(dinifilename),"%s%s.txt",PayNSprayBusinessDB,PayNSprays[id][Name]);
		if (db_Exists(dinifilename))
		{
			valstr(temp,PayNSprays[id][MoneyNow]);
			db_Set(dinifilename,"MoneyNow",temp);
			set(temp,nullstr);
		}
		format(string,sizeof(string),gettext(1521),0);
		}
		SystemMsg(playerid,COLOUR_OK,string);
		new logstring[256];
		format(logstring,sizeof(logstring), "player: %d:  %s: respray vehicle.",playerid,oGetPlayerName(playerid));
		WriteLog(logstring);
		Debug("businesses/paynspray.inc > ResprayVehicle - Stop");
		return 1;
}

public SellPayNSpray(playerid){
Debug("businesses/paynspray.inc > SellPayNSpray - Start");
new id=GetPayNSprayID(playerid);
new temp[MAX_STRING];
	if (id > -1)
	{
		if (IsPlayerRegistered(playerid))
		{
			if(strcomp(PayNSprays[id][Owner], oGetPlayerName(playerid), true) == 0)
			{
				SystemMsg(playerid,COLOUR_ERROR,gettext(1523));
			} else {
				new price;
				price = (PayNSprays[id][Cost] + PayNSprays[id][MoneyNow])*85 / 100;
				oGivePlayerMoney(playerid,0+price,1);
				set(PayNSprays[id][Owner],"Server");
				new dinifilename[MAX_STRING];
				format(dinifilename,sizeof(dinifilename),"%s%s.txt",PayNSprayBusinessDB,PayNSprays[id][Name]);
				if (db_Exists(dinifilename))
				{
					db_Set(dinifilename,"Owner","Server");
				}
				format(temp,sizeof(temp),gettext(1524),PayNSprays[id][Name]);
				SystemMsg(playerid,COLOUR_OK,temp);
				set(temp,nullstr);
				new logstring[256];
				format(logstring, sizeof (logstring), "player: %d:  %s: sold the '%s' (Pay'n'Spray)",playerid,oGetPlayerName(playerid),PayNSprays[id][Name]);
				WriteLog(logstring);
			}
		} else {
			SystemMsg(playerid,COLOUR_ERROR,gettext(561));
		}
	} else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(1525));
	}
	Debug("businesses/paynspray.inc > SellPayNSpray - Stop");
	return 0;
}

public CollectPayNSpray(playerid){
Debug("businesses/paynspray.inc > CollectPayNSPray - Start");
new id=GetPayNSprayID(playerid);
			new temp[MAX_STRING];
			if (id > -1)
			{
				if (IsPlayerRegistered(playerid))
				{
					if(strcomp(PayNSprays[id][Owner], oGetPlayerName(playerid), true) == 0)
					{
						SystemMsg(playerid,COLOUR_ERROR,gettext(1523));
					} else {
						if(PayNSprays[id][MoneyNow] > 0)
						{
							oGivePlayerMoney(playerid,0+PayNSprays[id][MoneyNow],1);
							SystemMsg(playerid,COLOUR_OK,gettext(1526));
							PayNSprays[id][MoneyNow] = 0;
							new dinifilename[MAX_STRING];
							format(dinifilename,sizeof(dinifilename),"%s%s.txt",PayNSprayBusinessDB,PayNSprays[id][Name]);
							if (db_Exists(dinifilename))
							{
								valstr(temp,PayNSprays[id][MoneyNow]);
								db_Set(dinifilename,"MoneyNow",temp);
								set(temp,nullstr);
							}
						} else {
							SystemMsg(playerid,COLOUR_ERROR,gettext(1527));
						}
					}
				} else {
					SystemMsg(playerid,COLOUR_ERROR,gettext(561));
				}
			} else {
				SystemMsg(playerid,COLOUR_ERROR,gettext(1525));
			}
			Debug("businesses/paynspray.inc > CollectPayNSPray - Stop");
			return 0;
}

public BuyPayNSpray(playerid){
Debug("businesses/paynspray.inc > ResprayVehicleShop - Start");
			new id=GetPayNSprayID(playerid);
			new temp[MAX_STRING];
			if (id > -1)
			{
				if (IsPlayerRegistered(playerid))
				{
					if (Player[playerid][Level] >= PayNSprays[id][Level])
					{
						new price;
						price = PayNSprays[id][Cost] + PayNSprays[id][MoneyNow];
						if(Player[playerid][Money] >= price)
						{
							if(strcomp(PayNSprays[id][Owner], oGetPlayerName(playerid), true) == 1)
							{
								SystemMsg(playerid,COLOUR_ERROR,gettext(1528));
							} else {

								oGivePlayerMoney(playerid,0-price,1);
								set(PayNSprays[id][Owner],oGetPlayerName(playerid));
								new dinifilename[MAX_STRING];
								format(dinifilename,sizeof(dinifilename),"%s%s.txt",PayNSprayBusinessDB,PayNSprays[id][Name]);
								if (db_Exists(dinifilename))
								{
									db_Set(dinifilename,"Owner",PayNSprays[id][Owner]);
								}
								format(temp,sizeof(temp),gettext(1529),PayNSprays[id][Name]);
								SystemMsg(playerid,COLOUR_OK,temp);
								SystemMsg(playerid,COLOUR_INFO,gettext(1530));
								SystemMsg(playerid,COLOUR_INFO,gettext(1531));
								SystemMsg(playerid,COLOUR_INFO,gettext(1532));
								set(temp,nullstr);

								new logstring[256];
								format(logstring, sizeof (logstring), "player: %d:  %s: bought the '%s' (Pay'n'Spray)",playerid,oGetPlayerName(playerid),PayNSprays[id][Name]);
								WriteLog(logstring);

							}
						} else {
							SystemMsg(playerid,COLOUR_ERROR,gettext(1533));
						}
					} else {
						format(temp,sizeof(temp),gettext(1534),PayNSprays[id][Level]);
						SystemMsg(playerid,COLOUR_ERROR,temp);
						set(temp,nullstr);
					}
				} else {
					SystemMsg(playerid,COLOUR_ERROR,gettext(561));
				}
			} else {
				SystemMsg(playerid,COLOUR_ERROR,gettext(1525));
			}
			Debug("businesses/paynspray.inc > ResprayVehicleShop - Stop");
			return 0;
}

public SetResprayPrice(playerid,value){
Debug("businesses/paynspray.inc > SetResprayPrice - Start");
new id=GetPayNSprayID(playerid);
if (value <= 0){
	SystemMsg(playerid,COLOUR_ERROR,gettext(1535));
	Debug("businesses/paynspray.inc > SetResprayPrice - Stop");
	return 1;
}
if (id > -1)
{
	if (IsPlayerRegistered(playerid))
	{
		if(strcomp(PayNSprays[id][Owner], oGetPlayerName(playerid), true) == 0)
		{
			SystemMsg(playerid,COLOUR_ERROR,gettext(1523));
		} else {
			if (value > PayNSprays[id][MaxPrice]){
				SystemMsgFormat(playerid,COLOUR_ERROR,gettext(1536),PayNSprays[id][MaxPrice]);
				return 1;
			}
			new dinifilename[MAX_STRING];
			format(dinifilename,sizeof(dinifilename),"%s%s.txt",PayNSprayBusinessDB,PayNSprays[id][Name]);
			if (db_Exists(dinifilename))
			{
				PayNSprays[id][ResprayPrice] = value;
				new temp[MAX_STRING];
				valstr(temp,PayNSprays[id][ResprayPrice]);
				db_Set(dinifilename,"ResprayPrice",temp);
				set(temp,nullstr);
			}
			new string[MAX_STRING];
			format(string,sizeof(string),gettext(1537),PayNSprays[id][Name],PayNSprays[id][ResprayPrice]);
			SystemMsg(playerid,COLOUR_OK,string);
			new logstring[256];
			format(logstring, sizeof (logstring), "player: %d:  %s: set price of '%s' (Pay'n'Spray) to $%d",playerid,oGetPlayerName(playerid),PayNSprays[id][Name],PayNSprays[id][ResprayPrice]);
			WriteLog(logstring);
		}
	} else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(561));
	}
} else {
	SystemMsg(playerid,COLOUR_ERROR,gettext(1525));
}
Debug("businesses/paynspray.inc > SetResprayPrice - Stop");
return 0;
}

stock PAYNSPRAY_IsInvalidVehicle(model){

	#define	PAYNSPRAY_MAX_BAD_MODELS   6

	new PAYNSPRAY_badModles[PAYNSPRAY_MAX_BAD_MODELS] = {
		V_POLMAV,V_COPBIKE,V_COPCARLA,V_COPCARSF,V_COPCARVG,V_COPCARRU
	};

	for (new i = 0; i < PAYNSPRAY_MAX_BAD_MODELS; i++) {
	    if (model == PAYNSPRAY_badModles[i]) return true;
	}

	return false;
}