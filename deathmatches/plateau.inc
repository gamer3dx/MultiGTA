// Created:    19.10.07
// Aurthor:    MadCat
// Based on PEN1 DM

#if defined _dm_plateau_included
  #endinput
#endif

#define _dm_plateau_included
#pragma library dm_plateau
#include "deathmatch"

static name[MAX_NAME] = "Plateau";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 7;
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_SNIPER,WEAPON_SAWEDOFF};
static Float:startcp[CoordInfo] = {-392.1998,2486.5840,41.3143}; 
static Float:worldbounds[DMWBInfo] = {-193.871093,-311.391967,2418.425048,2135.330322}; 
static Float:spawnpoints[][CoordInfo] = {
{-201.0050,2357.6503,102.9355},
{-217.7940,2392.6596,108.1198},
{-241.7736,2399.3955,108.7300},
{-263.7114,2403.8312,109.0810},
{-283.3517,2388.4492,110.0053},
{-298.9599,2369.7885,111.9380},
{-293.8353,2342.8227,111.1900},
{-275.9922,2317.0673,109.9295},
{-235.5797,2306.3928,109.0500},
{-226.3814,2333.6801,108.9657},
{-252.8674,2354.2189,108.8838},
{-253.4218,2336.3100,109.0383},
{-253.0398,2320.0317,109.9880},
{-246.1487,2280.9851,109.9326},
{-260.7557,2178.9389,111.5906},
{-268.3274,2153.6899,110.5943},
{-275.8409,2141.4787,112.7033},
{-295.3037,2151.3647,112.6075},
{-283.0235,2161.5693,112.8311},
{-274.9072,2183.3222,112.4682}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_plateau_init();

public dm_plateau_init() // unique named init function (must be ran on gamemode init)
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
 	

