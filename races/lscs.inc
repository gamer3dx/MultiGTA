//
// Converted:   25.04.08
// Aurthor:    	Sharon
// Converter: 	MadCat
//

#if defined _race_lscs_included
  #endinput
#endif

#define _race_lscs_included
#pragma library race_lscs
#include "race"

forward race_lscs_init();

static racename[MAX_NAME] = "LS Countryside Race";
static racefrequency = 10;
static lineupdelay = 240;
static minracers = 6;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 11.3526;
static Float:racecheckpoints[][CoordInfo] = {
{123.9827,-1705.7828,8.7573}, // 0
{118.0850,-1539.1171,7.2945},
{107.7675,-1490.9429,12.7952},
{153.5472,-1390.1949,48.4617},
{150.8260,-1218.3141,45.6226},
{256.6262,-1008.6379,54.8604},
{333.6590,-847.1541,11.2226},
{385.1934,-694.7964,23.0845},
{471.5993,-605.2053,36.3154},
{643.7766,-639.7946,16.9019},
{643.0110,-575.1237,15.9542},
{602.8603,-514.7201,15.9542},
{645.4496,-460.7169,15.9542},
{530.3125,-412.8660,28.0053},
{484.9729,-358.1815,31.2450},
{514.9326,-227.2164,37.9427},
{479.1991,-138.0775,30.1258},
{331.5779,-119.8549,1.1884},
{269.1812,-73.1742,1.1964},
{232.5227,-19.4055,1.1964},
{196.9384,54.9952,1.8486},
{26.1557,144.0238,1.8448},
{-38.6532,141.4871,2.8839},
{-33.6293,84.5922,2.8819},
{-60.7298,49.5491,2.8768},
{-59.7573,12.3395,2.8819},
{-107.6766,-88.7686,2.8799},
{-98.5311,-157.7970,2.8022},
{-26.4692,-206.2262,1.3633},
{122.2924,-214.3126,1.1965},
{297.1379,-212.2569,1.1954},
{359.3885,-144.7836,1.6744},
{551.2436,-145.9276,36.3798},
{770.4543,-138.2328,20.4850},
{888.2232,-89.3832,23.8839},
{1037.7819,-63.9672,22.3138},
{1251.4385,-144.7939,38.6504},
{1361.6262,-209.9842,7.7676},
{1488.9541,-204.1780,11.2145},
{1555.1927,-112.8484,19.1957},
{1559.0391,-5.3056,22.2351},
{1537.6740,82.9344,28.3114},
{1488.4067,161.1694,29.8827},
{1217.1759,280.8254,19.1729},
{1244.7957,343.3670,19.1728},
{1389.7181,280.8317,19.1730},
{1401.5636,214.7015,19.1841},
{1488.5275,220.5214,18.8460},
{1586.0387,261.1160,15.7092},
{1672.7751,248.4079,14.7497},
{1779.6217,196.2190,27.0176},
{1829.6550,190.0999,30.3611},
{2052.6177,104.8985,27.6640},
{2104.1047,98.6589,32.3014},
{2195.0205,85.2291,23.9680},
{2262.5139,91.6915,26.1025},
{2345.4275,124.2239,26.1026},
{2336.3669,376.5435,26.4556},
{2266.8250,374.0669,31.2140},
{2361.6096,313.6637,32.4294},
{2561.8711,294.3457,29.9200},
{2742.8992,124.5916,21.5045},
{2673.4382,48.4218,25.2538},
{2654.9438,23.1857,27.3227},
{2708.3245,-72.8857,42.8521},
{2684.6724,-189.9101,34.2834},
{2661.7893,-313.3696,43.9588},
{2620.1616,-366.3285,60.8801},
{2539.8425,-400.4968,78.1108},
{2497.8062,-527.2678,94.6736},
{2399.7500,-622.7779,124.1799},
{2500.2842,-749.0581,98.8072},
{2624.1877,-763.8385,87.9707},
{2754.9229,-577.2137,51.8212},
{2659.8008,-410.4524,55.0176},
{2509.9983,-387.3646,77.3894},
{2346.2847,-390.4238,67.6964},
{2222.6738,-377.0235,49.9799},
{2080.9229,-383.7252,69.4107},
{2014.2982,-481.0828,76.7734},
{2077.2949,-590.3631,72.9180},
{2160.6101,-664.7104,52.1795},
{2240.7332,-741.8379,38.9384},
{2285.6714,-869.9815,27.7659},
{2287.7439,-990.3686,26.5009},
{2286.8459,-1117.4656,26.6598}
};

static RaceID; // this will hold the id this race is assigned at init

public race_lscs_init() // unique named init function (must be ran on gamemode init)
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

