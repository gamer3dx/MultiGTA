//Clothes Business module.
//Created by MadCat
//Based on Original Business Handler from GTO, created by Iain Gilbert
//Coords from GTAT. DracoBlue is the best! ;)

forward Clothes_OnGameModeInit();
forward CShopPickupCheck(playerid);
forward GetPlayerCShopID(playerid);
forward IsPlayerAtCShop(playerid);
forward BuyClothes(playerid);
forward BuyCShop(playerid);
forward CollectCShop(playerid);
forward SellCShop(playerid);
forward SetClothesPrice(playerid,value);

new cshopinfoshowed[MAX_PLAYERS] = 0;

new ClothesBusinessDB[MAX_STRING] = "MultiGTA/Businesses/";

enum CShopInfo {
    Name[MAX_STRING],
    Float:Coord_X,
    Float:Coord_Y,
    Float:Coord_Z,
    Level,
    Cost,
    ClothesPrice,
    MaxPrice,
    CrimesDecrease,
    MoneyNow,
    ItemsInStock,
    Components,
    Owner[MAX_NAME]
}

new PlayerClothesTick[MAX_PLAYERS];

#define CLOTHES_CHANGE_TIME 90000 //90 seconds

#define MAX_CLOTHESSHOPS 17

new Float:CShops[MAX_CLOTHESSHOPS][CShopInfo] = {
	{"Clothes - Binco - (LV)Airport",1654.8408,1732.0372,10.82035,5,50000,100,300,7,0,100,0,"Server"},
	{"Clothes - Binco - (LV)The Emerald Isle",2103.2068,2259.4919,11.02345,5,50000,100,300,7,0,100,0,"Server"},
	{"Clothes - Binco - (LS)Ganton",2241.2378,-1662.6855,15.47665,5,50000,100,300,7,0,100,0,"Server"},
	{"Clothes - Binco - (SF)Jupiter Hill",-2376.4063,906.5512,45.44535,5,50000,100,300,7,0,100,0,"Server"},
	{"Clothes - ZIP - (LV)The Emerald Isle",2092.8723,2224.1904,11.0234,5,50000,200,600,20,0,100,0,"Server"},
	{"Clothes - ZIP - (LV)Starfish Casino",2574.8044,1902.5027,11.0234,5,50000,200,600,20,0,100,0,"Server"},
	{"Clothes - ZIP - (SF)Downtown",-1881.5189,862.4061,35.1719,5,50000,200,600,20,0,100,0,"Server"},
	{"Clothes - ZIP - (LS)Downtown",1460.6112,-1137.7312,24.0672,5,50000,200,600,20,0,100,0,"Server"},
	{"Clothes - Suburban - (LV)Creek",2780.1965,2450.0942,11.0625,5,50000,400,1200,30,0,100,0,"Server"},
	{"Clothes - Suburban - (SF)Hashburry",-2493.5940,-31.4737,25.7656,5,50000,400,1200,30,0,100,0,"Server"},
	{"Clothes - Suburban - (LS)Jefferson",2115.6672,-1212.7252,23.9647,5,50000,400,1200,30,0,100,0,"Server"},
	{"Clothes - Victim - (LV)Creek",2802.9480,2427.2095,11.0625,5,50000,500,1500,40,0,100,0,"Server"},
	{"Clothes - Victim - (SF)Downtown",-1694.7983,945.1563,24.8906,5,50000,500,1500,40,0,100,0,"Server"},
	{"Clothes - Victim - (LS)Rodeo",459.8151,-1506.0872,31.0038,5,50000,500,1500,40,0,100,0,"Server"},
	{"Clothes - ProLaps - (LV)Creek",2826.9070,2404.1396,11.0625,5,50000,600,1800,50,0,100,0,"Server"},
	{"Clothes - ProLaps - (LS)Rodeo",498.4181,-1358.2133,16.4319,5,50000,800,2400,50,0,100,0,"Server"},
	{"Clothes - Didier Sach - (LS)Rodeo",453.1914,-1481.5623,30.9058,5,100000,1000,3000,60,0,100,0,"Server"}
};

