//Gang module.
//Created by Iain Gilbert
//Modder: MadCat
//Upgrade idea from LS: The Next Episode

#include "base"
#include "world"

forward Gang_OnGameModeInit();
forward Gang_OnPlayerCommandText(playerid,text[]);
forward Gang_OnPlayerSpawn(playerid);
forward GangConfig();
forward GetFreeMemberID();
forward GangLoad(gangname[]);
forward PlayerGangColour(playerid);
forward GangJoinMember(gangid,playerid);
forward GangRemoveMember(gangid,kickplayername[]);
forward GangCreate(leaderid,gangname[],gangcolour,gangtag[]);
forward GangCleanup(gangid);
forward GangSaveAll();
forward GangMemberLogout(playerid,gangid);
forward GangSave(gangid);
forward GangUnload(gangid);
forward GangKill(gangid,killerid,victimid,reason);
forward GangMemberLogin(playerid,gangid);
forward SendGangMessage(gangid,message[MAX_STRING]);
forward GangOnlineCount(gangid);
forward GangGiveXP(gangid,xpamount,giverid);
forward SetGangColour(gangid,colour);
forward Gang_OnPlayerText(playerid,text[]);

#define MAX_GANGS MAX_PLAYERS // maximum gangs (at runtime), (total maximum gangs is unlimited)
#define MAX_GANG_SIZE 30 // maximum players in gang

new Gang_DB[MAX_STRING] = "MultiGTA/Gangs/";

new Gang_Create_Cost = 500000; // money cost to create a gang
new Gang_Colour_Cost = 100000; // cost to change gang colour
new Gang_Create_Level = 7; // need to be this level to create a gang
new Gang_Upgrade_Armour_Cost = 200000; // money costs to upgrade armour
new Gang_Upgrade_Friendlyfire_Cost = 1000000; //money costs to install friendlyfire

enum MemberInfo {
 member_active,
 member_online, // bool
 member_name[MAX_NAME],
 member_playerid
}

enum GangInfo {
	gang_active, //bool
	gang_name[MAX_NAME],
	gang_colour,
	gang_kills,
	gang_score,
	gang_tag[MAX_NAME],
	gang_bank,
	gang_upgrade_armour,
	gang_upgrade_friendlyfire,
	gang_upgrade_weapon,
	gang_loginmessage[100]
}

enum ColourInfo {
 	colour_name[MAX_NAME],
    	colour_code
}
#define COLOURS_SIZE 7
new GangColours[COLOURS_SIZE][ColourInfo] = {
{ "Orange",    COLOUR_ORANGE},
{ "Yellow",    COLOUR_YELLOW},
{ "Green",    COLOUR_GREEN},
{ "Ivory",	COLOUR_IVORY},
{ "LightGreen",	COLOUR_LIGHTGREEN},
{ "Violet",	COLOUR_VIOLET},
{ "LightYellow", COLOUR_LIGHTYELLOW}
};

new GangMemberInfo[MAX_GANGS*MAX_GANG_SIZE][MemberInfo]; //shit way of '3d' array
new GangMembers[MAX_GANGS][MAX_GANG_SIZE]; // shit

new Gangs[MAX_GANGS][GangInfo];
new GangInvite[MAX_PLAYERS];

public Gang_OnGameModeInit(){
	GangConfig();
	return 0;
}

public GangConfig()
{
	Debug("gang.inc > GangConfig - Start");

	if (!db_Exists(ConfigDB)) db_Create(ConfigDB);
	if (!db_Exists(DatabaseDB)) db_Create(DatabaseDB);
	new temp[MAX_STRING];

	set(temp,db_Get(DatabaseDB,"Gang_DB"));
	if (strlen(temp) > 0) set(Gang_DB,temp);
	else {set(temp,Gang_DB); db_Set(DatabaseDB,"Gang_DB",temp);}
	set(temp,nullstr);

	set(temp,db_Get(ConfigDB,"Gang_Create_Level"));
	if (strlen(temp) > 0) Gang_Create_Level = strval(temp);
	else {valstr(temp,Gang_Create_Level); db_Set(ConfigDB,"Gang_Create_Level",temp);}
	set(temp,nullstr);

	set(temp,db_Get(ConfigDB,"Gang_Upgrade_Armour_Cost"));
	if (strlen(temp) > 0) Gang_Upgrade_Armour_Cost = strval(temp);
	else {valstr(temp,Gang_Upgrade_Armour_Cost); db_Set(ConfigDB,"Gang_Upgrade_Armour_Cost",temp);}
	set(temp,nullstr);

	set(temp,db_Get(ConfigDB,"Gang_Upgrade_Friendlyfire_Cost"));
	if (strlen(temp) > 0) Gang_Upgrade_Friendlyfire_Cost = strval(temp);
	else {valstr(temp,Gang_Upgrade_Friendlyfire_Cost); db_Set(ConfigDB,"Gang_Upgrade_Friendlyfire_Cost",temp);}
	set(temp,nullstr);

	set(temp,db_Get(ConfigDB,"Gang_Create_Cost"));
	if (strlen(temp) > 0) Gang_Create_Cost = strval(temp);
	else {valstr(temp,Gang_Create_Cost); db_Set(ConfigDB,"Gang_Create_Cost",temp);}
	set(temp,nullstr);

	set(temp,db_Get(ConfigDB,"Gang_Colour_Cost"));
	if (strlen(temp) > 0) Gang_Colour_Cost =strval(temp);
	else {valstr(temp,Gang_Colour_Cost); db_Set(ConfigDB,"Gang_Colour_Cost",temp);}
	set(temp,nullstr);

	Debug("gang.inc > GangConfig - Stop");
	return;
}

public GetFreeMemberID()
{
 Debug("gang.inc > GetFreeMemberID - Start");
 for (new memberid = 1;memberid<MAX_GANG_SIZE*MAX_GANGS;memberid++)
 {
     if (GangMemberInfo[memberid][member_active] == 0)
     {
	 Debug("gang.inc > GetFreeMemberID - Stop");
         return memberid;
  }
 }
 Debug("gang.inc > GetFreeMemberID - Stop");
 return 0;
}

public GangLoad(gangname[])
{
 Debug("gang.inc > GangLoad - Start");
 // search for gang in runtime memory
 for (new id=1;id<MAX_GANGS;id++)
 {
  if (Gangs[id][gang_active] == 1)
  {
   if (strcomp(gangname,Gangs[id][gang_name],false) == 1)
   {
	Debug("gang.inc > GangLoad - Stop");
       	return id; // gang found in runtime memory
   }
  }
 }

 new gangid;
 for (new id=1;id<MAX_GANGS;id++)
 {
     if(!Gangs[id][gang_active])
     {
      gangid = id;
      Gangs[id][gang_active] = true; // find first free gang slot and andd our gang
      break;
  }
 }

 gang_load_db_dini(gangid,gangname);

 new logstring[256];
 format(logstring, sizeof (logstring), "gang_loaded: %d",gangid);
 WriteLog(logstring);
 Debug("gang.inc > GangLoad - Stop");
 return gangid;
}

