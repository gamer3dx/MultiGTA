#if defined _dm_bluemountains_included
  #endinput
#endif

#define _dm_bluemountains_included
#pragma library dm_bluemountains
#include "deathmatch"

static name[MAX_NAME] = "Blue Mountains";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 5; 
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_KATANA,WEAPON_SILENCED}; // silenced pistol, katana
static Float:startcp[CoordInfo] = {1653.5696,-1051.1740,23.8984}; 
static Float:worldbounds[DMWBInfo] = {-2764.539794,-2871.988769,-1430.154174,-1617.337646}; 
static Float:spawnpoints[][CoordInfo] = {
{-2793.1848,-1480.0119,138.0380},
{-2807.1145,-1480.7783,138.6280},
{-2827.9670,-1473.9110,136.9313},
{-2842.9968,-1467.0339,135.9218},
{-2861.7719,-1472.0378,135.9218},
{-2862.4077,-1493.3450,136.6696},
{-2859.5844,-1505.4146,137.3870},
{-2843.5341,-1497.3238,137.6306},
{-2870.9453,-1515.2366,137.5647},
{-2853.9213,-1524.9830,138.4589},
{-2819.4045,-1556.7651,140.5944},
{-2801.6870,-1559.2048,140.7643},
{-2794.1279,-1573.4329,141.3203},
{-2774.2292,-1575.9150,141.3502},
{-2780.1018,-1592.2696,141.3901},
{-2794.1179,-1600.8944,141.4071},
{-2810.5280,-1592.4870,141.3319},
{-2822.4594,-1581.0295,141.3203},
{-2840.1577,-1570.1165,141.3203},
{-2853.3884,-1557.1223,140.4786}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_bluemountains_init();

public dm_bluemountains_init() // unique named init function (must be ran on gamemode init)
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
 	

