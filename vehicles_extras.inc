//Fuel and speedo module.
//Created by Donny_K, converted by GTO Dev Team
//Modder (fixes, additions): MadCat
//Speed-O-Meter by tAxI

#if defined _vehicles_extras_included
#endinput
#endif

#define _vehicles_extras_included
#pragma library vehicles_extras
#include "base"
#include "businesses\fuel"
#include "derby"

forward Fuel_OnGameModeInit();
forward CheckFuel();
forward FuelRefill(playerid);
forward Fuel_OnPlayerCommandText(playerid, cmdtext[]);
forward Fuel_OnPlayerEnterCheckpoint(playerid);
forward Fuel_OnVehicleSpawn(vehicleid);
forward IsPlayerAtFuelCP(playerid);

//Fuel (Relevant code!)
new Petrol_Amount = 100;
new Petrol_Multiplier = 15;

new Float:Petrol[MAX_VEHICLES];

new PlayerSpeed[MAX_PLAYERS];

new bool:SpeedoWithPetrol[MAX_PLAYERS] = true;

new TimerFuel;

public Fuel_OnGameModeInit(){
	Debug("vehicles_extras.inc > Fuel_OnGameModeInit - Start");
	TimerFuel = SetTimer("CheckFuel", 500, 1);
	for(new c=0;c<MAX_VEHICLES;c++)
	{
		Petrol[c] = float(Petrol_Amount);
	}
	WriteLog("Fuel System Loaded");
	Debug("vehicles_extras.inc > Fuel_OnGameModeInit - Stop");
	return 0;
}

public IsPlayerAtFuelCP(playerid)
{
	Debug("vehicles_extras.inc > IsPlayerAtFuelCP - Start");
	for (new id=0;id<MAX_GSTATIONS;id++)
	{
		if (oIsPlayerInCheckpoint(playerid,fuelstations[id][Coord_X],fuelstations[id][Coord_Y],fuelstations[id][Coord_Z],10))
		{
			Debug("vehicles_extras.inc > IsPlayerAtFuelCP - Stop");
			return 1;
		}
	}
	Debug("vehicles_extras.inc > IsPlayerAtFuelCP - Stop");
	return 0;
}

