//Weapons module.
//Created by Iain Gilbert
//Modder: MadCat

forward IsPlayerAllowedWeapon(playerid,weaponid);
forward IsAllowedWeapon(weaponid);
forward Weapon_OnGameModeInit();
forward GetWeaponSlot(weaponid);

enum WeaponInfo {
	Weapon_ID, // id number of weapon
	Weapon_Name[MAX_NAME], // weapon name
	Weapon_Allowed, // is weapon allowed
	Weapon_MaxBullets, // max bullets
	Weapon_Cost, // cost per bullet
	Weapon_Level // player level required to buy this weapon
}

#define WEAPON_PACK 1000

#define WeaponsSize 44
new Weapons[WeaponsSize][WeaponInfo] = {

	//id, "Name" ,Allowed,bullets, cost,level
	{0, "Unarmed",1,1,0,0},
	{22, "Pistol",1,999,200,0},
	{25, "Shotgun",1,999,600,1},
	{23, "Silenced Pistol",1,999,600,2},
	{24, "Desert Eagle",1,999,1200,3},
	{26, "Sawn Off Shotgun",1,999,800,4},
	{28, "Micro Uzi",1,999,500,5},
	{9, "Chainsaw",1,1,5000,6},
	{32, "Tec9",1,999,300,7},
	{33, "Rifle",1,999,1000,8},
	{29, "MP5",1,999,2000,9},
	{4, "Knife",1,1,50,0},
	{46, "Parachute",5,1,1000,4},
	{27, "Combat Shotgun",1,999,1000,12},
	{31, "M4",1,999,4500,14},
	{12, "Long White Dildo" ,0 , 1 , 1000 , 0},
	{8, "Katana",1,1,3500,5},
	{30, "AK47",1,999,3500,18},
	{38, "Minigun",0,999,10000000,99},
	{34, "Sniper Rifle",1,200,5000,7},

	{39, "Sachel Charges",0,9,2000,99},
	{40, "Detonator",0,1,100,99},

	{10, "Purple Dildo",1,1,1000,0},
	{3, "Nitestick",1,1,500,0},
	{37, "Flame Thrower",1,4000,10000,16},
	{42, "Fire Extinguisher",1,10000,700,5},

	{1, "Brass Knuckles",1,1,25,0},
	{2, "Golf Club",1,1,250,0},
	{5, "Baseball Bat",1,1,100,0},
	{6, "Shovel",1,1,100,0},
	{7, "Pool cue",1,1,200,0},
	{11, "White Dildo",1,1,500,0},
	{13, "White Dildo 2",1,1,500,0},
	{14, "Flowers",0,1,1000,99},
	{15, "Cane",1,1,75,0},
	{16, "Grenades",1,10,1000,3},
	{17, "Tear Gas",0,1,1000,99},
	{18, "Molotovs",1,10,1000,3},
	{35, "RPG",0,25,1000,99},
	{36, "Missile Launcher",0,25,1000,99},
	{41, "Spray Paint",0,1,1000,99},
	{43, "Camera",0,1,1000,99},
	{44, "Nightvision Goggles",0,1,1000,99},
	{45, "Thermal Goggles",0,1,1000,99}
};

new WeaponDB[MAX_STRING] = "MultiGTA/Weapons/";

WeaponLoadAll()
{
	Debug("weapons.inc > WeaponLoadAll - Start");
	
	new temp[MAX_STRING];
	if (!db_Exists(DatabaseDB)) db_Create(DatabaseDB);
	set(temp,db_Get(DatabaseDB,"Weapon_DB"));
	if (strlen(temp) > 0) set(WeaponDB,temp);

	if (!db_Exists(WeaponDB))
	{
		WeaponSaveAll();
		return;
	}
	for (new i=0;i<WeaponsSize;i++)
	{
		new weapondbname[MAX_STRING];
		format(weapondbname,sizeof(weapondbname),"%s%s.txt",WeaponDB,Weapons[i][Weapon_Name]);
		if (!db_Exists(weapondbname))
		{
			WeaponSaveAll();	
		}
		set(temp,db_Get(weapondbname,"Name"));
		if (strlen(temp) > 0) set(Weapons[i][Weapon_Name],temp);
		set(temp,nullstr);
		set(temp,db_Get(weapondbname,"IsAllowed"));
		if (strlen(temp) > 0) Weapons[i][Weapon_Allowed] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(weapondbname,"Level"));
		if (strlen(temp) > 0) Weapons[i][Weapon_Level] = strval(temp);
		set(temp,nullstr);
		set(temp,db_Get(weapondbname,"Price"));
		if (strlen(temp) > 0) Weapons[i][Weapon_Cost] = strval(temp);
		set(temp,nullstr);
	}
	Debug("weapons.inc > WeaponLoadAll - Stop");
	return;
}

