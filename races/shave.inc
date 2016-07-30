//
// Converted:   27.04.08
// Aurthor:    	Switch
// Converter: 	MadCat
//

#if defined _race_shave_included
  #endinput
#endif

#define _race_shave_included
#pragma library race_shave
#include "race"

static racename[MAX_NAME] = "Shave";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 197.0173;
static Float:racecheckpoints[][CoordInfo] = {
{75.5080,-95.1919,0.4485},
{68.6645,-142.0897,0.8062},
{-32.731239,-141.347991,2.850350},
{-129.0057,-122.2574,2.8970},
{-297.7577,-63.3621,1.1304},
{-271.8772,67.3368,0.8592},
{-271.3748,120.0380,4.2292},
{-290.7363,112.3534,4.5158},
{-393.2559,29.1080,35.1361},
{-371.4740,-164.5270,56.2594},
{-538.3661,-190.2498,78.1863},
{-746.6474,-179.8985,66.4230},
{-821.2347,-117.2620,62.7330},
{-674.2108,-22.5275,70.5832},
{-885.7313,-33.2783,33.3609},
{-788.4532,18.5307,33.0190},
{-747.3611,109.9062,14.3462},
{-694.1469,222.3481,1.9997},
{-554.3903,107.9756,25.0088},
{-493.7426,12.2605,50.7144},
{-529.4949,-87.4784,62.3852},
{-622.2098,-52.2711,63.1360},
{-639.9692,132.7464,33.6822},
{-615.7090,234.9580,19.5876},
{-328.9536,176.3824,6.1313},
{-200.4228,202.6676,11.0972},
{-16.3231,129.9583,2.8973},
{109.4078,16.5517,0.3907},
{124.7572,-127.4821,1.3586},
{115.3164,-204.7754,1.3589},
{-76.4948,-179.0236,1.8390},
{-85.5051,-63.1126,2.8975},
{-27.5381,163.6372,2.2127}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_shave_init();

public race_shave_init() // unique named init function (must be ran on gamemode init)
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

