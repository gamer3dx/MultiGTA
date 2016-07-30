//
// Converted:   20.09.09
// Aurthor:    	GTAT Team
// Converter: 	MadCat
//

#if defined _race_beachtour_included
  #endinput
#endif

#define _race_beachtour_included
#pragma library race_beachtour
#include "race"

static racename[MAX_NAME] = "Beach Tour";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {};
static Float:startlineheading = 0.0;
static Float:racecheckpoints[][CoordInfo] = {
	{834.3040,-2021.4475,12.3261},
	{811.0192,-1815.1306,12.4976},
	{796.3671,-1843.8837,7.4596},
	{691.9362,-1844.6904,6.5254},
	{426.2700,-1777.0004,4.8476},
	{381.0976,-1805.8403,7.3136},
	{371.0703,-1656.9355,32.2114},
	{243.1379,-1613.9680,32.5746},
	{282.7295,-1501.5579,32.0916},
	{193.9565,-1523.6023,12.3738},
	{114.1042,-1628.1160,9.6587},
	{152.2227,-1753.1282,4.4970},
	{386.2919,-1784.1276,5.0763},
	{373.8014,-1815.3240,7.1918},
	{370.3465,-2034.2943,7.1534}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_beachtour_init();

public race_beachtour_init() // unique named init function (must be ran on gamemode init)
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

