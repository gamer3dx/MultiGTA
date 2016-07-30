//
// Converted:   10.08.08
// Aurthor:    	GTAT Team
// Converter: 	MadCat
//

#if defined _race_countryvillage_included
  #endinput
#endif

#define _race_countryvillage_included
#pragma library race_countryvillage
#include "race"

static racename[MAX_NAME] = "Country Village";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 225.0;
static Float:racecheckpoints[][CoordInfo] = {
{-295.1542,1573.3831,76.0111},
{-301.8010,1499.8794,75.6499},
{-305.9216,1395.1843,72.3068},
{-366.9702,1464.3815,63.1366},
{-324.9794,1317.3221,52.8868},
{-433.9323,1443.0209,33.6285},
{-341.0801,1263.0487,22.8778},
{-141.9965,1253.7776,20.2213},
{-117.6443,1155.4805,20.2451},
{-181.9250,1147.8494,20.2470},
{-193.2410,975.0428,19.3913},
{-276.0736,819.8696,15.0756},
{-52.8378,856.6041,18.4121},
{232.0607,973.6594,28.8440},
{346.9406,1007.8685,29.0481},
{359.1220,843.0159,20.4902},
{551.0827,738.0080,12.9891},
{735.3276,727.9520,29.5656}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_countryvillage_init();

public race_countryvillage_init() // unique named init function (must be ran on gamemode init)
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

