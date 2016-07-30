//Player module.
//Created by Iain Gilbert
//Modder: MadCat

#include "base"
#include "account"
#include "businesses\weapon"
#include "privilegies"
#include "hotspot"
#include "moneydrop"

forward Player_OnGameModeInit();
forward SyncPlayers();
forward SyncMoney(playerid);
forward CheckPlayerKick(playerid);
forward KickPlayer(playerid,kickerid,reason[]);
forward BanPlayer(playerid,hours,reason[]);
forward GivePlayerOwnedWeapons(playerid);
forward SendPlayerOwnedWeapons(playerid);
forward Player_OnPlayerRequestClass(playerid,classid);
forward player_Spawn(playerid);
forward player_ResetStats(playerid);
forward IsPlayerRegistered(playerid);
forward player_Disconnect(playerid);
forward player_OnPlayerConnect(playerid);
forward PlayerDeath(playerid,killerid,reason);
forward PlayerKill(killerid, victimid, reason);
forward Player_Kill(killerid,victimid,reason);
forward RegisterQuest(name[]);
forward ResetQuest(playerid);
forward oSetPlayerCheckpoint(playerid, Float:x, Float:y, Float:z, Float:size);
forward oDisablePlayerCheckpoint(playerid);
forward oIsPlayerInCheckpoint(playerid,Float:cpx,Float:cpy,Float:cpz,dist);
forward oSetPlayerRaceCheckpoint(playerid, type, Float:x, Float:y, Float:z, Float:nextx, Float:nexty, Float:nextz, Float:size);
forward oDisablePlayerRaceCheckpoint(playerid);
forward oIsPlayerInRaceCheckpoint(playerid,Float:cpx,Float:cpy,Float:cpz,dist);
forward oSetPlayerPos(playerid, Float:x, Float:y, Float:z);
forward Float:GetMaxHealth(playerid);
forward GetXPToLevel(playerid,level);
forward CalculatePlayerLevel(playerid);
forward GetPlayerLevel(playerid);
forward GivePlayerXP(playerid, xpamount,showtext);
forward CheckPlayerLevel(playerid);
forward PlayerLevelDown(playerid,level);
forward PlayerLevelUp(playerid,level);
forward oSetPlayerMoney(playerid,money);
forward oGivePlayerMoney(playerid,money,showtext);
forward oGetPlayerMoney(playerid);
forward Float:oGetPlayerHealth(playerid);
forward Float:oGetPlayerArmour(playerid);
forward oSetPlayerHealth(playerid,Float:health);
forward oGivePlayerWeapon(playerid,weaponid,ammo);
forward PlayerSave(playerid);
forward CreatePlayer(playerid);
forward PlayerLoadStats(playerid);
forward JailPlayer(playerid,time,reason[]);
forward UnJailPlayer(playerid);
forward SetSpecialColorForPlayer(playerid);
forward CheckWarns(playerid);
forward ConnectMSG(playerid);
forward DisconnectMsg(playerid, reason);
forward DeathMSG(playerid, killerid, reason);
forward SetPlayerCriminal(playerid,reason[],points);

new AntiTeleLastPos[MAX_PLAYERS][CoordInfo];
new PlayerOldPos[MAX_PLAYERS][CoordInfo];
new PlayerLastInterior[MAX_PLAYERS];

#define MAX_MONEY 99999999

#define PLAYER_WEAPON_SLOTS 13

new Player_DB[MAX_STRING] = "MultiGTA/Players/";

enum PlayerInfo {
	IsActive,  // is player online
	Float:Health,
	Money,
	BankMoney,
	Level,
	XP,
	Deaths,
	Kills,
	JailTime,
	MuteTime,
	Drinks,
	DrinksUsed,
	ClothesBuyed,
	GangMateKills,
	RacesWon,
	DMsWon,
	HaysWon,
	SpawnKills,
	SpawnDeaths,
	RacesPlayed,
	DMsPlayed,
	HaysPlayed,
	Suicides,
	SkyDives,
	GotBusted,
	WantedLevel,
	Crimes,
	WasAtHouse,
	BoxingSkill,
	BoxWins,
	BoxLoses,
	Warns,
	FuelCans,
	Hotwires,
	FoodEaten,
	GotJob,
	CopSkill,
	CopLevel,
	HotwiresCop,
	TotalPlaytime,
	Bounty,
	Heals,
	MinesPlanted,
	CashBox,
	CashBoxCop,
	CanUseBoats,
	CanUseMotoBikes,
	CanUseFlying,
	TaxiRides,
	TaxiPassengers,
	LottoNumber,
	SpawnPlace,
	FightingStyle
}

new Player[MAX_PLAYERS][PlayerInfo];

new PlayerCity[MAX_PLAYERS];

enum PWeap {
	wid,
	bullets
}

new PlayerWeapons[MAX_PLAYERS][PLAYER_WEAPON_SLOTS][PWeap]; // 0 - weaponid, 1 - ammo
new PlayerTempWeapons[MAX_PLAYERS][PLAYER_WEAPON_SLOTS][PWeap];

new PlayerGangid[MAX_PLAYERS];
new PlayerGangName[MAX_PLAYERS][MAX_NAME];
new PlayerGangTag[MAX_PLAYERS][MAX_NAME];

new PlayerSpawn[MAX_PLAYERS][CoordInfo];
new SpawnTime[MAX_PLAYERS];

//Quests
enum Quest_Stats {
	Quest_Name[MAX_NAME]
}

new PlayerQuest[MAX_PLAYERS]; // holds id of quest player is currently doing
new Quests[MAX_QUESTS][Quest_Stats]; // table of all registered quests
new QuestCount;

//Checkpoints streaming
new Float:PlayerCP[MAX_PLAYERS][CoordInfo];
new bool:PlayerCPActive[MAX_PLAYERS];

new Float:PlayerRaceCP[MAX_PLAYERS][CoordInfo];
new bool:PlayerRaceCPActive[MAX_PLAYERS];


//Protections
new PlayerJustTeleported[MAX_PLAYERS];
new ScriptMoneyUpdated[MAX_PLAYERS];

//Settings
new Player_Start_Money = 500;
new Player_Start_Bank_Money = 100;
new Player_Start_Weapon1 = WEAPON_COLT45;
new Player_Start_Weapon2 = 0;
new Player_Start_Weapon3 = 0;
new Warns_For_Ban = 5;

// LS Spawns
new Float:LSPlayerSpawns[][CoordInfo] = {
	{2805.0862,-1180.9579,25.4536},
	{2530.4785,-1063.5999,69.5695},
	{2042.9279,-1013.6509,39.7422},
	{1497.0605,-689.1375,95.1092},
	{1255.1125,-803.2986,84.1406},
	{829.4501,-858.8712,70.1248},
	{699.3003,-1059.2599,49.4217},
	{299.0901,-1155.5858,80.9099},
	{315.2544,-1772.5730,4.6776},
	{333.3540,-1519.2587,35.8672},
	{763.0789,-1564.3256,13.5536},
	{986.0850,-1095.0150,27.6041},
	{1109.6948,-972.4279,42.7656},
	{1438.9014,-927.2687,39.6406},
	{1656.6252,-1076.3080,23.8984},
	{1956.1605,-1116.0430,27.8305},
	{2224.1555,-1240.3496,25.1478},
	{2338.8757,-1289.3113,27.9766},
	{2436.4819,-1303.6724,24.6679},
	{2450.9358,-1493.6650,24.0000},
	{2519.8420,-1678.7356,14.8524},
	{2507.6228,-2019.1085,13.5469},
	{1957.7228,-2183.6519,13.5469},
	{1857.9117,-2042.3588,13.5469},
	{1734.1467,-2099.2144,13.8666},
	{2511.4326,-2589.3069,13.6443},
	{2785.9890,-2417.7148,13.6341},
	{2261.2937,-1930.7488,12.8935},
	{2105.2878,-1913.1111,13.3828},
	{1833.2399,-1681.1813,13.4816},
	{1665.9006,-1559.9375,13.5469},
	{1125.5248,-1372.0935,13.9844},
	{1289.0425,-1134.7928,23.8281},
	{2842.1575,-1335.5879,14.7421},
	{2767.4480,-1610.6735,10.9219},
	{2753.1375,-1308.9150,53.0938},
	{2789.2317,-2494.3286,13.6485},
	{2509.4431,-1679.9254,13.5469},
	{1544.2460,-1359.1956,329.4643},
	{1013.5293,-1030.5001,32.0392},
	{154.3016,-1942.1782,3.7734},
	{1683.1699,-2311.8606,13.5469},
	{1879.8899,-1389.7712,13.5703},
	{1265.0380,-803.7556,88.3125},
	{2783.9766,-1858.4088,9.8165},
	{1751.1097,-2106.4529,13.5469}, // El-Corona - Outside random house
	{2652.6418,-1989.9175,13.9988}, // Random house in willowfield - near playa de seville and stadium
	{2489.5225,-1957.9258,13.5881}, // Hotel in willowfield - near cluckin bell
	{2689.5203,-1695.9354,10.0517}, // Outside stadium - lots of cars
	{2770.5393,-1628.3069,12.1775}, // South in east beach - north of stadium - carparks nearby
	{2807.9282,-1176.8883,25.3805}, // North in east beach - near apartments
	{2552.5417,-958.0850,82.6345}, // Random house north of Las Colinas
	{2232.1309,-1159.5679,25.8906}, // Jefferson motel
	{2388.1003,-1279.8933,25.1291}, // House south of pig pen
	{2481.1885,-1536.7186,24.1467}, // East LS - near clucking bell and car wash
	{2495.0720,-1687.5278,13.5150}, // Outside CJ's house - lots of cars nearby
	{2306.8252,-1675.4340,13.9221}, // House in ganton - lots of cars nearby
	{2191.8403,-1455.8251,25.5391}, // House in south jefferson - lots of cars nearby
	{1830.1359,-1092.1849,23.8656}, // Mulholland intersection carpark
	{2015.3630,-1717.2535,13.5547}, // Idlewood house
	{1654.7091,-1656.8516,22.5156}, // Right next to PD
	{1219.0851,-1812.8058,16.5938}, // Conference Center
	{1508.6849,-1059.0846,25.0625}, // Across the street of BANK - lots of cars in intersection carpark
	{1421.0819,-885.3383,50.6531}, // Outside house in vinewood
	{1133.8237,-1272.1558,13.5469}, // Near hospital
	{1235.2196,-1608.6111,13.5469}, // Backalley west of mainstreet
	{590.4648,-1252.2269,18.2116}, // Outside "BAnk of San Andreas"
	{842.5260,-1007.7679,28.4185}, // North of Graveyard
	{911.9332,-1232.6490,16.9766}, // LS Film Studio
	{477.6021,-1496.6207,20.4345}, // Rodeo Place
	{255.4621,-1366.3256,53.1094}, // Outside propery in richman
	{281.5446,-1261.4562,73.9319}, // Another richman property
	{790.1918,-839.8533,60.6328}, // Mulholland house
	{1299.1859,-801.4249,84.1406}, // Maddoggs
	{1240.3170,-2036.6886,59.9575}, // Verdant Bluffs
	{2215.5181,-2627.8174,13.5469}, // Ocean docks 1
	{2509.4346,-2637.6543,13.6453} // Ocean Docks spawn 2
};

// SF Spawns
new Float:SFPlayerSpawns[][CoordInfo] = {
	{-2027.31,-52.52002,35.36787},
	{-1616.0839,685.8605,7.1875},
	{-2416.532,333.3283,34.96875},
	{-2233.179,605.3428,37.35936},
	{-2310.585,725.8716,49.43679},
	{-2539.836,836.675,52.09375},
	{-2207.0391,1036.5848,80.0078},
	{-2671.8718,260.1898,4.6328},
	{-2625.486,1408.049,7.101563},
	{-1830.453,654.2467,30.43128},
	{-2305.0908,814.3279,48.2070},
	{-2451.2314,899.1155,58.4800},
	{-2126.086,-136.0137,35.32031},
	{-1983.151,138.7324,27.68746},
	{-1967.3479,286.0576,35.1727},
	{-1989.6471,711.8321,46.5625},
	{-2018.0929,970.2704,45.4453},
	{-2062.5583,237.4662,36.2890},
	{-2653.6443,1388.2767,8.0739},
	{-2642.2583,-274.9985,8.3506},
	{-2188.8037,609.8431,36.2624},
	{-1754.9976,958.5851,25.8386},
	{-2665.4282,635.6348,16.0054},
	{-1635.0077,665.8105,8.4054},
	{-1830.9324,638.9214,31.3054},
	{-2664.8037,938.6110,80.7618},
	{-2015.0903,153.0959,27.6875},
	{-1442.9570,-279.0339,14.1484},
	{-2787.5466,241.4044,7.1875},
	{-1752.8690,947.0851,24.8828},
	{-2636.6431,1353.0575,7.1292},
	{-2167.4729,-424.2656,35.3359},
	{-2262.3223,2302.2454,4.8202},
	{-2723.4639,-314.8138,7.1839},  // golf course spawn
	{-2694.5344,64.5550,4.3359},  // in front of a house
	{-2458.2000,134.5419,35.1719},  // hotel
	{-2796.6589,219.5733,7.1875},  // house
	{-2706.5261,397.7129,4.3672},  // park
	{-2866.7683,691.9363,23.4989},  // house
	{-2764.9543,785.6434,52.7813},  // donut shop
	{-2660.9402,883.2115,79.7738},  // house
	{-2861.0796,1047.7109,33.6068}, //  parking lot
	{-2629.2009,1383.1367,7.1833},  // parking lot at the bridge
	{-2079.6802,1430.0189,7.1016},  // pier
	{-1660.2294,1382.6698,9.8047}, //  pier 69
	{-1674.1964,430.3246,7.1797},  // gas station]
	{-1954.9982,141.8080,27.1747},  // train station
	{-1956.1447,287.1091,35.4688},  // car shop
	{-1888.1117,615.7245,35.1719},  // random
	{-1922.5566,886.8939,35.3359},  // random
	{-1983.3458,1117.0645,53.1243},  // church
	{-2417.6458,970.1491,45.2969},  // gas station
	{-2108.0171,902.8030,76.5792},  // house
	{-2097.5664,658.0771,52.3672},  // random
	{-2263.6650,393.7423,34.7708},  // random
	{-2287.5027,149.1875,35.3125},  // baseball parking lot
	{-2039.3571,-97.7205,35.1641},  // driving school
	{-1867.5022,-141.9203,11.8984},  // factory
	{-1537.8992,116.0441,17.3226},  // docks ship
	{-1708.4763,7.0187,3.5489},  // docks hangar
	{-1427.0858,-288.9430,14.1484},  // airport
	{-2173.0654,-392.7444,35.3359},  // stadium
	{-2320.5286,-180.3870,35.3135},  // burger shot
	{-2930.0049,487.2518,4.9141}  // harbor

};

