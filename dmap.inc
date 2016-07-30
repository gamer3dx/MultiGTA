/*
 *            DMap 0.3
 *       (c) Copyright 2008 by DracoBlue
 *
 * @author    : DracoBlue (http://dracoblue.com)
 * @date      : 26th Aug 2008
 * @update    : 21st Sep 2008
 * @requires djson 1.5.1
 *
 * This file is provided as is (no warranties).
 *
 * Feel free to use it, a little message in
 * about box is honouring thing, isn't it?
 *
 *   Modifications for MultiGTA by MadCat
 */

#include "utils\dbs\djson"

forward dmap_TIMER();

new Use_DMap = 0;

new DMAP_last_icon = 0;

new dmap_saved[MAX_PLAYERS];

public dmap_TIMER() {
	new tmp[255];
	new tmp2[255];

	djAutocommit(false);
	for (new i=0;i<MAX_PLAYERS_EX;i++) {
		if (IsPlayerConnected(i)) {
			if (!dmap_saved[i]) {
				dmap_saved[i] = true;

				format(tmp,255,"items/p%d/id",i);
				djSetInt("dmap_feed.json",tmp,i);

				format(tmp,255,"items/p%d/name",i);
				djSet("dmap_feed.json",tmp,oGetPlayerName(i));

				format(tmp,255,"items/p%d/icon",i);
				djSet("dmap_feed.json",tmp,"58");
			}

			format(tmp2,255,"Health: %d",floatround(oGetPlayerHealth(i)));
			format(tmp,255,"items/p%d/text",i);
			djSet("dmap_feed.json",tmp,tmp2);

			format(tmp,255,"items/p%d/pos/x",i);
			djSetInt("dmap_feed.json",tmp,floatround(PlayerPos[i][Coord_X]));
			format(tmp,255,"items/p%d/pos/y",i);
			djSetInt("dmap_feed.json",tmp,floatround(PlayerPos[i][Coord_Y]));

		} else if (dmap_saved[i]) {
			// saved, but not connected, remove him!
			format(tmp,255,"items/p%d",i);
			djUnset("dmap_feed.json",tmp);
			dmap_saved[i] = false;
		}
	}
	djCommit("dmap_feed.json");
	djAutocommit(true);
}

dmap_Config()
{
	new temp[MAX_STRING];
	if (!db_Exists(ConfigDB)) db_Create(ConfigDB);
	
	set(temp,db_Get(ConfigDB,"Use_DMap"));
	if (strlen(temp) > 0) Use_DMap = strval(temp);
	else { valstr(temp,Use_DMap); db_Set(ConfigDB,"Use_DMap",temp);  }
	set(temp,nullstr);

	return;
}


stock CreateDMapIcon(icon_id,Float:x,Float:y,Float:z,name[],text[]) {
	if (Use_DMap == 0) return;
	djAutocommit(false);
	#pragma unused z
	new tmp[DJSON_MAX_STRING];
	DMAP_last_icon++;
	format(tmp,DJSON_MAX_STRING,"items/i%d/id",DMAP_last_icon);
	djSetInt("dmap_feed.json",tmp,DMAP_last_icon);
	format(tmp,DJSON_MAX_STRING,"items/i%d/name",DMAP_last_icon);
	djSet("dmap_feed.json",tmp,name);
	format(tmp,DJSON_MAX_STRING,"items/i%d/text",DMAP_last_icon);
	djSet("dmap_feed.json",tmp,text);
	format(tmp,DJSON_MAX_STRING,"items/i%d/icon",DMAP_last_icon);
	djSetFloat("dmap_feed.json",tmp,icon_id);
	format(tmp,DJSON_MAX_STRING,"items/i%d/pos/x",DMAP_last_icon);
	djSetInt("dmap_feed.json",tmp,floatround(x));
	format(tmp,DJSON_MAX_STRING,"items/i%d/pos/y",DMAP_last_icon);
	djSetInt("dmap_feed.json",tmp,floatround(y));
	djCommit("dmap_feed.json");
	djAutocommit(true);
}

dmap_OnGameModeInit() {
	//dmap_Config();
	if (Use_DMap == 1){
		WriteLog("<dmap> DMap 0.3 // (c) 2008 DracoBlue // http://www.dracoblue.net");
		WriteLog("<dmap> Loading ...");
		djCreateFile("dmap_feed.json");
		SetTimer("dmap_TIMER",5000,1);
		WriteLog("<dmap> Loaded!");
	}
}