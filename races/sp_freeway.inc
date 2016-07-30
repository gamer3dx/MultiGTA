//
// Converted:   02.08.08
// Aurthor:     Rockstar North Team
// Converter: 	MadCat
//

#if defined _race_sp_freeway_included
  #endinput
#endif

#define _race_sp_freeway_included
#pragma library race_sp_freeway
#include "race"

static racename[MAX_NAME] = "(SP)Freeway";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_SUPERGT,V_BANSHEE,V_BUFFALO,V_BULLET};
static Float:startlineheading = 270.4817;
static Float:racecheckpoints[][CoordInfo] = {
{852.0544,-1405.622,12.217},
{1102.789,-1405.766,12.428},
{1353.038,-1405.544,12.3251},
{1600.965,-1440.852,12.3828},
{1850.622,-1461.777,12.3984},
{831.7679,-1405.588,12.3849},
{1031.822,-1405.615,12.0966},
{1232.144,-1405.732,12.0633},
{1429.508,-1439.091,12.3828},
{1630.18,-1440.858,12.3828},
{1829.527,-1460.804,12.3448},
{2029.979,-1463.381,14.4553},
{2175.822,-1384.403,22.8281},
{2342.426,-1442.324,22.8281},
{2191.875,-1545.934,1.1724},
{1997.078,-1499.951,2.3804},
{1797.064,-1500.874,6.8307},
{1641.002,-1439.345,27.0747},
{1627.555,-1280.703,41.885},
{1683.507,-1089.012,55.7917},
{1566.054,-927.5945,42.7541},
{1366.105,-933.6759,33.1953},
{1166.087,-941.9011,41.8697},
{966.715,-962.2038,38.1905},
{784.2582,-1043.979,23.6177},
{677.2552,-1164.128,14.164},
{627.7037,-1337.904,12.4227}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_sp_freeway_init();

public race_sp_freeway_init() // unique named init function (must be ran on gamemode init)
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

