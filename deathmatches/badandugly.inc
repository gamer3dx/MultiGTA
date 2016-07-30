// Created:     09.09.06
// Aurthor:    Iain Gilbert

#if defined _dm_badandugly_included
  #endinput
#endif

#define _dm_badandugly_included
#pragma library dm_badandugly
#include "deathmatch"

static name[MAX_NAME] = "The Bad And The Ugly";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 3; 
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_DEAGLE};
static Float:startcp[CoordInfo] = {143.5471,-1567.5682,10.7642}; 
static Float:worldbounds[DMWBInfo] = {-323.325988,-469.007293,2276.812255,2182.392333};
static Float:spawnpoints[][CoordInfo] = {
{-457.9337,2221.7229,43.1974},
{-445.4723,2224.2827,42.4296},
{-433.1791,2252.6982,42.4296},
{-423.9286,2244.6816,42.4296},
{-401.7221,2258.8703,42.3958},
{-395.4619,2240.3461,42.4296},
{-410.3262,2261.5219,42.4296},
{-373.0246,2268.0539,42.2728},
{-363.6086,2262.6682,42.4843},
{-367.0554,2245.1052,42.4843},
{-349.8372,2239.4133,42.4843},
{-353.4781,2230.4685,42.4843},
{-355.2925,2217.1618,42.4843},
{-361.3502,2202.7827,42.4843},
{-390.0183,2211.8059,42.4248},
{-392.4468,2221.4060,42.4296},
{-395.5477,2233.3703,42.4296},
{-415.8809,2221.3332,42.4296},
{-422.4342,2225.8901,42.4296},
{-392.7534,2197.3142,42.4244}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_badandugly_init();

public dm_badandugly_init() // unique named init function (must be ran on gamemode init)
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
 	

