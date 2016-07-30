//Vehicle Health Bar Module
//Created by tAxI
//Converted by MadCat

forward Vehicle_HP_Bar_OnOnGameModeInit();

new Vehicle_HP_Bar_Enabled=0;

new TimerVHPBar;

new Text:vehiclehpbar[12];

VHPConfig()
{
	new temp[MAX_STRING];
	if (!db_Exists(ConfigDB)) db_Create(ConfigDB);
	
	set(temp,db_Get(ConfigDB,"Vehicle_HP_Bar_Enabled"));
	if (strlen(temp) > 0) Vehicle_HP_Bar_Enabled = strval(temp);
	else { valstr(temp,Vehicle_HP_Bar_Enabled); db_Set(ConfigDB,"Vehicle_HP_Bar_Enabled",temp);  }
	set(temp,nullstr);

	return;
}

public Vehicle_HP_Bar_OnOnGameModeInit(){
	VHPConfig();
	if (Vehicle_HP_Bar_Enabled == 1){

		vehiclehpbar[0] = TextDrawCreate(549.0,50.0,gettext(1739));
		TextDrawUseBox(vehiclehpbar[0],true);
		TextDrawBoxColor(vehiclehpbar[0],COLOUR_RED);
		TextDrawSetShadow(vehiclehpbar[0],0);
		TextDrawTextSize(vehiclehpbar[0],625,0);

		vehiclehpbar[1] = TextDrawCreate(551.0,59.0," ");
		TextDrawUseBox(vehiclehpbar[1],true);
		TextDrawBoxColor(vehiclehpbar[1],COLOUR_RED);
		TextDrawSetShadow(vehiclehpbar[1],0);
		TextDrawTextSize(vehiclehpbar[1],551,0);

		vehiclehpbar[2] = TextDrawCreate(551.0,59.0," ");
		TextDrawUseBox(vehiclehpbar[2],true);
		TextDrawBoxColor(vehiclehpbar[2],COLOUR_RED);
		TextDrawSetShadow(vehiclehpbar[2],0);
		TextDrawTextSize(vehiclehpbar[2],556,0);

		vehiclehpbar[3] = TextDrawCreate(551.0,59.0," ");
		TextDrawUseBox(vehiclehpbar[3],true);
		TextDrawBoxColor(vehiclehpbar[3],COLOUR_RED);
		TextDrawSetShadow(vehiclehpbar[3],0);
		TextDrawTextSize(vehiclehpbar[3],561,0);

		vehiclehpbar[4] = TextDrawCreate(551.0,59.0," ");
		TextDrawUseBox(vehiclehpbar[4],true);
		TextDrawBoxColor(vehiclehpbar[4],COLOUR_YELLOW);
		TextDrawSetShadow(vehiclehpbar[4],0);
		TextDrawTextSize(vehiclehpbar[4],566,0);

		vehiclehpbar[5] = TextDrawCreate(551.0,59.0," ");
		TextDrawUseBox(vehiclehpbar[5],true);
		TextDrawBoxColor(vehiclehpbar[5],COLOUR_YELLOW);
		TextDrawSetShadow(vehiclehpbar[5],0);
		TextDrawTextSize(vehiclehpbar[5],571,0);

		vehiclehpbar[6] = TextDrawCreate(551.0,59.0," ");
		TextDrawUseBox(vehiclehpbar[6],true);
		TextDrawBoxColor(vehiclehpbar[6],COLOUR_YELLOW);
		TextDrawSetShadow(vehiclehpbar[6],0);
		TextDrawTextSize(vehiclehpbar[6],576,0);

		vehiclehpbar[7] = TextDrawCreate(551.0,59.0," ");
		TextDrawUseBox(vehiclehpbar[7],true);
		TextDrawBoxColor(vehiclehpbar[7],COLOUR_YELLOW);
		TextDrawSetShadow(vehiclehpbar[7],0);
		TextDrawTextSize(vehiclehpbar[7],581,0);

		vehiclehpbar[8] = TextDrawCreate(551.0,59.0," ");
		TextDrawUseBox(vehiclehpbar[8],true);
		TextDrawBoxColor(vehiclehpbar[8],COLOUR_GREEN);
		TextDrawSetShadow(vehiclehpbar[8],0);
		TextDrawTextSize(vehiclehpbar[8],586,0);

		vehiclehpbar[9] = TextDrawCreate(551.0,59.0," ");
		TextDrawUseBox(vehiclehpbar[9],true);
		TextDrawBoxColor(vehiclehpbar[9],COLOUR_GREEN);
		TextDrawSetShadow(vehiclehpbar[9],0);
		TextDrawTextSize(vehiclehpbar[9],591,0);

		vehiclehpbar[10] = TextDrawCreate(551.0,59.0," ");
		TextDrawUseBox(vehiclehpbar[10],true);
		TextDrawBoxColor(vehiclehpbar[10],COLOUR_GREEN);
		TextDrawSetShadow(vehiclehpbar[10],0);
		TextDrawTextSize(vehiclehpbar[10],596,0);

		vehiclehpbar[11] = TextDrawCreate(551.0,59.0," ");
		TextDrawUseBox(vehiclehpbar[11],true);
		TextDrawBoxColor(vehiclehpbar[11],COLOUR_GREEN);
		TextDrawSetShadow(vehiclehpbar[11],0);
		TextDrawTextSize(vehiclehpbar[11],602,0);

		TimerVHPBar = SetTimer("VHPBAR_Timer",500,1);

		WriteLog("Vehicle Health Bar Loaded");
	}
	return 0;
}

