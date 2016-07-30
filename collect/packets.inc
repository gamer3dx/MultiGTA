//Packets
//Created by MadCat, some parts of code from GTAT

#define PACKETS_TOTALPACKETS 100

new PlayerPackets[MAX_PLAYERS][PACKETS_TOTALPACKETS];

new Packets_Enabled = 0;
new Reward_For_Packet = 250;

new TimerPackets;

forward Packets_OnGameModeInit();
forward PacketsTimer();

new PacketsLocations[PACKETS_TOTALPACKETS][CoordInfo] = {
	{2337.9263,-1136.3528,1054.8242},
	{242.5820,1041.5068,1088.3102},
	{-841.8667,530.2477,1357.4926},
	{2744.0874,-2453.8191,13.8623},
	{2646.0288,-1989.6432,13.5535},
	{1858.9302,-2110.2461,15.1671},
	{1742.5781,-2333.8772,13.5331},
	{1125.1849,-2049.3530,69.0078},
	{697.9554,-1601.7135,15.0492},
	{420.4923,-1504.0504,31.2363},
	{390.8416,-2058.3108,10.7173},
	{247.7118,-1267.9310,73.9530},
	{731.3740,-1276.1980,17.6484},
	{1292.2992,-769.5791,95.9632},
	{1733.4609,-1030.0360,23.9757},
	{1609.5057,-1123.4600,58.2538},
	{1478.2523,-1640.4812,14.1484},
	{1394.8328,-1460.7218,11.6406},
	{1870.3295,-1363.2775,19.1406},
	{2198.1013,-1102.6306,32.4922},
	{2766.7424,-1190.9443,69.4084},
	{2315.3687,-1437.7906,21.1682},
	{2253.2832,-1696.6903,13.7534},
	{1258.7711,-1505.1643,10.0540},
	{934.5101,-941.4482,42.7395},
	{2068.4753,-1556.1913,13.4277},
	{2810.6492,-1092.9696,30.8828},
	{2964.7507,-1877.7195,8.7813},
	{-1600.5095,-481.7027,22.6838},
	{-2063.1270,-859.6107,32.1719},
	{-2710.3137,-507.8643,5.9521},
	{-2401.7839,-267.8511,39.7981},
	{-2658.5566,-28.4018,4.9375},
	{-2533.3215,307.3026,35.1172},
	{-2129.8108,185.4952,42.2500},
	{-1645.9750,258.1824,1.3984},
	{-1700.1399,434.5136,10.9475},
	{-1803.3064,559.6868,35.1641},
	{-2019.6440,958.1732,46.0310},
	{-1984.0607,1110.3228,53.1427},
	{-2888.3386,814.1183,37.6175},
	{-2819.2339,1115.9329,26.8856},
	{-2947.0918,499.1192,2.4297},
	{-1556.5861,1333.7538,4.4428},
	{-2349.6248,1529.5477,26.0469},
	{-2651.3789,1459.5416,46.6094},
	{-2484.5708,1210.5939,38.7891},
	{-2705.8862,376.0865,4.9684},
	{-1162.0397,375.0684,1.9609},
	{-1643.6598,1205.9419,32.9125},
	{-2435.3887,519.8341,30.3634},
	{-1991.6627,-379.7937,25.7109},
	{-2535.2622,-690.2217,139.3203},
	{1544.5028,676.1615,11.3281},
	{2774.9043,901.3531,10.8984},
	{2127.9685,1144.2124,13.5095},
	{1914.2024,944.9398,10.8203},
	{2587.3857,1058.5851,10.8203},
	{2441.9243,1388.6166,10.9063},
	{2597.2046,1887.4022,11.0312},
	{2215.7375,2000.9509,10.8203},
	{2024.7529,2133.8464,13.8421},
	{1592.4811,1816.2223,10.8203},
	{1033.2139,2304.0969,10.8857},
	{1488.1289,2773.7478,10.8203},
	{2138.2432,2825.1833,10.8203},
	{2000.6481,1522.3734,17.0682},
	{2829.3032,2203.5325,11.0234},
	{2114.6143,2500.3572,11.0781},
	{1333.9301,2061.4570,10.8203},
	{1098.1569,1677.2511,6.6328},
	{1286.4943,1343.1189,10.8203},
	{2359.9209,549.5664,4.8645},
	{2403.1541,1826.1570,11.6563},
	{1918.5594,1938.9857,10.5900},
	{2190.6819,2411.9294,73.0313},
	{1477.4507,2435.3572,9.1049},
	{2954.2415,2119.4272,4.2174},
	{375.4927,850.9958,29.6698},
	{-348.6404,1611.9764,75.7623},
	{-651.1451,2153.0647,60.3828},
	{-805.7498,2239.2781,41.8364},
	{-2090.5981,2313.4663,25.9141},
	{-1435.6559,-964.7205,201.0197},
	{-2378.0996,-2192.5647,33.3326},
	{-6.0410,-2650.9136,40.8693},
	{-54.1274,-414.8699,2.7256},
	{2126.4268,93.2270,35.4759},
	{872.4697,-20.5013,69.0540},
	{-664.7361,879.5297,2.0000},
	{1389.7815,471.9503,20.0927},
	{536.3228,-196.3504,20.1187},
	{834.9543,-615.8219,16.3359},
	{24.3124,-1129.2719,1.6227},
	{2103.2117,-103.1112,2.26590},
	{-1673.4110,2598.2278,81.3594},
	{-662.2859,2316.1340,138.7995},
	{-326.8595,2215.2112,43.4709},
	{245.7820,1862.6420,18.1188},
	{-735.8580,-98.4437,71.9028}
};

