//
// Converted:   26.04.08
// Aurthor:    	BetaMaster
// Converter: 	MadCat
//

#if defined _race_jerry_included
  #endinput
#endif

#define _race_jerry_included
#pragma library race_jerry
#include "race"

forward race_jerry_init();

static racename[MAX_NAME] = "Jerry";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 270.0;
static Float:racecheckpoints[][CoordInfo] = {
	{-238.4895,2634.2556,63.0216},
	{265.7311,2738.7544,60.1957},
	{766.6691,2704.5486,48.0370},
	{783.9382,2240.9661,9.6757},
	{857.0721,1579.2692,17.3996},
	{734.7920,1110.5352,28.7389},
	{214.3634,1000.6898,27.6433},
	{373.0702,1493.9045,9.6383},
	{585.2939,1826.9283,14.3447},
	{561.2527,2276.3386,34.5613},
	{161.7783,2283.7639,22.0623},
	{-197.6888,2503.0015,25.7737},
	{-639.0822,2473.5417,75.6246},
	{-668.1237,2740.3530,58.7980},
	{-238.4895,2634.2556,63.0216}

};

static RaceID; // this will hold the id this race is assigned at init


public race_jerry_init() // unique named init function (must be ran on gamemode init)
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
