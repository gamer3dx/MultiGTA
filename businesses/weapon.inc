//Weapon Business module.
//Created by MadCat
//Based on Original Business Handler from GTO, created by Iain Gilbert
//Coords from SA-MP forum

#include "base"
#include "weapons"

new armdinfoshowed[MAX_PLAYERS] = 0;

new ArmsDealerBusinessDB[MAX_STRING] = "MultiGTA/Businesses/";

#define MAX_WEAPONS 50
#define MAX_BULLETS 999

#define PACK_COST 1500

forward ArmDealers_OnGameModeInit();
forward IsPlayerAtArmDealer(playerid);
forward ArmDealerPickupCheck(playerid);
forward BuyWeapon(playerid,weaponid,bullets);
forward GetPlayerArmDealerID(playerid);
forward BuyArmDealer(playerid);
forward CollectArmDealer(playerid);
forward SellArmDealer(playerid);
forward BuyWeaponFromDealer(playerid);

enum ArmDealerInfo {
    Name[MAX_STRING],
    Float:Coord_X,
    Float:Coord_Y,
    Float:Coord_Z,
    Level,
    Cost,
    MoneyNow,
    PickupModel,
    WeaponSell,
    Bullets,
    ItemsInStock,
    Components,
    Owner[MAX_NAME]
}

