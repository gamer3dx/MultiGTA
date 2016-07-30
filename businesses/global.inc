forward Business_OnPlayerCommandText(playerid,text[]);
forward Business_OnGameModeInit();
forward Business_OnVehicleSpawn(vehicleid);

new TimerMakeItems;

public Business_OnGameModeInit(){
	DrinkBusinessLoadAll();
	WeaponBusinessLoadAll();
	FuelBusinessLoadAll();
	ClothesBusinessLoadAll();
	SkydivePlacesLoadAll();
	MineShopsLoadAll();
	PayNSpraysLoadAll();
	DinnersLoadAll();
	CreateStreamMapIcon(51,2787.8,-2436.3,13.7);
	CreateStreamMapIcon(51,-1824.35,-107.68,5.71);
	CreateStreamMapIcon(51,2814.8,972.6,10.7);
	CreateDMapIcon(51,2787.8,-2436.3,13.7,gettext(1757),gettext(1758));
	CreateDMapIcon(51,-1824.35,-107.68,5.71,gettext(1757),gettext(1758));
	CreateDMapIcon(51,2814.8,972.6,10.7,gettext(1757),gettext(1758));
	new vid = CreateVehicle(V_RUMPO,2787.6250,-2436.1714,13.6337,90.0274,-1,-1,300);
	SetVehicleToRespawn(vid);
	vid = CreateVehicle(V_RUMPO,-1824.4592,-109.3575,5.6484,182.1951,-1,-1,300);
	SetVehicleToRespawn(vid);
	vid = CreateVehicle(V_RUMPO,2841.6790,954.5175,10.7500,92.1226,-1,-1,300);
	SetVehicleToRespawn(vid);
	TimerMakeItems = SetTimer("MakeItems",ONE_SA_DAY,1);
	WriteLog("Business System Fully Loaded");
	return 0;
}

public Business_OnVehicleSpawn(vehicleid){
	new Model = GetVehicleModel(vehicleid);
	if (Model == V_BURRITO || Model == V_RUMPO)
		{	
			PlayerHaul[vehicleid][Capacity]=50;
		}
	if (Model == V_YANKEE)
		{
			PlayerHaul[vehicleid][Capacity]=100;
		}
	if (PlayerHaul[vehicleid][Capacity] > 0){
		new logstring[256];
		format(logstring,sizeof(logstring),"Capacity added to %s (ID: %d)",GetVehicleName(Model),vehicleid);
		WriteLog(logstring);
	}
	return 0;
}

public Business_OnPlayerCommandText(playerid,text[])
{
	if (!IsPlayerConnected(playerid)) return 0;

	if (IsPlayerInAnyDM(playerid)) return 0; //Deathmatch near business. Don't do this

	if (IsPlayerInAnyHay(playerid)) return 0;

	dcmd(bbuy,4,text);
	dcmd(bcollect,8,text);
	dcmd(bsell,5,text);
	dcmd(buy,3,text);
	dcmd(bsetprice,9,text);
	dcmd(blist,5,text);
	dcmd(load,4,text);
	dcmd(buycomp,7,text);
	dcmd(putcomp,7,text);

	return 0;
}

dcmd_bbuy(playerid,params[])
{
	#pragma unused params
	Debug("businesses/global.inc > Command 'bbuy' - Start");
	if (IsPlayerAtCShop(playerid)){
		BuyCShop(playerid);
	} else if (IsPlayerAtDrinkVendor(playerid)){
		BuyDrinkVendor(playerid);
	} else if (IsPlayerAtMineShop(playerid)){
		BuyMineShop(playerid);
	} else if (IsPlayerAtPayNSpray(playerid)){
		BuyPayNSpray(playerid);
	} else if (IsPlayerAtFuelCP(playerid)){
		BuyFuelstation(playerid);
	} else if (IsPlayerAtArmDealer(playerid)){
		BuyArmDealer(playerid);
	} else if (IsPlayerAtDinner(playerid)){
		BuyDinner(playerid);
	} else if (IsPlayerAtSkydivePlace(playerid)){
		BuySkydivePlace(playerid);
	} else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(711));
	}
	Debug("businesses/global.inc > Command 'bbuy' - Stop");
	return 1;
}

