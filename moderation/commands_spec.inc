//Spectate Moderator Commands Module
//Converted by MadCat
//Based on part of code from Xtreme Admin System 2.2

#include "base"

enum SpectateData {
	bool:Spectating,                    
	SpectateID                          
}

new Spec[MAX_PLAYERS][SpectateData];

forward Spec_OnPlayerCommandText(playerid,text[]);
forward Spec_OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid);
forward Spec_OnPlayerExitVehicle(playerid, vehicleid);
forward Spec_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger);
forward Spec_OnPlayerDisconnect(playerid,reason);

public Spec_OnPlayerCommandText(playerid,text[])
{
	if (!IsPlayerConnected(playerid)) return 0;

	if (PlayerPriv[playerid][IsModerator] == 0) return 0;
	if (PlayerPriv[playerid][CanSpectate] == 0) return 0;

	dcmd(spec,4,text);
	
	return 0;
}

dcmd_spec(playerid,params[]) {
	    	if(isempty(params)){
			SystemMsg(playerid,COLOUR_ERROR,gettext(1326));
			return 1;
		}

	    	if(strcomp(params,"off",true) == 1) {
		        if(!Spec[playerid][Spectating]){
				SystemMsg(playerid,COLOUR_ERROR,gettext(1327));
				return 1;
			}
		        TogglePlayerSpectating(playerid,false);
			SetPlayerVirtualWorld(playerid,WORLD_DEFAULT);
		        Spec[playerid][Spectating] = false;
		        SystemMsg(playerid,COLOUR_OK,gettext(1560));
			return 1;
		}

		new specplayerid = FindPlayerID(params);
		if (specplayerid==-1) return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1571),params);
		if (specplayerid==-2) return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1572),params);

		if(IsPlayerConnected(specplayerid) && specplayerid != INVALID_PLAYER_ID && specplayerid != playerid) {
		    	if(Spec[specplayerid][Spectating]){
				SystemMsg(playerid,COLOUR_ERROR,gettext(1561));
				return 1;
			}

	       		if(Spec[playerid][Spectating] && Spec[playerid][SpectateID] == specplayerid){
				SystemMsg(playerid,COLOUR_ERROR,gettext(1562));
				return 1;
			}

			Spec[playerid][Spectating] = true, Spec[playerid][SpectateID] = specplayerid;
	        	SetPlayerInterior(playerid,GetPlayerInterior(specplayerid));
			SetPlayerVirtualWorld(playerid,GetPlayerVirtualWorld(specplayerid));
	        	TogglePlayerSpectating(playerid,true);
			if(!IsPlayerInAnyVehicle(specplayerid)){
				PlayerSpectatePlayer(playerid,specplayerid);
			} else {
				PlayerSpectateVehicle(playerid,GetPlayerVehicleID(specplayerid));
			}
			SystemMsgFormatStr(playerid,COLOUR_OK,gettext(1563),oGetPlayerName(specplayerid));
		} else {
			SystemMsg(playerid,COLOUR_ERROR,gettext(1564));
		}
		return 1;
}

public Spec_OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	for(new i = 0; i < MAX_PLAYERS_EX; i++){
		if(IsPlayerConnected(i) && Spec[i][SpectateID] == playerid && Spec[i][Spectating]){ 
			SetPlayerInterior(i,newinteriorid);
		}
	}
	return 0;
}

public Spec_OnPlayerExitVehicle(playerid, vehicleid)
{
	#pragma unused vehicleid
	for(new i = 0; i < MAX_PLAYERS_EX; i++){
		if(IsPlayerConnected(i) && Spec[i][SpectateID] == playerid  && Spec[i][Spectating])
		{ 
			TogglePlayerSpectating(i,false); 
			SetPlayerInterior(i,GetPlayerInterior(playerid));
			SetPlayerVirtualWorld(i,GetPlayerVirtualWorld(playerid)); 
			TogglePlayerSpectating(i,true); 
			PlayerSpectatePlayer(i,playerid); 
		}
	}
	return 0;
}

public Spec_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	#pragma unused ispassenger
	for(new i = 0; i < MAX_PLAYERS_EX; i++){
		if(IsPlayerConnected(i) && Spec[i][SpectateID] == playerid && Spec[i][Spectating])
		{ 
			TogglePlayerSpectating(i,false); 
			SetPlayerInterior(i,GetPlayerInterior(playerid));
			SetPlayerVirtualWorld(i,GetPlayerVirtualWorld(playerid));
			TogglePlayerSpectating(i,true); 
			PlayerSpectateVehicle(i,vehicleid);
		}	
	}
	return 0;
}

public Spec_OnPlayerDisconnect(playerid,reason)
{
	#pragma unused reason
	for(new i = 0; i < MAX_PLAYERS_EX; i++){
		if(IsPlayerConnected(i) && Spec[i][SpectateID] == playerid && Spec[i][Spectating]){ 
			TogglePlayerSpectating(i,false); 
			Spec[i][Spectating] = false, 
			Spec[i][SpectateID] = INVALID_PLAYER_ID;
		}
	}
	return 0;
}