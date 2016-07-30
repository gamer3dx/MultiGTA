// Created:     07.11.07
// Aurthor:    MadCat
// Based on PEN1 DM

#if defined _dm_towertop_included
  #endinput
#endif

#define _dm_towertop_included
#pragma library dm_towertop
#include "deathmatch"

static name[MAX_NAME] = "Tower Top";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 10;
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_SHOTGSPA};
static Float:startcp[CoordInfo] = {-2721.4453,-317.0628,7.8438}; 
static Float:worldbounds[DMWBInfo] = {-2709.496826,-2726.896484,-311.622131,-326.707092};
static Float:spawnpoints[][CoordInfo] = {
{-2722.0983,-319.3807,57.4872},
{-2719.1201,-316.3639,57.4872},
{-2717.3020,-316.3716,57.4872},
{-2714.4184,-318.8682,57.4872},
{-2714.6953,-321.2035,57.4872},
{-2721.9533,-321.0664,57.4872},
{-2719.4077,-318.0779,57.4872},
{-2717.7648,-317.7589,57.4872},
{-2716.5021,-318.8752,57.4872},
{-2715.6171,-320.2015,57.4872},
{-2716.6604,-321.0132,57.4872},
{-2718.7199,-321.6771,57.4872},
{-2718.9658,-320.5809,57.4872},
{-2717.7795,-319.1054,57.4872},
{-2717.4353,-319.3311,57.4872},
{-2717.7695,-320.1837,57.4872},
{-2718.7548,-320.5758,57.4872},
{-2718.5332,-318.3328,57.4872},
{-2720.2524,-320.5046,57.4872},
{-2718.7402,-319.7477,57.4872}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_towertop_init();

public dm_towertop_init() // unique named init function (must be ran on gamemode init)
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
 	

