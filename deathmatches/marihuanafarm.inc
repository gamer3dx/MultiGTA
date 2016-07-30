// Created:    		Unknown
// Aurthor:    		PaniC
// Converted by: 	MadCat

#if defined _dm_marihuanafarm_included
#endinput
#endif

#define _dm_marihuanafarm_included
#pragma library dm_marihuanafarm
#include "deathmatch"

static name[MAX_NAME] = "Marihuana Farm";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 11; 
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_FLAMETHROWER};
static Float:startcp[CoordInfo] = {-1050.2188,-1342.1931,130.3522};
static Float:worldbounds[DMWBInfo] = {-1031.1626,-1222.4324,-1065.9385,-1151.4442};
static Float:spawnpoints[][CoordInfo] = {
{-1184.3063,-1140.9867,129.2188},
{-1205.2019,-1073.9318,128.2656},
{-1039.8645,-1075.6873,129.2188},
{-1061.2871,-1144.0260,129.2188},
{-1086.0918,-1095.6030,131.5788},
{-1139.0787,-1095.5630,131.5874},
{-1128.5205,-1141.3351,129.2188},
{-1153.9856,-1110.3574,128.5381},
{-1082.0433,-1133.4869,129.2188},
{-1087.6727,-1074.3470,129.2188}
};
static objects[][DMOInfo] = {
{14400,-1093.5865,-1095.5461,128.9179,0.0000,0.0000,0.0000},
{14400,-1085.7172,-1095.1239,128.6712,0.0000,0.0000,0.0000},
{14400,-1078.6178,-1095.6401,128.8202,0.0000,0.0000,191.2501},
{14400,-1126.1849,-1095.6475,129.3085,0.0000,0.0000,191.2501},
{14400,-1140.1498,-1095.1632,128.9482,0.0000,0.0000,22.5000},
{14400,-1131.8798,-1095.4172,129.0062,0.0000,0.0000,11.2500},
{14400,-1079.0869,-1084.2947,129.0211,0.0000,0.0000,11.2500},
{14400,-1087.1699,-1084.3666,129.0211,0.0000,0.0000,11.2500},
{14400,-1093.4832,-1083.8958,129.0211,0.0000,0.0000,11.2500},
{14400,-1140.9619,-1084.4645,129.0211,0.0000,0.0000,0.0000},
{14400,-1136.1172,-1084.3601,128.8320,0.0000,0.0000,0.0000},
{14400,-1129.8979,-1084.3740,129.0211,0.0000,0.0000,0.0000},
{14400,-1124.1592,-1084.3119,129.0211,0.0000,0.0000,0.0000},
{14400,-1186.7672,-1095.8163,129.0211,0.0000,0.0000,0.0000},
{14400,-1179.7040,-1095.8651,129.0211,0.0000,0.0000,0.0000},
{14400,-1171.8054,-1095.8792,129.0211,0.0000,0.0000,0.0000},
{14400,-1154.7012,-1134.5233,129.0211,0.0000,0.0000,0.0000},
{14400,-1148.4440,-1134.6263,129.0211,0.0000,0.0000,0.0000},
{14400,-1139.7065,-1134.5883,129.0211,0.0000,0.0000,0.0000}
};

static dmid; // this will hold the id this dm is assigned at init

forward dm_marihuanafarm_init();

public dm_marihuanafarm_init() // unique named init function (must be ran on gamemode init)
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
 	