ClothesBusinessLoadAll()
{
	Debug("businesses/clothes.inc > ClothesBusinessLoadAll - Start");
	
	new temp[MAX_STRING];
	if (!db_Exists(DatabaseDB)) db_Create(DatabaseDB);
	if (!db_Isset(DatabaseDB,"Clothes_Business_DB")) db_Set(DatabaseDB,"Clothes_Business_DB",ClothesBusinessDB);
	set(temp,db_Get(DatabaseDB,"Clothes_Business_DB"));
	if (strlen(temp) > 0) set(ClothesBusinessDB,temp);
	if (!db_Exists(ClothesBusinessDB))
	{

		ClothesBusinessSaveAll();

		return;
	}
	for (new i=0;i<MAX_CLOTHESSHOPS;i++)
	{
		new ClothesBusinessDBname[MAX_STRING];
		format(ClothesBusinessDBname,sizeof(ClothesBusinessDBname),"%s%s.txt",ClothesBusinessDB,CShops[i][Name]);
		if (!db_Exists(ClothesBusinessDBname))
		{
			db_Create(ClothesBusinessDBname);
			ClothesBusinessSaveAll();
		}
		set(temp,db_Get(ClothesBusinessDBname,"Name"));
		if (strlen(temp) > 0) set(CShops[i][Name],temp);
		set(temp,nullstr);
		set(temp,db_Get(ClothesBusinessDBname,"Level"));
		if (strlen(temp) > 0) CShops[i][Level] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(ClothesBusinessDBname,"Cost"));
		if (strlen(temp) > 0) CShops[i][Cost] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(ClothesBusinessDBname,"ClothesPrice"));
		if (strlen(temp) > 0) CShops[i][ClothesPrice] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(ClothesBusinessDBname,"MaxPrice"));
		if (strlen(temp) > 0) CShops[i][MaxPrice] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(ClothesBusinessDBname,"MoneyNow"));
		if (strlen(temp) > 0) CShops[i][MoneyNow] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(ClothesBusinessDBname,"ItemsInStock"));
		if (strlen(temp) > 0) CShops[i][ItemsInStock] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(ClothesBusinessDBname,"Components"));
		if (strlen(temp) > 0) CShops[i][Components] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(ClothesBusinessDBname,"Owner"));
		if (strlen(temp) > 0) set(CShops[i][Owner],temp);
		set(temp,nullstr);
	}
	Debug("businesses/clothes.inc > ClothesBusinessLoadAll - Stop");
	return;

}

ClothesBusinessSaveAll()
{
	Debug("businesses/clothes.inc > ClothesBusinessSaveAll - Start");
	new temp[MAX_STRING];
	if (db_Exists(DatabaseDB))
	{
		db_Set(DatabaseDB,"Clothes_Business_DB",ClothesBusinessDB);
	}

	for (new i=0;i<MAX_CLOTHESSHOPS;i++)
	{
		new ClothesBusinessDBname[MAX_STRING];
		format(ClothesBusinessDBname,sizeof(ClothesBusinessDBname),"%s%s.txt",ClothesBusinessDB,CShops[i][Name]);
		if (db_Exists(ClothesBusinessDBname)) {
			continue;
		}
		db_Create(ClothesBusinessDBname);
		db_BeforeBigSaving(ClothesBusinessDBname);
		db_Set(ClothesBusinessDBname,"Name",CShops[i][Name]);
		valstr(temp,CShops[i][Level]);
		db_Set(ClothesBusinessDBname,"Level",temp);
		set(temp,nullstr);
		valstr(temp,CShops[i][Cost]);
		db_Set(ClothesBusinessDBname,"Cost",temp);
		set(temp,nullstr);
		valstr(temp,CShops[i][ClothesPrice]);
		db_Set(ClothesBusinessDBname,"ClothesPrice",temp);
		set(temp,nullstr);
		valstr(temp,CShops[i][MaxPrice]);
		db_Set(ClothesBusinessDBname,"MaxPrice",temp);
		set(temp,nullstr);
		valstr(temp,CShops[i][MoneyNow]);
		db_Set(ClothesBusinessDBname,"MoneyNow",temp);
		set(temp,nullstr);
		valstr(temp,CShops[i][ItemsInStock]);
		db_Set(ClothesBusinessDBname,"ItemsInStock",temp);
		set(temp,nullstr);
		valstr(temp,CShops[i][Components]);
		db_Set(ClothesBusinessDBname,"Components",temp);
		set(temp,nullstr);
		db_Set(ClothesBusinessDBname,"Owner",CShops[i][Owner]);
		db_AfterBigSaving(ClothesBusinessDBname);

	}
	Debug("businesses/clothes.inc > ClothesBusinessSaveAll - Stop");
	return;
}

