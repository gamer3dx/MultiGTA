//========================================================//
//=============[MidoStream - Object Streamer]=============//
//=====================[by MidoBan]=======================//
//Modifications for MultiGTA by MadCat

forward Objects_OnGameModeInit();
forward Objects_OnPlayerDisconnect(playerid,reason);

new TimerObjectStream;

#define MAX_STREAM_OBJECTS 1500
#define MAX_OBJECT_VIEW_DISTANCE 150.0
#define MAX_OBJECTS_FOR_PLAYER MAX_OBJECTS-10

enum object_enum
{
	modelid,
	Float:xpos,
	Float:ypos,
	Float:zpos,
	Float:xrot,
	Float:yrot,
	Float:zrot,
	virtualworld,
	attached,
	Float:xoff,
	Float:yoff,
	Float:zoff,
	bool:moving,
	Float:movx,
	Float:movy,
	Float:movz,
	Float:speed,
	movetimer
}

enum player_enum
{
	pobjects[MAX_STREAM_OBJECTS],
	bool:seen[MAX_STREAM_OBJECTS],
	playerobjectscount
}

new Objects[MAX_STREAM_OBJECTS][object_enum];
new PlayerObjects[MAX_PLAYERS][player_enum];
new ObjectsCount;

forward CreateStreamObject(modelid2,Float:xpos2,Float:ypos2,Float:zpos2,Float:xrot2,Float:yrot2,Float:zrot2,virtualworld2);
public CreateStreamObject(modelid2,Float:xpos2,Float:ypos2,Float:zpos2,Float:xrot2,Float:yrot2,Float:zrot2,virtualworld2)
{
	new i;
	for (i=0; i<MAX_STREAM_OBJECTS; i++)
	{
	    	if (!IsCreatedStreamObject(i))
	    	{
	        	Objects[i][modelid] = modelid2;
			Objects[i][xpos] = xpos2;
			Objects[i][ypos] = ypos2;
			Objects[i][zpos] = zpos2;
			Objects[i][xrot] = xrot2;
			Objects[i][yrot] = yrot2;
			Objects[i][zrot] = zrot2;
			Objects[i][virtualworld] = virtualworld2;
			Objects[i][attached] = -1;
			Objects[i][moving] = false;
			if (ObjectsCount < i) ObjectsCount = i;
			for (new playerid=0; playerid<MAX_PLAYERS_EX; playerid++)
			{
	    			if (IsPlayerConnected(playerid))
	    			{
					if (IsPlayerInRangeOfPoint(playerid,MAX_OBJECT_VIEW_DISTANCE,Objects[i][xpos],Objects[i][ypos],Objects[i][zpos]) && IsPlayerAndObjectInSameWorld(playerid,i))
					{
						if (PlayerObjects[playerid][seen][i] == false)
						{
							if (PlayerObjects[playerid][playerobjectscount] < MAX_OBJECTS_FOR_PLAYER){
					        		PlayerObjects[playerid][pobjects][i] = CreatePlayerObject(playerid,Objects[i][modelid],Objects[i][xpos],Objects[i][ypos],Objects[i][zpos],Objects[i][xrot],Objects[i][yrot],Objects[i][zrot]);
					        		PlayerObjects[playerid][playerobjectscount]++;
								PlayerObjects[playerid][seen][i] = true;
							}
						}
					}
				}
			}
			break;
		}
	}
	return i;
}

forward Float:GetXPos(id2);
public Float:GetXPos(id2)
{
	return Objects[id2][xpos];
}

forward Float:GetYPos(id2);
public Float:GetYPos(id2)
{
	return Objects[id2][ypos];
}

forward Float:GetZPos(id2);
public Float:GetZPos(id2)
{
	return Objects[id2][zpos];
}

forward Float:GetXRot(id2);
public Float:GetXRot(id2)
{
	return Objects[id2][xrot];
}

forward Float:GetYRot(id2);
public Float:GetYRot(id2)
{
	return Objects[id2][yrot];
}

forward Float:GetZRot(id2);
public Float:GetZRot(id2)
{
	return Objects[id2][zrot];
}

