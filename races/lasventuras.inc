//
// Converted:   29.09.09
// Aurthor:    	GTAT Team
// Converter: 	MadCat
//

#if defined _race_lasventuras_included
  #endinput
#endif

#define _race_lasventuras_included
#pragma library race_lasventuras
#include "race"

static racename[MAX_NAME] = "Las Venturas";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 180.0;
static Float:racecheckpoints[][CoordInfo] = {
{2049.8652,938.7567,9.3167},
{2046.6819,854.9907,6.3593},
{1329.9681,873.1325,6.4369},
{1222.3574,1184.4178,6.4370},
{1297.2716,1810.1230,10.5898},
{1296.7456,2412.8240,6.3596},
{1788.3285,2335.6875,6.1195},
{1790.3278,1424.3032,6.3664},
{1825.8422,833.1572,10.0233},
{2259.3572,764.5952,10.2164},
{2287.4658,914.9344,10.3581},
{2164.6824,993.2431,10.4413},
{2180.7261,1089.6458,10.4455}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_lasventuras_init();

public race_lasventuras_init() // unique named init function (must be ran on gamemode init)
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

