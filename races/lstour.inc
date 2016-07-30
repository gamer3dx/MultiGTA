//
// Converted:   10.05.08
// Aurthor:    	[T5]SpLendour
// Converter: 	MadCat
//

#if defined _race_lstour_included
  #endinput
#endif

#define _race_lstour_included
#pragma library race_lstour
#include "race"

static racename[MAX_NAME] = "Los-Santos Tour";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 0.1;
static Float:racecheckpoints[][CoordInfo] = {
{1057.938232,-1306.144409,13.087725},
{1056.933959,-1212.530151,16.719165},
{1098.050781,-1144.525390,23.282163},
{1180.395996,-1143.448242,23.312301},
{1283.894897,-1145.061767,23.283218},
{1355.847534,-1003.176269,27.067113},
{1385.062744,-869.546752,40.366893},
{1443.044067,-859.902648,51.863864},
{1511.872558,-823.106872,68.984703},
{1484.438232,-766.237792,88.875228},
{1407.336669,-688.773803,86.911567},
{1284.806152,-717.658081,92.921394},
{1172.187744,-759.371093,105.070831},
{985.815002,-804.454467,98.538246},
{880.007324,-858.670349,77.620895},
{776.700927,-906.691955,56.139881},
{739.010559,-942.304260,55.336101},
{715.077514,-940.029113,63.113098},
{704.655273,-934.918640,69.059867},
{682.853271,-905.663635,70.679550},
{656.552490,-890.230407,63.189891},
{555.625183,-899.849060,68.279991},
{486.228179,-1000.968566,91.032684},
{373.449401,-1019.031555,92.254981},
{210.615707,-1154.090820,61.061336},
{140.337326,-1252.627685,44.649822},
{174.678695,-1410.613037,45.132663},
{245.368499,-1335.059204,52.084136},
{573.743347,-1182.346435,44.482368},
{634.917968,-1104.980712,46.378372},
{741.628295,-954.812316,54.142879},
{866.314270,-866.074645,77.001968},
{954.119689,-819.843566,95.775413},
{1129.737182,-766.020385,108.609268},
{1245.602294,-747.715270,93.982391},
{1228.518310,-806.342041,87.892890},
{1242.776855,-827.005981,83.767723},
{1247.568603,-811.138977,83.769989}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_lstour_init();

public race_lstour_init() // unique named init function (must be ran on gamemode init)
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

