//
// Converted:   10.05.08
// Aurthor:    	[T5]SpLendour
// Converter: 	MadCat
//

#if defined _race_efenish_included
  #endinput
#endif

#define _race_efenish_included
#pragma library race_efenish
#include "race"

static racename[MAX_NAME] = "Efenish";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 3.0;
static Float:racecheckpoints[][CoordInfo] = {
{1072.017578,1304.322631,10.500017},
{1009.112976,1434.358032,10.358907},
{1007.900329,1585.541992,10.351679},
{1007.284606,1812.390258,10.518845},
{1009.789794,2073.028320,10.278334},
{1007.717895,2421.098144,10.344013},
{953.926269,2593.847900,10.260533},
{744.523498,2656.372558,17.634927},
{338.333984,2716.954101,59.559677},
{146.978683,2736.407226,54.173110},
{-94.687194,2635.899414,63.588138},
{-387.924468,2694.564453,63.756153},
{-564.453063,2697.355468,68.605575},
{-622.906311,2571.197753,88.488311},
{-494.741210,2437.763183,53.705833},
{-349.343261,2502.922851,36.019584},
{-164.276702,2482.837646,20.865859},
{123.973197,2496.449218,16.173662},
{450.784057,2504.668212,19.252243}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_efenish_init();

public race_efenish_init() // unique named init function (must be ran on gamemode init)
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

