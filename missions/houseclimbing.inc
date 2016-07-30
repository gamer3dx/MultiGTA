//House Climbing mission
//Created by Dracoblue
//Converted for MultiGTA by MadCat

#if defined _mission_houseclimbing_included
  #endinput
#endif

#define _mission_houseclimbing_included
#pragma library mission_houseclimbing
#include "mission"

forward mission_houseclimbing_init();

static missionname[MAX_NAME] = "House Climbing";
static Float:missioncheckpoints[][CoordInfo] = {
	{2473.8181,-1978.1577,13.4297},
	{2468.4492,-1973.2582,19.3476},
	{2479.7344,-1965.2040,21.8286},
	{2451.1973,-1957.5476,18.5500},
	{2440.3545,-1985.4976,18.5500},
	{2461.3169,-1981.5908,15.8370},
	{2466.1838,-1992.8887,18.3795},
	{2483.4734,-1989.6407,18.4428},
	{2501.2153,-1984.8195,17.3393},
	{2524.4531,-1997.2550,18.5558},
	{2530.0005,-2006.4343,13.5469}
};

static missiontext[][MAX_STRING] = {
	{"NotUsedTextForStartpoint"},
	{"You say: Hmm... Let's try it!"}, // Show when player start mission
	{"You say: Can I got on this?"},
	{"You say: Nice view!"},
	{"You say: Ouch ..."},
	{"You say: JUMMP"},
	{"You say: Nah, this is easy!"},
	{"You say: Did it!"},
	{"You say: Next one!"},
	{"You say: Third one!"},
	{"You say: Last one!"}
};

//Vehicle needed. 0 - no, 1 - any, between 399 and 612 - need selected vehicle, player need to find it
static neededvehicle[] = {
	0, // you need this vehicle to start mission
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0
};

//Mission time. 999 - disable timer, 0 - not set new time and use current, other - selected time (in seconds)
static time[] = {
	60, // this time will be set when you start mission
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0
};

static moneyreward = 500;
static xpreward = 0;

static maxoutofcartime = 30;

static MissionID; // this will hold the id this mission is assigned at init

public mission_houseclimbing_init() // unique named init function (must be ran on gamemode init)
{
	MissionID = Mission_Register(missionname);
	if (MissionID == INVALID_MISSION_ID) return;

	Mission[MissionID][mission_reward_money] = moneyreward;
	Mission[MissionID][mission_reward_xp] = xpreward;
	MissionMaxOutOfCarTime[MissionID] = maxoutofcartime;

	MissionSize[MissionID] = sizeof(missioncheckpoints);
	for (new cpid=0;cpid<sizeof(missioncheckpoints);cpid++)
	{
		MissionCheckpoints[MissionID][cpid][Coord_X] = missioncheckpoints[cpid][Coord_X];
		MissionCheckpoints[MissionID][cpid][Coord_Y] = missioncheckpoints[cpid][Coord_Y];
		MissionCheckpoints[MissionID][cpid][Coord_Z] = missioncheckpoints[cpid][Coord_Z];
		set(MissionText[MissionID][cpid],missiontext[cpid]);
		MissionNeedVehicle[MissionID][cpid] = neededvehicle[cpid];
		MissionTime[MissionID][cpid] = time[cpid];
		
	}
	
	AddActiveCheckpoint(GetMissionCPX(MissionID,0),GetMissionCPY(MissionID,0),GetMissionCPZ(MissionID,0),40,2);
	new logstring[MAX_STRING];
	format(logstring,MAX_STRING,"Mission '%s' (id: %d) loaded!",missionname,MissionID);
	WriteLog(logstring);
}

