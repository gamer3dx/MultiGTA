//Money drop module
//Created by Simon
//Converted by MadCat

forward MoneyDrop_OnOnGameModeInit();

new Drop_Money_On_Death = 0;

new moneyPickups[MAX_PLAYERS][7];

forward Moneydrop_OnPlayerPickUpPickup(playerid, pickupid);
public Moneydrop_OnPlayerPickUpPickup(playerid, pickupid)
{
	if (Drop_Money_On_Death == 0) return 0;
	new streampickupid = GetStreamPickupID(pickupid);
	for (new i = 0; i < MAX_PLAYERS_EX; i++)
	{
	    for (new j = 1; j < 6; j++)
	    {
	        if (moneyPickups[i][j] == streampickupid)
	        {
	            oGivePlayerMoney(playerid, moneyPickups[i][0],true);
	            DestroyStreamPickup(streampickupid);
	            moneyPickups[i][j] = -1;
	            return 0;
	        }
	    }
	}


	return 0;
}

MoneyDropConfig()
{
	new temp[MAX_STRING];
	if (!db_Exists(ConfigDB)) db_Create(ConfigDB);
	
	set(temp,db_Get(ConfigDB,"Drop_Money_On_Death"));
	if (strlen(temp) > 0) Drop_Money_On_Death = strval(temp);
	else { valstr(temp,Drop_Money_On_Death); db_Set(ConfigDB,"Drop_Money_On_Death",temp);  }
	set(temp,nullstr);

	return;
}

public MoneyDrop_OnOnGameModeInit(){
	MoneyDropConfig();
	if (Drop_Money_On_Death == 1) WriteLog("Money drop loaded");
	return 0;
}

stock DropDeathMoney(playerid,amount)
{

	if (moneyPickups[playerid][6])
	{
		KillTimer(moneyPickups[playerid][6]);
		DestroyDeathMoney(playerid);
	}

	if (amount > 0)
	{
	    	new pickupmodel = 1212;
	    
		moneyPickups[playerid][0] = amount / 5;
		
		if (moneyPickups[playerid][0] >= 10000)
			pickupmodel = 1550;
		
		for (new i = 1; i < 6; i++) moneyPickups[playerid][i] = CreateStreamPickup(pickupmodel, 3, PlayerPos[playerid][Coord_X] + (random(2 - -4) + -4), PlayerPos[playerid][Coord_Y] + (random(2 - -4) + -4), PlayerPos[playerid][Coord_Z]);
	}

	moneyPickups[playerid][6] = SetTimerEx("DestroyDeathMoney", 20000, 0, "i", playerid);
}

forward DestroyDeathMoney(playerid);
public DestroyDeathMoney(playerid)
{
	for (new i = 1; i < 6; i++)
	{
	    if (moneyPickups[playerid][i] != -1) DestroyStreamPickup(moneyPickups[playerid][i]);

	    moneyPickups[playerid][i] = -1;
	}

	moneyPickups[playerid][0] = 0;
	moneyPickups[playerid][6] = 0;
}
