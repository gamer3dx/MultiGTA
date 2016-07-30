//Clubs module.
//Created by MadCat
//Based on Bank Module, idea from Countryside RP, parts of code from SA-MP Filterscript

#include "base"
#include "player"

forward Club_OnGameModeInit();
forward Club_OnPlayerCommandText(playerid,text[]);
forward Club_OnPlayerDeath(playerid, killerid, reason);
forward Club_OnPlayerDisconnect(playerid);
forward Club_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
forward EnterClub(playerid);
forward ExitClub(playerid);
forward IsPlayerAtClubEntrance(playerid);
forward ClubPickupCheck(playerid);
forward ApplyDanceMove(playerid, dancemove);
forward ChangeToNextDanceMove(playerid);
forward ChangeToPreviousDanceMove(playerid);

new clubinfoshowed[MAX_PLAYERS] = 0;
new playerinclub[MAX_PLAYERS] = 0;

new WhereEnterInClub[MAX_PLAYERS][CoordInfo];

new playersinclub = 0;

new PlayerDancing[MAX_PLAYERS];
new PlayerCurrentMove[MAX_PLAYERS];
new Text:txtDanceHelper;

#define ClubsCount 3

new Club_Enabled = 1;

new Clubs[ClubsCount][CoordInfo] = {
	{1834.0199,-1684.1536,13.4490},// LS - Alhambra
	{-2552.6411,190.5722,6.0130}, // SF - Ocean Flats
	{2508.9209,1246.2365,10.8203} // LV - The Camel's Tod
}; 

stock ClubConfig()
{
	Debug("club.inc > ClubConfig - Start");
	if (!db_Exists(ConfigDB)) db_Create(ConfigDB);

	new temp[MAX_STRING];
	
	set(temp,db_Get(ConfigDB,"Club_Enabled"));
	if (strlen(temp) > 0) Club_Enabled = strval(temp); 
	else { valstr(temp,Club_Enabled); db_Set(ConfigDB,"Club_Enabled",temp); }
	set(temp,nullstr);
	
	Debug("club.inc > ClubConfig - Stop");
}

public Club_OnGameModeInit()
{
	ClubConfig();
	if (Club_Enabled == 0) return 0;
	Debug("club.inc > Club_OnGameModeInit- Start");
	for (new clubid=0;clubid<ClubsCount;clubid++)
	{
		CreateStreamPickup(1239,1,Clubs[clubid][Coord_X],Clubs[clubid][Coord_Y],Clubs[clubid][Coord_Z]);
		CreateStreamMapIcon(52,Clubs[clubid][Coord_X],Clubs[clubid][Coord_Y],Clubs[clubid][Coord_Z]);
		CreateDMapIcon(52,Clubs[clubid][Coord_X],Clubs[clubid][Coord_Y],Clubs[clubid][Coord_Z],gettext(1749),gettext(1750));
	}
	txtDanceHelper = TextDrawCreate(610.0, 400.0,gettext(60));
	TextDrawUseBox(txtDanceHelper, 0);
	TextDrawFont(txtDanceHelper, 2);
	TextDrawSetShadow(txtDanceHelper,0); // no shadow
    	TextDrawSetOutline(txtDanceHelper,1); // thickness 1
    	TextDrawBackgroundColor(txtDanceHelper,0x000000FF);
    	TextDrawColor(txtDanceHelper,0xFFFFFFFF);
    	TextDrawAlignment(txtDanceHelper,3); // align right
	SetInteriorWeapons(INTERIOR_CLUB,false);
	WriteLog("Clubs system Loaded");
	Debug("club.inc > Club_OnGameModeInit - Stop");
	return 0;
}

public IsPlayerAtClubEntrance(playerid)
{
	Debug("club.inc > IsPlayerAtClubEntrance- Start");
	for (new clubid=0;clubid<ClubsCount;clubid++)
	{
		if (IsPlayerInRangeOfPoint(playerid,2.0,Clubs[clubid][Coord_X],Clubs[clubid][Coord_Y],Clubs[clubid][Coord_Z]))
		{
			Debug("club.inc > IsPlayerAtClubEntrance- Stop");
			return 1;
		}
	}
	Debug("club.inc > IsPlayerAtClubEntrance- Stop");
	return 0;
}

