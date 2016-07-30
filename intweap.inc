//Interior weapons module
//Created by Elyks
//Converted for MultiGTA by MadCat

#include "player"

forward IntWeap_OnGameModeInit();
forward IntWeap_OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid);
forward IntWeap_OnPlayerConnect(playerid);

#define MAX_INTERIORS 30

new bool:WeaponsAllowedInInterior[MAX_INTERIORS];
new PlayerInteriorWeapon[MAX_PLAYERS][PLAYER_WEAPON_SLOTS];
new PlayerInteriorAmmo[MAX_PLAYERS][PLAYER_WEAPON_SLOTS];

public IntWeap_OnPlayerConnect(playerid){
	for(new w=0;w<PLAYER_WEAPON_SLOTS;w++){
	    	PlayerInteriorWeapon[playerid][w] = 0;
	    	PlayerInteriorAmmo[playerid][w] = 0;
	}
}

public IntWeap_OnGameModeInit(){
    	SetInteriorWeapons(INTERIOR_DEFAULT,true);
    	AllowInteriorWeapons(true);
	WriteLog("Interior weapons system loaded");
}

public IntWeap_OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid){
	if((WeaponsAllowedInInterior[newinteriorid] == true) && (WeaponsAllowedInInterior[oldinteriorid] == false)){
		for(new i;i<PLAYER_WEAPON_SLOTS;i++){
			GivePlayerWeapon(playerid,PlayerInteriorWeapon[playerid][i],PlayerInteriorAmmo[playerid][i]);
		}
	}
	
	if((WeaponsAllowedInInterior[newinteriorid] == false) && (WeaponsAllowedInInterior[oldinteriorid] == true)){
		for(new i;i<PLAYER_WEAPON_SLOTS;i++){
		       	GetPlayerWeaponData(playerid,i,PlayerInteriorWeapon[playerid][i],PlayerInteriorAmmo[playerid][i]);
		}
	    	ResetPlayerWeapons(playerid);
	}

	return 0;
}

stock SetInteriorWeapons(interiorid,bool:toggle){
    	WeaponsAllowedInInterior[interiorid] = toggle;
}