// Created:    2.09.07
// Aurthor:    MadCat
// Based on PEN1 DM

#if defined _dm_castle_included
  #endinput
#endif

#define _dm_castle_included
#pragma library dm_castle
#include "deathmatch"

static name[MAX_NAME] = "Castle";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 7; 
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_KATANA,WEAPON_TEC9,WEAPON_SNIPER};
static Float:startcp[CoordInfo] = {2137.8110,1138.3346,13.5036}; 
static Float:worldbounds[DMWBInfo] = {2300.00,2133.33,1153.33,993.33};  
static Float:spawnpoints[][CoordInfo] = {
{2288.3618,1051.8056,26.7031},
{2295.6547,1072.0712,26.7031},
{2257.2448,1050.5297,29.3750},
{2265.6586,1077.1247,29.3750},
{2265.5568,1105.7515,29.3750},
{2249.0554,1122.6114,33.5234},
{2248.7175,1089.3822,33.5234},
{2226.5446,1078.3436,33.5234},
{2202.1994,1065.5731,29.3818},
{2201.8271,1102.2849,29.4100},
{2225.2363,1117.0587,29.5353},
{2204.5791,1124.1284,26.7031},
{2196.0664,1104.9509,26.7064},
{2176.5778,1101.0277,23.3359},
{2158.5478,1112.6774,23.3359},
{2154.1950,1123.9622,23.3359},
{2159.6579,1137.0355,23.3359},
{2173.3613,1136.1483,23.3359},
{2234.7951,1063.1007,33.5234},
{2244.9992,1078.0931,33.5234}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_castle_init();

public dm_castle_init() // unique named init function (must be ran on gamemode init)
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
 	

