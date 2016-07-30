//
// Created:     18.09.06
// Aurthor:    Iain Gilbert
//

#if defined _race_flyingfree_included
  #endinput
#endif

#define _race_flyingfree_included
#pragma library race_flyingfree
#include "race"

static racename[MAX_NAME] = "Flying Free";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 269.8778;
static Float:racecheckpoints[][CoordInfo] = {
{2061.4482,2215.9102,10.4453},
{2093.0518,2216.0762,10.4450},
{2169.2773,2136.9636,10.2965},
{2373.6736,2149.3003,10.3048},
{2568.6799,2113.6638,10.2968},
{2643.9021,2144.7830,10.1311},
{2704.9390,2316.2732,6.3590},
{2572.2424,2550.5789,4.8080},
{2264.7070,2605.9133,6.3827},
{1542.2550,2453.9536,6.3588},
{1373.6936,2450.4348,6.3593},
{1226.3834,2114.8442,6.3626},
{1347.6971,1873.0059,10.2968},
{1663.8398,1713.1904,10.2968},
{1939.1622,1712.9327,10.2968},
{2129.4377,1772.9849,10.2968},
{2326.4624,1752.7520,10.2968},
{2289.8843,1532.9813,10.3006},
{2161.3049,1532.8400,10.2987},
{1998.1360,1543.9204,14.1627}
};

static RaceID;

forward race_flyingfree_init();

public race_flyingfree_init()
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

