// Created:    04.12.07
// Aurthor:    MadCat
// Based on PEN1 DM (BatesMotel)

#if defined _dm_silentmotel_included
  #endinput
#endif

#define _dm_silentmotel_included
#pragma library dm_silentmotel
#include "deathmatch"

static name[MAX_NAME] = "Silent Motel";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 1;
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_SILENCED};
static Float:startcp[CoordInfo] = {2660.00,1953.33,10.82}; 
static Float:worldbounds[DMWBInfo] = {2657.986328,2591.367187,2080.977783,1962.998535};
static Float:spawnpoints[][CoordInfo] = {
{2607.3103,1971.4505,10.6718},
{2626.5913,1966.7659,11.0220},
{2667.7346,1970.4898,10.8222},
{2645.1538,1980.6469,10.8203},
{2629.8906,1984.5904,10.8203},
{2655.5302,2019.5533,10.8192},
{2650.5493,2032.5130,10.8129},
{2615.6044,2025.3696,10.8203},
{2598.7846,2027.3834,11.0234},
{2611.8569,2058.6748,10.8129},
{2616.6914,2077.8188,10.8129},
{2627.1108,2063.0588,10.8203},
{2641.3708,2041.4680,10.9531},
{2619.0693,2071.7497,18.1359},
{2621.1816,2043.5540,18.1359},
{2651.6418,2020.5380,18.1359},
{2641.7919,2029.6855,14.1160},
{2624.2121,2046.8682,14.1160},
{2612.3266,2071.5541,14.1160},
{2638.1428,1979.2020,14.1160}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_silentmotel_init();

public dm_silentmotel_init() // unique named init function (must be ran on gamemode init)
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
 	