dcmd_bcollect(playerid,params[])
{
	#pragma unused params
	Debug("businesses/global.inc > Command 'bcollect' - Start");
	if (IsPlayerAtCShop(playerid)){
		CollectCShop(playerid);
	} else if (IsPlayerAtDrinkVendor(playerid)){
		CollectDrinkVendor(playerid);
	} else if (IsPlayerAtMineShop(playerid)){
		CollectMineShop(playerid);
	} else if (IsPlayerAtPayNSpray(playerid)){
		CollectPayNSpray(playerid);
	} else if (IsPlayerAtFuelCP(playerid)){
		CollectFuelstation(playerid);
	} else if (IsPlayerAtArmDealer(playerid)){
		CollectArmDealer(playerid);
	} else if (IsPlayerAtDinner(playerid)){
		CollectDinner(playerid);
	} else if (IsPlayerAtSkydivePlace(playerid)){
		CollectSkydivePlace(playerid);
	} else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(711));
	}
	Debug("businesses/global.inc > Command 'bcollect' - Stop");
	return 1;
}

dcmd_bsell(playerid,params[])
{
	#pragma unused params
	Debug("businesses/global.inc > Command 'bsell' - Start");
	if (IsPlayerAtCShop(playerid)){
		SellCShop(playerid);
	} else if (IsPlayerAtDrinkVendor(playerid)){
		SellDrinkVendor(playerid);
	} else if (IsPlayerAtMineShop(playerid)){
		SellMineShop(playerid);
	} else if (IsPlayerAtPayNSpray(playerid)){
		SellPayNSpray(playerid);
	} else if (IsPlayerAtFuelCP(playerid)){
		SellFuelstation(playerid);
	} else if (IsPlayerAtArmDealer(playerid)){
		SellArmDealer(playerid);
	} else if (IsPlayerAtDinner(playerid)){
		SellDinner(playerid);
	} else if (IsPlayerAtSkydivePlace(playerid)){
		SellSkydivePlace(playerid);
	} else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(711));
	}
	Debug("businesses/global.inc > Command 'bsell' - Stop");
	return 1;
}

dcmd_buy(playerid,params[])
{
	#pragma unused params
	Debug("businesses/global.inc > Command 'buy' - Start");
	if (IsPlayerAtCShop(playerid)){
		BuyClothes(playerid);
	} else if (IsPlayerAtDrinkVendor(playerid)){
		BuyDrink(playerid);
	} else if (IsPlayerAtMineShop(playerid)){
		BuyMine(playerid);
	} else if (IsPlayerAtPayNSpray(playerid)){
		ResprayVehicle(playerid);
	} else if (IsPlayerAtFuelPickup(playerid)){
		BuyCanister(playerid);
	} else if (IsPlayerAtArmDealer(playerid)){
		BuyWeaponFromDealer(playerid);
	} else if (IsPlayerAtDinner(playerid)){
		BuyFood(playerid);
	} else if (IsPlayerAtSkydivePlace(playerid)){
		DoSkydive(playerid);
	} else 	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(711));
	}
	Debug("businesses/global.inc > Command 'buy' - Stop");
	return 1;
}

dcmd_bsetprice(playerid,params[])
{
	Debug("businesses/global.inc > Command 'bsetprice' - Start");
	new price = strval(params);
	if (IsPlayerAtCShop(playerid)){
		SetClothesPrice(playerid,price);
	} else if (IsPlayerAtDrinkVendor(playerid)){
		SetDrinkPrice(playerid,price);
	} else if (IsPlayerAtMineShop(playerid)){
		SetMinePrice(playerid,price);
	} else if (IsPlayerAtPayNSpray(playerid)){
		SetResprayPrice(playerid,price);
	} else if (IsPlayerAtFuelCP(playerid)){
		SetFuelPrice(playerid,price);
	} else if (IsPlayerAtFuelPickup(playerid)){
		SetFuelCanisterPrice(playerid,price);
	} else if (IsPlayerAtDinner(playerid)){
		SetFoodPrice(playerid,price);
	} else if (IsPlayerAtSkydivePlace(playerid)){
		SetSkydivePrice(playerid,price);
	} else {
		SystemMsg(playerid,COLOUR_ERROR,gettext(711));
	}
	Debug("businesses/global.inc > Command 'bsetprice' - Stop");
	return 1;
}

