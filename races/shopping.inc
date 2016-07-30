//
// Converted:   20.09.09
// Aurthor:    	GTAT Team
// Converter: 	MadCat
//

#if defined _race_shopping_included
  #endinput
#endif

#define _race_shopping_included
#pragma library race_shopping
#include "race"

static racename[MAX_NAME] = "Shopping";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {};
static Float:startlineheading = 0.0;
static Float:racecheckpoints[][CoordInfo] = {
	{834.3040,-2021.4475,12.3261},
	{810.7695,-1761.1030,13.0639},
	{826.0718,-1603.1383,13.0514},
	{632.3682,-1569.2482,15.1009},
	{659.3041,-1400.3385,13.0671},
	{1137.0187,-1396.2368,12.9703},
	{1141.3309,-1345.1566,13.3721},
	{1084.9877,-1351.3271,13.1041},
	{1127.9197,-1456.0234,15.4608},
	{1161.2040,-1489.9047,22.4315}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_shopping_init();

public race_shopping_init() // unique named init function (must be ran on gamemode init)
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

