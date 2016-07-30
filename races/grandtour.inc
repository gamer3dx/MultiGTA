//
// Created:     Unknown
// Aurthor:    	TommyLR
// Converter: 	MadCat
//

#if defined _race_grandtour_included
  #endinput
#endif

#define _race_grandtour_included
#pragma library race_grandtour
#include "race"

forward race_grandtour_init();

static racename[MAX_NAME] = "Grand Tour";
static racefrequency = 10;
static lineupdelay = 420;
static minracers = 6;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 88.7779;
static Float:racecheckpoints[][CoordInfo] = {
     {-2161.5542,208.1582,34.7385},
     {-2250.1919,215.9451,35.1260},
     {-2264.6553,317.9467,35.3527},
     {-2377.8035,280.8392,33.0059},
     {-2527.5898,162.6635,4.1072},
     {-2702.8813,163.3876,4.2278},
     {-2706.2126,345.7263,4.1685},
     {-2708.8159,484.1431,4.5153},
     {-2752.3418,571.4223,14.0955},
     {-2751.7083,692.6898,40.8793},
     {-2737.7046,710.7960,40.8246},
     {-2492.0317,707.4057,33.6197},
     {-2380.1980,724.7695,34.7894},
     {-2216.8892,731.3777,49.0199},
     {-2095.6824,730.9844,69.1109},
     {-2002.2301,716.3704,44.9932},
     {-2004.5248,601.4590,34.7804},
     {-1996.4921,226.8935,27.9354},
     {-1879.5387,223.2334,38.5436},
     {-1769.9468,449.7451,38.4969},
     {-1634.5746,587.5946,39.9830},
     {-1465.0569,745.4326,46.1606},
     {-1429.1128,808.0278,46.9472},
     {-1335.9611,883.8917,46.4268},
     {-1293.5905,950.2773,45.2439},
     {-1125.0426,1104.0248,37.8017},
     {-995.1113,1158.2175,31.6553},
     {-905.1750,927.0567,18.5456},
     {-768.5289,707.3643,18.2815},
     {-262.7402,553.2479,15.8660},
     {-178.2435,570.7852,15.6628},
     {89.1348,665.3895,5.4331},
     {202.1422,727.6032,5.8967},
     {390.4712,759.4741,5.9243},
     {715.1945,704.0571,17.3052},
     {858.3228,706.9284,11.5419},
     {983.6392,743.5391,10.4714},
     {1327.4840,831.1554,6.7961},
     {1805.1980,819.8010,10.4984},
     {1699.4946,358.8647,31.0292},
     {1775.4570,270.3953,20.0501},
     {1992.6632,294.1921,35.1272},
     {2282.0098,298.2687,33.6595},
     {2343.5930,272.3645,27.3776},
     {2357.7603,-29.2379,27.3560},
     {2480.0862,-11.9329,27.4329},
     {2534.2795,10.8422,27.3396},
     {2554.5449,38.2764,27.3808},
     {2710.9751,-43.8135,39.2858},
     {2681.7939,-202.9776,34.9248},
     {2667.2930,-382.0087,52.7157},
     {2718.3481,-462.4705,53.8320},
     {2747.8755,-662.8290,68.0408},
     {2594.7988,-762.3275,92.3508},
     {2407.9153,-704.5621,124.3041},
     {2396.8181,-629.1509,126.3650},
     {2505.1445,-512.7581,93.3445},
     {2508.7373,-393.4811,78.3316},
     {2293.5276,-393.1061,59.6880},
     {2100.4990,-383.2884,68.1135},
     {2019.1660,-469.5103,76.3408},
     {2138.6960,-643.3951,58.4952},
     {2285.5581,-940.0953,26.1412},
     {2277.7773,-1150.7303,26.3165},
     {2218.3352,-1165.0167,25.2844}
};

static RaceID; // this will hold the id this race is assigned at init

public race_grandtour_init() // unique named init function (must be ran on gamemode init)
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