gang_load_db_dini(gangid,gangname[])
{
 Debug("gang.inc > gang_load_db_dini - Start");
 new dinifilename[MAX_STRING];
 format(dinifilename,sizeof(dinifilename),"%s%s.txt",Gang_DB,gangname);

 if (!db_Exists(dinifilename))
 {
  Debug("gang.inc > gang_load_db_dini - Stop");
  return;
 }

 set(Gangs[gangid][gang_name],db_Get(dinifilename,"Name"));
 Gangs[gangid][gang_colour] = strval(db_Get(dinifilename,"Colour"));
 Gangs[gangid][gang_kills] = strval(db_Get(dinifilename,"Kills"));
 Gangs[gangid][gang_score] = strval(db_Get(dinifilename,"Score"));
 Gangs[gangid][gang_bank] = strval(db_Get(dinifilename,"Bank"));
 Gangs[gangid][gang_upgrade_armour] = strval(db_Get(dinifilename,"Upgrade_Armour"));
 Gangs[gangid][gang_upgrade_friendlyfire] = strval(db_Get(dinifilename,"Upgrade_Friendlyfire"));
 Gangs[gangid][gang_upgrade_weapon] = strval(db_Get(dinifilename,"Upgrade_Weapon"));
 set(Gangs[gangid][gang_tag],db_Get(dinifilename,"Tag"));
 set(Gangs[gangid][gang_loginmessage],db_Get(dinifilename,"Login_Message"));

 if (isempty(Gangs[gangid][gang_loginmessage])) set(Gangs[gangid][gang_loginmessage],gettext(1605));

 for (new gangmember = 0;gangmember<MAX_GANG_SIZE;gangmember++)
 {

  GangMembers[gangid][gangmember] = GetFreeMemberID();
  new memberid = GangMembers[gangid][gangmember];
  if (memberid == 0)
  {
   WriteLog("script error: free gang member id not found.");
   Debug("gang.inc > gang_load_db_dini - Stop");
   break;
  }
  GangMemberInfo[memberid][member_active] = 1;
  GangMemberInfo[memberid][member_online] = 0;
  new dinistring[MAX_STRING];
  format(dinistring,sizeof(dinistring),"Member%d" ,gangmember);
  set(GangMemberInfo[memberid][member_name],db_Get(dinifilename,dinistring));
  GangMemberInfo[memberid][member_playerid] = INVALID_PLAYER_ID;
 }
 Debug("gang.inc > gang_load_db_dini - Stop");
}