// LV Spawns
new Float:LVPlayerSpawns[][CoordInfo] = {
	{1958.3783,1343.1572,15.3746},
	{2199.6531,1393.3678,10.8203},
	{2483.5977,1222.0825,10.8203},
	{2637.2712,1129.2743,11.1797},
	{2000.0106,1521.1111,17.0625},
	{2024.8190,1917.9425,12.3386},
	{2261.9048,2035.9547,10.8203},
	{2262.0986,2398.6572,10.8203},
	{2244.2566,2523.7280,10.8203},
	{2335.3228,2786.4478,10.8203},
	{2150.0186,2734.2297,11.1763},
	{2158.0811,2797.5488,10.8203},
	{1969.8301,2722.8564,10.8203},
	{1652.0555,2709.4072,10.8265},
	{1564.0052,2756.9463,10.8203},
	{1271.5452,2554.0227,10.8203},
	{1441.5894,2567.9099,10.8203},
	{1480.6473,2213.5718,11.0234},
	{1400.5906,2225.6960,11.0234},
	{1598.8419,2221.5676,11.0625},
	{1318.7759,1251.3580,10.8203},
	{1558.0731,1007.8292,10.8125},
	{1705.2347,1025.6808,10.8203},
	{1364.2782,1928.2811,16.6928},
	{1050.0034,1843.1744,13.8222},
	{2002.1711,1914.0096,40.3453},
	{1996.1078,1659.1333,22.1406},
	{2068.5278,730.0189,16.6928},
	{2597.6316,753.2578,17.3547},
	{2840.3926,1324.1161,11.3906},
	{2517.0908,1568.1057,11.5560},
	{2032.6072,1007.9802,10.8203},
	{1696.6542,1446.9955,10.7617},
	{2179.1113,1674.3562,11.0469},
	{2179.0591,1987.0236,10.8203},
	{2063.5339,2471.4036,10.8203},
	{2325.4937,570.6711,7.7813},
	{1435.8024,2662.3647,11.3926}, //  Northern train station
	{1457.4762,2773.4868,10.8203}, //  Northern golf club
	{1739.6390,2803.0569,14.2735}, //  Northern housing estate 1
	{1870.3096,2785.2471,14.2734}, //  Northern housing estate 2
	{1959.7142,2754.6863,10.8203}, //  Northern house 1
	{2314.2556,2759.4504,10.8203}, //  Northern industrial estate 1
	{2216.5674,2715.0334,10.8130}, //  Northern industrial estate 2
	{2101.4192,2678.7874,10.8130}, //  Northern near railway line
	{1951.1090,2660.3877,10.8203}, //  Northern house 2
	{1666.6949,2604.9861,10.8203}, //  Northern house 3
	{2808.3367,2421.5107,11.0625}, //  Northern shopping centre
	{2633.3203,2349.7061,10.6719}, //  V-Rock
	{2606.6348,2161.7490,10.8203}, //  South V-Rock
	{2616.5286,2100.6226,10.8158}, //  North Ammunation 1
	{2491.8816,2397.9370,10.8203}, //  North carpark 1
	{2531.7891,2530.3223,21.8750}, //  North carpark 2
	{2340.6677,2530.4324,10.8203}, //  North Pizza Stack
	{2097.6855,2491.3313,14.8390}, //  Emerald Isle
	{1893.1000,2423.2412,11.1782}, //  Souvenir shop
	{1698.9330,2241.8320,10.8203}, //  Northern casino
	{1479.4559,2249.0769,11.0234}, //  Baseball stadium 1
	{1298.1548,2083.4016,10.8127}, //  Baseball stadium 2
	{1117.8785,2304.1514,10.8203}, //  North carparks
	{1108.9878,1705.8639,10.8203}, //  Dirtring racing 1
	{1423.9780,1034.4188,10.8203}, //  Sumo
	{1537.4377,752.0641,11.0234}, //  Church
	{1917.9590,702.6984,11.1328}, //  Southern housing estate
	{2089.4785,658.0414,11.2707}, //  Southern house 1
	{2489.8286,928.3251,10.8280}, //  Wedding chapel
	{2697.4717,856.4916,9.8360}, //  Southern construction site
	{2845.6104,1288.1444,11.3906}, //  Southern train station
	{2437.9370,1293.1442,10.8203}, //  Wedding chapel (near Pyramid)
	{2299.5430,1451.4177,10.8203}, //  Carpark (near Pyramid)
	{2214.3008,2041.9165,10.8203}, //  Central parking lot
	{2005.9174,2152.0835,10.8203}, //  Central motel
	{2222.1042,1837.4220,10.8203}, //  Clowns Pocket
	{2025.6753,1916.4363,12.3382}, //  The Visage
	{2087.9902,1516.5336,10.8203}, //  Royal Casino
	{2172.1624,1398.7496,11.0625}, //  Auto Bahn
	{2139.1841,987.7975,10.8203}, //  Come-a-lot
	{1860.9672,1030.2910,10.8203}, //  Behind 4 Dragons
	{1673.2345,1316.1067,10.8203}, //  Airport carpark
	{1412.6187,2000.0596,14.7396} //  South baseball stadium houses
};

#define UnjailPlacesCount 8
new Float:UnjailPlace[UnjailPlacesCount][CoordInfo] = {
{633.365, -571.78, 16.340},
{1544.982, -1675.470, 13.600},
{-2164.396, -2388.342, 30.650},
{-1605.378, 717.512, 12.000},
{-1391.040, 2634.686, 55.984},
{-215.718, 985.399, 19.400},
{2335.229, 2455.809, 14.968},
{1225.165, 245.328, 19.554}
};

new TimerPlayer;

public Player_OnGameModeInit(){
	PlayerConfig();
	TimerPlayer = SetTimer("SyncPlayers", 1000, 1);
	SetInteriorWeapons(INTERIOR_JAIL,false);
	return 0;
}

PlayerConfig()
{
	Debug("player.inc > PlayerConfig - Start");
	if (!db_Exists(ConfigDB)) db_Create(ConfigDB);
	if (!db_Exists(DatabaseDB)) db_Create(DatabaseDB);

	new temp[MAX_STRING];
	set(temp,db_Get(DatabaseDB,"Player_DB"));
	if (strlen(temp) > 0) set(Player_DB,temp);
	else { set(temp,Player_DB); db_Set(DatabaseDB,"Player_DB",temp);  }
	set(temp,nullstr);
	set(temp,db_Get(ConfigDB,"Player_Start_Money"));
	if (strlen(temp) > 0) Player_Start_Money = strval(temp);
	else { valstr(temp,Player_Start_Money); db_Set(ConfigDB,"Player_Start_Money",temp);  }
	set(temp,nullstr);
	set(temp,db_Get(ConfigDB,"Player_Start_Bank_Money"));
	if (strlen(temp) > 0) Player_Start_Bank_Money = strval(temp);
	else { valstr(temp,Player_Start_Bank_Money); db_Set(ConfigDB,"Player_Start_Bank_Money",temp);  }
	set(temp,nullstr);
	set(temp,db_Get(ConfigDB,"Player_Start_Weapon1"));
	if (strlen(temp) > 0) Player_Start_Weapon1 = strval(temp);
	else { valstr(temp,Player_Start_Weapon1); db_Set(ConfigDB,"Player_Start_Weapon1",temp);  }
	set(temp,nullstr);
	set(temp,db_Get(ConfigDB,"Player_Start_Weapon2"));
	if (strlen(temp) > 0) Player_Start_Weapon2 = strval(temp);
	else { valstr(temp,Player_Start_Weapon2); db_Set(ConfigDB,"Player_Start_Weapon2",temp);  }
	set(temp,nullstr);
	set(temp,db_Get(ConfigDB,"Player_Start_Weapon3"));
	if (strlen(temp) > 0) Player_Start_Weapon3 = strval(temp);
	else { valstr(temp,Player_Start_Weapon3); db_Set(ConfigDB,"Player_Start_Weapon3",temp);  }
	set(temp,nullstr);
	set(temp,db_Get(ConfigDB,"Warns_For_Ban"));
	if (strlen(temp) > 0) Warns_For_Ban = strval(temp);
	else { valstr(temp,Warns_For_Ban); db_Set(ConfigDB,"Warns_For_Ban",temp);  }
	set(temp,nullstr);
	Debug("player.inc > PlayerConfig - Stop");
	return;
}

// needs to be ran by a timer
// sync player hp/money and check for mismatch
public SyncPlayers()
{
	Debug("player.inc > SyncPlayers - Start");
	for (new playerid = 0; playerid < MAX_PLAYERS_EX; playerid++)
	{
		if ((!IsPlayerConnected(playerid)) || (Player[playerid][IsActive] == 0)) continue;
		if (PlayerJustTeleported[playerid] > 0) PlayerJustTeleported[playerid]--;
		SpawnTime[playerid]++;
		if (Player[playerid][JailTime] != 0)
          	{
               		Player[playerid][JailTime]=Player[playerid][JailTime]-1;
               		if (Player[playerid][JailTime] == 0) UnJailPlayer(playerid);
               		if (Player[playerid][JailTime] < 0) Player[playerid][JailTime] = 0;
	  	}
          	if (Player[playerid][MuteTime] != 0)
          	{
              		Player[playerid][MuteTime]=Player[playerid][MuteTime]-1;
               		if (Player[playerid][MuteTime] < 0) Player[playerid][MuteTime] = 0;
	  	}
		if (MedicHealTime[playerid] != 0){
			MedicHealTime[playerid]=MedicHealTime[playerid]-1;
               		if (MedicHealTime[playerid] < 0) MedicHealTime[playerid] = 0;
		}
		new int = GetPlayerInterior(playerid);
		if(PlayerLastInterior[playerid] != int){
			PlayerJustTeleported[playerid]+=5;
		}
		PlayerLastInterior[playerid] = int;
		CheckPlayerKick(playerid);
	}
	Debug("player.inc > SyncPlayers - Stop");
	return;
}

// Check if any players have been marked to kick, and kicks them
public CheckPlayerKick(playerid)
{
	Debug("player.inc > CheckPlayerKick - Start");
	if(IsPlayerConnected(playerid))
	{
		if (KickList[playerid] == 1) // if we wanna kick this player
		{
			ShowTextForPlayer(playerid,gettext(409),999,5); //  send msg first
			TogglePlayerControllable(playerid,0);
			KickList[playerid]=2;
		}
		else if (KickList[playerid] >=2) // wait a second before doing the kicking (for msg to get through)
		{

			new logstring[256];
			format(logstring, sizeof (logstring), "player: %d:  %s: has been kicked",playerid,oGetPlayerName(playerid));
			WriteLog(logstring);
			KickList[playerid] = 0;
			//if (IsPlayerRegistered(playerid))
			//{
			//PlayerSave(playerid);
			//}
			GangMemberLogout(playerid,PlayerGangid[playerid]);
			player_Disconnect(playerid);
			Kick(playerid);
			Debug("player.inc > CheckPlayerKick - Stop");
			return;
		}
	}
	Debug("player.inc > CheckPlayerKick - Stop");
}

public KickPlayer(playerid,kickerid,reason[])
{
	Debug("player.inc > KickPlayer - Start");
	if (!IsPlayerConnected(playerid))
	{
		Debug("player.inc > KickPlayer - Stop");
		return;
	}
	new string[MAX_STRING];
	if (PlayerPriv[playerid][CannotBeKicked] == 1)
	{
		format(string,sizeof(string),gettext(410),reason);
		SystemMsg(playerid,COLOUR_ERROR,string);
		Debug("player.inc > KickPlayer - Stop");
		return;
	}
	if (KickList[playerid] != 0)
	{
		Debug("player.inc > KickPlayer - Stop");
		return; // we are already kicking this player
	}
	KickList[playerid]++;
	new kickername[MAX_NAME];
	if (kickerid == -1){ set(kickername,gettext(973));} else {set(kickername,oGetPlayerName(kickerid));}
	new playerip[IP_LEN]; playerip = ReturnPlayerIp(playerid);
	if (strlen(reason) > 0)
	{
		format(string,sizeof(string),gettext(411),kickername,reason);
		SystemMsg(playerid,COLOUR_RED,string);
		format(string,sizeof(string),gettext(412),oGetPlayerName(playerid),kickername,reason);
		SystemMsgToRegistered(COLOUR_INFO,string);
		new logstring[256];
		format(logstring, sizeof (logstring), "%s (%s) - kicked for %s",oGetPlayerName(playerid),playerip,reason);
		WriteSecurityLog(logstring);
		Kick(playerid);
	} else { // reason not set. kick without message.
		Kick(playerid);
		new logstring[256];
		format(logstring, sizeof (logstring), "%s (%s) - kicked without reason",oGetPlayerName(playerid),playerip);
		WriteSecurityLog(logstring);
	}
	Debug("player.inc > KickPlayer - Stop");
	return;
}

public BanPlayer(playerid,hours,reason[])
{
	if (hours < 1) return;
	Debug("player.inc > BanPlayer(playerid,reason[]) - Start");
	if (!IsPlayerConnected(playerid))
	{
		Debug("player.inc > BanPlayer - Stop");
		return;
	}
	new string[MAX_STRING];
	if (strlen(reason) == 0) set(reason,gettext(357));

	format(string,sizeof(string),gettext(414),hours,reason);
	SystemMsg(playerid,COLOUR_ERROR,string);
	format(string,sizeof(string),gettext(415),oGetPlayerName(playerid),hours,reason);
	SystemMsgToRegistered(COLOUR_INFO,string);
	TempBan(playerid,hours,reason);
	new logstring[256];
	format(logstring, sizeof (logstring), "%s (%s) - banned for %s on %d hours",playerid,oGetPlayerName(playerid),reason,hours);
	WriteSecurityLog(logstring);
	BanEx(playerid,reason);
	Debug("player.inc > BanPlayer - Stop");
	return;
}


