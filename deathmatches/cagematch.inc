// Created:     01.12.07
// Aurthor:    	MadCat
// Based on PEN1 DM

#if defined _dm_cagematch_included
  #endinput
#endif

#define _dm_cagematch_included
#pragma library dm_cagematch
#include "deathmatch"

static name[MAX_NAME] = "CageMatch";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 5; 
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_SHOTGUN,WEAPON_MP5,WEAPON_RIFLE};
static Float:startcp[CoordInfo] = {-2644.760498,1367.972290,7.162925}; 
static Float:worldbounds[DMWBInfo] = {-2642.598388,-2708.446044,1461.384033,1352.234741};
static Float:spawnpoints[][CoordInfo] = {
{-2666.3532,1431.3953,28.4615},
{-2665.9475,1421.4931,33.3903},
{-2665.4477,1403.2604,34.1505},
{-2665.5998,1392.4943,31.4916},
{-2665.5998,1379.6259,22.6039},
{-2679.1340,1376.9769,19.8698},
{-2677.3781,1388.1369,29.0098},
{-2677.3522,1402.0352,34.0004},
{-2677.6196,1416.2264,34.0356},
{-2677.1125,1434.5026,26.0574},
{-2686.8090,1442.0784,19.3791},
{-2687.8535,1426.2098,31.4316},
{-2688.0385,1408.8193,34.8315},
{-2688.0878,1393.0676,31.8180},
{-2688.0859,1380.1285,22.9927},
{-2698.0632,1375.1092,17.7906},
{-2698.1423,1385.4146,27.0826},
{-2697.1894,1395.4895,33.1974},
{-2697.1135,1410.0773,34.7889},
{-2696.9667,1431.4114,28.4490}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_cagematch_init();

public dm_cagematch_init() // unique named init function (must be ran on gamemode init)
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
 	

