// Created:    		Unknown
// Aurthor:    		PaniC
// Converted by: 	MadCat

#if defined _dm_fostervalley_included
  #endinput
#endif

#define _dm_fostervalley_included
#pragma library dm_fostervalley
#include "deathmatch"

static name[MAX_NAME] = "Foster Valley";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 4; 
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_UZI,WEAPON_DEAGLE};
static Float:startcp[CoordInfo] = {-2019.6835,-859.8499,32.1719};
static Float:worldbounds[DMWBInfo] = {-1965.0648,-725.7273,-1943.5498,-993.4793};
static Float:spawnpoints[][CoordInfo] = {
{-1960.8845,-911.9338,35.8909},
{-1953.9567,-894.3176,35.8909},
{-1962.4727,-873.3451,35.8906},
{-1949.0028,-872.2573,35.8828},
{-1947.7842,-911.8601,35.8909},
{-1949.2751,-805.4637,35.8909},
{-1962.0758,-841.8178,35.8906},
{-1948.4899,-806.1996,35.8909},
{-1948.1736,-843.8295,35.8828},
{-1955.0557,-828.1476,35.8909}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_fostervalley_init();

public dm_fostervalley_init() // unique named init function (must be ran on gamemode init)
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
 	
