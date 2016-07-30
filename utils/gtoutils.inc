//Useful functions
#include "privilegies"

forward Utils_OnGameModeInit();
forward SystemMsgScrolling(playerid,text[]);
forward TextScroller();
forward strcharsplit(string[], &index,seperator);
forward strchar(string[2]);
forward charstr(character);
forward CharReplace(findchar,newchar,string[MAX_STRING]);
forward Vary(source,varyamount);
forward strcomp(str1[],str2[],bool:ignorecase);
forward NameCharCheck(string[MAX_STRING]);
forward StringCharCheck(string[MAX_STRING]);
forward SystemMsgFormat(playerid, colour,const str[], define);
forward SystemMsgFormatStr(playerid, colour,const str[], define[]);
forward SystemMsgToRegistered(colour,message[]);
forward Report(playerid,reason[]);
forward ConvertSeconds(time);
forward PlayerCount();
forward SystemMsg(playerid,colour,msg[]);
forward BanCheck();
forward split(const strsrc[], strdest[][], delimiter);
forward oSetVehicleParamsForPlayer(vehicleid,playerid,objective,doorslocked);

new TimerTextScroll;
new TimerBanCheck;

enum VehParamInfo
{
	obj,
	locked
}

new VehicleParams[MAX_DEFAULT_VEHICLE_SPAWNS][MAX_PLAYERS][VehParamInfo];

#define isempty(%1) ((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))

// Encode player name, to avoid crashes.
EncodeName(nickname[])
{
	new tmp[MAX_STRING];
	set(tmp,nickname);
	new newchar = strchar("_");

	for (new i=0;i<MAX_STRING;i++)
	{
		if (tmp[i]==0) break; // null
	    if (((tmp[i]>=strchar("a")) && (tmp[i]<=strchar("z"))) || ((tmp[i]>=strchar("A")) && (tmp[i]<=strchar("Z"))) || ((tmp[i]>=strchar("0")) && (tmp[i]<=strchar("9"))))
	    {
			continue;
	    }
	    else
	    {
	        if (tmp[i] == strchar(".")) continue; // temp fix until new db handler is finished
	        tmp[i] = newchar;
		}
	}
	tmp=CharReplace(DBSeperator,newchar,tmp);
	tmp=CharReplace(strchar(" "),newchar,tmp);
	tmp=CharReplace(strchar("\n"),newchar,tmp);
	return tmp;
}


ovalstr(value)
{
	new temp[MAX_STRING];
	valstr(temp,value); // level
	return temp;
}

PlayerPlaySoundOnPlayer(playerid,soundid)
{
	PlayerPlaySound(playerid,soundid,PlayerPos[playerid][Coord_X],PlayerPos[playerid][Coord_Y],PlayerPos[playerid][Coord_Z]);
}
FormatPosition(position)
{
	new string[MAX_STRING];
	if (position == 1)
	{
	    format(string, sizeof(string), "%dst!",position);
	}
	else if (position == 2)
	{
		format(string, sizeof(string), "%dnd!",position);
	}
	else if (position == 3)
	{
		format(string, sizeof(string), "%drd!",position);
	}
	else
	{
		format(string, sizeof(string), "%dth.",position);
	}
	return string;
}


#define TEXT_SCROLL_SPEED 1000
//#define TEXT_SCROLL_SIZE 1023
#define TEXT_SCROLL_SIZE 8192
new ScrollingText[MAX_PLAYERS][TEXT_SCROLL_SIZE]; // hmmmmm
public SystemMsgScrolling(playerid,text[])
{
	for (new i = 0;i<strlen(text);i++)
	{
	    if (text[i] == 0) break;
	    if (text[i] == strchar("\n")) text[i] = strchar("_");
	}

	if (strlen(text) > 150) //  not allowed
	{
		SystemMsg(playerid,COLOUR_MISC,text);
  		return;
	}
	new wbreak;
	while ((strlen(ScrollingText[playerid]) > (TEXT_SCROLL_SIZE  - strlen(text))) && (wbreak < 5))
	{
		ScrollText(playerid);
		wbreak++;
	}
	if (wbreak >=5)
	{
		strdel(ScrollingText[playerid], 0, strlen(ScrollingText[playerid])-1);
	}
	strins(ScrollingText[playerid],text,strlen(ScrollingText[playerid]),TEXT_SCROLL_SIZE-1);
	strins(ScrollingText[playerid],"\n",strlen(ScrollingText[playerid]),TEXT_SCROLL_SIZE-1);
}