#define MAX_ARMDEALERS 162
new Armdealers[MAX_ARMDEALERS][ArmDealerInfo] = {
	{"ArmsDealer - Weapon Pack - Palomino Creek",2335.3181,63.7071,26.4828,10,100000,0,PICKUP_PACK,WEAPON_PACK,0,5000,0,"Server"},
	{"ArmsDealer - Weapon Pack - (LS)Market",1365.3409,-1282.4238,13.5469,10,100000,0,PICKUP_PACK,WEAPON_PACK,0,5000,0,"Server"},
	{"ArmsDealer - Weapon Pack - LV",2157.9685,940.0720,10.8203,10,100000,0,PICKUP_PACK,WEAPON_PACK,0,5000,0,"Server"},
	{"ArmsDealer - Weapon Pack - (LV)Old Venturas Strip",2537.6448,2081.0051,10.8203,10,100000,0,PICKUP_PACK,WEAPON_PACK,0,5000,0,"Server"},
	{"ArmsDealer - Weapon Pack - (SF)Ocean Flats",-2630.0916,209.7846,4.5073,10,100000,0,PICKUP_PACK,WEAPON_PACK,0,5000,0,"Server"},
	{"ArmsDealer - Weapon Pack - BlueBerry",240.2100,-181.3865,1.5781,10,100000,0,PICKUP_PACK,WEAPON_PACK,0,5000,0,"Server"},
	{"ArmsDealer - Weapon Pack - El Quebrados",-1510.3710,2609.0706,55.8359,10,100000,0,PICKUP_PACK,WEAPON_PACK,0,5000,0,"Server"},
	{"ArmsDealer - Weapon Pack - Fort Carson",-314.2716,828.0305,14.2422,10,100000,0,PICKUP_PACK,WEAPON_PACK,0,5000,0,"Server"},
	{"ArmsDealer - Weapon Pack - Bone County",778.7179,1874.4076,4.9257,10,100000,0,PICKUP_PACK,WEAPON_PACK,0,5000,0,"Server"},
	{"ArmsDealer - Weapon Pack - Angel Pine",-2095.2671,-2461.4656,30.6250,10,100000,0,PICKUP_PACK,WEAPON_PACK,0,5000,0,"Server"},
	{"ArmsDealer - Weapon Pack - (LS)Willowfield",2397.7991,-1980.6862,13.5469,10,100000,0,PICKUP_PACK,WEAPON_PACK,0,5000,0,"Server"},
	{"ArmsDealer - Molotov - (LV)Creek",2832.0, 2405.0, 18.0,3,20000,0,PICKUP_MOLOTOV,WEAPON_MOLTOV,10,5000,0,"Server"},
	{"ArmsDealer - FlameThrower - (LV)K.A.C.C. Military Fuels",2649.0, 2733.0, 11.0,16,100000,0,PICKUP_FLAMETHROWER,WEAPON_FLAMETHROWER,2000,5000,0,"Server"},
	{"ArmsDealer - Fire Extinguisher - (LV)Spinybed",2148.0, 2721.0, 11.0,5,14000,0,PICKUP_FIREEXTINGUISHER,WEAPON_FIREEXTINGUISHER,3000,5000,0,"Server"},
	{"ArmsDealer - Shotgun - (LV)Pilson Intersection",1345.0,2367.0,11.0,1,12000,0,PICKUP_SHOTGUN,WEAPON_SHOTGUN,30,5000,0,"Server"},
	{"ArmsDealer - AK47 - (LV)Redsands West",1625.0,1944.0,11.0,18,70000,0,PICKUP_AK47,WEAPON_AK47,60,5000,0,"Server"},
	{"ArmsDealer - Sawed Off Shotgun - (LV)Redsands West",1569.0,2150.0,11.0,4,16000,0,PICKUP_SAWEDOFF,WEAPON_SAWEDOFF,25,5000,0,"Server"},
	{"ArmsDealer - Sniper Rifle - (LV)Julius Thruway North",2225.0,2530.0,17.0,7,100000,0,PICKUP_SNIPER,WEAPON_SNIPER,20,5000,0,"Server"},
	{"ArmsDealer - Sniper Rifle - (LV)The Clowns Pocket",2337.0,1806.0,72.0,7,100000,0,PICKUP_SNIPER,WEAPON_SNIPER,20,5000,0,"Server"},
	{"ArmsDealer - M4 - (LV)Pilgrim",2575.0,1562.0,16.0,14,90000,0,PICKUP_M4,WEAPON_M4,70,5000,0,"Server"},
	{"ArmsDealer - MP5 - (LV)Come-A-Lot",2243.0,1132.0,11.0,9,40000,0,PICKUP_MP5,WEAPON_MP5,70,5000,0,"Server"},
	{"ArmsDealer - Tec9 - Red County 1",1761.0,591.0,10.0,7,6000,0,PICKUP_TEC9,WEAPON_TEC9,50,5000,0,"Server"},
	{"ArmsDealer - AK47 - (LV)The Four Dragons Casino",1923.0,1011.0,22.0,18,70000,0,PICKUP_AK47,WEAPON_AK47,60,5000,0,"Server"},
	{"ArmsDealer - Combat Shotgun - (LV)LVA Freight Depot",1407.0,1098.0,11.0,12,20000,0,PICKUP_SHOTGSPA,WEAPON_SHOTGSPA,50,5000,0,"Server"},
	{"ArmsDealer - MicroUzi - (LV)Redsands West",1446.35,1900.03,11.0,5,10000,0,PICKUP_UZI,WEAPON_UZI,60,5000,0,"Server"},
	{"ArmsDealer - Silenced Pistol - (LV)Blackfield",1098.0,1681.0,7.0,2,12000,0,PICKUP_SILENCED,WEAPON_SILENCED,30,5000,0,"Server"},
	{"ArmsDealer - Sawed Off Shotgun - (LV)Whitewood Estates",924.0,2138.0,11.0,4,16000,0,PICKUP_SAWEDOFF,WEAPON_SAWEDOFF,25,5000,0,"Server"},
	{"ArmsDealer - Grenade - (LV)Rockshore East",2809.0, 864.0, 21.0,3,20000,0,PICKUP_GRENADE,WEAPON_GRENADE,10,5000,0,"Server"},
	{"ArmsDealer - Molotov - (LV)Harry Gold Parkway",1781.0, 2072.0, 11.0,3,20000,0,PICKUP_MOLOTOV,WEAPON_MOLTOV,10,5000,0,"Server"},
	{"ArmsDealer - Molotov - (LV)Come-A-Lot",2478.0, 1182.0, 22.0,3,20000,0,PICKUP_MOLOTOV,WEAPON_MOLTOV,10,5000,0,"Server"},
	{"ArmsDealer - Pool cue - (LV)Linden Side",2854.0, 944.0, 11.0,0,4000,0,PICKUP_POOLSTICK,WEAPON_POOLSTICK,1,5000,0,"Server"},
	{"ArmsDealer - Nitestick - (LV)Roca Escalante",2241.0, 2425.0, 11.0,0,10000,0,PICKUP_NITESTICK,WEAPON_NITESTICK,1,5000,0,"Server"},
	{"ArmsDealer - Golf Club - LV",1418.0, 2774.0, 15.0,0,5000,0,PICKUP_GOLFCLUB,WEAPON_GOLFCLUB,1,5000,0,"Server"},
	{"ArmsDealer - Chainsaw - (LV)Whitewood Estates",1061.0, 2074.0, 11.0,6,100000,0,PICKUP_CHAINSAW,WEAPON_CHAINSAW,1,5000,0,"Server"},
	{"ArmsDealer - Katana - (LV)Pirates in Men's Pants",2000.0, 1526.0, 15.0,5,70000,0,PICKUP_KATANA,WEAPON_KATANA,1,5000,0,"Server"},
	{"ArmsDealer - Shovel - (LV)Pirates in Men's Pants",1997.0, 1658.0, 12.0,0,2000,0,PICKUP_SHOVEL,WEAPON_SHOVEL,1,5000,0,"Server"},
	{"ArmsDealer - Pistol - (LS)Ganton",2538.0,-1630.0,14.0,0,4000,0,PICKUP_COLT45,WEAPON_COLT45,35,5000,0,"Server"},
	{"ArmsDealer - Micro Uzi - (LS)Ganton",2551.33,-1740.0,6.49,5,10000,0,PICKUP_UZI,WEAPON_UZI,60,5000,0,"Server"},
	{"ArmsDealer - Sawed Off Shotgun - (LS)East Los Santos",2428.0,-1214.0,36.0,4,16000,0,PICKUP_SAWEDOFF,WEAPON_SAWEDOFF,25,5000,0,"Server"},
	{"ArmsDealer - Desert Eagle - (LS)Ocean Docks",2766.0,-2182.0,11.0,3,24000,0,PICKUP_DEAGLE,WEAPON_DEAGLE,30,5000,0,"Server"},
	{"ArmsDealer - Grenade - (LS)Idlewood",2142.0, -1804.0, 16.0,3,20000,0,PICKUP_GRENADE,WEAPON_GRENADE,10,5000,0,"Server"},
	{"ArmsDealer - MP5 - (LS)El Corona",1764.0,-1930.0,14.0,9,40000,0,PICKUP_MP5,WEAPON_MP5,70,5000,0,"Server"},
	{"ArmsDealer - Silenced Pistol - (LS)Conference Center",1214.0,-1816.0,17.0,2,12000,0,PICKUP_SILENCED,WEAPON_SILENCED,30,5000,0,"Server"},
	{"ArmsDealer - MP5 - (LS)Las Colinas",2266.0,-1028.0,59.0,9,40000,0,PICKUP_MP5,WEAPON_MP5,70,5000,0,"Server"},
	{"ArmsDealer - Sniper Rifle - (LS)Jefferson",2047.0,-1406.0,68.0,7,100000,0,PICKUP_SNIPER,WEAPON_SNIPER,20,5000,0,"Server"},
	{"ArmsDealer - AK47 - (LS)Mulholland",1308.97,-874.4,40.0,18,70000,0,PICKUP_AK47,WEAPON_AK47,60,5000,0,"Server"},
	{"ArmsDealer - Rifle - Red County",1102.0,-661.0,114.0,8,20000,0,PICKUP_RIFLE,WEAPON_RIFLE,30,5000,0,"Server"},
	{"ArmsDealer - Tec9 - (LS)Mulholland",899.8012,-792.078,102.0,7,6000,0,PICKUP_TEC9,WEAPON_TEC9,50,5000,0,"Server"},
	{"ArmsDealer - Pistol - (LS)Santa Maria Beach",338.0,-1875.0,4.0,0,4000,0,PICKUP_COLT45,WEAPON_COLT45,35,5000,0,"Server"},
	{"ArmsDealer - Molotov - (LS)Vinewood",886.0, -966.0, 37.0,3,20000,0,PICKUP_MOLOTOV,WEAPON_MOLTOV,10,5000,0,"Server"},
	{"ArmsDealer - Tec9 - (LS)Los Santos International",1408.0,-2380.0,14.0,7,6000,0,PICKUP_TEC9,WEAPON_TEC9,50,5000,0,"Server"},
	{"ArmsDealer - M4 - LS",1379.0,-2547.0,14.0,14,90000,0,PICKUP_M4,WEAPON_M4,70,5000,0,"Server"},
	{"ArmsDealer - Molotov - (LS)East Los Santos",2426.0, -1416.0, 24.0,3,20000,0,PICKUP_MOLOTOV,WEAPON_MOLTOV,10,5000,0,"Server"},
	{"ArmsDealer - Golf Club - LS",1457.0, -792.0, 90.0,0,5000,0,PICKUP_GOLFCLUB,WEAPON_GOLFCLUB,1,5000,0,"Server"},
	{"ArmsDealer - Chainsaw - (LS)Ocean Docs",2371.0, -2543.0, 3.0,6,100000,0,PICKUP_CHAINSAW,WEAPON_CHAINSAW,1,5000,0,"Server"},
	{"ArmsDealer - Knife - (LS)Market",1124.0, -1335.0, 13.0,0,1000,0,PICKUP_KNIFE,WEAPON_KNIFE,1,5000,0,"Server"},
	{"ArmsDealer - Molotov - (LS)Los Santos International",2197.0, -2475.0, 14.0,3,20000,0,PICKUP_MOLOTOV,WEAPON_MOLTOV,10,5000,0,"Server"},
	{"ArmsDealer - Katana - (LS)El Corona",1862.0, -1862.0, 14.0,5,70000,0,PICKUP_KATANA,WEAPON_KATANA,1,5000,0,"Server"},
	{"ArmsDealer - Brass Knuckle - (LS)Commerce",1339.0, -1765.0, 14.0,0,5000,0,PICKUP_BRASS_KNUCKLES,WEAPON_BRASSKNUCKLE,1,5000,0,"Server"},
	{"ArmsDealer - Chainsaw - (LS)Willowfield",2192.243, -1988.751, 13.4185,6,100000,0,PICKUP_CHAINSAW,WEAPON_CHAINSAW,1,5000,0,"Server"},
	{"ArmsDealer - Shovel - (LS)Ganton",2459.0, -1708.0, 13.6,0,2000,0,PICKUP_SHOVEL,WEAPON_SHOVEL,1,5000,0,"Server"},
	{"ArmsDealer - Grenade - (LS)Las Colinas",2441.0, -1013.0, 54.0,3,20000,0,PICKUP_GRENADE,WEAPON_GRENADE,10,5000,0,"Server"},
	{"ArmsDealer - MP5 - (SF)Ocean Flats",-2678.0,-128.0,4.0,9,40000,0,PICKUP_MP5,WEAPON_MP5,70,5000,0,"Server"},
	{"ArmsDealer - Desert Eagle - (SF)Garcia",-2212.0,109.0,35.0,3,24000,0,PICKUP_DEAGLE,WEAPON_DEAGLE,30,5000,0,"Server"},
	{"ArmsDealer - M4 - (SF)Palisades",-2903.0,784.0,35.0,14,90000,0,PICKUP_M4,WEAPON_M4,70,5000,0,"Server"},
	{"ArmsDealer - Tec9 - (SF)Battery Point",-2665.0,1452.0,7.0,7,6000,0,PICKUP_TEC9,WEAPON_TEC9,50,5000,0,"Server"},
	{"ArmsDealer - Pistol - (SF)Garcia",-2206.0,-23.0,35.0,0,4000,0,PICKUP_COLT45,WEAPON_COLT45,35,5000,0,"Server"},
	{"ArmsDealer - Shotgun - (SF)Doherty",-1841.106,-74.2171,14.7606,1,12000,0,PICKUP_SHOTGUN,WEAPON_SHOTGUN,30,5000,0,"Server"},
	{"ArmsDealer - FlameThrower - (SF)Easter Basin",-1579.0,29.45,17.0,16,100000,0,PICKUP_FLAMETHROWER,WEAPON_FLAMETHROWER,2000,5000,0,"Server"},
	{"ArmsDealer - Rifle - (SF)Foster Valley",-2094.0,-488.0,36.0,8,20000,0,PICKUP_RIFLE,WEAPON_RIFLE,30,5000,0,"Server"},
	{"ArmsDealer - AK47 - (SF)Foster Valley",-1968.0,-923.0,32.0,18,70000,0,PICKUP_AK47,WEAPON_AK47,60,5000,0,"Server"},
	{"ArmsDealer - Fire Extinguisher - (SF)Easter Basin",-1700.0, 415.0, 7.0,5,14000,0,PICKUP_FIREEXTINGUISHER,WEAPON_FIREEXTINGUISHER,3000,5000,0,"Server"},
	{"ArmsDealer - MicroUzi - (SF)Esplanade North",-1679.0,1410.0,7.0,5,10000,0,PICKUP_UZI,WEAPON_UZI,60,5000,0,"Server"},
	{"ArmsDealer - FlameThrower - (SF)Doherty",-2132.52,189.2507,35.5379,16,100000,0,PICKUP_FLAMETHROWER,WEAPON_FLAMETHROWER,2000,5000,0,"Server"},
	{"ArmsDealer - Tec9 - (SF)Calton Heights",-2092.0,1121.0,54.0,7,6000,0,PICKUP_TEC9,WEAPON_TEC9,50,5000,0,"Server"},
	{"ArmsDealer - Sniper Rifle - (SF)Downtown",-1629.0,1167.0,24.0,7,100000,0,PICKUP_SNIPER,WEAPON_SNIPER,20,5000,0,"Server"},
	{"ArmsDealer - Chainsaw - (SF)Kings",-2083.0, 298.0, 42.0,6,100000,0,PICKUP_CHAINSAW,WEAPON_CHAINSAW,1,5000,0,"Server"},
	{"ArmsDealer - Baseball Bat - (SF)Garcia",-2306.0, 93.0, 35.0,0,2000,0,PICKUP_BAT,WEAPON_BAT,1,5000,0,"Server"},
	{"ArmsDealer - Shovel - (SF)Ocean Flats",-2796.416, 123.686, 6.844,0,2000,0,PICKUP_SHOVEL,WEAPON_SHOVEL,1,5000,0,"Server"},
	{"ArmsDealer - Pool cue - (SF)Doherty",-2135.0, 197.0, 35.0,0,4000,0,PICKUP_POOLSTICK,WEAPON_POOLSTICK,1,5000,0,"Server"},
	{"ArmsDealer - Katana - (SF)Chinatown",-2208.0, 696.0, 50.0,5,70000,0,PICKUP_KATANA,WEAPON_KATANA,1,5000,0,"Server"},
	{"ArmsDealer - BrassKnuckle - (SF)Calton Heights",-2206.0, 961.0, 80.0,0,5000,0,PICKUP_BRASS_KNUCKLES,WEAPON_BRASSKNUCKLE,1,5000,0,"Server"},
	{"ArmsDealer - Nitestick - (SF)Doherty",-2222.0, -302.0, 43.0,0,10000,0,PICKUP_NITESTICK,WEAPON_NITESTICK,1,5000,0,"Server"},
	{"ArmsDealer - Knife - (SF)Downtown",-1871.0, 351.0, 26.0,0,1000,0,PICKUP_KNIFE,WEAPON_KNIFE,1,5000,0,"Server"},
	{"ArmsDealer - Golf Club - (SF)Avispa Country Club",-2715.0, -314.0, 7.0,0,5000,0,PICKUP_GOLFCLUB,WEAPON_GOLFCLUB,1,5000,0,"Server"},
	{"ArmsDealer - Chainsaw - (SF)Garcia",-2359.0, -82.0, 35.0,6,100000,0,PICKUP_CHAINSAW,WEAPON_CHAINSAW,1,5000,0,"Server"},
	{"ArmsDealer - Desert Eagle - Whetstone",-1870.0,-1625.0,22.0,3,24000,0,PICKUP_DEAGLE,WEAPON_DEAGLE,30,5000,0,"Server"},
	{"ArmsDealer - Fire Extinguisher - Whetstone",-1627.0, -2692.0, 49.0,5,14000,0,PICKUP_FIREEXTINGUISHER,WEAPON_FIREEXTINGUISHER,3000,5000,0,"Server"},
	{"ArmsDealer - MicroUzi - Angel Pine",-2038.0,-2562.0,31.0,5,10000,0,PICKUP_UZI,WEAPON_UZI,60,5000,0,"Server"},
	{"ArmsDealer - Rifle - Back o Beyond",-1035.0,-2258.0,70.0,8,20000,0,PICKUP_RIFLE,WEAPON_RIFLE,30,5000,0,"Server"},
	{"ArmsDealer - Sawed Off Shotgun - Palomino Creek",2366.0,23.0,28.0,4,16000,0,PICKUP_SAWEDOFF,WEAPON_SAWEDOFF,25,5000,0,"Server"},	
	{"ArmsDealer - Molotov - Palomino Creek",2255.0, -74.0, 32.0,3,20000,0,PICKUP_MOLOTOV,WEAPON_MOLTOV,10,5000,0,"Server"},
	{"ArmsDealer - MP5 - Montgomery",1296.0,392.0,20.0,9,40000,0,PICKUP_MP5,WEAPON_MP5,70,5000,0,"Server"},
	{"ArmsDealer - Tec9 - Red County 2",262.0,38.0,2.0,7,6000,0,PICKUP_TEC9,WEAPON_TEC9,50,5000,0,"Server"},
	{"ArmsDealer - AK47 - Red County",-121.0,-232.0,1.0,18,70000,0,PICKUP_AK47,WEAPON_AK47,60,5000,0,"Server"},
	{"ArmsDealer - Shovel - The Panopticon",-532.0, -106.0, 63.0,0,2000,0,PICKUP_SHOVEL,WEAPON_SHOVEL,1,5000,0,"Server"},
	{"ArmsDealer - M4 - Bone County",113.0,1811.0,18.0,14,90000,0,PICKUP_M4,WEAPON_M4,70,5000,0,"Server"},
	{"ArmsDealer - Desert Eagle - Lil Probe Inn",36.0,1372.0,9.0,3,24000,0,PICKUP_DEAGLE,WEAPON_DEAGLE,30,5000,0,"Server"},
	{"ArmsDealer - Shotgun - Fort Carson",24.0,969.0,20.0,1,12000,0,PICKUP_SHOTGUN,WEAPON_SHOTGUN,30,5000,0,"Server"},
	{"ArmsDealer - Molotov - Fort Carson",-170.0, 1025.0, 20.0,3,20000,0,PICKUP_MOLOTOV,WEAPON_MOLTOV,10,5000,0,"Server"},
	{"ArmsDealer - Pistol - Tierra Robada",-639.0,1181.0,13.0,0,4000,0,PICKUP_COLT45,WEAPON_COLT45,35,5000,0,"Server"},
	{"ArmsDealer - AK47 - Tierra Robada",-585.0,2714.0,72.0,18,70000,0,PICKUP_AK47,WEAPON_AK47,60,5000,0,"Server"},
	{"ArmsDealer - Fire Extinguisher - Valle Ocultado",-742.0, 2752.0, 47.0,5,14000,0,PICKUP_FIREEXTINGUISHER,WEAPON_FIREEXTINGUISHER,3000,5000,0,"Server"},
	{"ArmsDealer - Shotgun - Valle Ocultado",-932.02,2649.92,42.0,1,12000,0,PICKUP_SHOTGUN,WEAPON_SHOTGUN,30,5000,0,"Server"},
	{"ArmsDealer - MP5 - El Quebrados",-1474.0,2577.0,56.0,9,40000,0,PICKUP_MP5,WEAPON_MP5,70,5000,0,"Server"},
	{"ArmsDealer - Desert Eagle - Bayside Marina",-2352.0,2456.0,6.0,3,24000,0,PICKUP_DEAGLE,WEAPON_DEAGLE,30,5000,0,"Server"},
	{"ArmsDealer - Grenade - Bayside",-2520.0, 2293.0, 5.0,3,20000,0,PICKUP_GRENADE,WEAPON_GRENADE,10,5000,0,"Server"},
	{"ArmsDealer - FlameThrower - Shady Creeks",-1358.0,-2115.0,30.0,16,100000,0,PICKUP_FLAMETHROWER,WEAPON_FLAMETHROWER,2000,5000,0,"Server"},
	{"ArmsDealer - MicroUzi - Verdant Meadows",119.0,2409.0,17.0,5,10000,0,PICKUP_UZI,WEAPON_UZI,60,5000,0,"Server"},
	{"ArmsDealer - Shovel - Whetstone",-1809.0, -1662.0, 24.0,0,2000,0,PICKUP_SHOVEL,WEAPON_SHOVEL,1,5000,0,"Server"},
	{"ArmsDealer - Golf Club - Angel Pine",-2227.0, -2401.0, 31.4,0,5000,0,PICKUP_GOLFCLUB,WEAPON_GOLFCLUB,1,5000,0,"Server"},
	{"ArmsDealer - Shovel - Palomino Creek", 2240.0, -83.0, 27.0,0,2000,0,PICKUP_SHOVEL,WEAPON_SHOVEL,1,5000,0,"Server"},
	{"ArmsDealer - Pool cue - Red County", 294.0, -188.0, 2.0,0,4000,0,PICKUP_POOLSTICK,WEAPON_POOLSTICK,1,5000,0,"Server"},
	{"ArmsDealer - Chainsaw - The Panopticon",-761.0, -126.0, 66.0,6,100000,0,PICKUP_CHAINSAW,WEAPON_CHAINSAW,1,5000,0,"Server"},
	{"ArmsDealer - Katana - El Quebrados",-1568.0, 2718.0, 56.0,5,70000,0,PICKUP_KATANA,WEAPON_KATANA,1,5000,0,"Server"},
	{"ArmsDealer - Dildo - Bayside",-2401.0, 2360.0, 5.0,0,20000,0,PICKUP_DILDO,WEAPON_DILDO,1,5000,0,"Server"},
	{"ArmsDealer - Shovel - Hunter Quarry",637.0, 832.0, -43.0,0,2000,0,PICKUP_SHOVEL,WEAPON_SHOVEL,1,5000,0,"Server"},
	{"ArmsDealer - Chainsaw - Hunter Quarry",680.0, 826.0, -39.0,6,100000,0,PICKUP_CHAINSAW,WEAPON_CHAINSAW,1,5000,0,"Server"},
	{"ArmsDealer - Chainsaw - Red County",752.0, 260.0, 27.0,6,100000,0,PICKUP_CHAINSAW,WEAPON_CHAINSAW,1,5000,0,"Server"},
	{"ArmsDealer - Brass Knuckle - Las Payasadas",-246.0, 2725.0, 63.0,0,5000,0,PICKUP_BRASS_KNUCKLES,WEAPON_BRASSKNUCKLE,1,5000,0,"Server"},
	{"ArmsDealer - Knife - El Castillo del Diablo",-23.0, 2322.0, 24.0,0,1000,0,PICKUP_KNIFE,WEAPON_KNIFE,1,5000,0,"Server"},
	{"ArmsDealer - AK47 - (LS)Ocean Docs",2129.4,-2280.71,14.42,18,70000,0,PICKUP_AK47,WEAPON_AK47,60,5000,0,"Server"},
	{"ArmsDealer - Parachute - (SF)Downtown",-1542.857, 698.4825, 139.2658,4,20000,0,PICKUP_PARA,WEAPON_PARACHUTE,1,5000,0,"Server"},
	{"ArmsDealer - MP5 - (LS)Jefferson",2198.11,-1170.22,33.5,9,40000,0,PICKUP_MP5,WEAPON_MP5,130,5000,0,"Server"},
	{"ArmsDealer - Sniper Rifle - (LS)Mulholland",935.744,-926.0453,57.7642,7,100000,0,PICKUP_SNIPER,WEAPON_SNIPER,20,5000,0,"Server"},
	{"ArmsDealer - Shovel - Fern Ridge",842.9783, -17.3791, 64.2,0,2000,0,PICKUP_SHOVEL,WEAPON_SHOVEL,1,5000,0,"Server"},
	{"ArmsDealer - Chainsaw - (SF)Avispa Country Club",-2752.243, -272.2891, 6.5956,6,100000,0,PICKUP_CHAINSAW,WEAPON_CHAINSAW,1,5000,0,"Server"},
	{"ArmsDealer - Cane - (SF)Ocean Flats 1",-2617.473, -97.0801, 4.003,0,1500,0,PICKUP_CANE,WEAPON_CANE,1,5000,0,"Server"},
	{"ArmsDealer - Cane - (SF)Ocean Flats 2",-2777.192, -25.2984, 6.8721,0,1500,0,PICKUP_CANE,WEAPON_CANE,1,5000,0,"Server"},
	{"ArmsDealer - Cane - (SF)Ocean Flats 3",-2774.113, 87.8845, 6.7987,0,1500,0,PICKUP_CANE,WEAPON_CANE,1,5000,0,"Server"},
	{"ArmsDealer - Cane - (SF)City Hall",-2770.624, 389.0772, 4.2818,0,1500,0,PICKUP_CANE,WEAPON_CANE,1,5000,0,"Server"},
	{"ArmsDealer - Katana - (SF)Hashbury",-2535.631, 51.7034, 8.6512,5,70000,0,PICKUP_KATANA,WEAPON_KATANA,1,5000,0,"Server"},
	{"ArmsDealer - Cane - (SF)Hashbury",-2530.958, -34.1009, 25.2855,0,1500,0,PICKUP_CANE,WEAPON_CANE,1,5000,0,"Server"},
	{"ArmsDealer - Cane - (SF)Downtown",-1691.649, 946.7679, 24.8084,0,1500,0,PICKUP_CANE,WEAPON_CANE,1,5000,0,"Server"},
	{"ArmsDealer - Cane - (SF)Santa Flora",-2664.518, 636.5673, 14.2474,0,1500,0,PICKUP_CANE,WEAPON_CANE,1,5000,0,"Server"},
	{"ArmsDealer - FlameThrower - Flint County",-601.4012,-1068.6,23.6667,16,100000,0,PICKUP_FLAMETHROWER,WEAPON_FLAMETHROWER,2000,5000,0,"Server"},
	{"ArmsDealer - Cane - Beacon Hill",-377.2184, -1048.053, 58.9125,0,1500,0,PICKUP_CANE,WEAPON_CANE,1,5000,0,"Server"},
	{"ArmsDealer - Cane - Flint County",-45.5928, -1148.529, 1.3953,0,1500,0,PICKUP_CANE,WEAPON_CANE,1,5000,0,"Server"},
	{"ArmsDealer - Brass Knuckle - (LS)Ganton",2428.499, -1679.27, 13.1633,0,5000,0,PICKUP_BRASS_KNUCKLES,WEAPON_BRASSKNUCKLE,1,5000,0,"Server"},
	{"ArmsDealer - Grenade - (LS)East Beach",2820.013, -1426.519, 23.805,3,20000,0,PICKUP_GRENADE,WEAPON_GRENADE,10,5000,0,"Server"},
	{"ArmsDealer - MicroUzi - (LS)East Beach",2790.343,-1427.489,39.6258,5,10000,0,PICKUP_UZI,WEAPON_UZI,60,5000,0,"Server"},
	{"ArmsDealer - Tec9 - (LS)Las Colinas",2574.065,-1134.201,64.6535,7,6000,0,PICKUP_TEC9,WEAPON_TEC9,50,5000,0,"Server"},
	{"ArmsDealer - Pistol - (LS)Las Colinas",2423.892,-1117.452,41.2464,0,4000,0,PICKUP_COLT45,WEAPON_COLT45,35,5000,0,"Server"},
	{"ArmsDealer - Cane - (LS)Temple",1296.155, -1081.892, 26.1502,0,1500,0,PICKUP_CANE,WEAPON_CANE,1,5000,0,"Server"},
	{"ArmsDealer - Cane - (LS)Mulholland",1390.611, -800.4332, 81.7795,0,1500,0,PICKUP_CANE,WEAPON_CANE,1,5000,0,"Server"},
	{"ArmsDealer - Baseball Bat - (LV)Redsands West",1308.466, 2111.289, 10.7221,0,2000,0,PICKUP_BAT,WEAPON_BAT,1,5000,0,"Server"},
	{"ArmsDealer - Cane - (LV)The Emerald Isle",2183.116, 2396.827, 10.7722,0,1500,0,PICKUP_CANE,WEAPON_CANE,1,5000,0,"Server"},
	{"ArmsDealer - Baseball Bat - (LV)Blackfield",1081.133, 1603.697, 5.6,0,2000,0,PICKUP_BAT,WEAPON_BAT,1,5000,0,"Server"},
	{"ArmsDealer - Knife - Bone County",777.8668, 1948.123, 5.3634,0,1000,0,PICKUP_KNIFE,WEAPON_KNIFE,1,5000,0,"Server"},
	{"ArmsDealer - Sawed Off Shotgun - (LV)Las Venturas Airport",1706.352,1242.019,34.2952,4,16000,0,PICKUP_SAWEDOFF,WEAPON_SAWEDOFF,25,5000,0,"Server"},
	{"ArmsDealer - Cane - (LV)Prickle Pine 1",1420.945, 2519.882, 10.6199,0,1500,0,PICKUP_CANE,WEAPON_CANE,1,5000,0,"Server"},
	{"ArmsDealer - Cane - (LV)Prickle Pine 2",1372.996, 2605.758, 10.8776,0,1500,0,PICKUP_CANE,WEAPON_CANE,1,5000,0,"Server"},
	{"ArmsDealer - MP5 - (LV)Starfish Casino",2293.686,1982.286,31.4335,9,40000,0,PICKUP_MP5,WEAPON_MP5,70,5000,0,"Server"},
	{"ArmsDealer - Katana - (LV)Pilgrim",2631.263, 1722.395, 11.0312,5,70000,0,PICKUP_KATANA,WEAPON_KATANA,1,5000,0,"Server"},
	{"ArmsDealer - Cane - (LV)Pilgrim",2490.497, 1522.47, 10.576,0,1500,0,PICKUP_CANE,WEAPON_CANE,1,5000,0,"Server"},
	{"ArmsDealer - Cane - (LS)Rodeo",455.4583, -1485.896, 30.9717,0,1500,0,PICKUP_CANE,WEAPON_CANE,1,5000,0,"Server"},
	{"ArmsDealer - Tec9 - (LS)Ganton",2529.724,-1678.563,19.4225,7,6000,0,PICKUP_TEC9,WEAPON_TEC9,50,5000,0,"Server"},
	{"ArmsDealer - Sawed Off Shotgun - (LS)Ocean Docks",2254.378,-2261.689,14.3751,4,16000,0,PICKUP_SAWEDOFF,WEAPON_SAWEDOFF,15,5000,0,"Server"},
	{"ArmsDealer - Sniper Rifle - (LV)The Four Dragons Casino",2015.744,1004.045,39.1,7,100000,0,PICKUP_SNIPER,WEAPON_SNIPER,20,5000,0,"Server"},
	{"ArmsDealer - Katana - (LV)The Four Dragons Casino",2002.263, 981.3947, 10.5,5,70000,0,PICKUP_KATANA,WEAPON_KATANA,1,5000,0,"Server"},
	{"ArmsDealer - AK47 - (LS)Vinewood",825.921,-1165.813,17.8936,18,70000,0,PICKUP_AK47,WEAPON_AK47,60,5000,0,"Server"},
	{"ArmsDealer - Sawed Off Shotgun - (LS)Vinewood",832.603,-1273.861,14.483,4,16000,0,PICKUP_SAWEDOFF,WEAPON_SAWEDOFF,40,5000,0,"Server"}
};

