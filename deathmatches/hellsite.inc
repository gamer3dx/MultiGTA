// Created:    21.07.08
// Aurthor:    MadCat
// Based on PEN1 DM

#if defined _dm_hellsite_included
#endinput
#endif

#define _dm_hellsite_included
#pragma library dm_hellsite
#include "deathmatch"

static name[MAX_NAME] = "HellSite";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 7; 
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_RIFLE,WEAPON_MP5};
static Float:startcp[CoordInfo] = {2713.3049,920.2012,10.8691};
static Float:worldbounds[DMWBInfo] = {2716.776611,2557.756835,913.640930,782.650390};
static Float:spawnpoints[][CoordInfo] = {
{2561.792724,808.261169,5.315795},
{2579.542968,789.121215,5.315795},
{2608.919433,807.007629,5.315795},
{2649.036865,791.190856,5.315795},
{2653.740722,817.056091,5.718367},
{2658.625000,834.320312,8.820311},
{2677.076416,835.563964,13.712673},
{2694.696044,814.221862,10.954463},
{2714.515625,792.193908,10.898436},
{2714.651611,807.518493,13.712673},
{2692.330078,827.214538,10.093523},
{2690.867919,855.949279,9.921442},
{2672.057617,863.835815,10.939545},
{2690.520751,867.428161,13.187500},
{2700.315429,905.056701,10.531438},
{2689.150146,906.034240,10.563628},
{2654.769042,869.791503,6.166127},
{2630.310791,839.936218,5.315795},
{2606.780761,839.057006,5.209523},
{2616.362548,806.095520,10.954463}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_hellsite_init();

public dm_hellsite_init() // unique named init function (must be ran on gamemode init)
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
 	

