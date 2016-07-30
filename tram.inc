//Tram/RC Entering
//Created by damo
//Converted for MultiGTA by madCat

forward tram_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);

public tram_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if(newkeys == KEY_SECONDARY_ATTACK ){
		if(!IsPlayerInAnyVehicle(playerid)){
			new Float:x, Float:y, Float:z, vehicle;
			GetPlayerPos(playerid, x, y, z );
			GetVehicleWithinDistance(playerid, x, y, z, 20.0, vehicle);
			if(IsVehicleRcTram(vehicle)){
				oPutPlayerInVehicle(playerid, vehicle,0);
			}
		} else {
			new vehicleID = GetPlayerVehicleID(playerid);
			if(IsVehicleRcTram(vehicleID) || GetVehicleModel(vehicleID) == V_RCCAM){
				if(GetVehicleModel(vehicleID) != V_TRAM){
					new Float:x, Float:y, Float:z;
			   	 	GetPlayerPos(playerid, x, y, z);
					oSetPlayerPos(playerid, x+0.5, y, z+1.0);
				}
			}
		}
	}
}

GetVehicleWithinDistance( playerid, Float:x1, Float:y1, Float:z1, Float:dist, &veh){
	for(new i = 1; i < MAX_VEHICLES; i++){
		if(GetVehicleModel(i) > 0){
			if(GetPlayerVehicleID(playerid) != i ){
				new Float:x, Float:y, Float:z;
				new Float:x2, Float:y2, Float:z2;
				GetVehiclePos(i, x, y, z);
				x2 = x1 - x; y2 = y1 - y; z2 = z1 - z;
				new Float:vDist = (x2*x2+y2*y2+z2*z2);
				if( vDist < dist){
					veh = i;
					dist = vDist;
				}
			}
		}
	}
}

IsVehicleRcTram(vehicleid)
{
	new model = GetVehicleModel(vehicleid);
   	switch(model)
	{
		case V_TRAM, V_RCGOBLIN, V_RCBARON, V_RCBANDIT, V_RCRAIDER, V_RCTIGER: return 1;
		default: return 0;
	}
	return 0;
}