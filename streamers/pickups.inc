//Dynamic pickups module.
//Created by MadCat
//Based on Object Streamer by MidoBan

#define PICKUP_BRASS_KNUCKLES 331
#define PICKUP_GOLFCLUB 333
#define PICKUP_NITESTICK 334
#define PICKUP_KNIFE 335
#define PICKUP_BAT 336
#define PICKUP_SHOVEL 337
#define PICKUP_POOLSTICK 338
#define PICKUP_KATANA 338
#define PICKUP_CHAINSAW 341
#define PICKUP_DILDO 321
#define PICKUP_DILDO2 322
#define PICKUP_VIBRATOR1 323
#define PICKUP_VIBRATOR2 324
#define PICKUP_FLOWER 325
#define PICKUP_CANE 326
#define PICKUP_GRENADE 342
#define PICKUP_TEARGAS 343
#define PICKUP_MOLOTOV 344
#define PICKUP_MISSILE 345
#define PICKUP_COLT45 346
#define PICKUP_SILENCED 347
#define PICKUP_DEAGLE 348
#define PICKUP_SHOTGUN 349
#define PICKUP_SAWEDOFF 350
#define PICKUP_SHOTGSPA 351
#define PICKUP_UZI 352
#define PICKUP_MP5 353
#define PICKUP_HYDRAFLARE 354
#define PICKUP_AK47 355
#define PICKUP_M4 356
#define PICKUP_TEC9 372
#define PICKUP_RIFLE 357
#define PICKUP_SNIPER 358
#define PICKUP_ROCKETLAUNCHER 359
#define PICKUP_HEATSEEKER 360
#define PICKUP_FLAMETHROWER 361
#define PICKUP_MINIGUN 362
#define PICKUP_SACHEL 363
#define PICKUP_BOMB 364  // actually the detonator
#define PICKUP_SPRAYCAN 365
#define PICKUP_FIREEXTINGUISHER 366
#define PICKUP_CAMERA 367
#define PICKUP_NIGHTVISION_GOGGLES 368
#define PICKUP_THERMAL_GOGGLES 369
#define PICKUP_PARA 371
#define PICKUP_PACK 3013

forward Pickups_OnGameModeInit();

new TimerPickupStream;

#define MAX_STREAM_PICKUPS 4560 //3000 - Money drop, 1560 - Ingame
#define MAX_PICKUP_VIEW_DISTANCE 150.0
#define MAX_GAME_PICKUPS MAX_PICKUPS-48 //Really 2048 

enum pickup_enum
{
	pmodelid,
	ptype,
	Float:xpos,
	Float:ypos,
	Float:zpos,
	bool:spawned,
	ingameid
}

new Pickups[MAX_STREAM_PICKUPS][pickup_enum];
new StreamPickupsCount;
new PickupsSpawned;

forward CreateStreamPickup(modelid2,type2,Float:xpos2,Float:ypos2,Float:zpos2);
public CreateStreamPickup(modelid2,type2,Float:xpos2,Float:ypos2,Float:zpos2)
{
	new i;
	for (i=0; i<MAX_STREAM_PICKUPS; i++)
	{
	    	if (!IsPickupInStreamArray(i))
	    	{
	        	Pickups[i][pmodelid] = modelid2;
			Pickups[i][ptype] = type2;
			Pickups[i][xpos] = xpos2;
			Pickups[i][ypos] = ypos2;
			Pickups[i][zpos] = zpos2;
			Pickups[i][spawned] = false;
			Pickups[i][ingameid] = 0;
			if (StreamPickupsCount < i) StreamPickupsCount = i;
			break;
		}
	}
	return i;
}

forward DestroyStreamPickup(id2);
public DestroyStreamPickup(id2)
{
	if (IsPickupInStreamArray(id2)){
		if (Pickups[id2][spawned] == true){
			if (Pickups[id2][ingameid] != 0){
				Pickups[id2][spawned] = false;
				DestroyPickup(Pickups[id2][ingameid]);
				Pickups[id2][ingameid] = 0;
				PickupsSpawned--;
			}
		}
		Pickups[id2][pmodelid] = 0;
		return true;
	}
	return false;
}

public Pickups_OnGameModeInit()
{
	TimerPickupStream = SetTimer("PickupStreamTimer", 500, 1);
	new logstring[MAX_STRING];
	format(logstring,sizeof(logstring),"Pickups Streamer Loaded. Pickups: %d",StreamPickupsCount+1);
	WriteLog(logstring);
	return 1;
}

forward PickupStreamTimer();
public PickupStreamTimer()
{
	new bool:IsPlayerNearPickup[MAX_STREAM_PICKUPS] = false;
	for (new p=0; p<=StreamPickupsCount; p++)
	{
		if (IsPickupInStreamArray(p))
		{
			for (new playerid=0; playerid<MAX_PLAYERS_EX; playerid++)
			{
	    			if (IsPlayerConnected(playerid) && PlayerSpawned[playerid] == true)
	    			{
					if(IsPlayerInRangeOfPoint(playerid,MAX_PICKUP_VIEW_DISTANCE,Pickups[p][xpos],Pickups[p][ypos],Pickups[p][zpos])){
						IsPlayerNearPickup[p] = true;
						break;
					}
				}
			}
			if (IsPlayerNearPickup[p] == true){
				if (PickupsSpawned < MAX_GAME_PICKUPS){
					if (Pickups[p][spawned] == false){
						Pickups[p][spawned] = true;
						Pickups[p][ingameid] = CreatePickup(Pickups[p][pmodelid],Pickups[p][ptype],Pickups[p][xpos],Pickups[p][ypos],Pickups[p][zpos]);
						PickupsSpawned++;
					}
				}
			} else {
				if (Pickups[p][spawned] == true){
					if (Pickups[p][ingameid] != 0){
						Pickups[p][spawned] = false;
						DestroyPickup(Pickups[p][ingameid]);
						Pickups[p][ingameid] = 0;
						PickupsSpawned--;
					}
				}
			}
			IsPlayerNearPickup[p] = false;
		}

	}
}

forward IsPickupInStreamArray(id2);
public IsPickupInStreamArray(id2)
{
	if (Pickups[id2][pmodelid] > 0) return 1;
	return 0;
}

forward GetStreamPickupID(gamepickupid);
public GetStreamPickupID(gamepickupid)
{
	new id = -1;
	for (new i=0; i<MAX_STREAM_PICKUPS; i++)
	{
	    	if (IsPickupInStreamArray(i))
	    	{
	        	if (Pickups[i][ingameid] == gamepickupid){
				id = i;
				break;
			}
		}
	}
	return id;
}

//0 - The pickup does not display.
//2 - Pickupable, respawns after some time.
//3 - Pickupable, but doesn't respawn.
//4 - Disappears shortly after created (perhaps for weapon drops?)
//5 - Disappears shortly after created (perhaps for weapon drops?)
//8 - Pickupable, but has no effect. Disappears automatically.
//11 - Blows up a few seconds after being created (bombs?)
//12 - Blows up a few seconds after being created.
//13 - Slowly decends to the ground.
//14 - Pickupable, but only when in a vehicle. Falls through objects made with CreateObject, etc.
//15 - Pickupable, but doesn't respawn.
//19 - Pickupable, but has no effect (information icons?)
//22 - Pickupable, but doesn't respawn.
//23 - Pickupable, but doesn't disappear on pickup.