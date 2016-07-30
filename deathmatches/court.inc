// Created:     07.11.07
// Aurthor:    MadCat
// Based on PEN1 DM

#if defined _dm_court_included
  #endinput
#endif

#define _dm_court_included
#pragma library dm_court
#include "deathmatch"

static name[MAX_NAME] = "Tennis Court";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 3; 
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_GRENADE};
static Float:startcp[CoordInfo] = {1812.6981,2722.7920,10.8203}; 
static Float:worldbounds[DMWBInfo] = {1891.059692,1828.865722,2879.936035,2816.779785};
static Float:spawnpoints[][CoordInfo] = {
{1851.2285,2822.8227,10.8359},
{1852.7432,2842.6604,10.8359},
{1852.0660,2856.8874,10.8359},
{1852.0461,2875.3166,10.8359},
{1837.3153,2841.7124,10.8359},
{1837.1375,2822.2429,10.8359},
{1836.5969,2856.3059,10.8359},
{1836.6049,2873.7709,10.8359},
{1867.9331,2855.5839,10.8359},
{1867.6358,2874.9904,10.8359},
{1867.4312,2842.8276,10.8359},
{1867.6263,2820.9208,10.8359},
{1881.8323,2854.5500,10.8359},
{1883.0972,2875.8513,10.8359},
{1883.0516,2842.9174,10.8359},
{1882.2581,2822.0554,10.8359},
{1875.7822,2848.2319,10.8359},
{1859.3444,2849.0363,10.8359},
{1846.2702,2849.2026,10.8359},
{1836.2216,2849.2482,10.8359}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_court_init();

public dm_court_init() // unique named init function (must be ran on gamemode init)
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
 	