WeaponBusinessLoadAll()
{
	Debug("businesses/weapons.inc > WeaponBusinessLoadAll - Start");
	
	new temp[MAX_STRING];
	if (!db_Exists(DatabaseDB)) db_Create(DatabaseDB);
	if (!db_Isset(DatabaseDB,"ArmDealers_Business_DB")) db_Set(DatabaseDB,"ArmDealers_Business_DB",ArmsDealerBusinessDB);
	set(temp,db_Get(DatabaseDB,"ArmDealers_Business_DB"));
	if (strlen(temp) > 0) set(ArmsDealerBusinessDB,temp);
	if (!db_Exists(ArmsDealerBusinessDB))
	{

		WeaponBusinessSaveAll();

		return;
	}
	for (new i=0;i<MAX_ARMDEALERS;i++)
	{
		new ArmsDealerBusinessDBname[MAX_STRING];
		format(ArmsDealerBusinessDBname,sizeof(ArmsDealerBusinessDBname),"%s%s.txt",ArmsDealerBusinessDB,Armdealers[i][Name]);
		if (!db_Exists(ArmsDealerBusinessDBname))
		{
			WeaponBusinessSaveAll();
		}
		set(temp,db_Get(ArmsDealerBusinessDBname,"Name"));
		if (strlen(temp) > 0) set(Armdealers[i][Name],temp);
		set(temp,nullstr);
		set(temp,db_Get(ArmsDealerBusinessDBname,"MoneyNow"));
		if (strlen(temp) > 0) Armdealers[i][MoneyNow] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(ArmsDealerBusinessDBname,"ItemsInStock"));
		if (strlen(temp) > 0) Armdealers[i][ItemsInStock] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(ArmsDealerBusinessDBname,"Components"));
		if (strlen(temp) > 0) Armdealers[i][Components] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(ArmsDealerBusinessDBname,"Owner"));
		if (strlen(temp) > 0) set(Armdealers[i][Owner],temp);
		set(temp,nullstr);
	}
	Debug("businesses/weapons.inc > WeaponBusinessLoadAll - Stop");
	return;

}

