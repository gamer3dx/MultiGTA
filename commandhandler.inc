//Command handler module.
//Created by Iain Gilbert
//Modder: MadCat

#include "base"
#include "weaponsstat"
#include "utils\gtoutils"
#include "carlock"
#include "player"
#include "account"
#include "club"
#include "jobs\taxi"
#include "boxing"

new Menu:HelpMenu;

forward CH_OnGameModeInit();
forward CH_OnGameModeExit();
forward CH_OnPlayerSelectedMenuRow(playerid, row);
forward CH_OnPlayerExitedMenu(playerid);
forward CH_OnPlayerCommandText(playerid,text[]);

public CH_OnGameModeInit(){
	HelpMenu = CreateMenu(gettext(320), 1, 500.0, 150.0, 100.0, 140.0);
	if (IsValidMenu(HelpMenu)){
		SetMenuColumnHeader(HelpMenu, 0, gettext(321));
		AddMenuItem(HelpMenu,0,gettext(322)); //Events
		AddMenuItem(HelpMenu,0,gettext(328)); //Businesses
		AddMenuItem(HelpMenu,0,gettext(324)); //Gangs
		AddMenuItem(HelpMenu,0,gettext(325)); //Houses
		AddMenuItem(HelpMenu,0,gettext(327)); //Boxing
		AddMenuItem(HelpMenu,0,gettext(329)); //Criminal
		//AddMenuItem(HelpMenu,0,gettext(330)); //Drinks
		AddMenuItem(HelpMenu,0,gettext(1585)); //Bank
		AddMenuItem(HelpMenu,0,gettext(908)); //Taxi
		AddMenuItem(HelpMenu,0,gettext(333)); //Commands
		AddMenuItem(HelpMenu,0,gettext(334)); //Credits
		AddMenuItem(HelpMenu,0,gettext(907)); //Back
		WriteLog("Help menu created");
	} else {
		WriteLog("Warning!!! Failed to create Help Menu!");
	}
	return 0;
}

public CH_OnGameModeExit(){
	if (IsValidMenu(HelpMenu)) DestroyMenu(HelpMenu);
	return 0;
}

public CH_OnPlayerSelectedMenuRow(playerid, row){
	new Menu:Current = GetPlayerMenu(playerid);
	if(Current == HelpMenu)
	{
        	switch(row)
		{
			case 0: ShowEventsHelp(playerid); //Events
			case 1: ShowBusinessesHelp(playerid); //Businesses
			case 2: ShowGangHelp(playerid); //Gangs
			case 3: ShowHousesHelp(playerid); //Houses
			case 4: ShowBoxingHelp(playerid); //Boxing
			case 5: ShowCriminalHelp(playerid); //Criminal
			//case 6: ShowDrinksHelp(playerid); //Drinks
			case 6: ShowBankHelp(playerid); //Bank
			case 7: ShowTaxiHelp(playerid); //Taxi
			case 8: ShowCommands(playerid); //Commands 
			case 9: ShowCredits(playerid); //Credits
			case 10: return CloseMenu(playerid,Current);
		}
		return CloseMenu(playerid,Current);
	}

	return 0;
}

public CH_OnPlayerExitedMenu(playerid){
	new Menu:Current = GetPlayerMenu(playerid);
	if(Current == HelpMenu)
	{
        	return CloseMenu(playerid,Current);
	}
	return 1;
}

public CH_OnPlayerCommandText(playerid,text[])
{
	if (!IsPlayerConnected(playerid)) return 0;
	dcmd(enter,5,text);
	dcmd(exit,4,text);
	dcmd(help,4,text);
	dcmd(playtime,8,text);
	dcmd(stats,5,text);
	dcmd(level,5,text);
	dcmd(givecash,8,text);
	dcmd(bounty,6,text);
	dcmd(targets,7,text);
	dcmd(clear,5,text);
	dcmd(report,6,text);
	return 0;
}

