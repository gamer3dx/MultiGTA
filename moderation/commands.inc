//Moderator Commands Module
//Created by Iain Gilbert
//Modded: MadCat
//Ideas for Additions from Freeroam 2.4, Nitros Freeroam 3.4.0, Y_Less, me...

#include "base"
#include "antifall"
#include "commands_spec"

forward Moderator_OnGameModeInit();
forward Moderator_OnPlayerText(playerid,text[]);
forward Moderator_OnPlayerCommandText(playerid,text[]);
forward Moderator_OnGameModeExit();

new MarkedPlayer[MAX_PLAYERS];
new bool:ModSpySMS[MAX_PLAYERS];

public Moderator_OnPlayerCommandText(playerid,text[])
{
if (!IsPlayerConnected(playerid)) return 0;

if (PlayerPriv[playerid][IsModerator] == 0) return 0;
new cmd[20];
new idx;
set(cmd,strcharsplit(text, idx,strchar(" ")));
if(strcomp(cmd, "/mod", true) == 0) return 0;

set(cmd,strcharsplit(text, idx,strchar(" ")));

if(strcomp(cmd, "broadcast", true) == 1)
{
if (PlayerPriv[playerid][CanBroadcast] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod broadcast' - Start");
new string[MAX_STRING];
set(string,text);
strdel(string, 0, idx);
format(string,sizeof(string),gettext(1149),string);
SystemMsgToRegistered(COLOUR_INFO,string);
new logstring[MAX_STRING];
format(logstring, sizeof (logstring), "Moderator %s broadcasted message: '%s'",oGetPlayerName(playerid),string);
WriteModerLog(logstring);
Debug("moderation/commands.inc > Command '/mod broadcast' - Stop");
return 1;
}

if(strcomp(cmd, "help", true) == 1)
{
Debug("moderation/commands.inc > Command '/mod help' - Start");
SystemMsgScrolling(playerid,gettext(1150));
if (PlayerPriv[playerid][CanBroadcast] == 1) SystemMsgScrolling(playerid,gettext(1151));
if (PlayerPriv[playerid][CanWorldSave] == 1) SystemMsgScrolling(playerid,gettext(1152));
if (PlayerPriv[playerid][CanAnnounce] == 1) SystemMsgScrolling(playerid,gettext(1153));
if (PlayerPriv[playerid][CanGiveXP] == 1) SystemMsgScrolling(playerid,gettext(1154));
if (PlayerPriv[playerid][CanSlap] == 1) SystemMsgScrolling(playerid,gettext(1155));
if (PlayerPriv[playerid][CanGiveMoney] == 1) SystemMsgScrolling(playerid,gettext(1156));
if (PlayerPriv[playerid][CanSetMoney] == 1) SystemMsgScrolling(playerid,gettext(1157));
if (PlayerPriv[playerid][CanSetBank] == 1) SystemMsgScrolling(playerid,gettext(1158));
if (PlayerPriv[playerid][CanAddCrimes] == 1) SystemMsgScrolling(playerid,gettext(1159));
if (PlayerPriv[playerid][CanDisarm] == 1) SystemMsgScrolling(playerid,gettext(1160));
if (PlayerPriv[playerid][CanFreeze] == 1) SystemMsgScrolling(playerid,gettext(1161));
if (PlayerPriv[playerid][CanUnFreeze] == 1) SystemMsgScrolling(playerid,gettext(1162));
if (PlayerPriv[playerid][CanHeal] == 1) SystemMsgScrolling(playerid,gettext(1163));
if (PlayerPriv[playerid][CanGod] == 1) SystemMsgScrolling(playerid,gettext(1164));
if (PlayerPriv[playerid][CanMurder] == 1) SystemMsgScrolling(playerid,gettext(1165));
if (PlayerPriv[playerid][CanSilentMurder] == 1) SystemMsgScrolling(playerid,gettext(1166));
if (PlayerPriv[playerid][CanJail] == 1) SystemMsgScrolling(playerid,gettext(1167));
if (PlayerPriv[playerid][CanUnjail] == 1) SystemMsgScrolling(playerid,gettext(1168));
if (PlayerPriv[playerid][CanJailList] == 1) SystemMsgScrolling(playerid,gettext(1169));
if (PlayerPriv[playerid][CanMute] == 1) SystemMsgScrolling(playerid,gettext(1170));
if (PlayerPriv[playerid][CanMuteList] == 1) SystemMsgScrolling(playerid,gettext(1171));
if (PlayerPriv[playerid][CanUnmute] == 1) SystemMsgScrolling(playerid,gettext(1172));
if (PlayerPriv[playerid][CanWarn] == 1) SystemMsgScrolling(playerid,gettext(1173));
if (PlayerPriv[playerid][CanUnwarn] == 1) SystemMsgScrolling(playerid,gettext(1174));
if (PlayerPriv[playerid][CanLockAcc] == 1) SystemMsgScrolling(playerid,gettext(1175));
if (PlayerPriv[playerid][CanUnlockAcc] == 1) SystemMsgScrolling(playerid,gettext(1176));
if (PlayerPriv[playerid][CanMakeItems] == 1) SystemMsgScrolling(playerid,gettext(1177));
if (PlayerPriv[playerid][CanSeePlayersInfo] == 1) SystemMsgScrolling(playerid,gettext(1178));
if (PlayerPriv[playerid][CanStartDM] == 1){
	SystemMsgScrolling(playerid,gettext(1179));
	SystemMsgScrolling(playerid,gettext(1180));
	SystemMsgScrolling(playerid,gettext(1675));
}
if (PlayerPriv[playerid][CanStartRace] == 1){
	SystemMsgScrolling(playerid,gettext(1181));
	SystemMsgScrolling(playerid,gettext(170));
	SystemMsgScrolling(playerid,gettext(1671));
}
if (PlayerPriv[playerid][CanEditRace] == 1) SystemMsgScrolling(playerid,gettext(1182));
//if (PlayerPriv[playerid][CanStartHotwire] == 1) SystemMsgScrolling(playerid,gettext(1183));
if (PlayerPriv[playerid][CanStartCashBox] == 1) SystemMsgScrolling(playerid,gettext(1184));
if (PlayerPriv[playerid][CanTeleportToPlayers] == 1) SystemMsgScrolling(playerid,gettext(1185));
if (PlayerPriv[playerid][CanTeleportPlayersToYourself] == 1) SystemMsgScrolling(playerid,gettext(1186));
if (PlayerPriv[playerid][CanGiveWeapons] == 1) SystemMsgScrolling(playerid,gettext(1187));
if (PlayerPriv[playerid][CanKickPlayers] == 1) SystemMsgScrolling(playerid,gettext(1188));
if (PlayerPriv[playerid][CanLockServer] == 1) SystemMsgScrolling(playerid,gettext(1189));
if (PlayerPriv[playerid][CanUnlockServer] == 1) SystemMsgScrolling(playerid,gettext(1190));
if (PlayerPriv[playerid][CanMark] == 1) SystemMsgScrolling(playerid,gettext(1191));
if (PlayerPriv[playerid][CanSpySMS] == 1) SystemMsgScrolling(playerid,gettext(1457));
if (PlayerPriv[playerid][CanSetAntiBikeFall] == 1) SystemMsgScrolling(playerid,gettext(1468));
if (PlayerPriv[playerid][CanSpectate] == 1) SystemMsgScrolling(playerid,gettext(1325));
if (PlayerPriv[playerid][CanStartDuels] == 1){
SystemMsgScrolling(playerid,gettext(1462));
SystemMsgScrolling(playerid,gettext(1538));
}
if (PlayerPriv[playerid][CanBanPlayers] == 1) SystemMsgScrolling(playerid,gettext(1490));
if (PlayerPriv[playerid][CanTeleportToFilePos] == 1) SystemMsgScrolling(playerid,gettext(963));
if ((PlayerPriv[playerid][CanChangeWeather] == 1) && (Weather_Change_Enabled == 1)) SystemMsgScrolling(playerid,gettext(1464));
if (PlayerPriv[playerid][CanCrashPlayers] == 1) SystemMsgScrolling(playerid,gettext(1566));
if (PlayerPriv[playerid][CanBuildHouses] == 1) SystemMsgScrolling(playerid,gettext(1656));
if (PlayerPriv[playerid][CanStartHay] == 1){
	SystemMsgScrolling(playerid,gettext(1721));
	SystemMsgScrolling(playerid,gettext(1722));
	SystemMsgScrolling(playerid,gettext(1723));
}
if (PlayerPriv[playerid][CanMapTeleport] == 1) SystemMsgScrolling(playerid,gettext(1565));
if (PlayerPriv[playerid][CanExecFakeCMD] == 1) SystemMsgScrolling(playerid,gettext(1776));
Debug("moderation/commands.inc > Command '/mod help' - Stop");
return 1;
}

if(strcomp(cmd, "worldsave", true) == 1)
{
if (PlayerPriv[playerid][CanWorldSave] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod worldsave' - Start");
new logstring[MAX_STRING];
format(logstring, sizeof (logstring), "Moderator %s requested global save",oGetPlayerName(playerid));
WriteModerLog(logstring);
WorldSave();
Debug("moderation/commands.inc > Command '/mod worldsave' - Stop");
return 1;
}

if(strcomp(cmd, "spysms", true) == 1)
{
if (PlayerPriv[playerid][CanSpySMS] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod spysms' - Start");
new logstring[MAX_STRING];
if (ModSpySMS[playerid] == false){
format(logstring, sizeof (logstring), "Moderator %s now SMS spy",oGetPlayerName(playerid));
ModSpySMS[playerid] = true;
ModMsg(playerid,gettext(1458));
} else {
format(logstring, sizeof (logstring), "Moderator %s now not SMS spy",oGetPlayerName(playerid));
ModSpySMS[playerid] = false;
ModMsg(playerid,gettext(1459));
}
WriteModerLog(logstring);
Debug("moderation/commands.inc > Command '/mod spysms' - Stop");
return 1;
}

if(strcomp(cmd, "announce", true) == 1)
{
if (PlayerPriv[playerid][CanAnnounce] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod announce' - Start");
new string[MAX_STRING];
set(string,text);
strdel(string, 0, idx);
if (strlen(string)  == 0 || strlen(string)  == 1) {Debug("moderation/commands.inc > Command '/mod announce' - Stop"); return ModMsg(playerid,gettext(1153));}
GameTextForAll(string,5000,1);
new logstring[MAX_STRING];
format(logstring, sizeof (logstring), "Moderator %s announced message: '%s'",oGetPlayerName(playerid),string);
WriteModerLog(logstring);
Debug("moderation/commands.inc > Command '/mod announce' - Stop");
return 1;
}

if(strcomp(cmd, "givexp", true) == 1)
{
if (PlayerPriv[playerid][CanGiveXP] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod givexp' - Start");
new receivername[MAX_STRING]; receivername = strcharsplit(text, idx,strchar(" "));
if (isempty(receivername)){ Debug("moderation/commands.inc > Command '/mod givexp' - Stop"); return ModMsg(playerid,gettext(1154));}
new receiverid = FindPlayerID(receivername);
if (receiverid==-1) { Debug("moderation/commands.inc > Command '/mod givexp' - Stop"); return ModMsgFormatStr(playerid,gettext(1571),receivername); }
if (receiverid==-2) { Debug("moderation/commands.inc > Command '/mod givexp' - Stop"); return ModMsgFormatStr(playerid,gettext(1572),receivername); }

if ((receiverid < 0) || (receiverid >= MAX_PLAYERS))
{
ModMsg(playerid,gettext(1192));
Debug("moderation/commands.inc > Command '/mod givexp' - Stop");
return 1;
}

if (IsPlayerConnected(receiverid) == 0)
{
ModMsg(playerid,gettext(1192));
Debug("moderation/commands.inc > Command '/mod givexp' - Stop");
return 1;
}

new xpamount;
xpamount = strval(strcharsplit(text, idx,strchar(" ")));

if ((xpamount < -999999) || (xpamount == 0) || (xpamount > 999999))
{
ModMsg(playerid,gettext(1193));
Debug("moderation/commands.inc > Command '/mod givexp' - Stop");
return 1;
}

GivePlayerXP(receiverid,xpamount,1);

new string[MAX_STRING];

if (xpamount > 0)
{
format(string,sizeof(string),gettext(1194),oGetPlayerName(playerid),xpamount);
SystemMsg(receiverid,COLOUR_XP_GOOD,string);

format(string,sizeof(string),gettext(1195),xpamount,oGetPlayerName(receiverid));
ModMsg(playerid,string);
}
else
{
format(string,sizeof(string),gettext(1196),oGetPlayerName(playerid),xpamount);
SystemMsg(receiverid,COLOUR_XP_BAD,string);

format(string,sizeof(string),gettext(1197),xpamount,oGetPlayerName(receiverid));
ModMsg(playerid,string);
}
new logstring[MAX_STRING];
format(logstring, sizeof (logstring), "Moderator %s get %d XP for %s",oGetPlayerName(playerid),xpamount,oGetPlayerName(receiverid));
WriteModerLog(logstring);
Debug("moderation/commands.inc > Command '/mod givexp' - Stop");
return 1;
}

if(strcomp(cmd, "slap", true) == 1)
{
if (PlayerPriv[playerid][CanSlap] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod slap' - Start");
new receivername[MAX_STRING]; receivername = strcharsplit(text, idx,strchar(" "));
if (isempty(receivername)){ Debug("moderation/commands.inc > Command '/mod slap' - Stop"); return ModMsg(playerid,gettext(1155));}
new receiverid = FindPlayerID(receivername);
if (receiverid==-1) { Debug("moderation/commands.inc > Command '/mod slap' - Stop"); return ModMsgFormatStr(playerid,gettext(1571),receivername); }
if (receiverid==-2) { Debug("moderation/commands.inc > Command '/mod slap' - Stop"); return ModMsgFormatStr(playerid,gettext(1572),receivername); }

if ((receiverid < 0) || (receiverid >= MAX_PLAYERS))
{
ModMsg(playerid,gettext(1198));
Debug("moderation/commands.inc > Command '/mod slap' - Stop");
return 1;
}

if (IsPlayerConnected(receiverid) == 0)
{
ModMsg(playerid,gettext(1198));
Debug("moderation/commands.inc > Command '/mod slap' - Stop");
return 1;
}

new slapamount;
slapamount = strval(strcharsplit(text, idx,strchar(" ")));

if ((slapamount < -1) || (slapamount == 0) || (slapamount > 100))
{
ModMsg(playerid,gettext(1199));
Debug("moderation/commands.inc > Command '/mod slap' - Stop");
return 1;
}

oSetPlayerHealth(receiverid,oGetPlayerHealth(receiverid)-slapamount);

new string[MAX_STRING];

format(string,sizeof(string),gettext(1200),oGetPlayerName(playerid),slapamount);
SystemMsg(receiverid,COLOUR_INFO,string);

format(string,sizeof(string),gettext(1201),slapamount,oGetPlayerName(receiverid));
ModMsg(playerid,string);
new logstring[MAX_STRING];
format(logstring, sizeof (logstring), "Moderator %s slap %d HP for %s",oGetPlayerName(playerid),slapamount,oGetPlayerName(receiverid));
WriteModerLog(logstring);
Debug("moderation/commands.inc > Command '/mod slap' - Stop");
return 1;
}


if(strcomp(cmd, "givemoney", true) == 1)
{
if (PlayerPriv[playerid][CanGiveMoney] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod givemoney' - Start");
new receivername[MAX_STRING]; receivername = strcharsplit(text, idx,strchar(" "));
if (isempty(receivername)){ Debug("moderation/commands.inc > Command '/mod givemoney' - Stop"); return ModMsg(playerid,gettext(1156));}
new receiverid = FindPlayerID(receivername);
if (receiverid==-1) { Debug("moderation/commands.inc > Command '/mod givemoney' - Stop"); return ModMsgFormatStr(playerid,gettext(1571),receivername); }
if (receiverid==-2) { Debug("moderation/commands.inc > Command '/mod givemoney' - Stop"); return ModMsgFormatStr(playerid,gettext(1572),receivername); }

if ((receiverid < 0) || (receiverid >= MAX_PLAYERS))
{
ModMsg(playerid,gettext(1202));
Debug("moderation/commands.inc > Command '/mod givemoney' - Stop");
return 1;
}

if (IsPlayerConnected(receiverid) == 0)
{
ModMsg(playerid,gettext(1202));
Debug("moderation/commands.inc > Command '/mod givemoney' - Stop");
return 1;
}

new moneyamount;
moneyamount = strval(strcharsplit(text, idx,strchar(" ")));

if ((moneyamount < -999999) || (moneyamount == 0) || (moneyamount > MAX_MONEY))
{
ModMsg(playerid,gettext(1203));
return 1;
}

oGivePlayerMoney(receiverid,moneyamount,0);

new string[MAX_STRING];

if (moneyamount > 0)
{
format(string,sizeof(string),gettext(1204),oGetPlayerName(playerid),moneyamount);
SystemMsg(receiverid,COLOUR_MONEY_GOOD,string);

format(string,sizeof(string),gettext(1205),moneyamount,oGetPlayerName(receiverid));
ModMsg(playerid,string);
}
else
{
format(string,sizeof(string),gettext(1206),oGetPlayerName(playerid),moneyamount);
SystemMsg(receiverid,COLOUR_MONEY_BAD,string);

format(string,sizeof(string),gettext(1207),moneyamount,oGetPlayerName(receiverid));
ModMsg(playerid,string);
}

new logstring[MAX_STRING];
format(logstring, sizeof (logstring), "Moderator %s give $%d for %s",oGetPlayerName(playerid),moneyamount,oGetPlayerName(receiverid));
WriteModerLog(logstring);

return 1;
}

if(strcomp(cmd, "setmoney", true) == 1)
{
if (PlayerPriv[playerid][CanSetMoney] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod setmoney' - Start");
new receivername[MAX_STRING]; receivername = strcharsplit(text, idx,strchar(" "));
if (isempty(receivername)){ Debug("moderation/commands.inc > Command '/mod setmoney' - Stop"); return ModMsg(playerid,gettext(1157));}
new receiverid = FindPlayerID(receivername);
if (receiverid==-1) { Debug("moderation/commands.inc > Command '/mod setmoney' - Stop"); return ModMsgFormatStr(playerid,gettext(1571),receivername); }
if (receiverid==-2) { Debug("moderation/commands.inc > Command '/mod setmoney' - Stop"); return ModMsgFormatStr(playerid,gettext(1572),receivername); }

if ((receiverid < 0) || (receiverid >= MAX_PLAYERS))
{
ModMsg(playerid,gettext(1208));
Debug("moderation/commands.inc > Command '/mod setmoney' - Stop");
return 1;
}

if (IsPlayerConnected(receiverid) == 0)
{
ModMsg(playerid,gettext(1208));
Debug("moderation/commands.inc > Command '/mod setmoney' - Stop");
return 1;
}

new moneyamount;
moneyamount = strval(strcharsplit(text, idx,strchar(" ")));

if ((moneyamount < -0) || (moneyamount == 0) || (moneyamount > MAX_MONEY))
{
ModMsg(playerid,gettext(1209));
Debug("moderation/commands.inc > Command '/mod setmoney' - Stop");
return 1;
}

oSetPlayerMoney(receiverid,moneyamount);

new string[MAX_STRING];

format(string,sizeof(string),gettext(1210),moneyamount,oGetPlayerName(receiverid));
SystemMsg(receiverid,COLOUR_INFO,string);

format(string,sizeof(string),gettext(1211),oGetPlayerName(receiverid),moneyamount);
ModMsg(playerid,string);

new logstring[MAX_STRING];
format(logstring, sizeof (logstring), "Moderator %s set money of %s to $%d",oGetPlayerName(playerid),oGetPlayerName(receiverid),moneyamount);
WriteModerLog(logstring);
Debug("moderation/commands.inc > Command '/mod setmoney' - Stop");

return 1;
}

if(strcomp(cmd, "setbank", true) == 1)
{
if (PlayerPriv[playerid][CanSetBank] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod setbank' - Start");
new receivername[MAX_STRING]; receivername = strcharsplit(text, idx,strchar(" "));
if (isempty(receivername)){ Debug("moderation/commands.inc > Command '/mod setbank' - Stop"); return ModMsg(playerid,gettext(1158));}
new receiverid = FindPlayerID(receivername);
if (receiverid==-1) { Debug("moderation/commands.inc > Command '/mod setbank' - Stop"); return ModMsgFormatStr(playerid,gettext(1571),receivername); }
if (receiverid==-2) { Debug("moderation/commands.inc > Command '/mod setbank' - Stop"); return ModMsgFormatStr(playerid,gettext(1572),receivername); }

if ((receiverid < 0) || (receiverid >= MAX_PLAYERS))
{
ModMsg(playerid,gettext(1212));
Debug("moderation/commands.inc > Command '/mod setbank' - Stop");
return 1;
}

if (IsPlayerConnected(receiverid) == 0)
{
ModMsg(playerid,gettext(1212));
Debug("moderation/commands.inc > Command '/mod setbank' - Stop");
return 1;
}

new moneyamount;
moneyamount = strval(strcharsplit(text, idx,strchar(" ")));

if ((moneyamount < -0) || (moneyamount == 0) || (moneyamount > Max_Money_At_Bank))
{
ModMsg(playerid,gettext(1213));
Debug("moderation/commands.inc > Command '/mod setbank' - Stop");
return 1;
}

Player[receiverid][BankMoney]=moneyamount;

new string[MAX_STRING];

format(string,sizeof(string),gettext(1214),moneyamount,oGetPlayerName(receiverid));
SystemMsg(receiverid,COLOUR_INFO,string);

format(string,sizeof(string),gettext(1215),oGetPlayerName(receiverid),moneyamount);
ModMsg(playerid,string);

new logstring[MAX_STRING];
format(logstring, sizeof (logstring), "Moderator %s set bank money of %s to $%d",oGetPlayerName(playerid),oGetPlayerName(receiverid),moneyamount);
WriteModerLog(logstring);
Debug("moderation/commands.inc > Command '/mod setbank' - Stop");
return 1;
}

if(strcomp(cmd, "addcrimes", true) == 1)
{
if (PlayerPriv[playerid][CanAddCrimes] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod addcrimes' - Start");
new receivername[MAX_STRING]; receivername = strcharsplit(text, idx,strchar(" "));
if (isempty(receivername)){ Debug("moderation/commands.inc > Command '/mod addcrimes' - Stop"); return ModMsg(playerid,gettext(1159));}
new receiverid = FindPlayerID(receivername);
if (receiverid==-1) { Debug("moderation/commands.inc > Command '/mod addcrimes' - Stop"); return ModMsgFormatStr(playerid,gettext(1571),receivername); }
if (receiverid==-2) { Debug("moderation/commands.inc > Command '/mod addcrimes' - Stop"); return ModMsgFormatStr(playerid,gettext(1572),receivername); }

if ((receiverid < 0) || (receiverid >= MAX_PLAYERS))
{
ModMsg(playerid,gettext(1216));
Debug("moderation/commands.inc > Command '/mod addcrimes' - Stop");
return 1;
}

if (IsPlayerConnected(receiverid) == 0)
{
ModMsg(playerid,gettext(1216));
Debug("moderation/commands.inc > Command '/mod addcrimes' - Stop");
return 1;
}

new crimesamount;
crimesamount = strval(strcharsplit(text, idx,strchar(" ")));

if ((crimesamount < -99) || (crimesamount == 0) || (crimesamount > 99))
{
ModMsg(playerid,gettext(1217));
return 1;
}

Player[receiverid][Crimes] = Player[receiverid][Crimes]+crimesamount;
CheckCurrentCrimes(receiverid);

new string[MAX_STRING];

if (crimesamount > 0)
{
format(string,sizeof(string),gettext(1218),oGetPlayerName(playerid),crimesamount);
SystemMsg(receiverid,COLOUR_INFO,string);

format(string,sizeof(string),gettext(1219),crimesamount,oGetPlayerName(receiverid));
ModMsg(playerid,string);
}
else
{
format(string,sizeof(string),gettext(1220),oGetPlayerName(playerid),crimesamount);
SystemMsg(receiverid,COLOUR_INFO,string);

format(string,sizeof(string),gettext(1221),crimesamount,oGetPlayerName(receiverid));
ModMsg(playerid,string);
}

new logstring[MAX_STRING];
format(logstring, sizeof (logstring), "Moderator %s give %d crimes for %s",oGetPlayerName(playerid),crimesamount,oGetPlayerName(receiverid));
WriteModerLog(logstring);

return 1;
}

if(strcomp(cmd, "disarm", true) == 1)
{
if (PlayerPriv[playerid][CanDisarm] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod disarm' - Start");
new receivername[MAX_STRING]; receivername = strcharsplit(text, idx,strchar(" "));
if (isempty(receivername)){ Debug("moderation/commands.inc > Command '/mod disarm' - Stop"); return ModMsg(playerid,gettext(1160));}
new receiverid = FindPlayerID(receivername);
if (receiverid==-1) { Debug("moderation/commands.inc > Command '/mod disarm' - Stop"); return ModMsgFormatStr(playerid,gettext(1571),receivername); }
if (receiverid==-2) { Debug("moderation/commands.inc > Command '/mod disarm' - Stop"); return ModMsgFormatStr(playerid,gettext(1572),receivername); }

if ((receiverid < 0) || (receiverid >= MAX_PLAYERS))
{
ModMsg(playerid,gettext(1222));
Debug("moderation/commands.inc > Command '/mod disarm' - Stop");
return 1;
}

if (IsPlayerConnected(receiverid) == 0)
{
ModMsg(playerid,gettext(1222));
Debug("moderation/commands.inc > Command '/mod disarm' - Stop");
return 1;
}

ResetPlayerWeapons(receiverid);

new string[MAX_STRING];
format(string,sizeof(string),gettext(1223),oGetPlayerName(playerid));
SystemMsg(receiverid,COLOUR_INFO,string);
format(string,sizeof(string),gettext(1224),oGetPlayerName(receiverid));
ModMsg(playerid,string);

new logstring[MAX_STRING];
format(logstring, sizeof (logstring), "Moderator %s disarm %s",oGetPlayerName(playerid),oGetPlayerName(receiverid));
WriteModerLog(logstring);
Debug("moderation/commands.inc > Command '/mod disarm' - Stop");

return 1;
}

if(strcomp(cmd, "freeze", true) == 1)
{
if (PlayerPriv[playerid][CanFreeze] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod freeze' - Start");
new receivername[MAX_STRING]; receivername = strcharsplit(text, idx,strchar(" "));
if (isempty(receivername)){ Debug("moderation/commands.inc > Command '/mod freeze' - Stop"); return ModMsg(playerid,gettext(1161));}
new receiverid = FindPlayerID(receivername);
if (receiverid==-1) { Debug("moderation/commands.inc > Command '/mod freeze' - Stop"); return ModMsgFormatStr(playerid,gettext(1571),receivername); }
if (receiverid==-2) { Debug("moderation/commands.inc > Command '/mod freeze' - Stop"); return ModMsgFormatStr(playerid,gettext(1572),receivername); }

if ((receiverid < 0) || (receiverid >= MAX_PLAYERS))
{
ModMsg(playerid,gettext(1225));
Debug("moderation/commands.inc > Command '/mod freeze' - Stop");
return 1;
}

if (IsPlayerConnected(receiverid) == 0)
{
SystemMsg(playerid,COLOUR_INFO,gettext(1225));
Debug("moderation/commands.inc > Command '/mod freeze' - Stop");
return 1;
}

TogglePlayerControllable(receiverid, 0);
PlayerFreezed[receiverid] = true;

new string[MAX_STRING];
format(string,sizeof(string),gettext(1226),oGetPlayerName(receiverid));
ModMsg(playerid,string);

format(string,sizeof(string),gettext(1227),oGetPlayerName(playerid));
SystemMsg(receiverid,COLOUR_INFO,string);

new logstring[MAX_STRING];
format(logstring, sizeof (logstring), "Moderator %s freeze %s",oGetPlayerName(playerid),oGetPlayerName(receiverid));
WriteModerLog(logstring);
Debug("moderation/commands.inc > Command '/mod freeze' - Stop");
return 1;
}

if(strcomp(cmd, "unfreeze", true) == 1)
{
if (PlayerPriv[playerid][CanUnFreeze] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod unfreeze' - Start");
new receivername[MAX_STRING]; receivername = strcharsplit(text, idx,strchar(" "));
if (isempty(receivername)){ Debug("moderation/commands.inc > Command '/mod unfreeze' - Stop"); return ModMsg(playerid,gettext(1162));}
new receiverid = FindPlayerID(receivername);
if (receiverid==-1) { Debug("moderation/commands.inc > Command '/mod unfreeze' - Stop"); return ModMsgFormatStr(playerid,gettext(1571),receivername); }
if (receiverid==-2) { Debug("moderation/commands.inc > Command '/mod unfreeze' - Stop"); return ModMsgFormatStr(playerid,gettext(1572),receivername); }

if ((receiverid < 0) || (receiverid >= MAX_PLAYERS))
{
ModMsg(playerid,gettext(1228));
Debug("moderation/commands.inc > Command '/mod unfreeze' - Stop");
return 1;
}

if (IsPlayerConnected(receiverid) == 0)
{
ModMsg(playerid,gettext(1228));
Debug("moderation/commands.inc > Command '/mod unfreeze' - Stop");
return 1;
}

TogglePlayerControllable(receiverid, 1);
PlayerFreezed[receiverid] = false;

new string[MAX_STRING];
format(string,sizeof(string),gettext(1229),oGetPlayerName(receiverid));
ModMsg(playerid,string);

format(string,sizeof(string),gettext(1230),oGetPlayerName(playerid));
SystemMsg(receiverid,COLOUR_INFO,string);

new logstring[MAX_STRING];
format(logstring, sizeof (logstring), "Moderator %s unfreeze %s",oGetPlayerName(playerid),oGetPlayerName(receiverid));
WriteModerLog(logstring);
Debug("moderation/commands.inc > Command '/mod unfreeze' - Stop");

return 1;
}

if(strcomp(cmd, "heal", true) == 1)
{
if (PlayerPriv[playerid][CanHeal] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod heal' - Start");
new receivername[MAX_STRING]; receivername = strcharsplit(text, idx,strchar(" "));
if (isempty(receivername)){ Debug("moderation/commands.inc > Command '/mod heal' - Stop"); return ModMsg(playerid,gettext(1163));}
new receiverid = FindPlayerID(receivername);
if (receiverid==-1) { Debug("moderation/commands.inc > Command '/mod heal' - Stop"); return ModMsgFormatStr(playerid,gettext(1571),receivername); }
if (receiverid==-2) { Debug("moderation/commands.inc > Command '/mod heal' - Stop"); return ModMsgFormatStr(playerid,gettext(1572),receivername); }

if ((receiverid < 0) || (receiverid >= MAX_PLAYERS))
{
ModMsg(playerid,gettext(1231));
Debug("moderation/commands.inc > Command '/mod heal' - Stop");
return 1;
}

if (IsPlayerConnected(receiverid) == 0)
{
ModMsg(playerid,gettext(1231));
Debug("moderation/commands.inc > Command '/mod heal' - Stop");
return 1;
}

oSetPlayerHealth(receiverid,GetMaxHealth(receiverid));

new string[MAX_STRING];
format(string,sizeof(string),gettext(1232),oGetPlayerName(receiverid));
ModMsg(playerid,string);

format(string,sizeof(string),gettext(1233),oGetPlayerName(playerid));
SystemMsg(receiverid,COLOUR_INFO,string);

new logstring[MAX_STRING];
format(logstring, sizeof (logstring), "Moderator %s heal %s",oGetPlayerName(playerid),oGetPlayerName(receiverid));
WriteModerLog(logstring);
Debug("moderation/commands.inc > Command '/mod heal' - Stop");
return 1;
}

if(strcomp(cmd, "god", true) == 1)
{
if (PlayerPriv[playerid][CanGod] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod god' - Start");
new receivername[MAX_STRING]; receivername = strcharsplit(text, idx,strchar(" "));
if (isempty(receivername)){ Debug("moderation/commands.inc > Command '/mod god' - Stop"); return ModMsg(playerid,gettext(1164));}
new receiverid = FindPlayerID(receivername);
if (receiverid==-1) { Debug("moderation/commands.inc > Command '/mod god' - Stop"); return ModMsgFormatStr(playerid,gettext(1571),receivername); }
if (receiverid==-2) { Debug("moderation/commands.inc > Command '/mod god' - Stop"); return ModMsgFormatStr(playerid,gettext(1572),receivername); }

if ((receiverid < 0) || (receiverid >= MAX_PLAYERS))
{
ModMsg(playerid,gettext(1234));
Debug("moderation/commands.inc > Command '/mod god' - Stop");
return 1;
}

if (IsPlayerConnected(receiverid) == 0)
{
ModMsg(playerid,gettext(1234));
Debug("moderation/commands.inc > Command '/mod god' - Stop");
return 1;
}

new logstring[MAX_STRING];
new string[MAX_STRING];
SetPlayerHealth(receiverid, INFINITY);
format(string,sizeof(string),gettext(1235),oGetPlayerName(receiverid));
ModMsg(playerid,string);
format(string,sizeof(string),gettext(1236),oGetPlayerName(playerid));
SystemMsg(receiverid,COLOUR_INFO,string);
format(logstring, sizeof (logstring), "Moderator %s make god %s",oGetPlayerName(playerid),oGetPlayerName(receiverid));
WriteModerLog(logstring);
Debug("moderation/commands.inc > Command '/mod god' - Stop");
return 1;
}

if(strcomp(cmd, "antifall", true) == 1)
{
if (PlayerPriv[playerid][CanSetAntiBikeFall] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod antifall' - Start");
new receivername[MAX_STRING]; receivername = strcharsplit(text, idx,strchar(" "));
if (isempty(receivername)){ Debug("moderation/commands.inc > Command '/mod antifall' - Stop"); return ModMsg(playerid,gettext(1468));}
new receiverid = FindPlayerID(receivername);
if (receiverid==-1) { Debug("moderation/commands.inc > Command '/mod antifall' - Stop"); return ModMsgFormatStr(playerid,gettext(1571),receivername); }
if (receiverid==-2) { Debug("moderation/commands.inc > Command '/mod antifall' - Stop"); return ModMsgFormatStr(playerid,gettext(1572),receivername); }

if (IsPlayerConnected(receiverid) == 0)
{
ModMsg(playerid,gettext(1468));
Debug("moderation/commands.inc > Command '/mod antifall' - Stop");
return 1;
}

new logstring[MAX_STRING];
new string[MAX_STRING];
if (antifall[receiverid] == false){
antifall[receiverid] = true;
cmdoff[receiverid] = false;
format(string,sizeof(string),gettext(1469),oGetPlayerName(receiverid));
ModMsg(playerid,string);
format(string,sizeof(string),gettext(1470),oGetPlayerName(playerid));
SystemMsg(receiverid,COLOUR_INFO,string);
format(logstring, sizeof (logstring), "Moderator %s turn on anti-fall for %s",oGetPlayerName(playerid),oGetPlayerName(receiverid));
} else {
antifall[receiverid] = false;
cmdoff[receiverid] = false;
format(string,sizeof(string),gettext(1471),oGetPlayerName(receiverid));
ModMsg(playerid,string);
format(string,sizeof(string),gettext(1472),oGetPlayerName(playerid));
SystemMsg(receiverid,COLOUR_INFO,string);
format(logstring, sizeof (logstring), "Moderator %s turn off anti-fall for %s",oGetPlayerName(playerid),oGetPlayerName(receiverid));
}

WriteModerLog(logstring);
Debug("moderation/commands.inc > Command '/mod antifall' - Stop");
return 1;
}

if(strcomp(cmd, "murder", true) == 1)
{
if (PlayerPriv[playerid][CanMurder] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod murder' - Start");
new receivername[MAX_STRING]; receivername = strcharsplit(text, idx,strchar(" "));
if (isempty(receivername)){ Debug("moderation/commands.inc > Command '/mod murder' - Stop"); return ModMsg(playerid,gettext(1165));}
new receiverid = FindPlayerID(receivername);
if (receiverid==-1) { Debug("moderation/commands.inc > Command '/mod murder' - Stop"); return ModMsgFormatStr(playerid,gettext(1571),receivername); }
if (receiverid==-2) { Debug("moderation/commands.inc > Command '/mod murder' - Stop"); return ModMsgFormatStr(playerid,gettext(1572),receivername); }

if ((receiverid < 0) || (receiverid >= MAX_PLAYERS))
{
Debug("moderation/commands.inc > Command '/mod murder' - Stop");
ModMsg(playerid,gettext(1237));
return 1;
}

if (IsPlayerConnected(receiverid) == 0)
{
Debug("moderation/commands.inc > Command '/mod murder' - Stop");
ModMsg(playerid,gettext(1237));
return 1;
}

oSetPlayerHealth(receiverid,0);

new string[MAX_STRING];
format(string,sizeof(string),gettext(1238),oGetPlayerName(receiverid));
ModMsg(playerid,string);

format(string,sizeof(string),gettext(1239),oGetPlayerName(playerid));
SystemMsg(receiverid,COLOUR_INFO,string);

new logstring[MAX_STRING];
format(logstring, sizeof (logstring), "Moderator %s murder %s",oGetPlayerName(playerid),oGetPlayerName(receiverid));
WriteModerLog(logstring);
Debug("moderation/commands.inc > Command '/mod murder' - Stop");
return 1;
}

if(strcomp(cmd, "smurder", true) == 1)
{
if (PlayerPriv[playerid][CanSilentMurder] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod smurder' - Start");
new receivername[MAX_STRING]; receivername = strcharsplit(text, idx,strchar(" "));
if (isempty(receivername)){ Debug("moderation/commands.inc > Command '/mod smurder' - Stop"); return ModMsg(playerid,gettext(1166));}
new receiverid = FindPlayerID(receivername);
if (receiverid==-1) { Debug("moderation/commands.inc > Command '/mod smurder' - Stop"); return ModMsgFormatStr(playerid,gettext(1571),receivername); }
if (receiverid==-2) { Debug("moderation/commands.inc > Command '/mod smurder' - Stop"); return ModMsgFormatStr(playerid,gettext(1572),receivername); }

if ((receiverid < 0) || (receiverid >= MAX_PLAYERS))
{
ModMsg(playerid,gettext(1240));
Debug("moderation/commands.inc > Command '/mod smurder' - Stop");
return 1;
}

if (IsPlayerConnected(receiverid) == 0)
{
ModMsg(playerid,gettext(1240));
Debug("moderation/commands.inc > Command '/mod smurder' - Stop");
return 1;
}

oSetPlayerHealth(receiverid,0);

new string[MAX_STRING];
format(string,sizeof(string),gettext(1241),oGetPlayerName(receiverid));
ModMsg(playerid,string);

new logstring[MAX_STRING];
format(logstring, sizeof (logstring), "Moderator %s murder %s",oGetPlayerName(playerid),oGetPlayerName(receiverid));
WriteModerLog(logstring);
Debug("moderation/commands.inc > Command '/mod smurder' - Stop");
return 1;
}

if(strcomp(cmd, "crash", true) == 1)
{
if (PlayerPriv[playerid][CanCrashPlayers] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod crash' - Start");
new receivername[MAX_STRING]; receivername = strcharsplit(text, idx,strchar(" "));
if (isempty(receivername)){ Debug("moderation/commands.inc > Command '/mod crash' - Stop"); return ModMsg(playerid,gettext(1566));}
new receiverid = FindPlayerID(receivername);
if (receiverid==-1) { Debug("moderation/commands.inc > Command '/mod crash' - Stop"); return ModMsgFormatStr(playerid,gettext(1571),receivername); }
if (receiverid==-2) { Debug("moderation/commands.inc > Command '/mod crash' - Stop"); return ModMsgFormatStr(playerid,gettext(1572),receivername); }

if ((receiverid < 0) || (receiverid >= MAX_PLAYERS))
{
ModMsg(playerid,gettext(1567));
Debug("moderation/commands.inc > Command '/mod crash' - Stop");
return 1;
}

if (IsPlayerConnected(receiverid) == 0)
{
ModMsg(playerid,gettext(1567));
Debug("moderation/commands.inc > Command '/mod crash' - Stop");
return 1;
}

new objectcrash = CreatePlayerObject(receiverid,11111111,PlayerPos[receiverid][Coord_X],PlayerPos[receiverid][Coord_Y],PlayerPos[receiverid][Coord_Z],0,0,0);
DestroyPlayerObject(receiverid,objectcrash);

new string[MAX_STRING];
format(string,sizeof(string),gettext(1568),oGetPlayerName(receiverid));
ModMsg(playerid,string);

new logstring[MAX_STRING];
format(logstring, sizeof (logstring), "Moderator %s murder %s",oGetPlayerName(playerid),oGetPlayerName(receiverid));
WriteModerLog(logstring);
Debug("moderation/commands.inc > Command '/mod crash' - Stop");
return 1;
}

if(strcomp(cmd, "mark", true) == 1)
{
if (PlayerPriv[playerid][CanMark] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod mark' - Start");
new receivername[MAX_STRING]; receivername = strcharsplit(text, idx,strchar(" "));
if (isempty(receivername)){ Debug("moderation/commands.inc > Command '/mod mark' - Stop"); return ModMsg(playerid,gettext(1191));}
new receiverid = FindPlayerID(receivername);
if (receiverid==-1) { Debug("moderation/commands.inc > Command '/mod mark' - Stop"); return ModMsgFormatStr(playerid,gettext(1571),receivername); }
if (receiverid==-2) { Debug("moderation/commands.inc > Command '/mod mark' - Stop"); return ModMsgFormatStr(playerid,gettext(1572),receivername); }

if ((receiverid < 0) || (receiverid >= MAX_PLAYERS))
{
Debug("moderation/commands.inc > Command '/mod mark' - Stop");
SetPlayerMarkerForPlayer(playerid,MarkedPlayer[playerid],GetPlayerColor(MarkedPlayer[playerid]));
SystemMsg(playerid,COLOUR_INFO,gettext(1242));
return 1;
}

if (IsPlayerConnected(receiverid) == 0)
{
Debug("moderation/commands.inc > Command '/mod mark' - Stop");
ModMsg(playerid,gettext(1243));
return 1;
}

SetPlayerMarkerForPlayer(playerid,receiverid,0x00FFFFAA);
MarkedPlayer[playerid] = receiverid;

new string[MAX_STRING];
format(string,sizeof(string),gettext(1244),oGetPlayerName(receiverid));
ModMsg(playerid,string);

new logstring[MAX_STRING];
format(logstring, sizeof (logstring), "Moderator %s mark %s",oGetPlayerName(playerid),oGetPlayerName(receiverid));
WriteModerLog(logstring);
Debug("moderation/commands.inc > Command '/mod murder' - Stop");
return 1;
}

if(strcomp(cmd, "jail", true) == 1)
{
if (PlayerPriv[playerid][CanJail] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod jail' - Start");
new receivername[MAX_STRING]; receivername = strcharsplit(text, idx,strchar(" "));
if (isempty(receivername)){ Debug("moderation/commands.inc > Command '/mod jail' - Stop"); return ModMsg(playerid,gettext(1167));}
new receiverid = FindPlayerID(receivername);
if (receiverid==-1) { Debug("moderation/commands.inc > Command '/mod jail' - Stop"); return ModMsgFormatStr(playerid,gettext(1571),receivername); }
if (receiverid==-2) { Debug("moderation/commands.inc > Command '/mod jail' - Stop"); return ModMsgFormatStr(playerid,gettext(1572),receivername); }

if ((receiverid < 0) || (receiverid >= MAX_PLAYERS))
{
ModMsg(playerid,gettext(1245));
Debug("moderation/commands.inc > Command '/mod jail' - Stop");
return 1;
}

if (IsPlayerConnected(receiverid) == 0)
{
ModMsg(playerid,gettext(1245));
Debug("moderation/commands.inc > Command '/mod jail' - Stop");
return 1;
}

new jailtime;
jailtime = strval(strcharsplit(text, idx,strchar(" ")));

if ((jailtime < -999999) || (jailtime == 0) || (jailtime > 999999))
{
ModMsg(playerid,gettext(1246));
Debug("moderation/commands.inc > Command '/mod givexp' - Stop");
return 1;
}

JailPlayer(receiverid,jailtime,gettext(1247));

new string[MAX_STRING];
format(string,sizeof(string),gettext(1248),oGetPlayerName(receiverid),ConvertSeconds(jailtime));
ModMsg(playerid,string);

new logstring[MAX_STRING];
format(logstring, sizeof (logstring), "Moderator %s jail player %s",oGetPlayerName(playerid),oGetPlayerName(receiverid));
WriteModerLog(logstring);
Debug("moderation/commands.inc > Command '/mod jail' - Stop");
return 1;
}

if(strcomp(cmd, "unjail", true) == 1)
{
if (PlayerPriv[playerid][CanUnjail] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod unjail' - Start");
new receivername[MAX_STRING]; receivername = strcharsplit(text, idx,strchar(" "));
if (isempty(receivername)){ Debug("moderation/commands.inc > Command '/mod unjail' - Stop"); return ModMsg(playerid,gettext(1168));}
new receiverid = FindPlayerID(receivername);
if (receiverid==-1) { Debug("moderation/commands.inc > Command '/mod unjail' - Stop"); return ModMsgFormatStr(playerid,gettext(1571),receivername); }
if (receiverid==-2) { Debug("moderation/commands.inc > Command '/mod unjail' - Stop"); return ModMsgFormatStr(playerid,gettext(1572),receivername); }

if ((receiverid < 0) || (receiverid >= MAX_PLAYERS))
{
ModMsg(playerid,gettext(1249));
Debug("moderation/commands.inc > Command '/mod unjail' - Stop");
return 1;
}

if (IsPlayerConnected(receiverid) == 0)
{
ModMsg(playerid,gettext(1249));
Debug("moderation/commands.inc > Command '/mod unjail' - Stop");
return 1;
}

new string[MAX_STRING];
format(string,sizeof(string),gettext(1250),oGetPlayerName(playerid));
SystemMsg(receiverid,COLOUR_INFO,string);

UnJailPlayer(receiverid);

format(string,sizeof(string),gettext(1251),oGetPlayerName(receiverid));
ModMsg(playerid,string);

new logstring[MAX_STRING];
format(logstring, sizeof (logstring), "Moderator %s unjail player %s",oGetPlayerName(playerid),oGetPlayerName(receiverid));
WriteModerLog(logstring);
Debug("moderation/commands.inc > Command '/mod unjail' - Stop");
return 1;
}

if(strcomp(cmd, "jaillist", true) == 1)
{
if (PlayerPriv[playerid][CanJailList] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod jaillist' - Start");
new count = 0;
for (new pid=0;pid<MAX_PLAYERS_EX;pid++)
	{
		if (IsPlayerConnected(pid))
		{
			if (Player[pid][JailTime] > 0){
			count++;
			new string[MAX_STRING];
			format(string,sizeof(string),gettext(1252),oGetPlayerName(pid),ConvertSeconds(Player[pid][JailTime]));
			SystemMsgScrolling(playerid,string);
			}
		}
	}
if (count == 0){
ModMsg(playerid,gettext(1253));
}
new logstring[MAX_STRING];
format(logstring, sizeof (logstring), "Moderator %s request jailed list",oGetPlayerName(playerid));
WriteModerLog(logstring);
Debug("moderation/commands.inc > Command '/mod jaillist' - Stop");
return 1;
}

if(strcomp(cmd, "mute", true) == 1)
{
if (PlayerPriv[playerid][CanMute] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod mute' - Start");
new receivername[MAX_STRING]; receivername = strcharsplit(text, idx,strchar(" "));
if (isempty(receivername)){ Debug("moderation/commands.inc > Command '/mod mute' - Stop"); return ModMsg(playerid,gettext(1170));}
new receiverid = FindPlayerID(receivername);
if (receiverid==-1) { Debug("moderation/commands.inc > Command '/mod mute' - Stop"); return ModMsgFormatStr(playerid,gettext(1571),receivername); }
if (receiverid==-2) { Debug("moderation/commands.inc > Command '/mod mute' - Stop"); return ModMsgFormatStr(playerid,gettext(1572),receivername); }

if ((receiverid < 0) || (receiverid >= MAX_PLAYERS))
{
ModMsg(playerid,gettext(1254));
Debug("moderation/commands.inc > Command '/mod mute' - Stop");
return 1;
}

if (IsPlayerConnected(receiverid) == 0)
{
ModMsg(playerid,gettext(1254));
Debug("moderation/commands.inc > Command '/mod mute' - Stop");
return 1;
}

new mutetime;
mutetime = strval(strcharsplit(text, idx,strchar(" ")));

if ((mutetime < -99999999) || (mutetime == 0) || (mutetime > 99999999))
{
ModMsg(playerid,gettext(1255));
Debug("moderation/commands.inc > Command '/mod givexp' - Stop");
return 1;
}

Player[receiverid][MuteTime] = Player[receiverid][MuteTime] + mutetime;

new string[MAX_STRING];
format(string,sizeof(string),gettext(1256),oGetPlayerName(receiverid),ConvertSeconds(mutetime));
ModMsg(playerid,string);
format(string,sizeof(string),gettext(1257),ConvertSeconds(mutetime),oGetPlayerName(playerid));
SystemMsg(receiverid,COLOUR_INFO,string);

new logstring[MAX_STRING];
format(logstring, sizeof (logstring), "Moderator %s mute player %s",oGetPlayerName(playerid),oGetPlayerName(receiverid));
WriteModerLog(logstring);
Debug("moderation/commands.inc > Command '/mod mute' - Stop");
return 1;
}

if(strcomp(cmd, "mutelist", true) == 1)
{
if (PlayerPriv[playerid][CanMuteList] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod mutelist' - Start");
new count = 0;
for (new pid=0;pid<MAX_PLAYERS_EX;pid++)
	{
		if (IsPlayerConnected(pid))
		{
			if (Player[pid][MuteTime] > 0){
			count++;
			new string[MAX_STRING];
			format(string,sizeof(string),gettext(1258),oGetPlayerName(pid),ConvertSeconds(Player[pid][MuteTime]));
			SystemMsgScrolling(playerid,string);
			}
		}
	}
if (count == 0){
ModMsg(playerid,gettext(1259));
}
new logstring[MAX_STRING];
format(logstring, sizeof (logstring), "Moderator %s request muted list",oGetPlayerName(playerid));
WriteModerLog(logstring);
Debug("moderation/commands.inc > Command '/mod mutelist' - Stop");
return 1;
}

if(strcomp(cmd, "unmute", true) == 1)
{
if (PlayerPriv[playerid][CanUnmute] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod unmute' - Start");
new receivername[MAX_STRING]; receivername = strcharsplit(text, idx,strchar(" "));
if (isempty(receivername)){ Debug("moderation/commands.inc > Command '/mod unmute' - Stop"); return ModMsg(playerid,gettext(1172));}
new receiverid = FindPlayerID(receivername);
if (receiverid==-1) { Debug("moderation/commands.inc > Command '/mod unmute' - Stop"); return ModMsgFormatStr(playerid,gettext(1571),receivername); }
if (receiverid==-2) { Debug("moderation/commands.inc > Command '/mod unmute' - Stop"); return ModMsgFormatStr(playerid,gettext(1572),receivername); }

if ((receiverid < 0) || (receiverid >= MAX_PLAYERS))
{
ModMsg(playerid,gettext(1260));
Debug("moderation/commands.inc > Command '/mod unmute' - Stop");
return 1;
}

if (IsPlayerConnected(receiverid) == 0)
{
ModMsg(playerid,gettext(1260));
Debug("moderation/commands.inc > Command '/mod unmute' - Stop");
return 1;
}

Player[receiverid][MuteTime] = 0;

new string[MAX_STRING];
format(string,sizeof(string),gettext(1261),oGetPlayerName(receiverid));
ModMsg(playerid,string);
format(string,sizeof(string),gettext(1262),oGetPlayerName(playerid));
SystemMsg(receiverid,COLOUR_INFO,string);

new logstring[MAX_STRING];
format(logstring, sizeof (logstring), "Moderator %s unmute player %s",oGetPlayerName(playerid),oGetPlayerName(receiverid));
WriteModerLog(logstring);
Debug("moderation/commands.inc > Command '/mod unmute' - Stop");
return 1;
}

if(strcomp(cmd, "warn", true) == 1)
{
if (PlayerPriv[playerid][CanWarn] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod warn' - Start");
new receivername[MAX_STRING]; receivername = strcharsplit(text, idx,strchar(" "));
if (isempty(receivername)){ Debug("moderation/commands.inc > Command '/mod warn' - Stop"); return ModMsg(playerid,gettext(1173));}
new receiverid = FindPlayerID(receivername);
if (receiverid==-1) { Debug("moderation/commands.inc > Command '/mod warn' - Stop"); return ModMsgFormatStr(playerid,gettext(1571),receivername); }
if (receiverid==-2) { Debug("moderation/commands.inc > Command '/mod warn' - Stop"); return ModMsgFormatStr(playerid,gettext(1572),receivername); }

if ((receiverid < 0) || (receiverid >= MAX_PLAYERS))
{
ModMsg(playerid,gettext(1263));
Debug("moderation/commands.inc > Command '/mod warn' - Stop");
return 1;
}

if (!IsPlayerConnected(receiverid))
{
ModMsg(playerid,gettext(1263));
Debug("moderation/commands.inc > Command '/mod warn' - Stop");
return 1;
}

new string[MAX_STRING];
format(string,sizeof(string),gettext(1264),oGetPlayerName(playerid),Player[receiverid][Warns],Warns_For_Ban-Player[receiverid][Warns]);
SystemMsg(receiverid,COLOUR_INFO,string);
format(string,sizeof(string),gettext(1265),oGetPlayerName(receiverid),Player[receiverid][Warns],Warns_For_Ban-Player[receiverid][Warns]);
ModMsg(playerid,string);


Player[receiverid][Warns]++;
CheckWarns(receiverid);

new logstring[MAX_STRING];
format(logstring, sizeof (logstring), "Moderator %s warn player %s",oGetPlayerName(playerid),oGetPlayerName(receiverid));
WriteModerLog(logstring);
Debug("moderation/commands.inc > Command '/mod warn' - Stop");
return 1;
}

if(strcomp(cmd, "unwarn", true) == 1)
{
if (PlayerPriv[playerid][CanUnwarn] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod unwarn' - Start");
new receivername[MAX_STRING]; receivername = strcharsplit(text, idx,strchar(" "));
if (isempty(receivername)){ Debug("moderation/commands.inc > Command '/mod unwarn' - Stop"); return ModMsg(playerid,gettext(1174));}
new receiverid = FindPlayerID(receivername);
if (receiverid==-1) { Debug("moderation/commands.inc > Command '/mod unwarn' - Stop"); return ModMsgFormatStr(playerid,gettext(1571),receivername); }
if (receiverid==-2) { Debug("moderation/commands.inc > Command '/mod unwarn' - Stop"); return ModMsgFormatStr(playerid,gettext(1572),receivername); }

if ((receiverid < 0) || (receiverid >= MAX_PLAYERS))
{
ModMsg(playerid,gettext(1266));
Debug("moderation/commands.inc > Command '/mod unwarn' - Stop");
return 1;
}

if (IsPlayerConnected(receiverid) == 0)
{
ModMsg(playerid,gettext(1266));
Debug("moderation/commands.inc > Command '/mod unwarn' - Stop");
return 1;
}


Player[receiverid][Warns]--;

new string[MAX_STRING];
format(string,sizeof(string),gettext(1267),oGetPlayerName(playerid),Player[receiverid][Warns],Warns_For_Ban-Player[receiverid][Warns]);
SystemMsg(receiverid,COLOUR_INFO,string);
format(string,sizeof(string),gettext(1268),oGetPlayerName(receiverid),Player[receiverid][Warns],Warns_For_Ban-Player[receiverid][Warns]);
SystemMsg(receiverid,COLOUR_INFO,string);

new logstring[MAX_STRING];
format(logstring, sizeof (logstring), "Moderator %s unwarn player %s",oGetPlayerName(playerid),oGetPlayerName(receiverid));
WriteModerLog(logstring);
Debug("moderation/commands.inc > Command '/mod unwarn' - Stop");
return 1;
}

if(strcomp(cmd, "lockacc", true) == 1)
{
if (PlayerPriv[playerid][CanLockAcc] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod lockacc' - Start");
new accfile[MAX_STRING];
new dinifilename[MAX_STRING];
new message[MAX_STRING];
set(accfile,text);
strdel(accfile, 0, idx);
new logstring[MAX_STRING];
format(dinifilename,sizeof(dinifilename),"%s%s.txt",Account_DB,EncodeName(accfile));
if (db_Exists(dinifilename)){
db_Set(dinifilename,"Locked",ovalstr(1));
format(message, sizeof (message),gettext(1269),accfile);
ModMsg(playerid,message);
format(logstring, sizeof (logstring), "Moderator %s close account %s",oGetPlayerName(playerid),accfile);
WriteModerLog(logstring);
} else {
format(message, sizeof (message),gettext(1270),accfile);
ModMsg(playerid,message);
}
Debug("moderation/commands.inc > Command '/mod lockacc' - Stop");
return 1;
}

if(strcomp(cmd, "unlockacc", true) == 1)
{
if (PlayerPriv[playerid][CanUnlockAcc] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod unlockacc' - Start");
new accfile[MAX_STRING];
new dinifilename[MAX_STRING];
new message[MAX_STRING];
set(accfile,text);
strdel(accfile, 0, idx);
new logstring[MAX_STRING];
format(dinifilename,sizeof(dinifilename),"%s%s.txt",Account_DB,EncodeName(accfile));
if (db_Exists(dinifilename)){
db_Set(dinifilename,"Locked",ovalstr(0));
format(message, sizeof (message),gettext(1271),accfile);
ModMsg(playerid,message);
format(logstring, sizeof (logstring), "Moderator %s open account %s",oGetPlayerName(playerid),accfile);
WriteModerLog(logstring);
} else {
format(message, sizeof (message),gettext(1270),accfile);
ModMsg(playerid,message);
}
Debug("moderation/commands.inc > Command '/mod unlockacc' - Stop");
return 1;
}

if(strcomp(cmd, "makeitems", true) == 1)
{
Debug("moderation/commands.inc > Command '/mod makeitems' - Start");
MakeItems();
ModMsg(playerid,gettext(1272));
new logstring[MAX_STRING];
format(logstring, sizeof (logstring), "Moderator %s make items not in time.",oGetPlayerName(playerid));
WriteModerLog(logstring);
Debug("moderation/commands.inc > Command '/mod makeitems' - Stop");
return 1;
}

if(strcomp(cmd, "pinfo", true) == 1)
{
if (PlayerPriv[playerid][CanSeePlayersInfo] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod pinfo' - Start");
new receivername[MAX_STRING]; receivername = strcharsplit(text, idx,strchar(" "));
if (isempty(receivername)){ Debug("moderation/commands.inc > Command '/mod pinfo' - Stop"); return ModMsg(playerid,gettext(1178));}
new receiverid = FindPlayerID(receivername);
if (receiverid==-1) { Debug("moderation/commands.inc > Command '/mod pinfo' - Stop"); return ModMsgFormatStr(playerid,gettext(1571),receivername); }
if (receiverid==-2) { Debug("moderation/commands.inc > Command '/mod pinfo' - Stop"); return ModMsgFormatStr(playerid,gettext(1572),receivername); }

if ((receiverid < 0) || (receiverid >= MAX_PLAYERS))
{
ModMsg(playerid,gettext(1273));
Debug("moderation/commands.inc > Command '/mod pinfo' - Stop");
return 1;
}

if (IsPlayerConnected(receiverid) == 0)
{
ModMsg(playerid,gettext(1273));
Debug("moderation/commands.inc > Command '/mod pinfo' - Stop");
return 1;
}
new playerinfoip[IP_LEN]; playerinfoip = ReturnPlayerIp(receiverid);
new string[MAX_STRING];
format(string,MAX_STRING,gettext(1274),oGetPlayerName(receiverid),PlayerEmail[receiverid],playerinfoip,Player[receiverid][Health]);
ModMsg(playerid,string);
format(string,MAX_STRING,gettext(1275),oGetPlayerMoney(receiverid),Player[receiverid][BankMoney]);
ModMsg(playerid,string);
format(string,MAX_STRING,gettext(1276),Player[receiverid][XP],Player[receiverid][Level]);
ModMsg(playerid,string);
if (PlayerGangid[receiverid] != 0){
format(string,MAX_STRING,gettext(1277),PlayerGangName[receiverid],PlayerGangTag[receiverid]);
ModMsg(playerid,string);
}
format(string,MAX_STRING,gettext(1278),GetXYZZoneName(PlayerPos[receiverid][Coord_X],PlayerPos[receiverid][Coord_Y],PlayerPos[receiverid][Coord_Z]),GetPlayerVirtualWorld(receiverid),GetPlayerInterior(receiverid));
ModMsg(playerid,string);
new logstring[MAX_STRING];
format(logstring, sizeof (logstring), "Moderator %s looking for %s information (ID: %d).",oGetPlayerName(playerid),oGetPlayerName(receiverid),receiverid);
WriteModerLog(logstring);
Debug("moderation/commands.inc > Command '/mod pinfo' - Stop");
return 1;
}

if(strcomp(cmd, "weather", true) == 1)
{
if ((PlayerPriv[playerid][CanChangeWeather] == 0) || (Weather_Change_Enabled == 0)) return 0;
Debug("moderation/commands.inc > Command '/mod weather' - Start");
new tmp[MAX_STRING];
tmp = strcharsplit(text, idx,strchar(" "));
new weather;
if (isempty(tmp)){ 
	weather = random(50);
} else {
	weather = strval(tmp);
}

if ((weather < 0) || (weather >= 50))
{
ModMsg(playerid,gettext(349));
Debug("moderation/commands.inc > Command '/mod weather' - Stop");
return 1;
}

SetWeather(weather);
ModMsg(playerid,gettext(351));
new logstring[MAX_STRING];
format(logstring, sizeof (logstring), "Moderator %s change weather to %d",oGetPlayerName(playerid),weather);
WriteModerLog(logstring);
Debug("moderation/commands.inc > Command '/mod weather' - Stop");
return 1;
}

if(strcomp(cmd, "tpto", true) == 1)
{
	if (PlayerPriv[playerid][CanTeleportToPlayers] == 0) return 0;
	Debug("moderation/commands.inc > Command '/mod tpto' - Start");
	new receivername[MAX_STRING]; receivername = strcharsplit(text, idx,strchar(" "));
	if (isempty(receivername)){ Debug("moderation/commands.inc > Command '/mod tpto' - Stop"); return ModMsg(playerid,gettext(1185));}
	new receiverid = FindPlayerID(receivername);
	if (receiverid==-1) { Debug("moderation/commands.inc > Command '/mod tpto' - Stop"); return ModMsgFormatStr(playerid,gettext(1571),receivername); }
	if (receiverid==-2) { Debug("moderation/commands.inc > Command '/mod tpto' - Stop"); return ModMsgFormatStr(playerid,gettext(1572),receivername); }

	if (!IsPlayerConnected(receiverid)){
		ModMsg(playerid,gettext(1280));
		Debug("moderation/commands.inc > Command '/mod tpto' - Stop");
		return 1;
	}

	if (receiverid == playerid){
		ModMsg(playerid,gettext(1281));
		Debug("moderation/commands.inc > Command '/mod tpto' - Stop");
		return 1;
	}

	if (Spec[playerid][Spectating] == true){
		ModMsg(playerid,gettext(1569));
		Debug("moderation/commands.inc > Command '/mod tpto' - Stop");
		return 1;
	}

	if (Spec[receiverid][Spectating] == true){
		ModMsg(playerid,gettext(1570));
		Debug("moderation/commands.inc > Command '/mod tpto' - Stop");
		return 1;
	}

	SetPlayerInterior(playerid,GetPlayerInterior(receiverid));
	oSetPlayerPos(playerid,PlayerPos[receiverid][Coord_X],PlayerPos[receiverid][Coord_Y],PlayerPos[receiverid][Coord_Z]);
	SetPlayerVirtualWorld(playerid,GetPlayerVirtualWorld(receiverid));
	SetCameraBehindPlayer(playerid);

	new string[MAX_STRING];
	format(string,sizeof(string),gettext(1282),oGetPlayerName(receiverid));
	ModMsg(playerid,string);

	new logstring[MAX_STRING];
	format(logstring, sizeof (logstring), "Moderator %s teleported to player %s (ID: %d).",oGetPlayerName(playerid),oGetPlayerName(receiverid),receiverid);
	WriteModerLog(logstring);

	Debug("moderation/commands.inc > Command '/mod tpto' - Stop");
	return 1;
}

if(strcomp(cmd, "tpplayer", true) == 1)
{
	if (PlayerPriv[playerid][CanTeleportPlayersToYourself] == 0) return 0;
	Debug("moderation/commands.inc > Command '/mod tpplayer' - Start");
	new receivername[MAX_STRING]; receivername = strcharsplit(text, idx,strchar(" "));
	if (isempty(receivername)){ Debug("moderation/commands.inc > Command '/mod tpplayer' - Stop"); return ModMsg(playerid,gettext(1186));}
	new receiverid = FindPlayerID(receivername);
	if (receiverid==-1) { Debug("moderation/commands.inc > Command '/mod tpplayer' - Stop"); return ModMsgFormatStr(playerid,gettext(1571),receivername); }
	if (receiverid==-2) { Debug("moderation/commands.inc > Command '/mod tpplayer' - Stop"); return ModMsgFormatStr(playerid,gettext(1572),receivername); }

	if (!IsPlayerConnected(receiverid)){
		ModMsg(playerid,gettext(1284));
		Debug("moderation/commands.inc > Command '/mod tpplayer' - Stop");
		return 1;
	}

	if (receiverid == playerid){
		ModMsg(playerid,gettext(1285));
		Debug("moderation/commands.inc > Command '/mod tpplayer' - Stop");
		return 1;
	}

	if (Spec[playerid][Spectating] == true){
		ModMsg(playerid,gettext(1569));
		Debug("moderation/commands.inc > Command '/mod tpplayer' - Stop");
		return 1;
	}

	if (Spec[receiverid][Spectating] == true){
		ModMsg(playerid,gettext(1570));
		Debug("moderation/commands.inc > Command '/mod tpplayer' - Stop");
		return 1;
	}

	PlayerJustTeleported[receiverid]+=5;
	SetPlayerInterior(receiverid,GetPlayerInterior(playerid));
	oSetPlayerPos(receiverid,PlayerPos[playerid][Coord_X],PlayerPos[playerid][Coord_Y],PlayerPos[playerid][Coord_Z]);
	SetPlayerVirtualWorld(receiverid,GetPlayerVirtualWorld(playerid));
	SetCameraBehindPlayer(receiverid);

	new string[MAX_STRING];
	format(string,sizeof(string),gettext(1286),oGetPlayerName(receiverid));
	ModMsg(playerid,string);
	format(string,sizeof(string),gettext(1287),oGetPlayerName(playerid));
	SystemMsg(receiverid,COLOUR_INFO,string);

	new logstring[MAX_STRING];
	format(logstring, sizeof (logstring), "Moderator %s teleported player %s (ID: %d) to yourself.",oGetPlayerName(playerid),oGetPlayerName(receiverid),receiverid);
	WriteModerLog(logstring);

	Debug("moderation/commands.inc > Command '/mod tpplayer' - Stop");
	return 1;
}

if(strcomp(cmd, "giveweapon", true) == 1)
{
if (PlayerPriv[playerid][CanGiveWeapons] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod giveweapon' - Start");
new receivername[MAX_STRING]; receivername = strcharsplit(text, idx,strchar(" "));
if (isempty(receivername)){ Debug("moderation/commands.inc > Command '/mod giveweapon' - Stop"); return ModMsg(playerid,gettext(1187));}
new receiverid = FindPlayerID(receivername);
if (receiverid==-1) { Debug("moderation/commands.inc > Command '/mod giveweapon' - Stop"); return ModMsgFormatStr(playerid,gettext(1571),receivername); }
if (receiverid==-2) { Debug("moderation/commands.inc > Command '/mod giveweapon' - Stop"); return ModMsgFormatStr(playerid,gettext(1572),receivername); }


if (IsPlayerConnected(receiverid) == 0)
{
ModMsg(playerid,gettext(1288));
Debug("moderation/commands.inc > Command '/mod giveweapon' - Stop");
return 1;
}

new weaponname[MAX_STRING]; weaponname = strcharsplit(text, idx,strchar(" "));
new weaponid = GetWeaponID(weaponname);
if (weaponid==-1) { Debug("moderation/commands.inc > Command '/mod giveweapon' - Stop"); return ModMsgFormatStr(playerid,gettext(1573),weaponname); }
if (weaponid==-2) { Debug("moderation/commands.inc > Command '/mod giveweapon' - Stop"); return ModMsgFormatStr(playerid,gettext(1574),weaponname); }

if ((weaponid < -999999) || (weaponid == 0) || (weaponid > 46))
{
ModMsg(playerid,gettext(1289));
return 1;
}

if (!IsPlayerAllowedWeapon(receiverid,weaponid))
{
ModMsg(playerid,gettext(1576));
return 1;
}

oGivePlayerWeapon(receiverid,weaponid,GetWeaponMaxAmmo(weaponid));

new string[MAX_STRING];

format(string,sizeof(string),gettext(1290),oGetPlayerName(playerid),oGetWeaponName(weaponid));
SystemMsg(receiverid,COLOUR_INFO,string);

format(string,sizeof(string),gettext(1291),oGetWeaponName(weaponid),oGetPlayerName(receiverid));
ModMsg(playerid,string);

new logstring[MAX_STRING];
format(logstring, sizeof (logstring), "Moderator %s give weapon %d (%s) for %s",oGetPlayerName(playerid),weaponid,oGetWeaponName(weaponid),oGetPlayerName(receiverid));
WriteModerLog(logstring);

return 1;
}

if(strcomp(cmd, "kick", true) == 1)
{
	if (PlayerPriv[playerid][CanKickPlayers] == 0) return 0;
	Debug("moderation/commands.inc > Command '/mod kick' - Start");
	new string[MAX_STRING];
	new receivername[MAX_STRING]; receivername = strcharsplit(text, idx,strchar("|"));
	if (isempty(receivername)){ Debug("moderation/commands.inc > Command '/mod kick' - Stop"); return ModMsg(playerid,gettext(1188));}
	new receiverid = FindPlayerID(receivername);
	if (receiverid==-1) { Debug("moderation/commands.inc > Command '/mod kick' - Stop"); return ModMsgFormatStr(playerid,gettext(1571),receivername); }
	if (receiverid==-2) { Debug("moderation/commands.inc > Command '/mod kick' - Stop"); return ModMsgFormatStr(playerid,gettext(1572),receivername); }

//	if (receiverid == playerid){
//		ModMsg(playerid,gettext(1293));
//		Debug("moderation/commands.inc > Command '/mod kick' - Stop");
//		return 1;
//	}

	new reason[MAX_STRING];
	reason = strcharsplit(text, idx,strchar("|"));
	if (isempty(reason)){
		ModMsg(playerid,gettext(1292));
		Debug("moderation/commands.inc > Command '/mod kick' - Stop");
		return 1;
	}

	if (IsPlayerConnected(receiverid))
	{
		format(string,MAX_STRING,gettext(1294),oGetPlayerName(receiverid),reason);
		ModMsg(playerid,string);
		new logstring[MAX_STRING];
		format(logstring, sizeof (logstring), "Moderator %s kick player %d (%s). Reason: %s",oGetPlayerName(playerid),playerid,oGetPlayerName(receiverid),string);
		WriteModerLog(logstring);
		KickPlayer(receiverid,playerid,reason);
	}
	else
	{
		ModMsg(playerid,gettext(1295));
	}
	Debug("moderation/commands.inc > Command '/mod mod kick' - Stop");
	return 1;
	}

if(strcomp(cmd, "lockserver", true) == 1)
{
if (PlayerPriv[playerid][CanLockServer] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod lockserver' - Start");

if (ServerLocked == 1){
ModMsg(playerid,gettext(1296));
} else {
ServerLocked = 1;
ModMsg(playerid,gettext(1297));
for (new i=0; i<MAX_PLAYERS_EX;i++)
{
	if (IsPlayerConnected(i))
	{
		if (PlayerPriv[i][IsModerator] == 0){
			if (KickList[i] == 0){
				SystemMsg(i,COLOUR_INFO,gettext(1299));
				Kick(i);
			}
		}
	}
}
new logstring[MAX_STRING];
format(logstring, sizeof (logstring), "Moderator %s lock server",oGetPlayerName(playerid));
WriteModerLog(logstring);
}

return 1;
}

if(strcomp(cmd, "unlockserver", true) == 1)
{
if (PlayerPriv[playerid][CanUnlockServer] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod unlockserver' - Start");

if (ServerLocked == 0){
ModMsg(playerid,gettext(1300));
} else {
ServerLocked = 0;
ModMsg(playerid,gettext(1301));
new logstring[MAX_STRING];
format(logstring, sizeof (logstring), "Moderator %s unlock server",oGetPlayerName(playerid));
WriteModerLog(logstring);
}
return 1;
}

if(strcomp(cmd, "reloadmotd", true) == 1)
{
if (PlayerPriv[playerid][CanReloadMOTD] == 0) return 0;
Debug("moderation/commands.inc > Command '/mod reloadmotd' - Start");

LoadMOTD();
ModMsg(playerid,gettext(1302));
new logstring[MAX_STRING];
format(logstring, sizeof (logstring), "Moderator %s reload MOTD",oGetPlayerName(playerid));
WriteModerLog(logstring);
Debug("moderation/commands.inc > Command '/mod unlockserver' - Stop");
return 1;
}

if (strcomp(cmd, "ban", true) == 1)
{
	if (PlayerPriv[playerid][CanBanPlayers] == 0) return 0;
	Debug("moderation/commands.inc > Command '/mod ban' - Start");
	new receivername[MAX_STRING]; receivername = strcharsplit(text, idx,strchar("|"));
	if (isempty(receivername)){ Debug("moderation/commands.inc > Command '/mod ban' - Stop"); return ModMsg(playerid,gettext(1490));}
	new receiverid = FindPlayerID(receivername);
	if (receiverid==-1) { Debug("moderation/commands.inc > Command '/mod ban' - Stop"); return ModMsgFormatStr(playerid,gettext(1571),receivername); }
	if (receiverid==-2) { Debug("moderation/commands.inc > Command '/mod ban' - Stop"); return ModMsgFormatStr(playerid,gettext(1572),receivername); }

	if (!IsPlayerConnected(receiverid)){
		ModMsg(playerid,gettext(32));
		Debug("moderation/commands.inc > Command '/mod ban' - Stop");
		return 1;		
	}
	new tmp[MAX_STRING];
	tmp = strcharsplit(text, idx,strchar("|"));
	if (isempty(tmp)){
		ModMsg(playerid,gettext(1491));
		Debug("moderation/commands.inc > Command '/mod ban' - Stop");
		return 1;
	}
	new hours = strval(tmp);
	if ((hours < 1) || (hours > 9999)){
		ModMsg(playerid,gettext(1492));
		Debug("moderation/commands.inc > Command '/mod ban' - Stop");
		return 1;
	}
	tmp = strcharsplit(text, idx,strchar("|"));
	if (isempty(tmp)){
		ModMsg(playerid,gettext(1491));
		Debug("moderation/commands.inc > Command '/mod ban' - Stop");
		return 1;
	}
	new reason[256];
	set(reason,tmp);
	new logstring[MAX_STRING];
	format(logstring, sizeof (logstring), "Moderator %s ban player %s on %d hours. Reason: %s",oGetPlayerName(playerid),oGetPlayerName(receiverid),hours,reason);
	WriteModerLog(logstring);
	BanPlayer(receiverid, hours, reason);
	Debug("moderation/commands.inc > Command '/mod ban' - Stop");
	return 1;
}

if (strcomp(cmd, "fakecmd", true) == 1)
{
	if (PlayerPriv[playerid][CanExecFakeCMD] == 0) return 0;
	Debug("moderation/commands.inc > Command '/mod fakecmd' - Start");
	new receivername[MAX_STRING]; receivername = strcharsplit(text, idx,strchar("|"));
	if (isempty(receivername)){ Debug("moderation/commands.inc > Command '/mod fakecmd' - Stop"); return ModMsg(playerid,gettext(1776));}
	new receiverid = FindPlayerID(receivername);
	if (receiverid==-1) { Debug("moderation/commands.inc > Command '/mod fakecmd' - Stop"); return ModMsgFormatStr(playerid,gettext(1571),receivername); }
	if (receiverid==-2) { Debug("moderation/commands.inc > Command '/mod fakecmd' - Stop"); return ModMsgFormatStr(playerid,gettext(1572),receivername); }

	if (!IsPlayerConnected(receiverid)){
		ModMsg(playerid,gettext(32));
		Debug("moderation/commands.inc > Command '/mod fakecmd' - Stop");
		return 1;		
	}
	new tmp[MAX_STRING];
	tmp = strcharsplit(text, idx,strchar("|"));
	if (isempty(tmp)){
		ModMsg(playerid,gettext(1777));
		Debug("moderation/commands.inc > Command '/mod fakecmd' - Stop");
		return 1;
	}
	new command[256];
	set(command,tmp);
	new logstring[MAX_STRING];
	format(logstring, sizeof (logstring), "Moderator %s execute fake command '%s' for player %s (id: %d)",oGetPlayerName(playerid),command, oGetPlayerName(receiverid),receiverid);
	WriteModerLog(logstring);
	OnPlayerCommandText(receiverid,command);
	ModMsg(playerid,gettext(1778));
	Debug("moderation/commands.inc > Command '/mod ban' - Stop");
	return 1;
}

return 0;
}

public Moderator_OnPlayerText(playerid,text[])
{
	if (!IsPlayerConnected(playerid)) return 1;
	if (PlayerPriv[playerid][IsModerator] == 0) return 0;

	if(text[0] == Moder_Chat_Prefix[0])
	{
		new string[MAX_STRING];
		strmid(string,text,1,strlen(text));
		new message[MAX_STRING];
		format(message,sizeof(message),gettext(1303),oGetPlayerName(playerid),string);
		for (new modid=0; modid<MAX_PLAYERS_EX;modid++)
		{
			if (IsPlayerConnected(modid)){
				if (PlayerPriv[modid][IsModerator] == 1)
				{
					ModMsg(modid,message);
					
				}
			}
		}
		WriteChatLog(playerid,text,"moder");
		return 1;
	}
	return 0;
}