ScrollText(id)
{
	if (IsPlayerConnected(id))
	{
	    if (strlen(ScrollingText[id]) == 0) return;
		new string[MAX_STRING];
	    new idx;
	    set(string,strcharsplit(ScrollingText[id],idx,strchar("\n")));
	    if (strlen(string) < 1) return;
	    new colour;
	    if ((string[0] == strchar("~")) && (string[2] == strchar("~")))
	    {
	        if (string[1] == strchar("r")) colour = COLOUR_RED;
	        else if (string[1] == strchar("g")) colour = COLOUR_GREEN;
            else if (string[1] == strchar("b")) colour = COLOUR_BLUE;
            else if (string[1] == strchar("y")) colour = COLOUR_YELLOW;
            strdel(string, 0, 3);

        }
        if (colour != 0)
        {
        	SystemMsg(id,colour,string);
        }
        else
        {
	       	SystemMsg(id, COLOUR_MISC, string);
	  	}
		strdel(ScrollingText[id], 0, idx);
			
	}
	else
	{
		strdel(ScrollingText[id], 0, strlen(ScrollingText[id])-1);
	}
	return;
}

public TextScroller()
{
	for (new id = 0;id<MAX_PLAYERS_EX;id++)
	{
		if (IsPlayerConnected(id))
		{
		    ScrollText(id);
		}
	}
}

strcharsplit(string[], &index,seperator)
{
	new result[MAX_STRING];
	new idxpos = index;
	for (new i=index;i<strlen(string);i++)
	{
		index++;
		if (string[i] == seperator) break;
		if (string[i] == 0) break;
		result[i-idxpos] = string[i];
		
	}
	return result;
}

public strchar(string[2])
{
	return string[0];
}

charstr(character)
{
	new string[2];
	string[0] = character;
	return string;
}

CharReplace(findchar,newchar,string[MAX_STRING])
{
	for (new i = 0;i<MAX_STRING;i++)
	{
		if (string[i] == findchar)
		{
		    if (string[i] == 0) break;
		    string[i] = newchar;
		}
	}
	return string;
}

public Vary(source,varyamount)
{
	new rnd = random((varyamount*2)+1) - varyamount;
	source = source + rnd;
	return source;
}

// strcmp() returns are fucked, this is an override to fix returns
// will return 1/true if strings are equal
public strcomp(str1[],str2[],bool:ignorecase)
{
	if ((isempty(str1)) && (isempty(str2))) return 1;
	if (isempty(str1)) return 0;
	if (isempty(str2)) return 0;
	if (strcmp(str1,str2,ignorecase) == 0) return 1;
	return 0;
}

public NameCharCheck(string[MAX_STRING])
{
	for (new i=0;i<MAX_STRING;i++)
	{
	    if (string[i]==0) return 1; // null
		if (string[i] < 32) return 0;
		if (string[i] > 125) return 0;
	}
	return 1;
}

public StringCharCheck(string[MAX_STRING])
{
	for (new i=0;i<MAX_STRING;i++)
	{
	    if (string[i]==0) return 1; // null
		if ((string[i]<48) && (string[i]!=32)) return 0;
		if (string[i]>57 && string[i]<65) return 0;
		if (string[i]>90 && string[i]<97) return 0;
		if (string[i]>122) return 0;
	}
	return 1;
}

public SystemMsgToRegistered(colour, message[])
{
	for (new playerid=0;playerid<MAX_PLAYERS_EX;playerid++)
	{
        	if (IsPlayerRegistered(playerid))
        	{
	        	SystemMsg(playerid, colour, message);
		}
	}
}

public Report(playerid,reason[])
{
	if (!IsPlayerConnected(playerid)) return;
	new string[MAX_STRING];
	format(string,sizeof(string),gettext(1307),oGetPlayerName(playerid),reason);
        			
	for (new id=0;id<MAX_PLAYERS_EX;id++)
	{
    		if (IsPlayerConnected(id))
    		{
        		if (PlayerPriv[id][IsModerator] == 1)
        		{
				ModMsg(id,string);
			}
		}
	}
}


