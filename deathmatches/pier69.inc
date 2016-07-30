// Created:     04.12.07
// Aurthor:    	MadCat
// Based on PEN1 DM

#if defined _dm_pier69_included
#endinput
#endif

#define _dm_pier69_included
#pragma library dm_pier69
#include "deathmatch"

static name[MAX_NAME] = "Pier 69";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 16;
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_AK47,WEAPON_MP5};
static Float:startcp[CoordInfo] = {-1706.9750,1336.0399,7.1748};
static Float:worldbounds[DMWBInfo] = {-1612.470092,-1720.395019,1433.338745,1318.780883};
static Float:spawnpoints[][CoordInfo] = {
{-1642.1387,1391.8961,7.1796},
{-1648.1053,1370.5805,7.1796},
{-1658.5810,1346.9914,7.1835},
{-1685.5899,1334.9797,7.1796},
{-1701.2213,1367.0493,7.1721},
{-1694.9061,1374.4730,7.1796},
{-1671.3367,1388.6824,7.1796},
{-1662.2110,1403.1597,7.1721},
{-1642.1387,1391.8961,7.1796},
{-1652.2403,1427.4390,7.1875},
{-1663.6971,1413.4810,12.3906},
{-1674.1658,1401.0064,12.2031},
{-1684.9387,1372.2924,9.7971},
{-1669.1010,1356.0544,7.1721},
{-1650.8835,1378.4334,9.8046},
{-1644.0751,1384.5998,9.8046},
{-1635.7717,1392.6624,9.8046},
{-1654.7012,1430.1334,7.1844},
{-1690.0314,1397.9510,7.1809},
{-1703.6796,1377.8155,7.1875}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_pier69_init();

public dm_pier69_init() // unique named init function (must be ran on gamemode init)
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
 	

