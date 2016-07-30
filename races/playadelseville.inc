//
// Converted:   31.08.08
// Aurthor:    	Sneaky & G-sTyLeZzZ (original race name: PlayaDelSelville)
// Converter: 	MadCat
//

#if defined _race_playadelseville_included
  #endinput
#endif

#define _race_playadelseville_included
#pragma library race_playadelseville
#include "race"

static racename[MAX_NAME] = "Playa del Seville";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 269.5417;
static Float:racecheckpoints[][CoordInfo] = {
{2383.4169,-1256.5106,23.3766},
{2729.0866,-1274.6861,58.1529},
{2701.4233,-1506.0374,29.9756},
{2621.7141,-1444.3387,31.0482},
{2431.2102,-1470.4815,23.3983},
{2451.5742,-1732.3232,13.0667},
{2528.0051,-1757.6834,12.9483},
{2542.2026,-1933.1395,12.9484},
{2713.4394,-1959.6363,12.9254},
{2744.6818,-1993.8041,12.9387},
{2781.6826,-2018.9652,13.1046}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_playadelseville_init();

public race_playadelseville_init() // unique named init function (must be ran on gamemode init)
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

