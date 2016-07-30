//
// Created:     Unknown
// Aurthor:    	Switch
// Converter: 	MadCat
//

#if defined _race_offroad_included
  #endinput
#endif

#define _race_offroad_included
#pragma library race_offroad
#include "race"

forward race_offroad_init();

static racename[MAX_NAME] = "OffRoad";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_LANDSTAL,V_RANCHER,V_SANDKING,V_HUNTLEY,V_PATRIOT};
static Float:startlineheading = 178.0101;
static Float:racecheckpoints[][CoordInfo] = {
{1025.5874,-352.1767,73.8363},
{1046.1371,-448.2977,51.2698},
{1274.8077,-384.9712,2.3164},
{1850.0424,-330.9108,41.5122},
{1974.2798,39.1270,32.6214},
{2293.0544,-44.8297,26.7148},
{2317.6782,-98.8642,26.7118},
{2332.8201,92.5219,25.9575},
{2110.6680,97.7820,35.0512},
{1801.0560,194.6196,29.3695},
{1534.5748,252.4577,16.6556},
{1475.6809,177.2965,27.6776},
{1559.6138,-40.9808,21.5678},
{1442.9294,-213.2336,9.4197},
{1206.8671,-98.1241,39.9148},
{855.4489,-97.6680,27.3813},
{641.5322,-195.7723,11.3263},
{404.4324,-310.6018,11.5245},
{360.0536,-395.6732,20.8487},
{885.1068,-557.0125,21.1134},
{1025.5874,-352.1767,73.8363}
};

static RaceID; // this will hold the id this race is assigned at init

public race_offroad_init() // unique named init function (must be ran on gamemode init)
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

