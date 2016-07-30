//
// Created:     20.10.07
// Aurthor:    Frolov Dmitry
//

#if defined _race_ls_holiday_included
  #endinput
#endif

#define _race_ls_holiday_included
#pragma library race_ls_holiday
#include "race"

static racename[MAX_NAME] = "LS Holiday Race";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 79.7063;
static Float:racecheckpoints[][CoordInfo] = {
{1439.964355,-955.523620,35.808460},
{1263.656860,-930.543457,42.146244},
{1107.375244,-951.233154,42.310733},
{1083.826049,-1009.800170,34.966064},
{1082.592773,-1123.652832,23.571956},
{1058.531982,-1168.015136,23.537160},
{1058.459106,-1367.799438,13.090914},
{993.513854,-1400.417846,12.708389},
{669.449340,-1400.478759,13.105178},
{505.219848,-1422.841064,15.427358},
{359.114929,-1519.587402,32.696166},
{328.688262,-1617.117431,32.892742},
{375.862731,-1648.191162,32.423767},
{548.879272,-1668.240966,18.235240},
{671.138916,-1672.842773,12.949235},
{788.019653,-1674.782714,13.000005},
{810.268371,-1737.729980,13.087179},
{909.378967,-1787.381103,13.088311},
{1029.329223,-1817.760742,13.471518},
{1036.896728,-2059.692138,12.638694},
{1039.774780,-2281.584960,12.633927},
{1173.528930,-2429.571289,9.943174},
{1293.432250,-2463.654052,7.457790},
{1320.395629,-2425.916992,9.548416},
{1328.358642,-2329.138427,13.087031},
{1264.697875,-2324.776367,15.094599},
{1300.076660,-2378.136230,21.393438},
{1441.038940,-2378.284667,14.513422},
{1474.471191,-2358.127929,13.086652},
{1517.948364,-2309.210449,13.090334},
{1482.051513,-2240.515869,13.086902},
{1395.757080,-2216.440185,13.251085}


};

static RaceID; // this will hold the id this race is assigned at init

forward race_ls_holiday_init();

public race_ls_holiday_init() // unique named init function (must be ran on gamemode init)
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