public GivePlayerOwnedWeapons(playerid)
{
	Debug("player.inc > GivePlayerOwnedWeapons - Start");
	ResetPlayerWeapons(playerid);
	for (new weaponslot=0;weaponslot<PLAYER_WEAPON_SLOTS;weaponslot++)
	{	
		if (PlayerWeapons[playerid][weaponslot][wid] <= 0) continue;
		if (!IsPlayerAllowedWeapon(playerid,PlayerWeapons[playerid][weaponslot][wid])) continue;
		if (PlayerWeapons[playerid][weaponslot][wid] > 0)
		{
			GivePlayerWeapon(playerid,PlayerWeapons[playerid][weaponslot][wid],PlayerWeapons[playerid][weaponslot][bullets]);
		}
	}
	//ranging from 0 to 999, total 20 levels :)
	SetPlayerSkillLevel(playerid,WEAPONSKILL_PISTOL,Player[playerid][Level]*50);
    	SetPlayerSkillLevel(playerid,WEAPONSKILL_PISTOL_SILENCED,Player[playerid][Level]*50);
    	SetPlayerSkillLevel(playerid,WEAPONSKILL_DESERT_EAGLE,Player[playerid][Level]*50);
    	SetPlayerSkillLevel(playerid,WEAPONSKILL_SHOTGUN,Player[playerid][Level]*50);
    	SetPlayerSkillLevel(playerid,WEAPONSKILL_SAWNOFF_SHOTGUN,Player[playerid][Level]*50);
    	SetPlayerSkillLevel(playerid,WEAPONSKILL_SPAS12_SHOTGUN,Player[playerid][Level]*50);
    	SetPlayerSkillLevel(playerid,WEAPONSKILL_MICRO_UZI,Player[playerid][Level]*50);
    	SetPlayerSkillLevel(playerid,WEAPONSKILL_MP5,Player[playerid][Level]*50);
    	SetPlayerSkillLevel(playerid,WEAPONSKILL_AK47,Player[playerid][Level]*50);
    	SetPlayerSkillLevel(playerid,WEAPONSKILL_M4,Player[playerid][Level]*50);
    	SetPlayerSkillLevel(playerid,WEAPONSKILL_SNIPERRIFLE,Player[playerid][Level]*50);
	Debug("player.inc > GivePlayerOwnedWeapons - Stop");
}

public SendPlayerOwnedWeapons(playerid)
{
	Debug("player.inc > SendPlayerOwnedWeapons - Start");
	new string[MAX_STRING];
	SystemMsg(playerid,COLOUR_INFO,gettext(497));
	for (new weaponslot=0;weaponslot<PLAYER_WEAPON_SLOTS;weaponslot++)
	{
		if (PlayerWeapons[playerid][weaponslot][wid] <= 0) continue;
		if (PlayerWeapons[playerid][weaponslot][bullets] > 1)
		{
			format(string,sizeof(string),gettext(416),oGetWeaponName(PlayerWeapons[playerid][weaponslot][wid]),PlayerWeapons[playerid][weaponslot][bullets]);
			SystemMsg(playerid,COLOUR_INFO,string);
		}
		else if (PlayerWeapons[playerid][weaponslot][bullets] == 1)
		{
			format(string,sizeof(string),gettext(417),oGetWeaponName(PlayerWeapons[playerid][weaponslot][wid]));
			SystemMsg(playerid,COLOUR_INFO,string);
		}
	}
	Debug("player.inc > SendPlayerOwnedWeapons(playerid) - Start");
}

public player_Spawn(playerid)
{
	Debug("player.inc > player_Spawn - Start");
	SpawnTime[playerid] = 0;
	SetPlayerVirtualWorld(playerid,WORLD_DEFAULT);
	SetPlayerInterior(playerid,INTERIOR_DEFAULT);
	
	new rand;

	if ((PlayerSpawn[playerid][Coord_X] !=0.0) && (PlayerSpawn[playerid][Coord_Y] !=0.0) && (PlayerSpawn[playerid][Coord_Z] !=0.0))
	{
		oSetPlayerPos(playerid, PlayerSpawn[playerid][Coord_X], PlayerSpawn[playerid][Coord_Y], PlayerSpawn[playerid][Coord_Z]); // Warp the player
		PlayerSpawn[playerid][Coord_X] = 0.0;
		PlayerSpawn[playerid][Coord_Y] = 0.0;
		PlayerSpawn[playerid][Coord_Z] = 0.0;
	}
	else if (PlayerCity[playerid] != -1){
		if (PlayerCity[playerid] == 0)	
		{
			rand = random(sizeof(LSPlayerSpawns));
			oSetPlayerPos(playerid, LSPlayerSpawns[rand][Coord_X], LSPlayerSpawns[rand][Coord_Y], LSPlayerSpawns[rand][Coord_Z]);
		} else if (PlayerCity[playerid] == 1){
			rand = random(sizeof(SFPlayerSpawns));
			oSetPlayerPos(playerid, SFPlayerSpawns[rand][Coord_X], SFPlayerSpawns[rand][Coord_Y], SFPlayerSpawns[rand][Coord_Z]);
		} else if (PlayerCity[playerid] == 2){
			rand = random(sizeof(LVPlayerSpawns));
			oSetPlayerPos(playerid, LVPlayerSpawns[rand][Coord_X], LVPlayerSpawns[rand][Coord_Y], LVPlayerSpawns[rand][Coord_Z]);
		}
	} else {
		rand = random(3);
		if (rand == 0)	
		{
			rand = random(sizeof(LSPlayerSpawns));
			oSetPlayerPos(playerid, LSPlayerSpawns[rand][Coord_X], LSPlayerSpawns[rand][Coord_Y], LSPlayerSpawns[rand][Coord_Z]);
		} else if (rand == 1){
			rand = random(sizeof(SFPlayerSpawns));
			oSetPlayerPos(playerid, SFPlayerSpawns[rand][Coord_X], SFPlayerSpawns[rand][Coord_Y], SFPlayerSpawns[rand][Coord_Z]);
		} else if (rand == 2){
			rand = random(sizeof(LVPlayerSpawns));
			oSetPlayerPos(playerid, LVPlayerSpawns[rand][Coord_X], LVPlayerSpawns[rand][Coord_Y], LVPlayerSpawns[rand][Coord_Z]);
		}
	}

	if (!IsPlayerRegistered(playerid))
	{
		SystemMsg(playerid,COLOUR_INFO,gettext(418));
		SystemMsg(playerid,COLOUR_INFO,gettext(419));
		SystemMsg(playerid,COLOUR_INFO,gettext(420));
		new rnd = random(3);
		if (rnd == 0){
			rand = random(sizeof(LSPlayerSpawns));
			oSetPlayerPos(playerid, LSPlayerSpawns[rand][Coord_X], LSPlayerSpawns[rand][Coord_Y], LSPlayerSpawns[rand][Coord_Z]);
		} else if (rnd == 1){
			rand = random(sizeof(SFPlayerSpawns));
			oSetPlayerPos(playerid, SFPlayerSpawns[rand][Coord_X], SFPlayerSpawns[rand][Coord_Y], SFPlayerSpawns[rand][Coord_Z]);
		} else if (rnd == 2){
			rand = random(sizeof(LVPlayerSpawns));
			oSetPlayerPos(playerid, LVPlayerSpawns[rand][Coord_X], LVPlayerSpawns[rand][Coord_Y], LVPlayerSpawns[rand][Coord_Z]);
		}
	}
	oSetPlayerHealth(playerid,GetMaxHealth(playerid)); // set health based on level
	oSetPlayerMoney(playerid,Player[playerid][Money]);
	SetWantedLevel(playerid,Player[playerid][WantedLevel]);
	SetPlayerFightingStyle(playerid,Player[playerid][FightingStyle]);
	if (Player[playerid][GotJob] == JOB_COP){
		if (Player[playerid][CopLevel] == 0){
			SetPlayerSkin(playerid,SKIN_COP);
		} else if (Player[playerid][CopLevel] == 1){
			SetPlayerSkin(playerid,SKIN_GOODCOP);
		} else if (Player[playerid][CopLevel] == 2){
			SetPlayerSkin(playerid,SKIN_STREETPATROL);
		} else if (Player[playerid][CopLevel] == 3){
			SetPlayerSkin(playerid,SKIN_POLICEOFFICER);
		} else if (Player[playerid][CopLevel] == 4){
			SetPlayerSkin(playerid,SKIN_SWATMEMBER);
		} else if (Player[playerid][CopLevel] == 5){
			SetPlayerSkin(playerid,SKIN_FBIAGENT);
		}
		Player[playerid][Crimes] = 0;
		Player[playerid][WantedLevel] = 0;
	}
	if (Player[playerid][GotJob] == JOB_MEDIC){
		SetPlayerSkin(playerid,SKIN_MEDIC);
	}
	ResetPlayerWeapons(playerid);
	GivePlayerOwnedWeapons(playerid);
	CheckCurrentCrimes(playerid);
	SetSpecialColorForPlayer(playerid);
	Debug("player.inc > player_Spawn - Stop");
}

public player_ResetStats(playerid)
{
	Debug("player.inc > player_ResetStats - Start");
	// reset player stats
	PlayerRegistered[playerid] = false;

	Player[playerid][Level] = 0;
	Player[playerid][XP] = 0;
	Player[playerid][Drinks] = 0;
	Player[playerid][DrinksUsed] = 0;
	Player[playerid][ClothesBuyed] = 0;
	Player[playerid][GangMateKills] = 0;
	Player[playerid][RacesWon] = 0;
	Player[playerid][DMsWon] = 0;
	Player[playerid][HaysWon] = 0;
	Player[playerid][SpawnKills] = 0;
	Player[playerid][SpawnDeaths] = 0;
	Player[playerid][RacesPlayed] = 0;
	Player[playerid][DMsPlayed] = 0;
	Player[playerid][HaysPlayed] = 0;
	oSetPlayerMoney(playerid,0);
	Player[playerid][Kills] = 0;
	Player[playerid][Deaths] = 0;
	Player[playerid][Suicides] = 0;
	Player[playerid][SkyDives] = 0;
	Player[playerid][GotBusted] = 0;
	Player[playerid][WantedLevel] = 0;
	Player[playerid][Crimes] = 0;
	Player[playerid][WasAtHouse] = 0;
	Player[playerid][BoxingSkill] = 0;
	Player[playerid][BoxWins] = 0;
	Player[playerid][BoxLoses] = 0;
	Player[playerid][Warns] = 0;
	Player[playerid][FuelCans] = 0;
	Player[playerid][Hotwires] = 0;
	Player[playerid][FoodEaten] = 0;
	Player[playerid][GotJob] = 0;
	Player[playerid][CopSkill] = 0;
	Player[playerid][CopLevel] = 0;
	Player[playerid][HotwiresCop] = 0;
	Player[playerid][TotalPlaytime] = 0;
	Player[playerid][Bounty] = 0;
	Player[playerid][Heals] = 0;
	Player[playerid][MinesPlanted] = 0;
	Player[playerid][CashBox] = 0;
	Player[playerid][CashBoxCop] = 0;
	Player[playerid][CanUseBoats] = 0;
	Player[playerid][CanUseMotoBikes] = 0;
	Player[playerid][CanUseFlying] = 0;
	Player[playerid][TaxiRides] = 0;
	Player[playerid][TaxiPassengers] = 0;
	Player[playerid][LottoNumber] = 0;
	Player[playerid][SpawnPlace] = 0;
	Player[playerid][FightingStyle] = 0;
	KickList[playerid] = 0;
	ResetQuest(playerid);
	SpawnTime[playerid] = 0;
	PlayerGangid[playerid] = 0;
	set(PlayerGangName[playerid],nullstr);
	set(PlayerGangTag[playerid],"No GangTag");
	for (new i=0;i<PLAYER_WEAPON_SLOTS;i++)
	{
		PlayerWeapons[playerid][i][wid] = 0;
		PlayerWeapons[playerid][i][bullets] = 0;
	}
	PlayerWeapons[playerid][0][wid] = 0; // fists
	PlayerWeapons[playerid][0][bullets] = 1;
	PlayerJustTeleported[playerid] = 5;
	Player[playerid][JailTime] = 0;
	Player[playerid][MuteTime] = 0;

	AntiTeleLastPos[playerid][Coord_X] = AntiTeleLastPos[playerid][Coord_Y] = AntiTeleLastPos[playerid][Coord_Z] = 0.0;
	PlayerOldPos[playerid][Coord_X] = PlayerOldPos[playerid][Coord_Y] = PlayerOldPos[playerid][Coord_Z] = 0.0;
	PlayerLastInterior[playerid] = INTERIOR_DEFAULT;
	PlayerCity[playerid] = -1;
	PlayerSpawn[playerid][Coord_X] = PlayerSpawn[playerid][Coord_Y] = PlayerSpawn[playerid][Coord_Z] = 0.0;
	PlayerJustTeleported[playerid] = 5;
	ScriptMoneyUpdated[playerid] = 0;

	Debug("player.inc > player_ResetStats - Stop");
}

public IsPlayerRegistered(playerid)
{
	if (playerid == INVALID_PLAYER_ID) return 0;
	if (Player[playerid][IsActive] == 0) return 0;
	if (!IsPlayerConnected(playerid)) return 0;
	if (PlayerRegistered[playerid] == 0) return 0;
	return 1;
}

public player_Disconnect(playerid)
{
	Debug("player.inc > player_Disconnect - Start");
	if (IsPlayerRegistered(playerid))
	{
		PlayerSave(playerid);
	}
	GangMemberLogout(playerid,PlayerGangid[playerid]);
	
	Player[playerid][IsActive] = 0;
	player_ResetStats(playerid);
	player_ResetPrivilegies(playerid);
	Debug("player.inc > player_Disconnect - Stop");
}

public player_OnPlayerConnect(playerid)
{
	Debug("player.inc > player_OnPlayerConnect - Start");
	if (No_Numeric_Names == 1){
		if(isNumeric(oGetPlayerName(playerid))) {
	    		KickPlayer(playerid,INVALID_PLAYER_ID,gettext(1026));
			Debug("player.inc > player_OnPlayerConnect - Stop");
			return;
		}
	}
	Player[playerid][IsActive] = 1;
	PlayerCity[playerid] = -1;
	player_ResetStats(playerid);
	player_ResetPrivilegies(playerid);

	new playername[MAX_STRING];
	set(playername,oGetPlayerName(playerid));
	if (!NameCharCheck(playername))
	{
		KickPlayer(playerid,INVALID_PLAYER_ID,gettext(421)); // kick player
	}
	SetPlayerColor(playerid,COLOUR_PLAYER);
	ShowGameModeName(playerid);
  	set(PlayerGangTag[playerid],"No GangTag");
	new string[MAX_STRING];
	format(string,sizeof(string),gettext(1312),oGetPlayerName(playerid));
	SystemMsg(playerid,COLOUR_INFO,string);
	PlayerJustTeleported[playerid]+=5;
	Debug("player.inc > player_OnPlayerConnect - Stop");
}