public Clothes_OnGameModeInit()
{
	Debug("businesses/clothes.inc > Clothes_OnGameModeInit - Start");
	new count = 0;
	for (new id=0;id<MAX_CLOTHESSHOPS;id++)
	{
		CreateStreamPickup(1275,1,CShops[id][Coord_X],CShops[id][Coord_Y],CShops[id][Coord_Z]);
		CreateStreamMapIcon(45,CShops[id][Coord_X],CShops[id][Coord_Y],CShops[id][Coord_Z]);
		CreateDMapIcon(45,CShops[id][Coord_X],CShops[id][Coord_Y],CShops[id][Coord_Z],gettext(1747),gettext(1748));
		count++;
	}
	new logstring[256];
	format(logstring,sizeof(logstring),"Clothes System Loaded. Clothes shops: %d",count);
	WriteLog(logstring);
	Debug("businesses/clothes.inc > Clothes_OnGameModeInit - Stop");
}

public IsPlayerAtCShop(playerid)
{
	for (new id=0;id<MAX_CLOTHESSHOPS;id++)
	{
		if (IsPlayerInRangeOfPoint(playerid,1.0,CShops[id][Coord_X],CShops[id][Coord_Y],CShops[id][Coord_Z]))
		{
			return 1;
		}
	}
	return 0;
}

public GetPlayerCShopID(playerid)
{
	new lookupid=-1;
	for (new id=0;id<MAX_CLOTHESSHOPS;id++)
	{
		if (IsPlayerInRangeOfPoint(playerid,1.0,CShops[id][Coord_X],CShops[id][Coord_Y],CShops[id][Coord_Z])) lookupid = id;
	}
	return lookupid;
}

public CShopPickupCheck(playerid)
{
	if (playerid == INVALID_PLAYER_ID) return;
	if (!IsPlayerConnected(playerid)) return;
	if (!IsPlayerAtCShop(playerid)) {cshopinfoshowed[playerid] = 0; return;}
	if (cshopinfoshowed[playerid] == 1) return;
	cshopinfoshowed[playerid] = 1;
	new id=GetPlayerCShopID(playerid);
	SystemMsg(playerid,COLOUR_INFO,gettext(713));
	SystemMsgFormat(playerid,COLOUR_INFO,gettext(714),CShops[id][ClothesPrice]);
	if (strcomp(CShops[id][Owner],oGetPlayerName(playerid),false) == 1){
	SystemMsg(playerid,COLOUR_INFO,gettext(715));
	if (CShops[id][MoneyNow] > 0){
	SystemMsgFormat(playerid, COLOUR_INFO,gettext(716),CShops[id][MoneyNow]);
	}
	}
	new tempstr[256];
	if (strcomp(CShops[id][Owner],"Server",false) == 1){
	format(tempstr,sizeof(tempstr),gettext(717),CShops[id][Cost]+CShops[id][MoneyNow],CShops[id][Level]);
	SystemMsg(playerid,COLOUR_INFO,tempstr);
	}

}

