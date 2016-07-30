//
// Converted:   22.10.07
// Aurthor:    	BetaMaster
// Converter: 	MadCat
//

#if defined _race_dustyroads_included
  #endinput
#endif

#define _race_dustyroads_included
#pragma library race_dustyroads
#include "race"

forward race_dustyroads_init();

static racename[MAX_NAME] = "Dusty Roads";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
//static allowedcars[4] = {V_LANDSTAL,V_RANCHER,V_SANDKING,V_HUNTLEY};
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 94.8300;
static Float:racecheckpoints[][CoordInfo] = {
	{365.8413,-93.4504,1.0828},
	{232.6801,-239.7997,1.1363},
	{329.6740,-347.6389,8.5008},
	{545.7914,-206.3070,16.5101},
	{896.6926,-90.9153,22.3725},
	{1258.5741,-149.8508,38.1793},
	{1559.5702,-48.2781,20.7874},
	{1328.3936,231.9874,19.1134},
	{1253.1971,341.6346,19.1137},
	{1049.5676,188.8388,32.4139},
	{584.2161,189.0732,20.1444},
	{365.8413,-93.4504,1.0828}
};

static RaceID; // this will hold the id this race is assigned at init

public race_dustyroads_init() // unique named init function (must be ran on gamemode init)
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

