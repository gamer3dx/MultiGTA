//Boxing challenge module.
//Created by Fear (Mode: "The GodFather: LS/SF")
//Converter: MadCat

forward BoxingConfig();
forward LoadBoxers();
forward SaveBoxers();
forward Boxing_OnPlayerDisconnect(playerid);
forward Boxing_OnPlayerConnect(playerid);
forward Boxing_OnGameModeInit();
forward Boxing_OnPlayerCommandText(playerid,text[]);
forward Boxing_OnPlayerDeath(playerid, killerid, reason);
forward Boxing_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
forward Boxing_Timer();
forward BoxingPickupCheck(playerid);

new TimerBoxing;

new boxinginfoshowed[MAX_PLAYERS] = 0;

new Boxing_DB[MAX_STRING] = "MultiGTA/OtherDB/Boxers.txt";
new Boxing_Change_Style_Cost = 10000;

new InRing = 0;
new RoundStarted = 0;
new BoxDelay = 0;
new Boxer1 = 255;
new Boxer2 = 255;
new TBoxer = 255;
new BoxWaitTime[MAX_PLAYERS];
new BoxOffer[MAX_PLAYERS];

#define BoxEntrances 3

new BoxingEntrance[BoxEntrances][CoordInfo] = {
	{2229.4734,-1721.7090,13.5655}, // LS
	{-2269.3018,-155.9654,35.3203}, //SF
	{1968.7372,2295.0168,16.4559} //LV
};

enum pBoxingStats
{
    TitelName[128],
    TitelWins,
    TitelLoses,
};
new Titel[pBoxingStats];

public Boxing_OnGameModeInit(){
	BoxingConfig();
	LoadBoxers();
	for (new id=0;id<BoxEntrances;id++)
	{
		CreateStreamMapIcon(54,BoxingEntrance[id][Coord_X],BoxingEntrance[id][Coord_Y],BoxingEntrance[id][Coord_Z]);
		CreateDMapIcon(54,BoxingEntrance[id][Coord_X],BoxingEntrance[id][Coord_Y],BoxingEntrance[id][Coord_Z],gettext(1745),gettext(1746));
	}
	TimerBoxing = SetTimer("Boxing_Timer",1000,1);
	SetInteriorWeapons(INTERIOR_GYM,false);
	WriteLog("Boxing System Loaded");
	return 0;
}

public BoxingConfig()
{
	Debug("boxing.inc > BoxingConfig - Start");
	
	if (!db_Exists(DatabaseDB)) db_Create(DatabaseDB);
	new temp[MAX_STRING];
	set(temp,db_Get(DatabaseDB,"Boxing_DB"));
	if (strlen(temp) > 0) set(Boxing_DB,temp); 
	else { set(temp,Boxing_DB); db_Set(DatabaseDB,"Boxing_DB",temp);  }
	set(temp,nullstr);
	if (!db_Exists(Boxing_DB)) db_Create(Boxing_DB);
	set(temp,db_Get(ConfigDB,"Boxing_Change_Style_Cost"));
	if (strlen(temp) > 0) Boxing_Change_Style_Cost = strval(temp);
	else { valstr(temp,Boxing_Change_Style_Cost); db_Set(ConfigDB,"Boxing_Change_Style_Cost",temp);  }
	set(temp,nullstr);
	Debug("boxing.inc > BoxingConfig - Stop");
	return;
}

public LoadBoxers()
{
	Debug("boxing.inc > LoadBoxers - Start");
	new arrCoords[3][64];
	new strFromFile[256];
	new File: file = fopen(Boxing_DB, io_read);
	if (file)
	{
		fread(file, strFromFile);
		StripNewLine(strFromFile);
		split(strFromFile, arrCoords, ',');
		Titel[TitelWins] = strval(arrCoords[0]);
		strmid(Titel[TitelName], arrCoords[1], 0, strlen(arrCoords[1]), 255);
		Titel[TitelLoses] = strval(arrCoords[2]);
		fclose(file);
	} else {db_Create(Boxing_DB);}
	Debug("boxing.inc > LoadBoxers - Stop");
	return 0;
}

public SaveBoxers()
{
	Debug("boxing.inc > SaveBoxers - Start");
	new coordsstring[256];
	format(coordsstring, sizeof(coordsstring), "%d,%s,%d", Titel[TitelWins],Titel[TitelName],Titel[TitelLoses]);
	new File: file2 = fopen(Boxing_DB, io_write);
	if (file2){
		fwrite(file2, coordsstring);
		fclose(file2);
		WriteLog("All boxers saved!");
	} else {
		WriteLog("Error: can't write to file! Boxers not saved!");
	}
	Debug("boxing.inc > SaveBoxers - Stop");
	return 0;
}

