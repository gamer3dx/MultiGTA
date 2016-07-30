//World module.
//Created by Iain Gilbert
//Modder: MadCat

#include "base"
#include "account"
#include "player"
#include "vehicles"
#include "payday"
#include "groundhold"

forward WorldSave();
forward SkinsConfig();
forward GetXYZZoneID(Float:x, Float:y, Float:z);
forward IsXYZInZone(Float:x, Float:y, Float:z, zoneid);
forward IsPlayerInZone(playerid, zoneid);
forward IsPlayerInZoneName(playerid, zonename[]);
forward IsPlayerInCasino(playerid);
forward IsPlayerInStore(playerid);
forward loccmp(Float:x1,Float:y1,Float:z1,Float:x2,Float:y2,Float:z2);
forward Float:GetDistancePlayertoXYZ(playerid,Float:x2,Float:y2,Float:z2);
forward Float:GetDistanceXYZtoXYZ(Float:x1,Float:y1,Float:z1,Float:x2,Float:y2,Float:z2);
forward SyncActiveCP();
forward AddActiveCheckpoint(Float:cpx,Float:cpy,Float:cpz,cpdist,cpsize);
forward AddActiveRaceCheckpoint(type,Float:cpx,Float:cpy,Float:cpz,Float:nextcpx,Float:nextcpy,Float:nextcpz,cpdist,cpsize);
forward RemoveActiveCheckpoint(cpid);
forward RemoveActiveRaceCheckpoint(cpid);
forward SetPlayerActiveCP(playerid,cpid);
forward SetPlayerActiveRaceCP(playerid,cpid);
forward GetPlayerClosestActiveCP(playerid);
forward GetPlayerClosestActiveRaceCP(playerid);
forward SpawnWorld();
//forward update_zones();
forward SavePlayersData();
forward World_OnPlayerConnect(playerid);
forward World_OnPlayerDeath(playerid);
forward World_OnPlayerSpawn(playerid);
forward World_OnGameModeInit();

//new zoneupdates[MAX_PLAYERS];
//new player_zone[MAX_PLAYERS];
//new zoneupdate;

new TimerWorldSave;
new TimerSavePlayers;
new TimerSyncActiveCP;

#define MAX_ACTIVECP 255

#define MAX_ACTIVERACECP 5

#define WorldSaveDelay ONE_SA_DAY // How often we do a world save

new ActiveCPCount;
enum ActiveCPInfo { // active checkpoints are checkpoints around the world that player can see when they get near
	bool:IsActive,
    	Float:CP_X,
    	Float:CP_Y,
    	Float:CP_Z,
    	CP_Dist,  // distance from checkpoint before player can see it
    	CP_Size   // size of checkpoint (bugged)
    
}
new ActiveCheckpoints[MAX_ACTIVECP][ActiveCPInfo];

new ActiveRaceCPCount;
enum ActiveRaceCPInfo { // active checkpoints are checkpoints around the world that player can see when they get near
	bool:IsActive,
	CP_TYPE,
    	Float:CP_X,
    	Float:CP_Y,
    	Float:CP_Z,
	Float:NEXTCP_X,
	Float:NEXTCP_Y,
	Float:NEXTCP_Z,
    	CP_Dist,  // distance from checkpoint before player can see it
    	CP_Size   // size of checkpoint (bugged)
}
new ActiveRaceCheckpoints[MAX_ACTIVERACECP][ActiveRaceCPInfo];

new LastWorldSave = 1;

public WorldSave() // save all
{
    Debug("world.inc > WorldSave - Start");
    if (PlayerCount() > 0)
	{
		new starttime = Now();
		//SystemMsgToRegistered(COLOUR_GREEN,"Saving world.");

		for (new id = 0; id < MAX_PLAYERS_EX; id++)
		{
			if (IsPlayerConnected(id))
			{
				if (IsPlayerRegistered(id))
			   	{
					PlayerSave(id);
	            		}
			}
		}
		if ((Verbose_Save == 1) || (Verbose_Save == -1))
		{
			//SystemMsgToRegistered(COLOUR_GREEN,"Performing verbose save. Please be patient.");
			WeaponSaveAll();
			VehicleDataSaveAll();
			RaceSaveAll();
			DeathmatchSaveAll();
			HaySaveAll();
			HousesSaveAll();
			DrinkBusinessSaveAll();
			MineShopsSaveAll();
			PayNSpraysSaveAll();
			WeaponBusinessSaveAll();
			FuelBusinessSaveAll();
			ClothesBusinessSaveAll();
			SkydivePlacesSaveAll();
			if (Groundhold_Enabled == 1){
				GroundHoldZonesSaveAll();
			}
			
			if (Verbose_Save == -1){ Verbose_Save = 0; db_Set(ConfigDB,"Verbose_Save",ovalstr(Verbose_Save));}
		}
		else
		{
			GangSaveAll();
			RaceSaveScores();
			DeathmatchSaveScores();
			HaySaveScores();
		}

		LastWorldSave = 1;

	    	new logstring[256];
		format(logstring, sizeof (logstring), "World save complete! Time taken: %s.",ConvertSeconds(Now() - starttime));
	    	WriteLog(logstring);
	} else {
	    if (LastWorldSave == 1)
	    {
	        new starttime = Now();
			//SystemMsgToRegistered(COLOUR_GREEN,"Saving world.");

			for (new id = 0; id < MAX_PLAYERS_EX; id++)
			{
				if (IsPlayerConnected(id))
				{
					if (IsPlayerRegistered(id))
				    {
						PlayerSave(id);
		            }
				}
			}

			if ((Verbose_Save == 1) || (Verbose_Save == -1))
			{
				//SystemMsgToRegistered(COLOUR_GREEN,"Performing verbose save. Please be patient.");
				WeaponSaveAll();
				VehicleDataSaveAll();
				RaceSaveAll();
				DeathmatchSaveAll();
				HaySaveAll();
				HousesSaveAll();
				DrinkBusinessSaveAll();
				MineShopsSaveAll();
				PayNSpraysSaveAll();
				WeaponBusinessSaveAll();
				FuelBusinessSaveAll();
				ClothesBusinessSaveAll();
				SkydivePlacesSaveAll();
				if (Groundhold_Enabled == 1){
					GroundHoldZonesSaveAll();
				}

				if (Verbose_Save == -1) { Verbose_Save = 0; db_Set(ConfigDB,"Verbose_Save",ovalstr(Verbose_Save));}
			}
			else
			{
				GangSaveAll();
				RaceSaveScores();
				DeathmatchSaveScores();
				HaySaveScores();
			}

			LastWorldSave = 0;
			
		    	new logstring[256];
			format(logstring, sizeof (logstring), "World save complete! Time taken: %s.",ConvertSeconds(Now() - starttime));
		   	WriteLog(logstring);
			//SystemMsgToRegistered(COLOUR_GREEN,string);
	    }
	}
	Debug("world.inc > WorldSave - Stop");
}

new PlayerSkins_DB[MAX_STRING] = "MultiGTA/Main/Skins.txt";

public SkinsConfig()
{
	Debug("world.inc > SkinsConfig - Start");
	
	if (!db_Exists(DatabaseDB)) db_Create(DatabaseDB);
	new temp[MAX_STRING];
	set(temp,db_Get(DatabaseDB,"PlayerSkins_DB"));
	if (strlen(temp) > 0) set(PlayerSkins_DB,temp); 
	else { set(temp,PlayerSkins_DB); db_Set(DatabaseDB,"PlayerSkins_DB",temp);  }
	set(temp,nullstr);
	if (!db_Exists(PlayerSkins_DB)) return;
	Debug("world.inc > SkinsConfig - Stop");
	return;
}

//----------------------
// Zones
#define MAX_ZONES 367
enum zoneinfo {
	zone_name[MAX_STRING],
    	Float:zone_minx,
    	Float:zone_miny,
    	Float:zone_minz,
    	Float:zone_maxx,
    	Float:zone_maxy,
    	Float:zone_maxz,	
    	gang_owner[MAX_NAME],
    	gang_tag[MAX_NAME],
    	canbecaptured
}

