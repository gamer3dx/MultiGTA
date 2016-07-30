//
// Converted:   20.09.09
// Aurthor:    	GTAT Team
// Converter: 	MadCat
//

#if defined _race_toarea51_included
  #endinput
#endif

#define _race_toarea51_included
#pragma library race_toarea51
#include "race"

static racename[MAX_NAME] = "To Area 51";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {};
static Float:startlineheading = 258.0;
static Float:racecheckpoints[][CoordInfo] = {
	{-1386.7059,1946.8558,49.2371},
	{-1141.8184,1800.9583,40.4455},
	{-1008.4037,1855.4778,62.3944},
	{-899.3586,1791.7980,59.9049},
	{-881.4645,1971.3064,59.8437},
	{-483.0971,2000.7531,59.9161},
	{-460.8674,2087.0664,56.1567},
	{-371.2409,2175.8552,41.7796},
	{-265.9287,2222.0911,58.5820},
	{-90.6437,2120.5342,16.7632},
	{96.6732,1920.5974,17.8728},
	{213.2076,1901.4792,17.2072},
	{213.0910,1866.9298,12.8677}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_toarea51_init();

public race_toarea51_init() // unique named init function (must be ran on gamemode init)
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

