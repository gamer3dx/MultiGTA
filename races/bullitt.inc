//
// Converted:   26.04.08
// Aurthor:    	BetaMaster
// Converter: 	MadCat
//

#if defined _race_bullitt_included
  #endinput
#endif

#define _race_bullitt_included
#pragma library race_bullitt
#include "race"

forward race_bullitt_init();

static racename[MAX_NAME] = "Bullitt";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 0.0;
static Float:racecheckpoints[][CoordInfo] = {
	{-2255.3508,-9.0412,34.7392},
	{-2251.5623,222.1380,34.7310},
	{-2298.5608,408.8454,34.5772},
	{-2386.5610,606.7647,29.1675},
	{-2387.0933,938.8635,44.8636},
	{-2400.5325,1085.9475,55.1451},
	{-2603.2244,1122.9561,55.0181},
	{-2634.6245,1211.5518,54.1889},
	{-2459.3269,1199.3387,34.6738},
	{-2278.6982,1176.0835,55.1090},
	{-2141.6023,1133.1025,67.4458},
	{-2143.4910,883.4421,79.4120},
	{-2143.1716,704.8212,68.9809},
	{-2143.6868,477.9875,34.5826},
	{-2146.5952,170.4455,34.7388},
	{-2166.8662,21.9151,34.7385},
	{-2197.9082,-69.9361,34.7389},
	{-2255.7212,-52.7203,34.7389},
	{-2255.3508,-9.0412,34.7392}

};

static RaceID; // this will hold the id this race is assigned at init


public race_bullitt_init() // unique named init function (must be ran on gamemode init)
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
