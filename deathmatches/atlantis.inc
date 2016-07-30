// Created:    04.12.07
// Aurthor:    MadCat
// Based on PEN1 DM

#if defined _dm_atlantis_included
  #endinput
#endif

#define _dm_atlantis_included
#pragma library dm_atlantis
#include "deathmatch"

static name[MAX_NAME] = "Atlantis";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 12; 
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_SNIPER,WEAPON_MP5};
static Float:startcp[CoordInfo] = {1628.3453,586.7740,1.7578}; 
static Float:worldbounds[DMWBInfo] = {1585.835449,1384.099975,609.306701,445.063201};
static Float:spawnpoints[][CoordInfo] = {
{1499.9460,583.1378,-41.4245},
{1493.2855,572.4582,-40.1730},
{1485.8356,559.3757,-39.8906},
{1477.8157,544.7623,-39.3373},
{1470.8160,530.2221,-36.6528},
{1464.3558,516.4238,-33.3626},
{1433.5339,511.4612,-33.9862},
{1419.0744,532.7868,-38.9865},
{1405.6253,558.5703,-39.5980},
{1394.9287,578.6989,-38.6066},
{1380.2196,587.0228,-36.5684},
{1372.0994,573.9802,-35.6089},
{1363.6091,553.6951,-34.2159},
{1352.2979,525.4387,-31.2782},
{1419.3227,556.2225,-39.6003},
{1435.0451,550.7457,-39.5983},
{1446.6480,555.9124,-39.5986},
{1515.4549,576.4270,-42.6635},
{1524.0355,554.1083,-44.7462},
{1530.1280,511.3201,-43.6541}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_atlantis_init();

public dm_atlantis_init() // unique named init function (must be ran on gamemode init)
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
 	

