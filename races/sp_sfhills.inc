//
// Converted:   02.08.08
// Aurthor:     Rockstar North Team
// Converter: 	MadCat
//

#if defined _race_sp_sfhills_included
  #endinput
#endif

#define _race_sp_sfhills_included
#pragma library race_sp_sfhills
#include "race"

static racename[MAX_NAME] = "(SP)San-Fierro Hills";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_PHOENIX,V_BUFFALO,V_SULTAN,V_SUPERGT,V_ZR350};
static Float:startlineheading = 0.0;
static Float:racecheckpoints[][CoordInfo] = {
{-1584.141,1053.439,6.8392},
{-1584.128,1150.261,6.8387},
{-1699.077,1312.02,6.8293},
{-1875.891,1352.059,6.8322},
{-2060.741,1274.452,8.0659},
{-2258.306,1257.682,42.6038},
{-2143.281,1075.652,79.1249},
{-2143.141,896.6196,79.6432},
{-2143.084,696.6971,67.6451},
{-2146.737,532.8465,34.8181},
{-2055.564,504.0781,34.8168},
{-1968.175,605.5881,34.8199},
{-1820.393,475.0548,23.4295},
{-1876.903,288.4981,32.6699},
{-1897.585,87.7896,37.9384},
{-1911.151,-214.2394,38.032},
{-1910.974,-515.5727,38.0323},
{-1910.675,-816.3037,44.7432},
{-1910.658,-1117.408,38.0214},
{-1907.276,-1317.626,39.3164},
{-1996.896,-1289.264,37.3518},
{-2153.53,-1031.597,32.3638},
{-2205.567,-859.1885,54.6872},
{-2202.666,-759.2505,62.3413},
{-2352.255,-775.5399,95.1597},
{-2421.544,-610.6656,132.3493},
{-2627.138,-496.4363,70.09},
{-2351.247,-460.0875,79.9485},
{-2599.996,-372.7368,43.7794},
{-2702.314,-530.1367,12.4366},
{-2699.197,-401.205,7.7008},
{-2210.255,-348.0724,36.8384},
{-1897.734,-314.4976,48.9384},
{-1890.36,-45.4288,38.0312},
{-1886.556,55.293,38.0326},
{-1848.038,242.5393,30.825},
{-1736.635,316.5836,6.039},
{-1682.981,367.1342,6.028},
{-1559.293,528.332,6.8409},
{-1535.552,827.4427,6.8401},
{-1584.129,1115.6,6.8376}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_sp_sfhills_init();

public race_sp_sfhills_init() // unique named init function (must be ran on gamemode init)
{
	RaceID = RegisterRace(racename);
	if (RaceID == INVALID_RACE_ID) return;

	Race[RaceID][race_frequency] = racefrequency;
 	Race[RaceID][race_lineupdelay] = lineupdelay;
 	Race[RaceID][race_minracers] = minracers;

	Race[RaceID][race_startheading] = startlineheading;
	set(Race[RaceID][race_vehicles],allowedcars);

	RaceSize[RaceID] = sizeof(racecheckpoints);
	for (new cpid=0;cpid<sizeof(racecheckpoints);cpid++)
	{
		RaceCheckpoints[RaceID][cpid][Coord_X] = racecheckpoints[cpid][Coord_X];
		RaceCheckpoints[RaceID][cpid][Coord_Y] = racecheckpoints[cpid][Coord_Y];
		RaceCheckpoints[RaceID][cpid][Coord_Z] = racecheckpoints[cpid][Coord_Z];
	}

	RaceStats[RaceID][race_timer] = MakeRaceSleepTime(RaceID);
	Race[RaceID][race_cashprize] = CalculateRewardMoney(RaceID); 
	Race[RaceID][race_cashentry] = CalculateEntryFee(RaceID); 
	Race[RaceID][race_xpprize] = CalculateRewardXP(RaceID); 
	Race[RaceID][race_maxracetime] = CalculateMaxRaceTime(RaceID);
    RaceStats[RaceID][race_state] = RACE_STATE_SLEEPING;

}

