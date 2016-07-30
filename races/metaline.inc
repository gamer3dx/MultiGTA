//
// Converted:   10.05.08
// Aurthor:    	[T5]SpLendour
// Converter: 	MadCat
//

#if defined _race_metaline_included
  #endinput
#endif

#define _race_metaline_included
#pragma library race_metaline
#include "race"

static racename[MAX_NAME] = "MetaLine";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 294.6331;
static Float:racecheckpoints[][CoordInfo] = {
{-2672.974365,-384.935943,9.147475},
{-2362.367675,-342.579711,38.635307},
{-2112.378173,-339.059020,35.010417},
{-2006.631103,-283.002685,35.024742},
{-2005.824340,92.348037,27.259395},
{-2003.625976,357.695770,34.723354},
{-2003.736938,571.815246,34.723358},
{-2004.656982,724.492858,45.006427},
{-2036.209106,809.962402,51.956539},
{-2229.868164,810.666687,48.988704},
{-2398.017578,809.898925,34.726928},
{-2609.606445,811.632690,49.536376},
{-2750.630615,844.033813,58.348430},
{-2726.205810,1001.955078,54.229965},
{-2604.028564,1036.028320,72.202270},
{-2605.199707,1139.453491,55.140445},
{-2676.970214,1284.681152,55.137065},
{-2674.561523,1531.827514,60.175609},
{-2674.295898,1710.544799,67.391120},
{-2675.925537,2005.630737,59.606479},
{-2688.256835,2230.581787,55.136173},
{-2700.009765,2385.865234,65.176383},
{-2616.681152,2500.622558,26.457557},
{-2534.557861,2402.027099,14.718751},
{-2560.003906,2319.937011,4.541658},
{-2464.223632,2223.446777,4.549168}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_metaline_init();

public race_metaline_init() // unique named init function (must be ran on gamemode init)
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

