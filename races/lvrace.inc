//
// Converted:   10.05.08
// Aurthor:    	Switch
// Converter: 	MadCat
//

#if defined _race_lvrace_included
  #endinput
#endif

#define _race_lvrace_included
#pragma library race_lvrace
#include "race"

static racename[MAX_NAME] = "LVRace";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 337.8006;
static Float:racecheckpoints[][CoordInfo] = {
{1753.6545,850.6560,9.5190},
{1568.6921,833.5577,6.3990},
{1287.6051,905.6605,6.4374},
{1226.4041,1334.8127,6.3621},
{1209.0433,1502.7931,6.3621},
{1263.7335,1732.5851,8.0399},
{1401.9918,1872.6542,10.3007},
{1707.2323,1886.4031,10.2967},
{1728.4037,2053.3477,10.2970},
{1956.9373,2022.1593,10.2968},
{2176.9705,2137.1475,10.2968},
{2349.7729,2179.5166,10.3058},
{2287.6816,2291.3613,10.3413},
{2271.7271,2413.7090,10.3006},
{2189.2878,2325.5537,10.2968},
{2074.8704,1758.3037,10.2968},
{2047.5907,1587.5190,10.2968},
{2061.5239,1531.0094,10.2968},
{2047.8118,1361.2373,10.2968},
{2046.0575,861.8326,6.3588}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_lvrace_init();

public race_lvrace_init() // unique named init function (must be ran on gamemode init)
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

