//
// Converted:   10.08.08
// Aurthor:    	GTAT Team
// Converter: 	MadCat
//

#if defined _race_countryroad_included
  #endinput
#endif

#define _race_countryroad_included
#pragma library race_countryroad
#include "race"

static racename[MAX_NAME] = "Country Road";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 225.0;
static Float:racecheckpoints[][CoordInfo] = {
{-295.1542,1573.3831,76.0111},
{-301.4975,1496.0103,75.6700},
{-305.8924,1395.2058,72.3069},
{-366.9530,1464.4055,63.1385},
{-322.9260,1318.6128,53.1384},
{-446.9998,1492.3864,34.6927},
{-409.4112,1918.1522,57.7942},
{-436.1572,1762.0065,72.0316},
{-383.1084,2075.0764,60.8696},
{-654.7132,2058.2725,60.3828},
{-882.4459,1969.4230,60.8023},
{-1000.7413,1854.5486,63.1852},
{-1180.5957,1819.8922,41.2538},
{-1251.6730,1958.2874,50.4544},
{-1078.9517,2251.6218,87.8944},
{-1108.6061,2470.2358,81.4263},
{-1184.8412,2691.5342,45.8672},
{-777.9162,2730.1667,45.3489}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_countryroad_init();

public race_countryroad_init() // unique named init function (must be ran on gamemode init)
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