public PlayerDeath(playerid,killerid,reason)
{
	if (IsPlayerInDuel(playerid)) return;
	Debug("player.inc > PlayerDeath - Start");
	if ((Player[playerid][Money] - 100) > 0)
	{
		if (Drop_Money_On_Death == 1) DropDeathMoney(playerid,100);
		oGivePlayerMoney(playerid,-100,0);
	}
	else
	{
		oSetPlayerMoney(playerid,0);
	}
	if (Player[playerid][XP] - (Player[playerid][XP] /200) > 0)
	{
		GivePlayerXP(playerid,-(Player[playerid][XP] /200),1); // take death penalty xp
	}
	else
	{
		Player[playerid][XP] = 0;
		SetPlayerScore(playerid,Player[playerid][XP]);
	}
	PlayerSpawn[playerid][Coord_X] = 0.0;
	PlayerSpawn[playerid][Coord_Y] = 0.0;
	PlayerSpawn[playerid][Coord_Z] = 0.0;
	Debug("player.inc > PlayerDeath - Stop");
}

public PlayerKill(killerid, victimid, reason) // earn xp, kills++
{
	if (killerid == INVALID_PLAYER_ID) return;
	Debug("player.inc > PlayerKill - Start");
	// check weapon
	new slot=GetWeaponSlot(reason);
	if (IsWeapon(reason))
	{
		if (PlayerTempWeapons[killerid][slot][wid] > 0)
		{
			if (PlayerWeapons[killerid][slot][bullets] > 0)
			{// if player has bullets for this weapon
				if (!IsWeaponHandToHand(reason))
				{
					new lostbullets;
					lostbullets = Vary(2,1);
					PlayerWeapons[killerid][slot][bullets] = PlayerWeapons[killerid][slot][bullets] - lostbullets;
					if (PlayerWeapons[killerid][slot][bullets] < 0) PlayerWeapons[killerid][slot][bullets] = 0;
				}
			}
		}
	}

	if (Player[killerid][GotJob] == JOB_COP){
		CopKill(killerid, victimid, reason);
	} else if (Player[killerid][GotJob] == JOB_MEDIC){
		MedicKill(killerid,victimid, reason);
	} else if (Player[killerid][GotJob] == JOB_TAXIDRIVER){
		TaxiKill(killerid, victimid, reason);
	} else {
		Player_Kill(killerid,victimid,reason);
	}
	Debug("player.inc > PlayerKill - Stop");
	return;
}

public Player_Kill(killerid,victimid,reason){
	Debug("player.inc > Player_Kill - Start");
	SetPlayerCriminal(killerid,gettext(422),1);
	// check gang kill
	if (PlayerGangid[killerid] != 0)
	{
		if (GangKill(PlayerGangid[killerid],killerid,victimid,reason) == 1)
		{
			Debug("player.inc > Player_Kill - Stop");
			return;
		}
	}
	// earn xp
	new earnedXP;
	earnedXP = ((1+Player[victimid][Level]) * 75);
	if (IsPlayerKilledInHotspotZone(victimid,killerid) == 1){
		GivePlayerXP(killerid, earnedXP * Hotspot_Multiplier,1);
	} else {
		GivePlayerXP(killerid, earnedXP,1);
	}
	// steal cash
	new stolencash;
	stolencash = oGetPlayerMoney(victimid);
	oGivePlayerMoney(victimid,-stolencash/2,1);
	if (Drop_Money_On_Death == 1){
		DropDeathMoney(victimid,stolencash/2);
	} else {
		oGivePlayerMoney(killerid,stolencash/2,1);
	}
	if(Player[victimid][Bounty] > 0){
		SystemMsgFormat(killerid,COLOUR_MONEY_GOOD,gettext(423),Player[victimid][Bounty]);
		Player[killerid][BankMoney] = Player[killerid][BankMoney] + Player[victimid][Bounty];
		Player[victimid][Bounty] = 0;
	}
	Debug("player.inc > Player_Kill - Stop");
}

public RegisterQuest(name[])
{
	Debug("player.inc > RegisterQuest - Start");
	if (QuestCount >= MAX_QUESTS) return INVALID_QUEST_ID;
	QuestCount++;
	new questid =QuestCount;
	set(Quests[questid][Quest_Name],name);
	Debug("player.inc > RegisterQuest - Stop");
	return questid;
}

public ResetQuest(playerid)
{
	Debug("player.inc > ResetQuest - Start");
	oDisablePlayerCheckpoint(playerid);
	oDisablePlayerRaceCheckpoint(playerid);
	PlayerQuest[playerid] = 0;
	Debug("player.inc > ResetQuest - Stop");
}

public oSetPlayerCheckpoint(playerid, Float:x, Float:y, Float:z, Float:size)
{
	Debug("player.inc > oSetPlayerCheckpoint - Start");
	DisablePlayerCheckpoint(playerid);
	PlayerCP[playerid][Coord_X] = x;
	PlayerCP[playerid][Coord_Y] = y;
	PlayerCP[playerid][Coord_Z] = z;
	PlayerCPActive[playerid] = true;
	SetPlayerCheckpoint(playerid, x, y, z, size);
	Debug("player.inc > oSetPlayerCheckpoint - Stop");
}

public oDisablePlayerCheckpoint(playerid)
{
	Debug("player.inc > oDisablePlayerCheckpoint - Start");
	DisablePlayerCheckpoint(playerid);
	PlayerCPActive[playerid] = false;
	PlayerCP[playerid][Coord_X] = 0.0;
	PlayerCP[playerid][Coord_Y] = 0.0;
	PlayerCP[playerid][Coord_Z] = 0.0;
	Debug("player.inc > oDisablePlayerCheckpoint - Stop");
}

public oIsPlayerInCheckpoint(playerid,Float:cpx,Float:cpy,Float:cpz,dist)
{
	Debug("player.inc > oIsPlayerInCheckpoint - Start");
	if (!IsPlayerConnected(playerid)) {Debug("player.inc > oIsPlayerInCheckpoint - Stop"); return 0;}
	if (!PlayerCPActive[playerid]) {Debug("player.inc > oIsPlayerInCheckpoint - Stop"); return 0;}
	if (!loccmp(cpx,cpy,cpz,PlayerCP[playerid][Coord_X],PlayerCP[playerid][Coord_Y],PlayerCP[playerid][Coord_Z])) {Debug("player.inc > oIsPlayerInCheckpoint - Stop"); return 0;}

	if (GetDistanceXYZtoXYZ(PlayerPos[playerid][Coord_X],PlayerPos[playerid][Coord_Y],PlayerPos[playerid][Coord_Z],cpx,cpy,cpz) < dist)
	{
		Debug("player.inc > oIsPlayerInCheckpoint - Stop");
		return IsPlayerInCheckpoint(playerid);
	}
	Debug("player.inc > oIsPlayerInCheckpoint - Stop");
	return 0;
}

public oSetPlayerRaceCheckpoint(playerid, type, Float:x, Float:y, Float:z, Float:nextx, Float:nexty, Float:nextz, Float:size)
{
	Debug("player.inc > oSetPlayerRaceCheckpoint - Start");
	DisablePlayerRaceCheckpoint(playerid);
	PlayerRaceCP[playerid][Coord_X] = x;
	PlayerRaceCP[playerid][Coord_Y] = y;
	PlayerRaceCP[playerid][Coord_Z] = z;
	PlayerRaceCPActive[playerid] = true;
	SetPlayerRaceCheckpoint(playerid, type, Float:x, Float:y, Float:z, Float:nextx, Float:nexty, Float:nextz, Float:size);
	Debug("player.inc > oSetPlayerRaceCheckpoint - Stop");
}

public oDisablePlayerRaceCheckpoint(playerid)
{
	Debug("player.inc > oDisablePlayerRaceCheckpoint - Start");
	DisablePlayerRaceCheckpoint(playerid);
	PlayerRaceCPActive[playerid] = false;
	PlayerRaceCP[playerid][Coord_X] = 0.0;
	PlayerRaceCP[playerid][Coord_Y] = 0.0;
	PlayerRaceCP[playerid][Coord_Z] = 0.0;
	Debug("player.inc > oDisablePlayerRaceCheckpoint - Stop");
}

public oIsPlayerInRaceCheckpoint(playerid,Float:cpx,Float:cpy,Float:cpz,dist)
{
	Debug("player.inc > oIsPlayerInRaceCheckpoint - Start");
	if (!IsPlayerConnected(playerid)) {Debug("player.inc > oIsPlayerInRaceCheckpoint - Stop"); return 0;}
	if (!PlayerRaceCPActive[playerid]) {Debug("player.inc > oIsPlayerInRaceCheckpoint - Stop"); return 0;}
	if (!loccmp(cpx,cpy,cpz,PlayerRaceCP[playerid][Coord_X],PlayerRaceCP[playerid][Coord_Y],PlayerRaceCP[playerid][Coord_Z])) {Debug("player.inc > oIsPlayerInRaceCheckpoint - Stop"); return 0;}

	if (GetDistanceXYZtoXYZ(PlayerPos[playerid][Coord_X],PlayerPos[playerid][Coord_Y],PlayerPos[playerid][Coord_Z],cpx,cpy,cpz) < dist)
	{
		return IsPlayerInRaceCheckpoint(playerid);
	}
	return 0;
}

public oSetPlayerPos(playerid, Float:x, Float:y, Float:z)
{
	Debug("player.inc > oSetPlayerPos - Start");
	PlayerJustTeleported[playerid]+=5;
	AntiTeleLastPos[playerid][Coord_X] = x;
	AntiTeleLastPos[playerid][Coord_Y] = y;
	AntiTeleLastPos[playerid][Coord_Z] = z;
	PlayerOldPos[playerid][Coord_X] = x;
	PlayerOldPos[playerid][Coord_Y] = y;
	PlayerOldPos[playerid][Coord_Z] = z;
	PlayerPos[playerid][Coord_X] = x;
	PlayerPos[playerid][Coord_Y] = y;
	PlayerPos[playerid][Coord_Z] = z;
	SetPlayerPos(playerid,x,y,z);
	Debug("player.inc > oSetPlayerPos - Stop");
}

public Float:GetMaxHealth(playerid)
{
	Debug("player.inc > Float:GetMaxHealth - Start");
	if (!IsPlayerConnected(playerid)) {Debug("player.inc > Float:GetMaxHealth - Stop"); return 0.0;}
	new Float:maxhealth=50.0;
//	if (Player[playerid][Level] == 0) {Debug("player.inc > Float:GetMaxHealth - Stop"); return 50.0;}
//	if (Player[playerid][Level] >= 10) {Debug("player.inc > Float:GetMaxHealth - Stop"); return 100.0;}
	maxhealth = (50.0 + (Player[playerid][Level] * 2.5));
	Debug("player.inc > Float:GetMaxHealth - Stop");
	return maxhealth;
}

public GetXPToLevel(playerid,level)
{
	Debug("player.inc > GetXPToLevel - Start");
	if (!IsPlayerConnected(playerid)) {Debug("player.inc > GetXPToLevel - Stop"); return 0;}
	new xptolevel;
	xptolevel=0;
	if ((level <= MAX_LEVEL) && (level > Player[playerid][Level]))
	{
		xptolevel =LevelList[level][level_xp] - Player[playerid][XP];
	}
	else
	{
		xptolevel = 0;
	}
	Debug("player.inc > GetXPToLevel - Stop");
	return xptolevel;
}

public CalculatePlayerLevel(playerid)
{
	Debug("player.inc > CalculatePlayerLevel - Start");
	if (!IsPlayerConnected(playerid)) {Debug("player.inc > CalculatePlayerLevel - Stop");return 0;}
	new level;
	level = 0;

	for (new i = 0; i <= MAX_LEVEL; i++)
	{
		if (Player[playerid][XP] >=LevelList[i][level_xp])
		{
			level = i;
		}
	}
	Debug("player.inc > CalculatePlayerLevel - Stop");
	return level;
}

public GetPlayerLevel(playerid)
{
	if (!IsPlayerConnected(playerid)) return 0;
	return Player[playerid][Level];
}

public GivePlayerXP(playerid, xpamount,showtext) // showtext = do we display "You gain %d XP"  to client
{
	Debug("player.inc > GivePlayerXP - Start");
	if (!IsPlayerConnected(playerid))
	{
		Debug("player.inc > GivePlayerXP - Stop");
		return 0;
	}

	if (xpamount == 0)
	{
		Debug("player.inc > GivePlayerXP - Stop");
		return 0;
	}
	if (PlayerGangid[playerid] != 0)
	{ // in a gang
		if (xpamount > 100)
		{
			new gangxp = xpamount / 4;
			GangGiveXP(PlayerGangid[playerid],gangxp,playerid);
		}
	}

	
	new texttime;
	if (xpamount <= 50)
	{
		texttime = 500;
	}
	else if (xpamount < 200)
	{
		texttime = 1000;
	}
	else if (xpamount < 500)
	{
		texttime = 1500;
	}
	else if (xpamount < 1000)
	{
		texttime = 2000;
	}
	else if (xpamount < 2000)
	{
		texttime = 4000;
	}
	else
	{
		texttime = 5000;
	}

	new string[MAX_STRING];
	if ((Player[playerid][XP] + xpamount) < 0) // if after add xp, playerxp < 0
	{
		Player[playerid][XP] = 0;
		if (showtext == 1)
		{
			format(string, sizeof(string),gettext(424), xpamount);
			SystemMsg(playerid,COLOUR_XP_BAD,string);
		}
		CheckPlayerLevel(playerid);
		SetPlayerScore(playerid,Player[playerid][XP]);
		Debug("player.inc > GivePlayerXP - Stop");
		return 0;
	}

	if ((Player[playerid][XP] + xpamount) > MAX_XP) // if after add, playerxp > MAX
	{
		if (showtext == 1)
		{
			format(string, sizeof(string),gettext(425), MAX_XP - Player[playerid][XP]);
			SystemMsg(playerid,COLOUR_XP_GOOD,string);
			SystemMsg(playerid,COLOUR_INFO,gettext(426));
		}
		Player[playerid][XP] = MAX_XP;
		CheckPlayerLevel(playerid);
		SetPlayerScore(playerid,Player[playerid][XP]);
		Debug("player.inc > GivePlayerXP - Stop");
		return 0;
	}


	if (xpamount >= 0)
	{
		format(string, sizeof(string),gettext(427), xpamount);
		ShowTextForPlayer(playerid,string,texttime,3);
	}
	else
	{
	/* I think this is here... I am right! :)
		new levelxp = LevelList[GetPlayerLevel(playerid)][level_xp];
		new xpfromlevel = Player[playerid][XP] - levelxp;
		if ((xpfromlevel + xpamount) < levelxp)
		{ // we cant go down levels
			xpamount = 0-xpfromlevel;
		}
		if (xpamount > 0)
		{
			format(string, sizeof(string),gettext(428), xpamount);
			ShowTextForPlayer(playerid,string,texttime,3);
		}
	*/
		format(string, sizeof(string),gettext(428), xpamount);
		ShowTextForPlayer(playerid,string,texttime,3);
	}


	Player[playerid][XP] = Player[playerid][XP] + xpamount;

	if (showtext==1)
	{
		if (xpamount > 0)
		{
			format(string, sizeof(string),gettext(425), xpamount);
			SystemMsg(playerid,COLOUR_XP_GOOD,string);
		}
		else if (xpamount > 0)
		{
			format(string, sizeof(string),gettext(424), xpamount);
			SystemMsg(playerid,COLOUR_XP_BAD,string);
		}
	}
	SetPlayerScore(playerid,Player[playerid][XP]);
	CheckPlayerLevel(playerid);
	Debug("player.inc > GivePlayerXP - Stop");
	return 0;
}

