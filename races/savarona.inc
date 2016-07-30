//
// Converted:   10.05.08
// Aurthor:    	[T5]SpLendour
// Converter: 	MadCat
//

#if defined _race_savarona_included
  #endinput
#endif

#define _race_savarona_included
#pragma library race_savarona
#include "race"

static racename[MAX_NAME] = "Savarona";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 91.0;
static Float:racecheckpoints[][CoordInfo] = {
{2520.540039,1714.939819,10.534781},
{2409.137451,1713.353515,10.451227},
{2325.629394,1634.488525,10.536562},
{2310.218261,1471.941162,10.691179},
{2252.301513,1371.259155,10.622143},
{2045.277221,1335.451049,10.546524},
{1962.698120,1272.451538,10.544399},
{1797.530029,1274.155029,13.808528},
{1645.135375,1231.520996,10.548028},
{1582.419189,1133.670288,10.544239},
{1506.178100,989.962158,10.759756},
{1568.948364,876.979125,12.387019},
{1540.088745,654.611083,10.551517},
{1344.483642,712.656738,10.640743},
{1341.102294,844.655761,6.675293},
{1245.727416,965.714904,6.682555},
{1229.582885,1251.298095,6.599437},
{1199.656127,1700.545532,6.576948},
{1156.070312,1865.184936,9.740101},
{1215.889892,1976.475219,6.594070},
{1280.575927,2232.084716,9.225488},
{1268.663818,2425.740478,15.255013},
{1056.676147,2518.518066,10.597621},
{911.223144,2394.334716,10.721316},
{906.087951,2260.556396,10.539488},
{906.601135,2000.030883,10.542407},
{906.532104,1812.626953,10.696043},
{905.507141,1735.861083,13.811216}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_savarona_init();

public race_savarona_init() // unique named init function (must be ran on gamemode init)
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

