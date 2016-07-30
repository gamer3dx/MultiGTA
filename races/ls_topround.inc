//
// Created:     20.10.07
// Aurthor:    Frolov Dmitry
//

#if defined _race_ls_topround_included
  #endinput
#endif

#define _race_ls_topround_included
#pragma library race_ls_topround
#include "race"

static racename[MAX_NAME] = "LS Top Round";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 90.1063;
static Float:racecheckpoints[][CoordInfo] = {
{2398.393798,-1751.300415,13.087018},
{2329.658447,-1751.522460,13.084855},
{2314.598144,-1743.082519,13.088177},
{2332.614990,-1731.992553,13.086565},
{2497.185546,-1732.294921,13.086992},
{2623.036865,-1732.357543,11.000874},
{2642.896728,-1679.628051,10.437490},
{2643.059814,-1493.611694,29.963663},
{2642.620849,-1277.804809,47.449806},
{2642.740234,-1096.113769,69.092193},
{2709.330566,-1086.094238,68.952880},
{2722.569824,-1166.402343,69.007324},
{2722.702880,-1264.390747,59.287929},
{2722.548828,-1470.036865,29.985450},
{2722.748779,-1589.578125,13.849822},
{2673.334960,-1609.451904,16.174863},
{2498.707275,-1603.213500,16.731031},
{2375.960693,-1570.827880,20.278442},
{2342.281005,-1584.519165,23.425304},
{2342.983398,-1687.844604,13.063136},
{2396.638671,-1750.538574,13.087397}

};

static RaceID; // this will hold the id this race is assigned at init

forward race_ls_topround_init();

public race_ls_topround_init() // unique named init function (must be ran on gamemode init)
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