public Boxing_OnPlayerDisconnect(playerid)
{
Debug("boxing.inc > Boxing_OnPlayerDisconnect - Start");
BoxWaitTime[playerid] = 0;
BoxOffer[playerid] = 999; 
if(PlayerBoxing[playerid] > 0)
	{
	    if(Boxer1 == playerid)
	    {
	        if(IsPlayerConnected(Boxer2))
	        {
	        	PlayerBoxing[Boxer2] = 0;
	        	oSetPlayerPos(Boxer2,WhereHeEnter[Boxer2][Coord_X],WhereHeEnter[Boxer2][Coord_Y],WhereHeEnter[Boxer2][Coord_Z]);
			SetPlayerFightingStyle(Boxer2,Player[Boxer2][FightingStyle]);
	        	SetPlayerInterior(Boxer2, INTERIOR_DEFAULT);
	        	ShowTextForPlayer(Boxer2, gettext(109), 5000, 1);
			WriteLog("Boxing player 1 disconnect");
			}
	    }
	    else if(Boxer2 == playerid)
	    {
	        if(IsPlayerConnected(Boxer1))
	        {
	        	PlayerBoxing[Boxer1] = 0;
	        	oSetPlayerPos(Boxer1,WhereHeEnter[Boxer1][Coord_X],WhereHeEnter[Boxer1][Coord_Y],WhereHeEnter[Boxer1][Coord_Z]);
			SetPlayerFightingStyle(Boxer1,Player[Boxer1][FightingStyle]);
	        	SetPlayerInterior(Boxer1, INTERIOR_DEFAULT);
	        	ShowTextForPlayer(Boxer1, gettext(109), 5000, 1);
			WriteLog("Boxing player 2 disconnect");
			}
	    }
	InRing = 0;
     	RoundStarted = 0;
	Boxer1 = 255;
	Boxer2 = 255;
	TBoxer = 255;
	PlayerBoxing[playerid] = 0;
	WhereHeEnter[playerid][Coord_X] = WhereHeEnter[playerid][Coord_Y] = WhereHeEnter[playerid][Coord_Z] = 0.0;
}
Debug("boxing.inc > Boxing_OnPlayerDisconnect - Stop");
}

public Boxing_OnPlayerConnect(playerid)
{
Debug("boxing.inc > Boxing_OnPlayerConnect - Start");
BoxOffer[playerid] = 999;
PlayerBoxing[playerid] = 0;
Debug("boxing.inc > Boxing_OnPlayerConnect - Stop");
}

public BoxingPickupCheck(playerid)
{
	if (playerid == INVALID_PLAYER_ID) return;
	if (!IsPlayerConnected(playerid)) return;
	if (!IsPlayerAtBoxingEntrance(playerid)) {boxinginfoshowed[playerid] = 0; return;}
	if (boxinginfoshowed[playerid] == 1) return;
	boxinginfoshowed[playerid] = 1;
	ShowBoxingHelp(playerid);
}

