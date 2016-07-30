#include <a_samp> 							// samp
#include "base_defsettings"                 // settings defines
#include "base" 							// holds base script values

#include "utils\gtoutils" 					// misc used utils
#include "dmap"                             // online map. only if using DJson
#include "utils\dbs\djson"                  // djson db engine
#include "utils\dbs\dini"                   // dini db engine
#include "utils\db"                         // database handlers
#include "utils\dutils" 					// more used tools
#include "streamers\pickups"                // dynamic pickups
#include "streamers\mapicons"               // dynamic mapicons
#include "streamers\objects"                // dynamic objects
#include "restart"                          // improve stability
#include "weather"                          // weather
#include "synctime"                         // sync time on server
#include "intweap"                          // enable/disable weapons in interiors
#include "moneydrop"                        // drop money on death
#include "tempban"                          // temporary bans

#include "account" 							// account handler
#include "player"							// holds player values
#include "spawnsel"                         // spawn selection
#include "spawnplace"                       // change spawn place
#include "privilegies"                      // privilegies system
#include "weapons" 							// weapons and armsdealers
#include "vehicles"    		     			// vehicles
#include "world"							// functions for zone, location, world, etc
#include "commandhandler" 					// command handler
#include "gang"               			    // Gang handler
#include "housing"                          // Housing handler
#include "logging"                          // Logging handler
#include "race" 							// race handler, manages and runs all rasces
//#include "mission"                          // mission handler
#include "deathmatch"   			        // deathmatch handler
#include "hay"                              // hay events
#include "bank"         			   		// bank money to keep it on death
#include "payday" 							// pay players money based on level, job...
#include "chatsystem"                       // chat system
#include "phone"                            // phone talking
#include "vehicles_extras"                  // fuel, speedo
#include "school"                           // driving schools
#include "carlock"                          // lock/unlock your vehicle
//#include "hotwire"                          // hotwire event
#include "cashbox"                          // cashbox event
#include "motd"                             // message of the day
#include "rules"                            // rules command :)
#include "club"                             // just a fun feature
#include "stadium"                          // another fun feature :)
#include "multikill"                        // multikill
#include "hotspot"                          // hotspots
#include "groundhold"                       // hold ground to get money
#include "duel"                             // duel between players
#include "derby"                            // derby
#include "bets"                             // bets on players for winning in DM and Race
#include "votekick"                         // yeah :) from PEN1
#include "lotto"                            // lottery from Godfather
#include "hotspot"                          // idea from GTAT
#include "vehiclehpbar"                     // vehicle health bar
#include "tram"                             // tram/RC entering
#include "collect\packets"                  // packets from SP
#include "collect\horseshoes"               // horseshoes from SP
#include "collect\oysters"                  // oysters from SP

#include "businesses\drinks"                // energy drinks handler
#include "businesses\weapon"                // buy weapons handler
#include "businesses\clothes"               // clothes shop
#include "businesses\food"                  // food
#include "businesses\mines"                 // mines
#include "businesses\skydive"               // skydive business
#include "businesses\paynspray"             // respray vehicle, lost crimes
#include "businesses\management"            // control your business
#include "businesses\global"                // for commands

#include "jobs\cop"                         // police job
#include "jobs\medic"                       // medic job
#include "jobs\job_chat"                    // job chat :)
#include "jobs\taxi"                        // taxi driver job, converted from SAMP_DMA+LG+PB+SR
#include "bribe"                            // bribe for cops

#include "boxing"                           // boxing module, converted from Godfather
#include "wangexports"                      // wang exports, converted from SAMP_DMA+LG+PB+SR
#include "weaponsstat"                      // detailed kills/deaths stat

#include "races\sasuperlap"
#include "races\badlands"
#include "races\control"
#include "races\desertmadness"
#include "races\thegrove"
#include "races\offroad"
#include "races\flyingfree"
#include "races\mullholland"
#include "races\jerry"
#include "races\outburst"
#include "races\fastlane"
#include "races\marina"
#include "races\fullturn"
#include "races\backstreetbang"
#include "races\baysidetour"
#include "races\racepoint"
#include "races\goingup"
#include "races\dustyroads"
#include "races\sanchezpower"
#include "races\dustdevil"
#include "races\lvtour"
#include "races\oldschool"
#include "races\speeder"
#include "races\nowhere"
#include "races\madness"
#include "races\waterbridge"
#include "races\sfur"
#include "races\airportdrift"
#include "races\torrent"
#include "races\tolsma"
#include "races\juliusthruway"
#include "races\enclose"
#include "races\sfrace"
#include "races\tourismo"
#include "races\murderhorn"
#include "races\msr"
#include "races\reckless"
#include "races\grandtour"
#include "races\pace"
#include "races\savarona"
#include "races\hilltop"
#include "races\lscs"
#include "races\wasabisf"
#include "races\oil"
#include "races\rightturn"
#include "races\caper2"
#include "races\fromgrovetodesert"
#include "races\lscops"
#include "races\fleethecity"
#include "races\efenish"
#include "races\msr2"
#include "races\riversiderun"
#include "races\tourismo2"
#include "races\lstour"
#include "races\lsrace2"
#include "races\shave"
#include "races\backroads"
#include "races\freewheel"
#include "races\sels"
#include "races\countrycruise"
#include "races\beachfront"
#include "races\canalcontrol"
#include "races\lostinsmoke"
#include "races\lssf"
#include "races\m25"
#include "races\dune"
#include "races\caper"
#include "races\sflvl"
#include "races\wasabi2"
#include "races\striptease"
#include "races\lsrace"
#include "races\resign"
#include "races\thestrip"
#include "races\sflsl"
#include "races\freeway"
#include "races\roadrash"
#include "races\metaline"
#include "races\lvrace"
#include "races\annoying"
#include "races\lvrace2"
#include "races\sftour"
#include "races\treacherous"
#include "races\monstertruck"
#include "races\bullitt"
#include "races\desertrally"
#include "races\ls_trailer"
#include "races\ls_majestic"
#include "races\roundwego"
#include "races\sf_wood"
#include "races\sp_backroadwanderer"
#include "races\sp_badlandsa"
#include "races\sp_badlandsb"
#include "races\sp_banditocountry"
#include "races\sp_citycircuit"
#include "races\sp_countryendurance"
#include "races\sp_damrider"
#include "races\sp_dirtbikedanger"
#include "races\sp_freeway"
#include "races\sp_gogokarting"
#include "races\sp_intothecountry"
#include "races\sp_littleloop"
#include "races\sp_lowrider"
#include "races\sp_lvringroad"
#include "races\sp_sffastlane"
#include "races\sp_sfhills"
#include "races\sp_sftolv"
#include "races\sp_vinewood"
#include "races\airportrun"
#include "races\backstreet"
#include "races\bytheriver"
#include "races\casinotour"
#include "races\countryroad"
#include "races\countryvillage"
#include "races\dirtydesert"
#include "races\dirtygras"
#include "races\littlejourney"
#include "races\mountchilliad"
#include "races\offthetrack"
#include "races\overthehills"
#include "races\roadtohell"
#include "races\aroundandaround"
#include "races\somewherearound"
#include "races\thenextepisode"
#include "races\westofls"
#include "races\freewaytocountry"
#include "races\eastlascolinas"
#include "races\lsdocks"
#include "races\playadelseville"
#include "races\lsinternational"
#include "races\pershingvinewood"
#include "races\ls_holiday"
#include "races\ls_topround"
#include "races\morningcross"
#include "races\belowtheocean"
#include "races\toarea51"
#include "races\lasventuras"
#include "races\sanfierro"
#include "races\sfcitytours"
#include "races\crossways"
#include "races\tollstation"
#include "races\tourdels"
#include "races\bouncer"
#include "races\beachtour"
#include "races\loopway"
#include "races\shopping"