public CheckPlayerLevel(playerid)
{
	Debug("player.inc > CheckPlayerLevel - Start");
	if (!IsPlayerConnected(playerid))
	{
		Debug("player.inc > CheckPlayerLevel - Stop");
		return;
	}
	new level;
	level = CalculatePlayerLevel(playerid);
	if (Player[playerid][Level] < level)
	{
		PlayerLevelUp(playerid,level);
	}
	else if (Player[playerid][Level] > level)
	{
		PlayerLevelDown(playerid,level);
	}
	Debug("player.inc > CheckPlayerLevel - Stop");
}

public PlayerLevelDown(playerid,level)
{
	Debug("player.inc > PlayerLevelDown - Start");
	if (!IsPlayerConnected(playerid))
	{
		Debug("player.inc > PlayerLevelDown - Stop");
		return;
	}
	new string[256];
	Player[playerid][Level] = level;
	format(string, sizeof(string),gettext(429), Player[playerid][Level]);
	SystemMsg(playerid,COLOUR_XP_BAD,string);
	new logstring[256];
	format(logstring, sizeof (logstring), "player: %d:  %s: decreased his level to %d",playerid,oGetPlayerName(playerid),Player[playerid][Level]);
	WriteLog(logstring);
	PlayerPlaySoundOnPlayer(playerid,1057);
	Debug("player.inc > PlayerLevelDown - Stop");
}

public PlayerLevelUp(playerid,level)
{
	Debug("player.inc > PlayerLevelUp - Start");
	if (!IsPlayerConnected(playerid))
	{
		Debug("player.inc > PlayerLevelUp - Stop");
		return;
	}
	new string[256];
	Player[playerid][Level] = level;
	oSetPlayerHealth(playerid,GetMaxHealth(playerid));
	format(string, sizeof(string),gettext(430), Player[playerid][Level]);
	SystemMsg(playerid,COLOUR_XP_GOOD,string);
	new logstring[256];
	format(logstring, sizeof (logstring), "player: %d:  %s: increased his level to %d",playerid,oGetPlayerName(playerid),Player[playerid][Level]);
	WriteLog(logstring);
	PlayerPlaySoundOnPlayer(playerid,1057);

	new wepfound;
	for (new weaponid=1;weaponid<MAX_WEAPONS;weaponid++)
	{
		new lookupid=GetWeaponLookupID(weaponid);
		if (lookupid == -1) continue;
		if (Weapons[lookupid][Weapon_Allowed] == 0) continue;
		if (Weapons[lookupid][Weapon_Level] == GetPlayerLevel(playerid))
		{
			if (wepfound == 0)
			{
				SystemMsg(playerid,COLOUR_XP_GOOD,gettext(431));
				wepfound=1;
			}
			format(string, sizeof(string),gettext(432),oGetWeaponName(weaponid),GetWeaponCost(weaponid));
			SystemMsg(playerid,COLOUR_XP_GOOD,string);
		}
	}
	Debug("player.inc > PlayerLevelUp - Stop");
}

public oSetPlayerMoney(playerid,money)
{
	Debug("player.inc > oSetPlayerMoney - Start");
	if (!IsPlayerConnected(playerid))
	{
		Debug("player.inc > oSetPlayerMoney - Stop");
		return;
	}
	SetPlayerMoney(playerid,money);
	Player[playerid][Money] = money;
	new curHour, curMinute, curSecond;
	gettime(curHour, curMinute, curSecond);
	ScriptMoneyUpdated[playerid] = curSecond;
	Debug("player.inc > oSetPlayerMoney - Stop");
	return;
}

public oGivePlayerMoney(playerid,money,showtext)
{
	Debug("player.inc > oGivePlayerMoney - Start");
	if (!IsPlayerConnected(playerid))
	{
		Debug("player.inc > oGivePlayerMoney - Stop");
		return 0;
	}

	if (money == 0)
	{
		Debug("player.inc > oGivePlayerMoney - Stop");
		return 0;
	}

	new string[MAX_STRING];

	if ((Player[playerid][Money] + money) > MAX_MONEY)
	{
		oSetPlayerMoney(playerid,MAX_MONEY);
		format(string, sizeof(string), gettext(433), money);
		SystemMsg(playerid,COLOUR_INFO,string);
		new logstring[256];
		format(logstring, sizeof (logstring), "player: %d:  %s: is on max money",playerid,oGetPlayerName(playerid));
		WriteLog(logstring);
		Debug("player.inc > oGivePlayerMoney - Stop");
		return 0;
	}
	if ((Player[playerid][Money] + money) < 0)
	{
		oSetPlayerMoney(playerid,0);
		format(string, sizeof(string),gettext(434), money);
		SystemMsg(playerid,COLOUR_MONEY_BAD, string);
		Debug("player.inc > oGivePlayerMoney - Stop");
		return 0;
	}

	if (showtext == 1)
	{
		if (money >= 0)
		{
			format(string, sizeof(string),gettext(435), money);
			SystemMsg(playerid,COLOUR_MONEY_GOOD,string);
		}
		else
		{
			format(string, sizeof(string),gettext(436), money);
			SystemMsg(playerid,COLOUR_MONEY_BAD, string);
		}
	}

	oSetPlayerMoney(playerid,Player[playerid][Money] + money);
	Debug("player.inc > oGivePlayerMoney - Stop");
	return 0;
}

public oGetPlayerMoney(playerid)
{
	return Player[playerid][Money];
}

oGetPlayerName(playerid)
{
	new pname[MAX_PLAYER_NAME];
	GetPlayerName(playerid, pname, sizeof (pname));
	if (PlayerGangid[playerid] != 0){
		if (strcomp(PlayerGangTag[playerid],"No GangTag",true)==0){
			if ((strfind(pname,"[",true) != -1) && (strfind(pname,"]",true) != -1)){
				new whatcut[MAX_NAME];
				format(whatcut,sizeof(whatcut),"[%s]",PlayerGangTag[playerid]);
				strcut(pname,whatcut);
			}
		}
	}
	return pname;
}

public Float:oGetPlayerHealth(playerid)
{
	new Float:health;
	GetPlayerHealth(playerid, health);
	return health;
}

public Float:oGetPlayerArmour(playerid)
{
	new Float:armour;
	GetPlayerArmour(playerid, armour);
	return armour;
}

public oSetPlayerHealth(playerid,Float:health)
{
	Debug("player.inc > oSetPlayerHealth - Start");
	Player[playerid][Health] = health;
	SetPlayerHealth(playerid, health);
	Debug("player.inc > oSetPlayerHealth - Stop");
}

public oGivePlayerWeapon(playerid,weaponid,ammo)
{
	Debug("player.inc > oGivePlayerWeapon - Start");
	if (!IsPlayerAllowedWeapon(playerid,weaponid)) return;
	if (ammo > 0)
	{
		new slot = GetWeaponSlot(weaponid);
		if (!IsWeaponHandToHand(weaponid))
		{
			PlayerWeapons[playerid][slot][wid] = weaponid;
			PlayerWeapons[playerid][slot][bullets] = PlayerWeapons[playerid][slot][bullets] + ammo;
		}
		else
		{
			PlayerWeapons[playerid][slot][wid] = weaponid;
			PlayerWeapons[playerid][slot][bullets] = 1;
		}
		GivePlayerWeapon(playerid,weaponid,ammo);
	}
	Debug("player.inc > oGivePlayerWeapon - Stop");
}

public PlayerSave(playerid)
{
	Debug("player.inc > PlayerSave - Start");
	if (!IsPlayerRegistered(playerid)) {Debug("player.inc > PlayerSave - Stop");return 0;}

	player_save_db_dini(playerid);
	player_save_privilegies(playerid);

	new logstring[256];
	format(logstring, sizeof (logstring), "player: %d:  %s: player_saved successfully",playerid,oGetPlayerName(playerid));
	WriteLog(logstring);
	Debug("player.inc > PlayerSave - Stop");
	return 1;
}

public CreatePlayer(playerid)
{
	Debug("player.inc > CreatePlayer - Start");
	oSetPlayerMoney(playerid,Player_Start_Money);
	ResetPlayerWeapons(playerid);
	new bullets2 = 100;
	if (Player_Start_Weapon1 != 0)
	{
		if (bullets2 > GetWeaponMaxAmmo(Player_Start_Weapon1))
		{
			oGivePlayerWeapon(playerid,Player_Start_Weapon1,GetWeaponMaxAmmo(Player_Start_Weapon1));
		}
		else
		{
			oGivePlayerWeapon(playerid,Player_Start_Weapon1,bullets2);
		}
	}
	if (Player_Start_Weapon2 != 0)
	{
		if (bullets2 > GetWeaponMaxAmmo(Player_Start_Weapon2))
		{
			oGivePlayerWeapon(playerid,Player_Start_Weapon2,GetWeaponMaxAmmo(Player_Start_Weapon2));
		}
		else
		{
			oGivePlayerWeapon(playerid,Player_Start_Weapon2,bullets2);
		}
	}
	if (Player_Start_Weapon3 != 0)
	{
		if (bullets2 > GetWeaponMaxAmmo(Player_Start_Weapon3))
		{
			oGivePlayerWeapon(playerid,Player_Start_Weapon3,GetWeaponMaxAmmo(Player_Start_Weapon3));
		}
		else
		{
			oGivePlayerWeapon(playerid,Player_Start_Weapon3,bullets2);
		}
	}
	player_save_db_dini(playerid);

	new logstring[256];
	format(logstring, sizeof (logstring), "player: %d:  %s: player_created successfully",playerid,oGetPlayerName(playerid));
	WriteLog(logstring);
	Debug("player.inc > CreatePlayer - Stop");
	return;
}

SetWeaponsFromDBString(playerid,dbstring[])
{
	Debug("player.inc > SetWeaponsFromDBString - Start");
	new idx;
	for (new i=0;i<PLAYER_WEAPON_SLOTS;i++)
	{
		PlayerWeapons[playerid][i][wid] = strval(strcharsplit(dbstring,idx,strchar("/")));
		PlayerWeapons[playerid][i][bullets] = strval(strcharsplit(dbstring,idx,strchar("|")));
	}
	Debug("player.inc > SetWeaponsFromDBString - Stop");
}

CreateWeaponDBString(playerid)
{
	Debug("player.inc > CreateWeaponDBString - Start");
	new wepstr[MAX_STRING];
	for (new i=0;i<PLAYER_WEAPON_SLOTS;i++)
	{
		new temp[MAX_STRING];
		valstr(temp,PlayerWeapons[playerid][i][wid]);
		strins(wepstr, temp, strlen(wepstr));
		wepstr[strlen(wepstr)] =strchar("/");
		valstr(temp,PlayerWeapons[playerid][i][bullets]);
		strins(wepstr, temp, strlen(wepstr));
		wepstr[strlen(wepstr)] =strchar("|");
	}
	Debug("player.inc > CreateWeaponDBString - Stop");
	return wepstr;
}