WeaponSaveAll()
{
	Debug("weapons.inc > WeaponSaveAll - Start");
	new temp[MAX_STRING];
	if (db_Exists(DatabaseDB))
	{
		db_Set(DatabaseDB,"Weapon_DB",WeaponDB);
	}

	for (new i=0;i<WeaponsSize;i++)
	{
		new weapondbname[MAX_STRING];
		format(weapondbname,sizeof(weapondbname),"%s%s.txt",WeaponDB,Weapons[i][Weapon_Name]);
		if (db_Exists(weapondbname)) {
			continue;
		}
		db_Create(weapondbname);
		db_BeforeBigSaving(weapondbname);
		db_Set(weapondbname,"Name",Weapons[i][Weapon_Name]);
		valstr(temp,Weapons[i][Weapon_Allowed]);
		db_Set(weapondbname,"IsAllowed",temp);
		set(temp,nullstr);
		valstr(temp,Weapons[i][Weapon_Level]);
		db_Set(weapondbname,"Level",temp);
		set(temp,nullstr);
		valstr(temp,Weapons[i][Weapon_Cost]);
		db_Set(weapondbname,"Price",temp);
		set(temp,nullstr);
		db_AfterBigSaving(weapondbname);
	}
	WriteLog("All weapon saved!");
	Debug("weapons.inc > WeaponSaveAll - Stop");
	return;
}

oGetWeaponName(weaponid)
{
	new name[MAX_NAME];
	new lookupid=GetWeaponLookupID(weaponid);
	if (lookupid == -1) return name;
	set(name,Weapons[lookupid][Weapon_Name]);
	return name;
}

GetWeaponMaxAmmo(weaponid)
{
	new ammo;
	new lookupid=GetWeaponLookupID(weaponid);
	if (lookupid == -1) return ammo;
	ammo = Weapons[lookupid][Weapon_MaxBullets];
	return ammo;
}

GetWeaponLevel(weaponid)
{
	new level;
	new lookupid=GetWeaponLookupID(weaponid);
	if (lookupid == -1) return level;
	level = Weapons[lookupid][Weapon_Level];
	return level;
}

public IsPlayerAllowedWeapon(playerid,weaponid)
{
	new lookupid=GetWeaponLookupID(weaponid);
	if (lookupid == -1) return 0;
	if (Weapons[lookupid][Weapon_Allowed] == 0) return 0;
	if (Weapons[lookupid][Weapon_Level] > GetPlayerLevel(playerid)) return 0;
	return 1;
}

public IsAllowedWeapon(weaponid)
{
	new lookupid=GetWeaponLookupID(weaponid);
	if (lookupid == -1) return 0;
	if (Weapons[lookupid][Weapon_Allowed] == 0) return 0;
	return 1;
}

IsWeaponHandToHand(weaponid)
{
	new weaponlookupid=GetWeaponLookupID(weaponid);
	if (weaponlookupid == -1) return 0;
	if (Weapons[weaponlookupid][Weapon_MaxBullets] == 1) return 1;
	return 0;
}

GetWeaponLookupID(weaponid)
{
	new lookupid=-1;
	for (new i=0;i<WeaponsSize;i++)
	{
		if (weaponid == Weapons[i][Weapon_ID])
		{
			lookupid = i;
		}
	}
	return lookupid;
}

IsWeapon(weaponid)
{
	if ((weaponid >= 0) && (weaponid <= 45)) return 1;
	return 0;
}

GetWeaponCost(weaponid)
{
	new weaponlookupid=GetWeaponLookupID(weaponid);
	if (weaponlookupid == -1) return 0;
	return Weapons[weaponlookupid][Weapon_Cost];
}

public Weapon_OnGameModeInit(){
	WeaponLoadAll();
	WriteLog("Weapons System Loaded");
	return 0;
}

public GetWeaponSlot(weaponid){
	if(weaponid == 0 || weaponid == 1) return 0;
	else if(weaponid >= 2 && weaponid <= 9) return 1;
	else if(weaponid >= 22 && weaponid <= 24) return 2;
	else if(weaponid >= 25 && weaponid <= 27) return 3;
	else if(weaponid == 28 || weaponid == 29 || weaponid == 32) return 4;
	else if(weaponid == 30 || weaponid == 31) return 5;
	else if(weaponid == 33 || weaponid == 34) return 6;
	else if(weaponid >= 35 && weaponid <= 38) return 7;
	else if((weaponid >= 16 && weaponid <= 18) || weaponid == 39) return 8;
	else if(weaponid >= 41 && weaponid <= 43) return 9;
	else if(weaponid >= 10 && weaponid <= 15) return 10;
	else if(weaponid >= 45 && weaponid <= 46) return 11;
	else if(weaponid == 40) return 12;
	else return -1;
}

//Used for GetWeaponID
GetWeaponIDFromDB(weaponid)
{
	new dbid;
	new lookupid=GetWeaponLookupID(weaponid);
	if (lookupid == -1) return dbid;
	dbid = Weapons[lookupid][Weapon_ID];
	return dbid;
}

//Based on FindPlayerID code :)
stock GetWeaponID(partofname[]) {

	if (isNumeric(partofname)) return strval(partofname);
	if (strlen(partofname)==0) return -1;

	new correctsigns_weaponid=-1;
    	new hasmultiple=-1;

	new wname[MAX_NAME];
	for (new weaponid=0;weaponid<46;weaponid++) {
		set(wname,oGetWeaponName(weaponid));
		if (strcomp(wname,partofname,true)==1) return GetWeaponIDFromDB(weaponid);
		if (strfind(wname,partofname,true) != -1) {
			hasmultiple++;
			correctsigns_weaponid = GetWeaponIDFromDB(weaponid);
			if (hasmultiple>0) return -2;
		}
	}
	return correctsigns_weaponid;
}