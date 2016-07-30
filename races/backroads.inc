//
// Converted:   01.11.07
// Aurthor:    	BetaMaster
// Converter: 	MadCat
//

#if defined _race_backroads_included
  #endinput
#endif

#define _race_backroads_included
#pragma library race_backroads
#include "race"

forward race_backroads_init();

static racename[MAX_NAME] = "Back Roads";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 169.3398;
static Float:racecheckpoints[][CoordInfo] = {
{-110.4382,-97.4268,2.7422},
{-35.6646,-202.7928,1.2197},
{240.9247,-212.8593,1.0580},
{352.0965,-94.3385,0.9557},
{434.7423,12.2750,7.1566},
{525.5955,80.8106,21.0914},
{555.4393,165.9326,24.8435},
{807.4002,214.9393,37.1730},
{1015.1445,174.8448,30.0085},
{1166.1698,302.5778,17.9060},
{1314.3752,235.9252,19.0304},
{1425.5968,207.6501,18.1238},
{1816.6566,192.7651,29.6226},
{2073.2700,104.1172,27.1506},
{2394.0955,67.1737,25.9608},
{2638.5061,28.4389,26.3405},
{2689.5637,-163.7186,34.8071},
{2683.5525,-413.7129,53.2850},
{2758.5767,-736.6437,56.7092},
{2663.5098,-926.9838,73.8974},
{2459.0234,-970.7441,79.7118},
{2339.6152,-1047.1232,52.2521}

};

static RaceID; // this will hold the id this race is assigned at init

public race_backroads_init() // unique named init function (must be ran on gamemode init)
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

