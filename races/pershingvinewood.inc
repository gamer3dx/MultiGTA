//
// Converted:   31.08.08
// Aurthor:    	Sneaky & G-sTyLeZzZ (original race name: PershingVinewood)
// Converter: 	MadCat
//

#if defined _race_pershingvinewood_included
  #endinput
#endif

#define _race_pershingvinewood_included
#pragma library race_pershingvinewood
#include "race"

static racename[MAX_NAME] = "Pershing - Vinewood";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 269.5417;
static Float:racecheckpoints[][CoordInfo] = {
{1321.7145,-1732.1877,12.9550},
{1529.4891,-1718.7210,12.9542},
{1514.1761,-1592.4981,12.9330},
{1312.3471,-1587.6905,12.9484},
{1332.6831,-1732.1715,12.9483},
{1429.7036,-1709.9134,12.9482},
{1450.8643,-1592.2807,12.9484},
{1529.9271,-1612.8154,12.9482},
{1502.9599,-1733.2049,12.9484},
{1312.5986,-1716.2067,12.9481},
{1369.8142,-939.4336,33.7531}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_pershingvinewood_init();

public race_pershingvinewood_init() // unique named init function (must be ran on gamemode init)
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

