//Player Privilegies module.
//Created by MadCat

forward Privilegies_OnGameModeInit();
forward player_ResetPrivilegies(playerid);

new Player_Privilegies_DB[MAX_STRING] = "MultiGTA/Players/Privilegies/";

enum PlayerPrivilegies {
	IsModerator, 
	CanBroadcast,
	CanWorldSave,
	CanAnnounce,
	CanGiveXP,
	CanSlap,
	CanGiveMoney,
	CanSetMoney,
	CanSetBank,
	CanAddCrimes,
	CanDisarm,
	CanFreeze,
	CanUnFreeze,
	CanHeal,
	CanGod,
	CanMurder,
	CanSilentMurder,
	CanJail,
	CanUnjail,
	CanJailList,
	CanMute,
	CanMuteList,
	CanUnmute,
	CanWarn,
	CanUnwarn,
	CanLockAcc,
	CanUnlockAcc,
	CanMakeItems,
	CanStartDM,
	CanStartRace,
	CanEditRace,
	CanStartHotwire,
	CanTeleportToPlayers,
	CanSeePlayersInfo,
	CanTeleportPlayersToYourself,
	CanGiveWeapons,
	CanKickPlayers,
	CanLockServer,
	CanUnlockServer,
	CanReloadMOTD,
	CanStartCashBox,
	CanMark,
	CannotBeKicked,
	CanSpectate,
	CanSpySMS,
	CanSetAntiBikeFall,
	CanBanPlayers,
	CanTeleportToFilePos,
	CanChangeWeather,
	CanStartDuels,
	CanMapTeleport,
	CanCrashPlayers,
	CanBuildHouses,
	CanStartHay,
	CanExecFakeCMD
}

new PlayerPriv[MAX_PLAYERS][PlayerPrivilegies];

public Privilegies_OnGameModeInit(){
	PrivilegiesLoadConfig();
	return 0;
}

PrivilegiesLoadConfig()
{
	Debug("privilegies.inc > PrivilegiesLoadConfig - Start");
	if (!db_Exists(DatabaseDB)) db_Create(DatabaseDB);
	new temp[MAX_STRING];
	set(temp,db_Get(DatabaseDB,"Player_Privilegies_DB"));
	if (strlen(temp) > 0) set(Player_Privilegies_DB,temp);
	else {set(temp,Player_Privilegies_DB); db_Set(DatabaseDB,"Player_Privilegies_DB",temp);}
	set(temp,nullstr);
	return;
}

