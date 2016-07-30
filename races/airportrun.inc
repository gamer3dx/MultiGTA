//
// Converted:   10.08.08
// Aurthor:    	GTAT Team
// Converter: 	MadCat
//

#if defined _race_airportrun_included
  #endinput
#endif

#define _race_airportrun_included
#pragma library race_airportrun
#include "race"

static racename[MAX_NAME] = "Airport Run";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 180.0;
static Float:racecheckpoints[][CoordInfo] = {
{2049.8652,938.7567,9.3167},
{2041.2273,866.6457,6.4332},
{1814.5861,853.1345,10.2969},
{1748.4485,1640.1976,8.4755},
{1708.5771,1608.0829,9.6403},
{1590.1888,1528.5900,10.4537},
{1468.8960,1171.9122,13.1164},
{1469.2255,1112.6007,10.4381},
{1390.1951,1123.1671,10.4454},
{1388.8176,1183.9401,10.3433},
{1020.7244,1194.2949,10.3618},
{1006.5272,1363.6918,10.3219},
{1027.6503,1422.4170,5.5894},
{1082.4624,1614.4507,5.4452},
{1066.0825,1707.8318,10.4451},
{1038.3922,1809.5942,10.2850},
{1006.8558,2084.2778,10.2957},
{942.4736,2578.0203,10.1714},
{751.4840,2654.2153,17.1890},
{383.4261,2702.0505,60.3132},
{152.3662,2742.1953,54.6380},
{-183.3021,2634.9314,62.9309},
{-255.5781,2588.1750,63.1952}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_airportrun_init();

public race_airportrun_init() // unique named init function (must be ran on gamemode init)
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