public Boxing_Timer()
{
Debug("boxing.inc > Boxing_Timer - Start");
new string[256];
for(new i = 0; i < MAX_PLAYERS_EX; i++)
{
    if(IsPlayerConnected(i))
    {
	if(BoxWaitTime[i] > 0)
	{
	    if(BoxWaitTime[i] >= BoxDelay)
		{
		BoxDelay = 0;
		BoxWaitTime[i] = 0;
		PlayerPlaySoundOnPlayer(i, 1057);
		ShowTextForPlayer(i,gettext(110), 5000, 1);
		TogglePlayerControllable(i, 1);
		oSetPlayerHealth(i,100);
		SetPlayerArmour(i,0);
		RoundStarted = 1;
		}
		else
		{
		format(string, sizeof(string), "%d", BoxDelay - BoxWaitTime[i]);
		ShowTextForPlayer(i, string, 1500, 6);
		BoxWaitTime[i] += 1;
		}
	    }
    	}
if(RoundStarted > 0)
			{
			    if(PlayerBoxing[i] > 0)
			    {
			        new trigger = 0;
			        new Lost = 0;
		        	new Float:angle;
		            new Float:health;
					GetPlayerHealth(i, health);
		            if(health < 12)
					{
					    if(i == Boxer1) { Lost = 1; trigger = 1; }
			            else if(i == Boxer2) { Lost = 2; trigger = 1; }
					}
			        if(health < 28) { GetPlayerFacingAngle(i, angle); SetPlayerFacingAngle(i, angle + 85); }
			        if(trigger)
			        {
			            new winner[MAX_NAME];
			            new loser[MAX_NAME];
			            new titel[MAX_NAME];
			            if(Lost == 1)
			            {
			                if(IsPlayerConnected(Boxer1) && IsPlayerConnected(Boxer2))
			                {
					        oSetPlayerPos(Boxer1,WhereHeEnter[Boxer1][Coord_X],WhereHeEnter[Boxer1][Coord_Y],WhereHeEnter[Boxer1][Coord_Z]); 
						SetPlayerFightingStyle(Boxer1,Player[Boxer1][FightingStyle]);
						oSetPlayerPos(Boxer2,WhereHeEnter[Boxer2][Coord_X],WhereHeEnter[Boxer2][Coord_Y],WhereHeEnter[Boxer2][Coord_Z]); 
						SetPlayerFightingStyle(Boxer2,Player[Boxer2][FightingStyle]);
					        SetPlayerInterior(Boxer1, INTERIOR_DEFAULT); SetPlayerInterior(Boxer2, INTERIOR_DEFAULT);
			                	loser = oGetPlayerName(Boxer1);
			                	winner = oGetPlayerName(Boxer2);
		                		Player[Boxer1][BoxLoses] += 1;
						Player[Boxer2][BoxWins] += 1;
			                	if(TBoxer < 255)
			                	{
			                	    if(IsPlayerConnected(TBoxer))
			                	    {
				                	    if(TBoxer != Boxer2)
				                	    {
			                	            	TBoxer = Boxer2;
			                	            	titel = oGetPlayerName(TBoxer);
				                	        new nstring[MAX_NAME];
								format(nstring, sizeof(nstring), "%s", titel);
								strmid(Titel[TitelName], nstring, 0, strlen(nstring), 255);
				                	        Titel[TitelWins] = Player[TBoxer][BoxWins];
				                	        Titel[TitelLoses] = Player[TBoxer][BoxLoses];
				                	        SaveBoxers();
						                format(string, sizeof(string), gettext(111),  titel, loser);
								SystemMsgToRegistered(COLOUR_BOXING,string);
				                	      		}
									else
									{
								titel = oGetPlayerName(TBoxer);
								format(string, sizeof(string), gettext(112),  titel, loser);
								SystemMsgToRegistered(COLOUR_BOXING,string);
								Titel[TitelWins] = Player[TBoxer][BoxWins];
				                	        Titel[TitelLoses] = Player[Boxer2][BoxLoses];
				                	        SaveBoxers();
								}
								}
								}//TBoxer
								format(string, sizeof(string), gettext(113), winner);
								SystemMsg(Boxer1,COLOUR_BOXING,string);
								ShowTextForPlayer(Boxer1, gettext(156), 3500, 1);
								format(string, sizeof(string), gettext(114), loser);
								SystemMsg(Boxer2,COLOUR_BOXING,string);
								ShowTextForPlayer(Boxer2, gettext(155), 3500, 1);
								if(GetPlayerHealth(Boxer1, health) < 20)
								{
								    SystemMsg(Boxer1,COLOUR_BOXING,gettext(115));
								    oSetPlayerHealth(Boxer1, 30.0);
								}
								else
								{
								    	SystemMsg(Boxer1,COLOUR_BOXING,gettext(116));
							    		oSetPlayerHealth(Boxer1, GetMaxHealth(Boxer1));
								}
								if(GetPlayerHealth(Boxer2, health) < 20)
								{
								    	SystemMsg(Boxer2,COLOUR_BOXING,gettext(115));
							    		oSetPlayerHealth(Boxer2, 30.0);
								}
								else
								{
								   	SystemMsg(Boxer2,COLOUR_BOXING,gettext(116));
								    	oSetPlayerHealth(Boxer2, GetMaxHealth(Boxer2));
								}
                                				ShowTextForPlayer(Boxer1, gettext(117), 5000, 1); ShowTextForPlayer(Boxer2, gettext(117), 5000, 1);
								Player[Boxer2][BoxingSkill] += 1;
								PlayerBoxing[Boxer1] = 0;
								PlayerBoxing[Boxer2] = 0;
							}
			            }
			            else if(Lost == 2)
			            {
			                if(IsPlayerConnected(Boxer1) && IsPlayerConnected(Boxer2))
			                {
					        oSetPlayerPos(Boxer1,WhereHeEnter[Boxer1][Coord_X],WhereHeEnter[Boxer1][Coord_Y],WhereHeEnter[Boxer1][Coord_Z]); 
						SetPlayerFightingStyle(Boxer1,Player[Boxer1][FightingStyle]);
						oSetPlayerPos(Boxer2,WhereHeEnter[Boxer2][Coord_X],WhereHeEnter[Boxer2][Coord_Y],WhereHeEnter[Boxer2][Coord_Z]); 
						SetPlayerFightingStyle(Boxer2,Player[Boxer2][FightingStyle]);
					        SetPlayerInterior(Boxer1, INTERIOR_DEFAULT); SetPlayerInterior(Boxer2, INTERIOR_DEFAULT);
			                	winner = oGetPlayerName(Boxer1);
			                	loser = oGetPlayerName(Boxer2);
		                		Player[Boxer2][BoxLoses] += 1;
						Player[Boxer1][BoxWins] += 1;
			                	if(TBoxer < 255)
			                	{
			                	    if(IsPlayerConnected(TBoxer))
			                	    {
				                	    if(TBoxer != Boxer1)
				                	    {
				                	        TBoxer = Boxer1;
				                	        titel = oGetPlayerName(TBoxer);
				                	        new nstring[MAX_NAME];
								format(nstring, sizeof(nstring), "%s", titel);
								strmid(Titel[TitelName], nstring, 0, strlen(nstring), 255);
				                	        Titel[TitelWins] = Player[TBoxer][BoxWins];
				                	        Titel[TitelLoses] = Player[TBoxer][BoxLoses];
				                	        SaveBoxers();
						                format(string, sizeof(string), gettext(111),  titel, loser);
								SystemMsgToRegistered(COLOUR_BOXING,string);
									}
									else
									{
								titel = oGetPlayerName(TBoxer);
								format(string, sizeof(string), gettext(112),  titel, loser);
								SystemMsgToRegistered(COLOUR_BOXING,string);
								Titel[TitelWins] = Player[TBoxer][BoxWins];
				                	        Titel[TitelLoses] = Player[Boxer1][BoxLoses];
				                	        SaveBoxers();
										}
									}
								}//TBoxer
								format(string, sizeof(string), gettext(113), winner);
								SystemMsg(Boxer2,COLOUR_BOXING,string);
								ShowTextForPlayer(Boxer2, gettext(156), 3500, 1);
								format(string, sizeof(string), gettext(114), loser);
								SystemMsg(Boxer1,COLOUR_BOXING,string);
								ShowTextForPlayer(Boxer1,gettext(155), 3500, 1);
								if(GetPlayerHealth(Boxer1, health) < 20)
								{
								    SystemMsg(Boxer1,COLOUR_BOXING,gettext(115));
								    oSetPlayerHealth(Boxer1, 30.0);
								}
								else
								{
								    	SystemMsg(Boxer1,COLOUR_BOXING,gettext(116));
						    			oSetPlayerHealth(Boxer1, GetMaxHealth(Boxer1));
							}
							if(GetPlayerHealth(Boxer2, health) < 20)
							{
							    SystemMsg(Boxer2,COLOUR_BOXING,gettext(115));
							    oSetPlayerHealth(Boxer2, 30.0);
							}
							else
							{
							    	SystemMsg(Boxer2,COLOUR_BOXING,gettext(116));
						    		oSetPlayerHealth(Boxer2, GetMaxHealth(Boxer2));
							}
                                			ShowTextForPlayer(Boxer1, gettext(117), 5000, 1); ShowTextForPlayer(Boxer2, gettext(117), 5000, 1);
							Player[Boxer1][BoxingSkill] += 1;
							PlayerBoxing[Boxer1] = 0;
							PlayerBoxing[Boxer2] = 0;
						}
			    }
		            InRing = 0;
		            RoundStarted = 0;
		            Boxer1 = 255;
		            Boxer2 = 255;
		            TBoxer = 255;
		            trigger = 0;
		        }
		    }
		}
     }
Debug("boxing.inc > Boxing_Timer - Stop");
}