forward DestroyStreamObject(id2);
public DestroyStreamObject(id2)
{
	if (IsCreatedStreamObject(id2)){
		Objects[id2][modelid] = 0;
		KillTimer(Objects[id2][movetimer]);
		for (new i=0; i<MAX_PLAYERS_EX; i++)
		{
			if (IsPlayerConnected(i))
		    	{
			    	if (PlayerObjects[i][seen][id2] == true)
			    	{
					DestroyPlayerObject(i,PlayerObjects[i][pobjects][id2]);
		        		PlayerObjects[i][seen][id2] = false;
					PlayerObjects[i][playerobjectscount]--;
				}
			}
		}
	}
}

forward SetStreamObjectPos(id2,Float:xpos2,Float:ypos2,Float:zpos2);
public SetStreamObjectPos(id2,Float:xpos2,Float:ypos2,Float:zpos2)
{
	if (IsCreatedStreamObject(id2)){
	    	Objects[id2][xpos] = xpos2;
    		Objects[id2][ypos] = ypos2;
    		Objects[id2][zpos] = zpos2;
    		for (new i=0; i<MAX_PLAYERS_EX; i++)
    		{
    	    		if (IsPlayerConnected(i))
    	    		{	
       	 			if (PlayerObjects[i][seen][id2] == true) SetPlayerObjectPos(i,PlayerObjects[i][pobjects][id2],xpos2,ypos2,zpos2);
			}
		}
	}
}

forward SetStreamObjectRot(id2,Float:xrot2,Float:yrot2,Float:zrot2);
public SetStreamObjectRot(id2,Float:xrot2,Float:yrot2,Float:zrot2)
{
	if (IsCreatedStreamObject(id2)){
	    	Objects[id2][xrot] = xrot2;
	    	Objects[id2][yrot] = yrot2;
	    	Objects[id2][zrot] = zrot2;
	    	for (new i=0; i<MAX_PLAYERS_EX; i++)
	    	{
	        	if (IsPlayerConnected(i))
	        	{
	        		if (PlayerObjects[i][seen][id2] == true) SetPlayerObjectRot(i,PlayerObjects[i][pobjects][id2],xrot2,yrot2,zrot2);
			}
		}
	}
}

forward AttachStreamObjectToPlayer(id2,playerid,Float:xoff2,Float:yoff2,Float:zoff2,Float:xrot2,Float:yrot2,Float:zrot2);
public AttachStreamObjectToPlayer(id2,playerid,Float:xoff2,Float:yoff2,Float:zoff2,Float:xrot2,Float:yrot2,Float:zrot2)
{
	if (IsCreatedStreamObject(id2)){
	    	Objects[id2][attached] = playerid;
    		Objects[id2][xoff] = xoff2;
    		Objects[id2][yoff] = yoff2;
    		Objects[id2][zoff] = zoff2;
    		Objects[id2][xrot] = xrot2;
    		Objects[id2][yrot] = yrot2;
    		Objects[id2][zrot] = zrot2;
    		for (new i=0; i<MAX_PLAYERS_EX; i++)
    		{
       	 		if (IsPlayerConnected(i))
       	 		{
       	 			if (PlayerObjects[i][seen][id2] == true) AttachPlayerObjectToPlayer(i,PlayerObjects[i][pobjects][id2],playerid,xoff2,yoff2,zoff2,xrot2,yrot2,zrot2);
			}
		}
	}
}