//#include "missions\houseclimbing"
//#include "missions\pron"

#include "moderation\commands"   		    // moderator commands
#include "moderation\commands_dm"          	// moderator DM commands
#include "moderation\commands_race"        	// moderator race commands
#include "moderation\commands_minigames"  // moderator hotwire commands
#include "moderation\commands_spec"         // moderator spectate commands
#include "moderation\commands_teleport_file"//teleport to places
#include "moderation\commands_house"        // house creation
#include "moderation\commands_hay"          // moderator hay commands
#include "moderation\antifall"              // anti-bike-fall

#include "protections\driveby"          	// anti drive-by
#include "protections\carkill"              // anti car-kill
#include "protections\spawnkill"        	// protection VS spawnkill
#include "protections\flame"            	// no more bad words
#include "protections\anticaps"             // anti-caps
#include "protections\idle"             	// anti-idle
#include "protections\weaponcheat"      	// no comments
#include "protections\spam"                 // anti-spam
#include "protections\flood"            	// anti-flood
#include "protections\pingkicker"           // too high ping? use it
#include "protections\jetpack"              // anti-jetpack
#include "protections\freeze"               // anti-freeze
#include "protections\spoofkill"            // anti-spoof kill
#include "protections\teleport"             // anti-teleport
#include "protections\helikill"             // no comments
#include "protections\speed"                // yes, it's back :)
#include "protections\reconnect"            // no comments
#include "protections\bots"                 // vs bots
#include "protections\forbidnames"          // NUL, COM, LTP, etc...
#include "protections\money"                // money
#include "protections\eventkill"            // killing while waiting for events
#include "protections\swimkill"          	// kill swimming player
#include "protections\carjacking"           // carjacking
#include "protections\rconbrute"            // rcon bruteforce

#include "deathmatches\area51"
#include "deathmatches\towertop"
#include "deathmatches\badandugly"
#include "deathmatches\castle"
#include "deathmatches\bluemountains"
#include "deathmatches\plateau"
#include "deathmatches\pier69"
#include "deathmatches\lastdime"
#include "deathmatches\cagematch"
#include "deathmatches\cargoship"
#include "deathmatches\ganton"
#include "deathmatches\airportcourtyard"
#include "deathmatches\golfcourse"
#include "deathmatches\glen"
#include "deathmatches\hangar"
#include "deathmatches\bigfarm"
#include "deathmatches\cabinfever"
#include "deathmatches\nextekb"
#include "deathmatches\estateofhate"
#include "deathmatches\office"
#include "deathmatches\containers"
#include "deathmatches\campus"
#include "deathmatches\openwar"
#include "deathmatches\openwarii"
#include "deathmatches\cardealer"
#include "deathmatches\theboat"
#include "deathmatches\graveyard"
#include "deathmatches\suburb"
#include "deathmatches\lossantostennis"
#include "deathmatches\thefarm"
#include "deathmatches\minigunmadness"
#include "deathmatches\dildo"
#include "deathmatches\targetball"
#include "deathmatches\mbase"
#include "deathmatches\sfairport"
#include "deathmatches\court"
#include "deathmatches\maniacsgraveyard"
#include "deathmatches\basejump"
#include "deathmatches\silentmotel"
#include "deathmatches\underbridge"
#include "deathmatches\circle"
#include "deathmatches\baseballfield"
#include "deathmatches\construction"
#include "deathmatches\bigears"
#include "deathmatches\docks"
#include "deathmatches\thepinkroom"
#include "deathmatches\pirateview"
#include "deathmatches\hellsite"
#include "deathmatches\trailers"
#include "deathmatches\ruins"
#include "deathmatches\truckyard"
#include "deathmatches\atlantis"
#include "deathmatches\palominocreek"
#include "deathmatches\cityhall"
#include "deathmatches\rockettown"
#include "deathmatches\stuntpark"
#include "deathmatches\trainhouse"
#include "deathmatches\threetowers"
#include "deathmatches\veronasamurai"
#include "deathmatches\dump"
#include "deathmatches\deathway"
#include "deathmatches\quarry"
#include "deathmatches\oilrefinery"
#include "deathmatches\poolday"
#include "deathmatches\usnavy"
#include "deathmatches\abandon"
#include "deathmatches\wood"
#include "deathmatches\toyworld"
#include "deathmatches\sfdrivingschool"
#include "deathmatches\transformerstation"
#include "deathmatches\marihuanafarm"
#include "deathmatches\rooftopmadness"
#include "deathmatches\jizzyclub"
#include "deathmatches\smallroof"
#include "deathmatches\wang"
#include "deathmatches\farmonthehill"
#include "deathmatches\fostervalley"

#include "hays\blueberry"
#include "hays\lstower"
#include "hays\lvtower"
#include "hays\mine"
#include "hays\mtchilliad"

new TimerPickupCheck;
forward PickupCheckTimer();

main()
{
}

