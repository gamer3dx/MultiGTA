//
// Converted:   02.08.08
// Aurthor:     Rockstar North Team
// Converter: 	MadCat
//

#if defined _race_sp_badlandsa_included
  #endinput
#endif

#define _race_sp_badlandsa_included
#pragma library race_sp_badlandsa
#include "race"

static racename[MAX_NAME] = "(SP)Badlands A";
static racefrequency = 5;
static lineupdelay = 240;
static minracers = 3;
static allowedcars[MAX_RACE_VEHICLES+1] = {V_SABRE,V_PREVION,V_STALLION};
static Float:startlineheading = 180.0;
static Float:racecheckpoints[][CoordInfo] = {
{1559.983,17.468,26.1563},
{1556.426,-94.0345,19.4957},
{1434.738,-214.9645,7.3527},
{1321.254,-195.8984,15.7447},
{1211.007,-102.6927,39.3603},
{892.2923,-88.1123,22.4405},
{789.9247,-124.3171,21.3365},
{659.6545,-196.7497,11.7663},
{419.4744,-302.5507,6.446},
{289.8085,-380.3828,7.8753},
{207.4451,-298.923,2.4375},
{182.497,-217.8004,2.4216},
{22.0727,-207.7681,2.5827},
{-114.231,-132.3417,2.1236},
{-38.4818,140.8613,2.1234},
{-141.4718,187.6611,6.2406},
{-329.7878,173.921,5.403},
{-507.6872,221.0444,8.7633},
{-661.1096,226.3879,16.7064},
{-618.4406,-67.8995,62.3489},
{-507.8417,-45.9077,59.2556},
{-520.6511,79.6011,32.2117},
{-715.4454,225.1288,12.2899},
{-770.0811,130.1324,9.4651},
{-738.1151,25.555,32.2336},
{-885.785,-41.3927,33.2347},
{-716.4521,5.1206,59.1055},
{-759.2693,-92.0455,64.8686},
{-826.881,-167.7603,64.99},
{-728.9467,-173.5218,65.1248},
{-544.8638,-189.4458,77.8383}

};

static RaceID; // this will hold the id this race is assigned at init

forward race_sp_badlandsa_init();

public race_sp_badlandsa_init() // unique named init function (must be ran on gamemode init)
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

