//
// Converted:   02.08.08
// Aurthor:     Rockstar North Team
// Converter: 	MadCat
//

#if defined _race_sp_gogokarting_included
  #endinput
#endif

#define _race_sp_gogokarting_included
#pragma library race_sp_gogokarting
#include "race"

static racename[MAX_NAME] = "(SP)Go-Go Karting";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_KART};
static Float:startlineheading = 314.7592;
static Float:racecheckpoints[][CoordInfo] = {
{-2728.309,-310.8731,6.0391},
{-2668.671,-251.6207,5.4984},
{-2655.978,-104.6751,2.9982},
{-2460.835,-69.9766,31.2},
{-2421.263,55.6089,34.0156},
{-2545.26,137.9111,15.2177},
{-2604.136,136.8579,3.1797},
{-2558.661,245.625,10.7931},
{-2578.504,345.8331,7.2168},
{-2706.282,317.6024,3.1797},
{-2809.244,232.3965,6.0313},
{-2757.749,138.2248,5.9033},
{-2758.39,-43.991,6.1263},
{-2756.892,-184.0135,5.8843},
{-2812.464,-299.5974,6.039}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_sp_gogokarting_init();

public race_sp_gogokarting_init() // unique named init function (must be ran on gamemode init)
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

