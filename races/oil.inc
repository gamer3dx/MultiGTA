//
// Converted:   27.04.08
// Aurthor:    	Switch
// Converter: 	MadCat
//

#if defined _race_oil_included
  #endinput
#endif

#define _race_oil_included
#pragma library race_oil
#include "race"

static racename[MAX_NAME] = "Oil";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 79.5362;
static Float:racecheckpoints[][CoordInfo] = {
{584.4291,855.8440,-43.2536},
{496.7525,913.1824,-31.3340},
{547.6824,957.7469,-20.2355},
{706.6964,967.6871,-9.6315},
{802.9772,841.9870,8.6133},
{905.2349,908.0718,13.0287},
{866.1415,1068.3171,23.8571},
{738.5264,1148.6731,21.8394},
{521.9126,1100.1238,18.0406},
{458.5009,1042.9341,28.2076},
{345.6015,1025.4336,27.6782},
{334.4910,1178.7504,8.0722},
{324.3958,1277.4586,12.6929},
{329.4374,1342.3698,8.6449},
{372.7992,1364.4037,8.6780},
{509.4473,1367.6295,4.9663},
{618.8433,1388.9633,11.3670},
{667.8107,1454.9663,8.8943},
{746.0272,1494.3818,10.2132},
{848.3810,1560.4503,17.6769},
{903.3660,1601.4480,15.5604},
{924.6001,1652.9540,8.3166},
{921.0760,1779.7043,8.3180},
{945.9962,1902.7443,10.4898},
{934.9140,1975.0913,10.4898},
{854.7449,2008.0125,10.4899},
{774.5179,1989.5057,5.0082},
{726.3444,1881.3467,5.2654},
{604.2944,1729.7988,6.2958},
{454.5284,1621.1144,16.4481},
{443.0451,1511.8204,9.2578},
{430.7408,1367.3728,7.9581},
{455.3703,1234.0518,9.7796},
{391.3040,1118.8749,11.9908},
{368.2047,1002.2945,29.4785},
{400.9102,929.2210,24.4523},
{521.1584,887.5301,-37.7608},
{659.8228,882.3239,-41.1391}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_oil_init();

public race_oil_init() // unique named init function (must be ran on gamemode init)
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

