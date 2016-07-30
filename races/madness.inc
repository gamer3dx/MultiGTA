//
// Converted:   10.05.08
// Aurthor:    	[T5]M0BsT3R
// Converter: 	MadCat
//

#if defined _race_madness_included
  #endinput
#endif

#define _race_madness_included
#pragma library race_madness
#include "race"

static racename[MAX_NAME] = "Madness";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 225.0;
static Float:racecheckpoints[][CoordInfo] = {
{2211.414550,-2176.213867,13.017243},
{2246.270019,-2213.467529,13.002782},
{2302.959228,-2249.639892,13.035230},
{2393.349365,-2177.650390,13.118535},
{2547.937255,-2158.112060,12.883083},
{2666.741699,-2166.822021,10.619988},
{2787.683349,-2152.672607,10.589826},
{2842.161132,-2058.717041,10.643517},
{2853.959228,-1898.108764,10.586215},
{2878.432617,-1658.905639,10.542951},
{2918.260009,-1520.818847,10.540407},
{2880.680908,-1184.091674,10.535254},
{2778.157714,-1148.321044,30.641149},
{2692.702880,-1153.959106,52.961235},
{2562.840576,-1151.661987,45.488273},
{2371.498291,-1152.202636,27.111856},
{2321.937255,-1145.166015,26.449810},
{2389.674560,-1094.525024,36.649177},
{2541.500976,-1085.886840,62.043861},
{2549.833496,-1046.618286,69.074142},
{2429.487304,-1036.406372,53.047397},
{2247.476318,-1042.402465,53.576751},
{2169.198974,-1008.498840,62.464912},
{2201.060546,-1078.791748,38.867691},
{2175.102539,-1186.216918,23.867137},
{2187.504394,-1219.181884,23.494766},
{2272.566406,-1234.890014,23.473241},
{2298.568603,-1343.287597,23.593381},
{2285.479980,-1401.729248,23.577939},
{2296.055175,-1490.836914,22.917911}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_madness_init();

public race_madness_init() // unique named init function (must be ran on gamemode init)
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

