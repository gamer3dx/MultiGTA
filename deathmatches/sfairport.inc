// Created:    21.07.08
// Aurthor:    MadCat
// Based on PEN1 DM

#if defined _dm_sfairport_included
#endinput
#endif

#define _dm_sfairport_included
#pragma library dm_sfairport
#include "deathmatch"

static name[MAX_NAME] = "SF Airport";
static frequency = 5;
static lineupdelay = 240;
static minplayers = 3;
static minlevel = 6;
static maxtime = 400;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_UZI,WEAPON_AK47};
static Float:startcp[CoordInfo] = {-1417.8375,-303.6674,6.2031};
static Float:worldbounds[DMWBInfo] = {-1422.174560,-1478.915527,-161.093948,-250.884552};
static Float:spawnpoints[][CoordInfo] = {
{-1451.526733,-236.966674,5.836594},
{-1431.666503,-166.719131,6.000000},
{-1429.636352,-179.387420,6.000000},
{-1441.140136,-191.276687,6.000000},
{-1430.935180,-208.066604,6.000000},
{-1442.915893,-219.726501,6.000000},
{-1427.387695,-228.344039,6.000000},
{-1437.603637,-236.972335,6.000000},
{-1450.548095,-222.357696,6.335937},
{-1450.255615,-190.400558,6.335937},
{-1460.980468,-174.325408,6.000000},
{-1474.152954,-169.560546,6.000000},
{-1458.476440,-187.438095,6.000000},
{-1455.667358,-206.496429,6.000000},
{-1459.837524,-227.924499,6.335937},
{-1476.446166,-247.802703,6.000000},
{-1476.230712,-220.655532,6.000000},
{-1476.513916,-198.835647,6.000000},
{-1460.481323,-249.355911,6.000000},
{-1469.946777,-173.188613,6.006942}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_sfairport_init();

public dm_sfairport_init() // unique named init function (must be ran on gamemode init)
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
 	

