// Created:    		Unknown
// Aurthor:    		PaniC
// Converted by: 	MadCat

#if defined _dm_baseballfield_included
#endinput
#endif

#define _dm_baseballfield_included
#pragma library dm_baseballfield
#include "deathmatch"

static name[MAX_NAME] = "Baseball Field";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 4; 
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_SNIPER,WEAPON_AK47};
static Float:startcp[CoordInfo] = {-2260.8816,168.9745,35.1797};
static Float:worldbounds[DMWBInfo] = {-2281.0598,-2411.3059,228.1460,60.1657};
static Float:spawnpoints[][CoordInfo] = {
{-2306.8762,207.0832,35.3516},
{-2355.1809,213.8019,35.3125},
{-2305.7241,92.1098,35.3125},
{-2354.2053,84.3492,35.3125},
{-2354.0425,142.3580,35.3125},
{-2321.4573,223.6907,36.7738},
{-2288.8372,192.5210,36.5878},
{-2291.3691,107.8632,36.6961},
{-2321.7761,74.7119,36.6618},
{-2346.7334,174.7288,35.3125},
{-2377.0225,112.0270,35.3047},
{-2382.5823,68.3764,35.3203},
{-2391.1506,203.5921,35.3203}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_baseballfield_init();

public dm_baseballfield_init() // unique named init function (must be ran on gamemode init)
{
	dmid = RegisterDM(name);
	if (dmid == INVALID_DM_ID) return;

	Deathmatch[dmid][dm_frequency] = frequency;
 	Deathmatch[dmid][dm_lineupdelay] = lineupdelay;
 	Deathmatch[dmid][dm_minplayers] = minplayers;
 	Deathmatch[dmid][dm_minlevel] = minlevel;
 	Deathmatch[dmid][dm_cashprize] = CalculateCashPrize(minlevel);
	Deathmatch[dmid][dm_cashentry] = CalculateCashEntry(minlevel);
	Deathmatch[dmid][dm_xpprize] = CalculateXPPrize(minlevel);
	Deathmatch[dmid][dm_xpbonus] = CalculateXPBonus(minlevel);
	Deathmatch[dmid][dm_xpkill] = CalculateXPForKill(minlevel);
	Deathmatch[dmid][dm_xpdeath] = CalculateXPForDeath(minlevel);
	Deathmatch[dmid][dm_maxtime] = maxtime;
	Deathmatch[dmid][dm_activedmjoin] = activedmjoin;
	Deathmatch[dmid][dm_interior] = interior;
	Deathmatch[dmid][dm_suddenexplosiontime] = suddenexplosiontime;
	set(Deathmatch[dmid][dm_weapons],weapons);
	DeathmatchStats[dmid][dm_timer] = MakeDMSleepTime(dmid);
	DeathmatchStats[dmid][dm_state] = DM_STATE_SLEEPING;
	
	DMStartCP[dmid][Coord_X] = startcp[Coord_X];
	DMStartCP[dmid][Coord_Y] = startcp[Coord_Y];
	DMStartCP[dmid][Coord_Z] = startcp[Coord_Z];
	
	DMWorldBounds[dmid][x_max] = worldbounds[x_max];
	DMWorldBounds[dmid][x_min] = worldbounds[x_min];
	DMWorldBounds[dmid][y_max] = worldbounds[y_max];
	DMWorldBounds[dmid][y_min] = worldbounds[y_min];

	DMSpawnsSize[dmid] = sizeof(spawnpoints);
	for (new spid=0;spid<sizeof(spawnpoints);spid++)
	{
		DMSpawnPoints[dmid][spid][Coord_X] = spawnpoints[spid][Coord_X];
		DMSpawnPoints[dmid][spid][Coord_Y] = spawnpoints[spid][Coord_Y];
		DMSpawnPoints[dmid][spid][Coord_Z] = spawnpoints[spid][Coord_Z];
	}
	
	DMObjectsSize[dmid] = sizeof(objects);
	for (new spid=0;spid<sizeof(objects);spid++)
	{
		DMObjectInfo[dmid][spid][dmomodel] = objects[spid][dmomodel];
		DMObjectInfo[dmid][spid][dmox] = objects[spid][dmox];
		DMObjectInfo[dmid][spid][dmoy] = objects[spid][dmoy];
		DMObjectInfo[dmid][spid][dmoz] = objects[spid][dmoz];
		DMObjectInfo[dmid][spid][dmorx] = objects[spid][dmorx];
		DMObjectInfo[dmid][spid][dmory] = objects[spid][dmory];
		DMObjectInfo[dmid][spid][dmorz] = objects[spid][dmorz];
	}

 }
 	