WeaponBusinessSaveAll()
{
	Debug("businesses/weapons.inc > WeaponBusinessSaveAll  - Start");
	new temp[MAX_STRING];
	if (db_Exists(DatabaseDB))
	{
		db_Set(DatabaseDB,"ArmDealers_Business_DB",ArmsDealerBusinessDB);
	}

	for (new i=0;i<MAX_ARMDEALERS;i++)
	{
		new ArmsDealerBusinessDBname[MAX_STRING];
		format(ArmsDealerBusinessDBname,sizeof(ArmsDealerBusinessDBname),"%s%s.txt",ArmsDealerBusinessDB,Armdealers[i][Name]);
		if (db_Exists(ArmsDealerBusinessDBname)){
			continue;
		}
		db_Create(ArmsDealerBusinessDBname);
		db_BeforeBigSaving(ArmsDealerBusinessDBname);
		db_Set(ArmsDealerBusinessDBname,"Name",Armdealers[i][Name]);
		valstr(temp,Armdealers[i][MoneyNow]);
		db_Set(ArmsDealerBusinessDBname,"MoneyNow",temp);
		set(temp,nullstr);
		valstr(temp,Armdealers[i][ItemsInStock]);
		db_Set(ArmsDealerBusinessDBname,"ItemsInStock",temp);
		set(temp,nullstr);
		valstr(temp,Armdealers[i][Components]);
		db_Set(ArmsDealerBusinessDBname,"Components",temp);
		set(temp,nullstr);
		db_Set(ArmsDealerBusinessDBname,"Owner",Armdealers[i][Owner]);
		db_AfterBigSaving(ArmsDealerBusinessDBname);

	}
	Debug("businesses/weapons.inc > WeaponBusinessSaveAll  - Stop");
	return;
}