public Gang_OnPlayerCommandText(playerid,text[])
{

	new cmd[20];
	new idx;

	set(cmd,strcharsplit(text, idx,strchar(" ")));
	if (isempty(cmd)) return 0;

    	if(strcomp(cmd, "/gang", true) == 1)
	{
		if (!IsPlayerRegistered(playerid))
  		{
   			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(896));
   			return 1;
  		}

  		set(cmd,strcharsplit(text, idx,strchar(" ")));
  		if (isempty(cmd)) return 0;

  		if(strcomp(cmd, "bankget", true) == 1)
		{
		Debug("gang.inc > Command 'gang bankget' - Start");
		if (PlayerGangid[playerid] == 0)
      		{
          		SystemMsg(playerid,COLOUR_GANG_BAD,gettext(897));
			Debug("gang.inc > Command 'gang bankget' - Stop");
       			return 1;
      		}

		if (!IsPlayerAtBank(playerid))
		{
			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(898));
			Debug("gang.inc > Command 'gang bankget' - Stop");
			return 1;
		}
		
		set(cmd,strcharsplit(text, idx,strchar(" ")));
		if (isempty(cmd))
		{
			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(899));
			Debug("gang.inc > Command 'gang bankget' - Stop");
			return 1;
		}
		new amount = strval(cmd);
		if (amount <= 0)
		{
			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(899));
			Debug("gang.inc > Command 'gang bankget' - Stop");
			return 1;
		}
		new gangid = PlayerGangid[playerid];
 		new gangname[MAX_STRING];
 		set(gangname,Gangs[gangid][gang_name]);

		if (Gangs[gangid][gang_bank] == 0)
		{
			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(900));
			Debug("gang.inc > Command 'gang bankget' - Stop");
			return 1;
		}

		if (amount > Gangs[gangid][gang_bank])
		{
			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(901));
			Debug("gang.inc > Command 'gang bankget' - Stop");
			return 1;
		}

		oGivePlayerMoney(playerid,amount,0);
		Gangs[gangid][gang_bank] = Gangs[gangid][gang_bank] - amount;
		new string[MAX_STRING];
		format(string,MAX_STRING,gettext(902),amount,Gangs[gangid][gang_bank]);
		SystemMsg(playerid,COLOUR_MONEY_GOOD,string);
		new logstring[256];
		format(logstring, sizeof (logstring), "player: %d:  %s: withdrawed $%d from the gang bank. Current balance: $%d ",playerid,oGetPlayerName(playerid),amount,Gangs[gangid][gang_bank]);
		WriteLog(logstring);
		Debug("gang.inc > Command 'gang bankget' - Stop");
		return 1;
		}

  		if(strcomp(cmd, "bankput", true) == 1)
		{
		Debug("gang.inc > Command 'gang bankput' - Start");
		if (PlayerGangid[playerid] == 0)
      		{
          		SystemMsg(playerid,COLOUR_GANG_BAD,gettext(897));
			Debug("gang.inc > Command 'gang bankput' - Stop");
       			return 1;
      		}

		if (!IsPlayerAtBank(playerid))
		{
			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(898));
			Debug("gang.inc > Command 'gang bankput' - Stop");
			return 1;
		}
		
		set(cmd,strcharsplit(text, idx,strchar(" ")));
		if (isempty(cmd))
		{
			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(903));
			Debug("gang.inc > Command 'gang bankput' - Stop");
			return 1;
		}
		new amount = strval(cmd);
		if (amount <= 0)
		{
			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(903));
			Debug("gang.inc > Command 'gang bankput' - Stop");
			return 1;
		}

		if (amount > Player[playerid][Money])
		{
			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(904));
			Debug("gang.inc > Command 'gang bankput' - Stop");
			return 1;
		}
		new gangid = PlayerGangid[playerid];
 		new gangname[MAX_STRING];
 		set(gangname,Gangs[gangid][gang_name]);

		oGivePlayerMoney(playerid,-amount,0);
		Gangs[gangid][gang_bank] = Gangs[gangid][gang_bank] + amount;
		new string[MAX_STRING];
		format(string,MAX_STRING,gettext(905),amount,Gangs[gangid][gang_bank]);
		SystemMsg(playerid,COLOUR_MONEY_GOOD,string);
		new logstring[256];
		format(logstring, sizeof (logstring), "player: %d:  %s: withdrawed $%d from the gang bank. Current balance: $%d ",playerid,oGetPlayerName(playerid),amount,Gangs[gangid][gang_bank]);
		WriteLog(logstring);
		Debug("gang.inc > Command 'gang bankput' - Stop");
		return 1;
	}

	  	if(strcomp(cmd, "help", true) == 1)
  		{
			Debug("gang.inc > Command 'gang help' - Start");
   			SystemMsgScrolling(playerid,gettext(198));
   			SystemMsgScrolling(playerid,gettext(199));
   			SystemMsgScrolling(playerid,gettext(200));
			SystemMsgScrolling(playerid,gettext(201));
			SystemMsgScrolling(playerid,gettext(202));
   			SystemMsgScrolling(playerid,gettext(203));
			SystemMsgScrolling(playerid,gettext(204));
			Debug("gang.inc > Command 'gang help' - Stop");
   			return 1;
  		}

  		if(strcomp(cmd, "commands", true) == 1)
  		{
			Debug("gang.inc > Commandh 'gang commands' - Start");
   			SystemMsgScrolling(playerid,gettext(911));
   			SystemMsgScrolling(playerid,gettext(289));
			SystemMsgScrolling(playerid,gettext(290));
   			if (PlayerGangid[playerid] != 0)
			{
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
			Debug("gang.inc > Commandh 'gang commands' - Stop");
   			return 1;
  		}

  		if(strcomp(cmd, "colours", true) == 1)
  		{
			Debug("gang.inc > Command 'gang colours' - Start");
			SystemMsg(playerid,COLOUR_GANG,gettext(912));
   			new string[MAX_STRING];
			for(new i=0; i<COLOURS_SIZE; i++){
				format(string,MAX_STRING,gettext(913),GangColours[i][colour_name]);
				SystemMsgScrolling(playerid,string);
			}
			Debug("gang.inc > Command 'gang colours' - Stop");
   			return 1;
  		}

  		if((strcomp(cmd, "stats", true) == 1) || (strcomp(cmd, "stat", true) == 1) || (strcomp(cmd, "status", true) == 1))
  		{
			Debug("gang.inc > Command 'gang stats' - Start");
      			if (PlayerGangid[playerid] == 0)
      			{
          			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(897));
				Debug("gang.inc > Command 'gang stats' - Stop");
       				return 1;
      			}
      			new gangid = PlayerGangid[playerid];
			new string[MAX_STRING];
      			format(string, MAX_STRING,gettext(914), Gangs[gangid][gang_name]);
      			SystemMsg(playerid,COLOUR_GANG,string);
			format(string, MAX_STRING,gettext(915), Gangs[gangid][gang_tag]);
   			SystemMsg(playerid,COLOUR_GANG,string);
      			format(string, MAX_STRING,gettext(916), Gangs[gangid][gang_kills]);
   			SystemMsg(playerid,COLOUR_GANG,string);
   			format(string, MAX_STRING,gettext(917), Gangs[gangid][gang_score]);
   			SystemMsg(playerid,COLOUR_GANG,string);
   			format(string, MAX_STRING,gettext(918), GangOnlineCount(gangid));
   			SystemMsg(playerid,COLOUR_GANG,string);
			Debug("gang.inc > Command 'gang stats' - Stop");
   			return 1;
  		}

  		if(strcomp(cmd, "members", true) == 1)
  		{
			Debug("gang.inc > Command 'gang members' - Start");
      		if (PlayerGangid[playerid] == 0)
      		{
       			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(897));
			Debug("gang.inc > Command 'gang members' - Stop");
       			return 1;
      		}
      		new gangid = PlayerGangid[playerid];
      		new status[10];
		new string[MAX_STRING];
      		format(string, MAX_STRING,gettext(919), Gangs[gangid][gang_name]);
      		SystemMsg(playerid,COLOUR_GANG,string);

      		for (new gangmember = 0;gangmember<MAX_GANG_SIZE;gangmember++)
   			{
    			new memberid = GangMembers[gangid][gangmember];
    			if (strlen(GangMemberInfo[memberid][member_name]) > 0)
          		{
           			if (GangMemberInfo[memberid][member_online])
           			{
      					set(status,gettext(920));
         				format(string, MAX_STRING,gettext(921), GangMemberInfo[memberid][member_name],status,GetPlayerLevel(GangMemberInfo[memberid][member_playerid]));
     				}
     				else
     				{
      					set(status,gettext(922));
      					format(string, MAX_STRING,gettext(923), GangMemberInfo[memberid][member_name],status);
     				}
     				SystemMsg(playerid,COLOUR_GANG,string);
    			}
   			}
			Debug("gang.inc > Command 'gang members' - Stop");
   			return 1;
  		}

  		if(strcomp(cmd, "create", true) == 1)
  		{
			Debug("gang.inc > Command 'gang create' - Start");

      		if (PlayerGangid[playerid] != 0)
      		{
       			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(924));
			Debug("gang.inc > Command 'gang create' - Stop");
       			return 1;
      		}

		new string[MAX_STRING];
		if (GetPlayerLevel(playerid) < Gang_Create_Level)
      		{
          		format(string, MAX_STRING,gettext(1494), Gang_Create_Level);
       			SystemMsg(playerid,COLOUR_GANG_BAD,string);
			Debug("gang.inc > Command 'gang create' - Stop");
       			return 1;
      		}

      		if (oGetPlayerMoney(playerid) < Gang_Create_Cost)
      		{
          		format(string, MAX_STRING,gettext(925), Gang_Create_Cost);
       			SystemMsg(playerid,COLOUR_GANG_BAD,string);
			Debug("gang.inc > Command 'gang create' - Stop");
       			return 1;
      		}
   			new gangname[MAX_STRING];
   			new colourname[MAX_NAME];
   			new gangcolour=255;

   			set(colourname,strcharsplit(text, idx,strchar(" ")));
   			if(isempty(colourname))
   			{
       			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(926));
                	SystemMsg(playerid,COLOUR_GANG,gettext(927));
			Debug("gang.inc > Command 'gang create' - Stop");
    			return 1;
         	}

         	for (new colourid=0;colourid<COLOURS_SIZE;colourid++)
         	{
             	if (strcomp(colourname,GangColours[colourid][colour_name],true) == 1)
             	{
              		gangcolour = GangColours[colourid][colour_code];
    			}
         	}
         	if (gangcolour == 255)
         	{
          		SystemMsg(playerid,COLOUR_GANG_BAD,gettext(926));
                	SystemMsg(playerid,COLOUR_GANG,gettext(927));
			Debug("gang.inc > Command 'gang create' - Stop");
    			return 1;
   			}

			new gangtag[MAX_STRING];
			set(gangtag,strcharsplit(text, idx,strchar(" ")));
			if (isempty(gangtag)){
			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(928));
			Debug("gang.inc > Command 'gang create' - Stop");
			return 1;
			}
			if (strlen(gangtag) > 3){
			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(929));
			Debug("gang.inc > Command 'gang create' - Stop");
			return 1;
			}
			if (strlen(gangtag) < 2){
			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(930));
			Debug("gang.inc > Command 'gang create' - Stop");
			return 1;
			}

      		set(gangname,text);
         	strdel(gangname, 0, idx);
         	if (strlen(gangname) <3)
   			{
       			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(931));
			Debug("gang.inc > Command 'gang create' - Stop");
    			return 1;
   			}
   			if (strlen(gangname) >40)
   			{
       			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(932));
			Debug("gang.inc > Command 'gang create' - Stop");
    			return 1;
   			}
   			new tempname[MAX_STRING];
   			set(tempname,gangname);
   			if (!StringCharCheck(tempname))
   			{
    			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(933));
			Debug("gang.inc > Command 'gang create' - Stop");
    			return 1;
   			}

   			if (GangCreate(playerid,gangname,gangcolour,gangtag) != 0)
   			{
    				oGivePlayerMoney(playerid,0-Gang_Create_Cost,1);
				format (string,MAX_STRING,gettext(934),gangname,gangtag);
				SystemMsg(playerid,COLOUR_GANG,string);
				SetPlayerColor(playerid,gangcolour);
				new newname[MAX_NAME];
				new name[MAX_NAME];
				GetPlayerName(playerid, name, sizeof(name));
				format(newname,sizeof(newname),"[%s]%s",PlayerGangTag[playerid],name);
				SetPlayerName(playerid,newname);
    				new logstring[256];
    				format(logstring, sizeof (logstring), "player: %d:  %s: created gang '%s' with tag '%s'",playerid,oGetPlayerName(playerid),gangname,gangtag);
       				WriteLog(logstring);
   			}
   			else
   			{
    			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(935));
   			}
			Debug("gang.inc > Command 'gang create' - Stop");
  			return 1;
  		} // end create

  		if(strcomp(cmd, "join", true) == 1)
  		{
			Debug("gang.inc > Command 'gang join' - Start");
      		if (PlayerGangid[playerid] != 0)
      		{
       			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(924));
			return 1;
      		}
      		if (GangInvite[playerid] == 0)
      		{
       			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(936));
			Debug("gang.inc > Command 'gang join' - Stop");
    			return 1;
      		}

      		if (Player[playerid][GotJob] == JOB_COP)
      		{
       			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(937));
			Debug("gang.inc > Command 'gang join' - Stop");
    			return 1;
      		}

   			new gangid = GangInvite[playerid];

   			if (GangOnlineCount(gangid) >= MAX_GANG_SIZE)
   			{

    			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(938));
    			GangInvite[playerid] = 0;
			Debug("gang.inc > Command 'gang join' - Stop");
    			return 1;
   			}

		new string[MAX_STRING];
      		if (GangJoinMember(gangid,playerid) == 1)
      		{
		        format(string, MAX_STRING,gettext(939),Gangs[gangid][gang_name]);
    			SystemMsg(playerid,COLOUR_GANG,string);
    			format(string, MAX_STRING,gettext(940), oGetPlayerName(playerid));
    			SendGangMessage(gangid,string);
                	new logstring[256];
    			format(logstring, sizeof (logstring), "player: %d:  %s: have joined '%s' gang.",playerid,oGetPlayerName(playerid),Gangs[gangid][gang_name]);
    			WriteLog(logstring);
			Debug("gang.inc > Command 'gang join' - Stop");
    			return 1;
   		}else{
          		format(string, MAX_STRING,gettext(941), Gangs[gangid][gang_name]);
    			SystemMsg(playerid, COLOUR_GANG_BAD,string);
			Debug("gang.inc > Command 'gang join' - Stop");
			return 1;
   			}
  		}

  		if (PlayerGangid[playerid] == 0)
  		{
   			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(897));
   			return 1;
  		}
  		new gangid = PlayerGangid[playerid];
  		new leadermemberid = GangMembers[gangid][0];


  		if((strcomp(cmd, "quit", true) == 1) || (strcomp(cmd, "leave", true) == 1))
  		{
			Debug("gang.inc > Command 'gang quit' - Start");
      		if (PlayerGangid[playerid] == 0)
      		{
       			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(897));
			Debug("gang.inc > Command 'gang quit' - Stop");
       			return 1;
      		}
