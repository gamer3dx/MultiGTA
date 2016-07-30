//////////////////////////////////////
/// Icon Control                   ///
///                                ///
///                                ///
///                                ///
///          Copyright by Rafelder ///
///	     Modified by MadCat    ///
//////////////////////////////////////

#define MAX_ICONS 260
#define MAX_SHOWED_MAPICONS 80 //Really 100

new Float:XM[MAX_ICONS];
new Float:YM[MAX_ICONS];
new Float:ZM[MAX_ICONS];

new MarkerM[MAX_ICONS];

new IsValid[MAX_ICONS];
new Show[MAX_PLAYERS][MAX_ICONS];

new IconidM=-1;
new Count=0;
new ShowedForPlayer[MAX_PLAYERS];
forward MapIconsTimer();

new TimerMapIcons;

stock mapicons_OnGameModeInit(){
	TimerMapIcons = SetTimer("MapIconsTimer", 1000, 1);
	new logstring[MAX_STRING];
	format(logstring,sizeof(logstring),"Mapicons Streamer Loaded. Map icons: %d",IconidM);
	WriteLog(logstring);
	return 0;
}

stock CreateStreamMapIcon(markerid, Float:X, Float:Y, Float:Z)
{
	if (IconidM >= MAX_ICONS) return 0;
	IconidM++;
	IsValid[IconidM]=1;
	XM[IconidM]=X;
	YM[IconidM]=Y;
	ZM[IconidM]=Z;
	MarkerM[IconidM]=markerid;
	for(new i=0; i<MAX_PLAYERS_EX; i++) {
	Show[i][IconidM]=1;
	}
	return IconidM;
}

stock ResetMapIconVariables(playerid)
{
	for(new i=0; i<MAX_ICONS; i++) {
	Show[playerid][i]=1;
	}
	return 0;
}

stock IsValidMapIcon(iconid)
{
	if (IsValid[iconid] == 1) return 1;
	return 0;
}


stock GetMapIconPos(iconid, &Float:X, &Float:Y, &Float:Z)
{
	if (!IsValidMapIcon(iconid)) return 0;
	X=XM[iconid];
	Y=YM[iconid];
	Z=ZM[iconid];
	return 1;
}

public MapIconsTimer()
{
	for(new i=0; i<MAX_PLAYERS_EX; i++) {
		if (IsPlayerConnected(i)) {
			if (PlayerSpawned[i] == true){ //Connected and spawned - show icons.
				ShowedForPlayer[i]=0;
				for(new oldm=0; oldm<MAX_SHOWED_MAPICONS; oldm++) {RemovePlayerMapIcon(i, oldm);}
				Count=0;
				for(new m=0; m<MAX_ICONS; m++) {
					if (IsValidMapIcon(m)) {
						if (ShowedForPlayer[i] < MAX_SHOWED_MAPICONS) {
							if (Show[i][m] == 1) {
								if (GetDistanceToMapIcon(i, m) <= 400) {
									SetPlayerMapIcon(i, Count, XM[m], YM[m], ZM[m], MarkerM[m],COLOUR_WHITE);
									ShowedForPlayer[i]++;
									Count++;
								}
							
							}
						}
					}
				}
			}
		}
	}
	return 1;
}
stock GetDistanceToMapIcon(playerid, iconid)
{
	new Float:x2,Float:y2,Float:z2;
	new Float:output;
	GetMapIconPos(iconid, x2, y2, z2);
	output = floatsqroot(floatpower(floatabs(floatsub(x2, PlayerPos[playerid][Coord_X])), 2)+floatpower(floatabs(floatsub(y2, PlayerPos[playerid][Coord_Y])), 2));
	return floatround(output);
}

stock GetClosestStreamMapIcon(playerid)
{
	new x,Float:dis,Float:dis2,iconid;
	iconid = -1;
	dis = 99999.99;
	for (x=0;x<MAX_ICONS;x++){
		if (IsValidMapIcon(x)){
			dis2 = GetDistanceToMapIcon(playerid,x);
			if (dis2 < dis && dis2 != -1.00){
				dis = dis2;
				iconid = x;
			}
		}
	}
	return iconid;
}

stock ChangeStreamMapIconModel(streamiconid,newmodel)
{
	if (IsValidMapIcon(streamiconid)){
		MarkerM[streamiconid] = newmodel;
	}
}