public Packets_OnGameModeInit(){
	PacketsConfig();
	if (Packets_Enabled == 1){
		for (new id=0;id<PACKETS_TOTALPACKETS;id++){
			CreateStreamPickup(1276,1,PacketsLocations[id][Coord_X],PacketsLocations[id][Coord_Y],PacketsLocations[id][Coord_Z]);
		}
		TimerPackets = SetTimer("PacketsTimer",1000,1);
		WriteLog("Packets system loaded");
	}
	return 0;
}

PacketsConfig()
{
	if (!db_Exists(ConfigDB)) db_Create(ConfigDB);
	new temp[MAX_STRING];

	set(temp,db_Get(ConfigDB,"Packets_Enabled"));
	if (strlen(temp) > 0) Packets_Enabled = strval(temp); 
	else { valstr(temp,Packets_Enabled); db_Set(ConfigDB,"Packets_Enabled",temp); }
	set(temp,nullstr);

	set(temp,db_Get(ConfigDB,"Reward_For_Packet"));
	if (strlen(temp) > 0) Reward_For_Packet = strval(temp); 
	else { valstr(temp,Reward_For_Packet); db_Set(ConfigDB,"Reward_For_Packet",temp); }
}

public PacketsTimer(){
	for (new playerid=0; playerid<MAX_PLAYERS_EX; playerid++){
	    if (IsPlayerConnected(playerid)){
		if (PlayerQuest[playerid] == QUEST_NOQUEST){
			for (new packetid=0; packetid<PACKETS_TOTALPACKETS; packetid++){
				if (PlayerPackets[playerid][packetid] == 0){
					if (IsPlayerInRangeOfPoint(playerid,3.0,PacketsLocations[packetid][Coord_X],PacketsLocations[packetid][Coord_Y],PacketsLocations[packetid][Coord_Z])){
			        		PlayerPicksUpPacket(playerid,packetid);
					}
			    	}
			}
		}
	    }
	}
}

PlayerPicksUpPacket(playerid,packetid){
    	PlayerPackets[playerid][packetid] = 1;
	new pickedpickups = CountPickedPackets(playerid);
	oGivePlayerMoney(playerid,(Reward_For_Packet*pickedpickups),0);
	PlayerPlaySoundOnPlayer(playerid,1058);
	new text[MAX_STRING];
	format(text,MAX_STRING,gettext(1771),pickedpickups,PACKETS_TOTALPACKETS,(Reward_For_Packet*pickedpickups));
    	ShowTextForPlayer(playerid,text,5000,5);
}

SetPacketsFromDBString(playerid,dbstring[])
{
	new idx;
	for (new i=0;i<PACKETS_TOTALPACKETS;i++)
	{
		new packets;
		packets = strval(strcharsplit(dbstring,idx,strchar("/")));
		PlayerPackets[playerid][i] = packets;
	}
}

CreatePacketDBString(playerid)
{
	new wepstr[MAX_STRING];
	for (new i=0;i<PACKETS_TOTALPACKETS;i++)
	{
		new temp[MAX_STRING];
		valstr(temp,PlayerPackets[playerid][i]); // level
		strins(wepstr, temp, strlen(wepstr));
		wepstr[strlen(wepstr)] =strchar("/");
	}
	return wepstr;
}

CountPickedPackets(playerid)
{
	new picked = 0;
	for (new packetid=0;packetid<PACKETS_TOTALPACKETS;packetid++)
	{
		if (PlayerPackets[playerid][packetid] == 1) picked++;
	}
	return picked;
}