public Boxing_OnPlayerCommandText(playerid,text[])
{
	if (!IsPlayerConnected(playerid)) return 0;
	dcmd(boxcancel,9,text);
	dcmd(boxfight,8,text);
	dcmd(boxstats,8,text);
	dcmd(boxaccept,9,text);
	dcmd(fightstyle,10,text);
	return 0;
}

public Boxing_OnPlayerDeath(playerid, killerid, reason)
	{
	Debug("boxing.inc > Boxing_OnPlayerDeath(playerid, killerid, reason) - Start");
 	if (!IsPlayerConnected(playerid))
 	{
        Debug("boxing.inc > Boxing_OnPlayerDeath(playerid, killerid, reason) - Stop");
	return 1;
	}
	if(PlayerBoxing[playerid] > 0)
	{
	    if(Boxer1 == playerid)
	    {
	        if(IsPlayerConnected(Boxer2))
	        {
	        	PlayerBoxing[Boxer2] = 0;
	        	oSetPlayerPos(Boxer2,WhereHeEnter[Boxer2][Coord_X],WhereHeEnter[Boxer2][Coord_Y],WhereHeEnter[Boxer2][Coord_Z]);
			SetPlayerFightingStyle(Boxer2,Player[Boxer2][FightingStyle]);
	        	SetPlayerInterior(Boxer2, INTERIOR_DEFAULT);
	        	ShowTextForPlayer(Boxer2, gettext(109), 5000, 1);
			WriteLog("Boxing player 2 killed");
			}
	    }
	    else if(Boxer2 == playerid)
	    {
	        if(IsPlayerConnected(Boxer1))
	        {
	        	PlayerBoxing[Boxer1] = 0;
	        	oSetPlayerPos(Boxer1,WhereHeEnter[Boxer1][Coord_X],WhereHeEnter[Boxer1][Coord_Y],WhereHeEnter[Boxer1][Coord_Z]);
			SetPlayerFightingStyle(Boxer1,Player[Boxer1][FightingStyle]);
	        	SetPlayerInterior(Boxer1, INTERIOR_DEFAULT);
	        	ShowTextForPlayer(Boxer1, gettext(109), 5000, 1);
			WriteLog("Boxing player 1 killed");
			}
	    }
	InRing = 0;
     	RoundStarted = 0;
	Boxer1 = 255;
	Boxer2 = 255;
	TBoxer = 255;
	BoxDelay = 0;
	BoxWaitTime[playerid] = 0;
	if (killerid == INVALID_PLAYER_ID){ return 1 ;}
	SystemMsg(killerid,COLOUR_BOXING,gettext(153));
	JailPlayer(killerid,300,gettext(154));
	oGivePlayerMoney(killerid,-1000*(Player[playerid][BoxingSkill]+Player[playerid][Level]),1);
	GivePlayerXP(killerid,-100*(Player[playerid][BoxingSkill]+Player[playerid][Level]),1);
	PlayerBoxing[playerid] = 0;
	}
	Debug("boxing.inc > Boxing_OnPlayerDeath(playerid, killerid, reason) - Stop");
	return 0;
}

