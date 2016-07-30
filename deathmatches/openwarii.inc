// Created:    22.07.08
// Aurthor:    MadCat
// Based on PEN1 DM

#if defined _dm_openwarii_included
  #endinput
#endif

#define _dm_openwarii_included
#pragma library dm_openwarii
#include "deathmatch"

static name[MAX_NAME] = "Open War II";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 12;
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_SNIPER,WEAPON_MP5,WEAPON_M4};
static Float:startcp[CoordInfo] = {-1045.3302,1551.8055,33.3530}; 
static Float:worldbounds[DMWBInfo] = {-917.733154,-1057.083984,1624.611694,1371.150024};
static Float:spawnpoints[][CoordInfo] = {
{-1022.824157,1499.754272,42.228347},
{-1006.372436,1467.467041,44.810657},
{-977.588806,1469.636962,45.881484},
{-961.012878,1488.106933,44.739944},
{-983.069030,1502.133056,44.286571},
{-1008.879333,1502.362182,43.620056},
{-983.092468,1514.576538,43.227680},
{-966.023864,1529.632812,45.467483},
{-991.819702,1541.223876,41.513210},
{-1005.910949,1508.520263,42.822463},
{-1016.391967,1489.033935,45.046592},
{-982.443054,1522.132568,43.384670},
{-962.368591,1539.008178,45.134033},
{-980.277954,1554.918579,43.696430},
{-997.702209,1543.506103,40.359367},
{-962.434570,1514.624633,43.477249},
{-955.215820,1542.616210,44.629863},
{-974.435058,1566.427001,43.361637},
{-957.065673,1565.396362,42.419342},
{-993.346374,1571.567138,39.391059}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_openwarii_init();

public dm_openwarii_init() // unique named init function (must be ran on gamemode init)
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
 	

