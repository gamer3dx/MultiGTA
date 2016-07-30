//Dirtbike Stadium module.
//Created by MadCat

#include "base"
#include "utils\dutils"
#include "account"

new bool:InDirtbikeStadium[MAX_PLAYERS];

#define STADIUMS 3
#define DirtbikeStadiumExit -1496.1333,1653.9264,1052.5313

new StadiumsCP[STADIUMS][CoordInfo] = {
	{2695.3608,-1704.3518,11.8438}, // LS
	{-2111.2969,-444.1540,38.7344}, // SF
	{1099.5817,1601.6479,12.5469} //LV
};

enum DirtbikeInfo {
	Float:X,
	Float:Y,
	Float:Z,
	Float:R
}

new StadiumBikes[][DirtbikeInfo] = {
	{-1467.912109,1642.409667,1052.200317,169.967224},
	{-1482.207031,1640.650634,1052.531250,270.000000},
	{-1488.300292,1637.464233,1052.531250,180.000000},
	{-1485.289916,1637.770263,1052.531250,180.000000},
	{-1488.332519,1625.787719,1052.531250,180.000000},
	{-1485.593872,1625.930908,1052.531250,180.000000}
};

forward Stadium_OnGameModeInit();
forward IsPlayerAtStadiumEntrance(playerid);
forward IsPlayerAtDirtbikeStadiumExit(playerid);
forward Stadium_OnPlayerEnterCheckpoint(playerid);
forward Stadium_OnPlayerCommandText(playerid,text[]);

public Stadium_OnGameModeInit()
{
	for (new id=0;id<STADIUMS;id++)
	{
		AddActiveCheckpoint(StadiumsCP[id][Coord_X],StadiumsCP[id][Coord_Y],StadiumsCP[id][Coord_Z],40,2);
	}
	CreateStreamMapIcon(33,-2111.2969,-444.1540,38.7344); //SF stadium not used ingame and not marked on map
	new bikeid;
	for(new vid=0; vid<sizeof(StadiumBikes); vid++) {
		bikeid = CreateVehicle(468,StadiumBikes[vid][X],StadiumBikes[vid][Y],StadiumBikes[vid][Z],StadiumBikes[vid][R],-1,-1,9999);
		LinkVehicleToInterior(bikeid,INTERIOR_DIRTBIKE);		
	}
	AddActiveCheckpoint(DirtbikeStadiumExit,200,2);
	SetInteriorWeapons(INTERIOR_DIRTBIKE,false);
	WriteLog("Stadium Loaded");
	return 0;
}

public IsPlayerAtStadiumEntrance(playerid)
{
	for (new id=0;id<STADIUMS;id++)
	{
		if (oIsPlayerInCheckpoint(playerid,StadiumsCP[id][Coord_X],StadiumsCP[id][Coord_Y],StadiumsCP[id][Coord_Z],10))
		{
			return 1;
		}
	}
	return 0;
}

public IsPlayerAtDirtbikeStadiumExit(playerid)
{
	if (oIsPlayerInCheckpoint(playerid,DirtbikeStadiumExit,10))
	{
		return 1;
	}
	return 0;
}

public Stadium_OnPlayerEnterCheckpoint(playerid)
{
	if (playerid == INVALID_PLAYER_ID) return 1;
	if (!IsPlayerRegistered(playerid)) return 0;
	if (PlayerQuest[playerid] != 0) return 0;
	if (IsPlayerAtStadiumEntrance(playerid)){
		SystemMsg(playerid,COLOUR_INFO,gettext(1611));
	} else if (IsPlayerAtDirtbikeStadiumExit(playerid)){
		if (IsPlayerInAnyVehicle(playerid)){
			SystemMsg(playerid,COLOUR_ERROR,gettext(1386));
		} else {
			oDisablePlayerCheckpoint(playerid);
			SetPlayerInterior(playerid,0);
			oSetPlayerPos(playerid,WhereHeEnter[playerid][Coord_X],WhereHeEnter[playerid][Coord_Y],WhereHeEnter[playerid][Coord_Z]);
			WhereHeEnter[playerid][Coord_X] = WhereHeEnter[playerid][Coord_Y] = WhereHeEnter[playerid][Coord_Z] = 0.0;
			InDirtbikeStadium[playerid] = false;
		}
	}
	return 0;
}

public Stadium_OnPlayerCommandText(playerid,text[])
{
	if (!IsPlayerRegistered(playerid)) return 0;
	dcmd(dirtbike,8,text);
	return 0;
}

dcmd_dirtbike(playerid,params[])
{
	#pragma unused params
	if (IsPlayerAtStadiumEntrance(playerid)){
		if (IsPlayerInAnyVehicle(playerid)){
			SystemMsg(playerid,COLOUR_ERROR,gettext(1386));
		} else {
			if (Player[playerid][CanUseMotoBikes] == 1){
				oDisablePlayerCheckpoint(playerid);
				WhereHeEnter[playerid][Coord_X]=PlayerPos[playerid][Coord_X]; 
				WhereHeEnter[playerid][Coord_Y]=PlayerPos[playerid][Coord_Y]; 
				WhereHeEnter[playerid][Coord_Z]=PlayerPos[playerid][Coord_Z];
				SetPlayerInterior(playerid,INTERIOR_DIRTBIKE);
				oSetPlayerPos(playerid,-1488.1526,1643.3965,1060.6719);
				InDirtbikeStadium[playerid] = true;
				SystemMsg(playerid,COLOUR_INFO,gettext(1612));
			} else {
				SystemMsg(playerid,COLOUR_ERROR,gettext(1621));
			}
		}
	} else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(157));
	}
	return 1;
}