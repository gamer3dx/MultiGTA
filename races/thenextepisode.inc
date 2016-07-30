//
// Converted:   31.08.08
// Aurthor:    	Sneaky & G-sTyLeZzZ
// Converter: 	MadCat
//

#if defined _race_thenextepisode_included
  #endinput
#endif

#define _race_thenextepisode_included
#pragma library race_thenextepisode
#include "race"

static racename[MAX_NAME] = "The Next Episode";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 6.6760;
static Float:racecheckpoints[][CoordInfo] = {
{2041.3740,-1610.3919,12.9943},
{1954.1833,-1499.8421,2.9547},
{1715.3852,-1423.9581,12.9933},
{1751.6232,-1296.1260,13.0777},
{1927.4187,-1261.5832,16.5711},
{2068.8229,-1220.1429,23.4310},
{1997.7226,-1135.5924,25.0012},
{1849.9644,-1196.5407,22.5514},
{1848.4901,-1474.5122,12.9967},
{1821.6971,-1790.2397,12.9922},
{1792.6225,-1913.3776,13.0052}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_thenextepisode_init();

public race_thenextepisode_init() // unique named init function (must be ran on gamemode init)
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