IsPlayerAtBoxingEntrance(playerid)
{
	for (new id=0;id<BoxEntrances;id++)
	{
		if (IsPlayerInRangeOfPoint(playerid,20.0,BoxingEntrance[id][Coord_X],BoxingEntrance[id][Coord_Y],BoxingEntrance[id][Coord_Z])) return 1;
	}
	return 0;
}

dcmd_boxcancel(playerid,params[])
{
	#pragma unused params
	new giveplayer[MAX_NAME];
	new sendername[MAX_NAME];
	new string[MAX_STRING];
	if (BoxOffer[playerid] < 999){
		Debug("boxing.inc > Command 'boxcancel' - Start");
		set(giveplayer,oGetPlayerName(BoxOffer[playerid]));
		set(sendername,oGetPlayerName(playerid));
		format(string, sizeof(string),gettext(118),giveplayer);
		SystemMsg(playerid,COLOUR_BOXING,string);
		format(string, sizeof(string),gettext(119),sendername);
		SystemMsg(BoxOffer[playerid],COLOUR_BOXING,string);
		BoxOffer[playerid] = 999;
	} else if(PlayerBoxing[playerid] > 0) {
	        SystemMsg(playerid,COLOUR_BOXING,gettext(120));
		Debug("boxing.inc > Command 'boxcancel' - Stop");
	        return 1;
	} else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(121));
	}
	Debug("boxing.inc > Command 'boxcancel' - Stop");
	return 1;
}

dcmd_boxfight(playerid,params[])
{
    Debug("boxing.inc > Command 'boxfight' - Start");
    if(IsPlayerConnected(playerid))
    {
	new giveplayerid;
        if(InRing > 0)
        {
            SystemMsg(playerid,COLOUR_ERROR,gettext(122));
	    Debug("boxing.inc > Command 'boxfight' - Stop");
            return 1;
        }
        if(PlayerBoxing[playerid] > 0)
        {
            SystemMsg(playerid,COLOUR_ERROR,gettext(120));
	    Debug("boxing.inc > Command 'boxfight' - Stop");
            return 1;
        }
        if(!IsPlayerAtBoxingEntrance(playerid))
        {
            SystemMsg(playerid,COLOUR_ERROR,gettext(123));
	    Debug("boxing.inc > Command 'boxfight' - Stop");
            return 1;
        }
	if(isempty(params)) {
		SystemMsg(playerid,COLOUR_ERROR,gettext(124));
		Debug("boxing.inc > Command 'boxfight' - Stop");
		return 1;
	}

	giveplayerid = FindPlayerID(params);
	if (giveplayerid==-1) { Debug("boxing.inc > Command 'boxfight' - Stop"); return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1571),params); }
	if (giveplayerid==-2) { Debug("boxing.inc > Command 'boxfight' - Stop"); return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1572),params); }

	if(IsPlayerConnected(giveplayerid))
	{
	    if(giveplayerid != INVALID_PLAYER_ID)
	    {
		    if(giveplayerid == playerid) { SystemMsg(playerid,COLOUR_ERROR,gettext(125)); return 1; }
		    new giveplayer[MAX_NAME];
		    giveplayer=oGetPlayerName(giveplayerid);
		    new sendername[MAX_NAME];
		    sendername=oGetPlayerName(playerid);
		    new string[MAX_STRING];
		    format(string, sizeof(string),gettext(126), giveplayer);
		    SystemMsg(playerid,COLOUR_BOXING,string);
		    format(string, sizeof(string),gettext(127), sendername);
		    SystemMsg(giveplayerid,COLOUR_BOXING,string);
		    BoxOffer[giveplayerid] = playerid;
		    new logstring[256];
		    format(logstring,sizeof(logstring),"player: %d: :%s: offered a box challenge to %s",playerid,sendername,giveplayer);
		    WriteLog(logstring);
	    }
	} else{
	  SystemMsg(playerid,COLOUR_ERROR,gettext(128));
	  Debug("boxing.inc > Command 'boxfight' - Stop");
	  return 1;
	}
    }
    Debug("boxing.inc > Command 'boxfight' - Stop");
    return 1;
}

