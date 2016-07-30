//
// Converted:   25.04.08
// Aurthor:    	Sharon
// Converter: 	MadCat
//

#if defined _race_sels_included
  #endinput
#endif

#define _race_sels_included
#pragma library race_sels
#include "race"

forward race_sels_init();

static racename[MAX_NAME] = "South East LS Urban";
static racefrequency = 10;
static lineupdelay = 240;
static minracers = 6;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 359.7592;
static Float:racecheckpoints[][CoordInfo] = {
{2879.3225,-1583.5741,10.8750},
{2905.7305,-1335.2546,10.5547},
{2871.4705,-1180.6080,10.5377},
{2848.6416,-1138.6804,11.6500},
{2569.8489,-1152.6962,46.2913},
{2399.3735,-1151.2476,29.0491},
{2390.3049,-1176.3071,27.9192},
{2448.0286,-1196.0195,35.9330},
{2435.5056,-1258.8585,23.4922},
{2366.7610,-1274.4226,23.5007},
{2338.8882,-1253.6696,22.1559},
{2319.4875,-1218.5510,22.7831},
{2240.1240,-1220.2623,23.4683},
{2171.9775,-1240.4435,23.4790},
{2166.6509,-1343.1362,23.4841},
{2214.0779,-1404.6652,23.4797},
{2211.6223,-1572.3816,22.8907},
{2210.7568,-1732.5402,13.0575},
{2211.5625,-1986.3619,12.9934},
{2273.6104,-2088.7471,13.0964},
{2180.6953,-2179.5469,13.0212},
{2129.9663,-2214.2600,13.0413},
{2021.9941,-2165.3818,13.0415},
{1973.5796,-2252.4993,13.2052},
{1973.4224,-2487.8809,13.1980},
{2088.1787,-2503.3677,13.1988},
{2085.0588,-2590.0779,13.2065},
{1416.2345,-2586.7625,13.2077},
{1340.5222,-2588.3706,13.2156},
{1395.1750,-2671.0166,13.0314},
{2098.4458,-2684.5994,13.1554},
{2188.8677,-2499.1870,13.0798},
{2235.2678,-2427.2017,13.0546},
{2315.8169,-2331.4607,13.0395},
{2277.4241,-2247.3799,13.3572},
{2265.1838,-2192.9033,8.9916},
{2250.3750,-2144.6750,6.1890},
{2098.3960,-1988.4474,7.6441},
{1989.1896,-1874.5391,6.0576},
{1882.9415,-1827.8893,3.6429},
{1696.0597,-1786.4728,3.6449},
{1620.3317,-1742.1069,3.6165},
{1617.1023,-1639.0571,13.2055},
{1657.6254,-1578.1746,13.0421},
{1658.7218,-1466.5491,13.0406},
{1702.8978,-1441.4431,13.0426},
{1769.2415,-1464.8019,13.0297},
{1859.9641,-1500.2599,3.0563},
{2084.6252,-1526.5195,2.3857},
{2446.4373,-1619.9973,14.0469},
{2657.1453,-1623.4719,18.0589},
{2750.2153,-1657.6224,12.5861},
{2868.6631,-1681.9746,10.5340},
{2835.9417,-2036.4082,10.5912},
{2688.7769,-2048.0483,13.3394},
{2302.1350,-2063.0837,13.0326},
{2226.4907,-2135.8640,13.0190},
{2132.0779,-2112.3074,13.0389},
{1960.8812,-2109.4570,13.0414},
{1916.6362,-2165.0344,13.0426},
{1762.3882,-2165.0854,13.0426},
{1565.5057,-2116.2305,32.5347},
{1530.7880,-1968.4578,21.2763},
{1514.1046,-1872.9028,13.0425},
{1353.3082,-1862.2332,13.0423},
{1223.4066,-1852.0336,13.0426},
{1082.9462,-1847.4701,13.0819},
{935.9621,-1774.4515,13.2652},
{652.8855,-1735.5468,13.1714},
{461.7451,-1706.2933,10.5358},
{322.5588,-1710.9512,6.1850},
{227.7143,-1730.3584,3.8353},
{154.8723,-1749.3663,4.7037},
{156.5586,-1960.4346,3.4322}
};

static RaceID; // this will hold the id this race is assigned at init

public race_sels_init() // unique named init function (must be ran on gamemode init)
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

