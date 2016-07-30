//
// Converted:   02.08.08
// Aurthor:     Rockstar North Team
// Converter: 	MadCat
//

#if defined _race_sp_lvringroad_included
  #endinput
#endif

#define _race_sp_lvringroad_included
#pragma library race_sp_lvringroad
#include "race"

static racename[MAX_NAME] = "(SP)LV Ringroad";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_TURISMO,V_ALPHA,V_CHEETAH,V_INFERNUS};
static Float:startlineheading = 270.1450;
static Float:racecheckpoints[][CoordInfo] = {
{1446.426,834.0658,5.8125},
{1747.259,834.025,8.6815},
{2047.335,834.1916,5.7422},
{2348.722,834.1481,5.7422},
{2637.92,915.6277,5.7498},
{2726.824,1202.266,5.7422},
{2726.698,1502.967,5.7422},
{2726.958,1804.343,5.7393},
{2726.579,2125.796,5.7269},
{2705.172,2405.567,5.7277},
{2489.267,2614.273,4.1679},
{2188.479,2609.629,5.8164},
{1898.309,2529.155,5.8125},
{1603.524,2473.083,5.8516},
{1305.181,2439.19,5.7422},
{1208.232,2154.514,5.7422},
{1208.38,1853.837,5.7422},
{1208.322,1553.025,5.7422},
{1208.126,1252.556,5.7498},
{1229.187,952.9079,5.8125}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_sp_lvringroad_init();

public race_sp_lvringroad_init() // unique named init function (must be ran on gamemode init)
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