public OnGameModeInit()
{
//Console Message
    db_OnGameModeInit();
	Base_OnGameModeInit();
	print("\n----------------------------------");
	print(" ");
	printf("%s Initialising, please wait... \n",gamemodename);
	print(" ");
 	print("----------------------------------\n");
 	Logging_OnGameModeInit();

//Initialize everything that needs it
	dmap_OnGameModeInit();
	IntWeap_OnGameModeInit();
    Language_OnGameModeInit();
    Utils_OnGameModeInit();
	Account_OnGameModeInit();
	Player_OnGameModeInit();
	Privilegies_OnGameModeInit();
	Gang_OnGameModeInit();
	PayDay_OnGameModeInit();
	MOTD_OnGameModeInit();
	Rules_OnGameModeInit();
	CH_OnGameModeInit();
	// Protections
	AntiFlame_OnGameModeInit();
	AntiCaps_OnGameModeInit();
	SpawnKill_OnGameModeInit();
	PingKicker_OnGameModeInit();
	AntiDriveBy_OnGameModeInit();
	AntiCarKill_OnGameModeInit();
	AntiHeliKill_OnGameModeInit();
	AntiIdle_OnGameModeInit();
	AntiFlood_OnGameModeInit();
	AntiSpam_OnGameModeInit();
	AntiWeapon_OnGameModeInit();
	AntiSpoofKill_OnGameModeInit();
	AntiJetpack_OnGameModeInit();
	AntiMoneyCheat_OnGameModeInit();
	AntiTele_OnGameModeInit();
	AntiFreeze_OnGameModeInit();
	AntiSpeed_OnGameModeInit();
	Reconnect_OnGameModeInit();
	AntiBot_OnGameModeInit();
	ForbiddenNames_OnGameModeInit();
	AntiEventKill_OnGameModeInit();
	AntiSwimKill_OnGameModeInit();
	AntiCJ_OnGameModeInit();
	rconbrute_OnGameModeInit();
 	// Protections End
 	Vehicles_OnGameModeInit();
	DM_OnGameModeInit();
	Hay_OnGameModeInit();
	Race_OnGameModeInit();
	//Hotwire_OnGameModeInit();
	CashBox_OnGameModeInit();
	//Stat_OnGameModeInit();
	WangExport_OnGameModeInit();
 	Business_OnGameModeInit();
	Weapon_OnGameModeInit();
	Boxing_OnGameModeInit();
	ArmDealers_OnGameModeInit();
	Bank_OnGameModeInit();
	Club_OnGameModeInit();
	Drinks_OnGameModeInit();
	Mines_OnGameModeInit();
	PayNSpray_OnGameModeInit();
	Skydive_OnGameModeInit();
	Dinners_OnGameModeInit();
	Clothes_OnGameModeInit();
	Housing_OnGameModeInit();
	Cop_OnGameModeInit();
	Taxi_OnGameModeInit();
	carlock_OnGameModeInit();
	Medic_OnGameModeInit();
	FuelStations_OnGameModeInit();
	Groundhold_OnGameModeInit();
	Fuel_OnGameModeInit();
    Phone_OnGameModeInit();
    restart_OnGameModeInit();
    School_OnGameModeInit();
    Weather_OnGameModeInit();
    duel_OnGameModeInit();
    Derby_OnGameModeInit();
    Stadium_OnGameModeInit();
    multikill_OnGameModeInit();
    //Missions_OnGameModeInit();
    ModTP_OnGameModeInit();
    ModHB_OnGameModeInit();
    Votekick_OnGameModeInit();
    Hotspot_OnGameModeInit();
    SyncTime_OnGameModeInit();
    Lotto_OnGameModeInit();
    Vehicle_HP_Bar_OnOnGameModeInit();
    MoneyDrop_OnOnGameModeInit();
    Packets_OnGameModeInit();
    Horseshoes_OnGameModeInit();
    Oysters_OnGameModeInit();
    tempban_OnGameModeInit();

    race_outburst_init();
	race_sasuperlap_init();
	race_ls_majestic_init();
	race_thegrove_init();
    race_offroad_init();
 	race_countryroad_init();
    race_tourismo_init();
    race_oldschool_init();
    race_sp_citycircuit_init();
    race_sflsl_init();
    race_efenish_init();
	race_bytheriver_init();
    race_backstreetbang_init();
    race_belowtheocean_init();
	race_toarea51_init();
	race_lasventuras_init();
	race_sanfierro_init();
	race_sfcitytours_init();
	race_crossways_init();
	race_tollstation_init();
	race_tourdels_init();
	race_bouncer_init();
	race_beachtour_init();
	race_loopway_init();
	race_shopping_init();
    race_reckless_init();
    race_sp_damrider_init();
    race_freeway_init();
    race_riversiderun_init();
	race_sels_init();
	race_sp_freeway_init();
	race_msr2_init();
	race_roadtohell_init();
    race_fastlane_init();
    race_beachfront_init();
    race_sp_backroadwanderer_init();
    race_ls_topround_init();
    race_sflvl_init();
    race_lvtour_init();
    race_baysidetour_init();
    race_sfrace_init();
	race_dirtygras_init();
    race_sp_banditocountry_init();
    race_dune_init();
    race_savarona_init();
    race_wasabi2_init();
    race_sp_littleloop_init();
    race_freewheel_init();
    race_desertmadness_init();
    race_sftour_init();
    race_annoying_init();
    race_sp_badlandsa_init();
    race_torrent_init();
    race_goingup_init();
    race_countryvillage_init();
    race_oil_init();
	race_sp_vinewood_init();
	race_roundwego_init();
    race_flyingfree_init();
    race_sf_wood_init();
    race_dustdevil_init();
    race_sanchezpower_init();
    race_sp_sfhills_init();
    race_lstour_init();
	race_badlands_init();
	race_aroundandaround_init();
	race_somewherearound_init();
	race_thenextepisode_init();
	race_westofls_init();
	race_freewaytocountry_init();
	race_eastlascolinas_init();
	race_lsdocks_init();
	race_playadelseville_init();
	race_lsinternational_init();
	race_pershingvinewood_init();
	race_lsrace_init();
	race_shave_init();
	race_mountchilliad_init();
	race_sp_countryendurance_init();
	race_murderhorn_init();
	race_marina_init();
	race_lvrace_init();
	race_resign_init();
	race_sp_intothecountry_init();
    race_roadrash_init();
    race_desertrally_init();
    race_fullturn_init();
    race_lssf_init();
    race_sp_badlandsb_init();
    race_jerry_init();
    race_backstreet_init();
    race_ls_holiday_init();
    race_hilltop_init();
    race_lsrace2_init();
    race_sp_dirtbikedanger_init();
    race_m25_init();
    race_fleethecity_init();
    race_waterbridge_init();
    race_madness_init();
    race_control_init();
    race_sp_gogokarting_init();
    race_airportdrift_init();
    race_monstertruck_init();
    race_fromgrovetodesert_init();
    race_caper_init();
    race_juliusthruway_init();
	race_metaline_init();
	race_dirtydesert_init();
    race_nowhere_init();
    race_sp_lowrider_init();
    race_lscops_init();
    race_countrycruise_init();
    race_lscs_init();
    race_sp_sftolv_init();
 	race_overthehills_init();
    race_speeder_init();
    race_sfur_init();
    race_lostinsmoke_init();
    race_striptease_init();
    race_pace_init();
    race_mullholland_init();
    race_ls_trailer_init();
    race_airportrun_init();
    race_sp_lvringroad_init();
    race_msr_init();
    race_backroads_init();
    race_canalcontrol_init();
    race_enclose_init();
    race_rightturn_init();
	race_offthetrack_init();
    race_racepoint_init();
    race_bullitt_init();
    race_wasabisf_init();
    race_tourismo2_init();
    race_thestrip_init();
    race_morningcross_init();
    race_lvrace2_init();
	race_littlejourney_init();
    race_sp_sffastlane_init();
    race_grandtour_init();
    race_caper2_init();
    race_tolsma_init();
    race_treacherous_init();
    race_dustyroads_init();
	race_casinotour_init();

    dm_area51_init();
	dm_towertop_init();
  	dm_badandugly_init();
  	dm_rooftopmadness_init();
  	dm_nextekb_init();
	dm_castle_init();
	dm_cabinfever_init();
  	dm_bluemountains_init();
 	dm_cargoship_init();
 	dm_thefarm_init();
  	dm_golfcourse_init();
  	dm_palominocreek_init();
  	dm_pier69_init();
  	dm_mbase_init();
  	dm_pirateview_init();
  	dm_fostervalley_init();
  	dm_circle_init();
  	dm_deathway_init();
  	dm_maniacsgraveyard_init();
  	dm_oilrefinery_init();
	dm_cardealer_init();
	dm_office_init();
	dm_wang_init();
	dm_thepinkroom_init();
	dm_trailers_init();
	dm_threetowers_init();
	dm_jizzyclub_init();
	dm_sfairport_init();
	dm_ganton_init();
	dm_lossantostennis_init();
	dm_suburb_init();
	dm_airportcourtyard_init();
  	dm_plateau_init();
  	dm_estateofhate_init();
  	dm_toyworld_init();
  	dm_construction_init();
  	dm_truckyard_init();
  	dm_hangar_init();
  	dm_marihuanafarm_init();
  	dm_minigunmadness_init();
  	dm_glen_init();
  	dm_stuntpark_init();
  	dm_bigfarm_init();
  	dm_theboat_init();
  	dm_poolday_init();
	dm_campus_init();
  	dm_trainhouse_init();
  	dm_silentmotel_init();
  	dm_dump_init();
  	dm_containers_init();
 	dm_docks_init();
  	dm_openwar_init();
  	dm_smallroof_init();
  	dm_ruins_init();
  	dm_sfdrivingschool_init();
  	dm_hellsite_init();
	dm_cityhall_init();
  	dm_dildo_init();
  	dm_transformerstation_init();
  	dm_court_init();
  	dm_targetball_init();
  	dm_cagematch_init();
 	dm_veronasamurai_init();
  	dm_usnavy_init();
  	dm_atlantis_init();
  	dm_lastdime_init();
  	dm_openwarii_init();
  	dm_baseballfield_init();
  	dm_abandon_init();
	dm_wood_init();
	dm_bigears_init();
	dm_quarry_init();
	dm_underbridge_init();
	dm_graveyard_init();
	dm_basejump_init();
	dm_rockettown_init();
	dm_farmonthehill_init();

	hay_lvtower_init();
	hay_mine_init();
	hay_blueberry_init();
	hay_mtchilliad_init();
	hay_lstower_init();

	//mission_houseclimbing_init();
	//mission_pron_init();
	
//Set timers for anything that needs constant checking
	TimerPickupCheck = SetTimer("PickupCheckTimer",500,1);

//Spawn the world
    World_OnGameModeInit();
    mapicons_OnGameModeInit();
    Pickups_OnGameModeInit();
    Objects_OnGameModeInit();
    new logstring[MAX_STRING];
 	if (MAX_PLAYERS_EX > MAX_PLAYERS){
 	    format(logstring,MAX_STRING,"ERROR! Set 'maxplayers %d' in your 'server.cfg'",MAX_PLAYERS);
 	    WriteLog(logstring);
	    SendRconCommand("exit");
	} else {
		format(logstring,MAX_STRING,"%s Initialization Complete!",gamemodename);
		WriteLog(logstring);
	}
	return 1;
	}

	public OnPlayerConnect(playerid)
	{
	    Debug("gamemode.pwn > OnPlayerConnect - Start");
	    ResetOtherPlayerData(playerid); //Clear vars
	    if (IsNameForbidden(oGetPlayerName(playerid))){
	        KickPlayer(playerid,-1,gettext(1493));
	        Debug("gamemode.pwn > OnPlayerConnect - Stop");
			return 1;
	    }
	    if (ServerLocked == 1){
	        SystemMsg(playerid,COLOUR_ERROR,gettext(1311));
	        Kick(playerid);
	        Debug("gamemode.pwn > OnPlayerConnect - Stop");
			return 1;
	    }
	    ConnectMSG(playerid);
	    IntWeap_OnPlayerConnect(playerid);
	    Reconnect_OnPlayerConnect(playerid);
	    AntiBot_OnPlayerConnect(playerid);
	    AntiCJ_OnPlayerConnect(playerid);
	    SpawnSel_OnPlayerConnect(playerid);
	    Bets_OnPlayerConnect(playerid);
	    WeaponsStat_OnPlayerConnect(playerid);
	    player_OnPlayerConnect(playerid);
	    restart_OnPlayerConnect(playerid);
	    MOTD_OnPlayerConnect(playerid);
		Boxing_OnPlayerConnect(playerid);
		carlock_OnPlayerConnect(playerid);
		Phone_OnPlayerConnect(playerid);
		World_OnPlayerConnect(playerid);
		Derby_OnPlayerConnect(playerid);
		//Hotwire_OnPlayerConnect(playerid);
		//Missions_OnPlayerConnect(playerid);
		Votekick_OnPlayerConnect(playerid);
		multikill_OnPlayerConnect(playerid);
		Bribe_OnPlayerConnect(playerid);
		Account_OnPlayerConnect(playerid);
		tempban_OnPlayerConnect(playerid);
       	Debug("gamemode.pwn > OnPlayerConnect - Stop");
		return 1;
	}

	public OnPlayerDisconnect(playerid,reason)
	{
	    Debug("gamemode.pwn > OnPlayerDisconnect - Start");
		if (playerid == INVALID_PLAYER_ID)
		{
            Debug("gamemode.pwn > OnPlayerDisconnect - Stop)");
			return;
		}
		Spec_OnPlayerDisconnect(playerid,reason);
		Reconnect_OnPlayerDisconnect(playerid,reason);
		player_Disconnect(playerid);
		CashBox_OnPlayerDisconnect(playerid);
		DisconnectMsg(playerid, reason);
		Bets_OnPlayerDisconnect(playerid,reason);
		Race_OnPlayerDisconnect(playerid);
		Boxing_OnPlayerDisconnect(playerid);
		DM_OnPlayerDisconnect(playerid);
		Hay_OnPlayerDisconnect(playerid);
		Phone_OnPlayerDisconnect(playerid);
		Club_OnPlayerDisconnect(playerid);
		//Missions_OnPlayerDisconnect(playerid,reason);
		multikill_OnPlayerDisconnect(playerid);
		carlock_OnPlayerDisconnect(playerid,reason);
		Taxi_OnPlayerDisconnect(playerid);
		Votekick_OnPlayerDisconnect(playerid,reason);
		duel_OnPlayerDisconnect(playerid,reason);
		Derby_OnPlayerDisconnect(playerid,reason);
        SyncTime_OnPlayerDisconnect(playerid,reason);
        Bribe_OnPlayerDisconnect(playerid,reason);
        WeaponsStat_OnPlayerDisconnect(playerid,reason);
        Account_OnPlayerDisconnect(playerid,reason);
        Objects_OnPlayerDisconnect(playerid,reason);
        ModHB_OnPlayerDisconnect(playerid,reason);
		ResetOtherPlayerData(playerid); //Clear vars
		WorldSave(); 
		Debug("gamemode.pwn > OnPlayerDisconnect - Stop");
	}

	public OnPlayerEnterCheckpoint(playerid)
	{
	    Debug("gamemode.pwn > OnPlayerEnterCheckpoint - Start");
		DM_OnPlayerEnterCheckpoint(playerid);
		Hay_OnPlayerEnterCheckpoint(playerid);
		//Hotwire_OnPlayerEnterCheckpoint(playerid);
  		Fuel_OnPlayerEnterCheckpoint(playerid);
  		CashBox_OnPlayerEnterCheckpoint(playerid);
  		Wang_OnPlayerEnterCheckpoint(playerid);
  		//Missions_OnPlayerEnterCP(playerid);
  		Stadium_OnPlayerEnterCheckpoint(playerid);
  		Derby_OnPlayerEnterCheckpoint(playerid);
  		School_OnPlayerEnterCheckpoint(playerid);
        Debug("gamemode.pwn > OnPlayerEnterCheckpoint - Stop");
	}
	
	public OnPlayerEnterRaceCheckpoint(playerid)
	{
	    Race_OnPlayerEnterRaceCP(playerid);
	    return 1;
	}

	public OnPlayerDeath(playerid, killerid, reason)
	{
	    Debug("gamemode.pwn > OnPlayerDeath - Start");
 		if (!IsPlayerConnected(playerid))
 		{
            Debug("gamemode.pwn > OnPlayerDeath - Stop");
			return 1;
		}
		PlayerSpawned[playerid] = false;

		if (PlayerWantMedic[playerid] == 1){
		    new string[MAX_STRING];
		    format(string,MAX_STRING,gettext(1313),oGetPlayerName(playerid));
		    SendMsgToMedics(string);
		}
		PlayerCity[playerid] = GetXYZCityID(PlayerPos[playerid][Coord_X],PlayerPos[playerid][Coord_Y],PlayerPos[playerid][Coord_Z]);
    	//if(playerid != killerid)  Stat_AddPlayerKills(killerid, reason);
		if (IsPlayerInAnyDM(playerid))
		{
			DM_OnPlayerDeath(playerid,killerid);
		}
		else if (IsPlayerInAnyHay(playerid))
		{
		    Hay_OnPlayerDeath(playerid,killerid);
		}
		else
		{
            if (PlayerBoxing[playerid] > 0){
	 			Boxing_OnPlayerDeath(playerid,killerid,reason);
			} else	if (Player[playerid][GotJob] == JOB_COP){
				Cop_OnPlayerDeath(playerid,killerid,reason);
			} else if (Player[playerid][GotJob] == JOB_MEDIC){
               	Medic_OnPlayerDeath(playerid,killerid,reason);
			} else if (Player[playerid][GotJob] == JOB_TAXIDRIVER){
			    Taxi_OnPlayerDeath(playerid, killerid, reason);
			} else {
				PlayerDeath(playerid, killerid, reason);
				DeathMSG(playerid, killerid, reason);
			}
    	}
		CashBox_OnPlayerDeath(playerid);
		Phone_OnPlayerDeath(playerid,killerid,reason);
		World_OnPlayerDeath(playerid);
		//Missions_OnPlayerDeath(playerid, killerid, reason);
		duel_OnPlayerDeath(playerid,killerid,reason);
		Derby_OnPlayerDeath(playerid,killerid,reason);
		Bribe_OnPlayerDeath(playerid,killerid,reason);
		SyncTime_OnPlayerDeath(playerid, killerid, reason);
		WeaponsStat_OnPlayerDeath(playerid,killerid,reason);

		PlayerSkydive[playerid] = 0;

		Player[playerid][Deaths]++;
		if (killerid == INVALID_PLAYER_ID)
		{
        	new logstring[256];
			format(logstring, sizeof (logstring), "player: %d: %s: has died > Reason: (%d)",playerid,oGetPlayerName(playerid),reason);
    		WriteLog(logstring);
    		Player[playerid][Suicides]++;
			Debug("gamemode.pwn > OnPlayerDeath - Stop");
			return 1;
		} else {
		    new logstring[256];
			format(logstring, sizeof (logstring), "player: %d: %s: has killed player %s(%d)> Reason: (%d)",killerid,oGetPlayerName(killerid),oGetPlayerName(playerid),playerid,reason);
    		WriteLog(logstring);
    		Player[killerid][Kills]++;
   		}
		if (!IsPlayerInDuel(playerid) && !IsPlayerInDuel(killerid)){
 			if (IsPlayerInAnyDM(playerid))
			{
					OnPlayerDMKill(killerid,playerid,reason);
			}
			else
			{
			        multikill_OnPlayerDeath(playerid,killerid,reason);
					PlayerKill(killerid, playerid, reason);
			}
		}

		AntiSpoofKill_OnPlayerDeath(playerid, killerid, reason);
    	AntiDriveBy_OnPlayerDeath(playerid, killerid, reason);
    	AntiCarKill_OnPlayerDeath(playerid, killerid, reason);
    	AntiHeliKill_OnPlayerDeath(playerid, killerid, reason);
		AntiSpawnKill_OnPlayerDeath(playerid, killerid, reason);
		AntiWeapon_OnPlayerDeath(playerid, killerid, reason);
		AntiEventKill_OnPlayerDeath(playerid, killerid, reason);
		AntiSwimKill_OnPlayerDeath(playerid, killerid, reason);
    	Debug("gamemode.pwn > OnPlayerDeath - Stop");
		return 1;
}

	public OnPlayerSpawn(playerid)
	{
        Debug("gamemode.pwn > OnPlayerSpawn - Start");
  	  	if (!IsPlayerConnected(playerid))
		{
            Debug("gamemode.pwn > OnPlayerSpawn - Stop");
			return 1;
		}
		PlayerSpawned[playerid] = true;
  		World_OnPlayerSpawn(playerid);
  		AntiFreeze_OnPlayerSpawn(playerid);
  		Taxi_OnPlayerSpawn(playerid);
  		SyncTime_OnPlayerSpawn(playerid);
  		Gang_OnPlayerSpawn(playerid);

	    new dmid = GetPlayerDM(playerid);
   	 	if ((dmid == INVALID_DM_ID) || (DMPlayerStats[playerid][dm_player_active] == false))
		{
    		player_Spawn(playerid);
			SetSpecialColorForPlayer(playerid);
			duel_OnPlayerSpawn(playerid);
			Cop_OnPlayerSpawn(playerid);
			Medic_OnPlayerSpawn(playerid);
			Housing_OnPlayerSpawn(playerid);
		}
		else
		{
		    DM_OnPlayerSpawn(playerid,dmid);
		}
		if (Player[playerid][JailTime] > 0) {JailPlayer(playerid,Player[playerid][JailTime],gettext(1460)); }
		Debug("gamemode.pwn > OnPlayerSpawn - Stop");
		return 1;
	}

	public OnPlayerRequestClass(playerid, classid)
	{
	    Debug("gamemode.pwn > OnPlayerRequestClass - Start");
		SpawnSel_OnPlayerRequestClass(playerid,classid);
		Debug("gamemode.pwn > OnPlayerRequestClass - Stop");
		return 1;
	}

	public OnPlayerCommandText(playerid,cmdtext[])
	{
	    Debug("gamemode.pwn > OnPlayerCommandText - Start");
		if (!IsPlayerConnected(playerid))
		{
            Debug("gamemode.pwn > OnPlayerCommandText - Stop");
			return 1;
		}
		
		AntiSpam_OnPlayerCommand(playerid,cmdtext);
		
		new cmdfound;
		cmdfound += CH_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += Bank_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += Club_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += Account_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += Drink_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += Mines_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += Business_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += Race_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += ModRace_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += Moderator_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += DM_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += ModDM_OnPlayerCommandText(playerid,cmdtext);
		//cmdfound += ModMG_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += Spec_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += ModHB_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += ModHay_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += ModTP_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += Housing_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += Gang_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += Fuel_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += Cop_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += Chat_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += Medic_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += Boxing_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += Phone_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += carlock_OnPlayerCommandText(playerid,cmdtext);
		//cmdfound += Missions_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += Stadium_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += duel_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += Derby_OnPlayerCommandText(playerid, cmdtext);
		cmdfound += School_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += Taxi_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += Bets_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += Votekick_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += Lotto_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += Bribe_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += SpawnPlace_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += WeaponsStat_OnPlayerCommandText(playerid,cmdtext);
		cmdfound += Rules_OnPlayerCommandText(playerid,cmdtext);

		if (cmdfound > 0)
		{
		    new cmd[20];
			new idx;
			set(cmd,strcharsplit(cmdtext, idx,strchar(" ")));

			if ((strcomp(cmd, "/login", true) == 1) || (strcomp(cmd, "/register", true) == 1))
			{
				new logstring[256];
				format(logstring, sizeof (logstring), "player: %d:  %s:    %s *********",playerid,oGetPlayerName(playerid),cmd);
				WriteCMDLog(logstring);
			} else {
			    new logstring[256];
				format(logstring, sizeof (logstring), "player: %d:  %s:    %s",playerid,oGetPlayerName(playerid),cmdtext);
				WriteCMDLog(logstring);
			}
			Debug("gamemode.pwn > OnPlayerCommandText - Stop");
			return 1;
		}

		Debug("gamemode.pwn > OnPlayerCommandText - Stop");
		return 0;

	}

	public OnPlayerText(playerid, text[])
	{
		Debug("gamemode.pwn > OnPlayerText - Start");
		if (!IsPlayerConnected(playerid))
		{
            Debug("gamemode.pwn > OnPlayerText - Stop");
			return 1;
		}
	   	if (Player[playerid][MuteTime] > 0)
		{
		    SystemMsg(playerid,COLOUR_ERROR,gettext(1314));
		    Debug("gamemode.pwn > OnPlayerText - Stop");
			return 0;
		}
		AntiSpam_OnPlayerText(playerid,text);
		AntiFlood_OnPlayerText(playerid,text);
		AntiFlame_OnPlayerText(playerid,text);
		if(CheckForCaps(text)){SystemMsg(playerid,COLOUR_ERROR,gettext(1789));Debug("gamemode.pwn > OnPlayerText - Stop"); return 0; }
		if(Moderator_OnPlayerText(playerid,text)){Debug("gamemode.pwn > OnPlayerText - Stop"); return 0; }
  		if(JobChat_OnPlayerText(playerid, text)){Debug("gamemode.pwn > OnPlayerText - Stop"); return 0; }
		if(Phone_OnPlayerText(playerid,text)){Debug("gamemode.pwn > OnPlayerText - Stop"); return 0; }
		if(Gang_OnPlayerText(playerid,text)){Debug("gamemode.pwn > OnPlayerText - Stop"); return 0; }
		if (Real_Chat == 1){
            for (new i=0;i<MAX_PLAYERS;i++) {
     			if (IsPlayerConnected(i)) {
     			    if (GetPlayerDistanceToPointEx(i,PlayerPos[playerid][Coord_X],PlayerPos[playerid][Coord_Y],PlayerPos[playerid][Coord_Z])<RADIUS_SAY){
     			        SendPlayerMessageToPlayer(i,playerid,text);
     			    }
     			}
			}
		} else {
	 		SendPlayerMessageToAll(playerid,text);
		}
		if (Show_Chat_Bubble) SetPlayerChatBubble(playerid, text, GetPlayerColor(playerid), RADIUS_SAY, 10000);
	    WriteChatLog(playerid,text,"");
	    Debug("gamemode.pwn > OnPlayerText - Stop");
		return 0;
	}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	Debug("gamemode.pwn > OnPlayerStateChange - Start");
	//Hotwire_OnPlayerStateChange(playerid,newstate,oldstate);
    Management_OnPlayerStateChange(playerid, newstate, oldstate);
    Cop_OnPlayerStateChange(playerid,newstate,oldstate);
	Medic_OnPlayerStateChange(playerid,newstate,oldstate);
	School_OnPlayerStateChange(playerid,newstate,oldstate);
	Taxi_OnPlayerStateChange(playerid, newstate, oldstate);
	AntiFall_OnPlayerStateChange(playerid,newstate,oldstate);
	AntiCJ_OnPlayerStateChange(playerid, newstate, oldstate);
	AntiTele_OnPlayerStateChange(playerid, newstate, oldstate);
	Derby_OnPlayerStateChange(playerid, newstate, oldstate);
	Race_OnPlayerStateChange(playerid,newstate,oldstate);
	Debug("gamemode.pwn > OnPlayerStateChange - Stop");
}

