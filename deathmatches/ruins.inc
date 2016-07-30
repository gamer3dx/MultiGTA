// Created:    14.10.07
// Aurthor:    MadCat
// Based on PEN1 DM

#if defined _dm_ruins_included
  #endinput
#endif

#define _dm_ruins_included
#pragma library dm_ruins
#include "deathmatch"

static name[MAX_NAME] = "Ruins";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 12;
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_TEC9,WEAPON_DEAGLE};
static Float:startcp[CoordInfo] = {-1409.0408,2584.0815,55.8359}; 
static Float:worldbounds[DMWBInfo] = {-1275.137817,-1342.897705,2564.696777,2477.308837};  
static Float:spawnpoints[][CoordInfo] = {
{-1319.7839,2493.1047,87.0468},
{-1303.0017,2490.9863,87.1564},
{-1285.2250,2492.4404,87.0810},
{-1290.7830,2513.0266,87.0397},
{-1282.8666,2515.6174,87.1318},
{-1295.2890,2535.1074,87.7421},
{-1298.2517,2548.8232,87.4485},
{-1306.9410,2555.7507,87.2474},
{-1315.3458,2545.8996,87.7421},
{-1323.0771,2541.3781,87.0327},
{-1331.8085,2529.5646,87.1905},
{-1324.7119,2517.2150,87.0468},
{-1306.8546,2526.6608,87.5576},
{-1313.6994,2512.3193,87.0420},
{-1303.5278,2507.3820,87.0420},
{-1296.6198,2527.7497,87.6005},
{-1302.1783,2535.1020,87.7421},
{-1303.8275,2502.8425,87.0359},
{-1293.0081,2502.7255,86.9146},
{-1285.3707,2526.2500,87.5415}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_ruins_init();

public dm_ruins_init() // unique named init function (must be ran on gamemode init)
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
 	

