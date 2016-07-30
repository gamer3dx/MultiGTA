// Created:     2007.03.25
// Aurthor:    addam, and not author, just ported from EightBall-s Mod

#if defined _dm_poolday_included
#endinput
#endif

#define _dm_poolday_included
#pragma library dm_poolday
#include "deathmatch"

static name[MAX_NAME] = "fy_poolday";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 4;
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_KNIFE,WEAPON_DEAGLE,WEAPON_MP5};
static Float:startcp[CoordInfo] = {951.68,-1355.8486,13.0234};
static Float:worldbounds[DMWBInfo] = {1830.3374,1698.2655,2882.4065,2724.4895};
static Float:spawnpoints[][CoordInfo] = {
{1740.5600, 2860.7915, 10.9963},
{1750.7561, 2866.1342, 10.9963},
{1761.8535, 2869.1555, 10.9963},
{1773.4731, 2868.8784, 10.9963},
{1784.3555, 2865.9855, 10.9963},
{1794.2564, 2860.8962, 10.9963},
{1732.0480, 2825.3142, 11.0041},
{1735.7374, 2813.7873, 11.0041},
{1738.4523, 2802.6345, 11.0041},
{1747.1044, 2769.0388, 11.0041},
{1750.8385, 2757.8474, 11.0041},
{1753.4259, 2746.1997, 11.0041},
{1800.3695, 2825.2910, 11.0041},
{1797.4038, 2814.0288, 11.0041},
{1794.6258, 2802.5900, 11.0041},
{1785.2344, 2768.9804, 11.0041},
{1782.2141, 2758.0842, 11.0041},
{1779.2082, 2746.6228, 11.0041}
};
static objects[][DMOInfo] = {0};
	
static dmid; // this will hold the id this dm is assigned at init

forward dm_poolday_init();

public dm_poolday_init() // unique named init function (must be ran on gamemode init)
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
 	