public PickupCheckTimer()
{
    Debug("gamemode.pwn > PickupCheckTimer - Start");
	for(new i=0;i<MAX_PLAYERS;i++){
			if(IsPlayerConnected(i)){
			    if (!IsPlayerInAnyDM(i) && !IsPlayerInAnyHay(i)){
			    	if (IsPlayerInAnyVehicle(i)){
						MineShopPickupCheck(i);
						PayNSprayPickupCheck(i);
			    	} else {
			    		CShopPickupCheck(i);
			    		DrinkPickupCheck(i);
			    		ArmDealerPickupCheck(i);
			    		HousePickupCheck(i);
			    		CanisterPickupCheck(i);
			    		DinnerPickupCheck(i);
			    		BankPickupCheck(i);
			    		ClubPickupCheck(i);
						CopJobPickupCheck(i);
						MedicJobPickupCheck(i);
						SkydivePlacePickupCheck(i);
						BoxingPickupCheck(i);
					}
				}

			}
	}
	Debug("gamemode.pwn > PickupCheckTimer - Stop");
}

public OnVehicleDeath(vehicleid, killerid){
	//Hotwire_OnVehicleDeath(vehicleid,killerid);
	Vehicles_OnVehicleDeath(vehicleid,killerid);
	Derby_OnVehicleDeath(vehicleid,killerid);
	Mines_OnVehicleDeath(vehicleid,killerid);
	CarHaveNOS[vehicleid] = false;
	return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid){
	//Hotwire_OnPlayerExitVehicle(playerid,vehicleid);
	Vehicles_OnPlayerExitVehicle(playerid,vehicleid);
	carlock_OnPlayerExitVehicle(playerid,vehicleid);
	//Missions_OnPlayerExitVehicle(playerid, vehicleid);
	Taxi_OnPlayerExitVehicle(playerid, vehicleid);
	Spec_OnPlayerExitVehicle(playerid, vehicleid);
	return 1;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger){
	Vehicles_OnPlayerEnterVehicle(playerid,vehicleid);
	//Missions_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger);
	AntiFall_OnPlayerEnterVehicle(playerid,vehicleid);
	AntiCJ_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger);
	Spec_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger);
	return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys){
	Club_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
	AntiFall_OnPlayerKeyStateChange(playerid,newkeys, oldkeys);
	Hay_OnKeyStateChange(playerid, newkeys, oldkeys);
	tram_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
	return 1;
}