player_save_privilegies(playerid)
{
	Debug("privilegies.inc > player_save_privilegies - Start");
	if (strlen(oGetPlayerName(playerid)) < 1)
	{
		return;
	}
	new dinifilename[MAX_STRING];
	format(dinifilename,sizeof(dinifilename),"%s%s.txt",Player_Privilegies_DB,EncodeName(oGetPlayerName(playerid)));
	if (!db_Exists(dinifilename))
	{
		db_Create(dinifilename);
	}
	new temp[MAX_STRING];
	db_BeforeBigSaving(dinifilename);
	valstr(temp,PlayerPriv[playerid][IsModerator]);
	db_Set(dinifilename,"IsModerator",temp);
	if (PlayerPriv[playerid][IsModerator] == 0) return;
	valstr(temp,PlayerPriv[playerid][CanBroadcast]);
	db_Set(dinifilename,"CanBroadCast",temp);
	valstr(temp,PlayerPriv[playerid][CanWorldSave]);
	db_Set(dinifilename,"CanWorldSave",temp);
	valstr(temp,PlayerPriv[playerid][CanAnnounce]);
	db_Set(dinifilename,"CanAnnounce",temp);
	valstr(temp,PlayerPriv[playerid][CanGiveXP]);
	db_Set(dinifilename,"CanGiveXP",temp);
	valstr(temp,PlayerPriv[playerid][CanSlap]);
	db_Set(dinifilename,"CanSlap",temp);
	valstr(temp,PlayerPriv[playerid][CanGiveMoney]);
	db_Set(dinifilename,"CanGiveMoney",temp);
	valstr(temp,PlayerPriv[playerid][CanSetMoney]);
	db_Set(dinifilename,"CanSetMoney",temp);
	valstr(temp,PlayerPriv[playerid][CanSetBank]);
	db_Set(dinifilename,"CanSetBank",temp);
	valstr(temp,PlayerPriv[playerid][CanAddCrimes]);
	db_Set(dinifilename,"CanAddCrimes",temp);
	valstr(temp,PlayerPriv[playerid][CanDisarm]);
	db_Set(dinifilename,"CanDisarm",temp);
	valstr(temp,PlayerPriv[playerid][CanFreeze]);
	db_Set(dinifilename,"CanFreeze",temp);
	valstr(temp,PlayerPriv[playerid][CanUnFreeze]);
	db_Set(dinifilename,"CanUnFreeze",temp);
	valstr(temp,PlayerPriv[playerid][CanHeal]);
	db_Set(dinifilename,"CanHeal",temp);
	valstr(temp,PlayerPriv[playerid][CanGod]);	
	db_Set(dinifilename,"CanGod",temp);
	valstr(temp,PlayerPriv[playerid][CanMurder]);
	db_Set(dinifilename,"CanMurder",temp);
	valstr(temp,PlayerPriv[playerid][CanSilentMurder]);
	db_Set(dinifilename,"CanSilentMurder",temp);
	valstr(temp,PlayerPriv[playerid][CanJail]);
	db_Set(dinifilename,"CanJail",temp);
	valstr(temp,PlayerPriv[playerid][CanUnjail]);
	db_Set(dinifilename,"CanUnJail",temp);
	valstr(temp,PlayerPriv[playerid][CanJailList]);
	db_Set(dinifilename,"CanJailList",temp);
	valstr(temp,PlayerPriv[playerid][CanMute]);
	db_Set(dinifilename,"CanMute",temp);
	valstr(temp,PlayerPriv[playerid][CanMuteList]);
	db_Set(dinifilename,"CanMuteList",temp);
	valstr(temp,PlayerPriv[playerid][CanUnmute]);
	db_Set(dinifilename,"CanUnmute",temp);
	valstr(temp,PlayerPriv[playerid][CanWarn]);
	db_Set(dinifilename,"CanWarn",temp);
	valstr(temp,PlayerPriv[playerid][CanUnwarn]);
	db_Set(dinifilename,"CanUnwarn",temp);
	valstr(temp,PlayerPriv[playerid][CanLockAcc]);
	db_Set(dinifilename,"CanLockAcc",temp);
	valstr(temp,PlayerPriv[playerid][CanUnlockAcc]);
	db_Set(dinifilename,"CanUnLockAcc",temp);
	valstr(temp,PlayerPriv[playerid][CanMakeItems]);
	db_Set(dinifilename,"CanMakeItems",temp);
	valstr(temp,PlayerPriv[playerid][CanStartDM]);
	db_Set(dinifilename,"CanStartDM",temp);
	valstr(temp,PlayerPriv[playerid][CanStartRace]);
	db_Set(dinifilename,"CanStartRace",temp);
	valstr(temp,PlayerPriv[playerid][CanEditRace]);	
	db_Set(dinifilename,"CanEditRace",temp);
	valstr(temp,PlayerPriv[playerid][CanStartHotwire]);
	db_Set(dinifilename,"CanStartHotwire",temp);
	valstr(temp,PlayerPriv[playerid][CanTeleportToPlayers]);
	db_Set(dinifilename,"CanTeleportToPlayers",temp);
	valstr(temp,PlayerPriv[playerid][CanSeePlayersInfo]);
	db_Set(dinifilename,"CanSeePlayersInfo",temp);
	valstr(temp,PlayerPriv[playerid][CanTeleportPlayersToYourself]);
	db_Set(dinifilename,"CanTeleportPlayersToYourself",temp);
	valstr(temp,PlayerPriv[playerid][CanGiveWeapons]);
	db_Set(dinifilename,"CanGiveWeapons",temp);
	valstr(temp,PlayerPriv[playerid][CanKickPlayers]);
	db_Set(dinifilename,"CanKickPlayers",temp);
	valstr(temp,PlayerPriv[playerid][CanLockServer]);
	db_Set(dinifilename,"CanLockServer",temp);
	valstr(temp,PlayerPriv[playerid][CanUnlockServer]);
	db_Set(dinifilename,"CanUnlockServer",temp);
	valstr(temp,PlayerPriv[playerid][CanReloadMOTD]);
	db_Set(dinifilename,"CanReloadMOTD",temp);
	valstr(temp,PlayerPriv[playerid][CanStartCashBox]);
	db_Set(dinifilename,"CanStartCashBox",temp);
	valstr(temp,PlayerPriv[playerid][CanMark]);
	db_Set(dinifilename,"CanMark",temp);
	valstr(temp,PlayerPriv[playerid][CannotBeKicked]);
	db_Set(dinifilename,"CannotBeKicked",temp);
	valstr(temp,PlayerPriv[playerid][CanSpectate]);
	db_Set(dinifilename,"CanSpectate",temp);
	valstr(temp,PlayerPriv[playerid][CanSpySMS]);
	db_Set(dinifilename,"CanSpySMS",temp);
	valstr(temp,PlayerPriv[playerid][CanSetAntiBikeFall]);
	db_Set(dinifilename,"CanSetAntiBikeFall",temp);
	valstr(temp,PlayerPriv[playerid][CanBanPlayers]);
	db_Set(dinifilename,"CanBanPlayers",temp);
	valstr(temp,PlayerPriv[playerid][CanTeleportToFilePos]);
	db_Set(dinifilename,"CanTeleportToFilePos",temp);
	valstr(temp,PlayerPriv[playerid][CanChangeWeather]);
	db_Set(dinifilename,"CanChangeWeather",temp);
	valstr(temp,PlayerPriv[playerid][CanStartDuels]);
	db_Set(dinifilename,"CanStartDuels",temp);
	valstr(temp,PlayerPriv[playerid][CanMapTeleport]);
	db_Set(dinifilename,"CanMapTeleport",temp);
	valstr(temp,PlayerPriv[playerid][CanCrashPlayers]);
	db_Set(dinifilename,"CanCrashPlayers",temp);
	valstr(temp,PlayerPriv[playerid][CanBuildHouses]);
	db_Set(dinifilename,"CanBuildHouses",temp);
	valstr(temp,PlayerPriv[playerid][CanStartHay]);
	db_Set(dinifilename,"CanStartHay",temp);
	valstr(temp,PlayerPriv[playerid][CanExecFakeCMD]);
	db_Set(dinifilename,"CanExecFakeCMD",temp);
	db_AfterBigSaving(dinifilename);
	Debug("privilegies.inc > player_save_privilegies - Stop");

}