public CheckFuel()
{
	Debug("vehicles_extras.inc > CheckFuel - Start");
	new message[MAX_STRING];

	new Vi;
	
	new Float:x,Float:y,Float:z;

	for(new playerid=0;playerid<MAX_PLAYERS_EX;playerid++)
	{
		if(IsPlayerConnected(playerid))
		{
			GetPlayerPos(playerid, x, y, z);
				
			//PlayerSpeed[playerid] = floatround((floatsqroot(floatpower(floatabs(floatsub(x,PlayerPos[playerid][Coord_X])),2)+floatpower(floatabs(floatsub(y,PlayerPos[playerid][Coord_Y])),2)+floatpower(floatabs(floatsub(z,PlayerPos[playerid][Coord_Z])),2))*11000)/1400);
			PlayerSpeed[playerid] = floatround(GetPlayerSpeed(playerid)); //More effective maybe? :)
			if(IsPlayerInAnyVehicle(playerid) && gDerbyPlayerCar[playerid] == -1 && InDirtbikeStadium[playerid] == false){				

				if (PlayerSpeed[playerid] > 100) format(message,sizeof(message),gettext(65),PlayerSpeed[playerid]); 
				else if (PlayerSpeed[playerid] > 2) format(message,sizeof(message),gettext(66),PlayerSpeed[playerid]); 
				else format(message,sizeof(message),gettext(67),PlayerSpeed[playerid]); 
						
				new pvm = GetVehicleModel(GetPlayerVehicleID(playerid));
				if (pvm != V_BIKE && pvm != V_BMX && pvm != V_MTBIKE) 
				{
					new raceid = GetPlayerRace(playerid);
					if ((!raceid) || (RaceStats[raceid][race_state] != RACE_STATE_RACING))
					{
						SpeedoWithPetrol[playerid] = true;
						Vi = GetPlayerVehicleID(playerid);
					
						if (PlayerSpeed[playerid] > 2) 
						{
							Petrol[Vi] = Petrol[Vi] - (float(500)/float(1000 * Petrol_Multiplier));
						}

						if (Petrol[Vi] > 60) format(message, sizeof(message),gettext(68), message, floatround(Petrol[Vi]));
						else if (Petrol[Vi] > 40) format(message, sizeof(message),gettext(69), message, floatround(Petrol[Vi]));
						else if (Petrol[Vi] > 15) format(message, sizeof(message),gettext(70), message, floatround(Petrol[Vi]));
						else if (Petrol[Vi] <= 15) format(message, sizeof(message),gettext(71), message, floatround(Petrol[Vi]));
					
						if(Petrol[Vi] < 1)
						{
							if (Player[playerid][FuelCans] > 0){
								Player[playerid][FuelCans]--;
								Petrol[Vi] = float(Petrol_Amount);
								SystemMsg(playerid,COLOUR_OK,gettext(72));
							} else {
								RemovePlayerFromVehicle(playerid);
								SystemMsg(playerid,COLOUR_ERROR,gettext(73));
								if(Petrol[Vi] < 0) Petrol[Vi] = 0.00; 
							}
						}
					} else {
						SpeedoWithPetrol[playerid] = false;
					}
				} else {
					SpeedoWithPetrol[playerid] = false;
				}
				if (SpeedoWithPetrol[playerid] == true){
					format(message,sizeof(message),"~n~~n~~n~~n~~n~~n~~n~%s",message); //With petrol
				} else {
					format(message,sizeof(message),"~n~~n~~n~~n~~n~~n~~n~~n~%s",message); //Without petrol
				}
				ShowTextForPlayer(playerid, message, 1000, 5);
			}
			PlayerPos[playerid][Coord_X]=x;
			PlayerPos[playerid][Coord_Y]=y;
			PlayerPos[playerid][Coord_Z]=z;		
		}
	}
	Debug("vehicles_extras.inc > CheckFuel - Stop");
}


public FuelRefill(playerid)
{
	Debug("vehicles_extras.inc > FuelRefill - Start");
	new VID, id;
	VID = GetPlayerVehicleID(playerid);
	id = GetFuelStationID(playerid);
	if(Petrol[VID] < Petrol_Amount)
	{
		new Float:FillUp;
		FillUp = (float(Petrol_Amount) - Petrol[VID])*fuelstations[id][OneFuelPrice];
		if(GetPlayerMoney(playerid) >= floatround(FillUp/fuelstations[id][OneFuelPrice]))
		{
			Petrol[VID] +=FillUp/fuelstations[id][OneFuelPrice];
			new mess[64];
			format(mess, sizeof(mess),gettext(74), floatround(FillUp/fuelstations[id][OneFuelPrice]),fuelstations[id][OneFuelPrice]);
			SystemMsg(playerid,COLOUR_INFO,mess);
			if (strcomp(fuelstations[id][Owner],"Server",false) == 1){
				oGivePlayerMoney(playerid,-floatround(FillUp),0);
			} else if (strcomp(fuelstations[id][Owner],oGetPlayerName(playerid),false) == 1){
				SystemMsg(playerid,COLOUR_INFO,gettext(75));
			}
			else
			{
				oGivePlayerMoney(playerid,-floatround(FillUp/fuelstations[id][OneFuelPrice]),0);
				fuelstations[id][MoneyNow]=fuelstations[id][MoneyNow]+floatround(FillUp/fuelstations[id][OneFuelPrice]);
			}
			if (Petrol[VID] > 100) Petrol[VID] = 100;
		}
		else
		{
			SystemMsg(playerid,COLOUR_ERROR,gettext(76));
		}
	}
	else
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(77));
	}
	Debug("vehicles_extras.inc > FuelRefill - Stop");
	return 1;
}


