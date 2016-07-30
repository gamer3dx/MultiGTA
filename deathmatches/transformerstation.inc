// Created:    		Unknown
// Aurthor:    		PaniC
// Converted by: 	MadCat

#if defined _dm_transformerstation_included
#endinput
#endif

#define _dm_transformerstation_included
#pragma library dm_transformerstation
#include "deathmatch"

static name[MAX_NAME] = "Transformer Station";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 9;
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_TEC9,WEAPON_SAWEDOFF};
static Float:startcp[CoordInfo] = {872.5345,2015.0219,10.8203};
static Float:worldbounds[DMWBInfo] = {785.8947,748.0255,2081.4199,2037.2656};
static Float:spawnpoints[][CoordInfo] = {
{781.6649,2041.6450,6.7109},
{775.6770,2051.0217,6.7109},
{767.3374,2056.0029,6.7109},
{761.1628,2063.7065,6.7109},
{750.4757,2078.6450,6.7109},
{752.6568,2041.5389,6.7109},
{759.2098,2054.0967,6.7109},
{783.7416,2078.8621,6.7109},
{777.0743,2069.8103,6.7109}
};
static objects[][DMOInfo] = {
{974,782.0267,2080.9116,8.4887,0.0000,0.0000,0.0000},
{974,775.4092,2080.8696,8.4887,0.0000,0.0000,0.0000},
{974,768.7910,2080.8611,8.4887,0.0000,0.0000,0.0000},
{974,762.2645,2080.8469,8.4887,0.0000,0.0000,0.0000},
{974,755.6792,2080.8657,8.4887,0.0000,0.0000,0.0000},
{974,751.8131,2080.9182,8.4887,0.0000,0.0000,0.0000},
{974,751.8567,2037.5566,8.4887,0.0000,0.0000,0.0000},
{974,758.4402,2037.5570,8.4887,0.0000,0.0000,0.0000},
{974,765.1224,2037.5382,8.4887,0.0000,0.0000,0.0000},
{974,781.9727,2037.5603,8.4887,0.0000,0.0000,0.0000},
{974,771.6865,2037.5377,8.4887,0.0000,0.0000,0.0000},
{974,776.4999,2037.5334,8.4887,0.0000,0.0000,0.0000},
{974,785.3757,2040.9617,8.4962,0.0000,0.0000,270.0000},
{974,785.3400,2047.6339,8.4887,0.0000,0.0000,270.0000},
{974,785.3223,2054.3113,8.4887,0.0000,0.0000,270.0000},
{974,785.3168,2060.9424,8.4887,0.0000,0.0000,270.0000},
{974,785.3665,2067.6494,8.4887,0.0000,0.0000,270.0000},
{974,785.3297,2074.1948,8.4887,0.0000,0.0000,270.0000},
{974,785.3142,2077.4868,8.4887,0.0000,0.0000,270.0000},
{974,748.4391,2041.0494,8.4887,0.0000,0.0000,270.0000},
{974,748.4615,2047.4796,8.4887,0.0000,0.0000,270.0000},
{974,748.4517,2052.9622,8.4887,0.0000,0.0000,270.0000},
{974,748.4556,2059.4597,8.4887,0.0000,0.0000,270.0000},
{974,748.4738,2066.1096,8.4887,0.0000,0.0000,270.0000},
{974,748.4692,2072.5771,8.4887,0.0000,0.0000,270.0000},
{974,748.4851,2077.5374,8.4887,0.0000,0.0000,270.0000}
};

static dmid; // this will hold the id this dm is assigned at init

forward dm_transformerstation_init();

public dm_transformerstation_init() // unique named init function (must be ran on gamemode init)
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
 	

