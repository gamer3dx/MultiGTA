//
// Converted:   10.08.08
// Aurthor:    	GTAT Team
// Converter: 	MadCat
//

#if defined _race_roadtohell_included
  #endinput
#endif

#define _race_roadtohell_included
#pragma library race_roadtohell
#include "race"

static racename[MAX_NAME] = "Road To Hell";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_SANCHEZ};
static Float:startlineheading = 194.0;
static Float:racecheckpoints[][CoordInfo] = {
{-2288.8967,-1704.7334,477.6903},
{-2290.3140,-1767.6876,453.0849},
{-2444.7805,-1721.6613,432.0761},
{-2439.5166,-1807.0177,410.1805},
{-2545.5291,-1669.4901,400.6763},
{-2531.2400,-1800.1287,375.7292},
{-2591.5823,-1628.7065,344.1844},
{-2519.5920,-1891.7455,295.5653},
{-2650.8657,-1729.1810,257.7695},
{-2440.5220,-2083.0017,124.5757},
{-2597.6882,-2084.7253,131.5249},
{-2788.6501,-1781.4399,140.8068},
{-2768.2344,-1908.5768,116.4500},
{-2681.7615,-2060.1692,79.4961},
{-2633.3745,-2156.8411,70.6789},
{-2439.5945,-2135.6926,59.0765},
{-2327.1309,-2176.3748,36.3187},
{-2390.3459,-2202.4233,32.9582}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_roadtohell_init();

public race_roadtohell_init() // unique named init function (must be ran on gamemode init)
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