//	      	if (strcomp(GangMemberInfo[leadermemberid][member_name],oGetPlayerName(playerid),false) == 1)
//      		{
//     			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(942));
//			SystemMsg(playerid,COLOUR_GANG,gettext(943));
//			Debug("gang.inc > Command 'gang quit' - Stop");
//    			return 1;
//   		}
      		GangRemoveMember(gangid,oGetPlayerName(playerid));
		Debug("gang.inc > Command 'gang quit' - Stop");
      		return 1;
  		}

		if(strcomp(cmd, "invite", true) == 1)
  		{
			Debug("gang.inc > Command 'gang invite' - Start");
      		if (strcomp(GangMemberInfo[leadermemberid][member_name],oGetPlayerName(playerid),false) == 0)
      		{
       			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(906));
			Debug("gang.inc > Command 'gang invite' - Stop");
    			return 1;
   			}
      		new inviteid = INVALID_PLAYER_ID;

      		new inviteplayername[MAX_NAME];
   		set(inviteplayername,text);
            	strdel(inviteplayername, 0, idx);
            	if (isempty(inviteplayername))
            	{
       			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(944));
			Debug("gang.inc > Command 'gang invite' - Stop");
                	return 1;
         	}

            	inviteid = FindPlayerID(inviteplayername);

      		if (inviteid == -1 || inviteid == -2 || inviteid == INVALID_PLAYER_ID)
      		{
       			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(944));
			Debug("gang.inc > Command 'gang invite' - Stop");
               		return 1;
      		}
      		if (!IsPlayerRegistered(inviteid))
      		{
          		SystemMsg(playerid,COLOUR_GANG,gettext(945));
       			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(944));
			Debug("gang.inc > Command 'gang invite' - Stop");
                	return 1;
   			}

   			if (PlayerGangid[inviteid] != 0)
      		{
       			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(946));
			Debug("gang.inc > Command 'gang invite' - Stop");
    			return 1;
      		}

   		if (Player[inviteid][GotJob] == JOB_COP)
      		{
       			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(937));
			Debug("gang.inc > Command 'gang invite' - Stop");
    			return 1;
      		}
			new string[MAX_STRING];
         		format(string, MAX_STRING,gettext(947), oGetPlayerName(playerid),Gangs[gangid][gang_name]);
   			SystemMsg(inviteid, COLOUR_GANG,string);
   			format(string, MAX_STRING,gettext(948), oGetPlayerName(inviteid),Gangs[gangid][gang_name]);
   			SystemMsg(playerid, COLOUR_GANG,string);
   			GangInvite[inviteid] = gangid;
   			new logstring[256];
   			format(logstring, sizeof (logstring), "player: %d:  %s: has invited %s to join gang '%s'.",playerid,oGetPlayerName(playerid),oGetPlayerName(inviteid),Gangs[gangid][gang_name]);
      			WriteLog(logstring);
			Debug("gang.inc > Command 'gang invite' - Stop");
   			return 1;
  		}

  		if(strcomp(cmd, "kick", true) == 1)
  		{
			Debug("gang.inc > Command 'gang kick' - Start");
      		if (strcomp(GangMemberInfo[leadermemberid][member_name],oGetPlayerName(playerid),false) == 0)
      		{
       			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(906));
			Debug("gang.inc > Command 'gang kick' - Stop");
    			return 1;
   			}
   			new kickplayername[MAX_NAME];
   			set(kickplayername,text);
           		strdel(kickplayername, 0, idx);
            		if (isempty(kickplayername))
            		{
            			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(949));
				Debug("gang.inc > Command 'gang kick' - Stop");
    				return 1;
            		}
			new string[MAX_STRING];
           	 	if (GangRemoveMember(gangid,kickplayername))
            		{
       			format(string, MAX_STRING,gettext(950), kickplayername);
    			SystemMsg(playerid, COLOUR_GANG,string);
   			}
   			else
   			{
    			format(string, MAX_STRING,gettext(951), kickplayername);
    			SystemMsg(playerid, COLOUR_GANG,string);
   			}
			Debug("gang.inc > Command 'gang kick' - Stop");
   			return 1;

  		}

  		if((strcomp(cmd, "colour", true) == 1) || (strcomp(cmd, "COLOUR", true) == 1))
  		{
			Debug("gang.inc > Command 'gang colour' - Start");
      			if (strcomp(GangMemberInfo[leadermemberid][member_name],oGetPlayerName(playerid),false) == 0)
      			{
       				SystemMsg(playerid,COLOUR_GANG_BAD,gettext(906));
				Debug("gang.inc > Command 'gang colour' - Stop");
    				return 1;
   			}
    			if (oGetPlayerMoney(playerid) < Gang_Colour_Cost)
      			{
       				SystemMsgFormat(playerid,COLOUR_GANG_BAD,gettext(952), Gang_Colour_Cost);
				Debug("gang.inc > Command 'gang colour' - Stop");
    				return 1;
   			}
   			new colourname[MAX_NAME];
   			new gangcolour;
   			set(colourname,strcharsplit(text, idx,strchar(" ")));
   			if(isempty(colourname))
   			{
       				SystemMsg(playerid,COLOUR_GANG_BAD,gettext(953));
				Debug("gang.inc > Command 'gang colour' - Stop");
   				return 1;
         		}

        		for (new colourid=0;colourid<COLOURS_SIZE;colourid++)
         		{
            			if (strcomp(colourname,GangColours[colourid][colour_name],true) == 1)
             			{
              				gangcolour = GangColours[colourid][colour_code];
    				}
         		}
         		if (gangcolour == 0)
         		{
          			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(953));
				Debug("gang.inc > Command 'gang colour' - Stop");
    				return 1;
   			}

   			SystemMsgFormat(playerid,COLOUR_GANG_BAD,gettext(954), Gang_Colour_Cost);
   			SetGangColour(gangid,gangcolour);
   			oGivePlayerMoney(playerid,0-Gang_Colour_Cost,1);
			Debug("gang.inc > Command 'gang colour' - Stop");
			return 1;
  		}

  		if(strcomp(cmd, "setmessage", true) == 1)
  		{
		Debug("gang.inc > Command 'gang setmessage' - Start");

  		if (PlayerGangid[playerid] == 0)
  		{
   			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(897));
			Debug("gang.inc > Command 'gang setmessage' - Stop");
   			return 1;
  		}

      		if (strcomp(GangMemberInfo[leadermemberid][member_name],oGetPlayerName(playerid),false) == 0)
      		{
       			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(906));
			Debug("gang.inc > Command 'gang setmessage' - Stop");
    			return 1;
   		}

		new message[MAX_STRING];
		set(message,text);
		strdel(message, 0, idx);

		if(isempty(message))
		{
			SystemMsg(playerid,COLOUR_GANG,gettext(1606));
			Debug("gang.inc > Command 'gang setmessage' - Stop");
    			return 1;
        	}

      		if(strlen(message) > 99)
		{
			SystemMsg(playerid,COLOUR_GANG,gettext(1607));
			Debug("gang.inc > Command 'gang setmessage' - Stop");
			return 1;
		}
		format(Gangs[PlayerGangid[playerid]][gang_loginmessage],100,"%s",message);
		SystemMsg(playerid,COLOUR_GANG,gettext(1608));

		Debug("gang.inc > Command 'gang setmessage' - Stop");
  		return 1;
  		}

  		if(strcomp(cmd, "upgrade", true) == 1)
  		{
		Debug("gang.inc > Command 'gang upgrade' - Start");

  		if (PlayerGangid[playerid] == 0)
  		{
   			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(897));
			Debug("gang.inc > Command 'gang upgrade' - Stop");
   			return 1;
  		}

      		if (strcomp(GangMemberInfo[leadermemberid][member_name],oGetPlayerName(playerid),false) == 0)
      		{
       			SystemMsg(playerid,COLOUR_GANG_BAD,gettext(906));
			Debug("gang.inc > Command 'gang upgrade' - Stop");
    			return 1;
   		}

		new upgradetype[MAX_STRING];
		set(upgradetype,strcharsplit(text, idx,strchar(" ")));

		if(strcomp(upgradetype,"armour",true) == 1)
		{
			if (oGetPlayerMoney(playerid) > Gang_Upgrade_Armour_Cost)
      			{
				if (Gangs[gangid][gang_upgrade_armour] >= 100){
					SystemMsg(playerid,COLOUR_GANG_BAD,gettext(1008));
				} else {
					Gangs[gangid][gang_upgrade_armour] = Gangs[gangid][gang_upgrade_armour] + 20;
  					for (new gangmember = 0;gangmember<MAX_GANG_SIZE;gangmember++)
 					{
  						new memberid = GangMembers[gangid][gangmember]; // get memberid
  						if(GangMemberInfo[memberid][member_online]){
							if (floatround(oGetPlayerArmour(GangMemberInfo[memberid][member_playerid])) < Gangs[gangid][gang_upgrade_armour]) SetPlayerArmour(GangMemberInfo[memberid][member_playerid], Gangs[gangid][gang_upgrade_armour]);
							SystemMsgFormat(playerid,COLOUR_GANG,gettext(1010),Gangs[gangid][gang_upgrade_armour]);
						}
 					}
					oGivePlayerMoney(playerid,-Gang_Upgrade_Armour_Cost,0);
				}
      			} else {
				SystemMsgFormat(playerid,COLOUR_GANG,gettext(1645),Gang_Upgrade_Armour_Cost);
			}
       		} 
		else if(strcomp(upgradetype,"friendlyfire",true) == 1)
		{
			if (oGetPlayerMoney(playerid) > Gang_Upgrade_Friendlyfire_Cost)
      			{
				if (Gangs[gangid][gang_upgrade_friendlyfire] == 1){
					SystemMsg(playerid,COLOUR_GANG_BAD,gettext(1650));
				} else {
					Gangs[gangid][gang_upgrade_friendlyfire] = 1;
  					for (new gangmember = 0;gangmember<MAX_GANG_SIZE;gangmember++)
 					{
  						new memberid = GangMembers[gangid][gangmember]; // get memberid
  						if(GangMemberInfo[memberid][member_online]){
							SetPlayerTeam(GangMemberInfo[memberid][member_playerid], gangid);
       							SystemMsg(playerid,COLOUR_GANG,gettext(1651));
						}
 					}
					oGivePlayerMoney(playerid,-Gang_Upgrade_Friendlyfire_Cost,0);
				}
      			} else {
				SystemMsgFormat(playerid,COLOUR_GANG,gettext(1645),Gang_Upgrade_Friendlyfire_Cost);
			}
		}
		else if(strcomp(upgradetype,"weapon",true) == 1)
		{
			new weaponname[MAX_STRING]; weaponname = strcharsplit(text, idx,strchar(" "));
			if (isempty(weaponname)) { Debug("gang.inc > Command 'gang upgrade' - Stop"); return SystemMsg(playerid,COLOUR_ERROR,gettext(1791)); }
			new weaponid = GetWeaponID(weaponname);
			if (weaponid==-1) { Debug("gang.inc > Command 'gang upgrade' - Stop"); return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1573),weaponname); }
			if (weaponid==-2) { Debug("gang.inc > Command 'gang upgrade' - Stop"); return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1574),weaponname); }
			if (!IsAllowedWeapon(weaponid)) { Debug("gang.inc > Command 'gang upgrade' - Stop"); return SystemMsgFormatStr(playerid,COLOUR_ERROR,gettext(1790),weaponname); }
			new weaponcost = GetWeaponCost(weaponid) * GetWeaponLevel(weaponid) * GangTotalMembersCount(gangid);
			if (oGetPlayerMoney(playerid) > weaponcost)
      			{
				if (Gangs[gangid][gang_upgrade_weapon] == 1){
					SystemMsg(playerid,COLOUR_GANG_BAD,gettext(1650));
				} else {
					Gangs[gangid][gang_upgrade_weapon] = weaponid;
  					for (new gangmember = 0;gangmember<MAX_GANG_SIZE;gangmember++)
 					{
  						new memberid = GangMembers[gangid][gangmember]; // get memberid
  						if(GangMemberInfo[memberid][member_online]){
							GiveGangWeapon(GangMemberInfo[memberid][member_playerid],gangid);
       							SystemMsgFormatStr(playerid,COLOUR_GANG,gettext(1654),oGetWeaponName(Gangs[gangid][gang_upgrade_weapon]));
						}
 					}
					oGivePlayerMoney(playerid,-weaponcost,0);
				}
      			} else {
				SystemMsgFormat(playerid,COLOUR_GANG,gettext(1645),weaponcost);
        		}
		} else {
			new string[MAX_STRING];
			format(string,MAX_STRING,gettext(1646),Gang_Upgrade_Armour_Cost,Gang_Upgrade_Friendlyfire_Cost);
			SystemMsg(playerid,COLOUR_GANG,string);
		}


		Debug("gang.inc > Command 'gang upgrade' - Stop");
  		return 1;
  		}

	} // end gang
 	return 0;
}

