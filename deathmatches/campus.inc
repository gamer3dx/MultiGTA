// Created:    22.07.08
// Aurthor:    MadCat
// Based on PEN1 DM

#if defined _dm_campus_included
  #endinput
#endif

#define _dm_campus_included
#pragma library dm_campus
#include "deathmatch"

static name[MAX_NAME] = "Campus";
static frequency = 5;
static lineupdelay = 240;
static minplayers = 3;
static minlevel = 4;
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_SHOTGUN,WEAPON_COLT45,WEAPON_UZI};
static Float:startcp[CoordInfo] = {-1955.4435,-998.6854,32.1856};
static Float:worldbounds[DMWBInfo] = {-1945.177368,-1964.287597,-725.070068,-992.587463};
static Float:spawnpoints[][CoordInfo] = {
{-1956.510620,-733.428649,35.890884},
{-1949.640502,-754.378234,35.890884},
{-1960.540405,-776.620727,35.890884},
{-1952.167968,-805.626159,35.890884},
{-1961.134033,-831.926025,35.890884},
{-1949.323364,-833.500549,35.890884},
{-1947.387451,-816.819946,35.890884},
{-1960.678344,-808.517150,35.890884},
{-1961.050903,-789.858459,35.890884},
{-1948.747802,-778.394958,35.890884},
{-1949.252075,-874.873413,35.890625},
{-1957.587158,-874.148010,35.890625},
{-1960.225463,-885.377014,35.890884},
{-1947.817993,-898.808166,35.890884},
{-1958.291381,-913.520629,35.890884},
{-1949.461181,-928.960510,35.890884},
{-1958.427978,-942.187805,35.890884},
{-1950.338867,-952.927246,35.890884},
{-1954.652587,-972.795776,35.890884},
{-1954.223876,-982.866516,35.890884}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_campus_init();

public dm_campus_init() // unique named init function (must be ran on gamemode init)
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
 	

