// Created:    22.07.08
// Aurthor:    MadCat
// Based on PEN1 DM

#if defined _dm_estateofhate_included
  #endinput
#endif

#define _dm_estateofhate_included
#pragma library dm_estateofhate
#include "deathmatch"

static name[MAX_NAME] = "Estate Of Hate";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 5; 
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_TEC9,WEAPON_DEAGLE};
static Float:startcp[CoordInfo] = {2287.2112,775.9848,10.6719}; 
static Float:worldbounds[DMWBInfo] = {2282.328613,2151.107177,753.143188,629.206420}; 
static Float:spawnpoints[][CoordInfo] = {
{2269.245361,684.852722,11.351562},
{2245.361083,682.785400,11.453125},
{2217.729248,678.182250,11.351562},
{2192.125244,684.124328,11.366704},
{2165.291503,684.572326,11.359375},
{2178.287597,654.700683,11.460937},
{2206.803710,655.901184,11.468292},
{2235.117919,670.659179,11.460937},
{2258.568603,655.047607,11.453125},
{2269.953369,662.630798,11.453125},
{2264.205078,750.107788,11.359375},
{2244.603271,742.108154,11.359375},
{2227.417968,747.087219,11.460937},
{2192.171630,741.286254,11.162648},
{2204.338134,750.648803,11.272701},
{2172.685546,743.526794,15.114582},
{2251.416259,679.128112,14.685197},
{2233.238037,664.496765,15.341653},
{2169.614257,680.469726,14.844123},
{2160.389648,712.974121,10.671875}
};
static objects[][DMOInfo] = {0};

static dmid; // this will hold the id this dm is assigned at init

forward dm_estateofhate_init();

public dm_estateofhate_init() // unique named init function (must be ran on gamemode init)
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
 	

