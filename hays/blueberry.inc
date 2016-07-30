// Converted:    08.09.08
// Aurthor:    Elyks
// Converter: MadCat

#if defined _hay_blueberry_included
#endinput
#endif

#define _hay_blueberry_included
#pragma library hay_blueberry
#include "hay"

static name[MAX_NAME] = "BlueBerry";
static minplayers = 3;
static minlevel = 12; 
static maxtime = 300;
static frequency = 5;
static lineupdelay = 240; 
static cashprizeperlevel = 1200;
static cashentry = 1200;
static xpprizeperlevel = 600;
static levels = 15;
static objects = 80;
static movespersecond = 6;
static maxblocksperlevel = 7;
static minblocksperlevel = 2;
static Float:startcp[CoordInfo] = {-8.0,-40.0,1.0};

static hayid; // this will hold the id this hay is assigned at init

forward hay_blueberry_init();

public hay_blueberry_init() // unique named init function (must be ran on gamemode init)
{
	hayid = RegisterHay(name);
	if (hayid == INVALID_HAY_ID) return;
 	Hay[hayid][hay_minlevel] = minlevel;
 	Hay[hayid][hay_minplayers] = minplayers;
	Hay[hayid][hay_maxtime] = maxtime;
	Hay[hayid][hay_frequency] = frequency;
 	Hay[hayid][hay_lineupdelay] = lineupdelay;
 	Hay[hayid][hay_cashprizeperlevel] = cashprizeperlevel;
	Hay[hayid][hay_cashentry] = cashentry;
	Hay[hayid][hay_xpprizeperlevel] = xpprizeperlevel;
	Hay[hayid][hay_levels] = levels;
	Hay[hayid][hay_objects] = objects;
	Hay[hayid][hay_movespersecond] = movespersecond;
	Hay[hayid][hay_maxblocksperlevel] = maxblocksperlevel;
	Hay[hayid][hay_minblocksperlevel] = minblocksperlevel;

	HayStats[hayid][hay_timer] = MakeHaySleepTime(hayid);
	HayStats[hayid][hay_state] = HAY_STATE_SLEEPING;
	
	HayStartCP[hayid][Coord_X] = startcp[Coord_X];
	HayStartCP[hayid][Coord_Y] = startcp[Coord_Y];
	HayStartCP[hayid][Coord_Z] = startcp[Coord_Z];

}
 	