public OnVehicleSpawn(vehicleid)
{
    Business_OnVehicleSpawn(vehicleid);
	Fuel_OnVehicleSpawn(vehicleid);
	carlock_OnVehicleSpawn(vehicleid);
	return 1;
}

public OnPlayerSelectedMenuRow(playerid, row)
{
    CH_OnPlayerSelectedMenuRow(playerid, row);
	return 1;
}

public OnPlayerExitedMenu(playerid)
{
	CH_OnPlayerExitedMenu(playerid);
	return 1;
}

public OnGameModeExit(){
    Debug("gamemode.pwn > OnGameModeExit - Start");
	//WorldSave();
	CH_OnGameModeExit();
	KillTimers();
	tempban_OnGameModeExit();
	db_OnGameModeExit();
	GameModeExit();
	Debug("gamemode.pwn > OnGameModeExit - Stop");
	return 1;
}

public OnPlayerLeaveCheckpoint(playerid){
	return 1;
}

public OnPlayerLeaveRaceCheckpoint(playerid){
	return 1;
}

public OnPlayerRequestSpawn(playerid){
	if (IsLoggedBeforeSpawn(playerid) == 1){ SystemMsg(playerid,COLOUR_INFO,gettext(513)); return 0;}
    SpawnSel_OnPlayerRequestSpawn(playerid);
	return 1;
}