//ConvertSeconds by El Presy
//Converts an ammount of seconds into hours/minutes/seconds
ConvertSeconds(time)
{
	new string[40];
	//Less than 1 minute
	if(time < 60) format(string, sizeof(string), gettext(1592), time);
	//1 minute
	else if(time == 60) set(string,gettext(1593));
	//From 1 minute to 59 minutes
	else if(time > 60 && time < 3600){
		new Float: minutes;
		new seconds;
		minutes = time / 60;
		seconds = time % 60;
		format(string, sizeof(string), gettext(1594), minutes, seconds);
		//1 hour
	}else if(time == 3600) set(string,gettext(1595));
		//More than 1 hour
	else if(time > 3600){
		new Float: hours;
		new minutes_int;
		new Float: minutes;
		new seconds;
		hours = time / 3600;
		minutes_int = time % 3600;
		minutes = minutes_int / 60;
		seconds = minutes_int % 60;
		format(string, sizeof(string), gettext(1596), hours, minutes, seconds);
	}
	return string;
}

public PlayerCount()
{
	new count = 0;

	for (new playerid=0; playerid<MAX_PLAYERS_EX;playerid++)
	{
        if (IsPlayerConnected(playerid)) count++;
	}
	return count;
}

//Functions ripped from DracoBlue ChatSystem
stock SystemMsg(playerid,colour,msg[]) {
	if ((IsPlayerConnected(playerid))&&(strlen(msg)>0)) {
  		SendClientMessage(playerid,colour,msg);
	}
	return 1;
}

SystemMsgFormat(playerid, colour, const str[], define)
{
	new tmpbuf[256];
	format(tmpbuf, sizeof(tmpbuf), str, define);
	return SystemMsg(playerid,colour, tmpbuf);
}

SystemMsgFormatStr(playerid, colour, const str[], define[])
{
	new tmpbuf[256];
	format(tmpbuf, sizeof(tmpbuf), str, define);
	return SystemMsg(playerid, colour, tmpbuf);
}

ModMsgFormatStr(playerid, const str[], define[])
{
	new tmpbuf[256];
	format(tmpbuf, sizeof(tmpbuf), str, define);
	return ModMsg(playerid, tmpbuf);
}

stock ModMsg(playerid,msg[]) {
	return SystemMsg(playerid,COLOUR_MESSAGE_MOD,msg);
}

//From UF
stock GetPlayerDistanceToPointEx(playerid,Float:x,Float:y,Float:z) //By Sacky (Edited by Smugller)
{
	new Float:dis;
	dis = floatsqroot((x-PlayerPos[playerid][Coord_X])*(x-PlayerPos[playerid][Coord_X])+(y-PlayerPos[playerid][Coord_Y])*(y-PlayerPos[playerid][Coord_Y])+(z-PlayerPos[playerid][Coord_Z])*(z-PlayerPos[playerid][Coord_Z]));
	return floatround(dis);
}

stock GetPointDistanceToPointEx(Float:x,Float:y,Float:z,Float:x2,Float:y2,Float:z2) //By Sacky (Edited by Smugller)
{
	new Float:dis;
	dis = floatsqroot((x-x2)*(x-x2)+(y-y2)*(y-y2)+(z-z2)*(z-z2));
	return floatround(dis);
}

//Function from Make It Comfortable
stock strcut(string[],substring[]){
new fn = strfind(string,substring);
new ln = strlen(substring);
strdel(string,fn,fn+ln);
}

stock GetClosestPlayer(playerid)
{
	new x,Float:dis,Float:dis2,player;
	player = -1;
	dis = 99999.99;
	for (x=0;x<MAX_PLAYERS_EX;x++){
		if (x != playerid){
			dis2 = GetDistanceBetweenPlayers(x,playerid);
			if (dis2 < dis && dis2 != -1.00){
			dis = dis2;
			player = x;
			}
		}
	}
	return player;
}

