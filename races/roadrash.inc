//
// Created:     Unknown
// Aurthor:    	Switch
// Converter: 	MadCat
//

#if defined _race_roadrash_included
  #endinput
#endif

#define _race_roadrash_included
#pragma library race_roadrash
#include "race"

forward race_roadrash_init();

static racename[MAX_NAME] = "RoadRash";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 358.8766;
static Float:racecheckpoints[][CoordInfo] = {
{1160.8435,-1134.5485,23.2356},
{1158.0519,-916.3906,42.2880},
{1248.9576,-336.7861,6.1752},
{1352.7019,-34.6912,34.1436},
{1459.8367,174.4490,25.2380},
{1142.8296,402.8703,25.5250},
{575.6768,285.1383,17.4089},
{575.6768,285.1383,17.4089},
{500.4075,-283.3174,39.8664},
{264.3361,-569.4389,38.8921},
{-94.4051,-1025.6466,23.6669},
{3.5686,-1475.0807,3.6614},
{-35.6812,-1388.7163,10.7921},
{641.9627,-1221.3713,17.8376},
{1147.4009,-1145.0419,23.2263}
};

static RaceID; // this will hold the id this race is assigned at init


public race_roadrash_init() // unique named init function (must be ran on gamemode init)
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

