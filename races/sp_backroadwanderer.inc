//
// Converted:   02.08.08
// Aurthor:     Rockstar North Team
// Converter: 	MadCat
//

#if defined _race_sp_bwanderer_included
  #endinput
#endif

#define _race_sp_bwanderer_included
#pragma library race_sp_backroadwanderer
#include "race"

static racename[MAX_NAME] = "(SP)Backroad Wanderer";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_FCR900,V_NRG500,V_BF400};
static Float:startlineheading = 305.0457;
static Float:racecheckpoints[][CoordInfo] = {
{261.7816,-1243.111,71.8851},
{312.9973,-1206.853,74.9057},
{440.3485,-1186.672,67.2076},
{623.8817,-1109.967,45.6186},
{745.0934,-951.0351,53.766},
{873.6384,-860.0975,76.5242},
{1053.818,-777.5837,104.7803},
{1248.368,-731.3474,93.7346},
{1372.402,-675.6946,92.6217},
{1328.011,-583.8703,93.4113},
{1167.314,-633.0795,102.7501},
{968.4116,-646.252,120.7201},
{791.077,-800.4396,65.0537},
{620.9355,-905.8031,62.1261},
{454.9853,-1013.546,92.0091},
{276.8912,-1104.159,80.0612},
{141.6012,-1247.267,43.792},
{192.4688,-1380.597,47.6608}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_sp_backroadwanderer_init();

public race_sp_backroadwanderer_init() // unique named init function (must be ran on gamemode init)
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