stock GetDistanceBetweenPlayers(playerid, playerid2)
{
	return floatround(floatsqroot(floatpower(floatabs(floatsub(PlayerPos[playerid2][Coord_X], PlayerPos[playerid][Coord_X])), 2) + floatpower(floatabs(floatsub(PlayerPos[playerid2][Coord_Y], PlayerPos[playerid][Coord_Y])), 2) + floatpower(floatabs(floatsub(PlayerPos[playerid2][Coord_Z], PlayerPos[playerid][Coord_Z])), 2)));
}

stock ShowTextForPlayer(playerid,msg[],time,style) {
	if ((IsPlayerConnected(playerid))&&(strlen(msg)>0)){
		GameTextForPlayer(playerid,msg,time,style);
	}
}

public Utils_OnGameModeInit(){
	TimerTextScroll = SetTimer("TextScroller",TEXT_SCROLL_SPEED,1);
}

public split(const strsrc[], strdest[][], delimiter)
{
	new i, li;
	new aNum;
	new len;
	while(i <= strlen(strsrc)){
	    if(strsrc[i]==delimiter || i==strlen(strsrc)){
	        len = strmid(strdest[aNum], strsrc, li, i, 128);
	        strdest[aNum][len] = 0;
	        li = i+1;
	        aNum++;
		}
		i++;
	}
	return 1;
}

stock GetDistanceBetweenVehicles(vehicleid, vehicleid2) //Based on code By Slick (Edited by Smugller thx for Y_Less )
{
new Float:x1,Float:y1,Float:z1,Float:x2,Float:y2,Float:z2;
new Float:dis;
GetVehiclePos(vehicleid,x1,y1,z1);
GetVehiclePos(vehicleid2,x2,y2,z2);
dis = floatsqroot((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)+(z2-z1)*(z2-z1));
return floatround(dis);
}

IsKeyJustDown(key, newkeys, oldkeys)
{
	if((newkeys & key) && !(oldkeys & key)) return 1;
	return 0;
}

stock GetNowAsString()
{
	new hour,minute,second;
  	new year, month,day;
  	gettime(hour, minute, second);
  	getdate(year, month, day);
	new string[MAX_STRING];
	format(string,sizeof(string),"%d:%d:%d (%d.%d.%d)",hour,minute,second,day,month,year);
	return string;
}

stock GetNowTimeAsString()
{
	new hour,minute,second;
  	gettime(hour, minute, second);
	new string[MAX_STRING];
	format(string,sizeof(string),"%d:%d:%d",hour,minute,second);
	return string;
}

stock ini_GetKey(line[])
{
	new keyRes[256];
	keyRes[0] = 0;
    	if (strfind(line,"=",true)==-1) return keyRes;
    	strmid(keyRes,line,0,strfind(line,"=",true),sizeof(keyRes));
    	return keyRes;
}

stock ini_GetValue(line[])
{
	new valRes[256];
	valRes[0]=0;
	if (strfind(line,"=",true)==-1) return valRes;
	strmid(valRes,line,strfind(line,"=",true)+1,strlen(line),sizeof(valRes));
	return valRes;
}

CloseMenu(playerid,Menu:oldmenu){
	HideMenuForPlayer(oldmenu,playerid);
	if (PlayerFreezed[playerid] == false) TogglePlayerControllable(playerid,1);
	return 1;
}

FormatTimeForStat(time){
	new TimeFormated[3];
	new string[MAX_STRING];
	TimeFormated[0] = (time/3600);
	TimeFormated[1] = ((time%3600)/60);
	TimeFormated[2] = ((time%3600)%60);
	if(TimeFormated[1] < 10){
		if(TimeFormated[2] < 10){
	        	format(string,MAX_STRING,gettext(340),TimeFormated[0],TimeFormated[1],TimeFormated[2]);
		} else {
	        	format(string,MAX_STRING,gettext(341),TimeFormated[0],TimeFormated[1],TimeFormated[2]);
		}
	} else{
		if(TimeFormated[2] < 10){
        		format(string,MAX_STRING,gettext(344),TimeFormated[0],TimeFormated[1],TimeFormated[2]);
		} else {
        		format(string,MAX_STRING,gettext(346),TimeFormated[0],TimeFormated[1],TimeFormated[2]);
		}
	}
	return string;
}