public ArmDealers_OnGameModeInit()
{
	Debug("businesses/weapons.inc > ArmDealers_OnGameModeInit - Start");
	new count = 0;
	for (new id=0;id<MAX_ARMDEALERS;id++)
	{
		CreateStreamPickup(Armdealers[id][PickupModel],1,Armdealers[id][Coord_X],Armdealers[id][Coord_Y],Armdealers[id][Coord_Z]+1);
		CreateStreamPickup(1274,1,Armdealers[id][Coord_X],Armdealers[id][Coord_Y],Armdealers[id][Coord_Z]);
		CreateStreamMapIcon(6,Armdealers[id][Coord_X],Armdealers[id][Coord_Y],Armdealers[id][Coord_Z]);
		count++;
	}
	new logstring[256];
	format(logstring,sizeof(logstring),"ArmDealers System Loaded. Armdealers: %d",count);
	WriteLog(logstring);
	Debug("businesses/weapons.inc > ArmDealers_OnGameModeInit - Stop");
}

public IsPlayerAtArmDealer(playerid)
{
	for (new id=0;id<MAX_ARMDEALERS;id++)
	{
		if (IsPlayerInRangeOfPoint(playerid,1.0,Armdealers[id][Coord_X],Armdealers[id][Coord_Y],Armdealers[id][Coord_Z]))
		{
			return 1;
		}
	}
	return 0;
}

