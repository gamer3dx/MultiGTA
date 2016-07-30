//
// Converted:   27.04.08
// Aurthor:    	Switch
// Converter: 	MadCat
//

#if defined _race_hilltop_included
  #endinput
#endif

#define _race_hilltop_included
#pragma library race_hilltop
#include "race"

static racename[MAX_NAME] = "Hilltop";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 172.9301;
static Float:racecheckpoints[][CoordInfo] = {
{-1340.6406,-1002.9613,186.4016},
{-1353.9985,-1030.1969,174.0749},
{-1541.3699,-1002.2000,154.3438},
{-1452.0577,-1317.1277,100.1359},
{-1391.3861,-1433.7018,101.7703},
{-1222.7703,-1472.0977,109.3717},
{-1108.3251,-1464.9019,124.4537},
{-979.3915,-1405.9828,130.0339},
{-918.1014,-1461.4546,123.3569},
{-832.4865,-1583.0302,125.4507},
{-938.9722,-1624.5442,81.9274},
{-959.1837,-1641.9143,75.1369},
{-959.5011,-1721.3929,77.2384},
{-947.6267,-1810.6334,89.2591},
{-984.6214,-1950.6193,78.0874},
{-1044.7627,-2144.1431,34.2820},
{-967.8874,-2162.7122,34.4530},
{-715.3575,-2124.1733,25.7393},
{-675.5233,-1969.9429,21.3737},
{-635.8657,-1842.0179,22.6964},
{-436.3235,-1728.6573,7.5869},
{-475.6003,-1564.4796,9.8466},
{-375.0765,-1398.9211,23.5056},
{-306.9965,-1235.7668,18.5849},
{-153.7142,-1021.0463,0.7870},
{-366.5361,-1113.4277,69.3367},
{-373.3295,-1053.4851,58.8827}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_hilltop_init();

public race_hilltop_init() // unique named init function (must be ran on gamemode init)
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