public Gang_OnPlayerSpawn(playerid)
{
	new gangid = PlayerGangid[playerid];
	if (gangid != 0){
		if (Gangs[gangid][gang_upgrade_armour] > 0) SetPlayerArmour(playerid,Gangs[gangid][gang_upgrade_armour]);
		if (Gangs[gangid][gang_upgrade_friendlyfire] == 1) SetPlayerTeam(playerid,gangid);
		if (Gangs[gangid][gang_upgrade_weapon] > 0) GiveGangWeapon(playerid,gangid);
	}
	return 0;
}

public PlayerGangColour(playerid)
{
 Debug("gang.inc > PlayerGangColour - Start");
 new gangid = PlayerGangid[playerid];
 new newcolour = COLOUR_PLAYER;
 if (gangid != 0)
 {
     if (Gangs[gangid][gang_colour] != 0)
     {
         newcolour = Gangs[gangid][gang_colour];
  }
 }
 Debug("gang.inc > PlayerGangColour - Stop");
 return newcolour;
}

public GangJoinMember(gangid,playerid)
{
 Debug("gang.inc > GangJoinMember - Start");
 if (gangid == 0)
 {
  WriteLog("script_warning: invalid gang id.");
  Debug("gang.inc > GangJoinMember - Stop");
  return 0;
 }
 if (PlayerGangid[playerid] != 0)
    {
     WriteLog("script_warning: player already in a gang!");
     Debug("gang.inc > GangJoinMember - Stop");
     return 0;
    }

 if (GangOnlineCount(gangid) >= MAX_GANG_SIZE)
 {
  WriteLog("script_warning: Gang is already full.");
  Debug("gang.inc > GangJoinMember - Stop");
  return 0;
 }

 for (new gangmember = 0;gangmember<MAX_GANG_SIZE;gangmember++)
 {
  new memberid = GangMembers[gangid][gangmember]; // get memberid
  if (strlen(GangMemberInfo[memberid][member_name]) == 0) // if member slot free
  {
   set(GangMemberInfo[memberid][member_name],oGetPlayerName(playerid));
   GangMemberInfo[memberid][member_playerid] = playerid;
   GangMemberInfo[memberid][member_online] = 1;
   PlayerGangid[playerid] = gangid;
   set(PlayerGangName[playerid],Gangs[gangid][gang_name]);
   set(PlayerGangTag[playerid],Gangs[gangid][gang_tag]);
//   GangMemberLogin(gangid,playerid);
   GangMemberLogin(playerid,gangid);
   Debug("gang.inc > GangJoinMember - Stop");
   return 1;
  }
 }
 Debug("gang.inc > GangJoinMember - Stop");
 return 0;
}