dcmd_blist(playerid,params[])
{
	#pragma unused params
	Debug("businesses/global.inc > Command 'blist' - Start");
	new temp[MAX_STRING];
	new count = 0;
	for (new id=0;id<MAX_CLOTHESSHOPS;id++)
	{
		if(strcomp(CShops[id][Owner], oGetPlayerName(playerid), true) == 1)
		{
			count++;
			format(temp,sizeof(temp),gettext(703),count,CShops[id][Name],CShops[id][MoneyNow],CShops[id][ClothesPrice],CShops[id][Components],CShops[id][ItemsInStock]);
			SystemMsgScrolling(playerid,temp);
			set(temp,nullstr);
		}
	}
	for (new id=0;id<MAX_DRINKBUSINESSES;id++)
	{
		if(strcomp(DrinkVendors[id][Owner], oGetPlayerName(playerid), true) == 1)
		{
			count++;
			format(temp,sizeof(temp),gettext(704),count,DrinkVendors[id][Name],DrinkVendors[id][MoneyNow],DrinkVendors[id][DrinkPrice],DrinkVendors[id][Components],DrinkVendors[id][ItemsInStock]);
			SystemMsgScrolling(playerid,temp);
			set(temp,nullstr);
		}
	}
	for (new id=0;id<MAX_MINESHOPS;id++)
	{
		if(strcomp(MineShops[id][Owner], oGetPlayerName(playerid), true) == 1)
		{
			count++;
			format(temp,sizeof(temp),gettext(705),count,MineShops[id][Name],MineShops[id][MoneyNow],MineShops[id][MinePrice],MineShops[id][Components],MineShops[id][ItemsInStock]);
			SystemMsgScrolling(playerid,temp);
			set(temp,nullstr);
		}
	}
	for (new id=0;id<MAX_PAYNSPRAYS;id++)
	{
		if(strcomp(PayNSprays[id][Owner], oGetPlayerName(playerid), true) == 1)
		{
			count++;
			format(temp,sizeof(temp),gettext(705),count,PayNSprays[id][Name],PayNSprays[id][MoneyNow],PayNSprays[id][ResprayPrice],PayNSprays[id][Components],PayNSprays[id][ItemsInStock]);
			SystemMsgScrolling(playerid,temp);
			set(temp,nullstr);
		}
	}
	for (new id=0;id<MAX_GSTATIONS;id++)
	{
		if(strcomp(fuelstations[id][Owner], oGetPlayerName(playerid), true) == 1)
		{
			count++;
			format(temp,sizeof(temp),gettext(706),count,fuelstations[id][Name],fuelstations[id][MoneyNow],fuelstations[id][OneFuelPrice],fuelstations[id][Components],fuelstations[id][ItemsInStock]);
			SystemMsgScrolling(playerid,temp);
			set(temp,nullstr);
		}
	}
	for (new id=0;id<MAX_ARMDEALERS;id++)
	{
		if(strcomp(Armdealers[id][Owner], oGetPlayerName(playerid), true) == 1)
		{
			count++;
			format(temp,sizeof(temp),gettext(707),count,Armdealers[id][Name],Armdealers[id][MoneyNow],Armdealers[id][Components],Armdealers[id][ItemsInStock]);
			SystemMsgScrolling(playerid,temp);
			set(temp,nullstr);
		}
	}
	for (new id=0;id<MAX_DINNERS;id++)
	{
		if(strcomp(Dinners[id][Owner], oGetPlayerName(playerid), true) == 1)
		{
			count++;
			format(temp,sizeof(temp),gettext(708),count,Dinners[id][Name],Dinners[id][MoneyNow],Dinners[id][Components],Dinners[id][ItemsInStock]);
			SystemMsgScrolling(playerid,temp);
			set(temp,nullstr);
		}
	}
	for (new id=0;id<MAX_SKYDIVEPLACES;id++)
	{
		if(strcomp(SkydivePlaces[id][Owner], oGetPlayerName(playerid), true) == 1)
		{
			count++;
			format(temp,sizeof(temp),gettext(709),count,SkydivePlaces[id][Name],SkydivePlaces[id][MoneyNow],SkydivePlaces[id][Components],SkydivePlaces[id][ItemsInStock]);
			SystemMsgScrolling(playerid,temp);
			set(temp,nullstr);
		}
	}
	if(count < 1)
	{
		SystemMsg(playerid,COLOUR_ERROR,gettext(710));
	}
	Debug("businesses/global.inc > Command 'blist' - Stop");
	return 1;
}

dcmd_load(playerid,params[])
{
	#pragma unused params
	Debug("businesses/global.inc > Command 'load' - Start");
	ShowTruckLoad(playerid);
	Debug("businesses/global.inc > Command 'load' - Stop");
	return 1;
}

dcmd_buycomp(playerid,params[])
{
	#pragma unused params
	Debug("businesses/global.inc > Command 'buycomp' - Start");
	BuyComponents(playerid);
	Debug("businesses/global.inc > Command 'buycomp' - Stop");
	return 1;
}

dcmd_putcomp(playerid,params[])
{
	#pragma unused params
	Debug("businesses/global.inc > Command 'putcomp' - Start");
	SellOrPutComponents(playerid);
	Debug("businesses/global.inc > Command 'putcomp' - Stop");
	return 1;
}