stock ShowCriminalHelp(playerid){	
	SystemMsgScrolling(playerid,gettext(158));
	SystemMsgScrolling(playerid,gettext(159));
	SystemMsgScrolling(playerid,gettext(160));
	SystemMsgScrolling(playerid,gettext(161));
	SystemMsgScrolling(playerid,gettext(162));
	SystemMsgScrolling(playerid,gettext(163));
	SystemMsgScrolling(playerid,gettext(164));
	SystemMsgScrolling(playerid,gettext(165));
	SystemMsgScrolling(playerid,gettext(166));
	SystemMsgScrolling(playerid,gettext(167));
	SystemMsgScrolling(playerid,gettext(168));
	return 1;
}

stock ShowEventsHelp(playerid){
	SystemMsgScrolling(playerid,gettext(227));
	SystemMsgScrolling(playerid,gettext(169));
	SystemMsgScrolling(playerid,gettext(172));
	SystemMsgScrolling(playerid,gettext(173));
	SystemMsgScrolling(playerid,gettext(174));
	SystemMsgScrolling(playerid,gettext(175));
	SystemMsgScrolling(playerid,gettext(176));
	SystemMsgScrolling(playerid,gettext(177));
	SystemMsgScrolling(playerid,gettext(179));
	SystemMsgScrolling(playerid,gettext(181));
	SystemMsgScrolling(playerid,gettext(182));
	SystemMsgScrolling(playerid,gettext(183));
	SystemMsgScrolling(playerid,gettext(184));
	SystemMsgScrolling(playerid,gettext(185));
	SystemMsgScrolling(playerid,gettext(1007));
	SystemMsgScrolling(playerid,gettext(193));
	SystemMsgScrolling(playerid,gettext(194));
	SystemMsgScrolling(playerid,gettext(195));
	SystemMsgScrolling(playerid,gettext(196));
	SystemMsgScrolling(playerid,gettext(1677));
	SystemMsgScrolling(playerid,gettext(1678));
	SystemMsgScrolling(playerid,gettext(1679));
	SystemMsgScrolling(playerid,gettext(1680));
	SystemMsgScrolling(playerid,gettext(1681));
	SystemMsgScrolling(playerid,gettext(1682));
	return 1;
}

stock ShowHotwireHelp(playerid){
	SystemMsgScrolling(playerid,gettext(188));
	SystemMsgScrolling(playerid,gettext(189));
	SystemMsgScrolling(playerid,gettext(190));
	SystemMsgScrolling(playerid,gettext(191));
	return 1;
}

stock ShowGangHelp(playerid){
   	SystemMsgScrolling(playerid,gettext(198));
   	SystemMsgScrolling(playerid,gettext(199));
   	SystemMsgScrolling(playerid,gettext(200));
   	SystemMsgScrolling(playerid,gettext(201));
	SystemMsgScrolling(playerid,gettext(202));
	SystemMsgScrolling(playerid,gettext(203));
   	SystemMsgScrolling(playerid,gettext(204));
	return 1;
}

stock ShowBusinessesHelp(playerid){
	SystemMsgScrolling(playerid,gettext(326));
   	SystemMsgScrolling(playerid,gettext(205));
   	SystemMsgScrolling(playerid,gettext(206));
   	SystemMsgScrolling(playerid,gettext(207));
	new string[MAX_STRING];
	format(string,sizeof(string),gettext(208),ConvertSeconds(DRINK_TIME/1000));
   	SystemMsgScrolling(playerid,string);
   	SystemMsgScrolling(playerid,gettext(209));
   	SystemMsgScrolling(playerid,gettext(210));
	SystemMsgScrolling(playerid,gettext(1597));
	SystemMsgScrolling(playerid,gettext(1598));
	SystemMsgScrolling(playerid,gettext(1599));
	SystemMsgScrolling(playerid,gettext(1600));
	SystemMsgScrolling(playerid,gettext(1601));
	SystemMsgScrolling(playerid,gettext(1602));
	SystemMsgScrolling(playerid,gettext(1603));	
	return 1;
}

stock ShowBankHelp(playerid){
	SystemMsgScrolling(playerid,gettext(211));
	SystemMsgScrolling(playerid,gettext(212));
	SystemMsgScrolling(playerid,gettext(213));
	SystemMsgScrolling(playerid,gettext(214));
	SystemMsgScrolling(playerid,gettext(1586));
	SystemMsgScrolling(playerid,gettext(1587));
	if (PlayerGangid[playerid] != 0){
		SystemMsgScrolling(playerid,gettext(215));
		SystemMsgScrolling(playerid,gettext(216));
	}
	return 1;
}

