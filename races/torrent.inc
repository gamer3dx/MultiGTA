//
// Converted:   27.04.08
// Aurthor:    	Switch
// Converter: 	MadCat
//

#if defined _race_torrent_included
  #endinput
#endif

#define _race_torrent_included
#pragma library race_torrent
#include "race"

static racename[MAX_NAME] = "Torrent";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 358.443847;
static Float:racecheckpoints[][CoordInfo] = {
{-2003.2261,443.0197,34.7185},
{-1973.8206,595.4370,34.8762},
{-1872.5399,605.8795,34.7185},
{-1716.3418,643.4667,24.4426},
{-1715.3770,767.6246,24.4373},
{-1742.5040,841.1639,24.4400},
{-1834.6016,849.0295,31.2851},
{-1892.1844,868.2017,34.7811},
{-1889.3761,1001.8105,37.1503},
{-1884.9087,1191.3497,44.8839},
{-1722.9268,1309.6641,6.7435},
{-2050.6836,1315.4292,6.9624},
{-2393.9653,1377.2676,6.7709},
{-2852.2004,1230.4454,5.2875},
{-2808.0178,878.8523,43.6092},
{-2750.4714,824.8061,53.5253},
{-2751.3105,914.5374,61.2649},
{-2710.4868,1101.1232,44.9363},
{-2536.2666,1244.7742,34.7225},
{-2473.1587,1205.5818,34.8020},
{-2649.9124,1190.7092,55.1317},
{-2436.3542,1094.1882,55.2818},
{-2146.5586,1055.0726,55.2844},
{-2008.0594,1032.7104,55.2745},
{-2004.9968,783.2869,44.9998},
{-2003.8403,561.3269,34.7199},
{-2006.7401,165.5239,27.2420}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_torrent_init();

public race_torrent_init() // unique named init function (must be ran on gamemode init)
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

