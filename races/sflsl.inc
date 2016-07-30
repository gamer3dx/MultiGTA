//
// Converted:   25.04.08
// Aurthor:    	Sharon
// Converter: 	MadCat
//

#if defined _race_sflsl_included
  #endinput
#endif

#define _race_sflsl_included
#pragma library race_sflsl
#include "race"

forward race_sflsl_init();

static racename[MAX_NAME] = "SF to LS Long Race";
static racefrequency = 10;
static lineupdelay = 240;
static minracers = 6;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 180.0;
static Float:racecheckpoints[][CoordInfo] = {
{-1990.3601,153.6170,27.2661},
{-2009.7797,69.2410,28.3837},
{-1975.7734,-67.6699,27.7929},
{-1839.8068,-132.3302,5.5426},
{-1802.5867,-253.8629,18.6825},
{-1819.8679,-427.7473,14.6784},
{-1816.2679,-613.7869,15.8995},
{-1810.3801,-756.2398,32.3493},
{-1806.4540,-1006.0259,51.0233},
{-1696.7598,-1266.5023,51.2730},
{-1620.7424,-1382.7992,44.7439}, // 10
{-1534.1692,-1429.2837,40.2776},
{-1548.0287,-1288.8339,56.7801},
{-1654.3158,-1167.9126,71.4758},
{-1728.2670,-1044.5684,74.2870},
{-1719.6776,-826.9254,79.8623},
{-1628.3833,-899.2939,98.2478},
{-1594.0787,-1147.9567,102.5886},
{-1450.5111,-1322.8582,100.1931},
{-1362.4386,-1409.2667,109.9431},
{-1197.7253,-1345.5355,123.8744}, // 20
{-1022.6257,-1374.4321,130.0611},
{-923.9908,-1423.5220,125.7693},
{-900.3961,-1532.3795,118.8867},
{-831.4485,-1582.2535,125.4566},
{-875.8090,-1625.1580,111.1698},
{-962.8235,-1660.4165,74.6497},
{-940.1316,-1719.5734,77.2974},
{-883.0159,-1789.3279,86.5991},
{-864.4470,-1871.4272,88.2230},
{-976.2590,-1924.6415,80.1079}, // 30
{-1048.1400,-2063.3799,54.2702},
{-1064.3009,-2132.0933,36.0345},
{-1010.8696,-2156.6829,34.3983},
{-960.0522,-2239.1252,43.6203},
{-976.4098,-2359.8103,65.3603},
{-1069.6943,-2373.4163,46.3754},
{-1204.6019,-2352.2883,17.7135},
{-1276.5811,-2233.5452,22.6317},
{-1371.3423,-2168.2358,21.3345},
{-1541.1312,-2156.6606,7.0893}, // 40
{-1673.2211,-2199.5938,34.4280},
{-1736.5348,-2304.9636,45.8087},
{-1860.5889,-2372.0259,32.3629},
{-1866.4519,-2440.5959,31.9021},
{-1770.1814,-2497.8313,9.3057},
{-1718.9067,-2568.8965,15.7841},
{-1672.3121,-2606.9341,39.0048},
{-1589.5479,-2636.7388,53.1287},
{-1440.2522,-2630.7166,34.8299},
{-1320.2330,-2632.7898,15.7668}, // 50
{-1226.7344,-2636.4548,9.1865},
{-1128.7433,-2648.6553,15.0241},
{-1043.8824,-2682.0913,44.5243},
{-954.6475,-2669.0112,73.0234},
{-861.2192,-2673.1711,98.0948},
{-740.1964,-2658.6570,84.1249},
{-687.4394,-2559.7947,55.2508},
{-636.4093,-2442.9221,31.1570},
{-563.0577,-2345.3379,27.6131},
{-487.1831,-2269.7053,38.8291}, // 60
{-379.8187,-2261.1157,45.2420},
{-304.7767,-2229.2168,28.9495},
{-305.0233,-2163.7126,28.0626},
{-340.7131,-1912.7820,24.0688},
{-280.6056,-1736.3497,14.3037},
{-221.3721,-1587.8815,15.2740},
{-109.1177,-1455.6589,12.5318},
{2.7392,-1431.5649,7.7268},
{-7.2967,-1524.9623,2.1531},
{153.0822,-1587.5604,12.2430},
{284.3235,-1705.2118,7.0683},
{479.9966,-1721.2637,10.8367},
{651.0036,-1752.6079,13.0304},
{770.9557,-1784.3834,12.6553},
{830.8518,-1835.3380,12.1716},
{832.2042,-2065.6025,12.5943,}
};

static RaceID; // this will hold the id this race is assigned at init

public race_sflsl_init() // unique named init function (must be ran on gamemode init)
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

