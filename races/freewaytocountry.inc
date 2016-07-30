//
// Converted:   31.08.08
// Aurthor:    	Sneaky & G-sTyLeZzZ (original race name: Freeway-Country)
// Converter: 	MadCat
//

#if defined _race_freewaytocountry_included
  #endinput
#endif

#define _race_freewaytocountry_included
#pragma library race_freewaytocountry
#include "race"

static racename[MAX_NAME] = "Freeway To County";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 169.9008;
static Float:racecheckpoints[][CoordInfo] = {
{1037.0725,-2064.245849,12.502034},
{1396.4494,-2464.8571,5.8780},
{1428.6684,-2305.8010,-2.6374},
{1328.5947,-2450.4877,7.2172},
{1059.1744,-1902.4279,12.5479},
{791.3717,-1768.2581,12.8516},
{263.6041,-1680.2816,8.3314},
{2.9402,-1520.2677,2.6879},
{-88.6113,-1049.3132,20.8928},
{-51.8132,-840.9284,12.4160},
{-164.8066,-265.7881,3.1351}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_freewaytocountry_init();

public race_freewaytocountry_init() // unique named init function (must be ran on gamemode init)
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

