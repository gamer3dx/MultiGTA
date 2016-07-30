//
// Converted:   10.05.08
// Aurthor:    	Switch
// Converter: 	MadCat
//

#if defined _race_tourismo_included
  #endinput
#endif

#define _race_tourismo_included
#pragma library race_tourismo
#include "race"

static racename[MAX_NAME] = "Tourismo";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_TURISMO};
static Float:startlineheading = 97.8584;
static Float:racecheckpoints[][CoordInfo] = {
{-303.751464,1399.380371,71.920097},
{-372.2542,1438.7529,60.5814},
{-327.1424,1317.3958,52.3930},
{-436.3054,1424.6826,32.5984},
{-18.6400,1265.6381,7.8765},
{197.5922,1041.2305,22.3592},
{243.3795,978.3083,27.9001},
{588.0663,1070.3156,28.0064},
{849.9431,1651.2971,7.8721},
{790.2444,1829.4922,4.6445},
{652.6295,1822.5513,5.1758},
{526.7051,1727.0356,10.2161},
{614.2158,1956.3438,31.0934},
{398.5054,2361.6064,24.9979},
{-76.7480,2363.8047,18.0544},
{-366.4634,2548.4768,37.2279},
{-506.0084,2592.7737,53.1398}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_tourismo_init();

public race_tourismo_init() // unique named init function (must be ran on gamemode init)
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

