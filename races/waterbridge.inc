//
// Converted:   21.10.07
// Aurthor:    	BetaMaster
// Converter: 	MadCat
//

#if defined _race_waterbridge_included
  #endinput
#endif

#define _race_waterbridge_included
#pragma library race_waterbridge
#include "race"

forward race_waterbridge_init();

static racename[MAX_NAME] = "Water Under The Bridge";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_JETMAX,V_SPEEDER,V_SQUALO};
static Float:startlineheading = 82.8452;
static Float:racecheckpoints[][CoordInfo] = {
	{2263.9019,482.2055,-0.4068},
	{1867.4991,510.2875,-0.6732},
	{1377.3889,586.8354,-0.4513},
	{957.0933,594.8831,-0.4821},
	{386.1639,317.6354,-0.5569},
	{96.7395,192.6710,-0.5354},
	{-184.8372,302.7436,-0.3960},
	{-620.5742,310.7862,-0.5682},
	{-956.9727,43.1476,-0.6076},
	{-921.6808,-312.9729,-0.6370},
	{-421.1471,-334.2631,-0.5480},
	{-55.1399,-590.4675,-0.6147},
	{295.6742,-349.9172,-0.6093},
	{763.8837,-181.1484,-0.5527},
	{1258.2032,-184.4532,-0.4742},
	{1707.8536,-17.8053,-0.5373},
	{2104.1912,50.6238,-0.5744},
	{2263.9019,482.2055,-0.4068}
};

static RaceID; // this will hold the id this race is assigned at init

public race_waterbridge_init() // unique named init function (must be ran on gamemode init)
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

