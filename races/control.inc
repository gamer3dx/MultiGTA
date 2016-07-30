//
// Created:     Unknown
// Aurthor:    	Switch
// Converter: 	MadCat
//

#if defined _race_control_included
  #endinput
#endif

#define _race_control_included
#pragma library race_control
#include "race"

forward race_control_init();

static racename[MAX_NAME] = "Control";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 179.8689;
static Float:racecheckpoints[][CoordInfo] = {
	{2907.0249,2411.5366,10.4454},
	{2889.8762,2278.4941,10.4434},
	{2848.9297,2186.9211,10.2949}, 
	{2823.7085,1972.2928,10.3038}, 
	{2736.8684,1951.6503,13.0598}, 
	{2486.1311,1969.1985,10.3321}, 
	{2359.8628,1988.7867,10.2940}, 
	{2317.3884,2021.7556,10.2960}, 
	{2127.8538,2047.0547,10.2950}, 
	{2126.6360,2223.6951,10.2954}, 
	{2028.7001,2399.0110,10.3942}, 
	{2027.0480,2520.7468,12.0368}, 
	{1975.5859,2642.6438,10.2951}, 
	{1891.2207,2708.8679,10.3317}, 
	{1656.5216,2729.1362,10.3361}, 
	{1524.0276,2716.5779,10.3031}, 
	{1307.6930,2590.9280,10.2965}, 
	{1471.9924,2577.8123,10.4046}, 
	{1464.5354,2395.0864,10.3009}, 
	{1378.6266,2296.9993,10.4379}, 
	{1328.9310,2259.0662,10.4445}, 
	{1232.4971,2114.3821,6.3790}, 
	{1232.7604,1880.8486,6.3813}, 
	{1244.2897,1679.5634,6.7104}, 
	{1254.6846,1522.9485,9.9234}, 
	{1210.9384,1434.5048,6.3573}, 
	{1174.7994,1377.8687,10.4418}, 
	{1006.8182,1318.8185,10.2961}, 
	{1088.7847,1194.6664,10.2977}, 
	{1293.4116,1193.4810,10.3842}, 
	{1507.8230,1120.9666,10.2972}, 
	{1568.1940,888.4680,11.3424}, 
	{1493.1559,707.5800,10.2982}
};

static RaceID; // this will hold the id this race is assigned at init


public race_control_init() // unique named init function (must be ran on gamemode init)
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
