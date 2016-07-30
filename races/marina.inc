//
// Converted:   22.10.07
// Aurthor:    	BetaMaster
// Converter: 	MadCat
//

#if defined _race_marina_included
  #endinput
#endif

#define _race_marina_included
#pragma library race_marina
#include "race"

forward race_marina_init();

static racename[MAX_NAME] = "Marina Dash";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_JETMAX,V_SPEEDER,V_SQUALO};
static Float:startlineheading = 265.8989;
static Float:racecheckpoints[][CoordInfo] = {
	{1605.3835,537.2941,-0.4996},
	{1856.2898,508.7566,-0.4909},
	{2177.9133,290.8115,-0.5147},
	{2005.4084,-84.8071,-0.5983},
	{1593.8789,-126.2781,-0.4949},
	{1229.3861,-152.1033,-0.5574},
	{723.8047,-209.1560,-0.5187},
	{294.7455,-354.4502,-0.5888},
	{-52.6780,-610.8663,-0.5423},
	{14.9497,-912.9983,-0.5903},
	{90.9849,-1148.8900,-0.4832},
	{43.3004,-1607.7220,-0.6484},
	{263.8876,-2011.0693,-0.3202},
	{899.0437,-1962.6243,-0.4830}
};

static RaceID; // this will hold the id this race is assigned at init

public race_marina_init() // unique named init function (must be ran on gamemode init)
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

