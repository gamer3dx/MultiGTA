//Houses Creation Commands Module
//Created by MadCat

#include "base"
#include "housing"

new BuildHouse[MAX_PLAYERS][HouseInfo];
new BuildHouseStep[MAX_PLAYERS];

new Houses_Already_Build = 0;

forward ModHB_OnGameModeInit();
forward ModHB_OnPlayerCommandText(playerid,text[]);
forward ModHB_OnPlayerDisconnect(playerid,reason);

public ModHB_OnGameModeInit()
{
	HBConfig();
	WriteLog("Houses build system loaded");
	return 0;
}

HBConfig()
{
	
	new temp[MAX_STRING];

	if (!db_Exists(ConfigDB)) db_Create(ConfigDB);

	set(temp,db_Get(ConfigDB,"Houses_Already_Build"));
	if (strlen(temp) > 0) Houses_Already_Build = strval(temp);
	else { valstr(temp,Houses_Already_Build); db_Set(ConfigDB,"Houses_Already_Build",temp);  }
	set(temp,nullstr);

	return;
}

public ModHB_OnPlayerCommandText(playerid,text[])
{
	if (!IsPlayerConnected(playerid)) return 0;
	if (PlayerPriv[playerid][IsModerator] == 0) return 0;
	if (PlayerPriv[playerid][CanBuildHouses] == 0) return 0;
	dcmd(buildhouse,10,text);
	dcmd(cancelbuild,11,text);
	return 0;
}

public ModHB_OnPlayerDisconnect(playerid,reason)
{
	#pragma unused reason
	if(BuildHouseStep[playerid] != 0) ResetBuildHouse(playerid);
	return 0;
}

dcmd_buildhouse(playerid,params[])
{
	new string[MAX_STRING];
	if(BuildHouseStep[playerid] == 0) //Start
	{
		SystemMsg(playerid,COLOUR_INFO,gettext(1658));
		SystemMsg(playerid,COLOUR_INFO,gettext(1657));
		BuildHouseStep[playerid] = 1;
		Houses_Already_Build++;
	}
	else if(BuildHouseStep[playerid] == 1) //Entrance
	{
		SystemMsg(playerid,COLOUR_INFO,gettext(1659));
		SystemMsg(playerid,COLOUR_INFO,gettext(1660));
		SystemMsg(playerid,COLOUR_INFO,gettext(1657));
		BuildHouse[playerid][Entrance_X] = PlayerPos[playerid][Coord_X];
		BuildHouse[playerid][Entrance_X] = PlayerPos[playerid][Coord_Y];
		BuildHouse[playerid][Entrance_X] = PlayerPos[playerid][Coord_Z];
	    	BuildHouseStep[playerid] = 2;
	}
	else if(BuildHouseStep[playerid] == 2) //Exit
	{
		new int = GetPlayerInterior(playerid);
		if(int == 0) return SystemMsg(playerid,COLOUR_ERROR,gettext(1661));
		BuildHouse[playerid][InteriorID] = int;
		BuildHouse[playerid][Exit_X] = PlayerPos[playerid][Coord_X];
		BuildHouse[playerid][Exit_X] = PlayerPos[playerid][Coord_Y];
		BuildHouse[playerid][Exit_X] = PlayerPos[playerid][Coord_Z];
		SystemMsg(playerid,COLOUR_INFO,gettext(1662));
  		SystemMsg(playerid,COLOUR_INFO,gettext(1663));
		SystemMsg(playerid,COLOUR_INFO,gettext(1657));
		BuildHouseStep[playerid] = 3;
	}
    	else if(BuildHouseStep[playerid] == 3) //Description
	{
		new bool:okdesc = false;
    		for (new multi=0;multi<MAX_HOUSES;multi++)
		{
			if (strcomp(params,Houses[multi][Houses_Description],true) == 1){
				okdesc = true;
				break;
			}
		}
		if (!okdesc) return SystemMsg(playerid,COLOUR_ERROR,gettext(1664));
		set(BuildHouse[playerid][Houses_Description],params);
     		format(string,sizeof(string), gettext(1665), params);
		SystemMsg(playerid,COLOUR_INFO,string);
  		SystemMsg(playerid,COLOUR_INFO,gettext(1666));
		SystemMsg(playerid,COLOUR_INFO,gettext(1657));
		BuildHouseStep[playerid] = 4;
	}
	else if(BuildHouseStep[playerid] == 4)
	{
		SaveHouse(playerid);
	}
	return 1;
}

dcmd_cancelbuild(playerid,params[])
{
	#pragma unused params
	if(BuildHouseStep[playerid] == 0) return SystemMsg(playerid,COLOUR_ERROR,gettext(1667));
	ResetBuildHouse(playerid);
	Houses_Already_Build--;
	return SystemMsg(playerid,COLOUR_OK,gettext(1668));
}

ResetBuildHouse(playerid)
{
	BuildHouseStep[playerid] = 0;
	set(BuildHouse[playerid][Houses_UID],nullstr);
	BuildHouse[playerid][Entrance_X] = 0.0;
	BuildHouse[playerid][Entrance_Y] = 0.0;
	BuildHouse[playerid][Entrance_Z] = 0.0;
	BuildHouse[playerid][Exit_X] = 0.0;
	BuildHouse[playerid][Exit_Y] = 0.0;
	BuildHouse[playerid][Exit_Z] = 0.0;
	BuildHouse[playerid][InteriorID] = 0;
	BuildHouse[playerid][Houses_Cost] = 0;
	set(BuildHouse[playerid][Houses_Description],nullstr);
	return 1;
}

