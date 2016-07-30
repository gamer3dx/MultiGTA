//Core.
//Created by Iain Gilbert
//Modder: MadCat
//Version info idea from Las Venturas Arena. World bounds and INFINITY define by Y_Less

#define dcmd(%1,%2,%3) if ((strcmp((%3)[1], #%1, true, (%2)) == 0) && ((((%3)[(%2) + 1] == 0) && (dcmd_%1(playerid, "")))||(((%3)[(%2) + 1] == 32) && (dcmd_%1(playerid, (%3)[(%2) + 2]))))) return 1

#define VERSION_MAJOR		2
#define VERSION_MINOR		8
#define VERSION_BUILD		4

#define INFINITY (Float:0x7F800000)

#pragma tabsize 0

#define MAX_STRING 255
#define MAX_NAME MAX_PLAYER_NAME
#define INACTIVE_PLAYER_ID 255
#define INVALID_QUEST_ID 0
#define MAX_QUESTS 255
#define MAX_WIN 500000 // Maximum gambling win
#define MAX_COUNTDOWN 5
#define ONE_SA_DAY 1440000

#define MAX_VEHICLE_MODELS 149
#define MAX_DEFAULT_VEHICLE_SPAWNS 1970 // 18 more at derby, 6 at dirtbike, 3 - businesses trucks

#define DIALOGID_FILETELE 0
#define DIALOGID_FIGHTSTYLE 1

#define WORLD_DEFAULT 0
#define WORLD_DM 4
#define WORLD_RACE 5
#define WORLD_HAY 6
#define WORLD_PRIVATE 50

#define IP_LEN 16

#define INTERIOR_DEFAULT 0
#define INTERIOR_JET 1
#define INTERIOR_GYM 5
#define INTERIOR_JAIL 6
#define INTERIOR_DIRTBIKE 14
#define INTERIOR_DERBY 15
#define INTERIOR_CLUB 17

#define QUEST_NOQUEST 0
#define QUEST_SHOWSOMETHING MAX_QUESTS-3

new QUEST_DUEL = INVALID_QUEST_ID;

#define COLOUR_WHITE 0xFFFFFFAA
#define COLOUR_ORANGE 0xFF9900AA
#define COLOUR_IVORY 0xFFFF82FF
#define COLOUR_BLUE 0x0000FFFF
#define COLOUR_PURPLE 0x800080FF
#define COLOUR_RED 0xFF0000FF
#define COLOUR_LIGHTGREEN 0x00FF7FFF
#define COLOUR_VIOLET 0xEE82EEFF
#define COLOUR_YELLOW 0xFFFF00FF
#define COLOUR_SILVER 0xC0C0C0FF
#define COLOUR_LIGHTBLUE 0x87CEFAFF
#define COLOUR_PINK 0xFFB6C1FF
#define COLOUR_INDIGO 0x4B00B0FF
#define COLOUR_GOLD 0xFFD700FF
#define COLOUR_GREEN 0x008000FF
#define COLOUR_LIGHTYELLOW 0xFAFA02FF
#define COLOUR_GREY 0x778899FF
//stopped here
#define COLOUR_MAGENTA 0xFF00FFFF
#define COLOUR_BRIGHTGREEN 0x7CFC00FF
#define COLOUR_DARKBLUE 0x000080AFF
#define COLOUR_VIOLETRED 0xDB7093FF
#define COLOUR_BROWN 0x8B4513FF
#define COLOUR_GREENYELLOW 0xADFF2FFF
#define COLOUR_THISTLE 0xD8BFD8FF
#define COLOUR_TURQUISE 0x48D1CCFF
#define COLOUR_MAROON 0x800000FF
#define COLOUR_STEELBLUE 0xB0C4DEFF

#define COLOUR_MISC 0xAFAFAFAA // Grey - Misc
#define COLOUR_COMBAT_BAD 0xBB3333AA // Red - Combat
#define COLOUR_INVIS 0xAFAFAF00 
#define COLOUR_PLAYER 0xFFFFFFAA

