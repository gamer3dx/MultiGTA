//
// Converted:   20.09.09
// Aurthor:    	GTAT Team
// Converter: 	MadCat
//

#if defined _race_tollstation_included
  #endinput
#endif

#define _race_tollstation_included
#pragma library race_tollstation
#include "race"

static racename[MAX_NAME] = "Toll station";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {};
static Float:startlineheading = 90.0;
static Float:racecheckpoints[][CoordInfo] = {
{-1754.3436,926.6018,24.4716},
{-2003.5540,906.4507,45.0272},
{-2136.6721,809.3880,69.1412},
{-2387.2017,830.4695,36.6725},
{-2313.9856,959.1221,58.7981},
{-2259.6733,942.8302,66.2270},
{-2261.4548,692.1169,49.0240},
{-2282.6440,565.5038,34.7427},
{-2604.6462,547.5393,14.1880},
{-2623.3713,466.4590,14.1880},
{-2850.3438,481.7461,3.9557},
{-2813.4080,873.2627,43.7068},
{-2750.4568,820.0818,52.8736},
{-2730.0784,1097.0920,46.0474},
{-2686.1113,1278.5248,55.1568}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_tollstation_init();

public race_tollstation_init() // unique named init function (must be ran on gamemode init)
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

