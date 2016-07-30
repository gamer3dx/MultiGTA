// Created:     12.12.07
// Aurthor:    MadCat
// Based on PEN1 DM

#if defined _dm_bigears_included
  #endinput
#endif

#define _dm_bigears_included
#pragma library dm_bigears
#include "deathmatch"

static name[MAX_NAME] = "Big Ears";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 1; 
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_TEC9,WEAPON_COLT45};
static Float:startcp[CoordInfo] = {-375.4071,1515.6217,75.5625}; 
static Float:worldbounds[DMWBInfo] = {-231.129577,-417.137145,1656.435791,1502.358764};
static Float:spawnpoints[][CoordInfo] = {
{-344.8055,1608.1218,75.8182},
{-333.7873,1595.5852,76.1674},
{-346.5455,1581.9250,76.2736},
{-359.5425,1595.1401,76.7652},
{-347.7568,1626.0548,75.4668},
{-347.0443,1563.4007,75.6757},
{-340.7604,1546.7175,75.5625},
{-344.6003,1537.6513,75.5625},
{-338.0122,1540.9638,75.5625},
{-315.2297,1540.2291,75.5625},
{-302.3006,1553.9666,75.3593},
{-329.5207,1549.8393,75.5625},
{-303.9284,1582.7673,75.3593},
{-350.8038,1513.2609,75.5625},
{-367.8640,1510.6767,76.3117},
{-375.5019,1514.8021,75.5625},
{-391.0575,1510.1553,76.4990},
{-402.4384,1517.6770,75.4322},
{-378.8589,1541.5864,75.5625},
{-379.5998,1567.4360,75.7643}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_bigears_init();

public dm_bigears_init() // unique named init function (must be ran on gamemode init)
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
 	

