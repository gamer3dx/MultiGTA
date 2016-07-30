//
// Created:     Unknown
// Aurthor:    	Switch
// Converter: 	MadCat
//

#if defined _race_racepoint_included
  #endinput
#endif

#define _race_racepoint_included
#pragma library race_racepoint
#include "race"

static racename[MAX_NAME] = "RacePoint";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 34.9413;
static Float:racecheckpoints[][CoordInfo] = {
{-2427.3457,-603.7017,132.1822},
{-2628.0803,-485.4402,69.5166},
{-2319.1846,-443.9958,79.5618},
{-2671.7092,-476.2673,24.6808},
{-2821.4019,-419.0155,6.9350},
{-2692.8953,157.4807,4.0827},
{-2692.8953,157.4807,4.0827},
{-2490.9543,172.4697,6.7137},
{-2234.3022,320.2990,35.0675},
{-2146.5842,302.5507,35.0674},
{-2166.9277,-84.9404,35.0680},
{-2256.0684,-263.5397,42.0939},
{-2186.3596,-518.2983,47.9610},
{-2262.0188,-756.0815,76.5736},
{-2487.9646,-615.3112,132.4607}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_racepoint_init();

public race_racepoint_init() // unique named init function (must be ran on gamemode init)
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

