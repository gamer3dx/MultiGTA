//
// Converted:   02.08.08
// Aurthor:     Rockstar North Team
// Converter: 	MadCat
//

#if defined _race_sp_lowrider_included
  #endinput
#endif

#define _race_sp_lowrider_included
#pragma library race_sp_lowrider
#include "race"

static racename[MAX_NAME] = "(SP)Lowrider";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_BLADE,V_MAJESTIC,V_REMINGTN};
static Float:startlineheading = 270.0;
static Float:racecheckpoints[][CoordInfo] = {
{1492.407,-1872.365,15.8},
{1384.262,-1872.015,12.3828},
{1075.442,-1851.978,12.391},
{647.3663,-1734.185,12.4844},
{627.2474,-1228.55,16.9893},
{497.0043,-1282.818,14.6513},
{500.412,-1326.649,14.8178},
{401.0033,-1404.457,32.8973},
{420.3067,-1450.714,29.5842},
{329.1244,-1631.112,32.1247},
{366.4376,-1647.348,31.7176},
{369.98,-2030.34,5.55}

};

static RaceID; // this will hold the id this race is assigned at init

forward race_sp_lowrider_init();

public race_sp_lowrider_init() // unique named init function (must be ran on gamemode init)
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

