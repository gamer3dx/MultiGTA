//School and licensed module
//Created by MadCat
//Idea from PEN1:LS and Godfather

#include "base"
#include "utils\dutils"
#include "account"

#define BikeSchoolCoords 1174.039306,1351.143920,10.921875
#define FlySchoolCoords 415.623138,2533.696289,19.148437
#define BoatSchoolCoords -2186.847167,2414.963623,5.156250

new MotoBikeSchoolCost=50000;
new FlySchoolCost=100000;
new BoatSchoolCost=25000;

forward School_OnGameModeInit();
forward School_OnPlayerStateChange(playerid,newstate,oldstate);
forward School_OnPlayerEnterCheckpoint(playerid);
forward School_OnPlayerCommandText(playerid,text[]);

public School_OnGameModeInit(){
	AddActiveCheckpoint(BikeSchoolCoords,75,3);
	AddActiveCheckpoint(FlySchoolCoords,75,3);
	AddActiveCheckpoint(BoatSchoolCoords,75,3);
	CreateStreamMapIcon(36,BikeSchoolCoords);
	CreateStreamMapIcon(36,FlySchoolCoords);
	CreateStreamMapIcon(36,BoatSchoolCoords);
	CreateDMapIcon(36,BikeSchoolCoords,gettext(1763),gettext(1764));
	CreateDMapIcon(36,FlySchoolCoords,gettext(1765),gettext(1766));
	CreateDMapIcon(36,BoatSchoolCoords,gettext(1767),gettext(1768));	
	WriteLog("Driving schools loaded!");
	return 0;
}

public School_OnPlayerStateChange(playerid,newstate,oldstate){
	if (oldstate == PLAYER_STATE_ONFOOT && newstate == PLAYER_STATE_DRIVER){
		new vehiclemodel = GetVehicleModel(GetPlayerVehicleID(playerid));
		if (IsVehicleMotoBike(vehiclemodel) && Player[playerid][CanUseMotoBikes] == 0) PlayerCantUseVehicle(playerid);
		if (CanVehicleFly(vehiclemodel) && Player[playerid][CanUseFlying] == 0) PlayerCantUseVehicle(playerid);
		if (IsVehicleBoat(vehiclemodel) && Player[playerid][CanUseBoats] == 0) PlayerCantUseVehicle(playerid);
	}
	return 0;
}

public School_OnPlayerEnterCheckpoint(playerid)
{
	if (playerid == INVALID_PLAYER_ID) return 1;
	if (oIsPlayerInCheckpoint(playerid,BikeSchoolCoords,10)){
		if (Player[playerid][CanUseMotoBikes] == 1){
			SystemMsg(playerid,COLOUR_ERROR,gettext(1362));
		} else {
			SystemMsg(playerid,COLOUR_INFO,gettext(1364));
			SystemMsgFormat(playerid,COLOUR_INFO,gettext(1363),MotoBikeSchoolCost);
		}
	} else if (oIsPlayerInCheckpoint(playerid,FlySchoolCoords,10)){
		if (Player[playerid][CanUseFlying] == 1){
			SystemMsg(playerid,COLOUR_ERROR,gettext(1360));
		} else {
			SystemMsg(playerid,COLOUR_INFO,gettext(1365));
			SystemMsgFormat(playerid,COLOUR_INFO,gettext(1363),FlySchoolCost);
		}
	} else if (oIsPlayerInCheckpoint(playerid,BoatSchoolCoords,10)){
		if (Player[playerid][CanUseBoats] == 1){
			SystemMsg(playerid,COLOUR_ERROR,gettext(1358));
		} else {
			SystemMsg(playerid,COLOUR_INFO,gettext(1366));
			SystemMsgFormat(playerid,COLOUR_INFO,gettext(1363),BoatSchoolCost);
		}
	}
	return 0;
}

public School_OnPlayerCommandText(playerid,text[]){
	if (!IsPlayerConnected(playerid)) return 0;
	dcmd(learn,5,text);
	return 0;
}

stock PlayerCantUseVehicle(playerid){
	SystemMsg(playerid,COLOUR_INFO,gettext(1355));
	RemovePlayerFromVehicle(playerid);
}

dcmd_learn(playerid,params[])
{
	#pragma unused params
	if (oIsPlayerInCheckpoint(playerid,BikeSchoolCoords,10)){
		if (Player[playerid][CanUseMotoBikes] == 1){
			SystemMsg(playerid,COLOUR_ERROR,gettext(1362));
		} else {
			if (oGetPlayerMoney(playerid) < MotoBikeSchoolCost){
				SystemMsg(playerid,COLOUR_ERROR,gettext(868));
			} else {
				oGivePlayerMoney(playerid,-MotoBikeSchoolCost,1);
				Player[playerid][CanUseMotoBikes] = 1;
				SystemMsg(playerid,COLOUR_OK,gettext(1361));
			}
		}
	} else if (oIsPlayerInCheckpoint(playerid,FlySchoolCoords,10)){
		if (Player[playerid][CanUseFlying] == 1){
			SystemMsg(playerid,COLOUR_ERROR,gettext(1360));
		} else {
			if (oGetPlayerMoney(playerid) < FlySchoolCost){
				SystemMsg(playerid,COLOUR_ERROR,gettext(868));
			} else {
				oGivePlayerMoney(playerid,-FlySchoolCost,1);
				Player[playerid][CanUseFlying] = 1;
				SystemMsg(playerid,COLOUR_OK,gettext(1359));
			}
		}
	} else if (oIsPlayerInCheckpoint(playerid,BoatSchoolCoords,10)){
		if (Player[playerid][CanUseBoats] == 1){
			SystemMsg(playerid,COLOUR_ERROR,gettext(1358));
		} else {
			if (oGetPlayerMoney(playerid) < BoatSchoolCost){
				SystemMsg(playerid,COLOUR_ERROR,gettext(868));
			} else {
				oGivePlayerMoney(playerid,-BoatSchoolCost,1);
				Player[playerid][CanUseBoats] = 1;
				SystemMsg(playerid,COLOUR_OK,gettext(1357));
			}
		}
	} else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(1356));
	}
	return 1;
}