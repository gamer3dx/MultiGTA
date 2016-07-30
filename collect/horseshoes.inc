//Horseshoes
//Created by MadCat, based on Packets

#define HORSESHOES_TOTALHORSESHOES 49

new PlayerHorseshoes[MAX_PLAYERS][HORSESHOES_TOTALHORSESHOES];

new Horseshoes_Enabled = 0;
new Reward_For_Horseshoe = 200;

new TimerHorseshoes;

forward Horseshoes_OnGameModeInit();
forward HorseshoesTimer();

new HorseshoesLocations[HORSESHOES_TOTALHORSESHOES][CoordInfo] = {
	{1224.0,2617.0,11.0},
	{2323.0,1284.0,97.0},
	{2035.0,2305.0,18.0},
	{2491.0,2263.0,15.0},
	{1433.0,2796.0,20.0},
	{2071.0,712.0,11.0},
	{2239.0,1839.0,18.0},
	{2583.0,2387.0,16.0},
	{2864.0,857.0,13.0},
	{2612.0,2200.0,-1.0},
	{2274.0,1507.0,24.0},
	{2184.0,2529.0,11.0},
	{1863.0,2314.0,15.0},
	{2054.0,2434.0,166.0},
	{1603.0,1435.0,11.0},
	{1362.92,1015.24,11.0},
	{2058.7,2159.1001,16.0},
	{2003.0,1672.0,12.0},
	{2238.0,1135.0,49.0},
	{1934.0601,988.79,22.0},
	{1768.0,2847.0,9.0},
	{1084.0,1076.0,11.0},
	{2879.0,2522.0,11.0},
	{2371.0,2009.0,15.0},
	{1521.0,1690.0,10.6},
	{2417.0,1281.0,21.0},
	{1376.0,2304.0,15.0},
	{1393.0,1832.0,12.34},
	{984.0,2563.0,12.0},
	{1767.0,601.0,13.0},
	{2108.0,1003.0,46.0},
	{2705.98,1862.52,24.41},
	{2493.0,922.0,16.0},
	{1881.0,2846.0,11.0},
	{2020.0,2352.0,11.0},
	{1680.3,2226.8601,16.11},
	{1462.0,936.0,10.0},
	{2125.5,789.23,11.45},
	{2588.0,1902.0,15.0},
	{919.0,2070.0,11.0},
	{2173.0,2465.0,11.0},
	{2031.25,2207.3301,11.0},
	{2509.0,1144.0,19.0},
	{2215.0,1968.0,11.0},
	{2626.0,2841.0,11.0},
	{2440.0801,2161.0701,20.0},
	{1582.0,2401.0,19.0},
	{2077.0,1912.0,14.0},
	{970.0,1787.0,11.0}
};

public Horseshoes_OnGameModeInit(){
	HorseshoesConfig();
	if (Horseshoes_Enabled == 1){
		for (new id=0;id<HORSESHOES_TOTALHORSESHOES;id++){
			CreateStreamPickup(954,1,HorseshoesLocations[id][Coord_X],HorseshoesLocations[id][Coord_Y],HorseshoesLocations[id][Coord_Z]);
		}
		TimerHorseshoes = SetTimer("HorseshoesTimer",1000,1);
		WriteLog("Horseshoes system loaded");
	}
	return 0;
}

HorseshoesConfig()
{
	if (!db_Exists(ConfigDB)) db_Create(ConfigDB);
	new temp[MAX_STRING];

	set(temp,db_Get(ConfigDB,"Horseshoes_Enabled"));
	if (strlen(temp) > 0) Horseshoes_Enabled = strval(temp); 
	else { valstr(temp,Horseshoes_Enabled); db_Set(ConfigDB,"Horseshoes_Enabled",temp); }
	set(temp,nullstr);

	set(temp,db_Get(ConfigDB,"Reward_For_Horseshoe"));
	if (strlen(temp) > 0) Reward_For_Horseshoe = strval(temp); 
	else { valstr(temp,Reward_For_Horseshoe); db_Set(ConfigDB,"Reward_For_Horseshoe",temp); }
}

public HorseshoesTimer(){
	for (new playerid=0; playerid<MAX_PLAYERS_EX; playerid++){
	    if (IsPlayerConnected(playerid)){
		if (PlayerQuest[playerid] == QUEST_NOQUEST){
			for (new horseshoeid=0; horseshoeid<HORSESHOES_TOTALHORSESHOES; horseshoeid++){
				if (PlayerHorseshoes[playerid][horseshoeid] == 0){
					if (IsPlayerInRangeOfPoint(playerid,3.0,HorseshoesLocations[horseshoeid][Coord_X],HorseshoesLocations[horseshoeid][Coord_Y],HorseshoesLocations[horseshoeid][Coord_Z])){
			        		PlayerPicksUpHorseshoe(playerid,horseshoeid);
					}
			    	}
			}
		}
	    }
	}
}

PlayerPicksUpHorseshoe(playerid,horseshoeid){
    	PlayerHorseshoes[playerid][horseshoeid] = 1;
	new pickedpickups = CountPickedHorseshoes(playerid);
	oGivePlayerMoney(playerid,(Reward_For_Horseshoe*pickedpickups),0);
	PlayerPlaySoundOnPlayer(playerid,1058);
	new text[MAX_STRING];
	format(text,MAX_STRING,gettext(1775),pickedpickups,HORSESHOES_TOTALHORSESHOES,(Reward_For_Horseshoe*pickedpickups));
    	ShowTextForPlayer(playerid,text,5000,5);
}

SetHorseshoesFromDBString(playerid,dbstring[])
{
	new idx;
	for (new i=0;i<HORSESHOES_TOTALHORSESHOES;i++)
	{
		new horseshoes;
		horseshoes = strval(strcharsplit(dbstring,idx,strchar("/")));
		PlayerHorseshoes[playerid][i] = horseshoes;
	}
}

CreateHorseshoeDBString(playerid)
{
	new wepstr[MAX_STRING];
	for (new i=0;i<HORSESHOES_TOTALHORSESHOES;i++)
	{
		new temp[MAX_STRING];
		valstr(temp,PlayerHorseshoes[playerid][i]); // level
		strins(wepstr, temp, strlen(wepstr));
		wepstr[strlen(wepstr)] =strchar("/");
	}
	return wepstr;
}

CountPickedHorseshoes(playerid)
{
	new picked = 0;
	for (new horseshoeid=0;horseshoeid<HORSESHOES_TOTALHORSESHOES;horseshoeid++)
	{
		if (PlayerHorseshoes[playerid][horseshoeid] == 1) picked++;
	}
	return picked;
}