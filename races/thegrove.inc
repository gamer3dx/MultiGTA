//
// Created:     26.09.06
// Aurthor:    Iain Gilbert
//

#if defined _race_thegrove_included
  #endinput
#endif

#define _race_thegrove_included
#pragma library race_thegrove
#include "race"

static racename[MAX_NAME] = "Tearin Up The Grove";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 89.6004;
static Float:racecheckpoints[][CoordInfo] = {
{2455.3418,-1658.9462,13.1084}, // race tearin up grove start
{2328.3130,-1659.6624,13.3823}, //
{2153.5654,-1630.4480,13.4345}, //
{1985.4319,-1622.2905,15.4251}, //
{1806.4852,-1612.0620,13.1587}, //
{1591.4789,-1591.9191,13.1875}, //
{1247.5911,-1570.8453,13.1866}, //
{1018.6743,-1571.9991,13.1925}, //
{810.7006,-1658.2537,13.1866},
{827.2470,-1820.2151,12.1349},
{831.5974,-1943.2738,12.6706},
{828.5103,-2046.3984,12.6702}
};

static RaceID;

forward race_thegrove_init();

public race_thegrove_init()
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