player_save_db_dini(playerid)
{
	Debug("player.inc > player_save_db_dini - Start");

	if (strlen(oGetPlayerName(playerid)) < 1)
	{
		Debug("player.inc > player_save_db_dini - Stop");
		return;
	}
	if (!IsPlayerConnected(playerid))
	{
		Debug("player.inc > player_save_db_dini - Stop");
		return;
	}
	new dinifilename[MAX_STRING];
	format(dinifilename,sizeof(dinifilename),"%s%s.txt",Player_DB,EncodeName(oGetPlayerName(playerid)));
	if (!db_Exists(dinifilename))
	{
		db_Create(dinifilename);
	}

	new temp[MAX_STRING];

	#if DINI_OPTIMIZE == 1 && DB_TYPE == 1
	new File:file = fopen(dinifilename,io_write);
	if (file){
		new writeit[MAX_STRING];
		format(writeit,MAX_STRING,"Gang=%s\r\n",PlayerGangName[playerid]); fwrite(file,writeit);
	
		format(writeit,MAX_STRING,"GangTag=%s\r\n",PlayerGangTag[playerid]); fwrite(file,writeit);
	
		format(writeit,MAX_STRING,"Level=%d\r\n",Player[playerid][Level]); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"XP=%d\r\n",Player[playerid][XP]); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"Money=%d\r\n",Player[playerid][Money]); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"BankMoney=%d\r\n",Player[playerid][BankMoney]); fwrite(file,writeit);	
	
		format(writeit,MAX_STRING,"Deaths=%d\r\n",Player[playerid][Deaths]); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"Suicides=%d\r\n",Player[playerid][Suicides]); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"Kills=%d\r\n",Player[playerid][Kills]); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"JailTime=%d\r\n",Player[playerid][JailTime]); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"MuteTime=%d\r\n",Player[playerid][MuteTime]); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"Drinks=%d\r\n",Player[playerid][Drinks]); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"DrinksUsed=%d\r\n",Player[playerid][DrinksUsed]); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"ClothesBuyed=%d\r\n",Player[playerid][ClothesBuyed]); fwrite(file,writeit);
			
		format(writeit,MAX_STRING,"GangMateKills=%d\r\n",Player[playerid][GangMateKills]); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"RacesWon=%d\r\n",Player[playerid][RacesWon]); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"DMsWon=%d\r\n",Player[playerid][DMsWon]); fwrite(file,writeit);

		format(writeit,MAX_STRING,"HaysWon=%d\r\n",Player[playerid][HaysWon]); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"SpawnKills=%d\r\n",Player[playerid][SpawnKills]); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"SpawnDeaths=%d\r\n",Player[playerid][SpawnDeaths]); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"RacesPlayed=%d\r\n",Player[playerid][RacesPlayed]); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"DMsPlayed=%d\r\n",Player[playerid][DMsPlayed]); fwrite(file,writeit);

		format(writeit,MAX_STRING,"HaysPlayed=%d\r\n",Player[playerid][HaysPlayed]); fwrite(file,writeit);
			
		format(writeit,MAX_STRING,"SkyDives=%d\r\n",Player[playerid][SkyDives]); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"GotBusted=%d\r\n",Player[playerid][GotBusted]); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"WantedLevel=%d\r\n",Player[playerid][WantedLevel]); fwrite(file,writeit);	
			
		format(writeit,MAX_STRING,"Crimes=%d\r\n",Player[playerid][Crimes]); fwrite(file,writeit);	
		
		format(writeit,MAX_STRING,"WasAtHouse=%d\r\n",Player[playerid][WasAtHouse]); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"BoxingSkill=%d\r\n",Player[playerid][BoxingSkill]); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"BoxWins=%d\r\n",Player[playerid][BoxWins]); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"BoxLoses=%d\r\n",Player[playerid][BoxLoses]); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"Warns=%d\r\n",Player[playerid][Warns]); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"FuelCans=%d\r\n",Player[playerid][FuelCans]); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"Hotwires=%d\r\n",Player[playerid][Hotwires]); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"FoodEaten=%d\r\n",Player[playerid][FoodEaten]); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"GotJob=%d\r\n",Player[playerid][GotJob]); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"CopSkill=%d\r\n",Player[playerid][CopSkill]); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"CopLevel=%d\r\n",Player[playerid][CopLevel]); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"HotwiresCop=%d\r\n",Player[playerid][HotwiresCop]); fwrite(file,writeit);
	
		format(writeit,MAX_STRING,"TotalPlaytime=%d\r\n",Player[playerid][TotalPlaytime]); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"Bounty=%d\r\n",Player[playerid][Bounty]); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"Heals=%d\r\n",Player[playerid][Heals]); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"MinesPlanted=%d\r\n",Player[playerid][MinesPlanted]); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"CashBox=%d\r\n",Player[playerid][CashBox]); fwrite(file,writeit);	
		
		format(writeit,MAX_STRING,"CashBoxCop=%d\r\n",Player[playerid][CashBoxCop]); fwrite(file,writeit);
	
		format(writeit,MAX_STRING,"CanUseBoats=%d\r\n",Player[playerid][CanUseBoats]); fwrite(file,writeit);
	
		format(writeit,MAX_STRING,"CanUseMotoBikes=%d\r\n",Player[playerid][CanUseMotoBikes]); fwrite(file,writeit);
	
		format(writeit,MAX_STRING,"CanUseFlying=%d\r\n",Player[playerid][CanUseFlying]); fwrite(file,writeit);
	
		format(writeit,MAX_STRING,"TaxiRides=%d\r\n",Player[playerid][TaxiRides]); fwrite(file,writeit);
	
		format(writeit,MAX_STRING,"TaxiPassengers=%d\r\n",Player[playerid][TaxiPassengers]); fwrite(file,writeit);
	
		format(writeit,MAX_STRING,"LottoNumber=%d\r\n",Player[playerid][LottoNumber]); fwrite(file,writeit);

		format(writeit,MAX_STRING,"SpawnPlace=%d\r\n",Player[playerid][SpawnPlace]); fwrite(file,writeit);

		format(writeit,MAX_STRING,"FightingStyle=%d\r\n",Player[playerid][FightingStyle]); fwrite(file,writeit);

		if (PlayerPos[playerid][Coord_Z] > 900)
		{
			PlayerPos[playerid][Coord_X]=0;
			PlayerPos[playerid][Coord_Y]=0;
			PlayerPos[playerid][Coord_Z]=0;
		}
		format(temp,sizeof(temp),"%f",PlayerPos[playerid][Coord_X]);
		format(writeit,MAX_STRING,"X=%s\r\n",temp); fwrite(file,writeit);
		
		format(temp,sizeof(temp),"%f",PlayerPos[playerid][Coord_Y]);
		format(writeit,MAX_STRING,"Y=%s\r\n",temp); fwrite(file,writeit);
		
		format(temp,sizeof(temp),"%f",PlayerPos[playerid][Coord_Z]);
		format(writeit,MAX_STRING,"Z=%s\r\n",temp); fwrite(file,writeit);
		
		format(writeit,MAX_STRING,"WeaponsData=%s\r\n",CreateWeaponDBString(playerid)); fwrite(file,writeit);

		format(writeit,MAX_STRING,"WeaponsStatKills=%s\r\n",CreateWeaponKillStatDBString(playerid)); fwrite(file,writeit);

		format(writeit,MAX_STRING,"WeaponsStatDeaths=%s\r\n",CreateWeaponDeathStatDBString(playerid)); fwrite(file,writeit);

		format(writeit,MAX_STRING,"Packets=%s\r\n",CreatePacketDBString(playerid)); fwrite(file,writeit);

		format(writeit,MAX_STRING,"Horseshoes=%s\r\n",CreateHorseshoeDBString(playerid)); fwrite(file,writeit);

		format(writeit,MAX_STRING,"Oysters=%s\r\n",CreateOysterDBString(playerid)); fwrite(file,writeit);

		fclose(file);
	} else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(1473));
	}
	#else
	db_BeforeBigSaving(dinifilename);

	db_Set(dinifilename,"Gang",PlayerGangName[playerid]);
	
	db_Set(dinifilename,"GangTag",PlayerGangTag[playerid]);
	
	valstr(temp,Player[playerid][Level]);
	db_Set(dinifilename,"Level",temp);
	
	valstr(temp,Player[playerid][XP]);
	db_Set(dinifilename,"XP",temp);
	
	valstr(temp,Player[playerid][Money]);
	db_Set(dinifilename,"Money",temp);
	
	valstr(temp,Player[playerid][BankMoney]);
	db_Set(dinifilename,"BankMoney",temp);

	valstr(temp,Player[playerid][Deaths]);
	db_Set(dinifilename,"Deaths",temp);
	
	valstr(temp,Player[playerid][Suicides]);
	db_Set(dinifilename,"Suicides",temp);
	
	valstr(temp,Player[playerid][Kills]);
	db_Set(dinifilename,"Kills",temp);
	
	valstr(temp,Player[playerid][JailTime]);
	db_Set(dinifilename,"JailTime",temp);
	
	valstr(temp,Player[playerid][MuteTime]);
	db_Set(dinifilename,"MuteTime",temp);
	
	valstr(temp,Player[playerid][Drinks]);
	db_Set(dinifilename,"Drinks",temp);
	
	valstr(temp,Player[playerid][DrinksUsed]);
	db_Set(dinifilename,"DrinksUsed",temp);
	
	valstr(temp,Player[playerid][ClothesBuyed]);
	db_Set(dinifilename,"ClothesBuyed",temp);
		
	valstr(temp,Player[playerid][GangMateKills]);
	db_Set(dinifilename,"GangMateKills",temp);
	
	valstr(temp,Player[playerid][RacesWon]);
	db_Set(dinifilename,"RacesWon",temp);
	
	valstr(temp,Player[playerid][DMsWon]);
	db_Set(dinifilename,"DMsWon",temp);

	valstr(temp,Player[playerid][HaysWon]);
	db_Set(dinifilename,"HaysWon",temp);
	
	valstr(temp,Player[playerid][SpawnKills]);
	db_Set(dinifilename,"SpawnKills",temp);
	
	valstr(temp,Player[playerid][SpawnDeaths]);
	db_Set(dinifilename,"SpawnDeaths",temp);
	
	valstr(temp,Player[playerid][RacesPlayed]);
	db_Set(dinifilename,"RacesPlayed",temp);
	
	valstr(temp,Player[playerid][DMsPlayed]);
	db_Set(dinifilename,"DMsPlayed",temp);

	valstr(temp,Player[playerid][HaysPlayed]);
	db_Set(dinifilename,"HaysPlayed",temp);
	
	valstr(temp,Player[playerid][SkyDives]);
	db_Set(dinifilename,"SkyDives",temp);
	
	valstr(temp,Player[playerid][GotBusted]);
	db_Set(dinifilename,"GotBusted",temp);
	
	valstr(temp,Player[playerid][WantedLevel]);
	db_Set(dinifilename,"WantedLevel",temp);	
	
	valstr(temp,Player[playerid][Crimes]);
	db_Set(dinifilename,"Crimes",temp);	
	
	valstr(temp,Player[playerid][WasAtHouse]);
	db_Set(dinifilename,"WasAtHouse",temp);
	
	valstr(temp,Player[playerid][BoxingSkill]);
	db_Set(dinifilename,"BoxingSkill",temp);
	
	valstr(temp,Player[playerid][BoxWins]);
	db_Set(dinifilename,"BoxWins",temp);
	
	valstr(temp,Player[playerid][BoxLoses]);
	db_Set(dinifilename,"BoxLoses",temp);
	
	valstr(temp,Player[playerid][Warns]);
	db_Set(dinifilename,"Warns",temp);
	
	valstr(temp,Player[playerid][FuelCans]);
	db_Set(dinifilename,"FuelCans",temp);
	
	valstr(temp,Player[playerid][Hotwires]);
	db_Set(dinifilename,"Hotwires",temp);
	
	valstr(temp,Player[playerid][FoodEaten]);
	db_Set(dinifilename,"FoodEaten",temp);
	
	valstr(temp,Player[playerid][GotJob]);
	db_Set(dinifilename,"GotJob",temp);
	
	valstr(temp,Player[playerid][CopSkill]);
	db_Set(dinifilename,"CopSkill",temp);
	
	valstr(temp,Player[playerid][CopLevel]);
	db_Set(dinifilename,"CopLevel",temp);
	
	valstr(temp,Player[playerid][HotwiresCop]);
	db_Set(dinifilename,"HotwiresCop",temp);
	
	valstr(temp,Player[playerid][TotalPlaytime]);
	db_Set(dinifilename,"TotalPlaytime",temp);
	
	valstr(temp,Player[playerid][Bounty]);
	db_Set(dinifilename,"Bounty",temp);
	
	valstr(temp,Player[playerid][Heals]);
	db_Set(dinifilename,"Heals",temp);
	
	valstr(temp,Player[playerid][MinesPlanted]);
	db_Set(dinifilename,"MinesPlanted",temp);
	
	valstr(temp,Player[playerid][CashBox]);
	db_Set(dinifilename,"CashBox",temp);	
	
	valstr(temp,Player[playerid][CashBoxCop]);
	db_Set(dinifilename,"CashBoxCop",temp);

	valstr(temp,Player[playerid][CanUseBoats]);
	db_Set(dinifilename,"CanUseBoats",temp);

	valstr(temp,Player[playerid][CanUseMotoBikes]);
	db_Set(dinifilename,"CanUseMotoBikes",temp);

	valstr(temp,Player[playerid][CanUseFlying]);
	db_Set(dinifilename,"CanUseFlying",temp);

	valstr(temp,Player[playerid][TaxiRides]);
	db_Set(dinifilename,"TaxiRides",temp);

	valstr(temp,Player[playerid][TaxiPassengers]);
	db_Set(dinifilename,"TaxiPassengers",temp);

	valstr(temp,Player[playerid][LottoNumber]);
	db_Set(dinifilename,"LottoNumber",temp);

	valstr(temp,Player[playerid][SpawnPlace]);
	db_Set(dinifilename,"SpawnPlace",temp);

	valstr(temp,Player[playerid][FightingStyle]);
	db_Set(dinifilename,"FightingStyle",temp);


	if (PlayerPos[playerid][Coord_Z] > 900)
	{
		PlayerPos[playerid][Coord_X]=0;
		PlayerPos[playerid][Coord_Y]=0;
		PlayerPos[playerid][Coord_Z]=0;
	}
	format(temp,sizeof(temp),"%f",PlayerPos[playerid][Coord_X]);
	db_Set(dinifilename,"X",temp);
	
	format(temp,sizeof(temp),"%f",PlayerPos[playerid][Coord_Y]);
	db_Set(dinifilename,"Y",temp);
	
	format(temp,sizeof(temp),"%f",PlayerPos[playerid][Coord_Z]);
	db_Set(dinifilename,"Z",temp);
	
	db_Set(dinifilename,"WeaponsData",CreateWeaponDBString(playerid));

	db_Set(dinifilename,"WeaponsStatKills",CreateWeaponKillStatDBString(playerid));

	db_Set(dinifilename,"WeaponsStatDeaths",CreateWeaponDeathStatDBString(playerid));

	db_Set(dinifilename,"Packets",CreatePacketDBString(playerid));

	db_Set(dinifilename,"Horseshoes",CreateHorseshoeDBString(playerid));

	db_Set(dinifilename,"Oysters",CreateOysterDBString(playerid));

	db_AfterBigSaving(dinifilename);

	#endif

	Debug("player.inc > player_save_db_dini - Stop");

}


public PlayerLoadStats(playerid)
{
	Debug("player.inc > PlayerLoadStats - Start");
	if (!IsPlayerRegistered(playerid)) {Debug("player.inc > PlayerLoadStats - Stop"); return 0;} // login to acct first

	player_load_db_dini(playerid);
	player_load_privilegies(playerid);
	Debug("player.inc > PlayerLoadStats - Stop");
	return 1;
}

