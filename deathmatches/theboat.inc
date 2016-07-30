// Created:    22.07.08
// Aurthor:    MadCat
// Based on PEN1 DM

#if defined _dm_theboat_included
#endinput
#endif

#define _dm_theboat_included
#pragma library dm_theboat
#include "deathmatch"

static name[MAX_NAME] = "The Boat";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 4;
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_KNIFE,WEAPON_DEAGLE,WEAPON_MP5};
static Float:startcp[CoordInfo] = {-2413.6912,1397.2499,7.1875};
static Float:worldbounds[DMWBInfo] = {-2299.585205,-2478.575683,1561.617553,1528.177734};
static Float:spawnpoints[][CoordInfo] = {
{-2414.0000,1551.8000,2.0999},
{-2406.8999,1555.1999,2.0999},
{-2392.8000,1550.4000,2.0999},
{-2406.6000,1545.0999,2.0999},
{-2391.8999,1539.1999,2.0999},
{-2379.6999,1541.8000,2.0999},
{-2366.8999,1539.5999,2.0999},
{-2426.6000,1537.4000,2.0999},
{-2473.5000,1550.0999,33.2000},
{-2474.1999,1546.8000,23.6000},
{-2464.1999,1530.9000,17.2999},
{-2437.5000,1559.0000,17.2999},
{-2416.3000,1554.3000,26.0000},
{-2424.6999,1542.5000,26.0000},
{-2410.5000,1532.4000,26.0000},
{-2403.5000,1546.5000,26.0000},
{-2389.1999,1550.6999,26.0000},
{-2368.0000,1556.6999,17.2999},
{-2351.5000,1551.0999,23.1000},
{-2313.6999,1546.4000,18.7000}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_theboat_init();

public dm_theboat_init() // unique named init function (must be ran on gamemode init)
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
 	