new Float:zones[MAX_ZONES][zoneinfo] = {

// Indoor zones, originally by Iain Gilbert, now by Nitro
{ "Caligula's Palace",            2179.51,  1559.08,   1000.0,  2281.14,  1622.90,   1010.0, "NoOwner","NoTag",0},
{ "The Four Dragons Casino",      1925.15,  968.23,    992.46,  1982.30,  1067.41,   995.521,"NoOwner","NoTag",0},
{ "Redsands Casino",              1113.65,  -13.2672,  1000.67, 1143.28,  12.4690,   1003.06,"NoOwner","NoTag",0},

// Outdoor Zones list by mabako, from gta client files
{ "The Big Ear",                  -410.00,  1403.30,    -3.00,  -137.90,  1681.20,   200.00,"NoOwner","NoTag",1},
{ "Aldea Malvada",               -1372.10,  2498.50,     0.00, -1277.50,  2615.30,   200.00,"NoOwner","NoTag",1},
{ "Angel Pine",                  -2324.90, -2584.20,    -6.10, -1964.20, -2212.10,   200.00,"NoOwner","NoTag",1},
{ "Arco del Oeste",               -901.10,  2221.80,     0.00,  -592.00,  2571.90,   200.00,"NoOwner","NoTag",1},
{ "Avispa Country Club",         -2646.40,  -355.40,     0.00, -2270.00,  -222.50,   200.00,"NoOwner","NoTag",1},
{ "Avispa Country Club",         -2831.80,  -430.20,    -6.10, -2646.40,  -222.50,   200.00,"NoOwner","NoTag",1},
{ "Avispa Country Club",         -2361.50,  -417.10,     0.00, -2270.00,  -355.40,   200.00,"NoOwner","NoTag",1},
{ "Avispa Country Club",         -2667.80,  -302.10,   -28.80, -2646.40,  -262.30,    71.10,"NoOwner","NoTag",1},
{ "Avispa Country Club",         -2470.00,  -355.40,     0.00, -2270.00,  -318.40,    46.10,"NoOwner","NoTag",1},
{ "Avispa Country Club",         -2550.00,  -355.40,     0.00, -2470.00,  -318.40,    39.70,"NoOwner","NoTag",1},
{ "Back o Beyond",               -1166.90, -2641.10,     0.00,  -321.70, -1856.00,   200.00,"NoOwner","NoTag",1},
{ "Battery Point",               -2741.00,  1268.40,    -4.50, -2533.00,  1490.40,   200.00,"NoOwner","NoTag",1},
{ "Bayside",                     -2741.00,  2175.10,     0.00, -2353.10,  2722.70,   200.00,"NoOwner","NoTag",1},
{ "Bayside Marina",              -2353.10,  2275.70,     0.00, -2153.10,  2475.70,   200.00,"NoOwner","NoTag",1},
{ "Beacon Hill",                  -399.60, -1075.50,    -1.40,  -319.00,  -977.50,   198.50,"NoOwner","NoTag",1},
{ "Blackfield",                    964.30,  1203.20,   -89.00,  1197.30,  1403.20,   110.90,"NoOwner","NoTag",1},
{ "Blackfield",                    964.30,  1403.20,   -89.00,  1197.30,  1726.20,   110.90,"NoOwner","NoTag",1},
{ "Blackfield Chapel",            1375.60,   596.30,   -89.00,  1558.00,   823.20,   110.90,"NoOwner","NoTag",1},
{ "Blackfield Chapel",            1325.60,   596.30,   -89.00,  1375.60,   795.00,   110.90,"NoOwner","NoTag",1},
{ "Blackfield Intersection",      1197.30,  1044.60,   -89.00,  1277.00,  1163.30,   110.90,"NoOwner","NoTag",1},
{ "Blackfield Intersection",      1166.50,   795.00,   -89.00,  1375.60,  1044.60,   110.90,"NoOwner","NoTag",1},
{ "Blackfield Intersection",      1277.00,  1044.60,   -89.00,  1315.30,  1087.60,   110.90,"NoOwner","NoTag",1},
{ "Blackfield Intersection",      1375.60,   823.20,   -89.00,  1457.30,   919.40,   110.90,"NoOwner","NoTag",1},
{ "Blueberry",                     104.50,  -220.10,     2.30,   349.60,   152.20,   200.00,"NoOwner","NoTag",1},
{ "Blueberry",                      19.60,  -404.10,     3.80,   349.60,  -220.10,   200.00,"NoOwner","NoTag",1},
{ "Blueberry Acres",              -319.60,  -220.10,     0.00,   104.50,   293.30,   200.00,"NoOwner","NoTag",1},
{ "Caligula's Palace",            2087.30,  1543.20,   -89.00,  2437.30,  1703.20,   110.90,"NoOwner","NoTag",1},
{ "Caligula's Palace",            2137.40,  1703.20,   -89.00,  2437.30,  1783.20,   110.90,"NoOwner","NoTag",1},
{ "Calton Heights",              -2274.10,   744.10,    -6.10, -1982.30,  1358.90,   200.00,"NoOwner","NoTag",1},
{ "Chinatown",                   -2274.10,   578.30,    -7.60, -2078.60,   744.10,   200.00,"NoOwner","NoTag",1},
{ "City Hall",                   -2867.80,   277.40,    -9.10, -2593.40,   458.40,   200.00,"NoOwner","NoTag",1},
{ "Come-A-Lot",                   2087.30,   943.20,   -89.00,  2623.10,  1203.20,   110.90,"NoOwner","NoTag",1},
{ "Commerce",                     1323.90, -1842.20,   -89.00,  1701.90, -1722.20,   110.90,"NoOwner","NoTag",1},
{ "Commerce",                     1323.90, -1722.20,   -89.00,  1440.90, -1577.50,   110.90,"NoOwner","NoTag",1},
{ "Commerce",                     1370.80, -1577.50,   -89.00,  1463.90, -1384.90,   110.90,"NoOwner","NoTag",1},
{ "Commerce",                     1463.90, -1577.50,   -89.00,  1667.90, -1430.80,   110.90,"NoOwner","NoTag",1},
{ "Commerce",                     1583.50, -1722.20,   -89.00,  1758.90, -1577.50,   110.90,"NoOwner","NoTag",1},
{ "Commerce",                     1667.90, -1577.50,   -89.00,  1812.60, -1430.80,   110.90,"NoOwner","NoTag",1},
{ "Conference Center",            1046.10, -1804.20,   -89.00,  1323.90, -1722.20,   110.90,"NoOwner","NoTag",1},
{ "Conference Center",            1073.20, -1842.20,   -89.00,  1323.90, -1804.20,   110.90,"NoOwner","NoTag",1},
{ "Cranberry Station",           -2007.80,    56.30,     0.00, -1922.00,   224.70,   100.00,"NoOwner","NoTag",1},
{ "Creek",                        2749.90,  1937.20,   -89.00,  2921.60,  2669.70,   110.90,"NoOwner","NoTag",1},
{ "Dillimore",                     580.70,  -674.80,    -9.50,   861.00,  -404.70,   200.00,"NoOwner","NoTag",1},
{ "Doherty",                     -2270.00,  -324.10,    -0.00, -1794.90,  -222.50,   200.00,"NoOwner","NoTag",1},
{ "Doherty",                     -2173.00,  -222.50,    -0.00, -1794.90,   265.20,   200.00,"NoOwner","NoTag",1},
{ "Downtown",                    -1982.30,   744.10,    -6.10, -1871.70,  1274.20,   200.00,"NoOwner","NoTag",1},
{ "Downtown",                    -1871.70,  1176.40,    -4.50, -1620.30,  1274.20,   200.00,"NoOwner","NoTag",1},
{ "Downtown",                    -1700.00,   744.20,    -6.10, -1580.00,  1176.50,   200.00,"NoOwner","NoTag",1},
{ "Downtown",                    -1580.00,   744.20,    -6.10, -1499.80,  1025.90,   200.00,"NoOwner","NoTag",1},
{ "Downtown",                    -2078.60,   578.30,    -7.60, -1499.80,   744.20,   200.00,"NoOwner","NoTag",1},
{ "Downtown",                    -1993.20,   265.20,    -9.10, -1794.90,   578.30,   200.00,"NoOwner","NoTag",1},
{ "Downtown Los Santos",          1463.90, -1430.80,   -89.00,  1724.70, -1290.80,   110.90,"NoOwner","NoTag",1},
{ "Downtown Los Santos",          1724.70, -1430.80,   -89.00,  1812.60, -1250.90,   110.90,"NoOwner","NoTag",1},
{ "Downtown Los Santos",          1463.90, -1290.80,   -89.00,  1724.70, -1150.80,   110.90,"NoOwner","NoTag",1},
{ "Downtown Los Santos",          1370.80, -1384.90,   -89.00,  1463.90, -1170.80,   110.90,"NoOwner","NoTag",1},
{ "Downtown Los Santos",          1724.70, -1250.90,   -89.00,  1812.60, -1150.80,   110.90,"NoOwner","NoTag",1},
{ "Downtown Los Santos",          1370.80, -1170.80,   -89.00,  1463.90, -1130.80,   110.90,"NoOwner","NoTag",1},
{ "Downtown Los Santos",          1378.30, -1130.80,   -89.00,  1463.90, -1026.30,   110.90,"NoOwner","NoTag",1},
{ "Downtown Los Santos",          1391.00, -1026.30,   -89.00,  1463.90,  -926.90,   110.90,"NoOwner","NoTag",1},
{ "Downtown Los Santos",          1507.50, -1385.20,   110.90,  1582.50, -1325.30,   335.90,"NoOwner","NoTag",1},
{ "East Beach",                   2632.80, -1852.80,   -89.00,  2959.30, -1668.10,   110.90,"NoOwner","NoTag",1},
{ "East Beach",                   2632.80, -1668.10,   -89.00,  2747.70, -1393.40,   110.90,"NoOwner","NoTag",1},
{ "East Beach",                   2747.70, -1668.10,   -89.00,  2959.30, -1498.60,   110.90,"NoOwner","NoTag",1},
{ "East Beach",                   2747.70, -1498.60,   -89.00,  2959.30, -1120.00,   110.90,"NoOwner","NoTag",1},
{ "East Los Santos",              2421.00, -1628.50,   -89.00,  2632.80, -1454.30,   110.90,"NoOwner","NoTag",1},
{ "East Los Santos",              2222.50, -1628.50,   -89.00,  2421.00, -1494.00,   110.90,"NoOwner","NoTag",1},
{ "East Los Santos",              2266.20, -1494.00,   -89.00,  2381.60, -1372.00,   110.90,"NoOwner","NoTag",1},
{ "East Los Santos",              2381.60, -1494.00,   -89.00,  2421.00, -1454.30,   110.90,"NoOwner","NoTag",1},
{ "East Los Santos",              2281.40, -1372.00,   -89.00,  2381.60, -1135.00,   110.90,"NoOwner","NoTag",1},
{ "East Los Santos",              2381.60, -1454.30,   -89.00,  2462.10, -1135.00,   110.90,"NoOwner","NoTag",1},
{ "East Los Santos",              2462.10, -1454.30,   -89.00,  2581.70, -1135.00,   110.90,"NoOwner","NoTag",1},
{ "Easter Basin",                -1794.90,   249.90,    -9.10, -1242.90,   578.30,   200.00,"NoOwner","NoTag",1},
{ "Easter Basin",                -1794.90,   -50.00,    -0.00, -1499.80,   249.90,   200.00,"NoOwner","NoTag",1},
{ "Easter Bay Airport",          -1499.80,   -50.00,    -0.00, -1242.90,   249.90,   200.00,"NoOwner","NoTag",1},
{ "Easter Bay Airport",          -1794.90,  -730.10,    -3.00, -1213.90,   -50.00,   200.00,"NoOwner","NoTag",1},
{ "Easter Bay Airport",          -1213.90,  -730.10,     0.00, -1132.80,   -50.00,   200.00,"NoOwner","NoTag",1},
{ "Easter Bay Airport",          -1242.90,   -50.00,     0.00, -1213.90,   578.30,   200.00,"NoOwner","NoTag",1},
{ "Easter Bay Airport",          -1213.90,   -50.00,    -4.50,  -947.90,   578.30,   200.00,"NoOwner","NoTag",1},
{ "Easter Bay Airport",          -1315.40,  -405.30,    15.40, -1264.40,  -209.50,    25.40,"NoOwner","NoTag",1},
{ "Easter Bay Airport",          -1354.30,  -287.30,    15.40, -1315.40,  -209.50,    25.40,"NoOwner","NoTag",1},
{ "Easter Bay Airport",          -1490.30,  -209.50,    15.40, -1264.40,  -148.30,    25.40,"NoOwner","NoTag",1},
{ "Easter Bay Chemicals",        -1132.80,  -768.00,     0.00,  -956.40,  -578.10,   200.00,"NoOwner","NoTag",1},
{ "Easter Bay Chemicals",        -1132.80,  -787.30,     0.00,  -956.40,  -768.00,   200.00,"NoOwner","NoTag",1},
{ "El Castillo del Diablo",       -464.50,  2217.60,     0.00,  -208.50,  2580.30,   200.00,"NoOwner","NoTag",1},
{ "El Castillo del Diablo",       -208.50,  2123.00,    -7.60,   114.00,  2337.10,   200.00,"NoOwner","NoTag",1},
{ "El Castillo del Diablo",       -208.50,  2337.10,     0.00,     8.40,  2487.10,   200.00,"NoOwner","NoTag",1},
{ "El Corona",                    1812.60, -2179.20,   -89.00,  1970.60, -1852.80,   110.90,"NoOwner","NoTag",1},
{ "El Corona",                    1692.60, -2179.20,   -89.00,  1812.60, -1842.20,   110.90,"NoOwner","NoTag",1},
{ "El Quebrados",                -1645.20,  2498.50,     0.00, -1372.10,  2777.80,   200.00,"NoOwner","NoTag",1},
{ "Esplanade East",              -1620.30,  1176.50,    -4.50, -1580.00,  1274.20,   200.00,"NoOwner","NoTag",1},
{ "Esplanade East",              -1580.00,  1025.90,    -6.10, -1499.80,  1274.20,   200.00,"NoOwner","NoTag",1},
{ "Esplanade East",              -1499.80,   578.30,   -79.60, -1339.80,  1274.20,    20.30,"NoOwner","NoTag",1},
{ "Esplanade North",             -2533.00,  1358.90,    -4.50, -1996.60,  1501.20,   200.00,"NoOwner","NoTag",1},
{ "Esplanade North",             -1996.60,  1358.90,    -4.50, -1524.20,  1592.50,   200.00,"NoOwner","NoTag",1},
{ "Esplanade North",             -1982.30,  1274.20,    -4.50, -1524.20,  1358.90,   200.00,"NoOwner","NoTag",1},
{ "Fallen Tree",                  -792.20,  -698.50,    -5.30,  -452.40,  -380.00,   200.00,"NoOwner","NoTag",1},
{ "Fallow Bridge",                 434.30,   366.50,     0.00,   603.00,   555.60,   200.00,"NoOwner","NoTag",1},
{ "Fern Ridge",                    508.10,  -139.20,     0.00,  1306.60,   119.50,   200.00,"NoOwner","NoTag",1},
{ "Financial",                   -1871.70,   744.10,    -6.10, -1701.30,  1176.40,   300.00,"NoOwner","NoTag",1},
{ "Fisher's Lagoon",              1916.90,  -233.30,  -100.00,  2131.70,    13.80,   200.00,"NoOwner","NoTag",1},
{ "Flint Intersection",           -187.70, -1596.70,   -89.00,    17.00, -1276.60,   110.90,"NoOwner","NoTag",1},
{ "Flint Range",                  -594.10, -1648.50,     0.00,  -187.70, -1276.60,   200.00,"NoOwner","NoTag",1},
{ "Fort Carson",                  -376.20,   826.30,    -3.00,   123.70,  1220.40,   200.00,"NoOwner","NoTag",1},
{ "Foster Valley",               -2270.00,  -430.20,    -0.00, -2178.60,  -324.10,   200.00,"NoOwner","NoTag",1},
{ "Foster Valley",               -2178.60,  -599.80,    -0.00, -1794.90,  -324.10,   200.00,"NoOwner","NoTag",1},
{ "Foster Valley",               -2178.60, -1115.50,     0.00, -1794.90,  -599.80,   200.00,"NoOwner","NoTag",1},
{ "Foster Valley",               -2178.60, -1250.90,     0.00, -1794.90, -1115.50,   200.00,"NoOwner","NoTag",1},
{ "Frederick Bridge",             2759.20,   296.50,     0.00,  2774.20,   594.70,   200.00,"NoOwner","NoTag",1},
{ "Gant Bridge",                 -2741.40,  1659.60,    -6.10, -2616.40,  2175.10,   200.00,"NoOwner","NoTag",1},
{ "Gant Bridge",                 -2741.00,  1490.40,    -6.10, -2616.40,  1659.60,   200.00,"NoOwner","NoTag",1},
{ "Ganton",                       2222.50, -1852.80,   -89.00,  2632.80, -1722.30,   110.90,"NoOwner","NoTag",1},
{ "Ganton",                       2222.50, -1722.30,   -89.00,  2632.80, -1628.50,   110.90,"NoOwner","NoTag",1},
{ "Garcia",                      -2411.20,  -222.50,    -0.00, -2173.00,   265.20,   200.00,"NoOwner","NoTag",1},
{ "Garcia",                      -2395.10,  -222.50,    -5.30, -2354.00,  -204.70,   200.00,"NoOwner","NoTag",1},
{ "Garver Bridge",               -1339.80,   828.10,   -89.00, -1213.90,  1057.00,   110.90,"NoOwner","NoTag",1},
{ "Garver Bridge",               -1213.90,   950.00,   -89.00, -1087.90,  1178.90,   110.90,"NoOwner","NoTag",1},
{ "Garver Bridge",               -1499.80,   696.40,  -179.60, -1339.80,   925.30,    20.30,"NoOwner","NoTag",1},
{ "Glen Park",                    1812.60, -1449.60,   -89.00,  1996.90, -1350.70,   110.90,"NoOwner","NoTag",1},
{ "Glen Park",                    1812.60, -1100.80,   -89.00,  1994.30,  -973.30,   110.90,"NoOwner","NoTag",1},
{ "Glen Park",                    1812.60, -1350.70,   -89.00,  2056.80, -1100.80,   110.90,"NoOwner","NoTag",1},
{ "Green Palms",                   176.50,  1305.40,    -3.00,   338.60,  1520.70,   200.00,"NoOwner","NoTag",1},
{ "Greenglass College",            964.30,  1044.60,   -89.00,  1197.30,  1203.20,   110.90,"NoOwner","NoTag",1},
{ "Greenglass College",            964.30,   930.80,   -89.00,  1166.50,  1044.60,   110.90,"NoOwner","NoTag",1},
{ "Hampton Barns",                 603.00,   264.30,     0.00,   761.90,   366.50,   200.00,"NoOwner","NoTag",1},
{ "Hankypanky Point",             2576.90,    62.10,     0.00,  2759.20,   385.50,   200.00,"NoOwner","NoTag",1},
{ "Harry Gold Parkway",           1777.30,   863.20,   -89.00,  1817.30,  2342.80,   110.90,"NoOwner","NoTag",1},
{ "Hashbury",                    -2593.40,  -222.50,    -0.00, -2411.20,    54.70,   200.00,"NoOwner","NoTag",1},
{ "Hilltop Farm",                  967.30,  -450.30,    -3.00,  1176.70,  -217.90,   200.00,"NoOwner","NoTag",1},
{ "Hunter Quarry",                 337.20,   710.80,  -115.20,   860.50,  1031.70,   203.70,"NoOwner","NoTag",1},
{ "Idlewood",                     1812.60, -1852.80,   -89.00,  1971.60, -1742.30,   110.90,"NoOwner","NoTag",1},
{ "Idlewood",                     1812.60, -1742.30,   -89.00,  1951.60, -1602.30,   110.90,"NoOwner","NoTag",1},
{ "Idlewood",                     1951.60, -1742.30,   -89.00,  2124.60, -1602.30,   110.90,"NoOwner","NoTag",1},
{ "Idlewood",                     1812.60, -1602.30,   -89.00,  2124.60, -1449.60,   110.90,"NoOwner","NoTag",1},
{ "Idlewood",                     2124.60, -1742.30,   -89.00,  2222.50, -1494.00,   110.90,"NoOwner","NoTag",1},
{ "Idlewood",                     1971.60, -1852.80,   -89.00,  2222.50, -1742.30,   110.90,"NoOwner","NoTag",1},
{ "Jefferson",                    1996.90, -1449.60,   -89.00,  2056.80, -1350.70,   110.90,"NoOwner","NoTag",1},
{ "Jefferson",                    2124.60, -1494.00,   -89.00,  2266.20, -1449.60,   110.90,"NoOwner","NoTag",1},
{ "Jefferson",                    2056.80, -1372.00,   -89.00,  2281.40, -1210.70,   110.90,"NoOwner","NoTag",1},
{ "Jefferson",                    2056.80, -1210.70,   -89.00,  2185.30, -1126.30,   110.90,"NoOwner","NoTag",1},
{ "Jefferson",                    2185.30, -1210.70,   -89.00,  2281.40, -1154.50,   110.90,"NoOwner","NoTag",1},
{ "Jefferson",                    2056.80, -1449.60,   -89.00,  2266.20, -1372.00,   110.90,"NoOwner","NoTag",1},
{ "Julius Thruway East",          2623.10,   943.20,   -89.00,  2749.90,  1055.90,   110.90,"NoOwner","NoTag",1},
{ "Julius Thruway East",          2685.10,  1055.90,   -89.00,  2749.90,  2626.50,   110.90,"NoOwner","NoTag",1},
{ "Julius Thruway East",          2536.40,  2442.50,   -89.00,  2685.10,  2542.50,   110.90,"NoOwner","NoTag",1},
{ "Julius Thruway East",          2625.10,  2202.70,   -89.00,  2685.10,  2442.50,   110.90,"NoOwner","NoTag",1},
{ "Julius Thruway North",         2498.20,  2542.50,   -89.00,  2685.10,  2626.50,   110.90,"NoOwner","NoTag",1},
{ "Julius Thruway North",         2237.40,  2542.50,   -89.00,  2498.20,  2663.10,   110.90,"NoOwner","NoTag",1},
{ "Julius Thruway North",         2121.40,  2508.20,   -89.00,  2237.40,  2663.10,   110.90,"NoOwner","NoTag",1},
{ "Julius Thruway North",         1938.80,  2508.20,   -89.00,  2121.40,  2624.20,   110.90,"NoOwner","NoTag",1},
{ "Julius Thruway North",         1534.50,  2433.20,   -89.00,  1848.40,  2583.20,   110.90,"NoOwner","NoTag",1},
{ "Julius Thruway North",         1848.40,  2478.40,   -89.00,  1938.80,  2553.40,   110.90,"NoOwner","NoTag",1},
{ "Julius Thruway North",         1704.50,  2342.80,   -89.00,  1848.40,  2433.20,   110.90,"NoOwner","NoTag",1},
{ "Julius Thruway North",         1377.30,  2433.20,   -89.00,  1534.50,  2507.20,   110.90,"NoOwner","NoTag",1},
{ "Julius Thruway South",         1457.30,   823.20,   -89.00,  2377.30,   863.20,   110.90,"NoOwner","NoTag",1},
{ "Julius Thruway South",         2377.30,   788.80,   -89.00,  2537.30,   897.90,   110.90,"NoOwner","NoTag",1},
{ "Julius Thruway West",          1197.30,  1163.30,   -89.00,  1236.60,  2243.20,   110.90,"NoOwner","NoTag",1},
{ "Julius Thruway West",          1236.60,  2142.80,   -89.00,  1297.40,  2243.20,   110.90,"NoOwner","NoTag",1},
{ "Juniper Hill",                -2533.00,   578.30,    -7.60, -2274.10,   968.30,   200.00,"NoOwner","NoTag",1},
{ "Juniper Hollow",              -2533.00,   968.30,    -6.10, -2274.10,  1358.90,   200.00,"NoOwner","NoTag",1},
{ "K.A.C.C. Military Fuels",      2498.20,  2626.50,   -89.00,  2749.90,  2861.50,   110.90,"NoOwner","NoTag",1},
{ "Kincaid Bridge",              -1339.80,   599.20,   -89.00, -1213.90,   828.10,   110.90,"NoOwner","NoTag",1},
{ "Kincaid Bridge",              -1213.90,   721.10,   -89.00, -1087.90,   950.00,   110.90,"NoOwner","NoTag",1},
{ "Kincaid Bridge",              -1087.90,   855.30,   -89.00,  -961.90,   986.20,   110.90,"NoOwner","NoTag",1},
{ "King's",                      -2329.30,   458.40,    -7.60, -1993.20,   578.30,   200.00,"NoOwner","NoTag",1},
{ "King's",                      -2411.20,   265.20,    -9.10, -1993.20,   373.50,   200.00,"NoOwner","NoTag",1},
{ "King's",                      -2253.50,   373.50,    -9.10, -1993.20,   458.40,   200.00,"NoOwner","NoTag",1},
{ "LVA Freight Depot",            1457.30,   863.20,   -89.00,  1777.40,  1143.20,   110.90,"NoOwner","NoTag",1},
{ "LVA Freight Depot",            1375.60,   919.40,   -89.00,  1457.30,  1203.20,   110.90,"NoOwner","NoTag",1},
{ "LVA Freight Depot",            1277.00,  1087.60,   -89.00,  1375.60,  1203.20,   110.90,"NoOwner","NoTag",1},
{ "LVA Freight Depot",            1315.30,  1044.60,   -89.00,  1375.60,  1087.60,   110.90,"NoOwner","NoTag",1},
{ "LVA Freight Depot",            1236.60,  1163.40,   -89.00,  1277.00,  1203.20,   110.90,"NoOwner","NoTag",1},
{ "Las Barrancas",                -926.10,  1398.70,    -3.00,  -719.20,  1634.60,   200.00,"NoOwner","NoTag",1},
{ "Las Brujas",                   -365.10,  2123.00,    -3.00,  -208.50,  2217.60,   200.00,"NoOwner","NoTag",1},
{ "Las Colinas",                  1994.30, -1100.80,   -89.00,  2056.80,  -920.80,   110.90,"NoOwner","NoTag",1},
{ "Las Colinas",                  2056.80, -1126.30,   -89.00,  2126.80,  -920.80,   110.90,"NoOwner","NoTag",1},
{ "Las Colinas",                  2185.30, -1154.50,   -89.00,  2281.40,  -934.40,   110.90,"NoOwner","NoTag",1},
{ "Las Colinas",                  2126.80, -1126.30,   -89.00,  2185.30,  -934.40,   110.90,"NoOwner","NoTag",1},
{ "Las Colinas",                  2747.70, -1120.00,   -89.00,  2959.30,  -945.00,   110.90,"NoOwner","NoTag",1},
{ "Las Colinas",                  2632.70, -1135.00,   -89.00,  2747.70,  -945.00,   110.90,"NoOwner","NoTag",1},
{ "Las Colinas",                  2281.40, -1135.00,   -89.00,  2632.70,  -945.00,   110.90,"NoOwner","NoTag",1},
{ "Las Payasadas",                -354.30,  2580.30,     2.00,  -133.60,  2816.80,   200.00,"NoOwner","NoTag",1},
{ "Las Venturas Airport",         1236.60,  1203.20,   -89.00,  1457.30,  1883.10,   110.90,"NoOwner","NoTag",1},
{ "Las Venturas Airport",         1457.30,  1203.20,   -89.00,  1777.30,  1883.10,   110.90,"NoOwner","NoTag",1},
{ "Las Venturas Airport",         1457.30,  1143.20,   -89.00,  1777.40,  1203.20,   110.90,"NoOwner","NoTag",1},
{ "Las Venturas Airport",         1515.80,  1586.40,   -12.50,  1729.90,  1714.50,    87.50,"NoOwner","NoTag",1},
{ "Last Dime Motel",              1823.00,   596.30,   -89.00,  1997.20,   823.20,   110.90,"NoOwner","NoTag",1},
{ "Leafy Hollow",                -1166.90, -1856.00,     0.00,  -815.60, -1602.00,   200.00,"NoOwner","NoTag",1},
{ "Lil' Probe Inn",                -90.20,  1286.80,    -3.00,   153.80,  1554.10,   200.00,"NoOwner","NoTag",1},
{ "Linden Side",                  2749.90,   943.20,   -89.00,  2923.30,  1198.90,   110.90,"NoOwner","NoTag",1},
{ "Linden Station",               2749.90,  1198.90,   -89.00,  2923.30,  1548.90,   110.90,"NoOwner","NoTag",1},
{ "Linden Station",               2811.20,  1229.50,   -39.50,  2861.20,  1407.50,    60.40,"NoOwner","NoTag",1},
{ "Little Mexico",                1701.90, -1842.20,   -89.00,  1812.60, -1722.20,   110.90,"NoOwner","NoTag",1},
{ "Little Mexico",                1758.90, -1722.20,   -89.00,  1812.60, -1577.50,   110.90,"NoOwner","NoTag",1},
{ "Los Flores",                   2581.70, -1454.30,   -89.00,  2632.80, -1393.40,   110.90,"NoOwner","NoTag",1},
{ "Los Flores",                   2581.70, -1393.40,   -89.00,  2747.70, -1135.00,   110.90,"NoOwner","NoTag",1},
{ "Los Santos International",     1249.60, -2394.30,   -89.00,  1852.00, -2179.20,   110.90,"NoOwner","NoTag",1},
{ "Los Santos International",     1852.00, -2394.30,   -89.00,  2089.00, -2179.20,   110.90,"NoOwner","NoTag",1},
{ "Los Santos International",     1382.70, -2730.80,   -89.00,  2201.80, -2394.30,   110.90,"NoOwner","NoTag",1},
{ "Los Santos International",     1974.60, -2394.30,   -39.00,  2089.00, -2256.50,    60.90,"NoOwner","NoTag",1},
{ "Los Santos International",     1400.90, -2669.20,   -39.00,  2189.80, -2597.20,    60.90,"NoOwner","NoTag",1},
{ "Los Santos International",     2051.60, -2597.20,   -39.00,  2152.40, -2394.30,    60.90,"NoOwner","NoTag",1},
{ "Marina",                        647.70, -1804.20,   -89.00,   851.40, -1577.50,   110.90,"NoOwner","NoTag",1},
{ "Marina",                        647.70, -1577.50,   -89.00,   807.90, -1416.20,   110.90,"NoOwner","NoTag",1},
{ "Marina",                        807.90, -1577.50,   -89.00,   926.90, -1416.20,   110.90,"NoOwner","NoTag",1},
{ "Market",                        787.40, -1416.20,   -89.00,  1072.60, -1310.20,   110.90,"NoOwner","NoTag",1},
{ "Market",                        952.60, -1310.20,   -89.00,  1072.60, -1130.80,   110.90,"NoOwner","NoTag",1},
{ "Market",                       1072.60, -1416.20,   -89.00,  1370.80, -1130.80,   110.90,"NoOwner","NoTag",1},
{ "Market",                        926.90, -1577.50,   -89.00,  1370.80, -1416.20,   110.90,"NoOwner","NoTag",1},
{ "Market Station",                787.40, -1410.90,   -34.10,   866.00, -1310.20,    65.80,"NoOwner","NoTag",1},
{ "Martin Bridge",                -222.10,   293.30,     0.00,  -122.10,   476.40,   200.00,"NoOwner","NoTag",1},
{ "Missionary Hill",             -2994.40,  -811.20,     0.00, -2178.60,  -430.20,   200.00,"NoOwner","NoTag",1},
{ "Montgomery",                   1119.50,   119.50,    -3.00,  1451.40,   493.30,   200.00,"NoOwner","NoTag",1},
{ "Montgomery",                   1451.40,   347.40,    -6.10,  1582.40,   420.80,   200.00,"NoOwner","NoTag",1},
{ "Montgomery Intersection",      1546.60,   208.10,     0.00,  1745.80,   347.40,   200.00,"NoOwner","NoTag",1},
{ "Montgomery Intersection",      1582.40,   347.40,     0.00,  1664.60,   401.70,   200.00,"NoOwner","NoTag",1},
{ "Mulholland",                   1414.00,  -768.00,   -89.00,  1667.60,  -452.40,   110.90,"NoOwner","NoTag",1},
{ "Mulholland",                   1281.10,  -452.40,   -89.00,  1641.10,  -290.90,   110.90,"NoOwner","NoTag",1},
{ "Mulholland",                   1269.10,  -768.00,   -89.00,  1414.00,  -452.40,   110.90,"NoOwner","NoTag",1},
{ "Mulholland",                   1357.00,  -926.90,   -89.00,  1463.90,  -768.00,   110.90,"NoOwner","NoTag",1},
{ "Mulholland",                   1318.10,  -910.10,   -89.00,  1357.00,  -768.00,   110.90,"NoOwner","NoTag",1},
{ "Mulholland",                   1169.10,  -910.10,   -89.00,  1318.10,  -768.00,   110.90,"NoOwner","NoTag",1},
{ "Mulholland",                    768.60,  -954.60,   -89.00,   952.60,  -860.60,   110.90,"NoOwner","NoTag",1},
{ "Mulholland",                    687.80,  -860.60,   -89.00,   911.80,  -768.00,   110.90,"NoOwner","NoTag",1},
{ "Mulholland",                    737.50,  -768.00,   -89.00,  1142.20,  -674.80,   110.90,"NoOwner","NoTag",1},
{ "Mulholland",                   1096.40,  -910.10,   -89.00,  1169.10,  -768.00,   110.90,"NoOwner","NoTag",1},
{ "Mulholland",                    952.60,  -937.10,   -89.00,  1096.40,  -860.60,   110.90,"NoOwner","NoTag",1},
{ "Mulholland",                    911.80,  -860.60,   -89.00,  1096.40,  -768.00,   110.90,"NoOwner","NoTag",1},
{ "Mulholland",                    861.00,  -674.80,   -89.00,  1156.50,  -600.80,   110.90,"NoOwner","NoTag",1},
{ "Mulholland Intersection",      1463.90, -1150.80,   -89.00,  1812.60,  -768.00,   110.90,"NoOwner","NoTag",1},
{ "North Rock",                   2285.30,  -768.00,     0.00,  2770.50,  -269.70,   200.00,"NoOwner","NoTag",1},
{ "Ocean Docks",                  2373.70, -2697.00,   -89.00,  2809.20, -2330.40,   110.90,"NoOwner","NoTag",1},
{ "Ocean Docks",                  2201.80, -2418.30,   -89.00,  2324.00, -2095.00,   110.90,"NoOwner","NoTag",1},
{ "Ocean Docks",                  2324.00, -2302.30,   -89.00,  2703.50, -2145.10,   110.90,"NoOwner","NoTag",1},
{ "Ocean Docks",                  2089.00, -2394.30,   -89.00,  2201.80, -2235.80,   110.90,"NoOwner","NoTag",1},
{ "Ocean Docks",                  2201.80, -2730.80,   -89.00,  2324.00, -2418.30,   110.90,"NoOwner","NoTag",1},
{ "Ocean Docks",                  2703.50, -2302.30,   -89.00,  2959.30, -2126.90,   110.90,"NoOwner","NoTag",1},
{ "Ocean Docks",                  2324.00, -2145.10,   -89.00,  2703.50, -2059.20,   110.90,"NoOwner","NoTag",1},
{ "Ocean Flats",                 -2994.40,   277.40,    -9.10, -2867.80,   458.40,   200.00,"NoOwner","NoTag",1},
{ "Ocean Flats",                 -2994.40,  -222.50,    -0.00, -2593.40,   277.40,   200.00,"NoOwner","NoTag",1},
{ "Ocean Flats",                 -2994.40,  -430.20,    -0.00, -2831.80,  -222.50,   200.00,"NoOwner","NoTag",1},
{ "Octane Springs",                338.60,  1228.50,     0.00,   664.30,  1655.00,   200.00,"NoOwner","NoTag",1},
{ "Old Venturas Strip",           2162.30,  2012.10,   -89.00,  2685.10,  2202.70,   110.90,"NoOwner","NoTag",1},
{ "Palisades",                   -2994.40,   458.40,    -6.10, -2741.00,  1339.60,   200.00,"NoOwner","NoTag",1},
{ "Palomino Creek",               2160.20,  -149.00,     0.00,  2576.90,   228.30,   200.00,"NoOwner","NoTag",1},
{ "Paradiso",                    -2741.00,   793.40,    -6.10, -2533.00,  1268.40,   200.00,"NoOwner","NoTag",1},
{ "Pershing Square",              1440.90, -1722.20,   -89.00,  1583.50, -1577.50,   110.90,"NoOwner","NoTag",1},
{ "Pilgrim",                      2437.30,  1383.20,   -89.00,  2624.40,  1783.20,   110.90,"NoOwner","NoTag",1},
{ "Pilgrim",                      2624.40,  1383.20,   -89.00,  2685.10,  1783.20,   110.90,"NoOwner","NoTag",1},
{ "Pilson Intersection",          1098.30,  2243.20,   -89.00,  1377.30,  2507.20,   110.90,"NoOwner","NoTag",1},
{ "Pirates in Men's Pants",       1817.30,  1469.20,   -89.00,  2027.40,  1703.20,   110.90,"NoOwner","NoTag",1},
{ "Playa del Seville",            2703.50, -2126.90,   -89.00,  2959.30, -1852.80,   110.90,"NoOwner","NoTag",1},
{ "Prickle Pine",                 1534.50,  2583.20,   -89.00,  1848.40,  2863.20,   110.90,"NoOwner","NoTag",1},
{ "Prickle Pine",                 1117.40,  2507.20,   -89.00,  1534.50,  2723.20,   110.90,"NoOwner","NoTag",1},
{ "Prickle Pine",                 1848.40,  2553.40,   -89.00,  1938.80,  2863.20,   110.90,"NoOwner","NoTag",1},
{ "Prickle Pine",                 1938.80,  2624.20,   -89.00,  2121.40,  2861.50,   110.90,"NoOwner","NoTag",1},
{ "Queens",                      -2533.00,   458.40,     0.00, -2329.30,   578.30,   200.00,"NoOwner","NoTag",1},
{ "Queens",                      -2593.40,    54.70,     0.00, -2411.20,   458.40,   200.00,"NoOwner","NoTag",1},
{ "Queens",                      -2411.20,   373.50,     0.00, -2253.50,   458.40,   200.00,"NoOwner","NoTag",1},
{ "Randolph Industrial Estate",   1558.00,   596.30,   -89.00,  1823.00,   823.20,   110.90,"NoOwner","NoTag",1},
{ "Redsands East",                1817.30,  2011.80,   -89.00,  2106.70,  2202.70,   110.90,"NoOwner","NoTag",1},
{ "Redsands East",                1817.30,  2202.70,   -89.00,  2011.90,  2342.80,   110.90,"NoOwner","NoTag",1},
{ "Redsands East",                1848.40,  2342.80,   -89.00,  2011.90,  2478.40,   110.90,"NoOwner","NoTag",1},
{ "Redsands West",                1236.60,  1883.10,   -89.00,  1777.30,  2142.80,   110.90,"NoOwner","NoTag",1},
{ "Redsands West",                1297.40,  2142.80,   -89.00,  1777.30,  2243.20,   110.90,"NoOwner","NoTag",1},
{ "Redsands West",                1377.30,  2243.20,   -89.00,  1704.50,  2433.20,   110.90,"NoOwner","NoTag",1},
{ "Redsands West",                1704.50,  2243.20,   -89.00,  1777.30,  2342.80,   110.90,"NoOwner","NoTag",1},
{ "Regular Tom",                  -405.70,  1712.80,    -3.00,  -276.70,  1892.70,   200.00,"NoOwner","NoTag",1},
{ "Richman",                       647.50, -1118.20,   -89.00,   787.40,  -954.60,   110.90,"NoOwner","NoTag",1},
{ "Richman",                       647.50,  -954.60,   -89.00,   768.60,  -860.60,   110.90,"NoOwner","NoTag",1},
{ "Richman",                       225.10, -1369.60,   -89.00,   334.50, -1292.00,   110.90,"NoOwner","NoTag",1},
{ "Richman",                       225.10, -1292.00,   -89.00,   466.20, -1235.00,   110.90,"NoOwner","NoTag",1},
{ "Richman",                        72.60, -1404.90,   -89.00,   225.10, -1235.00,   110.90,"NoOwner","NoTag",1},
{ "Richman",                        72.60, -1235.00,   -89.00,   321.30, -1008.10,   110.90,"NoOwner","NoTag",1},
{ "Richman",                       321.30, -1235.00,   -89.00,   647.50, -1044.00,   110.90,"NoOwner","NoTag",1},
{ "Richman",                       321.30, -1044.00,   -89.00,   647.50,  -860.60,   110.90,"NoOwner","NoTag",1},
{ "Richman",                       321.30,  -860.60,   -89.00,   687.80,  -768.00,   110.90,"NoOwner","NoTag",1},
{ "Richman",                       321.30,  -768.00,   -89.00,   700.70,  -674.80,   110.90,"NoOwner","NoTag",1},
{ "Robada Intersection",         -1119.00,  1178.90,   -89.00,  -862.00,  1351.40,   110.90,"NoOwner","NoTag",1},
{ "Roca Escalante",               2237.40,  2202.70,   -89.00,  2536.40,  2542.50,   110.90,"NoOwner","NoTag",1},
{ "Roca Escalante",               2536.40,  2202.70,   -89.00,  2625.10,  2442.50,   110.90,"NoOwner","NoTag",1},
{ "Rockshore East",               2537.30,   676.50,   -89.00,  2902.30,   943.20,   110.90,"NoOwner","NoTag",1},
{ "Rockshore West",               1997.20,   596.30,   -89.00,  2377.30,   823.20,   110.90,"NoOwner","NoTag",1},
{ "Rockshore West",               2377.30,   596.30,   -89.00,  2537.30,   788.80,   110.90,"NoOwner","NoTag",1},
{ "Rodeo",                          72.60, -1684.60,   -89.00,   225.10, -1544.10,   110.90,"NoOwner","NoTag",1},
{ "Rodeo",                          72.60, -1544.10,   -89.00,   225.10, -1404.90,   110.90,"NoOwner","NoTag",1},
{ "Rodeo",                         225.10, -1684.60,   -89.00,   312.80, -1501.90,   110.90,"NoOwner","NoTag",1},
{ "Rodeo",                         225.10, -1501.90,   -89.00,   334.50, -1369.60,   110.90,"NoOwner","NoTag",1},
{ "Rodeo",                         334.50, -1501.90,   -89.00,   422.60, -1406.00,   110.90,"NoOwner","NoTag",1},
{ "Rodeo",                         312.80, -1684.60,   -89.00,   422.60, -1501.90,   110.90,"NoOwner","NoTag",1},
{ "Rodeo",                         422.60, -1684.60,   -89.00,   558.00, -1570.20,   110.90,"NoOwner","NoTag",1},
{ "Rodeo",                         558.00, -1684.60,   -89.00,   647.50, -1384.90,   110.90,"NoOwner","NoTag",1},
{ "Rodeo",                         466.20, -1570.20,   -89.00,   558.00, -1385.00,   110.90,"NoOwner","NoTag",1},
{ "Rodeo",                         422.60, -1570.20,   -89.00,   466.20, -1406.00,   110.90,"NoOwner","NoTag",1},
{ "Rodeo",                         466.20, -1385.00,   -89.00,   647.50, -1235.00,   110.90,"NoOwner","NoTag",1},
{ "Rodeo",                         334.50, -1406.00,   -89.00,   466.20, -1292.00,   110.90,"NoOwner","NoTag",1},
{ "Royal Casino",                 2087.30,  1383.20,   -89.00,  2437.30,  1543.20,   110.90,"NoOwner","NoTag",1},
{ "San Andreas Sound",            2450.30,   385.50,  -100.00,  2759.20,   562.30,   200.00,"NoOwner","NoTag",1},
{ "Santa Flora",                 -2741.00,   458.40,    -7.60, -2533.00,   793.40,   200.00,"NoOwner","NoTag",1},
{ "Santa Maria Beach",             342.60, -2173.20,   -89.00,   647.70, -1684.60,   110.90,"NoOwner","NoTag",1},
{ "Santa Maria Beach",              72.60, -2173.20,   -89.00,   342.60, -1684.60,   110.90,"NoOwner","NoTag",1},
{ "Shady Cabin",                 -1632.80, -2263.40,    -3.00, -1601.30, -2231.70,   200.00,"NoOwner","NoTag",1},
{ "Shady Creeks",                -1820.60, -2643.60,    -8.00, -1226.70, -1771.60,   200.00,"NoOwner","NoTag",1},
{ "Shady Creeks",                -2030.10, -2174.80,    -6.10, -1820.60, -1771.60,   200.00,"NoOwner","NoTag",1},
{ "Sobell Rail Yards",            2749.90,  1548.90,   -89.00,  2923.30,  1937.20,   110.90,"NoOwner","NoTag",1},
{ "Spinybed",                     2121.40,  2663.10,   -89.00,  2498.20,  2861.50,   110.90,"NoOwner","NoTag",1},
{ "Starfish Casino",              2437.30,  1783.20,   -89.00,  2685.10,  2012.10,   110.90,"NoOwner","NoTag",1},
{ "Starfish Casino",              2437.30,  1858.10,   -39.00,  2495.00,  1970.80,    60.90,"NoOwner","NoTag",1},
{ "Starfish Casino",              2162.30,  1883.20,   -89.00,  2437.30,  2012.10,   110.90,"NoOwner","NoTag",1},
{ "Temple",                       1252.30, -1130.80,   -89.00,  1378.30, -1026.30,   110.90,"NoOwner","NoTag",1},
{ "Temple",                       1252.30, -1026.30,   -89.00,  1391.00,  -926.90,   110.90,"NoOwner","NoTag",1},
{ "Temple",                       1252.30,  -926.90,   -89.00,  1357.00,  -910.10,   110.90,"NoOwner","NoTag",1},
{ "Temple",                        952.60, -1130.80,   -89.00,  1096.40,  -937.10,   110.90,"NoOwner","NoTag",1},
{ "Temple",                       1096.40, -1130.80,   -89.00,  1252.30, -1026.30,   110.90,"NoOwner","NoTag",1},
{ "Temple",                       1096.40, -1026.30,   -89.00,  1252.30,  -910.10,   110.90,"NoOwner","NoTag",1},
{ "The Camel's Toe",              2087.30,  1203.20,   -89.00,  2640.40,  1383.20,   110.90,"NoOwner","NoTag",1},
{ "The Clown's Pocket",           2162.30,  1783.20,   -89.00,  2437.30,  1883.20,   110.90,"NoOwner","NoTag",1},
{ "The Emerald Isle",             2011.90,  2202.70,   -89.00,  2237.40,  2508.20,   110.90,"NoOwner","NoTag",1},
{ "The Farm",                    -1209.60, -1317.10,   114.90,  -908.10,  -787.30,   251.90,"NoOwner","NoTag",1},
{ "The Four Dragons Casino",      1817.30,   863.20,   -89.00,  2027.30,  1083.20,   110.90,"NoOwner","NoTag",1},
{ "The High Roller",              1817.30,  1283.20,   -89.00,  2027.30,  1469.20,   110.90,"NoOwner","NoTag",1},
{ "The Mako Span",                1664.60,   401.70,     0.00,  1785.10,   567.20,   200.00,"NoOwner","NoTag",1},
{ "The Panopticon",               -947.90,  -304.30,    -1.10,  -319.60,   327.00,   200.00,"NoOwner","NoTag",1},
{ "The Pink Swan",                1817.30,  1083.20,   -89.00,  2027.30,  1283.20,   110.90,"NoOwner","NoTag",1},
{ "The Sherman Dam",              -968.70,  1929.40,    -3.00,  -481.10,  2155.20,   200.00,"NoOwner","NoTag",1},
{ "The Strip",                    2027.40,   863.20,   -89.00,  2087.30,  1703.20,   110.90,"NoOwner","NoTag",1},
{ "The Strip",                    2106.70,  1863.20,   -89.00,  2162.30,  2202.70,   110.90,"NoOwner","NoTag",1},
{ "The Strip",                    2027.40,  1783.20,   -89.00,  2162.30,  1863.20,   110.90,"NoOwner","NoTag",1},
{ "The Strip",                    2027.40,  1703.20,   -89.00,  2137.40,  1783.20,   110.90,"NoOwner","NoTag",1},
{ "The Visage",                   1817.30,  1863.20,   -89.00,  2106.70,  2011.80,   110.90,"NoOwner","NoTag",1},
{ "The Visage",                   1817.30,  1703.20,   -89.00,  2027.40,  1863.20,   110.90,"NoOwner","NoTag",1},
{ "Unity Station",                1692.60, -1971.80,   -20.40,  1812.60, -1932.80,    79.50,"NoOwner","NoTag",1},
{ "Valle Ocultado",               -936.60,  2611.40,     2.00,  -715.90,  2847.90,   200.00,"NoOwner","NoTag",1},
{ "Verdant Bluffs",                930.20, -2488.40,   -89.00,  1249.60, -2006.70,   110.90,"NoOwner","NoTag",1},
{ "Verdant Bluffs",               1073.20, -2006.70,   -89.00,  1249.60, -1842.20,   110.90,"NoOwner","NoTag",1},
{ "Verdant Bluffs",               1249.60, -2179.20,   -89.00,  1692.60, -1842.20,   110.90,"NoOwner","NoTag",1},
{ "Verdant Meadows",                37.00,  2337.10,    -3.00,   435.90,  2677.90,   200.00,"NoOwner","NoTag",1},
{ "Verona Beach",                  647.70, -2173.20,   -89.00,   930.20, -1804.20,   110.90,"NoOwner","NoTag",1},
{ "Verona Beach",                  930.20, -2006.70,   -89.00,  1073.20, -1804.20,   110.90,"NoOwner","NoTag",1},
{ "Verona Beach",                  851.40, -1804.20,   -89.00,  1046.10, -1577.50,   110.90,"NoOwner","NoTag",1},
{ "Verona Beach",                 1161.50, -1722.20,   -89.00,  1323.90, -1577.50,   110.90,"NoOwner","NoTag",1},
{ "Verona Beach",                 1046.10, -1722.20,   -89.00,  1161.50, -1577.50,   110.90,"NoOwner","NoTag",1},
{ "Vinewood",                      787.40, -1310.20,   -89.00,   952.60, -1130.80,   110.90,"NoOwner","NoTag",1},
{ "Vinewood",                      787.40, -1130.80,   -89.00,   952.60,  -954.60,   110.90,"NoOwner","NoTag",1},
{ "Vinewood",                      647.50, -1227.20,   -89.00,   787.40, -1118.20,   110.90,"NoOwner","NoTag",1},
{ "Vinewood",                      647.70, -1416.20,   -89.00,   787.40, -1227.20,   110.90,"NoOwner","NoTag",1},
{ "Whitewood Estates",             883.30,  1726.20,   -89.00,  1098.30,  2507.20,   110.90,"NoOwner","NoTag",1},
{ "Whitewood Estates",            1098.30,  1726.20,   -89.00,  1197.30,  2243.20,   110.90,"NoOwner","NoTag",1},
{ "Willowfield",                  1970.60, -2179.20,   -89.00,  2089.00, -1852.80,   110.90,"NoOwner","NoTag",1},
{ "Willowfield",                  2089.00, -2235.80,   -89.00,  2201.80, -1989.90,   110.90,"NoOwner","NoTag",1},
{ "Willowfield",                  2089.00, -1989.90,   -89.00,  2324.00, -1852.80,   110.90,"NoOwner","NoTag",1},
{ "Willowfield",                  2201.80, -2095.00,   -89.00,  2324.00, -1989.90,   110.90,"NoOwner","NoTag",1},
{ "Willowfield",                  2541.70, -1941.40,   -89.00,  2703.50, -1852.80,   110.90,"NoOwner","NoTag",1},
{ "Willowfield",                  2324.00, -2059.20,   -89.00,  2541.70, -1852.80,   110.90,"NoOwner","NoTag",1},
{ "Willowfield",                  2541.70, -2059.20,   -89.00,  2703.50, -1941.40,   110.90,"NoOwner","NoTag",1},
{ "Yellow Bell Station",          1377.40,  2600.40,   -21.90,  1492.40,  2687.30,    78.00,"NoOwner","NoTag",1},
// Main Zones
{ "Los Santos",                     44.60, -2892.90,  -242.90,  2997.00,  -768.00,   900.00,"NoOwner","NoTag",0},
{ "Las Venturas",                  869.40,   596.30,  -242.90,  2997.00,  2993.80,   900.00,"NoOwner","NoTag",0},
{ "Bone County",                  -480.50,   596.30,  -242.90,   869.40,  2993.80,   900.00,"NoOwner","NoTag",0},
{ "Tierra Robada",               -2997.40,  1659.60,  -242.90,  -480.50,  2993.80,   900.00,"NoOwner","NoTag",0},
{ "Tierra Robada",               -1213.90,   596.30,  -242.90,  -480.50,  1659.60,   900.00,"NoOwner","NoTag",0},
{ "San Fierro",                  -2997.40, -1115.50,  -242.90, -1213.90,  1659.60,   900.00,"NoOwner","NoTag",0},
{ "Red County",                  -1213.90,  -768.00,  -242.90,  2997.00,   596.30,   900.00,"NoOwner","NoTag",0},
{ "Flint County",                -1213.90, -2892.90,  -242.90,    44.60,  -768.00,   900.00,"NoOwner","NoTag",0},
{ "Whetstone",                   -2997.40, -2892.90,  -242.90, -1213.90, -1115.50,   900.00,"NoOwner","NoTag",0}
};

