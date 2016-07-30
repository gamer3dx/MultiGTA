//
// Converted:   10.05.08
// Aurthor:    	[T5]M0BsT3R
// Converter: 	MadCat
//

#if defined _race_sanchezpower_included
  #endinput
#endif

#define _race_sanchezpower_included
#pragma library race_sanchezpower
#include "race"

static racename[MAX_NAME] = "Sanchez Power";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_SANCHEZ};
static Float:startlineheading = 268.7001;
static Float:racecheckpoints[][CoordInfo] = {
{-2438.123291,1376.330688,6.704327},
{-2358.503417,1375.188598,6.784030},
{-2271.082519,1337.862182,6.707444},
{-2158.199951,1335.177734,6.706361},
{-2089.448242,1325.784057,6.708719},
{-2056.935302,1284.713745,7.087368},
{-2118.003662,1269.998413,19.021104},
{-2126.112304,1227.149902,33.600994},
{-2179.208984,1232.324951,33.582828},
{-2199.653564,1267.609619,32.713882},
{-2261.791748,1230.185302,46.802246},
{-2258.228027,1171.628051,55.338859},
{-2251.280761,1094.714599,79.539192},
{-2150.043945,1085.533569,79.514678},
{-2148.542480,952.264526,79.509017},
{-2071.770263,926.654113,62.813430},
{-2007.398559,928.883972,44.967769},
{-1898.935058,925.548217,34.720466},
{-1795.108276,923.958374,24.409891},
{-1711.744995,892.125244,24.407535},
{-1628.960083,843.854675,7.066728},
{-1556.182128,790.707763,6.706855},
{-1588.493530,732.528503,8.424935},
{-1675.903808,733.836059,20.043270},
{-1709.566406,704.954467,24.403247},
{-1734.124267,611.037353,24.411294},
{-1864.410034,607.168518,34.682289},
{-1894.656127,568.106567,34.613025},
{-1709.418212,566.678833,37.545261},
{-1651.144531,625.695800,23.841716},
{-1626.520874,685.340270,6.855525}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_sanchezpower_init();

public race_sanchezpower_init() // unique named init function (must be ran on gamemode init)
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

