// Created:    3.01.08
// Aurthor:    MadCat
// Based on PEN1 DM

#if defined _dm_quarry_included
  #endinput
#endif

#define _dm_quarry_included
#pragma library dm_quarry
#include "deathmatch"

static name[MAX_NAME] = "Quarry";
static frequency = 5;
static lineupdelay = 240;
static minplayers = 4;
static minlevel = 11;
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_SNIPER,WEAPON_M4};
static Float:startcp[CoordInfo] = {829.2277,858.5878,12.4015};
static Float:worldbounds[DMWBInfo] = {791.248107,403.263366,1051.693969,753.162170};
static Float:spawnpoints[][CoordInfo] = {
{676.7499,919.2949,-40.7850},
{690.8052,902.8717,-39.3112},
{694.1703,869.5484,-41.1643},
{676.2346,838.1369,-42.9609},
{663.9369,815.1820,-42.9609},
{629.3265,810.0342,-42.9609},
{591.9199,835.5535,-42.7549},
{550.8500,838.2711,-40.8498},
{514.9875,854.2427,-41.4098},
{494.8492,850.4771,-29.8959},
{518.4849,817.8931,-24.6635},
{529.4695,787.8928,-20.7801},
{577.9982,765.7546,-16.1111},
{641.7821,752.3074,-11.5680},
{680.1932,754.8502,-6.7673},
{726.7155,789.1483,-18.7223},
{737.2858,828.7702,-18.7011},
{735.6025,877.6398,-26.5009},
{702.8073,911.2553,-30.4552},
{685.5637,941.8954,-30.6918}
};
static objects[][DMOInfo] = {0};

static dmid;

forward dm_quarry_init();

public dm_quarry_init()
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
 	

