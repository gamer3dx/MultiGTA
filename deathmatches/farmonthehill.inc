// Created:    		Unknown
// Aurthor:    		PaniC
// Converted by: 	MadCat

#if defined _dm_farmonthehill_included
#endinput
#endif

#define _dm_farmonthehill_included
#pragma library dm_farmonthehill
#include "deathmatch"

static name[MAX_NAME] = "Farm On The Hill";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 20; 
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_M4,WEAPON_MP5,WEAPON_DEAGLE,WEAPON_SNIPER};
static Float:startcp[CoordInfo] = {-1549.0555,-1525.3533,38.0859};
static Float:worldbounds[DMWBInfo] = {-1398.3859,-1474.9664,-1437.4980,-1602.8281};
static Float:spawnpoints[][CoordInfo] = {
{-1415.5341,-1449.6016,101.7407},
{-1447.5891,-1448.4867,101.7578},
{-1451.2421,-1467.6295,101.7578},
{-1464.7236,-1487.7332,101.7578},
{-1468.4546,-1507.4238,101.7513},
{-1469.2178,-1529.9785,101.7578},
{-1465.5211,-1596.9784,101.7578},
{-1434.3140,-1571.8358,101.7578},
{-1438.9668,-1555.6205,101.7578},
{-1417.3325,-1552.4628,101.7578},
{-1417.7980,-1535.2312,101.7520},
{-1428.8199,-1503.2278,101.6719}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_farmonthehill_init();

public dm_farmonthehill_init() // unique named init function (must be ran on gamemode init)
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
 	