FormatTimeForEvent(time){
	new TimeFormated[2];
	new string[MAX_STRING];
	TimeFormated[0] = ((time%3600)/60);
	TimeFormated[1] = ((time%3600)%60);
	if(TimeFormated[0] < 10){
		if(TimeFormated[1] < 10){
	        	format(string,MAX_STRING,"0%d:0%d",TimeFormated[0],TimeFormated[1]);
		} else {
	        	format(string,MAX_STRING,"0%d:%d",TimeFormated[0],TimeFormated[1]);
		}
	} else{
		if(TimeFormated[1] < 10){
        		format(string,MAX_STRING,"%d:0%d",TimeFormated[0],TimeFormated[1]);
		} else {
        		format(string,MAX_STRING,"%d:%d",TimeFormated[0],TimeFormated[1]);
		}
	}
	return string;
}

//example: weekday = GetWeekdayNum(day, month, year);
stock GetWeekdayNum(d,m,y) //by d.wine
{
	m-=2;
	if(m<=0)
		{
		y--;
		m+=12;
		}
	new cen = y/100;
	y=getrem(y,100);
	new w = d + ((13*m-1)/5) + y + (y/4) + (cen/4) - 2*cen;
	w=getrem(w,7);
	if (w==0) w=7;
	return w-1;
}

stock getrem(a,b) //get remnant of division
{
	new div = a/b;
	new left = a-b*div;
	return left;
}

//From GTAT SVN. Improved by MadCat
stock FindPlayerID(partofname[]) {
	if (isNumeric(partofname)) return strval(partofname);
	if (isempty(partofname)) return -1;
	new correctsigns_userid=-1; 
	new hasmultiple=-1;
	new playername[MAX_STRING];
	for (new i=0;i<MAX_PLAYERS_EX;i++) {
		if (IsPlayerConnected(i)) {
			set(playername,oGetPlayerName(i));
			if (strcmp(playername,partofname,true)==0) return i;
			if (strfind(playername,partofname,true) != -1){
				hasmultiple++;
				correctsigns_userid=i;
				if (hasmultiple>0) return -2; 
			}
		}
	}
  	return correctsigns_userid;
}

FormatRatio(val1,val2){
	new temp[20];
	if (val2 == 0){
		format(temp,20,gettext(1652),0);
	} else {
		format(temp,20,gettext(1653),floatdiv(val1,val2));
	}
	return temp;
}

stock GetXYInBackOfPlayer(const playerid, &Float:x, &Float:y, const Float:distance)
{    	// Created by Y_Less, modified by ElykS :-D

    	new Float:a;

    	GetPlayerPos(playerid, x, y, a);
    	GetPlayerFacingAngle(playerid, a);

	if (IsPlayerInAnyVehicle(playerid)) {
		new vid=GetPlayerVehicleID(playerid);
		if (vid>0 && vid<=MAX_VEHICLES) GetVehicleZAngle(vid, a);
    	}

    	x -= (distance * floatsin(-a, degrees));
    	y -= (distance * floatcos(-a, degrees));
}

stock ReturnPlayerIp(playerid)
{
	new ip[IP_LEN];
	GetPlayerIp(playerid,ip,sizeof(ip));
	return ip;
}

public oSetVehicleParamsForPlayer(vehicleid,playerid,objective,doorslocked)
{
	VehicleParams[vehicleid][playerid][obj] = objective;
	VehicleParams[vehicleid][playerid][locked] = doorslocked;
	if(IsVehicleStreamedIn(vehicleid, playerid)) return SetVehicleParamsForPlayer(vehicleid, playerid, objective, doorslocked);
	return 1;
}

Float:GetPlayerSpeed(playerid) // Bzuco (simple maths) in KM/H
{
	new Float:Vx,Float:Vy,Float:Vz,Float:rtn;
	if(IsPlayerInAnyVehicle(playerid))
	{
		GetVehicleVelocity(GetPlayerVehicleID(playerid),Vx,Vy,Vz);
		rtn = floatsqroot(floatpower(floatabs(Vx),2)+floatpower(floatabs(Vy),2)+floatpower(floatabs(Vz),2));
	}
	else
	{
		GetPlayerVelocity(playerid,Vx,Vy,Vz);
		rtn = floatsqroot(floatpower(floatabs(Vx),2)+floatpower(floatabs(Vy),2)+floatpower(floatabs(Vz),2));
	}
	return floatmul(floatmul(rtn, 100), 1.61);
}