//
// Converted:   10.05.08
// Aurthor:    	[T5]SpLendour
// Converter: 	MadCat
//

#if defined _race_wasabisf_included
  #endinput
#endif

#define _race_wasabisf_included
#pragma library race_wasabisf
#include "race"

static racename[MAX_NAME] = "Wasabi SF";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 359.3008;
static Float:racecheckpoints[][CoordInfo] = {
{-2261.673828,632.535278,47.805957},
{-2260.291259,873.087646,69.504776},
{-2260.112304,1022.120117,86.445770},
{-2306.182128,1171.437866,50.668228},
{-2385.180664,1181.470214,38.582046},
{-2654.717041,1181.980346,55.138442},
{-2603.933105,1007.482971,78.716346},
{-2603.262695,946.308593,75.963996},
{-2608.076416,789.021301,48.883983},
{-2614.902343,667.845275,37.677131}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_wasabisf_init();

public race_wasabisf_init() // unique named init function (must be ran on gamemode init)
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