stock ShowHousesHelp(playerid){
	SystemMsgScrolling(playerid,gettext(217));
	SystemMsgScrolling(playerid,gettext(218));
	SystemMsgScrolling(playerid,gettext(219));
	SystemMsgScrolling(playerid,gettext(220));
	SystemMsgScrolling(playerid,gettext(221));
	SystemMsgScrolling(playerid,gettext(222));
	SystemMsgScrolling(playerid,gettext(223));
	SystemMsgScrolling(playerid,gettext(224));
	SystemMsgScrolling(playerid,gettext(225));
	SystemMsgScrolling(playerid,gettext(226));
	SystemMsgScrolling(playerid,gettext(301));
	SystemMsgScrolling(playerid,gettext(302));
   	SystemMsgScrolling(playerid,gettext(303));
   	SystemMsgScrolling(playerid,gettext(304));
	SystemMsgScrolling(playerid,gettext(305));
	SystemMsgScrolling(playerid,gettext(228));
	return 1;
}

stock ShowBoxingHelp(playerid){
	SystemMsgScrolling(playerid,gettext(229));
	SystemMsgScrolling(playerid,gettext(230));
	SystemMsgScrolling(playerid,gettext(231));
	SystemMsgScrolling(playerid,gettext(232));
	SystemMsgScrolling(playerid,gettext(233));
	new string[MAX_STRING];
	format(string,MAX_STRING,gettext(1785),Boxing_Change_Style_Cost);
	SystemMsgScrolling(playerid,string);
	return 1;
}

stock ShowTaxiHelp(playerid){
	new string[MAX_STRING];
	SystemMsgScrolling(playerid,gettext(1378));
	format(string, MAX_STRING,gettext(1379), TAXI_BONUS_PASSENGER_COUNT);
	SystemMsgScrolling(playerid,string);
	format(string, MAX_STRING, gettext(1380), TAXI_BONUS_CASH);
	SystemMsgScrolling(playerid,string);	
	SystemMsgScrolling(playerid,gettext(1381));
	SystemMsgScrolling(playerid,gettext(1382));
	SystemMsgScrolling(playerid,gettext(1383));
	return 1;
}