dcmd_boxstats(playerid,params[])
{
    #pragma unused params
    Debug("boxing.inc > Command 'boxstats' - Start");
    if(IsPlayerConnected(playerid))
    {
        new ttext[20];//Title
        new clevel = Player[playerid][BoxingSkill];
	if(clevel >= 0 && clevel <= 50) { set(ttext,gettext(129)); }
	else if(clevel >= 51 && clevel <= 200) { set(ttext,gettext(130)); }
	else if(clevel >= 201 && clevel <= 400) { set(ttext,gettext(131)); }
        new ntext[20];//NickName
        new level = Player[playerid][BoxWins];
        if(level > 0 && Player[playerid][BoxLoses] == 0)
        {
            	set(ntext,gettext(132));
        }
        else
        {
            	if(level >= 0 && level <= 10) { set(ntext,gettext(133)); }
            	else if(level >= 11 && level <= 20) { set(ntext,gettext(134)); }
            	else if(level >= 21 && level <= 30) { set(ntext,gettext(135)); }
            	else if(level >= 31 && level <= 40) { set(ntext,gettext(136)); }
            	else if(level >= 41 && level <= 50) { set(ntext,gettext(137)); }
           	else if(level >= 51 && level <= 60) { set(ntext,gettext(138)); }
            	else if(level >= 61 && level <= 70) { set(ntext,gettext(139)); }
            	else if(level >= 71) { set(ntext,gettext(140)); }
        }
	new string[MAX_STRING];
        SystemMsg(playerid,COLOUR_BOXING,gettext(141));
        format(string,MAX_STRING,gettext(142), Titel[TitelName],Titel[TitelWins],Titel[TitelLoses]);
	SystemMsg(playerid,COLOUR_BOXING,string);
	SystemMsg(playerid,COLOUR_BOXING,gettext(143));
	format(string,MAX_STRING,gettext(144), ttext);
	SystemMsg(playerid,COLOUR_BOXING,string);
	format(string,MAX_STRING,gettext(145), ntext);
	SystemMsg(playerid,COLOUR_BOXING,string);
	format(string,MAX_STRING,gettext(146), Player[playerid][BoxWins]);
	SystemMsg(playerid,COLOUR_BOXING,string);
	format(string,MAX_STRING,gettext(147), Player[playerid][BoxLoses]);
	SystemMsg(playerid,COLOUR_BOXING,string);
        SystemMsg(playerid,COLOUR_BOXING,"____________________________________________________");
	}
	new logstring[256];
	format(logstring,sizeof(logstring),"player: %d: %s : looking for box stat",playerid,oGetPlayerName(playerid));
	WriteLog(logstring);
	Debug("boxing.inc > Command 'boxstats' - Stop");
	return 1;
}