public GetPlayerArmDealerID(playerid)
{
	new lookupid=-1;
	for (new armdealersid=0;armdealersid<MAX_ARMDEALERS;armdealersid++)
	{
		if (IsPlayerInRangeOfPoint(playerid,1.0,Armdealers[armdealersid][Coord_X],Armdealers[armdealersid][Coord_Y],Armdealers[armdealersid][Coord_Z])) lookupid = armdealersid;
	}
	return lookupid;
}

public ArmDealerPickupCheck(playerid)
{
	if (playerid == INVALID_PLAYER_ID) return;
	if (!IsPlayerConnected(playerid)) return;
	if (!IsPlayerAtArmDealer(playerid)) {armdinfoshowed[playerid] = 0; return;}
	if (armdinfoshowed[playerid] == 1) return;
	armdinfoshowed[playerid] = 1;
	new id=GetPlayerArmDealerID(playerid);
	SystemMsg(playerid,COLOUR_INFO,gettext(839));
	new tempstr[256];
	format(tempstr,sizeof(tempstr),gettext(840),oGetWeaponName(Armdealers[id][WeaponSell]));
	SystemMsg(playerid,COLOUR_INFO,tempstr);
	if (Armdealers[id][WeaponSell] == WEAPON_PACK){
	SystemMsgFormat(playerid,COLOUR_INFO,gettext(841),PACK_COST);
	SystemMsg(playerid,COLOUR_INFO,gettext(842));
	} else {
	SystemMsgFormat(playerid,COLOUR_INFO,gettext(843),GetWeaponCost(Armdealers[id][WeaponSell]));
	SystemMsgFormat(playerid,COLOUR_INFO,gettext(844),Armdealers[id][Level]);
	}
	if (strcomp(Armdealers[id][Owner],oGetPlayerName(playerid),false) == 1){
	SystemMsgFormat(playerid,COLOUR_INFO,gettext(845),0);
	if (Armdealers[id][MoneyNow] > 0){
	SystemMsgFormat(playerid,COLOUR_INFO,gettext(846),Armdealers[id][MoneyNow]);
	}
	}
	if (strcomp(Armdealers[id][Owner],"Server",false) == 1){
	format(tempstr,sizeof(tempstr),gettext(847),Armdealers[id][Cost]+Armdealers[id][MoneyNow],Armdealers[id][Level]);
	SystemMsg(playerid,COLOUR_INFO,tempstr);
	}

}