player_load_db_dini(playerid)
{
	Debug("player.inc > player_load_db_dini - Start");

	if (!IsPlayerConnected(playerid)){
		Debug("player.inc > player_load_db_dini - Stop");
		return;
	}
	new dinifilename[MAX_STRING];
	format(dinifilename,sizeof(dinifilename),"%s%s.txt",Player_DB,EncodeName(oGetPlayerName(playerid)));

	if (!db_Exists(dinifilename))
	{
		Debug("player.inc > player_load_db_dini - Stop");
		return;
	}

	new gangname[MAX_NAME];
	new gangtag[MAX_NAME];

	#if DINI_OPTIMIZE == 1 && DB_TYPE == 1
	new File: UserFile = fopen(dinifilename, io_read);
	if (UserFile)
	{
		new key[MAX_STRING],val[MAX_STRING];
		new Data[MAX_STRING];
		while (fread(UserFile,Data,MAX_STRING))
		{
			StripNewLine(Data);
			set(key,ini_GetKey(Data));
			if(strcomp(key,"Gang",true)==1) {set(val,ini_GetValue(Data));set(gangname,val);}
			if(strcomp(key,"GangTag",true)==1) {set(val,ini_GetValue(Data));set(gangtag,val);}
			if(strcomp(key,"Level",true)==1) {set(val,ini_GetValue(Data));Player[playerid][Level] = strval(val);}
			if(strcomp(key,"XP",true)==1) {set(val,ini_GetValue(Data));Player[playerid][XP] = strval(val);}
			if(strcomp(key,"Money",true)==1) {set(val,ini_GetValue(Data));oSetPlayerMoney(playerid,strval(val));}
			if(strcomp(key,"BankMoney",true)==1) {set(val,ini_GetValue(Data));Player[playerid][BankMoney] = strval(val);}
			if(strcomp(key,"Deaths",true)==1) {set(val,ini_GetValue(Data));Player[playerid][Deaths] = strval(val);}
			if(strcomp(key,"Suicides",true)==1) {set(val,ini_GetValue(Data));Player[playerid][Suicides] = strval(val);}
			if(strcomp(key,"Kills",true)==1) {set(val,ini_GetValue(Data));Player[playerid][Kills] = strval(val);}
			if(strcomp(key,"JailTime",true)==1) {set(val,ini_GetValue(Data));Player[playerid][JailTime] = strval(val);}
			if(strcomp(key,"MuteTime",true)==1) {set(val,ini_GetValue(Data));Player[playerid][MuteTime] = strval(val);}
			if(strcomp(key,"Drinks",true)==1) {set(val,ini_GetValue(Data));Player[playerid][Drinks] = strval(val);}
			if(strcomp(key,"DrinksUsed",true)==1) {set(val,ini_GetValue(Data));Player[playerid][DrinksUsed] = strval(val);}
			if(strcomp(key,"ClothesBuyed",true)==1) {set(val,ini_GetValue(Data));Player[playerid][ClothesBuyed] = strval(val);}
			if(strcomp(key,"GangMateKills",true)==1) {set(val,ini_GetValue(Data));Player[playerid][GangMateKills] = strval(val);}
			if(strcomp(key,"RacesWon",true)==1) {set(val,ini_GetValue(Data));Player[playerid][RacesWon] = strval(val);}
			if(strcomp(key,"DMsWon",true)==1) {set(val,ini_GetValue(Data));Player[playerid][DMsWon] = strval(val);}
			if(strcomp(key,"HaysWon",true)==1) {set(val,ini_GetValue(Data));Player[playerid][HaysWon] = strval(val);}
			if(strcomp(key,"SpawnKills",true)==1) {set(val,ini_GetValue(Data));Player[playerid][SpawnKills] = strval(val);}
			if(strcomp(key,"SpawnDeaths",true)==1) {set(val,ini_GetValue(Data));Player[playerid][SpawnDeaths] = strval(val);}
			if(strcomp(key,"RacesPlayed",true)==1) {set(val,ini_GetValue(Data));Player[playerid][RacesPlayed] = strval(val);}
			if(strcomp(key,"DMsPlayed",true)==1) {set(val,ini_GetValue(Data));Player[playerid][DMsPlayed] = strval(val);}
			if(strcomp(key,"HaysPlayed",true)==1) {set(val,ini_GetValue(Data));Player[playerid][HaysPlayed] = strval(val);}
			if(strcomp(key,"SkyDives",true)==1) {set(val,ini_GetValue(Data));Player[playerid][SkyDives] = strval(val);}
			if(strcomp(key,"GotBusted",true)==1) {set(val,ini_GetValue(Data));Player[playerid][GotBusted] = strval(val);}
			if(strcomp(key,"WantedLevel",true)==1) {set(val,ini_GetValue(Data));Player[playerid][WantedLevel] = strval(val);}
			if(strcomp(key,"Crimes",true)==1) {set(val,ini_GetValue(Data));Player[playerid][Crimes] = strval(val);}
			if(strcomp(key,"WasAtHouse",true)==1) {set(val,ini_GetValue(Data));Player[playerid][WasAtHouse] = strval(val);}
			if(strcomp(key,"BoxingSkill",true)==1) {set(val,ini_GetValue(Data));Player[playerid][BoxingSkill] = strval(val);}
			if(strcomp(key,"BoxWins",true)==1) {set(val,ini_GetValue(Data));Player[playerid][BoxWins] = strval(val);}
			if(strcomp(key,"BoxLoses",true)==1) {set(val,ini_GetValue(Data));Player[playerid][BoxLoses] = strval(val);}
			if(strcomp(key,"Warns",true)==1) {set(val,ini_GetValue(Data));Player[playerid][Warns] = strval(val);}
			if(strcomp(key,"FuelCans",true)==1) {set(val,ini_GetValue(Data));Player[playerid][FuelCans] = strval(val);}
			if(strcomp(key,"Hotwires",true)==1) {set(val,ini_GetValue(Data));Player[playerid][Hotwires] = strval(val);}
			if(strcomp(key,"FoodEaten",true)==1) {set(val,ini_GetValue(Data));Player[playerid][FoodEaten] = strval(val);}
			if(strcomp(key,"GotJop",true)==1) {set(val,ini_GetValue(Data));Player[playerid][GotJob] = strval(val);}
			if(strcomp(key,"CopSkill",true)==1) {set(val,ini_GetValue(Data));Player[playerid][CopSkill] = strval(val);}
			if(strcomp(key,"CopLevel",true)==1) {set(val,ini_GetValue(Data));Player[playerid][CopLevel] = strval(val);}
			if(strcomp(key,"HotwiresCop",true)==1) {set(val,ini_GetValue(Data));Player[playerid][HotwiresCop] = strval(val);}
			if(strcomp(key,"TotalPlaytime",true)==1) {set(val,ini_GetValue(Data));Player[playerid][TotalPlaytime] = strval(val);}
			if(strcomp(key,"Bounty",true)==1) {set(val,ini_GetValue(Data));Player[playerid][Bounty] = strval(val);}
			if(strcomp(key,"Heals",true)==1) {set(val,ini_GetValue(Data));Player[playerid][Heals] = strval(val);}
			if(strcomp(key,"MinesPlanted",true)==1) {set(val,ini_GetValue(Data));Player[playerid][MinesPlanted] = strval(val);}
			if(strcomp(key,"CashBox",true)==1) {set(val,ini_GetValue(Data));Player[playerid][CashBox] = strval(val);}
			if(strcomp(key,"CashBoxCop",true)==1) {set(val,ini_GetValue(Data));Player[playerid][CashBoxCop] = strval(val);}
			if(strcomp(key,"CanUseBoats",true)==1) {set(val,ini_GetValue(Data));Player[playerid][CanUseBoats] = strval(val);}
			if(strcomp(key,"CanUseMotoBikes",true)==1) {set(val,ini_GetValue(Data));Player[playerid][CanUseMotoBikes] = strval(val);}
			if(strcomp(key,"CanUseFlying",true)==1) {set(val,ini_GetValue(Data));Player[playerid][CanUseFlying] = strval(val);}
			if(strcomp(key,"TaxiRides",true)==1) {set(val,ini_GetValue(Data));Player[playerid][TaxiRides] = strval(val);}
			if(strcomp(key,"TaxiPassengers",true)==1) {set(val,ini_GetValue(Data));Player[playerid][TaxiPassengers] = strval(val);}
			if(strcomp(key,"LottoNumber",true)==1) {set(val,ini_GetValue(Data));Player[playerid][LottoNumber] = strval(val);}
			if(strcomp(key,"SpawnPlace",true)==1) {set(val,ini_GetValue(Data));Player[playerid][SpawnPlace] = strval(val);}
			if(strcomp(key,"FightingStyle",true)==1) {set(val,ini_GetValue(Data));Player[playerid][FightingStyle] = strval(val);}
			if(strcomp(key,"X",true)==1) {set(val,ini_GetValue(Data));PlayerSpawn[playerid][Coord_X] = floatstr(val);}
			if(strcomp(key,"Y",true)==1) {set(val,ini_GetValue(Data));PlayerSpawn[playerid][Coord_Y] = floatstr(val);}
			if(strcomp(key,"Z",true)==1) {set(val,ini_GetValue(Data));PlayerSpawn[playerid][Coord_Z] = floatstr(val);}
			if(strcomp(key,"WeaponsData",true)==1) {set(val,ini_GetValue(Data));SetWeaponsFromDBString(playerid,val);}
			if(strcomp(key,"WeaponsStatKills",true)==1) {set(val,ini_GetValue(Data));SetWeaponsKillStatFromDBString(playerid,val);}
			if(strcomp(key,"WeaponsStatDeaths",true)==1) {set(val,ini_GetValue(Data));SetWeaponsDeathStatFromDBString(playerid,val);}
			if(strcomp(key,"Packets",true)==1) {set(val,ini_GetValue(Data));SetPacketsFromDBString(playerid,val);}
			if(strcomp(key,"Horseshoes",true)==1) {set(val,ini_GetValue(Data));SetHorseshoesFromDBString(playerid,val);}
			if(strcomp(key,"Oysters",true)==1) {set(val,ini_GetValue(Data));SetOystersFromDBString(playerid,val);}
			set(val,nullstr);
			set(key,nullstr);
		}
		fclose(UserFile);
	}
	#else
		Player[playerid][Level] = strval(db_Get(dinifilename,"Level"));
		Player[playerid][XP] = strval(db_Get(dinifilename,"XP"));
		oSetPlayerMoney(playerid,strval(db_Get(dinifilename,"Money")));
		Player[playerid][BankMoney] = strval(db_Get(dinifilename,"BankMoney"));
		Player[playerid][Deaths] = strval(db_Get(dinifilename,"Deaths"));
		Player[playerid][Suicides] = strval(db_Get(dinifilename,"Suicides"));
		Player[playerid][Kills] = strval(db_Get(dinifilename,"Kills"));
		Player[playerid][JailTime] = strval(db_Get(dinifilename,"JailTime"));
		Player[playerid][MuteTime] = strval(db_Get(dinifilename,"MuteTime"));
		Player[playerid][Drinks] = strval(db_Get(dinifilename,"Drinks"));
		Player[playerid][DrinksUsed] = strval(db_Get(dinifilename,"DrinksUsed"));
		Player[playerid][ClothesBuyed] = strval(db_Get(dinifilename,"ClothesBuyed"));
		Player[playerid][GangMateKills] = strval(db_Get(dinifilename,"GangMateKills"));
		Player[playerid][RacesWon] = strval(db_Get(dinifilename,"RacesWon"));
		Player[playerid][DMsWon] = strval(db_Get(dinifilename,"DMsWon"));
		Player[playerid][HaysWon] = strval(db_Get(dinifilename,"HaysWon"));
		Player[playerid][SpawnKills] = strval(db_Get(dinifilename,"SpawnKills"));
		Player[playerid][SpawnDeaths] = strval(db_Get(dinifilename,"SpawnDeaths"));
		Player[playerid][RacesPlayed] = strval(db_Get(dinifilename,"RacesPlayed"));
		Player[playerid][DMsPlayed] = strval(db_Get(dinifilename,"DMsPlayed"));
		Player[playerid][HaysPlayed] = strval(db_Get(dinifilename,"HaysPlayed"));
		Player[playerid][SkyDives] = strval(db_Get(dinifilename,"SkyDives"));
		Player[playerid][GotBusted] = strval(db_Get(dinifilename,"GotBusted"));
		Player[playerid][WantedLevel] = strval(db_Get(dinifilename,"WantedLevel"));
		Player[playerid][Crimes] = strval(db_Get(dinifilename,"Crimes"));
		Player[playerid][WasAtHouse] = strval(db_Get(dinifilename,"WasAtHouse"));
		Player[playerid][BoxingSkill] = strval(db_Get(dinifilename,"BoxingSkill"));
		Player[playerid][BoxWins] = strval(db_Get(dinifilename,"BoxWins"));
		Player[playerid][BoxLoses] = strval(db_Get(dinifilename,"BoxLoses"));
		Player[playerid][Warns] = strval(db_Get(dinifilename,"Warns"));
		Player[playerid][FuelCans] = strval(db_Get(dinifilename,"FuelCans"));
		Player[playerid][Hotwires] = strval(db_Get(dinifilename,"Hotwires"));
		Player[playerid][FoodEaten] = strval(db_Get(dinifilename,"FoodEaten"));
		Player[playerid][GotJob] = strval(db_Get(dinifilename,"GotJob"));
		Player[playerid][CopSkill] = strval(db_Get(dinifilename,"CopSkill"));
		Player[playerid][CopLevel] = strval(db_Get(dinifilename,"CopLevel"));
		Player[playerid][HotwiresCop] = strval(db_Get(dinifilename,"HotwiresCop"));
		Player[playerid][TotalPlaytime] = strval(db_Get(dinifilename,"TotalPlaytime"));
		Player[playerid][Bounty] = strval(db_Get(dinifilename,"Bounty"));
		Player[playerid][Heals] = strval(db_Get(dinifilename,"Heals"));
		Player[playerid][MinesPlanted] = strval(db_Get(dinifilename,"MinesPlanted"));
		Player[playerid][CashBox] = strval(db_Get(dinifilename,"CashBox"));
		Player[playerid][CashBoxCop] = strval(db_Get(dinifilename,"CashBoxCop"));
		Player[playerid][CanUseBoats] = strval(db_Get(dinifilename,"CanUseBoats"));
		Player[playerid][CanUseMotoBikes] = strval(db_Get(dinifilename,"CanUseMotoBikes"));
		Player[playerid][CanUseFlying] = strval(db_Get(dinifilename,"CanUseFlying"));
		Player[playerid][TaxiRides] = strval(db_Get(dinifilename,"TaxiRides"));
		Player[playerid][TaxiPassengers] = strval(db_Get(dinifilename,"TaxiPassengers"));
		Player[playerid][LottoNumber] = strval(db_Get(dinifilename,"LottoNumber"));
		Player[playerid][SpawnPlace] = strval(db_Get(dinifilename,"SpawnPlace"));
		Player[playerid][FightingStyle] = strval(db_Get(dinifilename,"FightingStyle"));
		PlayerSpawn[playerid][Coord_X] = floatstr(db_Get(dinifilename,"X"));
		PlayerSpawn[playerid][Coord_Y] = floatstr(db_Get(dinifilename,"Y"));
		PlayerSpawn[playerid][Coord_Z] = floatstr(db_Get(dinifilename,"Z"));
		set(gangname,db_Get(dinifilename,"Gang"));
		set(gangtag,db_Get(dinifilename,"GangTag"));
		SetWeaponsFromDBString(playerid,db_Get(dinifilename,"WeaponsData"));
		SetWeaponsKillStatFromDBString(playerid,db_Get(dinifilename,"WeaponsStatKills"));
		SetWeaponsDeathStatFromDBString(playerid,db_Get(dinifilename,"WeaponsStatDeaths"));
		SetPacketsFromDBString(playerid,db_Get(dinifilename,"Packets"));
		SetHorseshoesFromDBString(playerid,db_Get(dinifilename,"Horseshoes"));
		SetOystersFromDBString(playerid,db_Get(dinifilename,"Oysters"));
	#endif

	if (strlen(gangname) > 0)
	{
		set(PlayerGangName[playerid],gangname);
		set(PlayerGangTag[playerid],gangtag);
		PlayerGangid[playerid] = GangLoad(gangname);
		if (PlayerGangid[playerid] == 0)
		{
			new tmpname[MAX_NAME];
			set(PlayerGangName[playerid],tmpname);
			set(PlayerGangTag[playerid],"No GangTag");
			PlayerGangName[playerid][0] = 0;
			PlayerGangid[playerid] = 0;
		}
	}
	else
	{
		PlayerGangid[playerid] = 0; // no gang
	}
	SetPlayerScore(playerid,Player[playerid][XP]);
	SetWantedLevel(playerid,Player[playerid][WantedLevel]);
	GangMemberLogin(playerid,PlayerGangid[playerid]);
	oSetPlayerHealth(playerid,GetMaxHealth(playerid));
	if (Player[playerid][GotJob] == JOB_COP){
		if (Player[playerid][CopLevel] == 0){
			SetPlayerSkin(playerid,SKIN_COP);
			SystemMsg(playerid,COLOUR_POLICE,gettext(438));
		} else if (Player[playerid][CopLevel] == 1){
			SetPlayerSkin(playerid,SKIN_GOODCOP);
			SystemMsg(playerid,COLOUR_POLICE,gettext(439));
		} else if (Player[playerid][CopLevel] == 2){
			SetPlayerSkin(playerid,SKIN_STREETPATROL);
			SystemMsg(playerid,COLOUR_POLICE,gettext(440));
		} else if (Player[playerid][CopLevel] == 3){
			SetPlayerSkin(playerid,SKIN_POLICEOFFICER);
			SystemMsg(playerid,COLOUR_POLICE,gettext(441));
		} else if (Player[playerid][CopLevel] == 4){
			SetPlayerSkin(playerid,SKIN_SWATMEMBER);
			SystemMsg(playerid,COLOUR_POLICE,gettext(442));
		} else if (Player[playerid][CopLevel] == 5){
			SetPlayerSkin(playerid,SKIN_FBIAGENT);
			SystemMsg(playerid,COLOUR_POLICE,gettext(443));
		}
		Player[playerid][Crimes] = 0;
		Player[playerid][WantedLevel] = 0;
	}
	if (Player[playerid][GotJob] == JOB_MEDIC){
		SetPlayerSkin(playerid,SKIN_MEDIC);
		SystemMsg(playerid,COLOUR_INFO,gettext(444));
	}
	if (Player[playerid][GotJob] == JOB_TAXIDRIVER){
		SystemMsg(playerid,COLOUR_INFO,gettext(1384));
	}
	ResetPlayerWeapons(playerid);
	GivePlayerOwnedWeapons(playerid);
	SetSpecialColorForPlayer(playerid);
	Debug("player.inc > player_load_db_dini - Stop");
}

