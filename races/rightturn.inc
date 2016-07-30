//
// Converted:   21.10.07
// Aurthor:    	BetaMaster
// Converter: 	MadCat
//

#if defined _race_rightturn_included
  #endinput
#endif

#define _race_rightturn_included
#pragma library race_rightturn
#include "race"

static racename[MAX_NAME] = "Right Turn";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 88.9491;
static Float:racecheckpoints[][CoordInfo] = {
	{-2685.1467,468.3502,5.5702},
	{-2791.9600,467.1815,5.0058},
	{-2849.7869,484.8507,3.8342},
	{-2837.5994,700.3406,24.2679},
	{-2831.9951,961.6788,43.4732},
	{-2881.3501,1137.4923,19.0032},
	{-2659.4231,1289.7703,6.6061},
	{-2455.4395,1356.0621,6.6092},
	{-2469.7180,1219.1582,34.6492},
	{-2628.2292,1182.1476,34.5848},
	{-2734.6614,1104.1705,46.1033},
	{-2677.3391,1204.4344,54.9935},
	{-2611.9473,1127.1266,54.9887},
	{-2524.6282,1069.5999,58.2918},
	{-2526.9170,895.7682,64.2199},
	{-2526.9065,625.8153,27.6348},
	{-2539.2737,475.8054,14.0279},
	{-2685.1467,468.3502,5.5702}
};


static RaceID;

forward race_rightturn_init();

public race_rightturn_init()
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