//Text colours
#define COLOUR_ERROR 0xFF0000FF //red
#define COLOUR_INFO 0xFFFF00AA //yellow
#define COLOUR_OK 0x33AA33AA //green
#define COLOUR_MESSAGE_MOD 0xBFEEFFAA
#define COLOUR_GANG_CHAT 0xFFA500AA // orange
#define COLOUR_GANG 0xFF9900AA // Orange - Gang messages
#define COLOUR_GANG_BAD 0xD09900AA // Orange - Gang warning and error messages
#define COLOUR_MONEY_GOOD 0xFFFF00AA // Yellow - Money
#define COLOUR_MONEY_BAD 0xDAA520AA // Yellow - Money
#define COLOUR_XP_GOOD 0x00C7FFAA // Blue - XP
#define COLOUR_XP_BAD 0x00008BAA // Blue - XP
#define COLOUR_RACE 0x005407AA // green - dark
#define COLOUR_RACE_BAD 0x545407AA // yellow-green-dark
#define COLOUR_DM 0xFF0066AA // red - purple
#define COLOUR_DM_BAD 0xFF0066AA // red - purple
#define COLOUR_HAY COLOUR_DM
#define COLOUR_HAY_BAD COLOUR_DM_BAD
#define COLOUR_POLICE 0x3F63CCAA
#define COLOUR_MEDIC 0xFF8282AA
#define COLOUR_TAXI 0xF0E68CAA
#define COLOUR_BOXING COLOUR_INDIGO
#define COLOUR_DUEL 0xAA3333AA // Red
#define COLOUR_PHONE 0xB0C4DEAA
#define COLOUR_DERBY 0x8B7765AA

#define SetPlayerServerWorldBounds(%1) SetPlayerWorldBounds(%1,INFINITY, -INFINITY, INFINITY, -INFINITY)
#define PrivateWorld(%1) WORLD_PRIVATE+%1 

#define MAX_XP 780000
#define MAX_LEVEL 20

#define DRINK_TIME 30000 //30 seconds

#define SKIN_STANDART 250
#define SKIN_MEDIC 70
#define SKIN_COP 280
#define SKIN_GOODCOP 282
#define SKIN_STREETPATROL 283
#define SKIN_POLICEOFFICER 284
#define SKIN_SWATMEMBER 285
#define SKIN_FBIAGENT 286

#define JOB_NOJOB 0
#define JOB_COP 1
#define JOB_MEDIC 2
#define JOB_TAXIDRIVER 3

#define RADIUS_SAY 40

new WasInAmbulance[MAX_PLAYERS] = 0;

new PlayerWithCashBox = INVALID_PLAYER_ID;

enum pHaul
{
	Capacity,
	Load,
};

new PlayerHaul[MAX_VEHICLES][pHaul];

new bool:PlayerFreezed[MAX_PLAYERS];
new bool:PlayerSpawned[MAX_PLAYERS] = false;

new ConfigDB[MAX_STRING] = "MultiGTA/Config/Config.Base.txt";
new LevelDB[MAX_STRING] = "MultiGTA/Config/Config.Levels.txt";
new SecurityDB[MAX_STRING] = "MultiGTA/Config/Config.Security.txt";
new DatabaseDB[MAX_STRING] = "MultiGTA/Config/Config.Database.txt";
new Verbose_Save=-1; // do we save extra un-needed db info like weapons, races, vehicels, deathmatches, etc
new Real_Chat = 0;
new Show_Vehicles_Names = 1;
new No_Numeric_Names = 1;
new Moder_Chat_Prefix[MAX_STRING] = "#";
new Show_Chat_Bubble = 0;

new ServerLocked = 0;

new PlayerBoxing[MAX_PLAYERS];

new MedicHealTime[MAX_PLAYERS]=0;

// xyz coord info
enum CoordInfo {
    	Float:Coord_X,
    	Float:Coord_Y,
    	Float:Coord_Z
}

new WhereHeEnter[MAX_PLAYERS][CoordInfo];

new PlayerPos[MAX_PLAYERS][CoordInfo];

new KickList[MAX_PLAYERS];

new DBSeperator= 124; // seperate db fields.atm for db writing im just sending a string, which is written to a file.
new nullstr[MAX_STRING];

new bool:event_now = false;

new gamemodename[MAX_STRING];

enum LevelInfo {
	level_xp,
    	level_name[MAX_NAME]
}

// level rank names by Baby Face / Cool
new LevelList[MAX_LEVEL+1][LevelInfo] = { // experience required for each level
{  0,		"Wannabe" },
{  400,		"Balla" },
{  1000,	"Foot Criminal" },
{  2000,	"Foot Soldier" },
{  4000,	"Foot Lieutenant" },
{  8000,	"Foot Captain" },
{  14000,	"Street Criminal" },
{  24000,	"Street Soldier" },
{  38000,	"Street Lieutenant" },
{  54000,	"Street Captain" },
{  71000,	"HardCore Soldier" },
{  90000,	"1st Star General" },
{  115000,	"2nd Star General" },
{  145000,	"3rd Star General" },
{  190000,	"4th Star General" },
{  250000,	"5th Star General" },
{  330000,	"Set Leader" },
{  390000,	"Body Guard" },
{  480000,	"Left Hand Man" },
{  590000,	"Right Hand Man" },
{  720000, 	"GodFather" }
};

