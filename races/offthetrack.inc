//
// Converted:   10.08.08
// Aurthor:    	GTAT Team
// Converter: 	MadCat
//

#if defined _race_offthetrack_included
  #endinput
#endif

#define _race_offthetrack_included
#pragma library race_offthetrack
#include "race"

static racename[MAX_NAME] = "Off The Track";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_SANCHEZ};
static Float:startlineheading = 194.0;
static Float:racecheckpoints[][CoordInfo] = {
{-2288.8967,-1704.7334,477.6903},
{-2330.6409,-1815.0858,435.0002},
{-2446.4641,-1722.1838,431.5612},
{-2439.4517,-1808.0969,409.9866},
{-2520.7266,-1713.9172,401.8994},
{-2504.8550,-1569.6951,395.4035},
{-2450.5313,-1464.1143,388.6068},
{-2272.7061,-1475.0994,378.8693},
{-2203.9629,-1716.4771,376.1060},
{-2196.9836,-1557.0457,361.3687},
{-2370.9170,-1398.5199,359.5081},
{-2569.8167,-1502.8738,358.5945},
{-2430.3740,-1377.7899,332.2888},
{-2244.0105,-1314.9940,266.0325},
{-2330.5535,-1230.6334,235.7148},
{-2242.5598,-1276.4556,226.2734},
{-2480.0676,-1140.7546,184.2085},
{-2532.0991,-1112.3386,176.5482},
{-2524.4224,-1120.2855,177.5678}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_offthetrack_init();

public race_offthetrack_init() // unique named init function (must be ran on gamemode init)
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

