//
// Converted:   10.05.08
// Aurthor:    	Switch
// Converter: 	MadCat
//

#if defined _race_beachfront_included
  #endinput
#endif

#define _race_beachfront_included
#pragma library race_beachfront
#include "race"

static racename[MAX_NAME] = "BeachFront";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 360.0;
static Float:racecheckpoints[][CoordInfo] = {
{370.3983,-1758.0093,13.5985},
{297.9694,-1639.1511,32.9250},
{403.8886,-1402.3876,33.2149},
{679.2260,-1319.5569,13.2999},
{797.3391,-1335.9146,13.1551},
{751.6486,-1608.3232,12.7072},
{715.5649,-1756.8311,14.2363},
{255.0722,-1685.9547,8.9021},
{175.0049,-1524.9076,12.2229},
{627.7590,-1227.5138,17.8432},
{612.2310,-1586.9414,15.7806},
{502.7237,-1559.6768,16.4948},
{483.8761,-1431.2250,17.2911},
{354.4469,-1524.6852,32.8941},
{369.7531,-1680.2906,31.7380},
{369.9450,-2014.2040,7.4441}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_beachfront_init();

public race_beachfront_init() // unique named init function (must be ran on gamemode init)
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