public BuyClothes(playerid){
	new id=GetPlayerCShopID(playerid);
	if (oGetPlayerMoney(playerid) < CShops[id][ClothesPrice])
	{
	SystemMsg(playerid,COLOUR_ERROR,gettext(718));
	Debug("businesses/clothes.inc > BuyClothes - Stop");
	return 1;
	}
	if (CShops[id][ItemsInStock] == 0){
		ShowTextForPlayer(playerid, gettext(712), 5000, 1);
		Debug("businesses/clothes.inc > BuyClothes - Stop");
		return 1;
	}
	if ((Player[playerid][WantedLevel] == 0) && (Player[playerid][Crimes] == 0))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(719));
		Debug("businesses/clothes.inc > BuyClothes - Stop");
		return 1;
	}
	if(GetTickCount() > PlayerClothesTick[playerid] + CLOTHES_CHANGE_TIME) {
		    SystemMsgFormat(playerid,COLOUR_ERROR,gettext(720),(CLOTHES_CHANGE_TIME/1000));
		    Debug("businesses/clothes.inc > BuyClothes - Stop");
		    return 1;
		}
	new string[MAX_STRING];
	new temp[MAX_STRING];
	if (strcomp(CShops[id][Owner],"Server",false) == 1){
		oGivePlayerMoney(playerid,-CShops[id][ClothesPrice],0);
		CShops[id][MoneyNow]=CShops[id][MoneyNow]+CShops[id][ClothesPrice];
		CShops[id][ItemsInStock]--;
		new dinifilename[MAX_STRING];
		format(dinifilename,sizeof(dinifilename),"%s%s.txt",ClothesBusinessDB,CShops[id][Name]);
		if (db_Exists(dinifilename))
		{
			valstr(temp,CShops[id][MoneyNow]);
			db_Set(dinifilename,"MoneyNow",temp);
			set(temp,nullstr);
			valstr(temp,CShops[id][ItemsInStock]);
			db_Set(dinifilename,"ItemsInStock",temp);
			set(temp,nullstr);
		}
		Player[playerid][Crimes]=Player[playerid][Crimes]-CShops[id][CrimesDecrease];
		CheckCurrentCrimes(playerid);
		format(string,sizeof(string),gettext(721),0);
	} else if (strcomp(CShops[id][Owner],oGetPlayerName(playerid),false) == 1){
		Player[playerid][Crimes]=Player[playerid][Crimes]-CShops[id][CrimesDecrease];
		CheckCurrentCrimes(playerid);
		CShops[id][ItemsInStock]--;
		new dinifilename[MAX_STRING];
		format(dinifilename,sizeof(dinifilename),"%s%s.txt",ClothesBusinessDB,CShops[id][Name]);
		if (db_Exists(dinifilename))
		{
			valstr(temp,CShops[id][ItemsInStock]);
			db_Set(dinifilename,"ItemsInStock",temp);
			set(temp,nullstr);
		}
		format(string,sizeof(string), gettext(722),0);
	}
	else
	{
		oGivePlayerMoney(playerid,-CShops[id][ClothesPrice],0);
		Player[playerid][Crimes]=Player[playerid][Crimes]-CShops[id][CrimesDecrease];
		CheckCurrentCrimes(playerid);
		CShops[id][MoneyNow]=CShops[id][MoneyNow]+CShops[id][ClothesPrice];
		CShops[id][ItemsInStock]--;
		new dinifilename[MAX_STRING];
		format(dinifilename,sizeof(dinifilename),"%s%s.txt",ClothesBusinessDB,CShops[id][Name]);
		if (db_Exists(dinifilename))
		{
			valstr(temp,CShops[id][MoneyNow]);
			db_Set(dinifilename,"MoneyNow",temp);
			set(temp,nullstr);
			valstr(temp,CShops[id][ItemsInStock]);
			db_Set(dinifilename,"ItemsInStock",temp);
			set(temp,nullstr);
		}
		format(string,sizeof(string), gettext(721),0);
	}
	SystemMsg(playerid,COLOUR_OK,string);
	if (Player[playerid][WantedLevel] == 0){
		new copstring[256];
		format(copstring,sizeof(copstring),gettext(723),oGetPlayerName(playerid));
		SendMsgToCops(copstring);
	}
	Player[playerid][ClothesBuyed]++;
	PlayerClothesTick[playerid] = GetTickCount();
	new logstring[256];
	format(logstring,sizeof(logstring), "player: %d:  %s: buy new clothes. ",playerid,oGetPlayerName(playerid));
	WriteLog(logstring);
	Debug("businesses/clothes.inc > BuyClothes - Stop");
	return 0;
}