public BuyWeapon(playerid,weaponid,bullets)
{
	Debug("businesses/weapons.inc > BuyWeapon - Start");
	new string[MAX_STRING];
	if ((weaponid < 1) || (weaponid > 46))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(848));
		Debug("businesses/weapons.inc > BuyWeapon - Stop");
		return 0;
	}
	if (bullets < 1)
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(849));
		Debug("businesses/weapons.inc > BuyWeapon - Stop");
		return 0;
	}
	if (!IsPlayerAtArmDealer(playerid))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(851));
		Debug("businesses/weapons.inc > BuyWeapon - Stop");
		return 0;
	}

	if (!IsPlayerAllowedWeapon(playerid,weaponid))
	{
		format(string,sizeof(string),gettext(852),oGetWeaponName(weaponid));
		SystemMsg(playerid,COLOUR_ERROR,string);
		Debug("businesses/weapons.inc > BuyWeapon - Stop");
		return 0;
	}
	new purchasecost;
	purchasecost = GetWeaponCost(weaponid);
	if (purchasecost < 1)
	{
		format(string,sizeof(string),gettext(852),oGetWeaponName(weaponid));
		SystemMsg(playerid,COLOUR_ERROR,string);
		Debug("businesses/weapons.inc > BuyWeapon - Stop");
		return 0;
	}
	if (oGetPlayerMoney(playerid) < purchasecost)
	{
		format(string,sizeof(string),gettext(853), purchasecost);
		SystemMsg(playerid,COLOUR_ERROR,string);
		Debug("businesses/weapons.inc > BuyWeapon - Stop");
		return 0;
	}
	new id=GetPlayerArmDealerID(playerid);
	if (strcomp(Armdealers[id][Owner],"Server",false) == 1){
	oGivePlayerMoney(playerid,0-purchasecost,0);
	Armdealers[id][MoneyNow]=Armdealers[id][MoneyNow]+purchasecost;
	Armdealers[id][ItemsInStock]--;
	new dinifilename[MAX_STRING];
	new temp[MAX_STRING];
	format(dinifilename,sizeof(dinifilename),"%s%s.txt",ArmsDealerBusinessDB,Armdealers[id][Name]);
	if (db_Exists(dinifilename))
	{
		valstr(temp,Armdealers[id][MoneyNow]);
		db_Set(dinifilename,"MoneyNow",temp);
		set(temp,nullstr);
		valstr(temp,Armdealers[id][ItemsInStock]);
		db_Set(dinifilename,"ItemsInStock",temp);
		set(temp,nullstr);
	}
	} else if (strcomp(Armdealers[id][Owner],oGetPlayerName(playerid),false) == 1){
	Armdealers[id][ItemsInStock]--;
	new dinifilename[MAX_STRING];
	new temp[MAX_STRING];
	format(dinifilename,sizeof(dinifilename),"%s%s.txt",ArmsDealerBusinessDB,Armdealers[id][Name]);
	if (db_Exists(dinifilename))
	{
		valstr(temp,Armdealers[id][ItemsInStock]);
		db_Set(dinifilename,"ItemsInStock",temp);
		set(temp,nullstr);
	}
	SystemMsg(playerid,COLOUR_OK,gettext(854));
	}
	else
	{
	oGivePlayerMoney(playerid,-purchasecost,0);
	Armdealers[id][MoneyNow]=Armdealers[id][MoneyNow]+purchasecost;
	Armdealers[id][ItemsInStock]--;
	new dinifilename[MAX_STRING];
	new temp[MAX_STRING];
	format(dinifilename,sizeof(dinifilename),"%s%s.txt",ArmsDealerBusinessDB,Armdealers[id][Name]);
	if (db_Exists(dinifilename))
	{
		valstr(temp,Armdealers[id][MoneyNow]);
		db_Set(dinifilename,"MoneyNow",temp);
		set(temp,nullstr);
		valstr(temp,Armdealers[id][ItemsInStock]);
		db_Set(dinifilename,"ItemsInStock",temp);
		set(temp,nullstr);
	}
	}
	oGivePlayerWeapon(playerid,weaponid,bullets);
	if (bullets == 1){
	format(string,sizeof(string),gettext(855),oGetWeaponName(weaponid),purchasecost);
	} else {
	format(string,sizeof(string),gettext(856),oGetWeaponName(weaponid),bullets,purchasecost);
	}
	SystemMsg(playerid, COLOUR_OK, string);
	new logstring[256];
	format(logstring,sizeof(logstring), "player: %d:  %s: bought a weapon %s (id: %d) with %d bullets",playerid,oGetPlayerName(playerid),oGetWeaponName(weaponid),weaponid,bullets);
	WriteLog(logstring);
	Debug("businesses/weapons.inc > BuyWeapon - Stop");
	return 1;
}

