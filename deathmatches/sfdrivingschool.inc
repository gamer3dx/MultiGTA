// Created:    		Unknown
// Aurthor:    		PaniC
// Converted by: 	MadCat

#if defined _dm_sfdrivingschool_included
#endinput
#endif

#define _dm_sfdrivingschool_included
#pragma library dm_sfdrivingschool
#include "deathmatch"

static name[MAX_NAME] = "SF Driving School";
static frequency = 5;
static lineupdelay = 240; 
static minplayers = 3;
static minlevel = 6;
static maxtime = 300;
static activedmjoin = 1;
static interior = 0;
static suddenexplosiontime = 0;
static weapons[MAX_DMWEAPONS+1] = {WEAPON_MP5};
static Float:startcp[CoordInfo] = {-2026.8662,-100.3210,35.1641};
static Float:worldbounds[DMWBInfo] = {-2010.6792,-2096.9929,-102.5430,-281.2554};
static Float:spawnpoints[][CoordInfo] = {
{-2078.2610,-253.9062,35.3203},
{-2034.7181,-264.5694,35.3203},
{-2086.0020,-141.7501,35.3203},
{-2053.7451,-124.6469,35.2949},
{-2085.0144,-185.2310,35.3203},
{-2021.8567,-203.3992,35.3203},
{-2058.3569,-198.3664,35.3203},
{-2056.1069,-164.3869,35.3203},
{-2045.5935,-231.2384,35.3203}
};
static objects[][DMOInfo] = {
{1225,-2074.741943,-242.716003,34.726067,0.0000,0.0000,292.5000},
{1225,-2054.632324,-231.921997,34.726067,0.0000,0.0000,213.7500},
{1225,-2047.503174,-251.756226,34.726067,0.0000,0.0000,146.2500},
{1225,-2041.669189,-205.820038,34.726067,0.0000,0.0000,78.7500},
{1225,-2074.076660,-214.524216,34.726067,0.0000,0.0000,22.5000},
{1225,-2081.062988,-181.876312,34.726067,0.0000,0.0000,213.7500},
{1225,-2060.740234,-188.603302,34.726067,0.0000,0.0000,123.7500},
{1225,-2041.856812,-165.788315,34.726067,0.0000,0.0000,56.2502},
{1225,-2028.206909,-228.191010,34.726067,0.0000,0.0000,348.7502},
{1225,-2074.585205,-153.624161,34.726067,0.0000,0.0000,348.7502},
{1225,-2055.032227,-131.963699,34.733147,0.0000,0.0000,292.5003},
{3374,-2070.104980,-142.029816,35.118515,0.0000,0.0000,348.7500},
{3374,-2036.896606,-137.532074,35.106712,0.0000,0.0000,78.7501},
{3374,-2052.804932,-140.424332,35.118515,0.0000,0.0000,213.7500},
{3374,-2055.082764,-126.602615,35.103531,0.0000,0.0000,146.2501},
{3374,-2083.644531,-142.101990,35.118515,0.0000,0.0000,78.7501},
{3374,-2084.442383,-160.215546,35.118515,0.0000,0.0000,11.2502},
{3374,-2083.124512,-183.790344,35.118515,0.0000,0.0000,315.0002},
{3374,-2034.969604,-156.449188,35.118515,0.0000,0.0000,146.2501},
{3374,-2024.643921,-175.205139,35.118515,0.0000,0.0000,247.5002},
{3374,-2024.165894,-202.961334,35.118515,0.0000,0.0000,78.7501},
{3374,-2040.030029,-187.112823,35.118515,0.0000,0.0000,56.2501},
{3374,-2059.663574,-200.348907,35.118515,0.0000,0.0000,146.2501},
{3374,-2054.850098,-185.588501,35.125595,0.0000,0.0000,146.2501},
{3374,-2071.434082,-186.768616,35.118515,0.0000,0.0000,146.2501},
{3374,-2076.532471,-200.182190,35.118515,0.0000,0.0000,146.2501},
{3374,-2047.289673,-214.238815,35.118515,0.0000,0.0000,90.0001},
{3374,-2080.841553,-218.639618,35.118515,0.0000,0.0000,11.2501},
{3374,-2065.335693,-227.101913,35.118511,0.0000,0.0000,281.2501},
{3374,-2025.323120,-225.957733,35.118515,0.0000,0.0000,281.2501},
{3374,-2044.787720,-233.468201,35.118515,0.0000,0.0000,202.5002},
{3374,-2033.181396,-247.618271,35.118515,0.0000,0.0000,123.7502},
{3374,-2036.314209,-262.804474,35.118515,0.0000,0.0000,45.0003},
{3374,-2063.433105,-256.042053,35.118515,0.0000,0.0000,337.5003},
{3374,-2081.742432,-234.613770,35.118515,0.0000,0.0000,258.7505},
{3374,-2056.750000,-166.686722,35.118515,0.0000,0.0000,168.7505},
{3374,-2062.081787,-210.773926,35.118515,0.0000,0.0000,112.5003},
{3374,-2025.727539,-147.790466,35.118511,0.0000,0.0000,78.7501},
{3374,-2040.447876,-172.513763,35.125595,0.0000,0.0000,0.0001},
{3374,-2039.866455,-196.759399,35.118515,0.0000,0.0000,270.0002},
{3374,-2022.191162,-242.667725,35.118515,0.0000,0.0000,213.7502},
{3374,-2054.864746,-250.663193,35.118515,0.0000,0.0000,146.2503},
{3374,-2076.908203,-251.781433,35.118515,0.0000,0.0000,78.7504},
{3374,-2032.789185,-214.961533,35.118515,0.0000,0.0000,22.5004}
};

static dmid; // this will hold the id this dm is assigned at init

forward dm_sfdrivingschool_init();

public dm_sfdrivingschool_init() // unique named init function (must be ran on gamemode init)
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
 	