forward VHPBAR_Timer();
public VHPBAR_Timer(){
	for(new playerid;playerid<MAX_PLAYERS_EX;playerid++){
		if(IsPlayerConnected(playerid)){
			TextDrawHideForPlayer(playerid,vehiclehpbar[0]);
			TextDrawHideForPlayer(playerid,vehiclehpbar[1]);
			TextDrawHideForPlayer(playerid,vehiclehpbar[2]);
			TextDrawHideForPlayer(playerid,vehiclehpbar[3]);
			TextDrawHideForPlayer(playerid,vehiclehpbar[4]);
			TextDrawHideForPlayer(playerid,vehiclehpbar[5]);
			TextDrawHideForPlayer(playerid,vehiclehpbar[6]);
			TextDrawHideForPlayer(playerid,vehiclehpbar[7]);
			TextDrawHideForPlayer(playerid,vehiclehpbar[8]);
			TextDrawHideForPlayer(playerid,vehiclehpbar[9]);
			TextDrawHideForPlayer(playerid,vehiclehpbar[10]);
			TextDrawHideForPlayer(playerid,vehiclehpbar[11]);
			if (IsPlayerInAnyVehicle(playerid)){
				new Float:vhp;
				GetVehicleHealth(GetPlayerVehicleID(playerid),vhp);
				if(vhp >= 0 && vhp <= 249){
				    TextDrawShowForPlayer(playerid,vehiclehpbar[0]);
				}
				if(vhp >= 250 && vhp <= 317){
				    TextDrawShowForPlayer(playerid,vehiclehpbar[1]);
				}
				else if(vhp >= 318 && vhp <= 385){
					TextDrawShowForPlayer(playerid,vehiclehpbar[2]);
				}
				else if(vhp >= 386 && vhp <= 453){
					TextDrawShowForPlayer(playerid,vehiclehpbar[3]);
				}
				else if(vhp >= 454 && vhp <= 521){
					TextDrawShowForPlayer(playerid,vehiclehpbar[4]);
				}
				else if(vhp >= 522 && vhp <= 589){
					TextDrawShowForPlayer(playerid,vehiclehpbar[5]);
				}
				else if(vhp >= 590 && vhp <= 657){
					TextDrawShowForPlayer(playerid,vehiclehpbar[6]);
				}
				else if(vhp >= 658 && vhp <= 725){
					TextDrawShowForPlayer(playerid,vehiclehpbar[7]);
				}
				else if(vhp >= 726 && vhp <= 793){
					TextDrawShowForPlayer(playerid,vehiclehpbar[8]);
				}
				else if(vhp >= 794 && vhp <= 861){
					TextDrawShowForPlayer(playerid,vehiclehpbar[9]);
				}
				else if(vhp >= 862 && vhp <= 929){
					TextDrawShowForPlayer(playerid,vehiclehpbar[10]);
				}
				else if(vhp >= 930 && vhp <= 1000){
					TextDrawShowForPlayer(playerid,vehiclehpbar[11]);
				}
			}
		}
	}
}
