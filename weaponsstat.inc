//Weapons stat module
//Created by MadCat
//Part of code from GTAT

#define WEAPONS_FOR_STAT 55

new STATS_WeaponKills[MAX_PLAYERS][WEAPONS_FOR_STAT];
new STATS_WeaponDeaths[MAX_PLAYERS][WEAPONS_FOR_STAT];

forward WeaponsStat_OnPlayerDeath(playerid,killerid,reason);
forward WeaponsStat_OnPlayerConnect(playerid);
forward WeaponsStat_OnPlayerDisconnect(playerid,reason);

public WeaponsStat_OnPlayerDeath(playerid,killerid,reason)
{
	if (killerid != INVALID_PLAYER_ID){
	    	if(reason >= 0 && reason < WEAPONS_FOR_STAT){
			STATS_WeaponKills[killerid][reason]++;
		}
	}
    	if(reason >= 0 && reason < WEAPONS_FOR_STAT){
		STATS_WeaponDeaths[playerid][reason]++;
	}
	return 0;
}

public WeaponsStat_OnPlayerConnect(playerid)
{
	for(new i=0; i<WEAPONS_FOR_STAT; i++){
		STATS_WeaponKills[playerid][i] = 0;
	}
	for(new i=0; i<WEAPONS_FOR_STAT; i++){
		STATS_WeaponDeaths[playerid][i] = 0;
	}
	return 0;
}

public WeaponsStat_OnPlayerDisconnect(playerid,reason)
{
	#pragma unused reason
	for(new i=0; i<WEAPONS_FOR_STAT; i++){
		STATS_WeaponKills[playerid][i] = 0;
	}
	for(new i=0; i<WEAPONS_FOR_STAT; i++){
		STATS_WeaponDeaths[playerid][i] = 0;
	}
	return 0;
}

WeaponsStat_OnPlayerCommandText(playerid,text[])
{
	if (!IsPlayerRegistered(playerid)) return 0;
	dcmd(weaponstat,10,text);
	return 0;
}

dcmd_weaponstat(playerid,params[]){
	new string[MAX_STRING];
	new name[MAX_NAME];
	if (strcomp(params,"kills",true) == 1){
		SystemMsg(playerid,COLOUR_INFO,gettext(377));
		for (new weaponid=0;weaponid<WeaponsSize;weaponid++)
		{
			if (weaponid == 19 || weaponid == 20 || weaponid == 21) continue; //No weapons data
			if (strlen(string) < 120){
				set(name,nullstr);
				set(name,oGetWeaponName(weaponid));
				strcat(string,name);
				strcat(string,": ");
				strcat(string,ovalstr(STATS_WeaponKills[playerid][weaponid]));
				strcat(string," - ");
			} else {
				SystemMsg(playerid,COLOUR_INFO,string);
				set(string,nullstr);
			}
		}
		format(string,sizeof(string),gettext(378),STATS_WeaponDeaths[playerid][WEAPON_VEHICLE],STATS_WeaponDeaths[playerid][WEAPON_DROWN],STATS_WeaponDeaths[playerid][WEAPON_COLLISION]);
		SystemMsg(playerid,COLOUR_INFO,string);
	} else if (strcomp(params,"deaths",true) == 1){
		SystemMsg(playerid,COLOUR_INFO,gettext(966));
		for (new weaponid=0;weaponid<WeaponsSize;weaponid++)
		{
			if (weaponid == 19 || weaponid == 20 || weaponid == 21) continue; //No weapons data
			if (strlen(string) < 120){
				set(name,nullstr);
				set(name,oGetWeaponName(weaponid));
				strcat(string,name);
				strcat(string,": ");
				strcat(string,ovalstr(STATS_WeaponDeaths[playerid][weaponid]));
				strcat(string," - ");
			} else {
				SystemMsg(playerid,COLOUR_INFO,string);
				set(string,nullstr);
			}
		}
		format(string,sizeof(string),gettext(378),STATS_WeaponDeaths[playerid][WEAPON_VEHICLE],STATS_WeaponDeaths[playerid][WEAPON_DROWN],STATS_WeaponDeaths[playerid][WEAPON_COLLISION]);
		SystemMsg(playerid,COLOUR_INFO,string);
	} else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(967));
	}
	
}

stock GetFavoriteWeapon(playerid){
	new current_fav;
	new current_fav_kills = STATS_WeaponKills[playerid][current_fav];
    	for(new i=0; i<WEAPONS_FOR_STAT; i++){
        	if(STATS_WeaponKills[playerid][i] > current_fav_kills){
        		current_fav = i;
            		current_fav_kills = STATS_WeaponKills[playerid][current_fav];
        	}
    	}
	return current_fav;
}

stock GetFavoriteDeathReason(playerid){
	new current_fav;
	new current_fav_deaths = STATS_WeaponDeaths[playerid][current_fav];
    	for(new i=0; i<WEAPONS_FOR_STAT; i++){
        	if(STATS_WeaponDeaths[playerid][i] > current_fav_deaths){
        		current_fav = i;
            		current_fav_deaths = STATS_WeaponDeaths[playerid][current_fav];
        	}
    	}
	return current_fav;
}

stock SetWeaponsKillStatFromDBString(playerid,dbstring[])
{
	new idx;
	for (new i=0;i<WEAPONS_FOR_STAT;i++)
	{
		STATS_WeaponKills[playerid][i] = strval(strcharsplit(dbstring,idx,strchar("|")));
	}
}

stock CreateWeaponKillStatDBString(playerid)
{
	new wepstr[MAX_STRING];
	for (new i=0;i<WEAPONS_FOR_STAT;i++)
	{
		new temp[MAX_STRING];
		valstr(temp,STATS_WeaponKills[playerid][i]);
		strins(wepstr, temp, strlen(wepstr));
		wepstr[strlen(wepstr)]=strchar("|");
	}
	return wepstr;
}

stock SetWeaponsDeathStatFromDBString(playerid,dbstring[])
{
	new idx;
	for (new i=0;i<WEAPONS_FOR_STAT;i++)
	{
		STATS_WeaponDeaths[playerid][i] = strval(strcharsplit(dbstring,idx,strchar("|")));
	}
}

stock CreateWeaponDeathStatDBString(playerid)
{
	new wepstr[MAX_STRING];
	for (new i=0;i<WEAPONS_FOR_STAT;i++)
	{
		new temp[MAX_STRING];
		valstr(temp,STATS_WeaponDeaths[playerid][i]);
		strins(wepstr, temp, strlen(wepstr));
		wepstr[strlen(wepstr)]=strchar("|");
	}
	return wepstr;
}