#define MAX_CITY 3
enum cityinfo {
	city_name[MAX_NAME],
    	Float:city_minx,
    	Float:city_miny,
    	Float:city_minz,
    	Float:city_maxx,
    	Float:city_maxy,
    	Float:city_maxz
}

new Float:cityes[MAX_CITY][cityinfo] = {
{ "Los Santos",                     44.60, -2892.90,  -242.90,  2997.00,  -768.00,   900.00},
{ "San Fierro",                  -2997.40, -1115.50,  -242.90, -1213.90,  1659.60,   900.00},
{ "Las Venturas",                  869.40,   596.30,  -242.90,  2997.00,  2993.80,   900.00}
};

//---------------------------------------------------------------------------------------------------------------

new ZoneDB[MAX_STRING] = "MultiGTA/Zones/";

ZonesLoadAll()
{
	Debug("world.inc > ZonesLoadAll - Start");
	
	new temp[MAX_STRING];
	if (!db_Exists(DatabaseDB)) db_Create(DatabaseDB);
	set(temp,db_Get(DatabaseDB,"Zone_DB"));
	if (strlen(temp) > 0) set(ZoneDB,temp);
	if (!db_Exists(ZoneDB))
	{
		ZonesSaveAll();
		Debug("world.inc > ZonesLoadAll - Stop");
		return;
	}
	for (new i=0;i<MAX_ZONES;i++)
	{
		new zonedbname[MAX_STRING];
		format(zonedbname,sizeof(zonedbname),"%s%s.txt",ZoneDB,zones[i][zone_name]);
		if (!db_Exists(zonedbname)) ZonesSaveAll();

		set(temp,db_Get(zonedbname,"Gang_Owner"));
		if (strlen(temp) > 0) set(zones[i][gang_owner],temp);
		set(temp,nullstr);

		set(temp,db_Get(zonedbname,"Gang_Tag"));
		if (strlen(temp) > 0) set(zones[i][gang_tag],temp);
		set(temp,nullstr);

		set(temp,db_Get(zonedbname,"CanBeCaptured"));
		if (strlen(temp) > 0) zones[i][canbecaptured] = strval(temp);
		set(temp,nullstr);
	}
	Debug("world.inc > ZonesLoadAll - Stop");
	return;

}