public BuyCShop(playerid){
		Debug("businesses/clothes.inc > BuyCShop - Start");
		new id=GetPlayerCShopID(playerid);
		new temp[MAX_STRING];
		if (id > -1)
		{
			if (IsPlayerRegistered(playerid))
			{
				if (Player[playerid][Level] >= CShops[id][Level])
				{
					new price;
					price = CShops[id][Cost] + CShops[id][MoneyNow];
					if(Player[playerid][Money] >= price)
					{
						if(strcomp(CShops[id][Owner], oGetPlayerName(playerid), true) == 1)
						{
							SystemMsg(playerid,COLOUR_ERROR,gettext(724));
						} else {
							oGivePlayerMoney(playerid,0-price,1);
							set(CShops[id][Owner],oGetPlayerName(playerid));
							new dinifilename[MAX_STRING];
							format(dinifilename,sizeof(dinifilename),"%s%s.txt",ClothesBusinessDB,CShops[id][Name]);
							if (db_Exists(dinifilename))
							{
								db_Set(dinifilename,"Owner",CShops[id][Owner]);
							}
							format(temp,sizeof(temp),gettext(726),CShops[id][Name]);
							SystemMsg(playerid,COLOUR_OK,temp);
							SystemMsg(playerid,COLOUR_INFO,gettext(727));
							SystemMsg(playerid,COLOUR_INFO,gettext(728));
							SystemMsg(playerid,COLOUR_INFO,gettext(729));
							set(temp,nullstr);
							new logstring[256];
							format(logstring, sizeof (logstring), "player: %d:  %s: bought the '%s' (Clothes Shop)",playerid,oGetPlayerName(playerid),CShops[id][Name]);
							WriteLog(logstring);
						}
					} else {
						SystemMsg(playerid,COLOUR_ERROR,gettext(730));
					}
				} else {
					format(temp,sizeof(temp),gettext(731),CShops[id][Level]);
					SystemMsg(playerid,COLOUR_ERROR,temp);
					set(temp,nullstr);
				}
			} else {
				SystemMsg(playerid,COLOUR_ERROR,gettext(561));
			}
		} else {
			SystemMsg(playerid,COLOUR_ERROR,gettext(732));
		}
		Debug("businesses/clothes.inc > BuyCShop - Stop");
return 0;
}

public CollectCShop(playerid){
Debug("businesses/clothes.inc > CollectCShop - Start");
new id=GetPlayerCShopID(playerid);
new temp[MAX_STRING];
if (id > -1)
{
	if (IsPlayerRegistered(playerid))
	{
		if(strcomp(CShops[id][Owner], oGetPlayerName(playerid), true) == 0)
		{
			SystemMsg(playerid,COLOUR_ERROR,gettext(725));
		} else {
			if(CShops[id][MoneyNow] > 0)
			{
				oGivePlayerMoney(playerid,0+CShops[id][MoneyNow],1);
				SystemMsg(playerid,COLOUR_OK,gettext(734));
				CShops[id][MoneyNow] = 0;
				new dinifilename[MAX_STRING];
				format(dinifilename,sizeof(dinifilename),"%s%s.txt",ClothesBusinessDB,CShops[id][Name]);
				if (db_Exists(dinifilename))
				{
					valstr(temp,CShops[id][MoneyNow]);
					db_Set(dinifilename,"MoneyNow",temp);
					set(temp,nullstr);
				}
			} else {
				SystemMsg(playerid,COLOUR_ERROR,gettext(733));
			}
		}
	} else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(561));
	}
} else {
	SystemMsg(playerid,COLOUR_ERROR,gettext(732));
}
Debug("businesses/clothes.inc > CollectCShop - Stop");
return 0;
}