public Fuel_OnPlayerCommandText(playerid, cmdtext[])
{
	dcmd(gasloc,6,cmdtext);
	return 0;
}


public Fuel_OnPlayerEnterCheckpoint(playerid)
{
	Debug("vehicles_extras.inc > Fuel_OnPlayerEnterCheckpoint - Start");
	if (playerid == INVALID_PLAYER_ID) return 1;
	if (!IsPlayerConnected(playerid)) return 1;
	if (!IsPlayerAtFuelCP(playerid)) return 1;
	if(IsPlayerInAnyVehicle(playerid))
	{
		for (new c; c < MAX_GSTATIONS; c++)
		{
			
			if (oIsPlayerInCheckpoint(playerid,fuelstations[c][Coord_X],fuelstations[c][Coord_Y],fuelstations[c][Coord_Z],10))
			{
				SystemMsg(playerid,COLOUR_INFO,gettext(1117));
				new id=GetFuelStationID(playerid);
				if (strcomp(fuelstations[id][Owner],oGetPlayerName(playerid),false) == 1){
				if (fuelstations[id][MoneyNow] > 0){
				SystemMsgFormat(playerid,COLOUR_INFO,gettext(79),fuelstations[id][MoneyNow]);
				}
				}
				new string[MAX_STRING];
				if (strcomp(fuelstations[id][Owner],"Server",false) == 1){
				format(string,sizeof(string),gettext(80),fuelstations[id][Cost]+fuelstations[id][MoneyNow],fuelstations[id][Level]);
				SystemMsg(playerid,COLOUR_INFO,string);
				}
				FuelRefill(playerid);
				if (PlayerQuest[playerid] == QUEST_SHOWSOMETHING) ResetQuest(playerid); //reset the dummy quest
				Debug("vehicles_extras.inc > Fuel_OnPlayerEnterCheckpoint - Stop");
				return 1;
			}
		}
	}
	Debug("vehicles_extras.inc > Fuel_OnPlayerEnterCheckpoint - Stop");
	return 0;
}

public Fuel_OnVehicleSpawn(vehicleid)
{
	Debug("vehicles_extras.inc > Fuel_OnVehicleSpawn - Start");
	Petrol[vehicleid] = float(Petrol_Amount);
	Debug("vehicles_extras.inc > Fuel_OnVehicleSpawn - Stop");
}

dcmd_gasloc(playerid,params[])
{
	#pragma unused params
	Debug("vehicles_extras.inc > Command 'gasloc' - Start");
		
	if ((GetPlayerDM(playerid)) || (GetPlayerRace(playerid)) || (PlayerQuest[playerid] != QUEST_SHOWSOMETHING && PlayerQuest[playerid] != QUEST_NOQUEST)){Debug("vehicles_extras.inc > Command 'gasloc' - Stop"); return 1;}
		
	new Float:distance,d_n,d_w;
	for (new p; p < MAX_GSTATIONS; p++) 
	{
		distance = GetPlayerDistanceToPointEx(playerid,fuelstations[p][Coord_X],fuelstations[p][Coord_Y],fuelstations[p][Coord_Z]);
		if (distance < d_n || d_n == 0) { d_n = floatround(distance); d_w = p; }
	}
		
	if (PlayerQuest[playerid] != QUEST_SHOWSOMETHING)
	{
		oDisablePlayerCheckpoint(playerid);
		oSetPlayerCheckpoint(playerid,fuelstations[d_w][Coord_X],fuelstations[d_w][Coord_Y],fuelstations[d_w][Coord_Z],5);
       		PlayerQuest[playerid] = QUEST_SHOWSOMETHING; //set a dummy quest to keep the CP fixed
		SystemMsg(playerid,COLOUR_OK,gettext(78));
	} else {
          	ResetQuest(playerid); //reset the dummy quest to keep the CP fixed
	}
	Debug("vehicles_extras.inc > Command 'gasloc' - Stop");
	return 1;
}