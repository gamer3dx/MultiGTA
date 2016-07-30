// Created:    04.12.07
// Aurthor:    MadCat
// Based on PEN1 DM

#if defined _dm_glen_included
  #endinput
#endif

#define _dm_glen_included
#pragma library dm_glen
#include "deathmatch"

static name[MAX_NAME] = "Glen Park";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 2; 
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_SHOTGUN,WEAPON_COLT45,WEAPON_KNIFE};
static Float:startcp[CoordInfo] = {1968.7186,-1200.1226,25.6354}; 
static Float:worldbounds[DMWBInfo] = {2057.134277,1862.031494,-1146.036254,-1250.366210}; 
static Float:spawnpoints[][CoordInfo] = {
{1881.8935,-1236.1875,14.8684},
{1875.7109,-1215.1308,18.1211},
{1881.0621,-1184.9636,23.0437},
{1892.1373,-1161.8793,23.9885},
{1927.2458,-1159.4653,22.0408},
{1964.2587,-1158.0246,20.8300},
{1997.6809,-1157.1385,20.9142},
{2030.8094,-1156.8372,22.4368},
{2041.9680,-1179.8332,23.1596},
{2040.8148,-1196.5012,22.9908},
{2045.5078,-1225.6076,23.0405},
{2034.4610,-1242.3885,22.8622},
{2028.3110,-1223.9484,22.1317},
{2014.2260,-1228.1784,21.4079},
{1996.9699,-1231.0616,20.5663},
{1964.2161,-1236.5527,19.9210},
{1933.9885,-1239.5922,17.9814},
{1912.0308,-1226.4957,17.3690},
{1895.5753,-1226.8687,15.9814},
{1903.0312,-1204.8803,19.6590}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_glen_init();

public dm_glen_init() // unique named init function (must be ran on gamemode init)
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
 	