public OnObjectMoved(objectid){
	return 1;
}

public OnPlayerObjectMoved(playerid, objectid){
	return 1;
}

public OnEnterExitModShop(playerid, enterexit, interiorid){
	return 1;
}

public OnPlayerPickUpPickup(playerid, pickupid){
    Moneydrop_OnPlayerPickUpPickup(playerid, pickupid);
	return 1;
}

public OnVehicleMod(playerid,vehicleid, componentid){
    Vehicles_OnVehicleMod(vehicleid,componentid);
	return 1;
}

public OnVehiclePaintjob(playerid, vehicleid, paintjobid){
    Vehicles_OnVehiclePaintjob(vehicleid,paintjobid);
	return 1;
}

public OnVehicleRespray(playerid, vehicleid, color1, color2){
    Vehicles_OnVehicleRespray(vehicleid,color1,color2);
	return 1;
}

public OnRconLoginAttempt(ip[], password[], success){
    rconbrute_OnRconLoginAttempt(ip,password,success);
	return 1;
}

public OnPlayerStreamIn(playerid, forplayerid){
	return 1;
}

public OnPlayerStreamOut(playerid, forplayerid){
	return 1;
}

public OnVehicleStreamIn(vehicleid, forplayerid){
    SetVehicleParamsForPlayer(vehicleid, forplayerid, VehicleParams[vehicleid][forplayerid][obj], VehicleParams[vehicleid][forplayerid][locked]);
	return 1;
}

