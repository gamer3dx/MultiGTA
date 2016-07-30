//
// Created:     Unknown
// Aurthor:    	Switch
// Converter: 	MadCat
//

#if defined _race_outburst_included
  #endinput
#endif

#define _race_outburst_included
#pragma library race_outburst
#include "race"

forward race_outburst_init();

static racename[MAX_NAME] = "OutBurst";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 268.1271;
static Float:racecheckpoints[][CoordInfo] = {
{1285.1924,-1400.2336,12.3991},
{1371.4340,-1401.0148,12.6705},
{1391.8837,-1415.4141,12.7822},
{1401.2896,-1447.5729,12.8384},
{1388.2570,-1501.5649,7.9556},
{1359.4832,-1639.0118,7.8931},
{1369.4548,-1696.3931,8.8893},
{1365.7859,-1673.4946,12.8343},
{1349.5516,-1687.4941,12.8898},
{1339.8816,-1755.6333,12.7974},
{1335.0265,-1847.4253,12.8306},
{1257.5015,-1836.3048,12.6707},
{1191.9928,-1819.0349,12.8652},
{1166.3740,-1759.3741,12.8541},
{1090.6362,-1726.0326,12.9480},
{1045.9634,-1683.2914,12.8306},
{1047.0342,-1626.8513,12.8425},
{1142.3206,-1606.8188,13.2371},
{1136.5602,-1577.7400,12.7082},
{1128.7588,-1548.9973,13.6408},
{1141.9413,-1385.3491,13.0811},
{1151.1693,-1267.6648,12.9919},
{1168.3922,-1206.4060,18.9721},
{1210.3456,-1221.1150,17.1667},
{1200.8917,-1340.8575,12.6829}
};

static RaceID; // this will hold the id this race is assigned at init

public race_outburst_init() // unique named init function (must be ran on gamemode init)
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

	Race[RaceID][race_cashprize] = CalculateRewardMoney(RaceID); 
	Race[RaceID][race_cashentry] = CalculateEntryFee(RaceID); 
	Race[RaceID][race_xpprize] = CalculateRewardXP(RaceID); 
	Race[RaceID][race_maxracetime] = CalculateMaxRaceTime(RaceID);
    	RaceStats[RaceID][race_state] = RACE_STATE_SLEEPING;
    RaceStats[RaceID][race_state] = RACE_STATE_SLEEPING;
}

