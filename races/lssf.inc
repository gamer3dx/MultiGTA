//
// Converted:   10.05.08
// Aurthor:    	Sharon
// Converter: 	MadCat
//

#if defined _race_lssf_included
  #endinput
#endif

#define _race_lssf_included
#pragma library race_lssf
#include "race"

forward race_lssf_init();

static racename[MAX_NAME] = "LS to SF Quick Race";
static racefrequency = 10;
static lineupdelay = 240;
static minracers = 6;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 180.0;
static Float:racecheckpoints[][CoordInfo] = {
{2830.3704,-2006.1525,10.8819}, // 0
{2728.4426,-2162.4199,10.8737},
{2509.1643,-2164.5239,13.2787},
{2273.3535,-2240.9756,13.5284},
{2180.5083,-2181.9573,13.1470},
{2080.1328,-2171.8811,13.1517},
{1857.8047,-2164.5645,13.1542},
{1699.8956,-2162.8660,15.5439},
{1531.8489,-1923.6692,15.5980},
{1449.8094,-1869.7650,13.1627},
{1315.3521,-1796.6781,13.1542}, // 10
{1249.0037,-1713.7898,13.1542},
{1125.8906,-1749.6132,13.3423},
{1061.1483,-1795.9496,13.4460},
{960.6675,-1795.6539,13.8809},
{832.8568,-1842.2373,12.4003},
{831.1008,-2046.0332,12.6391},
{836.9288,-1849.4155,12.6004},
{721.7313,-1759.4833,14.1835},
{665.2725,-1740.2241,13.2500},
{411.7974,-1703.6847,8.9502}, // 20
{212.7084,-1635.7310,13.5680},
{35.8653,-1524.8021,5.0112},
{-129.6240,-1446.4982,2.5010},
{-144.6089,-1278.2548,2.4693},
{-135.3726,-1117.3335,3.8553},
{-149.6926,-1051.1798,1.9938},
{-259.6500,-1032.2034,34.3076},
{-423.9464,-1063.1532,54.0971},
{-465.1960,-978.9290,25.1828},
{-337.3862,-787.4973,31.7616}, // 30
{-322.2798,-863.4548,46.6655},
{-488.3389,-869.2460,52.0550},
{-752.8486,-1004.4107,76.7761},
{-910.5651,-1094.6915,97.9786},
{-1171.5229,-826.6508,65.7440},
{-1280.2646,-798.6162,69.1573},
{-1557.3560,-813.2859,52.5223},
{-1747.1089,-712.8826,29.3167},
{-1813.2867,-584.3546,16.1128},
{-1820.9955,-535.1839,14.9030}, // 40
{-1798.6604,-196.7542,14.4349},
{-1840.3811,-116.5923,5.2636},
{-1939.3480,-65.0292,25.3293},
{-2006.1570,-33.7418,34.8853},
{-2006.0947,152.4176,27.3104},
{-1937.9641,224.1685,33.9232}
}; // 46

static RaceID; // this will hold the id this race is assigned at init

public race_lssf_init() // unique named init function (must be ran on gamemode init)
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

