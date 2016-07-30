//
// Converted:   26.04.08
// Aurthor:    	BetaMaster
// Converter: 	MadCat
//

#if defined _race_dune_included
  #endinput
#endif

#define _race_dune_included
#pragma library race_dune
#include "race"

forward race_dune_init();

static racename[MAX_NAME] = "Dune";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 180.0;
static Float:racecheckpoints[][CoordInfo] = {
	{-2668.3191,-2246.4521,5.4405},
	{-2771.3958,-2530.3469,3.3513},
	{-2615.7175,-2775.1965,3.5570},
	{-2394.3357,-2854.2898,3.9257},
	{-2152.4116,-2829.8967,1.6272},
	{-1879.6536,-2751.2068,9.1258},
	{-2188.7422,-2806.9724,5.9882},
	{-2470.8572,-2735.6555,8.3306},
	{-2646.4084,-2464.7451,6.1860},
	{-2668.3191,-2246.4521,5.4405}

};

static RaceID; // this will hold the id this race is assigned at init


public race_dune_init() // unique named init function (must be ran on gamemode init)
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
