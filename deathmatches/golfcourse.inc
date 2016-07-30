// Created:     02.09.07
// Aurthor:    MadCat
// Based on PEN1 DM

#if defined _dm_golfcourse_included
  #endinput
#endif

#define _dm_golfcourse_included
#pragma library dm_golfcourse
#include "deathmatch"

static name[MAX_NAME] = "Golf Course";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 1; 
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_GOLFCLUB,WEAPON_COLT45,WEAPON_SHOTGUN};
static Float:startcp[CoordInfo] = {1459.5352,2773.5393,10.8203}; 
static Float:worldbounds[DMWBInfo] = {1416.058715,1120.567993,2857.985351,2726.704833};
static Float:spawnpoints[][CoordInfo] = {
{1391.6339,2759.9704,10.8203},
{1392.2662,2811.6540,10.8203},
{1364.8751,2839.1953,10.8203},
{1333.5506,2823.3559,10.8203},
{1335.0386,2778.4694,10.7719},
{1333.8549,2734.4785,10.9024},
{1284.7797,2754.7651,10.6213},
{1281.8505,2793.3073,10.8203},
{1306.7147,2846.1652,10.1443},
{1268.4077,2847.8320,10.8203},
{1261.1706,2816.7927,10.8125},
{1275.6538,2782.9189,10.8203},
{1280.8858,2760.1069,10.8203},
{1240.7932,2733.4338,10.8203},
{1185.6799,2809.5300,10.8203},
{1157.2840,2836.2663,10.7963},
{1130.5638,2810.2089,10.1405},
{1137.4342,2775.7878,10.7928},
{1168.8975,2769.6940,11.3416},
{1173.7413,2745.1337,10.8281}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_golfcourse_init();

public dm_golfcourse_init() // unique named init function (must be ran on gamemode init)
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
 	

