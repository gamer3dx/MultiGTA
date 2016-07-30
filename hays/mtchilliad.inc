// Converted:    08.09.08
// Aurthor:    Elyks
// Converter: MadCat

#if defined _hay_mtchilliad_included
#endinput
#endif

#define _hay_mtchilliad_included
#pragma library hay_mtchilliad
#include "hay"

static name[MAX_NAME] = "Mount Chilliad";
static minplayers = 3;
static minlevel = 16; 
static maxtime = 300;
static frequency = 5;
static lineupdelay = 240; 
static cashprizeperlevel = 1600;
static cashentry = 1600;
static xpprizeperlevel = 800;
static levels = 20;
static objects = 90;
static movespersecond = 8;
static maxblocksperlevel = 7;
static minblocksperlevel = 2;
static Float:startcp[CoordInfo] = {-2329.0,-1636.0,481.0};

static hayid; // this will hold the id this hay is assigned at init

forward hay_mtchilliad_init();

public hay_mtchilliad_init() // unique named init function (must be ran on gamemode init)
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
 	