public GangRemoveMember(gangid,kickplayername[])
{
 Debug("gang.inc > GangRemoveMember - Start");
 new playerid = INVALID_PLAYER_ID;

 for (new gangmember = 0;gangmember<MAX_GANG_SIZE;gangmember++)
 {

  new memberid = GangMembers[gangid][gangmember]; // get memberid
  if (strcomp(GangMemberInfo[memberid][member_name],kickplayername,true) == 1)
  {
   playerid = GangMemberInfo[memberid][member_playerid];
   GangMemberInfo[memberid][member_online] = 0;
   GangMemberInfo[memberid][member_playerid] = INVALID_PLAYER_ID;
   set(GangMemberInfo[memberid][member_name],nullstr);
   new string[MAX_STRING];
   format(string, MAX_STRING,gettext(955), kickplayername);
   SendGangMessage(gangid,string);
   new logstring[256];
   format(logstring, sizeof (logstring), "%s has been removed from gang '%s'", kickplayername,Gangs[gangid][gang_name]);
   WriteLog(logstring);
   if (playerid != INVALID_PLAYER_ID)
   {
    format(string, MAX_STRING,gettext(956), Gangs[gangid][gang_name]);
    SystemMsg(playerid,COLOUR_GANG,string);
    SetPlayerName(playerid,oGetPlayerName(playerid));
    PlayerGangid[playerid] = 0;
    set(PlayerGangName[playerid],nullstr);
    set(PlayerGangTag[playerid],"No GangTag");
    GangSave(gangid);
    SetSpecialColorForPlayer(playerid);
    SetPlayerTeam(playerid,0);
   }
   if (GangOnlineCount(gangid) == 0)
      	{
          GangUnload(gangid);
	}
   Debug("gang.inc > GangRemoveMember - Stop");
   return 1;
  }
 }
 Debug("gang.inc > GangRemoveMember - Stop");
 return 0;
}


public GangCreate(leaderid,gangname[],gangcolour,gangtag[])
{
 if (!IsPlayerRegistered(leaderid)) return 0;

 new tempname[MAX_STRING];
 set(tempname,gangname);
 if (!StringCharCheck(tempname) || IsNameForbidden(gangname))
  {
  SystemMsg(leaderid,COLOUR_GANG_BAD,gettext(933));
  return 0;
 }

  new dinifilename[MAX_STRING];
  format(dinifilename,sizeof(dinifilename),"%s%s.txt",Gang_DB,gangname);

 if (db_Exists(dinifilename))
 {
  	SystemMsg(leaderid,COLOUR_GANG_BAD,gettext(957));
   	return 0;
 }

 // Create the gang
 new gangid;
 for (new id=1;id<MAX_GANGS;id++)
 {
     if(!Gangs[id][gang_active])
     {
      gangid = id;
      Gangs[id][gang_active] = true; // find first free gang slot and andd our gang
      break;
  }
 }


 set(Gangs[gangid][gang_name],gangname);
 set(Gangs[gangid][gang_tag],gangtag);
 Gangs[gangid][gang_colour] = gangcolour;
 Gangs[gangid][gang_kills] = 0;
 Gangs[gangid][gang_score] = 0;
 set(Gangs[gangid][gang_loginmessage],gettext(1605));


 for (new gangmember = 0;gangmember<MAX_GANG_SIZE;gangmember++)
 {

  GangMembers[gangid][gangmember] = GetFreeMemberID();
  new memberid = GangMembers[gangid][gangmember];
  if (memberid == 0)
  {
   WriteLog("script error: free gang member id not found.");
   return 0;
  }

  set(GangMemberInfo[memberid][member_name],nullstr);
  GangMemberInfo[memberid][member_active] = 1;
  GangMemberInfo[memberid][member_online] = 0;
  GangMemberInfo[memberid][member_playerid] = INVALID_PLAYER_ID;
 }

 new memberid = GangMembers[gangid][0];
 GangMemberInfo[memberid][member_playerid] = leaderid;
 GangMemberInfo[memberid][member_online] = 1;
 set(GangMemberInfo[memberid][member_name],oGetPlayerName(leaderid));
 set(PlayerGangName[leaderid],gangname);
 set(PlayerGangTag[leaderid],gangtag);
 new name[MAX_NAME];
 new oldname[MAX_NAME];
 GetPlayerName(leaderid,oldname,sizeof(oldname));
 format(name,sizeof(name),"[%s]%s",PlayerGangTag[leaderid],oldname);
 PlayerGangid[leaderid] = gangid;

 GangSave(gangid);

 return gangid;
}

public GangCleanup(gangid)
{
 Debug("gang.inc > GangCleanup - Start");
 if (!Gangs[gangid][gang_active]) return;

 for (new gangmember = 0;gangmember<MAX_GANG_SIZE;gangmember++)
 {

  new memberid = GangMembers[gangid][gangmember]; // get memberid
  if (GangMemberInfo[memberid][member_online])
  {
      if (GangMemberInfo[memberid][member_playerid] == INVALID_PLAYER_ID) return;
      if (!IsPlayerRegistered(GangMemberInfo[memberid][member_playerid]))
   {
    GangMemberLogout(GangMemberInfo[memberid][member_playerid],gangid);
    GangMemberInfo[memberid][member_online] = 0;
   }
  }
 }

 if (GangOnlineCount(gangid) == 0)
    {
        GangUnload(gangid);
 }
 Debug("gang.inc > GangCleanup - Stop");
}

public GangSaveAll()
{
 Debug("gang.inc > GangSaveAll - Start");
 for (new i = 1;i<MAX_GANGS;i++)
 {
  if (Gangs[i][gang_active])
  {
      GangSave(i);
      GangCleanup(i);
  }
 }
 Debug("gang.inc > GangSaveAll - Stop");
}