public OnVehicleStreamOut(vehicleid, forplayerid){
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]){
    new len=strlen(inputtext);
	for(new i=0;i<len;i++) if(inputtext[i]=='%') inputtext[i]='#'; //Fix
	ModTP_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
	Boxing_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
	return 1;
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid){
    AntiTele_OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid);
    Spec_OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid);
    IntWeap_OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid);
	return 1;
}

stock KillTimers(){
	if (Anti_Bot_Enabled) KillTimer(TimerAntiBot);
	if (Anti_Idle_Enabled)	KillTimer(TimerAntiIdle);
	if (Anti_Jetpack_Enabled) KillTimer(AntiJetpackTimer);
	if (Anti_Money_Cheat_Enabled) KillTimer(TimerAntiMoney);
	if (PingKicker_Enabled) KillTimer(TimerAntiPing);
	if (Anti_SpeedCheat_Enabled) KillTimer(TimerAntiSpeed);
	if (Anti_Teleport_Enabled)	KillTimer(TimerAntiTele);
	KillTimer(TimerMakeItems);
	KillTimer(TimerMines);
	KillTimer(TimerSkyDive);
	KillTimer(TimerTaxi);
	KillTimer(TimerTextScroll);
	KillTimer(TimerBanCheck);
	KillTimer(TimerBoxing);
	KillTimer(TimerCheckDM);
	KillTimer(TimerCheckHay);
	KillTimer(TimerDuel);
	KillTimer(TimerDerbyBarrels);
	KillTimer(TimerDerbyForceEnd);
	KillTimer(TimerDerbyMoveCheck);
	KillTimer(TimerCashbox);
	if (Groundhold_Enabled) KillTimer(TimerGroundhold);
	if (Hotspots_Enabled)	KillTimer(TimerHotspot);
	KillTimer(TimerRent);
	if (Lottery_Enabled) KillTimer(TimerLotto);
	KillTimer(TimerMapIcons);
	if (Multikill_Enabled) KillTimer(TimerMultikill);
	KillTimer(TimerPayDay);
	KillTimer(TimerPhone);
	KillTimer(TimerPickupStream);
	KillTimer(TimerPlayer);
	KillTimer(TimerRace);
	if (Autorestart_Enabled) KillTimer(TimerRestart);
	KillTimer(TimerSyncTime);
    #if SAVE_VEHICLES_IN_DB
    KillTimer(TimerVehicles);
    #endif
    KillTimer(TimerFuel);
    KillTimer(TimerVoteKick);
    KillTimer(TimerWang);
    if (Weather_Change_Enabled) KillTimer(TimerWeather);
    KillTimer(TimerWorldSave);
	KillTimer(TimerSavePlayers);
	KillTimer(TimerSyncActiveCP);
	KillTimer(TimerPickupCheck);
	KillTimer(TimerObjectStream);
	KillTimer(TimerTempBan);
	if (Vehicle_HP_Bar_Enabled) KillTimer(TimerVHPBar);
	if (Packets_Enabled) KillTimer(TimerPackets);
	if (Horseshoes_Enabled) KillTimer(TimerHorseshoes);
	if (Oysters_Enabled) KillTimer(TimerOysters);
	WriteLog("All timers killed");
	return 0;
}

