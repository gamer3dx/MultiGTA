//
// Converted:   02.08.08
// Aurthor:     Rockstar North Team
// Converter: 	MadCat
//

#if defined _race_sp_cendurance_included
  #endinput
#endif

#define _race_sp_cendurance_included
#pragma library race_sp_countryendurance
#include "race"

static racename[MAX_NAME] = "(SP)Country Endurance";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_BULLET,V_BANSHEE,V_CHEETAH,V_INFERNUS,V_TURISMO};
static Float:startlineheading = 180.0;
static Float:racecheckpoints[][CoordInfo] = {
{-1761.655,-606.3881,15.8961},
{-1760.54,-686.4133,24.2389},
{-1502.876,-820.2545,61.3357},
{-1219.332,-759.4814,62.0285},
{-1096.152,-489.1358,32.7156},
{-911.5416,-455.9558,26.7166},
{-621.3364,-393.158,22.1878},
{-395.5605,-533.522,17.6561},
{-372.2669,-819.4839,28.1926},
{-584.5377,-1160.572,21.8153},
{-659.3533,-1548.814,22.1495},
{-717.1517,-1697.205,48.2586},
{-714.6181,-1395.094,60.5799},
{-764.046,-1385.027,82.2146},
{-762.7014,-1685.041,96.9103},
{-945.4141,-1903.227,81.3436},
{-1104.541,-2155.325,34.6761},
{-1186.201,-2444.541,54.4532},
{-982.3237,-2611.624,86.253},
{-713.2529,-2336.617,36.4201},
{-512.3461,-2166.417,53.5212},
{-258.5978,-2074.872,36.648},
{-260.1367,-1777.191,7.5735},
{-57.3444,-1601.375,1.3776},
{-124.296,-1466.575,2.4045},
{-143.4815,-1270.317,2.404},
{-113.7683,-998.821,23.9178},
{-377.3843,-838.2166,47.1444},
{-626.0056,-989.012,66.0268},
{-877.4561,-1097.426,96.1699},
{-925.9376,-1391.353,126.2905},
{-1219.141,-1347.248,122.5235},
{-1407.723,-1414.45,104.9232},
{-1577.013,-1168.802,102.3586},
{-1626.067,-882.9627,97.5243},
{-1744.303,-853.2858,77.1834},
{-1684.612,-1143.29,72.4364},
{-1549.538,-1286.347,57.1135},
{-1525.757,-1383.531,45.1796},
{-1626.142,-1375.587,45.2062},
{-1800.174,-1016.79,51.6322},
{-1817.478,-617.6195,15.9507}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_sp_countryendurance_init();

public race_sp_countryendurance_init() // unique named init function (must be ran on gamemode init)
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