stock ShowCommands(playerid){
	SystemMsgScrolling(playerid,gettext(234));
	SystemMsgScrolling(playerid,gettext(235));
	SystemMsgScrolling(playerid,gettext(236));
	SystemMsgScrolling(playerid,gettext(237));
	if (Auto_Login_Enabled == 1){SystemMsgScrolling(playerid,gettext(1465));}
	SystemMsgScrolling(playerid,gettext(238));
	SystemMsgScrolling(playerid,gettext(239));
	SystemMsgScrolling(playerid,gettext(240));
	SystemMsgScrolling(playerid,gettext(241));
	SystemMsgScrolling(playerid,gettext(242));
	SystemMsgScrolling(playerid,gettext(243));
	SystemMsgScrolling(playerid,gettext(244));
	SystemMsgScrolling(playerid,gettext(245));
	SystemMsgScrolling(playerid,gettext(1588));
	SystemMsgScrolling(playerid,gettext(289));
	if (PlayerGangid[playerid] != 0)
	{
	SystemMsgScrolling(playerid,gettext(290));
	SystemMsgScrolling(playerid,gettext(291));
	SystemMsgScrolling(playerid,gettext(292));
	SystemMsgScrolling(playerid,gettext(293));
	SystemMsgScrolling(playerid,gettext(294));
	SystemMsgScrolling(playerid,gettext(295));
	SystemMsgScrolling(playerid,gettext(296));
	SystemMsgScrolling(playerid,gettext(297));
	SystemMsgScrolling(playerid,gettext(298));
	SystemMsgScrolling(playerid,gettext(299));
	SystemMsgScrolling(playerid,gettext(300));
	SystemMsgScrolling(playerid,gettext(1604));
	SystemMsgScrolling(playerid,gettext(1622));
	}
	SystemMsgScrolling(playerid,gettext(246));
	SystemMsgScrolling(playerid,gettext(247));
	SystemMsgScrolling(playerid,gettext(248));
	SystemMsgScrolling(playerid,gettext(249));
	SystemMsgScrolling(playerid,gettext(250));
	SystemMsgScrolling(playerid,gettext(251));
	SystemMsgScrolling(playerid,gettext(252));
	SystemMsgScrolling(playerid,gettext(1374));
	SystemMsgScrolling(playerid,gettext(253));
	SystemMsgScrolling(playerid,gettext(254));
	SystemMsgScrolling(playerid,gettext(255));
	SystemMsgScrolling(playerid,gettext(256));
	SystemMsgScrolling(playerid,gettext(257));
	SystemMsgScrolling(playerid,gettext(258));
	SystemMsgScrolling(playerid,gettext(259));
	SystemMsgScrolling(playerid,gettext(260));
	SystemMsgScrolling(playerid,gettext(261));
	SystemMsgScrolling(playerid,gettext(262));
	SystemMsgScrolling(playerid,gettext(263));
	SystemMsgScrolling(playerid,gettext(264));
	SystemMsgScrolling(playerid,gettext(265));
	SystemMsgScrolling(playerid,gettext(266));
	SystemMsgScrolling(playerid,gettext(267));
	SystemMsgScrolling(playerid,gettext(268));
	SystemMsgScrolling(playerid,gettext(269));
	SystemMsgScrolling(playerid,gettext(270));
	SystemMsgScrolling(playerid,gettext(271));
	SystemMsgScrolling(playerid,gettext(272));
	SystemMsgScrolling(playerid,gettext(273));
	if (Player[playerid][GotJob] == JOB_COP){
	SystemMsgScrolling(playerid,gettext(274));
	SystemMsgScrolling(playerid,gettext(275));
	SystemMsgScrolling(playerid,gettext(276));
	}
	if (Player[playerid][GotJob] == JOB_MEDIC){
	SystemMsgScrolling(playerid,gettext(277));
	SystemMsgScrolling(playerid,gettext(278));
	SystemMsgScrolling(playerid,gettext(276));
	SystemMsgScrolling(playerid,gettext(280));
	} else {
	SystemMsgScrolling(playerid,gettext(281));
	}
	SystemMsgScrolling(playerid,gettext(282));
	SystemMsgScrolling(playerid,gettext(283));
	SystemMsgScrolling(playerid,gettext(284));
	SystemMsgScrolling(playerid,gettext(285));
	SystemMsgScrolling(playerid,gettext(286));
	SystemMsgScrolling(playerid,gettext(287));
	SystemMsgScrolling(playerid,gettext(288));
	SystemMsgScrolling(playerid,gettext(1308));
	SystemMsgScrolling(playerid,gettext(1385));
	SystemMsgScrolling(playerid,gettext(1445));
	SystemMsgScrolling(playerid,gettext(1446));
	if (CarLock_Enabled == 1){
	SystemMsgScrolling(playerid,gettext(976));
	SystemMsgScrolling(playerid,gettext(977));
	}
	SystemMsgScrolling(playerid,gettext(1367));
	SystemMsgScrolling(playerid,gettext(1478));
	SystemMsgScrolling(playerid,gettext(358));
	SystemMsgScrolling(playerid,gettext(376));
	SystemMsgScrolling(playerid,gettext(968));
	SystemMsgScrolling(playerid,gettext(1456));
	SystemMsgScrolling(playerid,gettext(1655));
	SystemMsgScrolling(playerid,gettext(1735));
	SystemMsgScrolling(playerid,gettext(1736));
	SystemMsgScrolling(playerid,gettext(1737));
	SystemMsgScrolling(playerid,gettext(1738));
	return 1;
}

stock ShowDrinksHelp(playerid){
   	SystemMsgScrolling(playerid,gettext(306));
	SystemMsgScrolling(playerid,gettext(307));
	SystemMsgScrolling(playerid,gettext(308));
	new string[256];
	format(string,sizeof(string),gettext(309),ConvertSeconds(DRINK_TIME/1000));
	SystemMsgScrolling(playerid,string);
	return 1;
}