ZonesSaveAll()
{
	Debug("world.inc > ZonesSaveAll - Start");
	new temp[MAX_STRING];
	if (db_Exists(DatabaseDB)) db_Set(DatabaseDB,"Zone_DB",ZoneDB);

	for (new i=0;i<MAX_ZONES;i++)
	{
		new zonedbname[MAX_STRING];
		format(zonedbname,sizeof(zonedbname),"%s%s.txt",ZoneDB,zones[i][zone_name]);
		if (db_Exists(zonedbname)){
			continue;
		}
		db_Create(zonedbname);

		db_BeforeBigSaving(zonedbname);

		db_Set(zonedbname,"Gang_Owner",zones[i][gang_owner]);

		db_Set(zonedbname,"Gang_Tag",zones[i][gang_tag]);

		valstr(temp,zones[i][canbecaptured]);
		db_Set(zonedbname,"CanBeCaptured",temp);
		set(temp,nullstr);

		db_AfterBigSaving(zonedbname);

	}
	Debug("world.inc > ZonesSaveAll - Stop");
	return;

}

GetXYZZoneName(Float:x, Float:y, Float:z) // will return first zone name that XYZ is found in
{
	Debug("world.inc > GetXYZZoneName - Start");
	new name[MAX_STRING] = "Unknown";
	if ((x == 0.0) && (y = 0.0)) return name;
	for (new zoneid = 0; zoneid < MAX_ZONES; zoneid++)
	{
 		if (IsXYZInZone(x, y, z, zoneid))// we are in this zone
		{
            		set(name,zones[zoneid][zone_name]);
			Debug("world.inc > GetXYZZoneName - Stop");
		   	return name;
		}
 	}
	Debug("world.inc > GetXYZZoneName - Stop");
 	return name;
}


