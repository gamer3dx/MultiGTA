//
// Converted:   26.04.08
// Aurthor:    	small4
// Converter: 	MadCat (based on 4wD rally)
//

#if defined _race_desertrally_included
  #endinput
#endif

#define _race_desertrally_included
#pragma library race_desertrally
#include "race"

forward race_desertrally_init();

static racename[MAX_NAME] = "Desert Rally";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 30.0;
static Float:racecheckpoints[][CoordInfo] = {
{-466.300232,2052.539551,59.867188},
{-442.448059,2111.615234,47.900311},
{-386.784546,2156.295654,41.960747},
{-377.953278,2207.856689,41.093750},
{-395.961426,2283.273438,39.310989},
{-481.260742,2302.251465,61.325409},
{-578.526123,2371.337402,78.896667},
{-681.515625,2463.700195,115.015648},
{-700.632813,2417.595459,128.750809},
{-726.309082,2356.931641,125.508141},
{-821.822327,2355.375488,119.798965},
{-841.129211,2416.813477,119.678513},
{-816.635742,2485.665527,105.153870},
{-772.468628,2556.921875,85.138832},
{-736.319946,2521.277588,76.028824},
{-665.856384,2496.655029,74.348076},
{-587.266602,2432.190674,68.319275},
{-521.187927,2443.526855,58.916058},
{-572.932007,2498.790527,69.975082},
{-629.772644,2574.540039,88.038933},
{-596.763245,2663.494141,78.646629},
{-559.522156,2703.793213,66.731583},
{-519.602051,2761.269775,71.549629},
{-476.292389,2794.685059,84.361259},
{-529.564209,2834.489258,87.860550},
{-652.888672,2863.598145,76.617561},
{-664.812744,2900.540039,64.308289},
{-736.167236,2899.184326,39.749325},
{-821.691956,2860.217529,65.375290},
{-889.690796,2865.995361,76.716179},
{-962.688354,2864.229736,81.564728},
{-1030.418213,2879.579346,67.403961},
{-1095.874756,2892.677246,65.889069},
{-1164.186890,2884.762695,76.906624},
{-1262.430786,2863.346436,97.842705},
{-1369.726563,2843.370117,97.105186},
{-1471.641968,2831.009766,102.364288},
{-1593.051025,2816.336182,97.610153},
{-1682.872314,2811.554199,105.832054},
{-1818.036621,2808.406738,112.494431},
{-1932.379517,2820.092041,133.379089},
{-2018.647095,2799.009033,159.889603},
{-2092.494629,2768.061279,172.144257},
{-2161.073486,2704.208008,160.628876},
{-2179.451660,2693.439941,154.927795},
{-2265.533936,2661.014404,72.731606}

};

static RaceID; // this will hold the id this race is assigned at init


public race_desertrally_init() // unique named init function (must be ran on gamemode init)
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
