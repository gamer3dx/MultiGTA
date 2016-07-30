//
// Created:     Unknown
// Aurthor:    	DracoBlue
// Converter: 	MadCat
//

#if defined _race_lvtour_included
  #endinput
#endif

#define _race_lvtour_included
#pragma library race_lvtour
#include "race"

forward race_lvtour_init();

static racename[MAX_NAME] = "Las-Venturas Tour";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 340.3265;
static Float:racecheckpoints[][CoordInfo] = {
	{1724.7892,1599.8022,9.6168},
	{1717.2925,1698.8955,10.2052},
	{1582.1213,1712.4835,10.2376},
	{1567.1868,1858.7642,10.2364},
	{1502.6525,1874.3527,10.2422},
	{1482.7777,2028.5792,10.5794},
	{1567.4860,2237.6667,10.3648},
	{1567.5251,2384.0544,10.2355},
	{1492.5149,2394.8992,10.2433},
	{1476.2971,2575.7642,10.2366},
	{1778.7147,2617.2830,10.3904},
	{1967.4408,2593.2390,10.3741},
	{2122.8545,2595.8872,6.4532},
	{2496.7698,2602.7446,4.5069},
	{2680.9028,2439.1870,6.2580},
	{2717.3892,2149.6777,6.2744},
	{2718.2571,1624.9746,6.2689},
	{2784.8892,1527.3192,9.4752},
	{2800.0361,1481.2019,10.3332},
	{2438.5027,1472.8168,10.2368},
	{2427.2893,1384.1130,10.2381},
	{2316.0950,1372.5933,10.2526},
	{2289.3647,1509.7212,16.7899},
	{2324.4282,1505.4305,16.7888},
	{2319.5371,1399.1589,23.2015},
	{2293.2393,1401.5000,23.1908},
	{2295.4768,1508.0170,29.5959},
	{2319.7048,1506.0809,29.5809},
	{2320.8108,1401.0339,35.9846},
	{2294.6328,1398.3114,35.9653},
	{2294.8586,1507.1433,42.3825},
	{2320.5793,1388.7689,43.1514}

};

static RaceID; // this will hold the id this race is assigned at init


public race_lvtour_init() // unique named init function (must be ran on gamemode init)
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
