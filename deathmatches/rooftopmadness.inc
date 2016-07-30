// Created:    		Unknown
// Aurthor:    		PaniC
// Converted by: 	MadCat

#if defined _dm_rooftopmadness_included
#endinput
#endif

#define _dm_rooftopmadness_included
#pragma library dm_rooftopmadness
#include "deathmatch"

static name[MAX_NAME] = "Rooftop Madness";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 3; 
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_AK47,WEAPON_CHAINSAW,WEAPON_COLT45};
static Float:startcp[CoordInfo] = {-2517.1311,555.4847,14.6172};
static Float:worldbounds[DMWBInfo] = {-2468.5071,-2492.7021,535.4303,511.9591};
static Float:spawnpoints[][CoordInfo] = {
{-2472.6538,516.3051,51.0638},
{-2489.1470,532.9020,51.0638},
{-2489.2625,516.3326,51.0638},
{-2472.7727,532.9048,51.0638},
{-2481.2712,516.2675,51.0638},
{-2481.2185,532.8273,51.0638},
{-2472.7197,524.8826,51.0638},
{-2489.2129,524.8209,51.0638}
};
static objects[][DMOInfo] = {
{971,-2489.851074,529.024536,53.658623,0.0000,0.0000,90.0000},
{971,-2489.844482,520.104980,53.658623,0.0000,0.0000,90.0000},
{971,-2485.434814,515.621826,53.658623,0.0000,0.0000,180.0000},
{971,-2476.527100,515.611023,53.658623,0.0000,0.0000,180.0000},
{971,-2472.084473,520.137512,53.658623,0.0000,0.0000,270.0000},
{971,-2472.084229,529.004883,53.658623,0.0000,0.0000,270.0000},
{971,-2476.550537,533.477051,53.658623,0.0000,0.0000,0.0000},
{971,-2485.416016,533.480530,53.658623,0.0000,0.0000,0.0000}
};

static dmid; // this will hold the id this dm is assigned at init

forward dm_rooftopmadness_init();

public dm_rooftopmadness_init() // unique named init function (must be ran on gamemode init)
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
 	

