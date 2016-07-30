//
// Converted:   10.05.08
// Aurthor:    	Switch
// Converter: 	MadCat
//

#if defined _race_msr_included
  #endinput
#endif

#define _race_msr_included
#pragma library race_msr
#include "race"

static racename[MAX_NAME] = "MSR";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 180.0;
static Float:racecheckpoints[][CoordInfo] = {
{-2706.4490,15.4894,3.8075},
{-2672.1636,-70.5870,3.8079},
{-2072.0862,-70.0750,34.7946},
{-2007.3146,-10.7509,34.1683},
{-1951.4784,345.2479,33.1302},
{-1820.2784,503.7693,28.1800},
{-1847.4680,606.0377,34.6383},
{-1691.1620,731.2337,22.7547},
{-1555.8347,964.8956,6.8882},
{-1678.0497,1186.9478,17.3264},
{-2001.4463,1176.0630,44.9197},
{-2143.3831,1126.4026,70.2794},
{-2143.2393,783.0396,69.0395},
{-2206.6082,731.7432,49.2045},
{-2261.0723,646.1559,48.9197},
{-2271.6750,508.4194,34.6385},
{-2343.5276,457.7750,32.2451},
{-2338.5803,309.3127,38.6040},
{-2706.4915,157.8556,3.9201}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_msr_init();

public race_msr_init() // unique named init function (must be ran on gamemode init)
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