public SellCShop(playerid){
Debug("businesses/clothes.inc > SellCShop - Start");
new id=GetPlayerCShopID(playerid);
new temp[MAX_STRING];
if (id > -1)
{
	if (IsPlayerRegistered(playerid))
	{
		if(strcomp(CShops[id][Owner], oGetPlayerName(playerid), true) == 0)
		{
			SystemMsg(playerid,COLOUR_ERROR,gettext(725));
		} else {
			new price;
			price = CShops[id][Cost] + CShops[id][MoneyNow];
			price = (price * 85) / 100;
			oGivePlayerMoney(playerid,0+price,1);
			set(CShops[id][Owner],"Server");
			new dinifilename[MAX_STRING];
			format(dinifilename,sizeof(dinifilename),"%s%s.txt",ClothesBusinessDB,CShops[id][Name]);
			if (db_Exists(dinifilename))
			{
				db_Set(dinifilename,"Owner","Server");
				valstr(temp,CShops[id][MoneyNow]);
				db_Set(dinifilename,"MoneyNow",temp);
				set(temp,nullstr);
			}
			format(temp,sizeof(temp),gettext(735),CShops[id][Name]);
			SystemMsg(playerid,COLOUR_OK,temp);
			set(temp,nullstr);
			new logstring[256];
			format(logstring, sizeof (logstring), "player: %d:  %s: sold the '%s' (Clothes shop)",playerid,oGetPlayerName(playerid),CShops[id][Name]);
			WriteLog(logstring);
		}
	} else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(561));
	}
} else {
	SystemMsg(playerid,COLOUR_ERROR,gettext(732));
}
Debug("businesses/clothes.inc > SellCShop - Stop");
return 0;
}

public SetClothesPrice(playerid,value){
new id=GetPlayerCShopID(playerid);
if (value <= 0){
	SystemMsg(playerid,COLOUR_ERROR,gettext(736));
	Debug("businesses/clothes.inc > SetClothesPrice - Stop");
	return 1;
}
if (id > -1)
{
	if (IsPlayerRegistered(playerid))
	{
		if(strcomp(CShops[id][Owner], oGetPlayerName(playerid), true) == 0)
		{
SystemMsg(playerid,COLOUR_ERROR,gettext(725));
		} else {
if (value > CShops[id][MaxPrice]){
	SystemMsgFormat(playerid,COLOUR_ERROR,gettext(737),CShops[id][MaxPrice]);
	return 1;
}
new dinifilename[MAX_STRING];
format(dinifilename,sizeof(dinifilename),"%s%s.txt",ClothesBusinessDB,CShops[id][Name]);
if (db_Exists(dinifilename))
{
	CShops[id][ClothesPrice] = value;
	new temp[MAX_STRING];
	valstr(temp,CShops[id][ClothesPrice]);
	db_Set(dinifilename,"ClothesPrice",temp);
	set(temp,nullstr);
}
new string[MAX_STRING];
format(string,sizeof(string),gettext(738),CShops[id][Name],CShops[id][ClothesPrice]);
SystemMsg(playerid,COLOUR_OK,string);
new logstring[256];
format(logstring, sizeof (logstring), "player: %d:  %s: set price of '%s' (Clothes shop) to $%d",playerid,oGetPlayerName(playerid),CShops[id][Name],CShops[id][ClothesPrice]);
WriteLog(logstring);
		}
	} else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(561));
	}
} else {
	SystemMsg(playerid,COLOUR_ERROR,gettext(732));
}
Debug("businesses/clothes.inc > SetClothesPrice - Stop");
return 0;
}
