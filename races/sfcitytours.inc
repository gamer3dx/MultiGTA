//
// Converted:   20.09.09
// Aurthor:    	GTAT Team
// Converter: 	MadCat
//

#if defined _race_sfcitytours_included
  #endinput
#endif

#define _race_sfcitytours_included
#pragma library race_sfcitytours
#include "race"

static racename[MAX_NAME] = "SF City Tours";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {};
static Float:startlineheading = 90.0;
static Float:racecheckpoints[][CoordInfo] = {
{-1754.3436,926.6018,24.4716},
{-2004.6738,954.0207,45.0240},
{-2049.3713,1081.8549,55.2974},
{-2525.3291,1065.9706,59.7966},
{-2504.9238,709.2115,30.3372},
{-2385.7244,680.1379,34.7416},
{-2249.2861,323.0274,34.8990},
{-1979.2489,343.4904,34.3074},
{-1833.6348,388.3438,16.7426},
{-1892.1940,133.3968,37.8921},
{-1877.1714,-156.0933,39.0727},
{-2180.8916,-294.6952,35.1374},
{-1896.2759,-239.2543,24.8433},
{-1802.4086,-578.9099,15.7365},
{-1515.0129,-399.6601,6.8052}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_sfcitytours_init();

public race_sfcitytours_init() // unique named init function (must be ran on gamemode init)
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

