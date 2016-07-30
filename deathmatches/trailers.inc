// Created:     06.11.07
// Aurthor:    MadCat
// Based on PEN1 DM and fy_poolday

#if defined _dm_trailers_included
#endinput
#endif

#define _dm_trailers_included
#pragma library dm_trailers
#include "deathmatch"

static name[MAX_NAME] = "Trailer Site Massacre";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 6;
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_CHAINSAW,WEAPON_SAWEDOFF};
static Float:startcp[CoordInfo] = {-903.2100,-467.5856,25.8747};
static Float:worldbounds[DMWBInfo] = {-894.849060,-976.860656,-478.207092,-555.739562};
static Float:spawnpoints[][CoordInfo] = {
{-954.2405,-493.7746,25.9609},
{-963.6157,-513.7619,25.9536},
{-960.1833,-531.4965,25.9609},
{-951.4559,-541.3097,25.9609},
{-939.5800,-533.6817,25.9536},
{-934.4351,-544.8859,25.9536},
{-918.8156,-536.5830,25.9536},
{-941.4017,-489.9997,26.3636},
{-923.7095,-491.2413,25.9609},
{-930.2659,-498.9821,25.9609},
{-959.3814,-506.4108,26.2386},
{-966.5636,-511.5343,25.9536},
{-939.3737,-516.2214,29.0451},
{-928.0540,-520.6565,28.8428},
{-947.7541,-521.8938,25.9536},
{-959.6712,-495.1586,25.9536},
{-966.6309,-506.2429,25.9536},
{-920.2644,-491.3035,25.9609},
{-905.4075,-504.3875,25.9609},
{-910.1005,-542.8261,25.9536}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_trailers_init();

public dm_trailers_init() // unique named init function (must be ran on gamemode init)
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
 	

