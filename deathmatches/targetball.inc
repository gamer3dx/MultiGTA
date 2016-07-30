// Created:    04.12.07
// Aurthor:    MadCat
// Based on PEN1 DM (BaseBall)

#if defined _dm_targetball_included
#endinput
#endif

#define _dm_targetball_included
#pragma library dm_targetball
#include "deathmatch"

static name[MAX_NAME] = "TargetBall";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 4;
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_RIFLE};
static Float:startcp[CoordInfo] = {1517.3936,2063.3218,10.8203};
static Float:worldbounds[DMWBInfo] = {1417.637329,1302.848022,2221.235351,2105.183105};
static Float:spawnpoints[][CoordInfo] = {
{1385.1632,2184.9621,11.0234},
{1350.7724,2181.6347,11.0234},
{1355.4664,2157.8945,11.0156},
{1382.2999,2149.5642,11.0234},
{1367.5633,2122.6916,11.0156},
{1332.4495,2123.6621,11.0156},
{1318.2277,2160.7478,11.0234},
{1371.0817,2196.4843,9.7578},
{1392.5690,2169.9697,9.7578},
{1307.5499,2211.6296,18.5314},
{1368.5488,2212.8352,18.5314},
{1402.2598,2192.4653,15.9296},
{1408.3751,2136.1364,17.6796},
{1411.8095,2113.9982,12.0156},
{1404.2510,2182.2041,13.1349},
{1360.6475,2204.7431,13.4361},
{1330.3060,2204.5368,13.4997},
{1298.8336,2213.7812,12.0156},
{1317.0581,2197.8852,11.0234},
{1394.9490,2137.0610,11.0234}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_targetball_init();

public dm_targetball_init() // unique named init function (must be ran on gamemode init)
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
 	

