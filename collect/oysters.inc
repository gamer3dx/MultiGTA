//Oysters
//Created by MadCat, based on Horseshoes

#define OYSTERS_TOTALOYSTERS 50

new PlayerOysters[MAX_PLAYERS][OYSTERS_TOTALOYSTERS];

new Oysters_Enabled = 0;
new Reward_For_Oyster = 300;

new TimerOysters;

forward Oysters_OnGameModeInit();
forward OystersTimer();

new OystersLocations[OYSTERS_TOTALOYSTERS][CoordInfo] = {
	{979.0,-2210.0,-3.0},
	{2750.0,-2584.0,-5.0},
	{1279.0,-806.0,85.0},
	{2945.1299,-2051.9299,-3.0},
	{67.0,-1018.0,-5.0},
	{2327.0,-2662.0,-5.0},
	{2621.0,-2506.0,-5.0},
	{1249.0,-2687.0,-1.0},
	{725.0,-1849.0,-5.0},
	{723.0,-1586.0,-3.0},
	{155.0,-1975.0,-8.0},
	{1968.0,-1203.0,17.0},
	{-2657.0,1564.0,-6.0},
	{-1252.0,501.0,-8.0},
	{-1625.0,4.0,-10.0},
	{-1484.0,1489.0,-10.0},
	{-2505.4058,1543.7236,-22.5553},
	{-2727.0,-469.0,-5.0},
	{-1266.0,966.0,-10.0},
	{-1013.0,478.0,-7.0},
	{-1364.0,390.0,-5.0},
	{2578.0,2382.0,16.0},
	{2090.0,1898.0,8.0},
	{2130.0,1152.0,7.0},
	{2013.0,1670.0,7.0},
	{2531.0,1569.0,9.0},
	{2998.0,2998.0,-10.0},
	{-832.0,925.0,-2.0},
	{486.0,-253.0,-4.0},
	{-90.0,-910.0,-5.0},
	{26.43,-1320.9399,-10.04},
	{-207.0,-1682.0,-8.0},
	{-1672.0,-1641.0,-2.0},
	{-1175.0,-2639.0,-2.5},
	{-1097.0,-2858.0,-8.0},
	{-2889.0,-1042.0,-9.0},
	{-659.0,874.0,-2.0},
	{-955.0,2628.0,35.0},
	{-1066.0,2197.0,32.0},
	{40.0,-531.0,-8.0},
	{-765.0,247.0,-8.0},
	{2098.0,-108.0,-2.0},
	{2767.0,470.0,-8.0},
	{-783.0,2116.0,35.0},
	{-821.0,1374.0,-8.0},
	{-2110.5,2329.72,-7.5},
	{-1538.0,1708.0,-3.27},
	{-2685.0,2153.0,-5.0},
	{796.0,2939.0,-5.0},
	{2179.0,235.0,-5.0}
};

public Oysters_OnGameModeInit(){
	OystersConfig();
	if (Oysters_Enabled == 1){
		for (new id=0;id<OYSTERS_TOTALOYSTERS;id++){
			CreateStreamPickup(954,1,OystersLocations[id][Coord_X],OystersLocations[id][Coord_Y],OystersLocations[id][Coord_Z]);
		}
		TimerOysters = SetTimer("OystersTimer",1000,1);
		WriteLog("Oysters system loaded");
	}
	return 0;
}

OystersConfig()
{
	if (!db_Exists(ConfigDB)) db_Create(ConfigDB);
	new temp[MAX_STRING];

	set(temp,db_Get(ConfigDB,"Oysters_Enabled"));
	if (strlen(temp) > 0) Oysters_Enabled = strval(temp); 
	else { valstr(temp,Oysters_Enabled); db_Set(ConfigDB,"Oysters_Enabled",temp); }
	set(temp,nullstr);

	set(temp,db_Get(ConfigDB,"Reward_For_Oyster"));
	if (strlen(temp) > 0) Reward_For_Oyster = strval(temp); 
	else { valstr(temp,Reward_For_Oyster); db_Set(ConfigDB,"Reward_For_Oyster",temp); }
}

public OystersTimer(){
	for (new playerid=0; playerid<MAX_PLAYERS_EX; playerid++){
	    if (IsPlayerConnected(playerid)){
		if (PlayerQuest[playerid] == QUEST_NOQUEST){
			for (new oysterid=0; oysterid<OYSTERS_TOTALOYSTERS; oysterid++){
				if (PlayerOysters[playerid][oysterid] == 0){
					if (IsPlayerInRangeOfPoint(playerid,3.0,OystersLocations[oysterid][Coord_X],OystersLocations[oysterid][Coord_Y],OystersLocations[oysterid][Coord_Z])){
			        		PlayerFoundOyster(playerid,oysterid);
					}
			    	}
			}
		}
	    }
	}
}

PlayerFoundOyster(playerid,oysterid){
    	PlayerOysters[playerid][oysterid] = 1;
	new pickedpickups = CountPickedOysters(playerid);
	oGivePlayerMoney(playerid,(Reward_For_Oyster*pickedpickups),0);
	PlayerPlaySoundOnPlayer(playerid,1058);
	new text[MAX_STRING];
	format(text,MAX_STRING,gettext(1774),pickedpickups,OYSTERS_TOTALOYSTERS,(Reward_For_Oyster*pickedpickups));
    	ShowTextForPlayer(playerid,text,5000,5);
}

SetOystersFromDBString(playerid,dbstring[])
{
	new idx;
	for (new i=0;i<OYSTERS_TOTALOYSTERS;i++)
	{
		new oysters;
		oysters = strval(strcharsplit(dbstring,idx,strchar("/")));
		PlayerOysters[playerid][i] = oysters;
	}
}

CreateOysterDBString(playerid)
{
	new wepstr[MAX_STRING];
	for (new i=0;i<OYSTERS_TOTALOYSTERS;i++)
	{
		new temp[MAX_STRING];
		valstr(temp,PlayerOysters[playerid][i]); // level
		strins(wepstr, temp, strlen(wepstr));
		wepstr[strlen(wepstr)] =strchar("/");
	}
	return wepstr;
}

CountPickedOysters(playerid)
{
	new picked = 0;
	for (new oysterid=0;oysterid<OYSTERS_TOTALOYSTERS;oysterid++)
	{
		if (PlayerOysters[playerid][oysterid] == 1) picked++;
	}
	return picked;
}