public GangMemberLogout(playerid,gangid)
{
 Debug("gang.inc > GangMemberLogout - Start");
 GangInvite[playerid] = 0;
 if (gangid == 0)
 {
  Debug("gang.inc > GangMemberLogout - Stop");
  return;
 }
 for (new gangmember = 0;gangmember<MAX_GANG_SIZE;gangmember++)
 {
  new memberid = GangMembers[gangid][gangmember]; // get memberid
  if (GangMemberInfo[memberid][member_playerid] == playerid)
  {
   GangMemberInfo[memberid][member_online] = 0;
   GangMemberInfo[memberid][member_playerid] = INVALID_PLAYER_ID;
   break;
  }
 }
 if (GangOnlineCount(gangid) == 0)
 {
    GangUnload(gangid);
 }
 else
 {
  new string[MAX_STRING];
  format(string, MAX_STRING,gettext(958), oGetPlayerName(playerid));
  SendGangMessage(gangid,string);
 }
 SetPlayerTeam(playerid,0);
 GangSave(gangid);
 Debug("gang.inc > GangMemberLogout - Stop");
}

public GangSave(gangid)
{
 Debug("gang.inc > GangSave - Start");
 if (gangid == 0) { Debug("gang.inc > GangSave - Stop"); return 0;}
 if (!Gangs[gangid][gang_active]) { Debug("gang.inc > GangSave - Stop"); return 0;}
 if (isempty(Gangs[gangid][gang_name])) { Debug("gang.inc > GangSave - Stop"); return 0;}
 gang_save_db_dini(gangid);
 Debug("gang.inc > GangSave - Stop");
 return 1;
}

gang_save_db_dini(gangid)
{
 Debug("gang.inc > gang_save_db_dini - Start");
 new gangname[MAX_STRING];
 set(gangname,Gangs[gangid][gang_name]);
 new dinifilename[MAX_STRING];
 format(dinifilename,sizeof(dinifilename),"%s%s.txt",Gang_DB,gangname);
 if (!db_Exists(dinifilename))
 {
  if (strlen(gangname) > 1){
   db_Create(dinifilename);
  }
 }

 if (isempty(Gangs[gangid][gang_loginmessage])) set(Gangs[gangid][gang_loginmessage],gettext(1605));
 #if DINI_OPTIMIZE == 1 && DB_TYPE == 1
 new File:file = fopen(dinifilename,io_write);
	if (file){
		new writeit[MAX_STRING];
		format(writeit,MAX_STRING,"Name=%s\r\n",gangname); fwrite(file,writeit);
		format(writeit,MAX_STRING,"Tag=%s\r\n",Gangs[gangid][gang_tag]); fwrite(file,writeit);
		format(writeit,MAX_STRING,"Colour=%d\r\n",Gangs[gangid][gang_colour]); fwrite(file,writeit);
		format(writeit,MAX_STRING,"Kills=%d\r\n",Gangs[gangid][gang_kills]); fwrite(file,writeit);
		format(writeit,MAX_STRING,"Score=%d\r\n",Gangs[gangid][gang_score]); fwrite(file,writeit);
		format(writeit,MAX_STRING,"Bank=%d\r\n",Gangs[gangid][gang_bank]); fwrite(file,writeit);
		format(writeit,MAX_STRING,"Upgrade_Armour=%d\r\n",Gangs[gangid][gang_upgrade_armour]); fwrite(file,writeit);
		format(writeit,MAX_STRING,"Upgrade_Friendlyfire=%d\r\n",Gangs[gangid][gang_upgrade_friendlyfire]); fwrite(file,writeit);
		format(writeit,MAX_STRING,"Upgrade_Weapon=%d\r\n",Gangs[gangid][gang_upgrade_weapon]); fwrite(file,writeit);
 		for (new gangmember = 0;gangmember<MAX_GANG_SIZE;gangmember++)
 		{
 			new memberid = GangMembers[gangid][gangmember];
  			format(writeit,MAX_STRING,"Member%d=%s\r\n",gangmember,GangMemberInfo[memberid][member_name]); fwrite(file,writeit);
 		}
		format(writeit,MAX_STRING,"Login_Message=%s\r\n",Gangs[gangid][gang_loginmessage]); fwrite(file,writeit);
		fclose(file);
	}
 #else

 new temp[MAX_STRING];
 db_BeforeBigSaving(dinifilename);
 db_Set(dinifilename,"Name",gangname);

 if (!db_Isset(dinifilename,"Tag")) db_Set(dinifilename,"Tag",Gangs[gangid][gang_tag]);

 valstr(temp,Gangs[gangid][gang_colour]);
 db_Set(dinifilename,"Colour",temp);
 set(temp,nullstr);

 valstr(temp,Gangs[gangid][gang_kills]);
 db_Set(dinifilename,"Kills",temp);
 set(temp,nullstr);

 valstr(temp,Gangs[gangid][gang_score]);
 db_Set(dinifilename,"Score",temp);
 set(temp,nullstr);

 valstr(temp,Gangs[gangid][gang_bank]);
 db_Set(dinifilename,"Bank",temp);
 set(temp,nullstr);

 valstr(temp,Gangs[gangid][gang_upgrade_armour]);
 db_Set(dinifilename,"Upgrade_Armour",temp);
 set(temp,nullstr);

 valstr(temp,Gangs[gangid][gang_upgrade_friendlyfire]);
 db_Set(dinifilename,"Upgrade_Friendlyfire",temp);
 set(temp,nullstr);

 valstr(temp,Gangs[gangid][gang_upgrade_weapon]);
 db_Set(dinifilename,"Upgrade_Weapon",temp);
 set(temp,nullstr);

 for (new gangmember = 0;gangmember<MAX_GANG_SIZE;gangmember++)
 {
 	new memberid = GangMembers[gangid][gangmember];
	format(temp,MAX_STRING,"Member%d",gangmember);
	db_Set(dinifilename,temp,GangMemberInfo[memberid][member_name]);
 }

 db_Set(dinifilename,"Login_Message",Gangs[gangid][gang_loginmessage]);
 db_AfterBigSaving(dinifilename);
 #endif
 Debug("gang.inc > gang_save_db_dini - Stop");
}

public GangUnload(gangid)
{
	Debug("gang.inc > GangUnload - Start");
 	GangSave(gangid);
	new dinifilename[MAX_STRING];
 	format(dinifilename,sizeof(dinifilename),"%s%s.txt",Gang_DB,Gangs[gangid][gang_name]);
 	if (IsGangFileEmpty(dinifilename) == 1){ //No members in gang. remove file.
		if (db_Exists(dinifilename)){ 
			db_Remove(dinifilename);
			new logstring[MAX_STRING];
			format(logstring,MAX_STRING,"gang %s (%d) removed (no members in file)",Gangs[gangid][gang_name],gangid);
			WriteLog(logstring);
		}
		
	}
 	for (new gangmember = 0;gangmember<MAX_GANG_SIZE;gangmember++)
 	{
  		new memberid = GangMembers[gangid][gangmember];
 		GangMemberInfo[memberid][member_active] = 0;
  		GangMemberInfo[memberid][member_online] = 0;
  		GangMemberInfo[memberid][member_playerid] = INVALID_PLAYER_ID;
 	}
 	Gangs[gangid][gang_active] = 0;
 	Debug("gang.inc > GangUnload - Stop");
}

public GangKill(gangid,killerid,victimid,reason)
{
 Debug("gang.inc > GangKill - Start");
 if (victimid != INVALID_PLAYER_ID)
 {
     Gangs[gangid][gang_kills]++;
 }
 if (PlayerGangid[killerid] == PlayerGangid[victimid])
    { // killed a gang member
  new string[MAX_STRING];
  format(string, MAX_STRING,gettext(959), oGetPlayerName(killerid),oGetPlayerName(victimid));
  Player[killerid][GangMateKills]++;
  SendGangMessage(PlayerGangid[killerid],string);
  Debug("gang.inc > GangKill - Stop");
  return 1;
 }
 Debug("gang.inc > GangKill - Stop");
 return 0;
}