dcmd_boxaccept(playerid,params[])
{
	#pragma unused params
	Debug("boxing.inc > Command 'boxaccept' - Start");
	if(BoxOffer[playerid] < 999)
		    {
		    if(IsPlayerConnected(BoxOffer[playerid]))
		        {
			if (PlayerQuest[playerid] != 0){
				SystemMsg(playerid,COLOUR_ERROR,gettext(148));				
				Debug("boxing.inc > Command 'boxaccept' - Stop");
				return 1;
			}
		        new points;
		        new mypoints;
			new giveplayer[MAX_NAME],sendername[MAX_NAME];
			giveplayer = oGetPlayerName(BoxOffer[playerid]);
			sendername = oGetPlayerName(playerid);
		        new level = Player[BoxOffer[playerid]][BoxingSkill];
			if(level >= 0 && level <= 50) { points = 40; }
			else if(level >= 51 && level <= 100) { points = 50; }
			else if(level >= 101 && level <= 200) { points = 60; }
			else if(level >= 201 && level <= 400) { points = 70; }
			else if(level >= 401) { points = 80; }
			new clevel = Player[playerid][BoxingSkill];
			if(clevel >= 0 && clevel <= 50) { mypoints = 40; }
			else if(clevel >= 51 && clevel <= 100) { mypoints = 50; }
			else if(clevel >= 101 && clevel <= 200) { mypoints = 60; }
			else if(clevel >= 201 && clevel <= 400) { mypoints = 70; }
			else if(clevel >= 401) { mypoints = 80; }
			new string[MAX_STRING];
			format(string,MAX_STRING,gettext(149),giveplayer);
			SystemMsg(playerid,COLOUR_BOXING,string);
			format(string,MAX_STRING,gettext(150),sendername);
			SystemMsg(BoxOffer[playerid],COLOUR_BOXING,string);
			oSetPlayerHealth(playerid, mypoints);
			oSetPlayerHealth(BoxOffer[playerid], points);
			SetPlayerInterior(playerid, INTERIOR_GYM); SetPlayerInterior(BoxOffer[playerid], INTERIOR_GYM);
			WhereHeEnter[playerid][Coord_X] = PlayerPos[playerid][Coord_X];
			WhereHeEnter[playerid][Coord_Y] = PlayerPos[playerid][Coord_Y];
			WhereHeEnter[playerid][Coord_Z] = PlayerPos[playerid][Coord_Z];
			WhereHeEnter[BoxOffer[playerid]][Coord_X] = PlayerPos[BoxOffer[playerid]][Coord_X];
			WhereHeEnter[BoxOffer[playerid]][Coord_Y] = PlayerPos[BoxOffer[playerid]][Coord_X];
			WhereHeEnter[BoxOffer[playerid]][Coord_Z] = PlayerPos[BoxOffer[playerid]][Coord_X];
			oSetPlayerPos(playerid, 762.9852,2.4439,1001.5942); SetPlayerFacingAngle(playerid, 131.8632); SetCameraBehindPlayer(playerid); SetPlayerFightingStyle(playerid,FIGHT_STYLE_BOXING);
			oSetPlayerPos(BoxOffer[playerid], 758.7064,-1.8038,1001.5942); SetPlayerFacingAngle(BoxOffer[playerid], 313.1165); SetCameraBehindPlayer(BoxOffer[playerid]); SetPlayerFightingStyle(BoxOffer[playerid],FIGHT_STYLE_BOXING);
			TogglePlayerControllable(playerid, 0); TogglePlayerControllable(BoxOffer[playerid], 0);
			ShowTextForPlayer(playerid,gettext(151), 3000, 1); ShowTextForPlayer(BoxOffer[playerid],gettext(151), 3000, 1);
			new name[MAX_NAME];
			new dstring[MAX_NAME];
			new wstring[MAX_NAME];
			name = oGetPlayerName(playerid);
			format(dstring, sizeof(dstring), "%s", name);
			strmid(wstring, dstring, 0, strlen(dstring), 255);
			if(strcomp(Titel[TitelName] ,wstring, true ) == 1 )
			{
				format(string,MAX_STRING,gettext(152),  sendername, giveplayer);
				SystemMsgToRegistered(COLOUR_BOXING,string);
				TBoxer = playerid;
				BoxDelay = 15;
			}
			name = oGetPlayerName(BoxOffer[playerid]);
			format(dstring, sizeof(dstring), "%s", name);
			strmid(wstring, dstring, 0, strlen(dstring), 255);
			if(strcomp(Titel[TitelName] ,wstring, true ) == 1 )
			{
				format(string,MAX_STRING,gettext(152),  giveplayer, sendername);
				SystemMsgToRegistered(COLOUR_BOXING,string);
				TBoxer = BoxOffer[playerid];
				BoxDelay = 15;
			}
			BoxWaitTime[playerid] = 1; BoxWaitTime[BoxOffer[playerid]] = 1;
			if(BoxDelay < 1) { BoxDelay = 20; }
			InRing = 1;
			Boxer1 = BoxOffer[playerid];
			Boxer2 = playerid;
			PlayerBoxing[playerid] = 1;
			PlayerBoxing[BoxOffer[playerid]] = 1;
			BoxOffer[playerid] = 999;
			}
		} else {
			SystemMsg(playerid,COLOUR_ERROR,gettext(121));
		}
	Debug("boxing.inc > Command 'boxaccept' - Stop");
	return 1;
}

