//
// Converted:   10.05.08
// Aurthor:    	Switch
// Converter: 	MadCat
//

#if defined _race_sfrace_included
  #endinput
#endif

#define _race_sfrace_included
#pragma library race_sfrace
#include "race"

static racename[MAX_NAME] = "SFRace";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 351.7199;
static Float:racecheckpoints[][CoordInfo] = {
{-2908.6084,-1166.1984,9.1680},
{-2859.2830,-818.8527,6.7187},
{-2502.1252,-334.0524,29.7034},
{-2129.4766,-351.1667,34.6964},
{-2021.9302,-312.3736,34.9638},
{-2003.0897,-271.9948,34.9981},
{-1991.8639,-69.5321,33.8623},
{-1799.0338,-103.0793,5.6857},
{-1809.7943,201.0103,14.6408},
{-1586.8282,465.3767,6.7110},
{-1593.0995,1054.3867,6.8281},
{-1858.0253,1352.2291,6.7905},
{-2068.9509,1277.8479,8.9683},
{-2261.1189,1122.5388,71.9575},
{-2261.0310,722.7672,48.9766},
{-2229.9541,503.7214,34.7030},
{-2252.2991,262.0502,34.8437},
{-2253.5896,34.5404,34.8516},
{-2256.0115,-238.5709,37.3031},
{-2140.2639,-509.3277,38.2051},
{-1975.8826,-580.7640,25.5114},
{-1735.3674,-579.6141,16.0156},
{-1540.5515,-426.4935,5.5341}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_sfrace_init();

public race_sfrace_init() // unique named init function (must be ran on gamemode init)
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

