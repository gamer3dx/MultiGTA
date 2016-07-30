//
// Converted:   27.04.08
// Aurthor:    	Switch
// Converter: 	MadCat
//

#if defined _race_resign_included
  #endinput
#endif

#define _race_resign_included
#pragma library race_resign
#include "race"

static racename[MAX_NAME] = "Resign";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 268.124328;
static Float:racecheckpoints[][CoordInfo] = {
{974.93719,-1220.7427,16.08133},
{1006.1689,-1220.9491,16.2471},
{1062.4503,-1194.7141,19.3173},
{1076.8715,-1137.2945,23.1392},
{1078.6150,-1126.0304,23.2811},
{1094.7816,-1006.3018,35.2889},
{1160.4100,-1013.2400,33.9643},
{1204.7186,-1061.7125,29.2327},
{1215.3063,-1167.2137,22.3825},
{1196.3456,-1278.5696,12.8267},
{1115.0056,-1281.2150,13.0317},
{1038.6140,-1323.4547,12.8770},
{993.1263,-1373.7225,12.8094},
{959.2253,-1424.0623,13.0278},
{955.9920,-1515.0447,13.0372},
{953.2468,-1565.0441,13.0436},
{917.3477,-1589.5106,12.8640},
{892.6718,-1662.7319,13.0281},
{859.0109,-1698.3864,13.0337},
{805.8491,-1814.2603,12.5061},
{700.2075,-1802.9972,11.9489},
{615.2578,-1779.8561,13.7594},
{562.5355,-1770.0513,5.2907},
{464.5749,-1759.3885,5.1472},
{424.5989,-1716.2426,8.9011},
{228.2158,-1653.2490,11.6972},
{166.0582,-1555.0018,11.2326},
{207.0750,-1521.0518,15.2309},
{312.6575,-1474.0096,33.0246},
{398.2770,-1423.5264,33.1566},
{394.0118,-1520.1317,31.7545},
{455.9608,-1579.6277,24.7857},
{552.1781,-1561.9960,15.3663},
{558.2655,-1420.2988,14.8961},
{626.2953,-1375.5494,12.9930},
{664.9439,-1305.4113,12.9422},
{715.9057,-1206.4670,17.8632},
{815.8288,-1148.4673,23.3683},
{941.4568,-1159.9495,23.0095},
{914.9048,-1220.8583,16.4569}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_resign_init();

public race_resign_init() // unique named init function (must be ran on gamemode init)
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

