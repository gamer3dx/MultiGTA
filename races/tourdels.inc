//
// Converted:   20.09.09
// Aurthor:    	GTAT Team
// Converter: 	MadCat
//

#if defined _race_tourdels_included
  #endinput
#endif

#define _race_tourdels_included
#pragma library race_tourdels
#include "race"

static racename[MAX_NAME] = "Tour De LS";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {};
static Float:startlineheading = 0.0;
static Float:racecheckpoints[][CoordInfo] = {
	{834.3040,-2021.4475,12.3261},
	{831.0298,-1801.5812,13.3464},
	{631.9240,-1724.5365,13.9629},
	{633.8140,-1213.3284,18.1195},
	{1252.7922,-937.6370,42.5349},
	{1267.7091,-1145.0344,23.0344},
	{1841.5944,-1180.6193,23.6193},
	{1833.5143,-1562.0354,13.0354},
	{2042.7925,-1609.5391,13.5391},
	{2487.3000,-1667.5599,13.5599}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_tourdels_init();

public race_tourdels_init() // unique named init function (must be ran on gamemode init)
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