new MAX_PLAYERS_EX = MAX_PLAYERS;

BaseConfig()
{
	Debug("base.inc > BaseConfig - Start");
	

	new temp[MAX_STRING];

	if (!db_Exists(ConfigDB)) db_Create(ConfigDB);
	if (!db_Exists(SecurityDB)) db_Create(SecurityDB);
	if (!db_Exists(DatabaseDB)) db_Create(DatabaseDB);
	if (!db_Exists(LevelDB)) db_Create(LevelDB);

	set(temp,db_Get(ConfigDB,"Verbose_Save"));
	if (strlen(temp) > 0) Verbose_Save = strval(temp);
	else { valstr(temp,Verbose_Save); db_Set(ConfigDB,"Verbose_Save",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(ConfigDB,"Real_Chat"));
	if (strlen(temp) > 0) Real_Chat = strval(temp);
	else { valstr(temp,Real_Chat); db_Set(ConfigDB,"Real_Chat",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(ConfigDB,"Show_Vehicles_Names"));
	if (strlen(temp) > 0) Show_Vehicles_Names = strval(temp);
	else { valstr(temp,Show_Vehicles_Names); db_Set(ConfigDB,"Show_Vehicles_Names",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(ConfigDB,"No_Numeric_Names"));
	if (strlen(temp) > 0) No_Numeric_Names = strval(temp);
	else { valstr(temp,No_Numeric_Names); db_Set(ConfigDB,"No_Numeric_Names",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(ConfigDB,"Moder_Chat_Prefix"));
	if (strlen(temp) > 0) set(Moder_Chat_Prefix,temp);
	else { set(temp,Moder_Chat_Prefix); db_Set(ConfigDB,"Moder_Chat_Prefix",temp);  }
	set(temp,nullstr);

	set(temp,db_Get(ConfigDB,"Show_Chat_Bubble"));
	if (strlen(temp) > 0) Show_Chat_Bubble = strval(temp);
	else { valstr(temp,Show_Chat_Bubble); db_Set(ConfigDB,"Show_Chat_Bubble",temp);  }
	set(temp,nullstr);

	for (new i=0;i<=MAX_LEVEL;i++)
	{
		new tempname[MAX_STRING];
	    	format(tempname,sizeof(tempname),"Level%d_Rankname",i);
		set(temp,db_Get(LevelDB,tempname));
		if (strlen(temp) > 0) set(LevelList[i][level_name],temp);
		else db_Set(LevelDB,tempname,LevelList[i][level_name]);
		
		format(tempname,sizeof(tempname),"Level%d_Experience",i);
		set(temp,db_Get(LevelDB,tempname));
		if (strlen(temp) > 0) LevelList[i][level_xp] = strval(temp);
		else {valstr(temp,LevelList[i][level_xp]); db_Set(LevelDB,tempname,temp);}
	}

	Debug("base.inc > BaseConfig - Stop");
	return;
}

GetLevelName(level)
{
	Debug("base.inc > GetLevelName - Start");
	new name[MAX_NAME];
	if ((level < 0) || (level > MAX_LEVEL+1)) return name;
	set(name,LevelList[level][level_name]);
	Debug("base.inc > GetLevelName - Stop");
	return name;
}

forward Base_OnGameModeInit();

public Base_OnGameModeInit(){
	BaseConfig();
	MAX_PLAYERS_EX = GetMaxPlayers();
	set(gamemodename,"MultiGTA v" #VERSION_MAJOR "." #VERSION_MINOR "." #VERSION_BUILD "");
	SetGameModeText(gamemodename);
	//Init of 0.1 Functions
	ShowPlayerMarkers(PLAYER_MARKERS_MODE_GLOBAL);
	ShowNameTags(1);
	EnableStuntBonusForAll(0);
	//Init of new 0.2 Functions
	UsePlayerPedAnims();
	AllowAdminTeleport(1);
	DisableInteriorEnterExits();
	//EnableZoneNames(true);
}

forward ShowGameModeName(playerid);

public ShowGameModeName(playerid){
	ShowTextForPlayer(playerid,"Multi~r~GTA:~n~~w~San Andreas",5000,5);
}


#include "language"