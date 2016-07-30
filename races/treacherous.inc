//
// Converted:   22.10.07
// Aurthor:    	BetaMaster
// Converter: 	MadCat
//

#if defined _race_treacherous_included
  #endinput
#endif

#define _race_treacherous_included
#pragma library race_treacherous
#include "race"

forward race_treacherous_init();

static racename[MAX_NAME] = "Treacherous Sarasvati";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_JETMAX,V_SPEEDER,V_SQUALO};
static Float:startlineheading = 93.1853;
static Float:racecheckpoints[][CoordInfo] = {
	{900.9455,-1993.5244,-0.6398},
	{563.0526,-1985.7623,-0.4228},
	{297.5530,-2105.4958,-0.3344},
	{104.7726,-2623.2646,-0.6955},
	{-164.9999,-2939.4036,-0.9317},
	{-583.9410,-2954.3740,0.0783},
	{-1001.8202,-2923.7803,-0.2472},
	{-1185.4100,-2611.4873,-0.5829},
	{-1207.2715,-2315.3098,-0.5005},
	{-1358.7010,-1765.7704,-0.5170},
	{-1695.6187,-1591.4415,-0.4977},
	{-2070.7090,-1225.3699,-0.5728},
	{-2448.2678,-898.4624,-0.5409},
	{-2819.0938,-1020.7505,-0.5891},
	{-2993.8794,-1386.1599,-0.1949},
	{-3000.0728,-1744.1499,-0.7221},
	{-2738.9871,-2158.1477,-0.5212}
};

static RaceID; // this will hold the id this race is assigned at init

public race_treacherous_init() // unique named init function (must be ran on gamemode init)
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

