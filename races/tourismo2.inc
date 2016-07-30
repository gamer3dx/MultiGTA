//
// Converted:   10.05.08
// Aurthor:    	Switch
// Converter: 	MadCat
//

#if defined _race_tourismo2_included
  #endinput
#endif

#define _race_tourismo2_included
#pragma library race_tourismo2
#include "race"

static racename[MAX_NAME] = "Tourismo 2";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_TURISMO};
static Float:startlineheading = 268.0;
static Float:racecheckpoints[][CoordInfo] = {
{-347.2929,2509.8737,35.7340},
{-547.1313,2421.2659,62.4518},
{-709.6786,2669.7039,58.3930},
{-881.1180,2725.4900,45.5582},
{-1376.9172,2681.5010,52.9486},
{-1685.7078,2726.9143,61.3124},
{-1827.2687,2657.9575,54.0813},
{-1878.4998,2527.8867,48.2664},
{-1807.1605,2101.8152,8.1186},
{-1572.4445,1839.3124,26.2898},
{-1119.3523,1805.8833,42.1429},
{-712.9918,2063.6211,60.0883},
{-386.6433,2052.0862,61.9121},
{-427.2162,1768.7974,71.6460},
{-397.3694,1910.1670,57.7438},
{-422.9149,1674.7365,37.6806},
{-402.6476,1419.7163,38.5052},
{-302.0606,1510.9429,75.0665}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_tourismo2_init();

public race_tourismo2_init() // unique named init function (must be ran on gamemode init)
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

