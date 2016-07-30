//
// Created:     26.09.06
// Aurthor:    Iain Gilbert
//

#if defined _race_mullholland_included
  #endinput
#endif

#define _race_mullholland_included
#pragma library race_mullholland
#include "race"

static racename[MAX_NAME] = "Mullholland Getaway";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 148.4892;
static Float:racecheckpoints[][CoordInfo] = {
{1244.7372,-820.5668,83.9473}, // race_mullholland
{1245.2424,-769.0816,91.6533},
{1353.1168,-685.2146,92.0110},
{1399.3284,-594.3857,78.6258},
{1169.5573,-631.0978,103.5522},
{955.6980,-655.6189,120.8480},
{757.2582,-804.3942,66.0920},
{672.8386,-889.9088,66.1142},
{738.2806,-958.3984,54.0768},
{691.2849,-991.9446,51.8564},
{681.5355,-674.7605,15.9911},
{700.1636,-465.7200,16.1380}
};

static RaceID;

forward race_mullholland_init();

public race_mullholland_init()
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