dcmd_fightstyle(playerid,params[]){
	#pragma unused params
        if(!IsPlayerAtBoxingEntrance(playerid))
        {
            SystemMsg(playerid,COLOUR_ERROR,gettext(123));
            return 1;
        }
	if (oGetPlayerMoney(playerid) < Boxing_Change_Style_Cost)
        {
            SystemMsg(playerid,COLOUR_ERROR,gettext(394));
            return 1;
        }
	ShowPlayerDialog(playerid, DIALOGID_FIGHTSTYLE, DIALOG_STYLE_LIST, gettext(1786), "Elbow\nBoxing\nGrabkick\nKneehead\nKungfu\nNormal", gettext(1780), gettext(1781)); //Can't load it from file O_o
	return 1;
}

public Boxing_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(dialogid == DIALOGID_FIGHTSTYLE)
	{
		if(response)
		{
			if (oGetPlayerMoney(playerid) < Boxing_Change_Style_Cost)
        		{
            			SystemMsg(playerid,COLOUR_ERROR,gettext(394));
            			return 1;
        		}
			if(listitem == 0)
			{
				if (GetPlayerFightingStyle(playerid) == FIGHT_STYLE_ELBOW){
					SystemMsg(playerid,COLOUR_ERROR,gettext(1788));
					return 1;
				}
				SetPlayerFightingStyle (playerid, FIGHT_STYLE_ELBOW);
				Player[playerid][FightingStyle] = FIGHT_STYLE_ELBOW;
				oGivePlayerMoney(playerid,-Boxing_Change_Style_Cost,1);
				SystemMsg(playerid,COLOUR_OK,gettext(1787));
			}
			if(listitem == 1)
			{
				if (GetPlayerFightingStyle(playerid) == FIGHT_STYLE_BOXING){
					SystemMsg(playerid,COLOUR_ERROR,gettext(1788));
					return 1;
				}
				SetPlayerFightingStyle (playerid, FIGHT_STYLE_BOXING);
				Player[playerid][FightingStyle] = FIGHT_STYLE_BOXING;
				oGivePlayerMoney(playerid,-Boxing_Change_Style_Cost,1);
				SystemMsg(playerid,COLOUR_OK,gettext(1787));
			}
			if(listitem == 2)
			{
				if (GetPlayerFightingStyle(playerid) == FIGHT_STYLE_GRABKICK){
					SystemMsg(playerid,COLOUR_ERROR,gettext(1788));
					return 1;
				}
			    	SetPlayerFightingStyle (playerid, FIGHT_STYLE_GRABKICK);
				Player[playerid][FightingStyle] = FIGHT_STYLE_GRABKICK;
				oGivePlayerMoney(playerid,-Boxing_Change_Style_Cost,1);
				SystemMsg(playerid,COLOUR_OK,gettext(1787));
			}
			if(listitem == 3)
			{
				if (GetPlayerFightingStyle(playerid) == FIGHT_STYLE_KNEEHEAD){
					SystemMsg(playerid,COLOUR_ERROR,gettext(1788));
					return 1;
				}
				SetPlayerFightingStyle (playerid, FIGHT_STYLE_KNEEHEAD);
				Player[playerid][FightingStyle] = FIGHT_STYLE_KNEEHEAD;
				oGivePlayerMoney(playerid,-Boxing_Change_Style_Cost,1);
				SystemMsg(playerid,COLOUR_OK,gettext(1787));
			}
			if(listitem == 4)
			{
				if (GetPlayerFightingStyle(playerid) == FIGHT_STYLE_KUNGFU){
					SystemMsg(playerid,COLOUR_ERROR,gettext(1788));
					return 1;
				}
			    	SetPlayerFightingStyle (playerid, FIGHT_STYLE_KUNGFU);
				Player[playerid][FightingStyle] = FIGHT_STYLE_KUNGFU;
				oGivePlayerMoney(playerid,-Boxing_Change_Style_Cost,1);
				SystemMsg(playerid,COLOUR_OK,gettext(1787));
			}
			if(listitem == 5)
			{
				if (GetPlayerFightingStyle(playerid) == FIGHT_STYLE_NORMAL){
					SystemMsg(playerid,COLOUR_ERROR,gettext(1788));
					return 1;
				}
				SetPlayerFightingStyle (playerid, FIGHT_STYLE_NORMAL);
				Player[playerid][FightingStyle] = FIGHT_STYLE_NORMAL;
				oGivePlayerMoney(playerid,-Boxing_Change_Style_Cost,1);
				SystemMsg(playerid,COLOUR_OK,gettext(1787));
			}
		}
		return 1;
	}
	return 0;
 }