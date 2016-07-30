// Created:     07.08.05
// Aurthor:    	LusoFresh, Wrong_killer
// Converter:	MadCat

#if defined _dm_oilrefinery_included
  #endinput
#endif

#define _dm_oilrefinery_included
#pragma library dm_oilrefinery
#include "deathmatch"

static name[MAX_NAME] = "Oil Refinery";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 6;
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_SHOTGUN,WEAPON_DEAGLE,WEAPON_KNIFE};
static Float:startcp[CoordInfo] = {312.0745,1409.3800,8.5169}; 
static Float:worldbounds[DMWBInfo] = {288.250213,112.130455,1484.262939,1336.295410}; 
static Float:spawnpoints[][CoordInfo] = {
{286.5413,1337.7733,10.5859},
{113.0810,1337.4951,10.5859},
{146.4913,1410.1583,10.5859},
{112.5693,1484.3369,10.6311},
{262.7251,1469.4077,10.5859},
{117.5676,1342.6348,10.5859},
{141.9213,1375.8594,10.5925},
{135.8059,1409.1870,10.5940},
{129.7063,1477.3684,10.6101},
{190.6844,1449.3586,10.5859},
{206.6887,1398.6771,10.5859},
{186.0208,1356.0344,10.5859},
{188.2015,1371.2696,23.7343},
{224.2314,1371.5162,10.5859},
{248.3055,1360.4553,10.7075},
{246.3993,1361.9348,23.3702},
{246.9925,1386.0461,10.7075},
{246.7565,1410.3547,23.3750},
{247.4889,1430.6126,10.7075},
{251.3623,1477.0354,10.5859},
{215.5012,1465.5297,14.9218},
{215.4031,1467.0494,23.7343},
{206.8190,1445.1154,10.5859},
{220.8736,1423.6634,10.5859},
{202.9299,1409.8955,11.9554}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_oilrefinery_init();

public dm_oilrefinery_init() // unique named init function (must be ran on gamemode init)
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
 	

