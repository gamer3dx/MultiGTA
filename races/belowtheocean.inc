//
// Converted:   20.09.09
// Aurthor:    	GTAT Team
// Converter: 	MadCat
//

#if defined _race_belowtheocean_included
  #endinput
#endif

#define _race_belowtheocean_included
#pragma library race_belowtheocean
#include "race"

static racename[MAX_NAME] = "Below The Ocean";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_PREDATOR, V_SQUALO, V_SPEEDER, V_REEFER, V_TROPIC, V_COASTG, V_DINGHY, V_MARQUIS, V_JETMAX, V_LAUNCH};
static Float:startlineheading = 96.65;
static Float:racecheckpoints[][CoordInfo] = {
	{2205.6067,-196.9662,-24.2243},
	{2091.0776,-191.8239,-27.2902},
	{1821.3916,-42.6723,-13.9356},
	{1655.7207,-7.1221,-5.5368},
	{1568.9159,-162.5826,-6.2072},
	{1417.9907,-256.7511,-8.4130},
	{1248.6377,-169.2113,-6.9809}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_belowtheocean_init();

public race_belowtheocean_init() // unique named init function (must be ran on gamemode init)
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

