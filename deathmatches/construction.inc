// Created:     03.01.08
// Aurthor:    MadCat
// Based on PEN1 DM

#if defined _dm_construction_included
#endinput
#endif

#define _dm_construction_included
#pragma library dm_construction
#include "deathmatch"

static name[MAX_NAME] = "Construction";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 7; 
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_RIFLE,WEAPON_MP5};
static Float:startcp[CoordInfo] = {2223.0593,1838.9752,10.8203};
static Float:worldbounds[DMWBInfo] = {2500.844726,2370.079833,1966.328613,1782.882202};
static Float:spawnpoints[][CoordInfo] = {
{2382.4218,1952.5798,11.6498},
{2379.1965,1888.5764,11.6562},
{2375.1362,1873.3023,11.6562},
{2402.5129,1829.2460,11.6562},
{2407.7539,1853.5402,6.0156},
{2379.9084,1917.8975,6.0156},
{2420.8828,1955.0521,6.0156},
{2448.9028,1961.2872,10.7819},
{2483.7099,1952.5963,10.4051},
{2477.9812,1919.6159,13.4829},
{2479.5566,1918.5473,10.4697},
{2475.6303,1918.9008,10.8812},
{2492.4956,1873.9971,10.8203},
{2487.2255,1852.8271,10.820},
{2420.5664,1906.3629,6.0156},
{2445.2629,1931.9699,7.0514},
{2457.1042,1919.9311,10.8647},
{2462.5893,1880.6854,8.1640},
{2450.8505,1876.0681,8.1640},
{2473.6347,1873.9982,10.5453}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_construction_init();

public dm_construction_init() // unique named init function (must be ran on gamemode init)
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
 	