public ClubPickupCheck(playerid)
{
	if (Club_Enabled == 0) return;
	Debug("club.inc > ClubPickupCheck - Start");
	if (playerid == INVALID_PLAYER_ID) return;
	if (!IsPlayerConnected(playerid)) return;
	if (!IsPlayerAtClubEntrance(playerid)) {clubinfoshowed[playerid] = 0; return;}
	if (clubinfoshowed[playerid] == 1) return;
	clubinfoshowed[playerid] = 1;
	SystemMsg(playerid,COLOUR_INFO,gettext(61));
	Debug("club.inc > ClubPickupCheck - Stop");
}



public Club_OnPlayerCommandText(playerid,text[])
{
	if (Club_Enabled == 0) return 0;
	
	if (!IsPlayerConnected(playerid)) return 0;

	dcmd(dance1,6,text);
	dcmd(dance2,6,text);
	dcmd(dance3,6,text);
	dcmd(dance4,6,text);

	return 0;
}

public EnterClub(playerid){
	Debug("club.inc > EnterClub - Start");
	if(playersinclub <= 40)
	{
		WhereEnterInClub[playerid][Coord_X]=PlayerPos[playerid][Coord_X]; 
		WhereEnterInClub[playerid][Coord_Y]=PlayerPos[playerid][Coord_Y]; 
		WhereEnterInClub[playerid][Coord_Z]=PlayerPos[playerid][Coord_Z];
		oSetPlayerPos(playerid,493.2806,-21.1408,1000.6797);
		SetPlayerInterior(playerid,INTERIOR_CLUB);
		SetPlayerFacingAngle(playerid,1.9034);
		playersinclub = playersinclub + 1;
		playerinclub[playerid]=1;
		SystemMsg(playerid,COLOUR_OK,gettext(63));
	} else { 
		SystemMsg(playerid,COLOUR_ERROR,gettext(64));
	}
	Debug("club.inc > EnterClub - Stop");
}

public ExitClub(playerid){
	Debug("club.inc > ExitClub - Start");
	oSetPlayerPos(playerid,WhereEnterInClub[playerid][Coord_X],WhereEnterInClub[playerid][Coord_Y],WhereEnterInClub[playerid][Coord_Z]);
	SetPlayerInterior(playerid,INTERIOR_DEFAULT);
	playersinclub = playersinclub - 1;
	playerinclub[playerid]=0;
	Debug("club.inc > ExitClub - Stop");
}

public Club_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if (Club_Enabled == 0) return 0;
	if(!PlayerDancing[playerid]) return 0;
	Debug("club.inc > Club_OnPlayerKeyStateChange - Start");
	
	if(IsKeyJustDown(KEY_FIRE,newkeys,oldkeys)) {
        	ChangeToNextDanceMove(playerid);
		Debug("club.inc > Club_OnPlayerKeyStateChange - Stop");
		return 0;
	}
	if(IsKeyJustDown(KEY_JUMP,newkeys,oldkeys)) {
	   	ChangeToPreviousDanceMove(playerid);
		Debug("club.inc > Club_OnPlayerKeyStateChange - Stop");
		return 0;
	}

	if(IsKeyJustDown(KEY_SPRINT,newkeys,oldkeys)) {
		PlayerCurrentMove[playerid] = 0;
	        PlayerDancing[playerid] = 0;
	        ClearAnimations(playerid);
	        TextDrawHideForPlayer(playerid,txtDanceHelper);
		Debug("club.inc > Club_OnPlayerKeyStateChange - Stop");
		return 0;
    	}
	Debug("club.inc > Club_OnPlayerKeyStateChange - Stop");
	return 0;
}

