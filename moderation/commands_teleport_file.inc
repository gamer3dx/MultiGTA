//Teleports from file
//Created by MadCat
//Part of code from PenLS

#include "base"

#define MAX_LOCS 50

new LocsAdded = 0;

enum pTele
{
	Float:pLocationsposx,
	Float:pLocationsposy,
	Float:pLocationsposz,
	pLocationsint,
	pLocationsName[MAX_NAME]
};

new TeleLocations[MAX_LOCS][pTele];

new TeleLocationsDB[MAX_STRING] = "MultiGTA/OtherDB/Locations.txt";

forward ModTP_OnGameModeInit();
forward ModTP_OnPlayerCommandText(playerid,text[]);
forward ModTP_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);

public ModTP_OnGameModeInit(){
	ModTP_Config();
	LoadLocations();
	return 0;
}

public ModTP_OnPlayerCommandText(playerid,text[])
{
	if (!IsPlayerConnected(playerid)) return 0;

	if (PlayerPriv[playerid][IsModerator] == 0) return 0;
	if (PlayerPriv[playerid][CanTeleportToFilePos] == 0) return 0;

	dcmd(gotoplace,9,text);
	return 0;
}

ModTP_Config()
{
	Debug("moderation/commands_teleport_file.inc > ModTP_Config - Start");
	
	if (!db_Exists(DatabaseDB)) db_Create(DatabaseDB);
	new temp[MAX_STRING];
	set(temp,db_Get(DatabaseDB,"TeleLocations_DB"));
	if (strlen(temp) > 0) set(TeleLocationsDB,temp); 
	else { set(temp,TeleLocationsDB); db_Set(DatabaseDB,"TeleLocations_DB",temp);  }
	set(temp,nullstr);
	if (!db_Exists(TeleLocationsDB)) db_Create(TeleLocationsDB);
	Debug("moderation/commands_teleport_file.inc > ModTP_Config - Stop");
	return;
}

LoadLocations()
{
	Debug("moderation/commands_teleport_file.inc > LoadLocations - Start");
	new arrCoords[5][MAX_STRING];
	new strFromFile2[MAX_STRING];
	new logstring[MAX_STRING];

	if (!db_Exists(TeleLocationsDB)){ db_Create(TeleLocationsDB); Debug("moderation/commands_teleport_file.inc > LoadLocations - Stop"); return 0;}
	
	new File: file = fopen(TeleLocationsDB, io_read);
	if (file)
	{
		while (fread(file,strFromFile2,MAX_STRING))
		{
			if (LocsAdded >= MAX_LOCS) continue;
			StripNewLine(strFromFile2);
			split(strFromFile2, arrCoords, ',');
			TeleLocations[LocsAdded][pLocationsposx] = floatstr(arrCoords[0]);
			TeleLocations[LocsAdded][pLocationsposy] = floatstr(arrCoords[1]);
			TeleLocations[LocsAdded][pLocationsposz] = floatstr(arrCoords[2]);
			TeleLocations[LocsAdded][pLocationsint] = strval(arrCoords[3]);
			set(TeleLocations[LocsAdded][pLocationsName],arrCoords[4]);
			LocsAdded++;
		}
		fclose(file);
		format(logstring,MAX_STRING,"%d teleport locations loaded from file!",LocsAdded);
	} else {
		format(logstring,MAX_STRING,"Warning: Can't open file with locations (%s)",TeleLocationsDB);
	}
	WriteLog(logstring);
	Debug("moderation/commands_teleport_file.inc > LoadLocations - Stop");
	return 1;
}

dcmd_gotoplace(playerid,params[])
{
	#pragma unused params
	if (Spec[playerid][Spectating] == true){
		ModMsg(playerid,gettext(1569));
		return 1;
	}
	if (LocsAdded == 0){
		SystemMsg(playerid,COLOUR_RED,gettext(964));
	} else {
		new string[(MAX_NAME*MAX_LOCS)+(MAX_LOCS*2)+1];
		for(new i=0; i<LocsAdded; i++){
			if (i == 0){
				format(string,MAX_STRING,"%s",TeleLocations[i][pLocationsName]);
			} else {
				format(string,MAX_STRING,"%s\n%s",string,TeleLocations[i][pLocationsName]);
			}
		}
		ShowPlayerDialog(playerid, DIALOGID_FILETELE, DIALOG_STYLE_LIST, gettext(1779), string, gettext(1780), gettext(1781));
	}
	return 1;
}

public ModTP_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]){

	if (dialogid == DIALOGID_FILETELE){
		if (Spec[playerid][Spectating] == true){
			ModMsg(playerid,gettext(1569));
			return 1;
		}
		if (response){
			for(new wheretoport=0; wheretoport<LocsAdded; wheretoport++){
				if (wheretoport == listitem){
					if (GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
					{
						SetVehiclePos(GetPlayerVehicleID(playerid),TeleLocations[wheretoport][pLocationsposx], TeleLocations[wheretoport][pLocationsposy], TeleLocations[wheretoport][pLocationsposz]);
						PlayerJustTeleported[playerid]+=3;
					} else 	{
						oSetPlayerPos(playerid,TeleLocations[wheretoport][pLocationsposx], TeleLocations[wheretoport][pLocationsposy], TeleLocations[wheretoport][pLocationsposz]);
					}
					SetCameraBehindPlayer(playerid);

					SetPlayerInterior(playerid,TeleLocations[wheretoport][pLocationsint]);
					new string[MAX_STRING];
					format(string,MAX_STRING,gettext(910),TeleLocations[wheretoport][pLocationsName],wheretoport);
					SystemMsg(playerid,COLOUR_GREEN,string);

					new logstring[MAX_STRING];
					format(logstring, sizeof (logstring), "Moderator %s teleport to '%s' (ID: %d)",oGetPlayerName(playerid),TeleLocations[wheretoport][pLocationsName],wheretoport);
					WriteModerLog(logstring);
					break;
				}
			}	
		}
	}
	return 0;
}