//
// Converted:   02.08.08
// Aurthor:     Rockstar North Team
// Converter: 	MadCat
//

#if defined _race_sp_bcountry_included
  #endinput
#endif

#define _race_sp_bcountry_included
#pragma library race_sp_banditocountry
#include "race"

static racename[MAX_NAME] = "(SP)Bandito Country";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_BANDITO};
static Float:startlineheading = 316.6392;
static Float:racecheckpoints[][CoordInfo] = {
{-1936.791,-2452.235,29.6377},
{-1862.54,-2379.436,30.1425},
{-1690.144,-2303.846,43.3626},
{-1676.317,-2132.699,35.2082},
{-1855.421,-2087.117,58.3697},
{-1919.802,-1937.457,78.5313},
{-1749.334,-1899.519,95.8199},
{-1571.292,-1904.166,84.3076},
{-1483.784,-1783.286,49.8768},
{-1429.878,-1915.641,25.1751},
{-1301.801,-2084.054,22.9116},
{-1269.912,-2248.736,21.3152},
{-1146.514,-2371.508,27.0467},
{-959.5773,-2306.27,56.1859},
{-867.9595,-2180.891,25.6567},
{-843.4177,-2018.734,20.9828},
{-767.7295,-1865.533,11.4149},
{-666.3682,-1996.276,23.9143},
{-486.6196,-2025.765,48.245},
{-323.4612,-1914.206,11.1951}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_sp_banditocountry_init();

public race_sp_banditocountry_init() // unique named init function (must be ran on gamemode init)
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

