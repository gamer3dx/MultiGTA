//
// Converted:   21.10.07
// Aurthor:    	BetaMaster
// Converter: 	MadCat
//

#if defined _race_fastlane_included
  #endinput
#endif

#define _race_fastlane_included
#pragma library race_fastlane
#include "race"

forward race_fastlane_init();

static racename[MAX_NAME] = "FastLane";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 108.9946;
static Float:racecheckpoints[][CoordInfo] = {
	{960.4440,733.6182,10.4246},
	{702.5709,657.0952,9.2782},
	{381.6338,755.2902,5.8612},
	{-73.7830,584.2697,13.7547},
	{-569.7733,616.8351,16.3647},
	{-888.3674,846.1666,19.1802},
	{-1125.6389,1517.3347,22.2438},
	{-1612.1970,1782.3490,9.9386},
	{-1743.8693,2172.2529,16.6304},
	{-2007.6545,2572.9683,54.8235},
	{-2695.5010,2552.4841,76.3446},
	{-2714.0632,2360.6011,71.1038},
	{-2628.7134,2506.1189,27.8944},
	{-2488.9316,2428.6089,16.0156},
	{-2276.6877,2354.9668,4.6336}

};

static RaceID; // this will hold the id this race is assigned at init


public race_fastlane_init() // unique named init function (must be ran on gamemode init)
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
