// Created:    22.07.08
// Aurthor:    MadCat
// Based on PEN1 DM

#if defined _dm_openwar_included
  #endinput
#endif

#define _dm_openwar_included
#pragma library dm_openwar
#include "deathmatch"

static name[MAX_NAME] = "Open War";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 12;
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_SNIPER,WEAPON_MP5,WEAPON_M4};
static Float:startcp[CoordInfo] = {423.1488,2715.2151,62.5617}; 
static Float:worldbounds[DMWBInfo] = {501.959289,350.183990,2843.548828,2711.603515};
static Float:spawnpoints[][CoordInfo] = {
{430.462677,2826.691162,66.874069},
{406.193725,2826.384033,69.845222},
{408.969787,2810.299316,73.814643},
{416.482116,2797.083740,76.032676},
{404.729156,2786.895263,79.656318},
{409.597564,2768.879638,80.159225},
{433.972686,2760.937988,76.903701},
{449.738006,2770.400146,75.939552},
{463.059326,2767.709960,74.864318},
{466.403991,2754.863525,72.958236},
{454.817810,2742.226074,72.194137},
{438.489959,2743.230468,74.487152},
{422.691314,2751.591552,77.566909},
{404.779998,2753.803710,79.958526},
{394.402557,2764.798828,82.646354},
{383.893371,2775.504638,82.438392},
{383.553100,2789.244140,80.991226},
{386.117462,2803.502197,77.027824},
{376.570831,2814.023925,73.704299},
{357.974090,2807.351806,77.893058}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_openwar_init();

public dm_openwar_init() // unique named init function (must be ran on gamemode init)
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
 	

