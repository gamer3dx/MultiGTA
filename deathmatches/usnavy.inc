// Created:     09.09.06
// Aurthor:    Iain Gilbert
// Spawns from PEN1: DM (Carrier)

#if defined _dm_usnavy_included
  #endinput
#endif

#define _dm_usnavy_included
#pragma library dm_usnavy
#include "deathmatch"

static name[MAX_NAME] = "The US Navy";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 14; 
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_SHOTGSPA,WEAPON_MP5,WEAPON_M4}; // combat shotgun, mp5, m4
static Float:startcp[CoordInfo] = {2786.84, -2456.29, 13.63};
static Float:worldbounds[DMWBInfo] = {-1246.768554,-1463.660766,513.935119,486.338897};
static Float:spawnpoints[][CoordInfo] = {
{-1373.08,500.86,11.20},
{-1291.24,490.64,11.20},
{-1288.25,513.94,11.20},
{-1394.59,488.72,11.20},
{-1352.81,517.50,11.20},
{-1446.91,501.29,3.04},
{-1376.54,513.85,3.04},
{-1428.41,506.40,3.03},
{-1405.13,512.51,3.03},
{-1391.05,493.08,3.03},
{-1379.26,509.39,3.03},
{-1383.26,498.98,3.03},
{-1355.66,499.26,11.19},
{-1395.70,496.46,11.20},
{-1356.50,503.27,11.19},
{-1330.50,492.67,11.19},
{-1294.04,506.88,11.19},
{-1428.41,506.40,3.03},
{-1354.38,492.38,11.19},
{-1347.96,495.45,18.23},
{-1329.11,492.85,21.25},
{-1343.04,489.92,21.25},
{-1384.18,493.16,21.25},
{-1428.41,506.40,3.03},
{-1318.85,492.63,18.23},
{-1358.90,500.47,18.23},
{-1353.82,493.64,11.19}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_usnavy_init();

public dm_usnavy_init() // unique named init function (must be ran on gamemode init)
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
 	

