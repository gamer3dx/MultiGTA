//
// Created:     18.09.06
// Aurthor:    Iain Gilbert
//

#if defined _race_backstreetbang_included
  #endinput
#endif

#define _race_backstreetbang_included
#pragma library race_backstreetbang
#include "race"

static racename[MAX_NAME] = "Backstreet Bang";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 88.9491;
static Float:racecheckpoints[][CoordInfo] = {
{2169.8728,2022.4692,10.3516},
{2090.0566,2037.6790,10.5000},
{2005.1310,2048.6021,10.5000},
{1990.9373,2139.4656,10.5000},
{1996.5925,2199.5171,10.5000},
{2029.6523,2206.1169,10.5000},
{2014.6067,2301.7781,10.5000},
{2027.4928,2414.8936,10.3516},
{1975.6697,2643.1221,10.3516},
{1881.7070,2712.0186,10.3516},
{1526.7462,2772.8616,10.3516},
{1444.2147,2819.7632,10.5014},
{1322.0309,2803.3960,10.4915},
{1150.1769,2825.6453,10.4921},
{1126.1074,2786.4648,10.4999},
{1160.8470,2720.6135,10.4484},
{1245.1222,2634.2136,10.7033},
{1293.9686,2528.9114,10.3515}
};


static RaceID;

forward race_backstreetbang_init();

public race_backstreetbang_init()
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

