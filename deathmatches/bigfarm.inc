// Created:     03.01.08
// Aurthor:    MadCat
// Based on PEN1 DM

#if defined _dm_bigfarm_included
#endinput
#endif

#define _dm_bigfarm_included
#pragma library dm_bigfarm
#include "deathmatch"

static name[MAX_NAME] = "Big Farm";
static frequency = 5;
static lineupdelay = 240;
static minplayers = 3;
static minlevel = 6;
static maxtime = 400;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_UZI,WEAPON_SILENCED,WEAPON_SAWEDOFF};
static Float:startcp[CoordInfo] = {2174.1602,49.5045,26.4179};
static Float:worldbounds[DMWBInfo] = {31.267517,-168.000457,146.926879,-132.549957};
static Float:spawnpoints[][CoordInfo] = {
{-85.9687,-102.3379,6.4843},
{-133.0231,-93.6653,6.4843},
{-134.3374,-93.3423,3.1180},
{-87.1723,-101.7279,3.1180},
{-60.3396,-50.9021,3.1171},
{-87.3552,-43.6859,3.1171},
{-91.6672,-39.8117,6.4843},
{-95.9001,3.0886,6.1406},
{-63.6371,43.0064,6.4765},
{-61.3079,48.4507,3.1102},
{-52.2062,45.1792,3.1171},
{-42.1615,52.7089,6.4067},
{-32.7658,55.2863,3.1171},
{-15.8193,68.5410,6.1093},
{-0.2330,76.6996,3.1171},
{-10.6606,91.7912,3.1171},
{-39.4165,102.6915,3.1171},
{-60.2613,110.4963,3.1171},
{-78.8289,91.3817,3.1171},
{-94.1715,55.4353,3.1093}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_bigfarm_init();

public dm_bigfarm_init() // unique named init function (must be ran on gamemode init)
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
 	

