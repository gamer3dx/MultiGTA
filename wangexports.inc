//Wang Exports module.
//Created by BetaMaster
//Converted by MadCat

#include "base"

forward Wang_Init();
forward WangExportsPickupCheck(playerid);
forward WangExport_OnGameModeInit();
forward WangExport_Update();
forward Wang_OnPlayerEnterCheckpoint(playerid);
forward WangExport_SellVehicle(playerid);

new Float:WangPoint_X = -1957.5327;
new Float:WangPoint_Y = 300.2131;
new Float:WangPoint_Z = 35.4688;

new WANG_WantedVehicle;

new TimerWang;

public WangExport_OnGameModeInit(){
	Wang_Init();
	WangExport_Update();
	TimerWang = SetTimer("WangExport_Update",ONE_SA_DAY,1);
	WriteLog("Wang Export Loaded");
	return 0;
}

public Wang_Init(){
	WANG_WantedVehicle = GetVehicleModel(random(ActiveVehiclesCount));
	while (IsVehicleBoat(WANG_WantedVehicle) || CanVehicleFly(WANG_WantedVehicle)) WANG_WantedVehicle = GetVehicleModel(random(ActiveVehiclesCount));
	while (IsVehicleModelCloseToWang(WANG_WantedVehicle)) WANG_WantedVehicle = GetVehicleModel(random(ActiveVehiclesCount));
	AddActiveCheckpoint(WangPoint_X,WangPoint_Y,WangPoint_Z,25,10);
}

public WangExport_Update() {
	WANG_WantedVehicle = GetVehicleModel(random(ActiveVehiclesCount));
	while (IsVehicleBoat(WANG_WantedVehicle) || CanVehicleFly(WANG_WantedVehicle)) WANG_WantedVehicle = GetVehicleModel(random(ActiveVehiclesCount));
	while (IsVehicleModelCloseToWang(WANG_WantedVehicle)) WANG_WantedVehicle = GetVehicleModel(random(ActiveVehiclesCount));
	new string[MAX_STRING];
	format(string, MAX_STRING,gettext(4), GetVehicleName(WANG_WantedVehicle));
	SystemMsgToRegistered(COLOUR_INFO,string);
}

public Wang_OnPlayerEnterCheckpoint(playerid)
{
	if (oIsPlayerInCheckpoint(playerid,WangPoint_X,WangPoint_Y,WangPoint_Z,25)){
		WangExport_SellVehicle(playerid);
	}
}

//------------------------------------------------------------------------------------------------------

public WangExport_SellVehicle(playerid) {
	if(GetPlayerState(playerid)!=PLAYER_STATE_DRIVER) return;
	new string[MAX_STRING];
	new vid = GetPlayerVehicleID(playerid);
	if(GetVehicleModel(vid) == WANG_WantedVehicle){
		new Float:vhp;
		GetVehicleHealth(vid,vhp);
		new payment = GetVehicleCost(WANG_WantedVehicle) + trunc(10*vhp);
		format(string, MAX_STRING,gettext(5),GetVehicleName(WANG_WantedVehicle),payment);
		SystemMsg(playerid,COLOUR_OK,string);
		oGivePlayerMoney(playerid, payment,0);
		SetVehicleToRespawn(vid);
		WangExport_Update();
	} else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(6));
		format(string,sizeof(string),gettext(4),GetVehicleName(WANG_WantedVehicle));
		SystemMsg(playerid,COLOUR_INFO,string);
	}
}

IsVehicleModelCloseToWang(model){
	for (new vehicleid = 0; vehicleid < ActiveVehiclesCount; vehicleid++)
	{
		if (GetVehicleModel(vehicleid) == model){
			new Float:x,Float:y,Float:z;
			new dis;
			GetVehiclePos(vehicleid,x,y,z);
			dis = floatround(floatsqroot((WangPoint_X-x)*(WangPoint_X-x)+(WangPoint_Y-y)*(WangPoint_Y-y)+(WangPoint_Z-z)*(WangPoint_Z-z)));
			if (dis < 500) return 1;
		}
	}
	return 0;
}
