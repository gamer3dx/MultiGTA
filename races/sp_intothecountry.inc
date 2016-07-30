//
// Converted:   02.08.08
// Aurthor:     Rockstar North Team
// Converter: 	MadCat
//

#if defined _race_sp_intocountry_included
  #endinput
#endif

#define _race_sp_intocountry_included
#pragma library race_sp_intothecountry
#include "race"

static racename[MAX_NAME] = "(SP)Into The Country";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_BULLET,V_BANSHEE,V_CHEETAH,V_TURISMO};
static Float:startlineheading = 180.0;
static Float:racecheckpoints[][CoordInfo] = {
{1331.926,-2280.51,13.1732},
{1331.775,-2541.682,13.1765},
{1544.308,-2684.983,7.5074},
{1945.946,-2685.131,6.5549},
{2175.141,-2557.994,13.173},
{2352.719,-2222.223,13.1737},
{2740.522,-2168.782,10.7349},
{2844.832,-1812.965,10.6737},
{2926.094,-1421.263,10.657},
{2888.946,-1022.685,10.6607},
{2896.483,-621.0989,10.63},
{2718.623,-273.2495,27.6705},
{2774.299,119.9424,22.8712},
{2612.82,327.3945,25.6764},
{2213.983,324.9601,32.5307},
{1817.184,278.3323,21.5314},
{1621.185,312.8743,20.8534},
{1608.029,372.27,26.4284},
{1658.672,317.3336,30.0474},
{1651.536,-68.8376,35.9156},
{1668.653,-369.1116,34.3923},
{1699.785,-665.8258,43.3311},
{1640.03,-1062.329,60.8035},
{1594.408,-1457.834,28.3679},
{1622.355,-1856.504,25.4707},
{1452.159,-2117.271,13.1726}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_sp_intothecountry_init();

public race_sp_intothecountry_init() // unique named init function (must be ran on gamemode init)
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

