//
// Converted:   10.08.08
// Aurthor:    	GTAT Team
// Converter: 	MadCat
//

#if defined _race_mountchilliad_included
  #endinput
#endif

#define _race_mountchilliad_included
#pragma library race_mountchilliad
#include "race"

static racename[MAX_NAME] = "Mount Chilliad";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_SANCHEZ};
static Float:startlineheading = 194.0;
static Float:racecheckpoints[][CoordInfo] = {
{-2288.8967,-1704.7334,477.6903},
{-2281.5349,-1747.3247,461.3008},
{-2325.3198,-1811.2308,436.5499},
{-2441.9612,-1720.3864,432.3093},
{-2436.9941,-1810.6803,410.6423},
{-2527.7751,-1631.0872,399.2267},
{-2474.9851,-1474.0204,390.2994},
{-2341.9551,-1448.8215,382.5401},
{-2234.8799,-1545.9670,378.8185},
{-2208.5388,-1719.4735,376.2674},
{-2203.0347,-1537.6018,360.6990},
{-2309.5757,-1411.2526,358.2424},
{-2564.3845,-1503.2842,358.6855},
{-2336.4309,-1304.5925,309.2161},
{-2665.5935,-1396.8043,254.2428},
{-2608.4990,-1283.2968,220.1940},
{-2515.6326,-1120.1257,177.0773},
{-2721.2026,-1285.3386,152.9354},
{-2813.0696,-1511.7955,138.9581}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_mountchilliad_init();

public race_mountchilliad_init() // unique named init function (must be ran on gamemode init)
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

