//
// Created:     Unknown
// Aurthor:    	DracoBlue
// Converter: 	MadCat
//

#if defined _race_badlands_included
  #endinput
#endif

#define _race_badlands_included
#pragma library race_badlands
#include "race"

forward race_badlands_init();

static racename[MAX_NAME] = "Badlands";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 26.6399;
static Float:racecheckpoints[][CoordInfo] = {
	{2878.0393,-563.4216,12.0417},
	{2741.3186,-172.4296,31.3180},
	{2747.4573,-16.9082,33.4069},
	{2643.5718,43.9948,26.0266},
	{2411.6248,40.6211,26.1099},
	{2394.1831,82.0550,26.1096},
	{2239.1394,91.4120,26.1116},
	{2223.8179,55.1054,26.1114},
	{2102.0916,40.7775,26.1086},
	{1822.6056,68.3718,35.1294},
	{1552.8129,116.8515,29.1603},
	{1536.9557,80.6066,28.1877},
	{1560.9054,-38.1332,20.9823},
	{1527.1973,-174.9872,13.7823},
	{1397.8855,-212.7132,5.9491},
	{1258.0315,-146.8423,38.3945},
	{1173.5824,-74.4719,32.0679},
	{901.5374,-90.1843,21.5083},
	{724.4869,-177.8819,20.6593},
	{548.9446,-203.7013,16.8159},
	{388.9404,-314.4740,13.0009},
	{309.3302,-368.2439,9.3072},
	{249.7424,-369.1298,8.4233},
	{196.1395,-320.6905,1.3451},
	{51.2417,-211.6626,1.2312},
	{-272.9534,-156.8957,2.1737},
	{-286.6761,22.8343,0.8513},
	{-207.6824,221.5323,11.5855},
	{-338.9590,269.3241,1.8507},
	{-633.0015,271.0437,1.8513},
	{-775.1334,152.4531,6.5364},
	{-890.2253,-33.0688,33.3589},
	{-675.6279,14.2368,69.3689},
	{-915.3752,-150.3240,53.4996},
	{-980.9408,-402.9523,36.0406},
	{-1026.1073,-445.7716,35.9748},
	{-1038.8149,-588.7558,31.7820}

};

static RaceID; // this will hold the id this race is assigned at init


public race_badlands_init() // unique named init function (must be ran on gamemode init)
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
