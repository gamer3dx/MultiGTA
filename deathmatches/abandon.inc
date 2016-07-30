// Created:     03.01.08
// Aurthor:    MadCat
// Based on PEN1 DM

#if defined _dm_abandon_included
  #endinput
#endif

#define _dm_abandon_included
#pragma library dm_abandon
#include "deathmatch"

static name[MAX_NAME] = "Abandon";
static frequency = 5;
static lineupdelay = 240;
static minplayers = 3;
static minlevel = 4;
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_SHOTGUN,WEAPON_DEAGLE,WEAPON_UZI};
static Float:startcp[CoordInfo] = {403.0012,2540.1353,16.5455};
static Float:worldbounds[DMWBInfo] = {375.972534,77.408653,2477.095214,2372.656494};
static Float:spawnpoints[][CoordInfo] = {
{357.7252,2441.6767,16.4989},
{336.1304,2436.5124,16.6853},
{317.5841,2426.1472,16.7082},
{307.6272,2407.8088,16.5279},
{293.9517,2438.1022,16.4765},
{284.5486,2451.7810,16.4765},
{263.0063,2425.0908,16.7845},
{248.8664,2439.5046,16.6326},
{235.9276,2453.5495,16.6697},
{227.8102,2441.2724,16.7317},
{237.6287,2432.1630,17.0399},
{215.0467,2414.0468,16.4765},
{207.9912,2424.9277,16.8768},
{188.4344,2448.1381,17.0116},
{164.0355,2422.1796,16.6435},
{156.7850,2425.3662,16.7346},
{133.6560,2437.5903,16.4765},
{118.4888,2404.3940,16.4765},
{109.4164,2420.1242,16.8460},
{94.8912,2417.5468,16.9591}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_abandon_init();

public dm_abandon_init() // unique named init function (must be ran on gamemode init)
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
 	

