// Created:     28.08.08
// Aurthor:    	MadCat
// Some spawns from GTAT Ganghouse module and Mini DM [SAWOFF]

#if defined _dm_toyworld_included
#endinput
#endif

#define _dm_toyworld_included
#pragma library dm_toyworld
#include "deathmatch"

static name[MAX_NAME] = "Toy World";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 10;
static maxtime = 300;
static activedmjoin = 1;
static interior = 10;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_SHOTGSPA,WEAPON_M4,WEAPON_MP5,WEAPON_DEAGLE};
static Float:startcp[CoordInfo] = {-2245.2336,128.6670,35.3203};
static Float:worldbounds[DMWBInfo] = {3000.0,-3000.0,3000.0,-3000.0};
static Float:spawnpoints[][CoordInfo] = {
{-1136.3870,1069.7966,1345.7938},
{-1132.1959,1028.8492,1345.7335},
{-1065.7354,1021.2379,1343.1431},
{-1040.0916,1098.2620,1343.1339},
{-969.4229,1050.0602,1345.0519},
{-974.9559,1089.8304,1344.9742},
{-1133.1169,1028.1941,1345.7385},
{-1136.6593,1087.1188,1345.8151},
{-1101.6686,1019.9733,1342.0938},
{-1064.2626,1022.6210,1343.2338},
{-1047.7893,1094.7968,1343.7289},
{-1039.6228,1098.5415,1343.1370},
{-1008.5620,1021.0773,1341.0078},
{-969.7017,1032.2458,1345.0674},
{-973.6133,1089.1442,1344.9845},
{-1136.3495,1030.0719,1345.7588},
{-1136.3159,1066.8989,1345.7935},
{-1125.5807,1052.8723,1345.7097},
{-1125.4327,1062.6508,1345.7118},
{-970.8738,1090.0911,1345.0026},
{-979.8319,1066.0507,1344.9663},
{-979.9651,1056.2080,1344.9784},
{-970.7556,1053.0442,1345.0398}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_toyworld_init();

public dm_toyworld_init() // unique named init function (must be ran on gamemode init)
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
 	

