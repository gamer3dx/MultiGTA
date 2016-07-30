//
// Converted:   10.05.08
// Aurthor:    	Switch
// Converter: 	MadCat
//

#if defined _race_lsrace_included
  #endinput
#endif

#define _race_lsrace_included
#pragma library race_lsrace
#include "race"

static racename[MAX_NAME] = "LSRace";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 9.2603;
static Float:racecheckpoints[][CoordInfo] = {
{2905.7305,-1335.2546,10.5547},
{2821.5581,-1141.8853,15.8710},
{2391.7834,-1154.3229,28.2620},
{2128.0623,-1083.0441,23.8318},
{1831.9017,-1030.3024,36.1764},
{1708.6503,-1094.7461,56.0305},
{1606.3248,-1715.6747,27.8894},
{1469.1903,-2124.5723,13.2589},
{1339.2551,-2457.6050,13.2338},
{1677.8434,-2675.8174,5.7187},
{2154.1982,-2621.6726,13.2282},
{2219.4734,-2346.1616,13.2265},
{2486.6650,-2162.4805,13.3195},
{2829.9673,-2007.4019,10.7871},
{2875.0083,-1591.7281,10.5547}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_lsrace_init();

public race_lsrace_init() // unique named init function (must be ran on gamemode init)
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

