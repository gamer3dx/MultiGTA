//
// Converted:   21.10.07
// Aurthor:    	BetaMaster
// Converter: 	MadCat
//

#if defined _race_baysidetour_included
  #endinput
#endif

#define _race_baysidetour_included
#pragma library race_baysidetour
#include "race"

forward race_baysidetour_init();

static racename[MAX_NAME] = "Bayside Tour";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_SANCHEZ,V_BF400,V_NRG500};
static Float:startlineheading = 129.4430;
static Float:racecheckpoints[][CoordInfo] = {
	{-2227.9651,2326.5593,7.5469},
	{-2287.2295,2279.4375,4.2537},
	{-2431.0364,2301.7419,4.2659},
	{-2497.8606,2214.2861,4.2680},
	{-2615.0693,2304.8044,7.4872},
	{-2594.2239,2458.5981,19.6351},
	{-2468.9353,2554.7456,22.8853},
	{-2272.0784,2552.6316,3.2372},
	{-2287.2439,2419.6404,4.1978},
	{-2227.9651,2326.5593,7.5469}
};

static RaceID; // this will hold the id this race is assigned at init

public race_baysidetour_init() // unique named init function (must be ran on gamemode init)
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

