//
// Converted:   02.03.08
// Aurthor:    	[T5]M0BsT3R
// Converter: 	MadCat
//

#if defined _race_airportdrift_included
  #endinput
#endif

#define _race_airportdrift_included
#pragma library race_airportdrift
#include "race"

forward race_airportdrift_init();

static racename[MAX_NAME] = "Airport Drift";
static racefrequency = 5;
static lineupdelay = 360;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 270.0;
static Float:racecheckpoints[][CoordInfo] = {
{1641.613037,-2494.802246,13.262164},
{1752.547241,-2494.509765,13.262045},
{1877.498901,-2494.250244,13.262215},
{1979.191162,-2494.036132,13.246547},
{2105.304931,-2521.671142,13.247272},
{2077.678466,-2583.963867,13.254074},
{2007.208618,-2591.084716,13.256665},
{1845.371948,-2591.938964,13.254321},
{1691.591674,-2590.049316,13.254143},
{1579.452392,-2593.446044,13.254219},
{1466.013305,-2593.858398,13.254453},
{1436.478881,-2578.102050,13.254903},
{1425.042968,-2506.675292,13.261818},
{1527.351562,-2448.915771,13.262099},
{1626.292114,-2455.336425,13.262209},
{1769.207031,-2460.147460,13.261928},
{1914.678710,-2465.753417,13.247097},
{2096.097656,-2513.585937,13.245666},
{2101.088378,-2562.698242,13.254510},
{2019.928710,-2589.124023,13.254289},
{1920.147827,-2588.722412,13.254215},
{1811.266967,-2592.078369,13.254430},
{1719.398437,-2599.979492,13.254434},
{1444.580688,-2583.190917,13.255313},
{1495.358398,-2466.446533,13.262063},
{1691.262817,-2465.742919,13.262585},
{1873.822509,-2419.173095,13.261491},
{1967.671508,-2236.490966,15.569885},
{1962.108154,-2131.799804,13.207971}
};

static RaceID; // this will hold the id this race is assigned at init


public race_airportdrift_init() // unique named init function (must be ran on gamemode init)
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
