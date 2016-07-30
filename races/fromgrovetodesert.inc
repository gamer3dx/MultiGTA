//
// Converted:   09.02.08
// Aurthor:    	Unknown
// Converter: 	MadCat
//

#pragma library race_fromgrovetodesert
#include "race"

static racename[MAX_NAME] = "From Grove to Desert";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 86.7804;
static Float:racecheckpoints[][CoordInfo] = {
{2465.7177,-1660.3724,13.2853},
{2466.3913,-1659.2476,13.2730},
{2230.3969,-1651.8457,14.8853},
{2042.1217,-1585.7912,12.4930},
{1757.0664,-1497.9885,13.2386},
{1634.1660,-1268.0389,44.8240},
{1700.4100,-788.0457,54.4364},
{1681.3482,-328.7845,44.6487},
{1710.1727,413.6195,30.3474},
{1805.6254,856.1033,10.2779},
{1804.9501,1499.5039,6.3497},
{1806.9200,2075.8081,3.5216},
{1696.6276,2550.0615,10.1337},
{1271.8763,2477.6315,7.5900},
{874.5404,2632.8847,10.7686},
{414.2304,2705.3574,60.2227},
{-147.4193,2637.8093,63.3655},
{-990.5606,2712.9819,45.4749},
{-1352.2336,2656.4082,50.9077},
{-1763.8919,2722.5424,58.7738},
{-1739.6956,2686.3793,59.1830},
{-1646.2891,2506.0366,85.5454},
{-1724.1046,2468.7963,75.6796},
{-1692.5458,2408.6210,58.7775},
{-1754.4077,2308.6386,38.0148},
{-1703.0332,2333.1354,31.1738},
{-1820.5017,2135.0878,7.3230},
{-1529.3759,1844.4141,28.7795},
{-1222.3950,1805.5933,41.0207},
{-1113.9796,1738.0986,34.7169},
{-1035.9995,1552.0054,32.6223},
{-1033.0654,1421.4401,38.1640},
{-991.6223,1441.1114,37.8667},
{-933.0621,1412.1275,29.4266},
{-880.5450,1497.7905,23.9794},
{-780.4501,1297.9459,13.2124},
{-492.8377,1064.8624,10.6389},
{-295.4931,812.4038,14.2077}
};

static RaceID;

forward race_fromgrovetodesert_init();

public race_fromgrovetodesert_init()
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

