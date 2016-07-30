//
// Converted:   10.05.08
// Aurthor:    	[T5]SpLendour
// Converter: 	MadCat
//

#if defined _race_canalcontrol_included
  #endinput
#endif

#define _race_canalcontrol_included
#pragma library race_canalcontrol
#include "race"

static racename[MAX_NAME] = "Canal Control";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1];
static Float:startlineheading = 250.0;
static Float:racecheckpoints[][CoordInfo] = {
{1778.057128,-1800.013549,3.550803},
{1953.155639,-1834.914550,7.722711},
{2153.680664,-1852.730957,3.560343},
{2380.793945,-1875.490478,8.848906},
{2577.663085,-1804.835571,1.187000},
{2582.393066,-1646.926635,1.979814},
{2627.036376,-1491.668090,16.030214},
{2740.693359,-1422.910522,15.828024},
{2822.708496,-1474.707519,15.192466},
{2853.788085,-1563.322753,10.487495},
{2733.869628,-1658.112915,12.611770},
{2611.145263,-1628.810058,4.866787},
{2613.574218,-1742.261596,5.807778},
{2575.781005,-1937.187011,3.429639},
{2497.876220,-2049.393310,7.595331},
{2355.825439,-2100.305419,13.120325},
{2302.035644,-2137.680419,13.300503},
{2213.644775,-2305.537597,13.147626},
{2188.094970,-2330.729736,13.114697}
};

static RaceID; // this will hold the id this race is assigned at init

forward race_canalcontrol_init();

public race_canalcontrol_init() // unique named init function (must be ran on gamemode init)
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

