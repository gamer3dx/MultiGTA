//
// Converted:   10.08.08
// Aurthor:    	GTAT Team
// Converter: 	MadCat
//

#if defined _race_dirtygras_included
  #endinput
#endif

#define _race_dirtygras_included
#pragma library race_dirtygras
#include "race"

static racename[MAX_NAME] = "Dirty Gras";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_FCR900};
static Float:startlineheading = 0.0;
static Float:racecheckpoints[][CoordInfo] = {
{2600.5000,-1060.0000,70.0000},
{2580.4775,-987.8892,79.7233},
{2622.4006,-948.3683,79.1626},
{2759.6250,-814.2032,41.2315},
{2675.9016,-396.0258,52.8353},
{2708.6042,-23.0999,33.6867},
{2570.0129,41.6304,26.0043},
{2393.9985,57.2119,26.0051},
{2362.6226,91.9394,26.0855},
{2131.0818,94.9897,34.4584},
{2015.6649,126.5036,29.2184},
{1803.7711,194.4204,28.8768},
{1585.0201,261.7657,15.6618},
{1362.3611,219.8177,19.0745},
{1191.0225,306.9443,19.0760},
{1066.8306,195.1143,33.0769},
{952.8058,172.1949,30.3229},
{523.8744,116.5803,22.7704},
{503.1252,51.8757,18.9709},
{375.3436,-94.2039,1.1733},
{301.4321,-71.8409,1.0979},
{122.8399,-80.8880,1.2462},
{104.1882,-37.3951,1.0531},
{77.3569,67.5204,0.6166},
{-241.8848,207.7090,10.7631},
{-359.3857,173.3275,5.1116},
{-641.3604,234.1507,18.2991},
{-669.4056,155.0416,26.3959},
{-619.3485,-70.8390,63.2912},
{-512.2908,-109.9753,63.9229},
{-548.1541,-189.8305,78.0744},
{-796.0224,-185.3231,65.8070},
{-871.8847,-130.7385,58.5890},
{-958.9503,-299.7908,36.0730},
{-951.6574,-447.2997,32.4435},
{-737.6842,-451.1595,17.4894},
{-751.8131,-565.8499,40.2359},
{-413.3936,-641.7401,12.0309},
{-385.7838,-720.4185,23.7023},
{-623.8939,-729.8489,56.3711},
{-888.3771,-1112.0491,98.6285},
{-1258.1876,-736.4663,64.1514}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_dirtygras_init();

public race_dirtygras_init() // unique named init function (must be ran on gamemode init)
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