public JailPlayer(playerid,time,reason[])
{
	Debug("player.inc > JailPlayer - Start");
	ResetQuest(playerid);
	ResetPlayerWeapons(playerid);
	SetPlayerInterior(playerid,INTERIOR_JAIL);
	oSetPlayerPos(playerid,265.1273,77.6823,1001.0391);
	ShowTextForPlayer(playerid, gettext(445), 5000, 3);
	new string[256];
	format(string,sizeof(string),gettext(446),ConvertSeconds(time),reason);
	SystemMsg(playerid,COLOUR_INFO,string);
	//Player[playerid][JailTime] = Player[playerid][JailTime] + time;
	Player[playerid][JailTime] = time;
	SetPlayerVirtualWorld(playerid,PrivateWorld(playerid));
	new logstring[256];
	format(logstring, sizeof (logstring), "player: %d:  %s: jailed. total time: %s",playerid,oGetPlayerName(playerid),ConvertSeconds(Player[playerid][JailTime]));
	WriteLog(logstring);
	Debug("player.inc > JailPlayer - Stop");
}

public UnJailPlayer(playerid)
{
	Debug("player.inc > UnJailPlayer - Start");
	SetPlayerInterior(playerid,INTERIOR_DEFAULT);
	new u = random(UnjailPlacesCount);
	SetPlayerPos(playerid, UnjailPlace[u][Coord_X], UnjailPlace[u][Coord_Y], UnjailPlace[u][Coord_Z]);
	SetCameraBehindPlayer(playerid);
	ShowTextForPlayer(playerid,gettext(447), 5000, 3);
	Player[playerid][JailTime] = 0;
	SetPlayerVirtualWorld(playerid,WORLD_DEFAULT);
	new logstring[256];
	format(logstring, sizeof (logstring), "player: %d:  %s: unjailed",playerid,oGetPlayerName(playerid));
	WriteLog(logstring);
	Debug("player.inc > UnJailPlayer - Stop");
}

forward SetWantedLevel(playerid,level);
public SetWantedLevel(playerid,level){
Debug("player.inc > SetWantedLevel - Start");
Player[playerid][WantedLevel]=level;
SetPlayerWantedLevel(playerid,level);
CheckWantedLevel(playerid);
Debug("player.inc > SetWantedLevel - Stop");
return 1;
}

forward CheckWantedLevel(playerid);
public CheckWantedLevel(playerid){
Debug("player.inc > CheckWantedLevel - Start");
if (GetPlayerWantedLevel(playerid) < 0) SetPlayerWantedLevel(playerid,0);
if (GetPlayerWantedLevel(playerid) > 6) SetPlayerWantedLevel(playerid,6);
if (Player[playerid][WantedLevel] < 0) Player[playerid][WantedLevel]=0;
if (Player[playerid][WantedLevel] > 6) Player[playerid][WantedLevel]=6;
SetSpecialColorForPlayer(playerid);
Debug("player.inc > CheckWantedLevel - Stop");
}

forward CheckCurrentCrimes(playerid);

public CheckCurrentCrimes(playerid){
Debug("player.inc > CheckCurrentCrimes - Start");
new crimes = Player[playerid][Crimes];
if (crimes < 0) {Player[playerid][Crimes] = 0; SetWantedLevel(playerid,0); new string[MAX_STRING]; format(string,MAX_STRING,gettext(1389),oGetPlayerName(playerid)); SendMsgToCops(string);}
if (crimes == 0) {SetWantedLevel(playerid,0);}
if (crimes > 0 && crimes <= 10) { SetWantedLevel(playerid,1);}
else if (crimes> 10 && crimes <= 20) { SetWantedLevel(playerid,2);}
else if (crimes> 20 && crimes <= 30) { SetWantedLevel(playerid,3);}
else if (crimes> 30 && crimes <= 40) { SetWantedLevel(playerid,4);}
else if (crimes> 40 && crimes <= 50) { SetWantedLevel(playerid,5);}
else if (crimes> 50){SetWantedLevel(playerid,6);}
CheckWantedLevel(playerid);
Debug("player.inc > CheckCurrentCrimes - Stop");
return;
}

public SetPlayerCriminal(playerid,reason[],points)
{
	if(IsPlayerConnected(playerid))
	{
		Player[playerid][Crimes] = Player[playerid][Crimes] + points;
		if (points > 0){ //If give points, send messages.
			new string[MAX_STRING];
			format(string, MAX_STRING, gettext(1390),reason,Player[playerid][WantedLevel]);
			SystemMsg(playerid,COLOUR_INFO,string);
			format(string,MAX_STRING,gettext(1391),reason,oGetPlayerName(playerid));
			for (new copid = 0; copid < MAX_PLAYERS_EX; copid++)
			{
				if (IsPlayerConnected(copid)){
					if (Player[copid][GotJob] == JOB_COP){
						PlayCrimeReportForPlayer(copid,playerid,3);
					}
				}
			}
			SendMsgToCops(string);
		}
		CheckCurrentCrimes(playerid);
	}
	return 1;
}

public SetSpecialColorForPlayer(playerid){
	Debug("player.inc > SetSpecialColorForPlayer - Start");
	if (Player[playerid][WantedLevel]>0 && Player[playerid][WantedLevel]<7){
		SetPlayerColor(playerid,0xFF9966AA);
	} else {
		if (PlayerGangid[playerid] == 0){
			SetPlayerColor(playerid,COLOUR_PLAYER);		
		} else {
			SetPlayerColor(playerid,PlayerGangColour(playerid));
		}
	}
	if (Player[playerid][GotJob] == JOB_COP) SetPlayerColor(playerid,COLOUR_BLUE);
	if (Player[playerid][GotJob] == JOB_MEDIC) SetPlayerColor(playerid,COLOUR_MEDIC);
	if ((Player[playerid][GotJob] == JOB_TAXIDRIVER) && (GetPlayerTaxiStatus(playerid) == 0)) SetPlayerColor(playerid,COLOUR_TAXI); //Taxidriver in vehicle :)
	if (PlayerWithCashBox == playerid) SetPlayerColor(playerid,0xFF00FAAA);
	Debug("player.inc > SetSpecialColorForPlayer - Stop");
}

public CheckWarns(playerid){
	if ((Player[playerid][Warns] > Warns_For_Ban) || (Player[playerid][Warns] == Warns_For_Ban)){
	BanPlayer(playerid,99999,gettext(448));
	new logstring[256];
	format(logstring, sizeof (logstring), "player: %d:  %s: BANNED (too much warnings)",playerid,oGetPlayerName(playerid));
	WriteLog(logstring);
	} else {
	SystemMsgFormat(playerid,COLOUR_INFO,gettext(449),(Warns_For_Ban-Player[playerid][Warns]));
	}
}

public ConnectMSG(playerid)
{
	Debug("player.inc > ConnectMSG - Start");
	new string[MAX_STRING];
	format(string,sizeof(string),gettext(450), oGetPlayerName(playerid));
	SendClientMessageToAll(COLOUR_LIGHTGREEN, string);
	WriteLog(string);
	Debug("player.inc > ConnectMSG - Stop");
	return 1;
}

public DisconnectMsg(playerid, reason)
{
	Debug("player.inc > DisconnectMsg - Start");
	new string[MAX_STRING];
	switch(reason)
 	{
 	case 0:
 		{
		format(string,sizeof(string),gettext(451), oGetPlayerName(playerid));
		SendClientMessageToAll(COLOUR_LIGHTGREEN, string);
		WriteLog(string);
		Debug("player.inc > DisconnectMsg - Stop");
		}
	case 1:
		{
		format(string,sizeof(string),gettext(452), oGetPlayerName(playerid));
		SendClientMessageToAll(COLOUR_LIGHTGREEN, string);
		WriteLog(string);
		Debug("player.inc > DisconnectMsg - Stop");
		}
	case 2:
		{
		format(string,sizeof(string),gettext(453), oGetPlayerName(playerid));
		SendClientMessageToAll(COLOUR_LIGHTGREEN, string);
		WriteLog(string);
		Debug("player.inc > DisconnectMsg - Stop");
		}
	}
}

public DeathMSG(playerid, killerid, reason)
{
if (IsPlayerInDuel(playerid) && IsPlayerInDuel(killerid)) return;
Debug("player.inc > DeathMsg - Start");
new string[256];
switch (reason)
{
	case WEAPON_DROWN:
	{
    		format(string, sizeof(string),gettext(454), oGetPlayerName(playerid));
	}
	case WEAPON_COLLISION:
	{
    		format(string, sizeof(string),gettext(455), oGetPlayerName(playerid));
	}
	case 0:
	{
    		format(string, sizeof(string),gettext(456), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_BRASSKNUCKLE:
	{
    		format(string, sizeof(string),gettext(457), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_GOLFCLUB:
	{
    		format(string, sizeof(string),gettext(458), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_NITESTICK:
	{
    		format(string, sizeof(string),gettext(459), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_KNIFE:
	{
    		format(string, sizeof(string),gettext(460), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_BAT:
	{
    		format(string, sizeof(string),gettext(461), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_SHOVEL:
	{
    		format(string, sizeof(string),gettext(462), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_POOLSTICK:
	{
    		format(string, sizeof(string),gettext(463), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_KATANA:
	{
    		format(string, sizeof(string),gettext(464), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_CHAINSAW:
	{
    		format(string, sizeof(string),gettext(465), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_DILDO:
	{
    		format(string, sizeof(string),gettext(466), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_DILDO2:
	{
    		format(string, sizeof(string),gettext(467), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_VIBRATOR:
	{
    		format(string, sizeof(string),gettext(468), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_VIBRATOR2:
	{
    		format(string, sizeof(string),gettext(468), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_FLOWER:
	{
    		format(string, sizeof(string),gettext(469), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_CANE:
	{
    		format(string, sizeof(string),gettext(470), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_GRENADE:
	{
    		format(string, sizeof(string),gettext(471), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_TEARGAS:
	{
    		format(string, sizeof(string),gettext(472), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_MOLTOV:
	{
    		format(string, sizeof(string),gettext(473), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case 19:
	{
    		format(string, sizeof(string),gettext(474), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case 20:
	{
    		format(string, sizeof(string),gettext(474), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case 21:
	{
    		format(string, sizeof(string),gettext(475), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_COLT45:
	{
    		format(string, sizeof(string),gettext(476), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_SILENCED:
	{
    		format(string, sizeof(string),gettext(477), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_DEAGLE:
	{
    		format(string, sizeof(string),gettext(478), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_SHOTGUN:
	{
    		format(string, sizeof(string),gettext(479), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_SAWEDOFF:
	{
    		format(string, sizeof(string),gettext(480), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_SHOTGSPA:
	{
    		format(string, sizeof(string),gettext(481), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_UZI:
	{
    		format(string, sizeof(string),gettext(482), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_MP5:
	{
    		format(string, sizeof(string),gettext(483), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_AK47:
	{
    		format(string, sizeof(string),gettext(484), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_M4:
	{
    		format(string, sizeof(string),gettext(485), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_TEC9:
	{
    		format(string, sizeof(string),gettext(486), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_RIFLE:
	{
    		format(string, sizeof(string),gettext(487), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_SNIPER:
	{
    		format(string, sizeof(string),gettext(488), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_ROCKETLAUNCHER:
	{
    		format(string, sizeof(string),gettext(489), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_HEATSEEKER:
	{
    		format(string, sizeof(string),gettext(490), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_FLAMETHROWER:
	{
    		format(string, sizeof(string),gettext(491), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_MINIGUN:
	{
    		format(string, sizeof(string),gettext(492), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_SATCHEL:
	{
    		format(string, sizeof(string),gettext(493), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_SPRAYCAN:
	{
   		format(string, sizeof(string),gettext(494), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_FIREEXTINGUISHER:
	{
    		format(string, sizeof(string),gettext(495), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	case WEAPON_VEHICLE:
	{
    		format(string, sizeof(string),gettext(496), oGetPlayerName(playerid), oGetPlayerName(killerid));
	}
	default:
	{
		format(string, sizeof(string),gettext(455), oGetPlayerName(playerid));
	}
}
SendClientMessageToAll(COLOUR_LIGHTGREEN,string);
Debug("player.inc > DeathMsg - Stop");
}