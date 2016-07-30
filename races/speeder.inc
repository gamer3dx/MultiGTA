//
// Converted:   10.05.08
// Aurthor:    	Switch
// Converter: 	MadCat
//

#if defined _race_speeder_included
  #endinput
#endif

#define _race_speeder_included
#pragma library race_speeder
#include "race"

static racename[MAX_NAME] = "Speeder";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_SPEEDER};
static Float:startlineheading = 159.1057;
static Float:racecheckpoints[][CoordInfo] = {
{2172.3281,238.7373,-0.5924},
{1998.2402,-196.7896,-0.4682},
{1700.8491,-13.7777,-0.5286},
{775.6961,-177.2213,-0.5293},
{-200.2351,-438.7097,-0.4298},
{-321.0355,-594.4092,-0.4446},
{-63.6141,-916.8986,-0.4716},
{69.6226,-1209.3696,-0.3828},
{54.3822,-1498.5344,-0.4234},
{-33.5629,-1802.9198,-0.7326},
{150.0131,-1974.3967,-0.5749},
{372.6609,-1978.8724,-0.4049},
{633.4167,-2000.1893,-0.4890},
{721.2785,-1923.3398,-0.6149},
{728.7688,-1530.3708,-0.4884}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_speeder_init();

public race_speeder_init() // unique named init function (must be ran on gamemode init)
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