player_load_privilegies(playerid)
{
	Debug("privilegies.inc > player_load_privilegies - Start");
	// player - admin? give all privilegies!
	if (IsPlayerAdmin(playerid)){
		PlayerPriv[playerid][IsModerator] = 1; 
		PlayerPriv[playerid][CanBroadcast] = 1;
		PlayerPriv[playerid][CanWorldSave] = 1;
		PlayerPriv[playerid][CanAnnounce] = 1;
		PlayerPriv[playerid][CanGiveXP] = 1;
		PlayerPriv[playerid][CanSlap] = 1;
		PlayerPriv[playerid][CanGiveMoney] = 1;
		PlayerPriv[playerid][CanSetMoney] = 1;
		PlayerPriv[playerid][CanSetBank] = 1;
		PlayerPriv[playerid][CanAddCrimes] = 1;
		PlayerPriv[playerid][CanDisarm] = 1;
		PlayerPriv[playerid][CanFreeze] = 1;
		PlayerPriv[playerid][CanUnFreeze] = 1;
		PlayerPriv[playerid][CanHeal] = 1;
		PlayerPriv[playerid][CanGod] = 1;
		PlayerPriv[playerid][CanMurder] = 1;
		PlayerPriv[playerid][CanSilentMurder] = 1;
		PlayerPriv[playerid][CanJail] = 1;
		PlayerPriv[playerid][CanUnjail] = 1;
		PlayerPriv[playerid][CanJailList] = 1;
		PlayerPriv[playerid][CanMute] = 1;
		PlayerPriv[playerid][CanMuteList] = 1;
		PlayerPriv[playerid][CanUnmute] = 1;
		PlayerPriv[playerid][CanWarn] = 1;
		PlayerPriv[playerid][CanUnwarn] = 1;
		PlayerPriv[playerid][CanLockAcc] = 1;
		PlayerPriv[playerid][CanUnlockAcc] = 1;
		PlayerPriv[playerid][CanMakeItems] = 1;
		PlayerPriv[playerid][CanStartDM] = 1;
		PlayerPriv[playerid][CanStartRace] = 1;
		PlayerPriv[playerid][CanEditRace] = 1;
		PlayerPriv[playerid][CanStartHotwire] = 1;
		PlayerPriv[playerid][CanTeleportToPlayers] = 1;
		PlayerPriv[playerid][CanSeePlayersInfo] = 1;
		PlayerPriv[playerid][CanTeleportPlayersToYourself] = 1;
		PlayerPriv[playerid][CanGiveWeapons] = 1;
		PlayerPriv[playerid][CanKickPlayers] = 1;
		PlayerPriv[playerid][CanLockServer] = 1;
		PlayerPriv[playerid][CanUnlockServer] = 1;
		PlayerPriv[playerid][CanReloadMOTD] = 1;
		PlayerPriv[playerid][CanStartCashBox] = 1;
		PlayerPriv[playerid][CanMark] = 1;
		PlayerPriv[playerid][CannotBeKicked] = 1;
		PlayerPriv[playerid][CanSpectate] = 1;
		PlayerPriv[playerid][CanSpySMS] = 1;
		PlayerPriv[playerid][CanSetAntiBikeFall] = 1;
		PlayerPriv[playerid][CanBanPlayers] = 1;
		PlayerPriv[playerid][CanTeleportToFilePos] = 1;
		PlayerPriv[playerid][CanChangeWeather] = 1;
		PlayerPriv[playerid][CanStartDuels] = 1;
		PlayerPriv[playerid][CanMapTeleport] = 1;
		PlayerPriv[playerid][CanCrashPlayers] = 1;
		PlayerPriv[playerid][CanBuildHouses] = 1;
		PlayerPriv[playerid][CanStartHay] = 1;
		PlayerPriv[playerid][CanExecFakeCMD] = 1;
		AllowPlayerTeleport(playerid,true);
		player_save_privilegies(playerid);
		return;
	}
	new dinifilename[MAX_STRING];
	format(dinifilename,sizeof(dinifilename),"%s%s.txt",Player_Privilegies_DB,EncodeName(oGetPlayerName(playerid)));

	if (!db_Exists(dinifilename))
	{
		return;
	}

	PlayerPriv[playerid][IsModerator] = strval(db_Get(dinifilename,"IsModerator"));
	if (PlayerPriv[playerid][IsModerator] == 0) return;
	PlayerPriv[playerid][CanBroadcast] = strval(db_Get(dinifilename,"CanBroadCast"));
	PlayerPriv[playerid][CanWorldSave] = strval(db_Get(dinifilename,"CanWorldSave"));
	PlayerPriv[playerid][CanAnnounce] = strval(db_Get(dinifilename,"CanAnnounce"));
	PlayerPriv[playerid][CanGiveXP] = strval(db_Get(dinifilename,"CanGiveXP"));
	PlayerPriv[playerid][CanSlap] = strval(db_Get(dinifilename,"CanSlap"));
	PlayerPriv[playerid][CanGiveMoney] = strval(db_Get(dinifilename,"CanGiveMoney"));
	PlayerPriv[playerid][CanSetMoney] = strval(db_Get(dinifilename,"CanSetMoney"));
	PlayerPriv[playerid][CanSetBank] = strval(db_Get(dinifilename,"CanSetBank"));
	PlayerPriv[playerid][CanAddCrimes] = strval(db_Get(dinifilename,"CanAddCrimes"));
	PlayerPriv[playerid][CanDisarm] = strval(db_Get(dinifilename,"CanDisarm"));
	PlayerPriv[playerid][CanFreeze] = strval(db_Get(dinifilename,"CanFreeze"));
	PlayerPriv[playerid][CanUnFreeze] = strval(db_Get(dinifilename,"CanUnFreeze"));
	PlayerPriv[playerid][CanHeal] = strval(db_Get(dinifilename,"CanHeal"));
	PlayerPriv[playerid][CanGod] = strval(db_Get(dinifilename,"CanGod"));
	PlayerPriv[playerid][CanMurder] = strval(db_Get(dinifilename,"CanMurder"));
	PlayerPriv[playerid][CanSilentMurder] = strval(db_Get(dinifilename,"CanSilentMurder"));
	PlayerPriv[playerid][CanJail] = strval(db_Get(dinifilename,"CanJail"));
	PlayerPriv[playerid][CanUnjail] = strval(db_Get(dinifilename,"CanUnJail"));
	PlayerPriv[playerid][CanJailList] = strval(db_Get(dinifilename,"CanJailList"));
	PlayerPriv[playerid][CanMute] = strval(db_Get(dinifilename,"CanMute"));
	PlayerPriv[playerid][CanMuteList] = strval(db_Get(dinifilename,"CanMuteList"));
	PlayerPriv[playerid][CanUnmute] = strval(db_Get(dinifilename,"CanUnmute"));
	PlayerPriv[playerid][CanWarn] = strval(db_Get(dinifilename,"CanWarn"));
	PlayerPriv[playerid][CanUnwarn] = strval(db_Get(dinifilename,"CanUnwarn"));
	PlayerPriv[playerid][CanLockAcc] = strval(db_Get(dinifilename,"CanLockAcc"));
	PlayerPriv[playerid][CanUnlockAcc] = strval(db_Get(dinifilename,"CanUnLockAcc"));
	PlayerPriv[playerid][CanMakeItems] = strval(db_Get(dinifilename,"CanMakeItems"));
	PlayerPriv[playerid][CanStartDM] = strval(db_Get(dinifilename,"CanStartDM"));
	PlayerPriv[playerid][CanStartRace] = strval(db_Get(dinifilename,"CanStartRace"));
	PlayerPriv[playerid][CanEditRace] = strval(db_Get(dinifilename,"CanEditRace"));
	PlayerPriv[playerid][CanStartHotwire] = strval(db_Get(dinifilename,"CanStartHotwire"));
	PlayerPriv[playerid][CanTeleportToPlayers] = strval(db_Get(dinifilename,"CanTeleportToPlayers"));
	PlayerPriv[playerid][CanSeePlayersInfo] = strval(db_Get(dinifilename,"CanSeePlayersInfo"));
	PlayerPriv[playerid][CanTeleportPlayersToYourself] = strval(db_Get(dinifilename,"CanTeleportPlayersToYourself"));
	PlayerPriv[playerid][CanGiveWeapons] = strval(db_Get(dinifilename,"CanGiveWeapons"));
	PlayerPriv[playerid][CanKickPlayers] = strval(db_Get(dinifilename,"CanKickPlayers"));
	PlayerPriv[playerid][CanLockServer] = strval(db_Get(dinifilename,"CanLockServer"));
	PlayerPriv[playerid][CanUnlockServer] = strval(db_Get(dinifilename,"CanUnlockServer"));
	PlayerPriv[playerid][CanReloadMOTD] = strval(db_Get(dinifilename,"CanReloadMOTD"));
	PlayerPriv[playerid][CanStartCashBox] = strval(db_Get(dinifilename,"CanStartCashBox"));
	PlayerPriv[playerid][CanMark] = strval(db_Get(dinifilename,"CanMark"));
	PlayerPriv[playerid][CannotBeKicked] = strval(db_Get(dinifilename,"CannotBeKicked"));
	PlayerPriv[playerid][CanSpectate] = strval(db_Get(dinifilename,"CanSpectate"));
	PlayerPriv[playerid][CanSpySMS] = strval(db_Get(dinifilename,"CanSpySMS"));
	PlayerPriv[playerid][CanSetAntiBikeFall] = strval(db_Get(dinifilename,"CanSetAntiBikeFall"));
	PlayerPriv[playerid][CanBanPlayers] = strval(db_Get(dinifilename,"CanBanPlayers"));
	PlayerPriv[playerid][CanTeleportToFilePos] = strval(db_Get(dinifilename,"CanTeleportToFilePos"));
	PlayerPriv[playerid][CanChangeWeather] = strval(db_Get(dinifilename,"CanChangeWeather"));
	PlayerPriv[playerid][CanStartDuels] = strval(db_Get(dinifilename,"CanStartDuels"));
	PlayerPriv[playerid][CanMapTeleport] = strval(db_Get(dinifilename,"CanMapTeleport"));
	if (PlayerPriv[playerid][CanMapTeleport] == 1) AllowPlayerTeleport(playerid,true);
	PlayerPriv[playerid][CanCrashPlayers] = strval(db_Get(dinifilename,"CanCrashPlayers"));
	PlayerPriv[playerid][CanBuildHouses] = strval(db_Get(dinifilename,"CanBuildHouses"));
	PlayerPriv[playerid][CanStartHay] = strval(db_Get(dinifilename,"CanStartHay"));
	PlayerPriv[playerid][CanExecFakeCMD] = strval(db_Get(dinifilename,"CanExecFakeCMD"));
	Debug("privilegies.inc > player_load_privilegies - Stop");
}