public GetXYZZoneID(Float:x, Float:y, Float:z) // will return first zoneID that XYZ is found in
{
	Debug("world.inc > GetXYZZoneID - Start");
	for (new zoneid = 0; zoneid < MAX_ZONES; zoneid++)
	{
 		if (IsXYZInZone(x, y, z, zoneid))// we are in this zone
		{
			Debug("world.inc > GetXYZZoneID - Stop");
			return zoneid;
		}
	}
	Debug("world.inc > GetXYZZoneID - Stop");
	return -1;
}

public IsXYZInZone(Float:x, Float:y, Float:z, zoneid)
{
 if(zoneid == -1) return 0;
 if(x >= zones[zoneid][zone_minx] && x < zones[zoneid][zone_maxx]
 && y >= zones[zoneid][zone_miny] && y < zones[zoneid][zone_maxy]
 && z >= zones[zoneid][zone_minz] && z < zones[zoneid][zone_maxz])
 {
 	return 1;
 }
 return 0;
}

GetMaxZones() //for hotspot module
{
	return MAX_ZONES;
}

GetZoneNameByID(zoneid) //for hotspot module
{
	new name[MAX_STRING];
	copy(name,zones[zoneid][zone_name],strlen(zones[zoneid][zone_name]));
	return name;
}

