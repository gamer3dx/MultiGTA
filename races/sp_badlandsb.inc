//
// Converted:   02.08.08
// Aurthor:     Rockstar North Team
// Converter: 	MadCat
//

#if defined _race_sp_badlandsb_included
  #endinput
#endif

#define _race_sp_badlandsb_included
#pragma library race_sp_badlandsb
#include "race"

static racename[MAX_NAME] = "(SP)Badlands B";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_ZR350,V_ALPHA,V_SULTAN};
static Float:startlineheading = 90.7717;
static Float:racecheckpoints[][CoordInfo] = {
{-544.8638,-189.4458,81.8383},
{-728.9467,-173.5218,65.1248},
{-826.881,-167.7603,64.99},
{-759.2693,-92.0455,64.8686},
{-716.4521,5.1206,59.1055},
{-885.785,-41.3927,33.2347},
{-738.1151,25.555,32.2336},
{-770.0811,130.1324,9.4651},
{-715.4454,225.1288,12.2899},
{-624.6024,187.2405,15.4381},
{-520.6511,79.6011,32.2117},
{-507.8417,-45.9077,59.2556},
{-618.4406,-67.8995,62.3489},
{-658.3503,146.4855,28.2693},
{-507.6872,221.0444,8.7633},
{-329.7878,173.921,5.5},
{-149.4881,189.6471,7.2644},
{-38.4818,140.8613,2.1234},
{-114.231,-132.3417,2.1236},
{22.0727,-207.7681,2.5827},
{182.497,-217.8004,2.4216},
{207.4451,-298.923,2.4375},
{279.6441,-378.3416,7.9003},
{419.4744,-302.5507,6.446},
{573.52,-202.8494,13.7118},
{748.4567,-150.8298,18.3722},
{820.4231,-111.5991,23.2476},
{1000.324,-80.3516,21.0326},
{1187.393,-83.879,35.2576},
{1265.846,-153.0857,36.8097},
{1434.738,-214.9645,7.3527},
{1548.962,-146.2702,16.4932},
{1559.983,17.468,23.1563}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_sp_badlandsb_init();

public race_sp_badlandsb_init() // unique named init function (must be ran on gamemode init)
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

