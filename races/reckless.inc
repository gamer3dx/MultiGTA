//
// Created:     Unknown
// Aurthor:    	Switch
// Converter: 	MadCat
//

#if defined _race_reckless_included
  #endinput
#endif

#define _race_reckless_included
#pragma library race_reckless
#include "race"

forward race_reckless_init();

static racename[MAX_NAME] = "ReckLess";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 94.2046;
static Float:racecheckpoints[][CoordInfo] = {
{-244.9915,2634.4763,62.1790},
{-465.3546,2716.7439,63.6523},
{-856.0282,2727.0300,45.3945},
{-1397.4844,2602.6472,54.9359},
{-1548.4818,2611.2261,55.2594},
{-1615.0706,2648.1719,55.6176},
{-1700.4635,2294.7146,19.8048},
{-1817.2694,2123.0344,7.6342},
{-1785.9661,1912.5222,14.6227},
{-1690.6467,1845.4386,24.1793},
{-1652.9586,2118.0566,19.1704},
{-1900.0123,2364.1497,46.2160},
{-1946.0165,2475.8149,54.4946},
{-2715.1855,2362.2883,70.9323},
{-2496.6321,2406.4617,15.8570},
{-2465.9277,2249.1221,4.3625}
};

static RaceID; // this will hold the id this race is assigned at init


public race_reckless_init() // unique named init function (must be ran on gamemode init)
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