stock ShowCredits(playerid){
	SystemMsgScrolling(playerid,gettext(310));
   	SystemMsgScrolling(playerid,gettext(311));
   	SystemMsgScrolling(playerid,gettext(312));
	SystemMsgScrolling(playerid,gettext(313));
	SystemMsgScrolling(playerid,gettext(314));
   	SystemMsgScrolling(playerid,gettext(315));
	SystemMsgScrolling(playerid,gettext(316));
	SystemMsgScrolling(playerid,gettext(317));
	SystemMsgScrolling(playerid,gettext(318));
	SystemMsgScrolling(playerid,gettext(319));
	return 1;
}

dcmd_enter(playerid,params[])
{
	#pragma unused params
	if (IsPlayerInAnyDM(playerid) == 1) return 0; //Deathmatch near house. Don't do this
	if (IsPlayerInAnyHay(playerid) == 1) return 0;
	Debug("commandhandler.inc > Command 'enter' - Start");
	if (IsPlayerAtHouseEntrance(playerid)){
		EnterHouse(playerid);
	} else if (IsPlayerAtClubEntrance(playerid) && Club_Enabled == 1){
		EnterClub(playerid);
	} else 
	{
	SystemMsg(playerid,COLOUR_ERROR,gettext(157));
	}
	Debug("commandhandler.inc > Command 'enter' - Stop");
	return 1;
}

dcmd_exit(playerid,params[])
{
	#pragma unused params
	Debug("commandhandler.inc > Command 'exit' - Start");
	if (IsPlayerAtHouseExit(playerid)){
		ExitHouse(playerid);
	} else if (IsPlayerInRangeOfPoint(playerid,4.0,493.0916,-24.4315,1000.6797) && Club_Enabled == 1){
		ExitClub(playerid);
	} else 
	{
	SystemMsg(playerid,COLOUR_ERROR,gettext(157));
	}
	Debug("commandhandler.inc > Command 'exit' - Stop");
	return 1;
}

dcmd_help(playerid,params[])
{
	#pragma unused params
	Debug("commandhandler.inc > Command 'help' - Start");
	if (IsValidMenu(HelpMenu)){
		ShowMenuForPlayer(HelpMenu,playerid);
		TogglePlayerControllable(playerid,false);
	}
	Debug("commandhandler.inc > Command 'help' - Stop");
	return 1;
}

dcmd_playtime(playerid,params[])
{
	#pragma unused params
	Debug("commandhandler.inc > Command 'playtime' - Start");
	new string[256];
	format(string,sizeof(string),gettext(336),FormatTimeForStat(PlayTimeForStats[playerid]));
	SystemMsg(playerid,COLOUR_INFO,string);
	format(string,sizeof(string),gettext(337),FormatTimeForStat(Player[playerid][TotalPlaytime]));
	SystemMsg(playerid,COLOUR_INFO,string);
	Debug("commandhandler.inc > Command 'playtime' - Stop");
	return 1;
}

