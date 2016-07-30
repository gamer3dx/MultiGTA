//
// Converted:   26.04.08
// Aurthor:    	BetaMaster
// Converter: 	MadCat
//

#if defined _race_pace_included
  #endinput
#endif

#define _race_pace_included
#pragma library race_pace
#include "race"

forward race_pace_init();

static racename[MAX_NAME] = "Pace";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 344.2663;
static Float:racecheckpoints[][CoordInfo] = {
	{435.0856,-529.4698,42.5854},
	{478.6890,-371.6828,28.8377},
	{528.2913,-122.0558,37.1176},
	{525.0408,245.8118,14.1129},
	{649.2278,310.5838,19.4539},
	{933.0985,373.4023,19.4416},
	{1102.7587,576.3670,19.4540},
	{1388.8677,439.8092,19.4410},
	{1745.0463,386.3632,19.2303},
	{1997.3301,348.0247,27.3102},
	{2147.7876,240.4559,14.0727},
	{2344.0476,228.7420,25.8998},
	{2315.3071,283.9052,26.8515},
	{1932.1979,286.6570,31.4463},
	{1711.5120,306.9321,20.2171},
	{1768.0015,580.3362,23.3722},
	{1764.0168,835.8906,9.7988},
	{1426.9980,835.3503,6.3840},
	{1063.9213,782.1961,10.2507}

};

static RaceID; // this will hold the id this race is assigned at init


public race_pace_init() // unique named init function (must be ran on gamemode init)
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
