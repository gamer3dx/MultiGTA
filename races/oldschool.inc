//
// Converted:   21.10.07
// Aurthor:    	BetaMaster
// Converter: 	MadCat
//

#if defined _race_oldschool_included
  #endinput
#endif

#define _race_oldschool_included
#pragma library race_oldschool
#include "race"

forward race_oldschool_init();

static racename[MAX_NAME] = "Old School";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 273.6591;
static Float:racecheckpoints[][CoordInfo] = {
	{2780.3391,-1854.2334,9.6620},
	{2821.6328,-1913.3925,10.8022},
	{2754.8606,-2148.6377,10.7986},
	{2347.2690,-2208.0137,13.2454},
	{2262.0869,-2196.5430,10.3588},
	{2041.5787,-1929.7871,7.8537},
	{1746.9832,-1790.1371,3.8521},
	{1618.0137,-1630.2446,13.4177},
	{1897.1365,-1612.3605,13.2532},
	{2214.6819,-1647.6506,15.1649},
	{2481.7512,-1694.6499,13.3900},
	{2642.4231,-1699.0020,10.6031},
	{2780.3391,-1854.2334,9.6620}
};

static RaceID; // this will hold the id this race is assigned at init


public race_oldschool_init() // unique named init function (must be ran on gamemode init)
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