dcmd_stats(playerid,params[])
{
	Debug("commandhandler.inc > Command 'stats' - Start");
	new giveplayerid = INVALID_PLAYER_ID;
	if (isempty(params)){
		giveplayerid = playerid;
	} else {
		giveplayerid = FindPlayerID(params);
		if (giveplayerid==-1) { Debug("commandhandler.inc > Command 'stats' - Stop"); return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1571),params); }
		if (giveplayerid==-2) { Debug("commandhandler.inc > Command 'stats' - Stop"); return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1572),params); }
		if (!IsPlayerConnected(giveplayerid)){
			SystemMsg(playerid,COLOUR_ERROR,gettext(32));
			Debug("commandhandler.inc > Command 'stats' - Stop");
			return 1;
		}
	}
	new string[MAX_STRING];
	format(string,MAX_STRING,gettext(338),oGetPlayerName(giveplayerid));
	SystemMsgScrolling(playerid,string);
	format(string,MAX_STRING,gettext(339), Player[giveplayerid][Level], Player[giveplayerid][XP], GetLevelName(Player[giveplayerid][Level]));
	SystemMsgScrolling(playerid,string);
	format(string,MAX_STRING,gettext(348),FormatTimeForStat(PlayTimeForStats[giveplayerid]),FormatTimeForStat(Player[giveplayerid][TotalPlaytime]));
	SystemMsgScrolling(playerid,string);
	if (PlayerGangid[giveplayerid] != 0){
		format(string,MAX_STRING,gettext(343), PlayerGangName[giveplayerid], Player[giveplayerid][GangMateKills]);
		SystemMsgScrolling(playerid,string);
	}
	format(string,MAX_STRING,gettext(345), Player[giveplayerid][Money]+Player[giveplayerid][BankMoney], Player[giveplayerid][Bounty]);
	SystemMsgScrolling(playerid,string);
	format(string,MAX_STRING,gettext(347), Player[giveplayerid][Kills], Player[giveplayerid][Deaths], Player[giveplayerid][Suicides], FormatRatio(Player[giveplayerid][Kills],Player[giveplayerid][Deaths]));
	SystemMsgScrolling(playerid,string);
	new fw = GetFavoriteWeapon(playerid);
	new dr = GetFavoriteDeathReason(playerid);
	format(string,MAX_STRING,gettext(1589),oGetWeaponName(fw),STATS_WeaponKills[playerid][fw],oGetWeaponName(dr),STATS_WeaponDeaths[playerid][dr]);
	SystemMsgScrolling(playerid,string);
	format(string,MAX_STRING,gettext(350), Player[giveplayerid][Drinks],Player[giveplayerid][DrinksUsed]);
	SystemMsgScrolling(playerid,string);
	format(string,MAX_STRING,gettext(352), Player[giveplayerid][FuelCans], Player[giveplayerid][ClothesBuyed], Player[giveplayerid][FoodEaten]);
	SystemMsgScrolling(playerid,string);
	format(string,MAX_STRING,gettext(354), Player[giveplayerid][SpawnKills], Player[giveplayerid][SpawnDeaths]);
	SystemMsgScrolling(playerid,string);
	format(string,MAX_STRING,gettext(356), Player[giveplayerid][RacesPlayed], Player[giveplayerid][RacesWon], FormatRatio(Player[giveplayerid][RacesPlayed],Player[giveplayerid][RacesWon]));
	SystemMsgScrolling(playerid,string);
	format(string,MAX_STRING,gettext(342), Player[giveplayerid][DMsPlayed], Player[giveplayerid][DMsWon], FormatRatio(Player[giveplayerid][DMsPlayed],Player[giveplayerid][DMsWon])); 
	SystemMsgScrolling(playerid,string);
	format(string,MAX_STRING,gettext(1676), Player[giveplayerid][HaysPlayed], Player[giveplayerid][HaysWon], FormatRatio(Player[giveplayerid][HaysPlayed],Player[giveplayerid][HaysWon])); 
	SystemMsgScrolling(playerid,string);
	if (GetPlayerHouse(playerid) != -1){
		format(string,MAX_STRING,gettext(362),Player[giveplayerid][WasAtHouse]);
		SystemMsgScrolling(playerid,string);
	}
	if (GetPlayerRent(playerid) != -1){
		format(string,MAX_STRING,gettext(363),Player[giveplayerid][WasAtHouse]);
		SystemMsgScrolling(playerid,string);
	}
	format(string,MAX_STRING,gettext(365), Player[giveplayerid][WantedLevel], Player[giveplayerid][Crimes], Player[giveplayerid][GotBusted]);
	SystemMsgScrolling(playerid,string);
	format(string,MAX_STRING,gettext(368), Player[giveplayerid][BoxingSkill], Player[giveplayerid][BoxWins], Player[giveplayerid][BoxLoses]);
	SystemMsgScrolling(playerid,string);
	if (Player[giveplayerid][JailTime] > 0){
		format(string,MAX_STRING,gettext(371), FormatTimeForStat(Player[giveplayerid][JailTime]));
		SystemMsgScrolling(playerid,string);
	}
	if (Player[giveplayerid][MuteTime] > 0){
		format(string,MAX_STRING,gettext(372), FormatTimeForStat(Player[giveplayerid][MuteTime]));
		SystemMsgScrolling(playerid,string);
	}
	format(string,MAX_STRING,gettext(373),Player[giveplayerid][Warns]);
	SystemMsgScrolling(playerid,string);
	if (Player[giveplayerid][GotJob] == JOB_COP){
		format(string,MAX_STRING,gettext(375),Player[giveplayerid][CopSkill],Player[giveplayerid][CopLevel]);
		SystemMsgScrolling(playerid,string);
	}
	if (Player[giveplayerid][GotJob] == JOB_MEDIC){
		format(string,MAX_STRING,gettext(379),Player[giveplayerid][Heals]);
		SystemMsgScrolling(playerid,string);
	}
	Debug("commandhandler.inc > Command 'stats' - Stop");
	return 1;
}

