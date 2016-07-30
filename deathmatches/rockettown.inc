// Created:    04.12.07
// Aurthor:    MadCat
// Based on PEN1 DM (TownL)

#if defined _dm_rockettown_included
  #endinput
#endif

#define _dm_rockettown_included
#pragma library dm_rockettown
#include "deathmatch"

static name[MAX_NAME] = "Rocket Town";
static frequency = 5;
static lineupdelay = 240;
static minplayers = 4;
static minlevel = 0;
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_ROCKETLAUNCHER};
static Float:startcp[CoordInfo] = {-2466.1228,2233.4314,4.0904};
static Float:worldbounds[DMWBInfo] = {-2173.2800,-2680.6550,2602.2607,2189.3039};
static Float:spawnpoints[][CoordInfo] = {
{-2491.6259,2512.6875,17.9628},
{-2418.1779,2491.6738,12.6011},
{-2331.3730,2449.3051,4.5793},
{-2258.7104,2363.4191,4.7017},
{-2269.4956,2287.7995,4.5408},
{-2318.1982,2344.1909,4.7100},
{-2390.5913,2369.7250,3.4261},
{-2462.6271,2331.7487,4.5538},
{-2445.6987,2225.5644,4.5638},
{-2572.6936,2275.2321,5.5831},
{-2615.9306,2280.9875,7.9118},
{-2567.5949,2329.7658,4.5524},
{-2586.4924,2422.6491,14.5742},
{-2509.1833,2418.7590,16.3179},
{-2400.1367,2430.2783,10.5509},
{-2345.6103,2366.3693,5.0084},
{-2484.5319,2374.0327,11.6093},
{-2491.3911,2291.8522,4.7052},
{-2515.1752,2268.4709,4.5578},
{-2466.4389,2512.8823,16.7077}
};
static objects[][DMOInfo] = {0};

static dmid;

forward dm_rockettown_init();

public dm_rockettown_init()
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
 	

