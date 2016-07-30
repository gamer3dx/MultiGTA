//
// Converted:   31.08.08
// Aurthor:    	Sneaky & G-sTyLeZzZ
// Converter: 	MadCat
//

#if defined _race_lsinternational_included
  #endinput
#endif

#define _race_lsinternational_included
#pragma library race_lsinternational
#include "race"

static racename[MAX_NAME] = "LS International";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 171.7808;
static Float:racecheckpoints[][CoordInfo] = {
{1039.3666,-2044.8092,12.4971},
{1038.1484,-2282.5976,12.5139},
{1380.8040,-2466.4697,6.5941},
{1544.4093,-2285.7946,-3.4268},
{1614.5571,-2318.5063,-3.2864},
{1796.1323,-2303.1892,-3.1898},
{1732.4553,-2266.6105,-3.2863},
{1568.3089,-2280.7553,-3.2574},
{1506.1223,-2250.5476,-3.4271},
{1427.3889,-2315.5068,-1.8210},
{1052.6343,-2047.1510,12.5064}
};


static RaceID; // this will hold the id this race is assigned at init

forward race_lsinternational_init();

public race_lsinternational_init() // unique named init function (must be ran on gamemode init)
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