dcmd_level(playerid,params[])
{
	#pragma unused params
	Debug("commandhandler.inc > Command 'level' - Start");
	CheckPlayerLevel(playerid);
	new string[256];
	SystemMsg(playerid,COLOUR_INFO,gettext(380));
	format(string,MAX_STRING,gettext(381), Player[playerid][Level]);
	SystemMsg(playerid,COLOUR_INFO,string);
	format(string,MAX_STRING,gettext(382), Player[playerid][XP]);
	SystemMsg(playerid,COLOUR_INFO,string);
	format(string,MAX_STRING,gettext(383), GetXPToLevel(playerid, Player[playerid][Level] + 1));
	SystemMsg(playerid,COLOUR_INFO,string);
	Debug("commandhandler.inc > Command 'level' - Stop");
	return 1;
}

dcmd_report(playerid,params[])
{
	Debug("commandhandler.inc > Command 'report' - Start");
	new string[256];

	if(isempty(params))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(398));
		Debug("commandhandler.inc > Command 'report' - Stop");
		return 1;
	}

	new count = 0;
	for (new modid=0; modid<MAX_PLAYERS_EX;modid++)
	{
		if (IsPlayerConnected(modid)){
			if (PlayerPriv[modid][IsModerator] == 1){
				format(string,MAX_STRING,gettext(400),oGetPlayerName(playerid),params);
				ModMsg(modid,string);
				count++;
			}
		}
	
	}

	if (count == 0)
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(401));
	} else {
		SystemMsg(playerid,COLOUR_OK,gettext(402));
	}
	Debug("commandhandler.inc > Command 'report' - Stop");
	return 1;
}

dcmd_targets(playerid,params[])
{
	#pragma unused params
	Debug("commandhandler.inc > Command 'targets' - Start");
	new temp[MAX_STRING];
	new count = 0;
	for (new i=0;i<MAX_PLAYERS_EX;i++)
	{
		if (IsPlayerConnected(i)){
			if(Player[i][Bounty]>0)
			{
				count++;
				format(temp,sizeof(temp),gettext(396),count,oGetPlayerName(i),Player[i][Bounty]);
				SystemMsgScrolling(playerid,temp);
				set(temp,nullstr);
			}
		}
	}
	if(count < 1)
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(397));
	}
	Debug("commandhandler.inc > Command 'targets' - Stop");
	return 1;
}

dcmd_clear(playerid,params[])
{
	#pragma unused params
	Debug("commandhandler.inc > Command 'clear' - Start");
	for (new a=1;a<=10;a++) SystemMsg(playerid,COLOUR_MISC,"\n");
	Debug("commandhandler.inc > Command 'clear' - Stop");
	return 1;
}

