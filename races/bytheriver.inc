//
// Converted:   10.08.08
// Aurthor:    	GTAT Team
// Converter: 	MadCat
//

#if defined _race_bytheriver_included
  #endinput
#endif

#define _race_bytheriver_included
#pragma library race_bytheriver
#include "race"

static racename[MAX_NAME] = "By The River";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_PREDATOR, V_SQUALO, V_SPEEDER, V_REEFER, V_TROPIC, V_COASTG, V_DINGHY, V_MARQUIS, V_JETMAX, V_LAUNCH};
static Float:startlineheading = 270.0;
static Float:racecheckpoints[][CoordInfo] = {
{-1630.0000,-728.0000,0.0000},
{-1218.0105,-695.1329,-0.4375},
{-973.0982,-372.0755,-0.3623},
{-313.1291,-382.9452,-0.5265},
{-28.0271,-578.7032,-0.4599},
{344.7602,-312.4796,-0.5299},
{1038.0355,-108.7877,-0.4634},
{1439.1934,-244.2360,-0.5192},
{1732.9988,-18.6970,-0.4622},
{2100.1648,24.6768,-0.5150},
{2166.2598,343.7691,-0.3835},
{2323.7695,527.8209,-0.4271}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_bytheriver_init();

public race_bytheriver_init() // unique named init function (must be ran on gamemode init)
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

