//
// Converted:   26.04.08
// Aurthor:    	Switch
// Converter: 	MadCat
//

#if defined _race_caper2_included
  #endinput
#endif

#define _race_caper2_included
#pragma library race_caper2
#include "race"

static racename[MAX_NAME] = "Caper 2";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 99.7859;
static Float:racecheckpoints[][CoordInfo] = {
{2786.4567,-2142.6342,10.7251},
{2650.2190,-2164.0388,10.8391},
{2173.9878,-2356.7258,13.1071},
{2132.7468,-2211.9414,13.1203},
{1962.6104,-2155.9619,13.1201},
{1961.6570,-1989.7781,13.1292},
{1962.1296,-1885.1221,13.1203},
{1943.3909,-1742.5186,13.1192},
{1824.5719,-1603.1301,13.1063},
{1838.6664,-1528.1621,13.1162},
{1862.9625,-1465.7365,13.1225},
{2110.8342,-1451.4069,23.5657},
{2143.6660,-1384.5609,23.5732},
{2313.4246,-1384.9012,23.6094},
{2367.2896,-1370.4769,23.5848},
{2382.3455,-1258.3184,23.5673},
{2516.1687,-1256.6057,34.6056},
{2642.8872,-1242.3362,49.5824},
{2630.0640,-1048.2833,69.1512},
{2328.1199,-1078.7473,49.9872},
{2148.7192,-1001.6094,61.9569},
{1979.3420,-1065.9506,24.2380},
{1996.1587,-1260.8354,23.5576},
{2069.2571,-1274.6775,23.5598},
{2109.3875,-1396.3995,23.5657},
{2112.8718,-1501.6371,23.5341},
{2092.8560,-1893.5023,13.1186},
{2216.1191,-1908.4226,13.1090},
{2236.8047,-2018.7897,13.2842},
{2399.6504,-2012.2312,13.2901},
{2514.9619,-2009.0873,13.0193}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_caper2_init();

public race_caper2_init() // unique named init function (must be ran on gamemode init)
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

