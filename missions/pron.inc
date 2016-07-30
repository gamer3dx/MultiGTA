//Pron Mission
//Created by Dracoblue
//Converted for MultiGTA by MadCat

#if defined _mission_pron_included
  #endinput
#endif

#define _mission_pron_included
#pragma library mission_pron
#include "mission"

forward mission_pron_init();

static missionname[MAX_NAME] = "MR Pron";
static Float:missioncheckpoints[][CoordInfo] = {
	{2800.4209,-1063.6849,30.7198},
	{733.1689,-1359.9813,25.6922},
	{2800.4209,-1063.6849,30.7198}
};

static missiontext[][MAX_STRING] = {
	{"NotUsedTextForStartpoint"},
	{"There is some films hidden at the film studio... It's porn! Get it!"}, // Show when player start mission
	{"Deliver them to the hotel!"}
};

//Vehicle needed. 0 - no, 1 - any, between 399 and 612 - need selected vehicle, player need to find it
static neededvehicle[] = {
	0, // you need this vehicle to start mission
	0,
	0
};

//Mission time. 999 - disable timer, 0 - not set new time and use current, other - selected time (in seconds)
static time[] = {
	999, // this time will be set when you start mission
	0,
	0
};

static moneyreward = 50;
static xpreward = 0;

static maxoutofcartime = 0;

static MissionID; // this will hold the id this mission is assigned at init

public mission_pron_init() // unique named init function (must be ran on gamemode init)
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

