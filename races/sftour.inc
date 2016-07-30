//
// Created:     Unknown
// Aurthor:    	DracoBlue
// Converter: 	MadCat
//

#if defined _race_sftour_included
  #endinput
#endif

#define _race_sftour_included
#pragma library race_sftour
#include "race"

forward race_sftour_init();

static racename[MAX_NAME] = "San-Fierro Tour";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 7.4535;
static Float:racecheckpoints[][CoordInfo] = {
	{-1903.6914,-812.2429,44.7258},
	{-1922.7479,-674.2916,40.8599},
	{-1923.8647,-599.5046,24.5117},
	{-2036.6589,-578.6338,27.7179},
	{-2247.3894,-392.0201,50.6401},
	{-2252.4399,176.3849,34.9454},
	{-2006.2700,227.3615,27.9153},
	{-2004.7780,588.4307,34.7882},
	{-1839.9333,604.5129,34.7893},
	{-1831.0491,720.4266,39.0801},
	{-1898.2781,729.2193,45.0700},
	{-2121.1960,730.7721,69.1865},
	{-2142.3855,898.5939,79.6211},
	{-2122.8733,917.1022,79.6652},
	{-2005.4423,925.1073,45.0695},
	{-2003.5913,1047.2124,55.3419},
	{-2446.6335,1091.8855,55.3513},
	{-2675.4067,1203.2448,55.2029},
	{-2732.0334,1192.6666,53.4031},
	{-2733.2261,1104.0082,46.2783},
	{-2749.6479,1016.0234,53.8841},
	{-2751.0999,560.8705,14.1710},
	{-2708.1570,482.0063,4.2555},
	{-2838.6794,465.7323,3.9667},
	{-2849.9438,346.0463,4.1272},
	{-2818.0149,-462.8958,6.8111},
	{-2676.4607,-518.5759,17.5658},
	{-2651.4849,-392.7055,33.3551},
	{-2391.9597,-373.9563,74.9361},
	{-2324.6616,-451.8440,79.7511},
	{-2416.9292,-417.2825,85.2076},
	{-2627.9668,-497.1491,70.0705},
	{-2492.4988,-480.0302,95.9211},
	{-2429.1594,-595.9906,131.9933},
	{-2486.3777,-615.3236,132.3340}

};

static RaceID; // this will hold the id this race is assigned at init


public race_sftour_init() // unique named init function (must be ran on gamemode init)
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
