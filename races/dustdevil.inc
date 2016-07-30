//
// Converted:   21.10.07
// Aurthor:    	BetaMaster
// Converter: 	MadCat
//

#if defined _race_dustdevil_included
  #endinput
#endif

#define _race_dustdevil_included
#pragma library race_dustdevil
#include "race"

forward race_dustdevil_init();

static racename[MAX_NAME] = "Dust Devil";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 90.4741;
static Float:racecheckpoints[][CoordInfo] = {
	{-768.8769,2730.1309,45.4151},
	{-994.3484,2714.3503,45.8672},
	{-1202.7035,2631.5608,52.1334},
	{-1109.4838,2419.6350,81.4255},
	{-1150.9731,2102.6924,84.8138},
	{-1211.9895,1864.1584,40.3733},
	{-971.7077,1832.3748,61.6141},
	{-756.6122,2056.0596,59.9461},
	{-436.6937,2120.0767,46.5704},
	{-371.6131,2351.1440,29.2572},
	{-588.2740,2514.8264,75.5239},
	{-768.8769,2730.1309,45.4151}

};

static RaceID; // this will hold the id this race is assigned at init

public race_dustdevil_init() // unique named init function (must be ran on gamemode init)
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