forward MoveStreamObject(id2,Float:movx2,Float:movy2,Float:movz2,Float:speed2);
public MoveStreamObject(id2,Float:movx2,Float:movy2,Float:movz2,Float:speed2)
{
	if (IsCreatedStreamObject(id2)){
	    	Objects[id2][moving] = true;
	    	Objects[id2][movx] = movx2;
	    	Objects[id2][movy] = movy2;
	    	Objects[id2][movz] = movz2;
	    	Objects[id2][speed] = speed2;
	    	for (new i=0; i<MAX_PLAYERS_EX; i++)
	    	{
	        	if (IsPlayerConnected(i))
	        	{
	        		if (PlayerObjects[i][seen][id2] == true) MovePlayerObject(i,PlayerObjects[i][pobjects][id2],movx2,movy2,movz2,speed2);
			}
		}
		new Float:time = (GetPointDistanceToPointEx(Objects[id2][xpos],Objects[id2][ypos],Objects[id2][zpos],movx2,movy2,movz2)/speed2)/1.17;
		new bool:xisbigger = (movx2 >= Objects[id2][xpos]) ? true : false;
		new bool:yisbigger = (movy2 >= Objects[id2][ypos]) ? true : false;
		new bool:zisbigger = (movz2 >= Objects[id2][zpos]) ? true : false;
		new xadd = (time == 0.0) ? 0 : (xisbigger) ? floatround(((movx2 - Objects[id2][xpos])/time),floatround_ceil) : floatround(((movx2 - Objects[id2][xpos])/time),floatround_floor);
		new yadd = (time == 0.0) ? 0 : (yisbigger) ? floatround(((movy2 - Objects[id2][ypos])/time),floatround_ceil) : floatround(((movy2 - Objects[id2][ypos])/time),floatround_floor);
		new zadd = (time == 0.0) ? 0 : (zisbigger) ? floatround(((movz2 - Objects[id2][zpos])/time),floatround_ceil) : floatround(((movz2 - Objects[id2][zpos])/time),floatround_floor);
		KillTimer(Objects[id2][movetimer]);
		Objects[id2][movetimer] = SetTimerEx("MoveTimer",1000,1,"iiiibbb",id2,xadd,yadd,zadd,xisbigger,yisbigger,zisbigger);
	}
}

public Objects_OnPlayerDisconnect(playerid,reason)
{
	#pragma unused reason
	for (new i=0; i<=ObjectsCount; i++)
	{
	    	if (PlayerObjects[playerid][seen][i] == true)
	    	{
	       		DestroyPlayerObject(playerid,PlayerObjects[playerid][pobjects][i]);
	        	PlayerObjects[playerid][seen][i] = false;
			PlayerObjects[playerid][playerobjectscount]--;
		}
	}
}

public Objects_OnGameModeInit()
{
	TimerObjectStream = SetTimer("ObjectStreamTimer", 500, 1);
	new logstring[MAX_STRING];
	format(logstring,sizeof(logstring),"Objects Streamer Loaded. Objects: %d",ObjectsCount+1);
	WriteLog(logstring);
	return 1;
}

forward ObjectStreamTimer();
public ObjectStreamTimer()
{
	for (new playerid=0; playerid<MAX_PLAYERS_EX; playerid++)
	{
	    if (IsPlayerConnected(playerid))
	    {
			for (new p=0; p<=ObjectsCount; p++)
			{
			    if (IsCreatedStreamObject(p))
			    {
			        if (Objects[p][attached] != -1)
			        {
			            Objects[p][xpos] = PlayerPos[Objects[p][attached]][Coord_X]+Objects[p][xoff];
			            Objects[p][ypos] = PlayerPos[Objects[p][attached]][Coord_Y]+Objects[p][yoff];
			            Objects[p][zpos] = PlayerPos[Objects[p][attached]][Coord_Z]+Objects[p][zoff];
				}
				if (GetPlayerDistanceToPointEx(playerid,Objects[p][xpos],Objects[p][ypos],Objects[p][zpos]) < MAX_OBJECT_VIEW_DISTANCE && IsPlayerAndObjectInSameWorld(playerid,p))
				{
					if (PlayerObjects[playerid][seen][p] == false)
				    	{
						if (PlayerObjects[playerid][playerobjectscount] < MAX_OBJECTS_FOR_PLAYER){
					        	PlayerObjects[playerid][pobjects][p] = CreatePlayerObject(playerid,Objects[p][modelid],Objects[p][xpos],Objects[p][ypos],Objects[p][zpos],Objects[p][xrot],Objects[p][yrot],Objects[p][zrot]);
					        	PlayerObjects[playerid][playerobjectscount]++;
							if (Objects[p][attached] != -1)
					        	{
					        		AttachPlayerObjectToPlayer(playerid,PlayerObjects[playerid][pobjects][p],Objects[p][attached],Objects[p][xoff],Objects[p][yoff],Objects[p][zoff],Objects[p][xrot],Objects[p][yrot],Objects[p][zrot]);
							}
							else if (Objects[p][moving] == true)
							{
								MovePlayerObject(playerid,PlayerObjects[playerid][pobjects][p],Objects[p][movx],Objects[p][movy],Objects[p][movz],Objects[p][speed]);
							}
					        	PlayerObjects[playerid][seen][p] = true;
							}
						}
					}
					else if (PlayerObjects[playerid][seen][p] == true)
					{
					    	DestroyPlayerObject(playerid,PlayerObjects[playerid][pobjects][p]);
					    	PlayerObjects[playerid][seen][p] = false;
						PlayerObjects[playerid][playerobjectscount]--;
					}
				}
			}
		}
	}
}

