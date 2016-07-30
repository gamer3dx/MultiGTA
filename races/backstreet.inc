//
// Converted:   10.08.08
// Aurthor:    	GTAT Team
// Converter: 	MadCat
//

#if defined _race_backstreet_included
  #endinput
#endif

#define _race_backstreet_included
#pragma library race_backstreet
#include "race"

static racename[MAX_NAME] = "Backstreet";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_QUAD};
static Float:startlineheading = 270.0;
static Float:racecheckpoints[][CoordInfo] = {
{-2807.0000,-209.0000,6.6000},
{-2757.2156,-199.4456,6.5118},
{-2758.6777,-6.7023,6.3982},
{-2706.4063,0.2886,3.6628},
{-2687.5676,39.3545,3.6649},
{-2652.4919,49.7651,3.6595},
{-2650.4124,85.9286,3.5440},
{-2604.0117,105.8878,3.6665},
{-2613.7434,173.6033,3.8085},
{-2619.7188,217.5218,4.1223},
{-2716.7996,231.8622,3.8204},
{-2759.5396,230.6646,6.5598},
{-2757.2813,147.5332,6.5149},
{-2759.8928,85.1460,6.4015},
{-2653.9861,83.3718,3.5462},
{-2655.9294,40.7959,3.6661},
{-2706.5244,25.7448,3.6599},
{-2696.4690,-83.5939,3.8138},
{-2655.4790,-87.2394,3.6236},
{-2660.2847,-207.4700,3.7480},
{-2757.6943,-201.8109,6.5162},
{-2762.4229,228.5705,6.5573},
{-2809.0586,235.7288,6.5128},
{-2870.2004,465.6982,4.3106},
{-2991.4790,469.6839,4.3956}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_backstreet_init();

public race_backstreet_init() // unique named init function (must be ran on gamemode init)
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