stock GetXYZCityID(Float:x, Float:y, Float:z) // will return first cityID that XYZ is found in
{
	Debug("world.inc > GetXYZCityID - Start");
	for (new cityid = 0; cityid < MAX_CITY; cityid++)
	{
 		if (IsXYZInCity(x, y, z, cityid))// we are in this city
		{
			Debug("world.inc > GetXYZCityID - Stop");
			return cityid;
		}
	}
	Debug("world.inc > GetXYZCityID - Start");
	return -1;
}

stock GetXYZCityName(Float:x, Float:y, Float:z) // will return first city name that XYZ is found in
{
	Debug("world.inc > GetXYZCityName - Start");
	new name[MAX_STRING] = "Unknown";
	if ((x == 0.0) && (y = 0.0)) return name;
	for (new cityid = 0; cityid < MAX_CITY; cityid++)
	{
 		if (IsXYZInCity(x, y, z, cityid))// we are in this city
		{
            		set(name,cityes[cityid][city_name]);
			Debug("world.inc > GetXYZCityName - Stop");
		   	return name;
		}
 	}
	Debug("world.inc > GetXYZCityName - Stop");
 	return name;
}

stock IsXYZInCity(Float:x, Float:y, Float:z, cityid)
{
 if(cityid == -1) return 0;
 if(x >= cityes[cityid][city_minx] && x < cityes[cityid][city_maxx]
 && y >= cityes[cityid][city_miny] && y < cityes[cityid][city_maxy]
 && z >= cityes[cityid][city_minz] && z < cityes[cityid][city_maxz])
 {
 	return 1;
 }
 return 0;
}

