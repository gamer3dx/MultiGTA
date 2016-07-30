//
// Converted:   02.08.08
// Aurthor:     Rockstar North Team
// Converter: 	MadCat
//

#if defined _race_sp_vinewood_included
  #endinput
#endif

#define _race_sp_vinewood_included
#pragma library race_sp_vinewood
#include "race"

static racename[MAX_NAME] = "(SP)Vinewood";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_SUNRISE,V_NEBULA,V_SENTINEL,V_WINDSOR,V_BLISTAC};
static Float:startlineheading = 0.0;
static Float:racecheckpoints[][CoordInfo] = {
{1357.656,-1361.67,12.3828},
{1357.689,-1186.404,20.2744},
{1369.741,-1000.526,27.02},
{1505.818,-868.7814,61.0462},
{1452.794,-711.5325,89.8766},
{1253.572,-729.2587,93.4292},
{1059.306,-775.9847,105.3241},
{878.665,-858.1913,76.8826},
{721.7916,-980.4561,51.9555},
{571.0251,-1045.203,72.2876},
{373.2952,-1075.644,72.8049},
{288.4963,-1249.747,72.721},
{481.2224,-1240.976,19.3512},
{518.334,-1377.603,14.9457},
{448.0064,-1540.832,27.8796},
{487.4595,-1661.575,20.74},
{687.2253,-1673.48,10.7959},
{866.4786,-1584.698,12.3828},
{1066.729,-1572.305,12.375},
{1261.214,-1572.243,12.3906}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_sp_vinewood_init();

public race_sp_vinewood_init() // unique named init function (must be ran on gamemode init)
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