dcmd_givecash(playerid,params[])
{
	Debug("commandhandler.inc > Command 'givecash' - Start");
	new string[256];
	new giveplayerid, moneys,idx;
	new tmp[256];
	tmp = strcharsplit(params, idx,strchar(" "));

	if(isempty(tmp))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(384));
		Debug("commandhandler.inc > Command 'givecash' - Stop");
		return 1;
	}
		
	giveplayerid = FindPlayerID(tmp);
	if (giveplayerid==-1) { Debug("commandhandler.inc > Command 'givecash' - Stop"); return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1571),tmp);}
	if (giveplayerid==-2) { Debug("commandhandler.inc > Command 'givecash' - Stop"); return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1572),tmp);}

	if (giveplayerid == playerid){
		SystemMsg(playerid,COLOUR_ERROR,gettext(385));
		Debug("commandhandler.inc > Command 'givecash' - Stop");
		return 1;
	}

	tmp = strcharsplit(params, idx,strchar(" "));
	if(isempty(tmp))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(384));
		Debug("commandhandler.inc > Command 'givecash' - Stop");
		return 1;
	}
	moneys = strval(tmp);

	if (IsPlayerConnected(giveplayerid))
	{
		if (moneys > 0 && oGetPlayerMoney(playerid) >= moneys)
		{
			oGivePlayerMoney(playerid, (0 - moneys),0);
			oGivePlayerMoney(giveplayerid, moneys,0);
			format(string,MAX_STRING,gettext(386), moneys, oGetPlayerName(giveplayerid),giveplayerid);
			SystemMsg(playerid,COLOUR_MONEY_GOOD,string);
			format(string,MAX_STRING,gettext(387), moneys, oGetPlayerName(playerid), playerid);
			SystemMsg(giveplayerid,COLOUR_MONEY_GOOD,string);

			new logstring[256];
			format(logstring, sizeof (logstring), "%s(playerid:%d) has transfered %d to %s(playerid:%d)",oGetPlayerName(playerid), playerid, moneys, oGetPlayerName(giveplayerid), giveplayerid);
			WriteLog(logstring);
		}
		else
		{
			SystemMsg(playerid,COLOUR_ERROR,gettext(388));
		}
	}
	else
	{
		format(string,MAX_STRING,gettext(389),giveplayerid);
		SystemMsg(playerid,COLOUR_ERROR,string);
	}
	Debug("commandhandler.inc > Command 'givecash' - Stop");
	return 1;
}

dcmd_bounty(playerid,params[])
{
	Debug("commandhandler.inc > Command 'bounty' - Start");
	new string[256];
	new giveplayerid, moneys,idx;
	new tmp[256];
	tmp = strcharsplit(params, idx,strchar(" "));

	if(isempty(tmp))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(390));
		Debug("commandhandler.inc > Command 'bounty' - Stop");
		return 1;
	}
		
	giveplayerid = FindPlayerID(tmp);
	if (giveplayerid==-1) { Debug("commandhandler.inc > Command 'bounty' - Stop"); return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1571),tmp); }
	if (giveplayerid==-2) { Debug("commandhandler.inc > Command 'bounty' - Stop"); return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1572),tmp); }

	if (giveplayerid == playerid){
		SystemMsg(playerid,COLOUR_ERROR,gettext(391));
		Debug("commandhandler.inc > Command 'bounty' - Stop");
		return 1;
	}

	tmp = strcharsplit(params, idx,strchar(" "));
	if(isempty(tmp))
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(390));
		Debug("commandhandler.inc > Command 'bounty' - Stop");
		return 1;
	}
	moneys = strval(tmp);

	if (IsPlayerConnected(giveplayerid))
	{
		if (GetDistanceBetweenPlayers(playerid,giveplayerid) < 5){
			if (moneys > 0 && oGetPlayerMoney(playerid) >= moneys)
			{
				oGivePlayerMoney(playerid, (0 - moneys),0);
				Player[giveplayerid][Bounty] = Player[giveplayerid][Bounty]+moneys;
				format(string,MAX_STRING,gettext(392),moneys,oGetPlayerName(giveplayerid));
				SystemMsg(playerid,COLOUR_OK,string);
				format(string,MAX_STRING,gettext(393),oGetPlayerName(playerid),moneys);
				SystemMsg(giveplayerid,COLOUR_INFO,string);
				
				new logstring[256];
				format(logstring, sizeof (logstring), "%s(playerid:%d) set a bounty $%d to %s(playerid:%d)",oGetPlayerName(playerid), playerid, moneys, oGetPlayerName(giveplayerid), giveplayerid);
				WriteLog(logstring);
			} else {
				SystemMsg(playerid,COLOUR_ERROR,gettext(394));
			}
		} else {
			SystemMsg(playerid,COLOUR_ERROR,gettext(399));
		}
	} else {
		format(string,MAX_STRING,gettext(395), giveplayerid);
		SystemMsg(playerid,COLOUR_ERROR,string);
	}
	Debug("commandhandler.inc > Command 'bounty' - Stop");
	return 1;
}