SaveHouse(playerid)
{
	new File:hFile = fopen("NewMultiGTAHouses.txt", io_append);
	if (hFile){
		new filestring[1024];
	        format(filestring, sizeof(filestring), "	{'House%d',%f,%f,%f,%f,%f,%f,%d,%d,'Unknown',0,'%s',0,%d,'Unknown',0,0,%s,%s,%s}, //Created by %s\r\n",
		Houses_Already_Build + MAX_HOUSES,
		BuildHouse[playerid][Entrance_X],
		BuildHouse[playerid][Entrance_Y],
		BuildHouse[playerid][Entrance_Z],
		BuildHouse[playerid][Exit_X],
		BuildHouse[playerid][Exit_Y],
		BuildHouse[playerid][Exit_Z],
		BuildHouse[playerid][InteriorID],
		SetHouseCostByDesc(BuildHouse[playerid][Houses_Description]),
		BuildHouse[playerid][Houses_Description],
		SetHouseRentCostByDesc(BuildHouse[playerid][Houses_Description]),
		WriteWeaponByID(SetHouseWeapon1ByDesc(BuildHouse[playerid][Houses_Description])),
		WriteWeaponByID(SetHouseWeapon2ByDesc(BuildHouse[playerid][Houses_Description])),
		WriteWeaponByID(SetHouseWeapon3ByDesc(BuildHouse[playerid][Houses_Description])),
		oGetPlayerName(playerid)
		);
		fwrite(hFile, filestring);
		fclose(hFile);
        }
	BuildHouseStep[playerid] = 0;
	return SystemMsg(playerid,COLOUR_OK,gettext(1669));
}

SetHouseCostByDesc(desc[]){
    	for (new multi=0;multi<MAX_HOUSES;multi++)
	{
		if (strcmp(desc,Houses[multi][Houses_Description],false) == 0){
			return Houses[multi][Houses_Cost];
		}
	}
	return -999999;
}

SetHouseRentCostByDesc(desc[]){
    	for (new multi=0;multi<MAX_HOUSES;multi++)
	{
		if (strcomp(desc,Houses[multi][Houses_Description],false) == 1){
			return Houses[multi][Houses_RentCost];
		}
	}
	return -999999;
}

SetHouseWeapon1ByDesc(desc[]){
    	for (new multi=0;multi<MAX_HOUSES;multi++)
	{
		if (strcomp(desc,Houses[multi][Houses_Description],false) == 1){
			return Houses[multi][Weapon1];
		}
	}
	return -1;
}

SetHouseWeapon2ByDesc(desc[]){
    	for (new multi=0;multi<MAX_HOUSES;multi++)
	{
		if (strcomp(desc,Houses[multi][Houses_Description],false) == 1){
			return Houses[multi][Weapon2];
		}
	}
	return -1;
}

SetHouseWeapon3ByDesc(desc[]){
    	for (new multi=0;multi<MAX_HOUSES;multi++)
	{
		if (strcomp(desc,Houses[multi][Houses_Description],false) == 1){
			return Houses[multi][Weapon3];
		}
	}
	return -1;
}

WriteWeaponByID(id){
	new name[MAX_PLAYER_NAME];
	switch (id)
	{
		case 0: set(name,"0");
		case 1: set(name,"WEAPON_BRASSKNUCKLE");
		case 2: set(name,"WEAPON_GOLFCLUB");
		case 3: set(name,"WEAPON_NITESTICK");
		case 4: set(name,"WEAPON_KNIFE");
		case 5: set(name,"WEAPON_BAT");
		case 6: set(name,"WEAPON_SHOVEL");
		case 7: set(name,"WEAPON_POOLSTICK");
		case 8: set(name,"WEAPON_KATANA");
		case 9: set(name,"WEAPON_CHAINSAW");
		case 10: set(name,"WEAPON_DILDO");
		case 11: set(name,"WEAPON_DILDO2");
		case 12: set(name,"WEAPON_VIBRATOR");
		case 13: set(name,"WEAPON_VIBRATOR2");
		case 14: set(name,"WEAPON_FLOWER");
		case 15: set(name,"WEAPON_CANE");
		case 16: set(name,"WEAPON_GRENADE");
		case 17: set(name,"WEAPON_TEARGAS");
		case 18: set(name,"WEAPON_MOLTOV");
		case 19: set(name,"19");
		case 20: set(name,"20");
		case 21: set(name,"WEAPON_MOLTOV");
		case 22: set(name,"WEAPON_COLT45");
		case 23: set(name,"WEAPON_SILENCED");
		case 24: set(name,"WEAPON_DEAGLE");
		case 25: set(name,"WEAPON_SHOTGUN");
		case 26: set(name,"WEAPON_SAWEDOFF");
		case 27: set(name,"WEAPON_SHOTGSPA");
		case 28: set(name,"WEAPON_UZI");
		case 29: set(name,"WEAPON_MP5");
		case 30: set(name,"WEAPON_AK47");
		case 31: set(name,"WEAPON_M4");
		case 32: set(name,"WEAPON_TEC9");
		case 33: set(name,"WEAPON_RIFLE");
		case 34: set(name,"WEAPON_SNIPER");
		case 35: set(name,"WEAPON_ROCKETLAUNCHER");
		case 36: set(name,"WEAPON_HEATSEEKER");
		case 37: set(name,"WEAPON_FLAMETHROWER");
		case 38: set(name,"WEAPON_MINIGUN");
		case 39: set(name,"WEAPON_SATCHEL");
		case 40: set(name,"WEAPON_BOMB");
		case 41: set(name,"WEAPON_SPRAYCAN");
		case 42: set(name,"WEAPON_FIREEXTINGUISHER");
		case 43: set(name,"WEAPON_CAMERA");
		case 44: set(name,"44");
		case 45: set(name,"45");
		case 46: set(name,"WEAPON_PARACHUTE");
	}
	return name;
}