public IsPlayerInZone(playerid, zoneid)
{
 if(zoneid == -1) return 0;
 return IsXYZInZone(PlayerPos[playerid][Coord_X], PlayerPos[playerid][Coord_Y], PlayerPos[playerid][Coord_Z], zoneid);
}

public IsPlayerInZoneName(playerid, zonename[])
{
 for (new zoneid = 0; zoneid < MAX_ZONES; zoneid++)
 {
 	if (strcomp(zonename,zones[zoneid][zone_name],false))
 	{
 		return IsPlayerInZone(playerid, zoneid);
 	}
 }
 return 0;
}


public IsPlayerInCasino(playerid)  // used for gamble. return 1 if true
{
	if (IsPlayerInZoneName(playerid,"Caligula's Palace")) return 1;
	if (IsPlayerInZoneName(playerid,"The Four Dragons Casino")) return 1;
	if (IsPlayerInZoneName(playerid,"Redsands Casino")) return 1;
	return 0;
}


public IsPlayerInStore(playerid)  // used for health gain, return 1 if true
{ // TODO
	if (IsPlayerInZoneName(playerid,"24/7")) return 1;
	return 0;
}


//----------------------------------------------------------------------------------------------------



public loccmp(Float:x1,Float:y1,Float:z1,Float:x2,Float:y2,Float:z2)
{
	if ((x1 == x2) && (y1 == y2) && (z1 == z2)) return 1;
	return 0;
}

public Float:GetDistancePlayertoXYZ(playerid,Float:x2,Float:y2,Float:z2)
{
	if (!IsPlayerConnected(playerid)) {return 65000.0;}
	new Float:dist = floatsqroot(floatpower(floatabs(floatsub(x2,PlayerPos[playerid][Coord_X])),2)+floatpower(floatabs(floatsub(y2,PlayerPos[playerid][Coord_Y])),2)+floatpower(floatabs(floatsub(z2,PlayerPos[playerid][Coord_Z])),2));
	return dist;
}

public Float:GetDistanceXYZtoXYZ(Float:x1,Float:y1,Float:z1,Float:x2,Float:y2,Float:z2)
{
	new Float:dist = floatsqroot(floatpower(floatabs(floatsub(x2,x1)),2)+floatpower(floatabs(floatsub(y2,y1)),2)+floatpower(floatabs(floatsub(z2,z1)),2));
	return dist;
}




//----------------------------------------------------

public SyncActiveCP()
{
	Debug("world.inc > SyncActiveCP - Start");
	for (new playerid=0;playerid<MAX_PLAYERS_EX;playerid++)
	{
	    if (!IsPlayerConnected(playerid)) continue;
	    if (PlayerSpawned[playerid] == false) continue; //Player is not spawned. We don't need to sync CPs
	    if (GetPlayerVirtualWorld(playerid) != WORLD_DEFAULT) continue;
	    if (PlayerQuest[playerid] == 0)
	   	{
	    		new cpid = GetPlayerClosestActiveCP(playerid);
			if (cpid != 0){
	    			SetPlayerActiveCP(playerid,cpid);
			} else {
				oDisablePlayerCheckpoint(playerid);
			}
			cpid = GetPlayerClosestActiveRaceCP(playerid);
			if (cpid != 0){
	    			SetPlayerActiveRaceCP(playerid,cpid);
			} else {
				oDisablePlayerRaceCheckpoint(playerid);
			}
		}
	}
	Debug("world.inc > SyncActiveCP - Stop");
}



public AddActiveCheckpoint(Float:cpx,Float:cpy,Float:cpz,cpdist,cpsize) //
{
	Debug("world.inc > AddActiveCheckpoint - Start");
	if (ActiveCPCount == MAX_ACTIVECP) {Debug("world.inc > AddActiveCheckpoint - Stop"); return 0;}
	for (new cpid=1;cpid<=MAX_ACTIVECP;cpid++)
	{
		if (ActiveCheckpoints[cpid][IsActive] == false)
		{ // we have found an inactive cp to use
			ActiveCPCount++; // 0 will be invalid, so we ++ first
			ActiveCheckpoints[cpid][CP_X] = cpx;
			ActiveCheckpoints[cpid][CP_Y] = cpy;
			ActiveCheckpoints[cpid][CP_Z] = cpz;
			ActiveCheckpoints[cpid][CP_Dist] = cpdist;
			ActiveCheckpoints[cpid][CP_Size] = cpsize;
			ActiveCheckpoints[cpid][IsActive] = true;
			Debug("world.inc > AddActiveCheckpoint - Stop");
			return cpid; // exit and send back our new cpid
		}
 	}
	Debug("world.inc > AddActiveCheckpoint - Stop");
 	return 0;
}

public AddActiveRaceCheckpoint(type,Float:cpx,Float:cpy,Float:cpz,Float:nextcpx,Float:nextcpy,Float:nextcpz,cpdist,cpsize) //
{
	Debug("world.inc > AddActiveRaceCheckpoint - Start");
	if (ActiveRaceCPCount == MAX_ACTIVERACECP) {Debug("world.inc > AddActiveRaceCheckpoint - Stop"); return 0;}
	for (new cpid=1;cpid<=MAX_ACTIVERACECP;cpid++)
	{
		if (ActiveRaceCheckpoints[cpid][IsActive] == false)
		{ // we have found an inactive cp to use
			ActiveRaceCPCount++; // 0 will be invalid, so we ++ first
			ActiveRaceCheckpoints[cpid][CP_TYPE] = type;
			ActiveRaceCheckpoints[cpid][CP_X] = cpx;
			ActiveRaceCheckpoints[cpid][CP_Y] = cpy;
			ActiveRaceCheckpoints[cpid][CP_Z] = cpz;
			ActiveRaceCheckpoints[cpid][NEXTCP_X] = nextcpx;
			ActiveRaceCheckpoints[cpid][NEXTCP_Y] = nextcpy;
			ActiveRaceCheckpoints[cpid][NEXTCP_Z] = nextcpz;
			ActiveRaceCheckpoints[cpid][CP_Dist] = cpdist;
			ActiveRaceCheckpoints[cpid][CP_Size] = cpsize;
			ActiveRaceCheckpoints[cpid][IsActive] = true;
			Debug("world.inc > AddActiveRaceCheckpoint - Stop");
			return cpid; // exit and send back our new cpid
		}
 	}
	Debug("world.inc > AddActiveRaceCheckpoint - Stop");
 	return 0;
}

