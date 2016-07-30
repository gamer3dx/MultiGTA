//
// Converted:   02.08.08
// Aurthor:     Rockstar North Team
// Converter: 	MadCat
//

#if defined _race_sp_sftolv_included
  #endinput
#endif

#define _race_sp_sftolv_included
#pragma library race_sp_sftolv
#include "race"

static racename[MAX_NAME] = "(SP)SF to LV";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_BANSHEE,V_CHEETAH,V_INFERNUS,V_TURISMO};
static Float:startlineheading = 90.7950;
static Float:racecheckpoints[][CoordInfo] = {
{-2001.856,1076.302,54.5637},
{-2302.386,1076.754,54.5859},
{-2596.785,1136.728,54.4297},
{-2673.767,1427.293,54.4271},
{-2673.775,1728.238,66.8937},
{-2673.807,2028.896,57.2377},
{-2731.742,2323.183,67.5411},
{-2622.094,2602.597,68.7},
{-2322.772,2637.1,53.2135},
{-2027.122,2585.865,53.9804},
{-1885.621,2321.522,38.5395},
{-1675.296,2107.697,17.3995},
{-1629.976,1810.828,11.059},
{-1347.988,1705.67,4.8179},
{-1125.87,1504.095,22.1877},
{-1010.766,1227.044,31.1773},
{-908.7908,944.9431,17.337},
{-744.0402,694.0286,16.9982},
{-469.1425,573.061,16.0649},
{-168.3162,562.0268,14.9046},
{110.1296,673.4105,4.6336},
{400.681,749.8367,5.0607},
{686.3097,656.6995,7.8963},
{975.3585,738.1249,9.6719},
{1242.045,876.267,13.1519},
{1226.81,1169.877,5.8125},
{1226.664,1469.936,5.742}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_sp_sftolv_init();

public race_sp_sftolv_init() // unique named init function (must be ran on gamemode init)
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

