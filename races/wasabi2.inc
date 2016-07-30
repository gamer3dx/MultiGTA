//
// Converted:   10.05.08
// Aurthor:    	[T5]SpLendour
// Converter: 	MadCat
//

#if defined _race_wasabi2_included
  #endinput
#endif

#define _race_wasabi2_included
#pragma library race_wasabi2
#include "race"

static racename[MAX_NAME] = "Wasabi 2";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 30.5270;
static Float:racecheckpoints[][CoordInfo] = {
{145.091842,-1739.782592,5.451718},
{156.510848,-1556.349975,10.679671},
{270.091674,-1500.445678,33.091583},
{485.172576,-1352.974487,17.247461},
{710.970947,-1206.711914,17.803247},
{798.123229,-1101.999023,23.476594},
{883.505310,-992.122985,35.532928},
{995.357116,-944.838928,41.488903},
{1054.288085,-958.239135,42.260776},
{1282.756835,-932.692687,41.461502},
{1579.037231,-967.598754,37.678215},
{1985.227172,-1008.935729,32.568809},
{2173.382812,-1036.142822,60.350440},
{2231.337890,-1170.089233,36.010311}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_wasabi2_init();

public race_wasabi2_init() // unique named init function (must be ran on gamemode init)
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

