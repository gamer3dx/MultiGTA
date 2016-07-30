//Anti-bike-fall module
//Created by tAxI
//Converted by MadCat

forward AntiFall_OnPlayerKeyStateChange(playerid,newkeys,oldkeys);
forward AntiFall_OnPlayerEnterVehicle(playerid,vehicleid);
forward AntiFall_OnPlayerStateChange(playerid,newstate,oldstate);

#define KEY_EXIT_VEHICLE	16

new bool:antifall[MAX_PLAYERS];
new bool:cmdoff[MAX_PLAYERS];
new lastv[MAX_PLAYERS];


stock AntiFall_OnPlayerEnterVehicle(playerid,vehicleid)
{
	lastv[playerid] = vehicleid;
	return 0;
}

stock AntiFall_OnPlayerStateChange(playerid,newstate,oldstate)
{
	if (oldstate == PLAYER_STATE_DRIVER) {
 		if(antifall[playerid] == true && newstate == PLAYER_STATE_ONFOOT) {
    			if(cmdoff[playerid] == true) {
     				cmdoff[playerid] = false;
     			}
      			else 
			{
       				oPutPlayerInVehicle(playerid,lastv[playerid],0);
			}
       		}
    	}
    	if (oldstate == PLAYER_STATE_PASSENGER) {
 		if(antifall[playerid] == true && newstate == PLAYER_STATE_ONFOOT) {
    			if(cmdoff[playerid] == true) {
     				cmdoff[playerid] = false;
     			}
      			else 
			{
       				oPutPlayerInVehicle(playerid,lastv[playerid],1);
			}
       		}
    	}
	return 0;
}

stock AntiFall_OnPlayerKeyStateChange(playerid,newkeys, oldkeys)
{
	#pragma unused oldkeys
	if(newkeys == KEY_EXIT_VEHICLE) {
		cmdoff[playerid] = true;
	}
	return 1;
}