public GangMemberLogin(playerid,gangid)
{
 Debug("gang.inc > GangMemberLogin - Start");
 if (PlayerGangid[playerid] != 0)
 {
  PlayerGangid[playerid] = 0;
 }
 if (gangid == 0)
 {
  Debug("gang.inc > GangMemberLogin - Stop");
  return 0;
 }
 new string[MAX_STRING];
 for (new gangmember = 0;gangmember<MAX_GANG_SIZE;gangmember++)
 {
  new memberid = GangMembers[gangid][gangmember]; // get memberid

  if (strcomp(GangMemberInfo[memberid][member_name],oGetPlayerName(playerid),false) == 1)
  {
   format(string, MAX_STRING,gettext(960), oGetPlayerName(playerid));
   SendGangMessage(gangid,string);
   GangMemberInfo[memberid][member_online] = 1;
   GangMemberInfo[memberid][member_playerid] = playerid;
   PlayerGangid[playerid] = gangid;
   set(PlayerGangName[playerid],Gangs[gangid][gang_name]);
   set(PlayerGangTag[playerid],Gangs[gangid][gang_tag]);
   if (Gangs[gangid][gang_upgrade_armour] > 0 && floatround(oGetPlayerArmour(GangMemberInfo[memberid][member_playerid])) < Gangs[gangid][gang_upgrade_armour]) SetPlayerArmour(playerid,Gangs[gangid][gang_upgrade_armour]);
   if (Gangs[gangid][gang_upgrade_friendlyfire] == 1) SetPlayerTeam(playerid,gangid);
   if (Gangs[gangid][gang_upgrade_weapon] > 0) GiveGangWeapon(playerid,gangid);
   format(string, MAX_STRING,gettext(961),Gangs[gangid][gang_name],GangOnlineCount(gangid)-1);
   SystemMsg(playerid,COLOUR_GANG,string);
   if (strcomp(PlayerGangTag[playerid],"No GangTag",false) == 0){
	new newname[MAX_NAME];
	new name[MAX_NAME];
	GetPlayerName(playerid, name, sizeof(name));
	format(newname,sizeof(newname),"[%s]%s",PlayerGangTag[playerid],name);
	SetPlayerName(playerid,newname);
  }
   GangInvite[playerid] = 0;
   SetPlayerColor(playerid,Gangs[gangid][gang_colour]);
   if (strlen(Gangs[gangid][gang_loginmessage])) SystemMsgFormatStr(playerid,Gangs[gangid][gang_colour],gettext(1609),Gangs[gangid][gang_loginmessage]);
   Debug("gang.inc > GangMemberLogin - Stop");
   return 1;
  }
 }
 Debug("gang.inc > GangMemberLogin - Stop");
 return 0;
}


public SendGangMessage(gangid,message[MAX_STRING])
{
 Debug("gang.inc > SendGangMessage - Stop");
 for (new playerid = 0;playerid<MAX_GANG_SIZE;playerid++)
 {
  if(PlayerGangid[playerid] != 0){
   if(PlayerGangid[playerid] == gangid) SystemMsg(playerid, Gangs[PlayerGangid[playerid]][gang_colour],message);
  }
 }
 Debug("gang.inc > SendGangMessage - Stop");
}

public GangOnlineCount(gangid)
{
 Debug("gang.inc > GangOnlineCount - Start");
 new memberscount;
 for (new gangmember = 0;gangmember<MAX_GANG_SIZE;gangmember++)
 {
  new memberid = GangMembers[gangid][gangmember]; // get memberid
  if (GangMemberInfo[memberid][member_online]) memberscount++;
 }
 Debug("gang.inc > GangOnlineCount - Stop");
 return memberscount;
}

public GangGiveXP(gangid,xpamount,giverid)
{
 Debug("gang.inc > GangGiveXP - Start");
 Gangs[gangid][gang_score] = Gangs[gangid][gang_score] + xpamount;
 new giveamount;
 giveamount = xpamount / GangOnlineCount(gangid);

 for (new playerid = 0;playerid<MAX_PLAYERS_EX;playerid++)
 {
  if (IsPlayerConnected(playerid)){
  if (PlayerGangid[playerid] != 0){
   if((PlayerGangid[playerid] == gangid) && (giverid != playerid))
   {
    GivePlayerXP(playerid,giveamount,0);
    new string[MAX_STRING];
    format(string, MAX_STRING,gettext(962),xpamount,oGetPlayerName(giverid));
    SystemMsg(playerid, COLOUR_XP_GOOD, string);
   }
  }
  }
 }
 Debug("gang.inc > GangGiveXP - Stop");
 return 0;
}


public SetGangColour(gangid,colour)
{
 Debug("gang.inc > SetGangColour - Start");
 Gangs[gangid][gang_colour] = colour;
 for (new gangmember = 0;gangmember<MAX_GANG_SIZE;gangmember++)
 {
  new memberid = GangMembers[gangid][gangmember]; // get memberid
  if(GangMemberInfo[memberid][member_online] == 1)
  {
   SetPlayerColor(GangMemberInfo[memberid][member_playerid],colour);
  }
 }
 Debug("gang.inc > SetGangColour - Stop");
}

public Gang_OnPlayerText(playerid,text[]){
	if (PlayerGangid[playerid] == 0) return 0;
	if(text[0] == '!'){
	    	new gangmessage[MAX_STRING];
            	strmid(gangmessage,text,1,strlen(text));
	    	if (isempty(gangmessage)) return 0;
	    	format(gangmessage, sizeof(gangmessage), gettext(895), oGetPlayerName(playerid),gangmessage);
        	SendGangMessage(PlayerGangid[playerid],gangmessage);
	    	WriteChatLog(playerid,text,"gang");
            	return 1;
	}
	return 0;
}

stock IsGangFileEmpty(filename[]){
	if (!db_Exists(filename)) return true;
	new dinistring[MAX_STRING];
 	for (new gangmember = 0;gangmember<MAX_GANG_SIZE;gangmember++)
 	{
  		format(dinistring,MAX_STRING,"Member%d" ,gangmember);
  		if(strlen(db_Get(filename,dinistring)) > 0) return false;
 	}
	return true;
}

stock GiveGangWeapon(playerid,gangid){
	new slot = GetWeaponSlot(Gangs[gangid][gang_upgrade_weapon]);
	if (slot == -1) return 1;
	if (PlayerWeapons[playerid][slot][wid] != Gangs[gangid][gang_upgrade_weapon]){
		if (IsPlayerAllowedWeapon(playerid, Gangs[gangid][gang_upgrade_weapon])){
			if (!IsPlayerInAnyDM(playerid) && !GetPlayerRace(playerid)){
				oGivePlayerWeapon(playerid,Gangs[gangid][gang_upgrade_weapon],floatround(floatdiv(GetWeaponMaxAmmo(Gangs[gangid][gang_upgrade_weapon]),GangTotalMembersCount(gangid))));
			}
		}
	}
	return 1;
}

stock GangTotalMembersCount(gangid)
{
 	Debug("gang.inc > GangTotalMembersCount - Start");
 	new memberscount;
 	for (new gangmember = 0;gangmember<MAX_GANG_SIZE;gangmember++)
 	{
  		new memberid = GangMembers[gangid][gangmember]; // get memberid
  		if (strlen(GangMemberInfo[memberid][member_name]) > 0) memberscount++;
 	}
	if (memberscount == 0) memberscount = 1; //I need this to defeat "Divizion by zero" :)
 	Debug("gang.inc > GangTotalMembersCount - Stop");
 	return memberscount;
}