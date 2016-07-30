// Created:    4.09.07
// Aurthor:    MadCat
// Based on PEN1 DM

#if defined _dm_pirateview_included
  #endinput
#endif

#define _dm_pirateview_included
#pragma library dm_pirateview
#include "deathmatch"

static name[MAX_NAME] = "Pirate View";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 14;
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_SHOTGSPA,WEAPON_M4,WEAPON_MP5};
static Float:startcp[CoordInfo] = {2088.0618,1516.1726,10.8203}; 
static Float:worldbounds[DMWBInfo] = {2178.243896,2090.109863,1514.654663,1451.326904};  
static Float:spawnpoints[][CoordInfo] = {
{2137.4006,1455.2048,27.8929},
{2137.9558,1467.3087,31.1718},
{2138.0566,1494.5308,31.1660},
{2138.2197,1511.8736,26.7112},
{2118.0073,1512.7097,27.7401},
{2105.0129,1511.9368,26.6970},
{2100.9492,1503.3431,31.1634},
{2111.0163,1501.9272,31.1659},
{2094.8085,1486.3914,24.1406},
{2093.5439,1479.9764,24.1406},
{2122.8593,1484.8154,24.3319},
{2123.3327,1463.9471,24.6542},
{2151.9140,1456.1571,24.3830},
{2172.8510,1478.9230,24.1406},
{2172.6398,1487.5362,24.1406},
{2173.0905,1494.8740,32.5621},
{2161.3176,1507.0122,31.1646},
{2172.5659,1464.2864,27.9117},
{2109.3796,1483.4403,24.1406},
{2162.4743,1483.1285,24.1406}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_pirateview_init();

public dm_pirateview_init() // unique named init function (must be ran on gamemode init)
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
 	

