//
// Converted:   10.05.08
// Aurthor:    	Switch
// Converter: 	MadCat
//

#if defined _race_caper_included
  #endinput
#endif

#define _race_caper_included
#pragma library race_caper
#include "race"

static racename[MAX_NAME] = "Caper";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 1.1809;
static Float:racecheckpoints[][CoordInfo] = {
{2713.6401,-1992.7823,13.1473},
{2706.8013,-1931.6128,13.0617},
{2594.7644,-1932.3586,13.1462},
{2516.9309,-1917.2277,13.1356},
{2552.7896,-1733.0706,13.1461},
{2639.0056,-1710.3000,10.5355},
{2666.9111,-1658.2222,10.4583},
{2694.0273,-1623.1388,14.0536},
{2393.4470,-1611.6151,10.0727},
{2159.7671,-1545.8871,2.0512},
{1940.1145,-1502.3821,3.0710},
{1802.9741,-1485.6189,7.7787},
{1748.9027,-1449.0697,13.3098},
{1714.9545,-1423.2085,13.1348},
{1732.1384,-1301.7416,13.1885},
{1909.2593,-1261.0325,13.4740},
{2086.7593,-1301.6969,23.5774},
{2322.0020,-1300.6818,23.8316},
{2370.0977,-1312.8074,23.5898},
{2390.4080,-1413.9270,23.5905},
{2517.9688,-1444.6980,28.1151},
{2661.7588,-1435.4243,30.2469},
{2750.2556,-1488.6401,29.8157},
{2869.7810,-1502.2765,10.6111},
{2811.7844,-1657.4480,10.4476},
{2665.8267,-1657.0422,10.4489}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_caper_init();

public race_caper_init() // unique named init function (must be ran on gamemode init)
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

