//
// Converted:   20.09.09
// Aurthor:    	GTAT Team
// Converter: 	MadCat
//

#if defined _race_crossways_included
  #endinput
#endif

#define _race_crossways_included
#pragma library race_crossways
#include "race"

static racename[MAX_NAME] = "Crossways";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {};
static Float:startlineheading = 90.0;
static Float:racecheckpoints[][CoordInfo] = {
{-1754.3436,926.6018,24.4716},
{-2004.2781,908.5661,45.0289},
{-2022.2596,565.8070,34.7427},
{-2142.3899,542.8919,34.7427},
{-2134.1121,208.7781,35.0319},
{-1910.0372,234.5699,34.7410},
{-1903.2134,264.9237,40.7231},
{-1885.1327,223.4241,38.7118},
{-1836.6261,319.5524,38.2602},
{-1712.1949,471.9021,37.9818},
{-1864.5502,414.5244,16.7347},
{-1994.1348,209.0420,27.4145},
{-1939.9012,223.7026,24.4775},
{-1655.1564,476.1320,19.7324},
{-1559.0381,599.4666,6.7661},
{-1632.0706,682.5699,6.9158},
{-1583.4519,717.2373,-5.5167},
{-1632.3821,650.7613,-5.5151}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_crossways_init();

public race_crossways_init() // unique named init function (must be ran on gamemode init)
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

