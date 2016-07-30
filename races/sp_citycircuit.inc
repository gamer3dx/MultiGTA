//
// Converted:   02.08.08
// Aurthor:     Rockstar North Team
// Converter: 	MadCat
//

#if defined _race_sp_citycircuit_included
  #endinput
#endif

#define _race_sp_citycircuit_included
#pragma library race_sp_citycircuit
#include "race"

static racename[MAX_NAME] = "(SP)City Circuit";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_FCR900,V_BF400,V_NRG500};
static Float:startlineheading = 269.8550;
static Float:racecheckpoints[][CoordInfo] = {
{1927.48,-1515.516,2.2978},
{2018.4,-1515.405,2.4054},
{2179.006,-1559.661,1.1797},
{2370.138,-1619.553,7.3323},
{2570.841,-1619.056,17.6841},
{2756.846,-1657.173,11.5191},
{2915.178,-1534.362,9.875},
{2916.228,-1333.491,9.875},
{2819.331,-1142.638,15.7018},
{2622.646,-1152.872,50.4181},
{2423.186,-1154.169,30.5642},
{2167.046,-1101.915,24.3864},
{2006.077,-1027.839,34.3152},
{1835.824,-991.5906,35.8824},
{1636.313,-1005.15,49.8831},
{1617.024,-1215.833,51.0566},
{1577.972,-1417.831,27.6154},
{1722.823,-1525.932,18.3927}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_sp_citycircuit_init();

public race_sp_citycircuit_init() // unique named init function (must be ran on gamemode init)
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

