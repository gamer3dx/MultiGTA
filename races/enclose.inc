//
// Converted:   26.04.08
// Aurthor:    	Switch
// Converter: 	MadCat
//

#if defined _race_enclose_included
  #endinput
#endif

#define _race_enclose_included
#pragma library race_enclose
#include "race"

static racename[MAX_NAME] = "Enclose";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 174.9599;
static Float:racecheckpoints[][CoordInfo] = {
{2187.4380,1207.0509,10.2941},
{2181.7495,1152.6239,11.4193},
{2139.0605,1073.6729,11.8901},
{2075.8535,1051.5021,10.2872},
{2069.7153,1145.7050,10.3039},
{2051.0618,1207.5472,10.2961},
{2046.2145,1423.3301,10.2961},
{2053.0798,1541.2687,10.2960},
{2049.4795,1701.5417,10.2961},
{2116.5435,1837.6656,10.2961},
{2128.8782,1966.9749,10.3211},
{2127.1077,2118.2327,10.2960},
{2127.8813,2224.1440,10.2959},
{2072.3218,2313.3286,10.2948},
{2028.2240,2408.7466,10.2969},
{2041.2173,2452.1567,10.2949},
{2121.9824,2452.9285,10.2960},
{2226.4033,2437.6785,10.2926},
{2214.7600,2362.1882,10.2961},
{2164.1077,2290.1396,10.2976},
{2147.2456,2122.8555,10.2961},
{2148.2808,1982.2407,10.2987},
{2135.8943,1841.2278,10.3001},
{2085.1406,1772.8593,10.2963},
{2048.5164,1689.2438,10.2961},
{2047.9674,1572.2931,10.2961},
{2048.4014,1506.6959,10.2961},
{2064.2258,1440.9530,10.2962},
{2091.5720,1373.2731,10.2952},
{2149.9819,1373.5031,10.2968},
{2225.5425,1351.5120,10.4188},
{2227.3210,1278.1823,10.2961}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_enclose_init();

public race_enclose_init() // unique named init function (must be ran on gamemode init)
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