public RemoveActiveCheckpoint(cpid)
{
	Debug("world.inc > RemoveActiveCheckpoint - Start");
	if (ActiveCheckpoints[cpid][IsActive] == false) {Debug("world.inc > RemoveActiveCheckpoint - Stop");return 0;}

	ActiveCheckpoints[cpid][CP_X] = 0;
	ActiveCheckpoints[cpid][CP_Y] = 0;
	ActiveCheckpoints[cpid][CP_Z] = 0;
	ActiveCheckpoints[cpid][CP_Size] = 0;
	ActiveCheckpoints[cpid][IsActive] = false;
	ActiveCPCount--;
	Debug("world.inc > RemoveActiveCheckpoint - Stop");
	return 1;
}

public RemoveActiveRaceCheckpoint(cpid)
{
	Debug("world.inc > RemoveActiveRaceCheckpoint - Start");
	if (ActiveRaceCheckpoints[cpid][IsActive] == false) {Debug("world.inc > RemoveActiveRaceCheckpoint - Stop");return 0;}
	ActiveRaceCheckpoints[cpid][CP_TYPE] = 0;
	ActiveRaceCheckpoints[cpid][CP_X] = 0;
	ActiveRaceCheckpoints[cpid][CP_Y] = 0;
	ActiveRaceCheckpoints[cpid][CP_Z] = 0;
	ActiveRaceCheckpoints[cpid][NEXTCP_X] = 0;
	ActiveRaceCheckpoints[cpid][NEXTCP_Y] = 0;
	ActiveRaceCheckpoints[cpid][NEXTCP_Z] = 0;
	ActiveRaceCheckpoints[cpid][CP_Size] = 0;
	ActiveRaceCheckpoints[cpid][IsActive] = false;
	ActiveRaceCPCount--;
	Debug("world.inc > RemoveActiveRaceCheckpoint - Stop");
	return 1;
}

public SetPlayerActiveCP(playerid,cpid)
{
	if (!IsPlayerConnected(playerid)) {Debug("world.inc > SetPlayerActiveCP - Start");return 0;}
	if (!ActiveCheckpoints[cpid][IsActive]) {Debug("world.inc > SetPlayerActiveCP - Stop");return 0;}
	if ((PlayerCP[playerid][Coord_X] == ActiveCheckpoints[cpid][CP_X]) && (PlayerCP[playerid][Coord_Y] == ActiveCheckpoints[cpid][CP_Y]) && (PlayerCP[playerid][Coord_Z] == ActiveCheckpoints[cpid][CP_Z]))
	{ // if player cp already == this cp
		Debug("world.inc > SetPlayerActiveCP - Stop");
	    	return 0;
	}
	oSetPlayerCheckpoint(playerid,ActiveCheckpoints[cpid][CP_X],ActiveCheckpoints[cpid][CP_Y],ActiveCheckpoints[cpid][CP_Z],ActiveCheckpoints[cpid][CP_Size]);
	Debug("world.inc > SetPlayerActiveCP - Stop");
	return 1;

}

public SetPlayerActiveRaceCP(playerid,cpid)
{
	if (!IsPlayerConnected(playerid)) {Debug("world.inc > SetPlayerActiveRaceCP - Start");return 0;}
	if (!ActiveRaceCheckpoints[cpid][IsActive]) {Debug("world.inc > SetPlayerActiveRaceCP - Stop");return 0;}
	if ((PlayerCP[playerid][Coord_X] == ActiveRaceCheckpoints[cpid][CP_X]) && (PlayerCP[playerid][Coord_Y] == ActiveRaceCheckpoints[cpid][CP_Y]) && (PlayerCP[playerid][Coord_Z] == ActiveRaceCheckpoints[cpid][CP_Z]))
	{ // if player cp already == this cp
		Debug("world.inc > SetPlayerActiveRaceCP - Stop");
	    	return 0;
	}
	oSetPlayerRaceCheckpoint(playerid,ActiveRaceCheckpoints[cpid][CP_TYPE],ActiveRaceCheckpoints[cpid][CP_X],ActiveRaceCheckpoints[cpid][CP_Y],ActiveRaceCheckpoints[cpid][CP_Z],ActiveRaceCheckpoints[cpid][NEXTCP_X],ActiveRaceCheckpoints[cpid][NEXTCP_Y],ActiveRaceCheckpoints[cpid][NEXTCP_Z],ActiveRaceCheckpoints[cpid][CP_Size]);
	Debug("world.inc > SetPlayerActiveRaceCP - Stop");
	return 1;
}

public GetPlayerClosestActiveCP(playerid)
{
Debug("world.inc > GetPlayerClosestActiveCP - Start");
new Float:cpdist=65000.0;
new retval=0;
for (new cpid=1;cpid<MAX_ACTIVECP;cpid++)
{
    if (ActiveCheckpoints[cpid][IsActive])
    {
		new Float:playerdist=GetDistancePlayertoXYZ(playerid,ActiveCheckpoints[cpid][CP_X],ActiveCheckpoints[cpid][CP_Y],ActiveCheckpoints[cpid][CP_Z]);
		if (playerdist < ActiveCheckpoints[cpid][CP_Dist])
		{
			if (playerdist < cpdist)
			{
				cpdist = playerdist;
				retval = cpid;
			}
		}
	}
}
Debug("world.inc > GetPlayerClosestActiveCP - Stop");
return retval;
}

public GetPlayerClosestActiveRaceCP(playerid)
{
Debug("world.inc > GetPlayerClosestActiveRaceCP - Start");
new Float:cpdist=65000.0;
new retval=0;
for (new cpid=1;cpid<MAX_ACTIVERACECP;cpid++)
{
    if (ActiveRaceCheckpoints[cpid][IsActive])
    {
		new Float:playerdist=GetDistancePlayertoXYZ(playerid,ActiveRaceCheckpoints[cpid][CP_X],ActiveRaceCheckpoints[cpid][CP_Y],ActiveRaceCheckpoints[cpid][CP_Z]);
		if (playerdist < ActiveRaceCheckpoints[cpid][CP_Dist])
		{
			if (playerdist < cpdist)
			{
				cpdist = playerdist;
				retval = cpid;
			}
		}
	}
}
Debug("world.inc > GetPlayerClosestActiveRaceCP - Stop");
return retval;
}


//----------------------------------------------------------------------------------------------------




public SpawnWorld() // run at startup to spawn world
{
	Debug("world.inc > SpawnWorld - Start");
	
	new filestring[256];
	new File: file = fopen(PlayerSkins_DB, io_read);
	new count = 0;
	if (!file){WriteLog("Cannot open skins file!"); SendRconCommand("exit"); return;}
	while (fread(file,filestring,sizeof(filestring)))
	{
		StripNewLine(filestring);
		if (strlen(filestring) == 0) return;
		if (strfind(filestring, "//",true) != -1) continue;
		if (!isNumeric(filestring)) continue;
		AddPlayerClass(strval(filestring),508.2252,-84.8755,998.9609,10.7533,0,0,0,0,-1,-1);
		count++;
	}
	fclose(file);
	new logstring[MAX_STRING];
	format(logstring,sizeof(logstring),"All player classes Loaded. Classes: %d",count);
	WriteLog(logstring);
	Debug("world.inc > SpawnWorld - Stop");
}

/*
public update_zones() {
 Debug("world.inc > update_zones - Start");
//new temp[MAX_ZONE_NAME+3];
 for(new i=0; i<MAX_PLAYERS_EX; i++) {
  if(IsPlayerConnected(i) && zoneupdates[i] == 1) {
   if(IsPlayerInZone(i,player_zone[i])) {  }
   else {
    new player_zone_before;
    player_zone_before = player_zone[i];
    player_zone[i] = -1;
    for(new j=0; j<sizeof(zones);j++) {
     if(IsPlayerInZone(i,j) && player_zone[i] == -1) {
      if(player_zone_before == -1) ShowTextForPlayer(i,zones[j][zone_name],1000,1);
   	  else  if(strcomp(zones[j][zone_name],zones[player_zone_before][zone_name],true) != 1)
		{ new text[256];
		  format(text, sizeof (text),gettext(499),zones[j][zone_name]);
		  ShowTextForPlayer(i,text,1000,1);
		}
      player_zone[i] = j;
     }
    }
    if(player_zone[i] == -1) player_zone[i] = player_zone_before;
   }
  }
 }
 Debug("world.inc > update_zones - Stop");
}
*/

public SavePlayersData(){
for (new id = 0; id < MAX_PLAYERS_EX; id++)
{
	if (IsPlayerConnected(id))
	{
		if (IsPlayerRegistered(id))
		{
			PlayerSave(id);
        	}
	}
}
}

public World_OnPlayerConnect(playerid){
	//Debug("world.inc > World_OnPlayerConnect - Start");
  	//zoneupdates[playerid] = 0;
	//player_zone[playerid] = -1;
	//Debug("world.inc > World_OnPlayerConnect - Stop");
	return 0;
}

public World_OnPlayerDeath(playerid){
	//Debug("world.inc > World_OnPlayerDeath - Start");
	//zoneupdates[playerid] = 0;
	//player_zone[playerid] = -1;
	//Debug("world.inc > World_OnPlayerDeath - Stop");
	return 0;
}

public World_OnPlayerSpawn(playerid){
	Debug("world.inc > World_OnPlayerSpawn - Start");
	//zoneupdates[playerid] = 1;
	//player_zone[playerid] = -1;
	//if(!zoneupdate) zoneupdate = SetTimer("update_zones",1000,4);
	Debug("world.inc > World_OnPlayerSpawn - Stop");
}

public World_OnGameModeInit(){
	ZonesLoadAll();
	SpawnWorld();

 	if ((Verbose_Save == 1) || (Verbose_Save == -1))
	{
	    WorldSave();
		if (Verbose_Save == -1) { Verbose_Save = 0; db_Set(ConfigDB,"Verbose_Save",ovalstr(Verbose_Save));}
	}
	TimerWorldSave = SetTimer("WorldSave", WorldSaveDelay, 1);
	TimerSavePlayers = SetTimer("SavePlayersData",120000,1); //2 mins
	TimerSyncActiveCP = SetTimer("SyncActiveCP", 2000, 1);
	return 0;
}