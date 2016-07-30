// Created:    22.07.08
// Aurthor:    MadCat
// Based on PEN1 DM

#if defined _dm_suburb_included
  #endinput
#endif

#define _dm_suburb_included
#pragma library dm_suburb
#include "deathmatch"

static name[MAX_NAME] = "Suburb";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 6;
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_SHOTGUN,WEAPON_DEAGLE,WEAPON_KNIFE};
static Float:startcp[CoordInfo] = {1716.8483,2062.3621,10.8203}; 
static Float:worldbounds[DMWBInfo] = {1713.880371,1517.777587,2179.589111,2004.526245}; 
static Float:spawnpoints[][CoordInfo] = {
{1686.8115,2047.1845,11.4687},
{1680.4648,2067.4697,11.3593},
{1687.1687,2092.5822,11.4609},
{1686.5659,2123.7673,11.4609},
{1681.8796,2151.8674,10.8125},
{1639.4726,2148.9140,11.3125},
{1598.2259,2147.0393,11.4609},
{1553.0197,2132.7119,11.4609},
{1548.5054,2096.1933,11.4609},
{1553.4620,2075.6684,11.3593},
{1686.8115,2047.1845,11.4687},
{1596.7316,2038.7486,11.4687},
{1646.3155,2058.3007,11.0947},
{1643.4617,2083.2248,11.3125},
{1643.1154,2110.0581,11.3125},
{1644.2626,2130.5080,11.2031},
{1639.4996,2148.1203,11.3125},
{1660.6635,2154.9812,10.8203},
{1670.1702,2100.9658,11.4609},
{1670.1602,2055.5131,11.3671}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_suburb_init();

public dm_suburb_init() // unique named init function (must be ran on gamemode init)
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
 	