ApplyDanceMove(playerid, dancemove)
{
	Debug("club.inc > ApplyDanceMove - Start");
	switch(dancemove) {
		case 0:
			ApplyAnimation(playerid,"DANCING","DAN_LOOP_A",4.0,1,0,0,0,-1);
		case 1:
			ApplyAnimation(playerid,"DANCING","DNCE_M_A",4.0,1,0,0,0,-1);
		case 2:
			ApplyAnimation(playerid,"DANCING","DNCE_M_B",4.0,1,0,0,0,-1);
		case 3:
			ApplyAnimation(playerid,"DANCING","DNCE_M_C",4.0,1,0,0,0,-1);
		case 4:
			ApplyAnimation(playerid,"DANCING","DNCE_M_D",4.0,1,0,0,0,-1);
		case 5:
			ApplyAnimation(playerid,"DANCING","DNCE_M_E",4.0,1,0,0,0,-1);
	}
	Debug("club.inc > ApplyDanceMove - Stop");
}

ChangeToNextDanceMove(playerid)
{
	Debug("club.inc > ChangeToNextDanceMove - Start");
	PlayerCurrentMove[playerid]++;
	if(PlayerCurrentMove[playerid]==6) PlayerCurrentMove[playerid]=0;
	ApplyDanceMove(playerid,PlayerCurrentMove[playerid]);
	Debug("club.inc > ChangeToNextDanceMove - Stop");
}

ChangeToPreviousDanceMove(playerid)
{
	Debug("club.inc > ChangeToPreviousDanceMove - Start");
    	PlayerCurrentMove[playerid]--;
    	if(PlayerCurrentMove[playerid]<0) PlayerCurrentMove[playerid]=5;
	ApplyDanceMove(playerid,PlayerCurrentMove[playerid]);
	Debug("club.inc > ChangeToPreviousDanceMove - Stop");
}

public Club_OnPlayerDeath(playerid, killerid, reason)
{	
	if (Club_Enabled == 0) return 0;
	Debug("club.inc > Club_OnPlayerDeath - Start");
	if(PlayerDancing[playerid]) {
		PlayerCurrentMove[playerid] = 0;
        	PlayerDancing[playerid] = 0;
        	TextDrawHideForPlayer(playerid,txtDanceHelper);
	}
	Debug("club.inc > Club_OnPlayerDeath - Stop");
 	return 0;
}

public Club_OnPlayerDisconnect(playerid)
{
	if (Club_Enabled == 0) return 0;
	Debug("club.inc > Club_OnPlayerDisconnect - Start");
	clubinfoshowed[playerid] = 0;
	playerinclub[playerid] = 0;
	WhereEnterInClub[playerid][Coord_X] = 0.0;
	WhereEnterInClub[playerid][Coord_Y] = 0.0;
	WhereEnterInClub[playerid][Coord_Z] = 0.0;
	PlayerDancing[playerid] = 0;
	PlayerCurrentMove[playerid] = 0;
        TextDrawHideForPlayer(playerid,txtDanceHelper);
	Debug("club.inc > Club_OnPlayerDisconnect - Stop");
 	return 0;
}

dcmd_dance1(playerid,params[])
{
	#pragma unused params
	Debug("club.inc > Command 'dance1' - Start");
	if (playerinclub[playerid] == 1){
		SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE1);
	} else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(62));
	}
	Debug("club.inc > Command 'dance1' - Stop");
	return 1;
}

dcmd_dance2(playerid,params[])
{
	#pragma unused params
	Debug("club.inc > Command 'dance2' - Start");
	if (playerinclub[playerid] == 1){
		SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE2);
	} else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(62));
	}
	Debug("club.inc > Command 'dance2' - Stop");
	return 1;
}

dcmd_dance3(playerid,params[])
{
	#pragma unused params
	Debug("club.inc > Command 'dance3' - Start");
	if (playerinclub[playerid] == 1){
		SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE3);
	} else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(62));
	}
	Debug("club.inc > Command 'dance3' - Stop");
	return 1;
}

dcmd_dance4(playerid,params[])
{
	#pragma unused params
	Debug("club.inc > Command 'dance4' - Start");
	if (playerinclub[playerid] == 1){
		PlayerCurrentMove[playerid] = 0;
       		PlayerDancing[playerid] = 1;
       		ChangeToNextDanceMove(playerid);
       		TextDrawShowForPlayer(playerid,txtDanceHelper);
	} else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(62));
	}
	Debug("club.inc > Command 'dance4' - Stop");
	return 1;
}