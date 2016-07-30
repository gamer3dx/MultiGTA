// Created:     09.12.07
// Aurthor:    MadCat
// Based on PEN1 DM

#if defined _dm_trainhouse_included
  #endinput
#endif

#define _dm_trainhouse_included
#pragma library dm_trainhouse
#include "deathmatch"

static name[MAX_NAME] = "Train House";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 5;
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_TEC9,WEAPON_DEAGLE};
static Float:startcp[CoordInfo] = {2271.3777,-2248.3506,13.6922}; 
static Float:worldbounds[DMWBInfo] = {2275.377441,2208.169433,-2241.233886,-2308.467041}; 
static Float:spawnpoints[][CoordInfo] = {
{2250.4814,-2247.7460,14.7646},
{2244.9904,-2255.5761,14.7646},
{2239.6884,-2260.0654,14.7646},
{2232.0678,-2266.9426,14.7646},
{2232.0937,-2273.5715,14.7646},
{2226.3872,-2277.8525,14.7646},
{2219.9338,-2283.1064,14.7646},
{2214.3127,-2286.6809,14.7646},
{2227.3652,-2288.8601,13.5468},
{2233.1918,-2300.9316,14.7646},
{2243.7399,-2292.3212,14.7646},
{2237.9831,-2293.1369,14.7646},
{2230.2687,-2286.1542,14.3751},
{2238.1127,-2283.1376,13.5545},
{2246.3830,-2284.8955,14.7646},
{2251.7368,-2279.6181,14.7646},
{2261.4252,-2274.3825,14.7646},
{2266.5688,-2266.0278,14.7646},
{2257.3869,-2264.9348,13.5468},
{2249.0915,-2272.1533,13.5545}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_trainhouse_init();

public dm_trainhouse_init() // unique named init function (must be ran on gamemode init)
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
 	

