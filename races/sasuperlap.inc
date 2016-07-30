//
// Created:     Unknown
// Aurthor:    	<Ob>TheBigShow
// Converter: 	MadCat
//

#if defined _race_sasuperlap_included
  #endinput
#endif

#define _race_sasuperlap_included
#pragma library race_sasuperlap
#include "race"

forward race_sasuperlap_init();

static racename[MAX_NAME] = "San Andreas SuperLap";
static racefrequency = 10;
static lineupdelay = 420;
static minracers = 6;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 9.0383;
static Float:racecheckpoints[][CoordInfo] = {
{1337.3922,-2603.9678,12.9350},
{1319.4271,-2505.7991,12.3792},
{1276.7544,-2462.0837,7.7836},
{1026.3000,-2114.8643,12.4984},
{1008.1651,-1805.3320,13.6126},
{525.4780,-1723.1520,12.0306},
{-45.5214,-1513.6318,1.3916},
{-80.2300,-1072.5876,12.7777},
{-215.6758,-917.4574,39.0156},
{-540.8911,-916.3362,56.6094},
{-778.9705,-1003.7074,78.6664},
{-894.6289,-1111.2031,98.4481},
{-1033.3359,-969.8350,88.1191},
{-1235.5913,-774.8771,63.8581},
{-1584.7209,-803.8812,49.3579},
{-1761.5322,-644.9853,18.5685},
{-1796.6022,-579.3846,15.7162},
{-1884.5443,-563.3736,24.0080},
{-1897.2456,-183.3216,37.8132},
{-1889.7617,89.8022,37.7474},
{-1815.6743,382.5504,38.4430},
{-1576.8052,640.2637,42.8775},
{-1294.9646,935.6993,45.6861},
{-1106.2336,1125.2803,36.9338},
{-957.3311,1102.0011,27.5839},
{-907.1514,1015.7446,20.2331},
{-780.5422,999.5911,17.5319},
{-724.8663,884.0908,12.7803},
{-734.4766,700.9045,17.3234},
{-516.2930,602.6057,16.5636},
{-136.9297,577.5159,15.5402},
{235.7465,770.6647,5.7608},
{248.6335,919.8965,24.6934},
{186.4514,1118.3917,14.8678},
{231.4632,1198.2474,15.0114},
{368.4298,1478.6488,7.8739},
{616.4224,1735.0350,5.5010},
{663.5975,2153.8889,20.1615},
{635.5342,2423.8608,29.6123},
{988.9566,2559.6619,10.3147},
{1152.2128,2439.9985,10.2334},
{1549.9310,2461.9182,6.2812},
{2123.2278,2582.5444,6.3335},
{2534.1067,2580.5627,4.4284},
{2720.0422,2281.8689,6.3017},
{2719.2339,1834.8881,6.2878},
{2719.4773,1397.1244,6.2889},
{2719.3545,1095.5803,6.3152},
{2163.2778,832.4836,6.3055},
{1795.7275,809.0789,10.5353},
{1755.7993,579.0029,23.6986},
{1620.5258,234.8327,29.7507},
{1697.6593,278.8692,18.4390},
{1888.3174,285.2278,28.1743},
{2311.2031,319.0734,32.2280},
{2764.5352,131.5428,21.3223},
{2704.5674,-318.5583,27.8964},
{2885.0745,-727.1423,10.4074},
{2879.6128,-1013.5520,10.4460},
{2900.8188,-1303.4000,10.4531},
{2836.9436,-1786.3442,10.6181},
{2818.0303,-2109.1458,10.5669},
{2464.3447,-2164.3459,13.2363},
{2166.8994,-2561.2874,13.1182},
{1842.8575,-2675.3457,5.6142},
{1336.9805,-2595.7952,12.9450}
};

static RaceID; // this will hold the id this race is assigned at init

public race_sasuperlap_init() // unique named init function (must be ran on gamemode init)
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