forward MoveTimer(id2,xadd,yadd,zadd,bool:xisbigger,bool:yisbigger,bool:zisbigger);
public MoveTimer(id2,xadd,yadd,zadd,bool:xisbigger,bool:yisbigger,bool:zisbigger)
{
	new bool:reached = false;
	reached = xisbigger ? ((Objects[id2][xpos] >= Objects[id2][movx]) ? true : false) : ((Objects[id2][xpos] <= Objects[id2][movx]) ? true : false);
	if (reached)
	{
	    	reached = yisbigger ? ((Objects[id2][ypos] >= Objects[id2][movy]) ? true : false) : ((Objects[id2][ypos] <= Objects[id2][movy]) ? true : false);
	    	if (reached)
	    	{
	       		reached = zisbigger ? ((Objects[id2][zpos] >= Objects[id2][movz]) ? true : false) : ((Objects[id2][zpos] <= Objects[id2][movz]) ? true : false);
		}
	}
	if (reached)
	{
	    	Objects[id2][moving] = false;
	    	for (new playerid=0; playerid<MAX_PLAYERS_EX; playerid++)
	    	{
	        	if (IsPlayerConnected(playerid))
	        	{
	        		if (PlayerObjects[playerid][seen][id2] == true && Objects[id2][moving] == true)
	        		{
	        			MovePlayerObject(playerid,PlayerObjects[playerid][pobjects][id2],Objects[id2][movx],Objects[id2][movy],Objects[id2][movz],Objects[id2][speed]);
				}
			}
		}
		Objects[id2][xpos] = Objects[id2][movx];
		Objects[id2][ypos] = Objects[id2][movy];
		Objects[id2][zpos] = Objects[id2][movz];
		KillTimer(Objects[id2][movetimer]);
	}
	else
	{
	    	new bool:found = false;
	    	for (new playerid=0; playerid<MAX_PLAYERS_EX; playerid++)
	    	{
	       		if (IsPlayerConnected(playerid))
	        	{
		        	if (PlayerObjects[playerid][seen][id2] == true)
		        	{
		            		GetPlayerObjectPos(playerid,PlayerObjects[playerid][pobjects][id2],Objects[id2][xpos],Objects[id2][ypos],Objects[id2][zpos]);
		            		found = true;
		            		break;
	 			}
			}
		}
		if (!found)
		{
	        	Objects[id2][xpos] = Objects[id2][xpos]+xadd;
	        	Objects[id2][ypos] = Objects[id2][ypos]+yadd;
	        	Objects[id2][zpos] = Objects[id2][zpos]+zadd;
		}
	}
}

forward IsCreatedStreamObject(id2);
public IsCreatedStreamObject(id2)
{
	if (Objects[id2][modelid] > 0) return 1;
	return 0;
}

forward IsPlayerAndObjectInSameWorld(playerid,objectid);
public IsPlayerAndObjectInSameWorld(playerid,objectid)
{
	if (Objects[objectid][virtualworld] == -1) return 1; // In every world
	if (Objects[objectid][virtualworld] == GetPlayerVirtualWorld(playerid)) return 1; //Same world
	return 0;
}