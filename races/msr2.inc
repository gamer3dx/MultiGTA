//
// Converted:   10.05.08
// Aurthor:    	Switch
// Converter: 	MadCat
//

#if defined _race_msr2_included
  #endinput
#endif

#define _race_msr2_included
#pragma library race_msr2
#include "race"

static racename[MAX_NAME] = "MSR2";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 360.0;
static Float:racecheckpoints[][CoordInfo] = {
{-2706.3987,477.1884,3.8159},
{-2750.8359,738.7856,46.9461},
{-2785.1592,815.6907,48.3771},
{-2881.3201,1056.1127,32.1300},
{-2595.0513,1336.6538,6.6632},
{-2455.9399,1337.5952,8.3969},
{-2438.3279,1188.3245,34.6397},
{-2111.1614,1176.7477,54.4826},
{-1966.4388,1151.7014,46.7530},
{-2006.5571,1066.5409,55.1931},
{-2375.2876,1051.4071,55.2010},
{-2388.7717,503.0630,28.8892},
{-2539.4268,325.6007,19.1946},
{-2703.7786,217.7770,3.8027}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_msr2_init();

public race_msr2_init() // unique named init function (must be ran on gamemode init)
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

