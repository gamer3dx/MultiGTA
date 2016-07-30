//
// Converted:   20.09.09
// Aurthor:    	GTAT Team
// Converter: 	MadCat
//

#if defined _race_loopway_included
  #endinput
#endif

#define _race_loopway_included
#pragma library race_loopway
#include "race"

static racename[MAX_NAME] = "loopway";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {};
static Float:startlineheading = 0.0;
static Float:racecheckpoints[][CoordInfo] = {
	{834.3040,-2021.4475,12.3261},
	{836.7853,-1800.3737,12.9211},
	{1030.7134,-1796.6311,13.1809},
	{1037.7620,-1726.4679,12.8641},
	{1167.1971,-1711.8549,13.1692},
	{1177.4766,-1848.5063,12.8798},
	{1418.4879,-1876.3409,12.8617},
	{1398.7749,-1926.9299,16.4181},
	{1227.8176,-1927.2703,30.7117},
	{1096.7626,-1970.9039,45.4117},
	{1097.5264,-2101.9033,64.2658},
	{1240.0229,-2036.9771,59.4214}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_loopway_init();

public race_loopway_init() // unique named init function (must be ran on gamemode init)
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

