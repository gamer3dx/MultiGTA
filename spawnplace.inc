//Spawn place selection module
//Created by MadCat
//Idea by BetaMaster

#define SPAWN_RANDOM 0
#define SPAWN_JOB 1
#define SPAWN_HOUSE_OWNER 2
#define SPAWN_HOUSE_RENTER 3

forward SpawnPlace_OnPlayerCommandText(playerid,text[]);

public SpawnPlace_OnPlayerCommandText(playerid,text[])
{
	dcmd(spawnset,8,text);
	return 0;
}

dcmd_spawnset(playerid,params[])
{
	if (strcomp("job",params,true) == 1){
		if (Player[playerid][GotJob] == JOB_NOJOB){
			SystemMsg(playerid,COLOUR_ERROR,gettext(361));
			return 1;
		}
		if (Player[playerid][GotJob] == JOB_TAXIDRIVER){
			SystemMsg(playerid,COLOUR_ERROR,gettext(366));
			return 1;
		}
		Player[playerid][SpawnPlace] = SPAWN_JOB;
		SystemMsg(playerid,COLOUR_OK,gettext(367));
		return 1;
	}
	if (strcomp("house",params,true) == 1){
		new house = GetPlayerHouse(playerid);
		if (house == -1){
			SystemMsg(playerid,COLOUR_ERROR,gettext(1096));
			return 1;
		}
		Player[playerid][SpawnPlace] = SPAWN_HOUSE_OWNER;
		SystemMsg(playerid,COLOUR_OK,gettext(369));
		return 1;
	}
	if (strcomp("rent",params,true) == 1){
		new rent = GetPlayerRent(playerid);
		if (rent == -1){
			SystemMsg(playerid,COLOUR_ERROR,gettext(1101));
			return 1;
		}
		Player[playerid][SpawnPlace] = SPAWN_HOUSE_OWNER;
		SystemMsg(playerid,COLOUR_OK,gettext(370));
		return 1;
	}
	if (strcomp("random",params,true) == 1){
		Player[playerid][SpawnPlace] = SPAWN_RANDOM;
		SystemMsg(playerid,COLOUR_OK,gettext(374));
		return 1;
	}
	SystemMsg(playerid,COLOUR_ERROR,gettext(359));
	return 1;
}