stock ResetOtherPlayerData(playerid){
	Debug("gamemode.pwn > ResetOtherPlayerData - Start");
	//businesses\clothes.inc
	cshopinfoshowed[playerid] = PlayerClothesTick[playerid] = 0;
	//businesses\drinks.inc
	dvendorinfoshowed[playerid] = PlayerDrinkTick[playerid] = 0;
	//businesses\food.inc
	dinnerinfoshowed[playerid] = 0;
	//businesses\fuel.inc
	canisterinfoshowed[playerid] = 0;
	//businesses\mines.inc
	mineshopinfoshowed[playerid] = 0;
	//businesses\paynspray.inc
	paynsprayinfoshowed[playerid] = 0;
	//businesses\skydive.inc
	skydiveplaceinfoshowed[playerid] = PlayerSkydive[playerid] = 0;
	//businesses\weapon.inc
	armdinfoshowed[playerid] = 0;
	//jobs\medic.inc
	medicjobinfoshowed[playerid] = PlayerWantMedic[playerid] = 0;
	//jobs\taxi.inc
	Taxi_Reset(playerid);
	//jobs\cop.inc
	copjobinfoshowed[playerid] = WasInCopCar[playerid] = 0;
	//moderation\commands.inc
	MarkedPlayer[playerid] = INVALID_PLAYER_ID;
	ModSpySMS[playerid] = false;
	//moderation\commands_race.inc
	PlayerRaceEdit[playerid]=INVALID_RACE_ID; // will hold id of race a player is currently creating
	//moderation\commands_spec.inc
	Spec[playerid][Spectating] = false;
	Spec[playerid][SpectateID] = INVALID_PLAYER_ID;
	//moderation\antifall.inc
	antifall[playerid] = cmdoff[playerid] = false;
	lastv[playerid] = INVALID_VEHICLE_ID;
	//protections\driveby.inc
	DriveBy[playerid] = 0;
	//protections\flame.inc
	PlayerFlameCount[playerid] = 0;
	//protections\flood.inc
	TimeLastMsg[playerid] = FloodCount[playerid] = 0;
	//protections\freeze.inc
	FreezePos[playerid][Coord_X] = FreezePos[playerid][Coord_Y] = FreezePos[playerid][Coord_Z] = 0.0;
	//protections\idle.inc
	playerIdle[playerid] = 0;
	playerIdlePos[playerid][Coord_X] = playerIdlePos[playerid][Coord_Y] = playerIdlePos[playerid][Coord_Z] = 0.0;
	//protections\pingkicker.inc
	HighPing[playerid] = 0;
	//protections\rconbrute.inc
	RconBruteAttempts[playerid] = 0;
	//protections\reconnect.inc
	set(PlayerIP[playerid],nullstr);
	//protections\spam.inc
	set(LastCommand[playerid],nullstr);
	set(LastText[playerid],nullstr);
	CommandSpamCount[playerid] = SpamCount[playerid] = 0;
	//protections\spawnkill.inc
	SpawnKill[playerid] = 0;
	//protections\speed.inc
	ResetPlayerSpeedCheatStats(playerid);
	//protections\carkill.inc
	CarKill[playerid] = 0;
	//mapicons.inc
	ResetMapIconVariables(playerid);
	//payday.inc
	PlayTimeForStats[playerid] = 0;
	PlayTimeForPayDay[playerid] = 0;
	//spawnsel.inc
	SpawnSelection[playerid] = 0;
	//stadium.inc
	InDirtbikeStadium[playerid] = false;
	//vehicles_extras.inc
	PlayerSpeed[playerid] = 0;
	SpeedoWithPetrol[playerid] = false;
	//bank.inc
	bankinfoshowed[playerid] = 0;
	//base.inc
	WasInAmbulance[playerid] = PlayerBoxing[playerid] = MedicHealTime[playerid] = 0;
	PlayerFreezed[playerid] = false;
	PlayerPos[playerid][Coord_X] = PlayerPos[playerid][Coord_Y] = PlayerPos[playerid][Coord_Z];
	//gang.inc
	GangInvite[playerid] = 0;
	//groundhold.inc
	for (new gh = 0; gh<GROUNDHOLD_SIZE; gh++) PlayerGroundHoldTime[gh][playerid] = PlayerHoldingGround[gh][playerid] = 0;
	//hotwire.inc
	//housing.inc
	houseinfoshowed[playerid] = 0;
	WhereHeEnter[playerid][Coord_X] = WhereHeEnter[playerid][Coord_Y] = WhereHeEnter[playerid][Coord_Z] = 0.0;
	PlayerHouseID[playerid]=-1;
	Debug("gamemode.pwn > ResetOtherPlayerData - Stop");
	return 0;
}
