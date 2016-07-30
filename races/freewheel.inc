//
// Converted:   10.05.08
// Aurthor:    	Switch
// Converter: 	MadCat
//

#if defined _race_freewheel_included
  #endinput
#endif

#define _race_freewheel_included
#pragma library race_freewheel
#include "race"

static racename[MAX_NAME] = "FreeWheel";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 330.0;
static Float:racecheckpoints[][CoordInfo] = {
{-413.0344,-889.7043,25.8435},
{-337.3363,-785.6618,31.7379},
{-311.1908,-866.9036,46.6402},
{-538.2558,-912.4324,56.4735},
{-767.6442,-1004.3062,77.9672},
{-895.8164,-1111.3229,98.6168},
{-1224.3892,-765.2197,63.6690},
{-973.8251,-441.0424,34.8714},
{-486.8981,-402.6943,14.4156},
{-384.0918,-694.1434,19.9672},
{-361.5254,-796.3683,29.1719},
{-526.8291,-1027.2772,24.2020}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_freewheel_init();

public race_freewheel_init() // unique named init function (must be ran on gamemode init)
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