public player_ResetPrivilegies(playerid)
{
	Debug("player.inc > player_ResetPrivilegies - Start");
	PlayerPriv[playerid][IsModerator] = 0; 
	PlayerPriv[playerid][CanBroadcast] = 0;
	PlayerPriv[playerid][CanWorldSave] = 0;
	PlayerPriv[playerid][CanAnnounce] = 0;
	PlayerPriv[playerid][CanGiveXP] = 0;
	PlayerPriv[playerid][CanSlap] = 0;
	PlayerPriv[playerid][CanGiveMoney] = 0;
	PlayerPriv[playerid][CanSetMoney] = 0;
	PlayerPriv[playerid][CanSetBank] = 0;
	PlayerPriv[playerid][CanAddCrimes] = 0;
	PlayerPriv[playerid][CanDisarm] = 0;
	PlayerPriv[playerid][CanFreeze] = 0;
	PlayerPriv[playerid][CanUnFreeze] = 0;
	PlayerPriv[playerid][CanHeal] = 0;
	PlayerPriv[playerid][CanGod] = 0;
	PlayerPriv[playerid][CanMurder] = 0;
	PlayerPriv[playerid][CanSilentMurder] = 0;
	PlayerPriv[playerid][CanJail] = 0;
	PlayerPriv[playerid][CanUnjail] = 0;
	PlayerPriv[playerid][CanJailList] = 0;
	PlayerPriv[playerid][CanMute] = 0;
	PlayerPriv[playerid][CanMuteList] = 0;
	PlayerPriv[playerid][CanUnmute] = 0;
	PlayerPriv[playerid][CanWarn] = 0;
	PlayerPriv[playerid][CanUnwarn] = 0;
	PlayerPriv[playerid][CanLockAcc] = 0;
	PlayerPriv[playerid][CanUnlockAcc] = 0;
	PlayerPriv[playerid][CanMakeItems] = 0;
	PlayerPriv[playerid][CanStartDM] = 0;
	PlayerPriv[playerid][CanStartRace] = 0;
	PlayerPriv[playerid][CanEditRace] = 0;
	PlayerPriv[playerid][CanStartHotwire] = 0;
	PlayerPriv[playerid][CanTeleportToPlayers] = 0;
	PlayerPriv[playerid][CanSeePlayersInfo] = 0;
	PlayerPriv[playerid][CanTeleportPlayersToYourself] = 0;
	PlayerPriv[playerid][CanGiveWeapons] = 0;
	PlayerPriv[playerid][CanKickPlayers] = 0;
	PlayerPriv[playerid][CanLockServer] = 0;
	PlayerPriv[playerid][CanUnlockServer] = 0;
	PlayerPriv[playerid][CanReloadMOTD] = 0;
	PlayerPriv[playerid][CanStartCashBox] = 0;
	PlayerPriv[playerid][CanMark] = 0;
	PlayerPriv[playerid][CannotBeKicked] = 0;
	PlayerPriv[playerid][CanSpectate] = 0;
	PlayerPriv[playerid][CanSpySMS] = 0;
	PlayerPriv[playerid][CanSetAntiBikeFall] = 0;
	PlayerPriv[playerid][CanBanPlayers] = 0;
	PlayerPriv[playerid][CanTeleportToFilePos] = 0;
	PlayerPriv[playerid][CanChangeWeather] = 0;
	PlayerPriv[playerid][CanStartDuels] = 0;
	PlayerPriv[playerid][CanMapTeleport] = 0;
	PlayerPriv[playerid][CanCrashPlayers] = 0;
	PlayerPriv[playerid][CanBuildHouses] = 0;
	PlayerPriv[playerid][CanStartHay] = 0;
	PlayerPriv[playerid][CanExecFakeCMD] = 0;
	AllowPlayerTeleport(playerid,false);
	Debug("player.inc > player_ResetPrivilegies - Stop");
}