public BuyArmDealer(playerid){
Debug("businesses/weapons.inc > BuyArmDealer - Start");
			new id=GetPlayerArmDealerID(playerid);
			new temp[MAX_STRING];
			if (id > -1)
			{
				if (IsPlayerRegistered(playerid))
				{
					if (GetPlayerLevel(playerid) >= Armdealers[id][Level])
					{
						new price;
						price = Armdealers[id][Cost] + Armdealers[id][MoneyNow];
						if(oGetPlayerMoney(playerid) >= price)
						{
							if(strcomp(Armdealers[id][Owner], oGetPlayerName(playerid), true) == 1)
							{
								SystemMsg(playerid,COLOUR_ERROR,gettext(857));
							} else {

								oGivePlayerMoney(playerid,0-price,1);
								set(Armdealers[id][Owner],oGetPlayerName(playerid));
								new dinifilename[MAX_STRING];
								format(dinifilename,sizeof(dinifilename),"%s%s.txt",ArmsDealerBusinessDB,Armdealers[id][Name]);
								if (db_Exists(dinifilename))
								{
									db_Set(dinifilename,"Owner",Armdealers[id][Owner]);
								}
								format(temp,sizeof(temp),gettext(858),Armdealers[id][Name]);
								SystemMsg(playerid,COLOUR_OK,temp);
								SystemMsg(playerid,COLOUR_INFO,gettext(859));
								SystemMsg(playerid,COLOUR_INFO,gettext(860));
								set(temp,nullstr);
								new logstring[256];
								format(logstring, sizeof (logstring), "player: %d:  %s: bought the '%s' (ArmsDealer)",playerid,oGetPlayerName(playerid),Armdealers[id][Name]);
								WriteLog(logstring);

							}
						} else {
							SystemMsg(playerid,COLOUR_ERROR,gettext(861));
						}
					} else {
						format(temp,sizeof(temp),gettext(862),Armdealers[id][Level]);
						SystemMsg(playerid,COLOUR_ERROR,temp);
						set(temp,nullstr);
					}
				} else {
					SystemMsg(playerid,COLOUR_ERROR,gettext(561));
				}
			} else {
				SystemMsg(playerid,COLOUR_ERROR,gettext(863));
			}
			Debug("businesses/weapons.inc > BuyArmDealer - Stop");
			return 0;
		}

public CollectArmDealer(playerid){
			Debug("businesses/weapons.inc > CollectArmDealer - Start");
			new id=GetPlayerArmDealerID(playerid);
			new temp[MAX_STRING];
			if (id > -1)
			{
				if (IsPlayerRegistered(playerid))
				{
					if(strcomp(Armdealers[id][Owner], oGetPlayerName(playerid), true) == 0)
					{
						SystemMsg(playerid,COLOUR_ERROR,gettext(864));
					} else {
						if(Armdealers[id][MoneyNow] > 0)
						{
							oGivePlayerMoney(playerid,0+Armdealers[id][MoneyNow],1);
							SystemMsg(playerid,COLOUR_OK,gettext(865));
							Armdealers[id][MoneyNow] = 0;
							new dinifilename[MAX_STRING];
							format(dinifilename,sizeof(dinifilename),"%s%s.txt",ArmsDealerBusinessDB,Armdealers[id][Name]);
							if (db_Exists(dinifilename))
							{
								valstr(temp,Armdealers[id][MoneyNow]);
								db_Set(dinifilename,"MoneyNow",temp);
								set(temp,nullstr);
							}
						} else {
							SystemMsg(playerid,COLOUR_ERROR,gettext(866));
						}
					}
				} else {
					SystemMsg(playerid,COLOUR_ERROR,gettext(561));
				}
			} else {
				SystemMsg(playerid,COLOUR_ERROR,gettext(863));
			}
			Debug("businesses/weapons.inc > CollectArmDealer - Stop");
			return 0;
}

public SellArmDealer(playerid){
Debug("businesses/weapons.inc > SellArmDealer - Start");
			new id=GetPlayerArmDealerID(playerid);
			new temp[MAX_STRING];
			if (id > -1)
			{
				if (IsPlayerRegistered(playerid))
				{
					if(strcomp(Armdealers[id][Owner], oGetPlayerName(playerid), true) == 0)
					{
						SystemMsg(playerid,COLOUR_ERROR,gettext(864));
					} else {
						new price;
						price = Armdealers[id][Cost] + Armdealers[id][MoneyNow];
						price = (price * 85) / 100;
						oGivePlayerMoney(playerid,0+price,1);
						set(Armdealers[id][Owner],"Server");
						new dinifilename[MAX_STRING];
						format(dinifilename,sizeof(dinifilename),"%s%s.txt",ArmsDealerBusinessDB,Armdealers[id][Name]);
						if (db_Exists(dinifilename))
						{
							db_Set(dinifilename,"Owner","Server");
						}
						format(temp,sizeof(temp),gettext(867),Armdealers[id][Name]);
						SystemMsg(playerid,COLOUR_OK,temp);
						set(temp,nullstr);
						new logstring[256];
						format(logstring, sizeof (logstring), "player: %d:  %s: sold the '%s' (ArmsDealer)",playerid,oGetPlayerName(playerid),Armdealers[id][Name]);
						WriteLog(logstring);
					}
				} else {
					SystemMsg(playerid,COLOUR_ERROR,gettext(561));
				}
			} else {
				SystemMsg(playerid,COLOUR_ERROR,gettext(863));
			}
			Debug("businesses/weapons.inc > SellArmDealer - Stop");
			return 1;
		}

public BuyWeaponFromDealer(playerid)
{
	Debug("businesses/weapons.inc > BuyWeaponFromDealer - Start");
	if (!IsPlayerAtArmDealer(playerid))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(851));
		Debug("businesses/weapons.inc > BuyWeaponFromDealer - Stop");
		return 1;
	}
	new id = GetPlayerArmDealerID(playerid);
	if (Armdealers[id][ItemsInStock] == 0){
		ShowTextForPlayer(playerid, gettext(712), 5000, 1);
		Debug("businesses/weapons.inc > BuyWeaponFromDealer - Stop");
		return 1;
	}
	if (Armdealers[id][WeaponSell] == WEAPON_PACK){
		if (oGetPlayerMoney(playerid) < PACK_COST){
		SystemMsg(playerid,COLOUR_ERROR,gettext(868));
		Debug("businesses/weapons.inc > BuyWeaponFromDealer - Stop");
		return 0;
		}
		if (GetPlayerLevel(playerid) < 5){
		SystemMsg(playerid,COLOUR_ERROR,gettext(869));
		Debug("businesses/weapons.inc > BuyWeaponFromDealer - Stop");
		return 0;
		}
		GivePlayerWeapon(playerid,WEAPON_UZI,150);
		GivePlayerWeapon(playerid,WEAPON_SHOTGUN,40);
		GivePlayerWeapon(playerid,WEAPON_COLT45,90);
		oGivePlayerMoney(playerid,-PACK_COST,1);
		SystemMsg(playerid,COLOUR_OK,gettext(870));
		new logstring[256];
		format(logstring,sizeof(logstring), "player: %d:  %s: bought a emergency weapon pack",playerid,oGetPlayerName(playerid));
		WriteLog(logstring);
	} else {
		BuyWeapon(playerid,Armdealers[id][WeaponSell],Armdealers[id][Bullets]);
	}
	Debug("businesses/weapons.inc > BuyWeaponFromDealer - Stop");
	return 1;
}

