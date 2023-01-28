/*
-> Staff 1 - Helper
-> Staff 2 - Junior Staff
-> Staff 3 - Senior Staff
-> Staff 4 - Head Staff
-> Staff 5 - Diamond Founder
*/
#include <ylib\YSI_Storage\y_ini>
#include <ylib\YSI_Coding\y_hooks>
#include <ylib\YSI_Visual\y_commands>
#include <easy-dialog>

static stock const USER_PATH[64] = "/Users/%s.ini";

static
        player_Staff[MAX_PLAYERS],
        player_Skin[MAX_PLAYERS];

new bool:StaffDuty[MAX_PLAYERS];
new staff_vehicle[MAX_PLAYERS] = { INVALID_VEHICLE_ID, ... };
new Text3D:staff_label[MAX_PLAYERS];

hook Account_Load(const playerid, const string: name[], const string: value[]);
hook Account_Load(const playerid, const string: name[], const string: value[])
{
    INI_Int("Staff", player_Staff[playerid]);
    INI_Int("Skin", player_Skin[playerid]);
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerConnect(playerid)
{
	staff_vehicle[playerid] = INVALID_VEHICLE_ID;
	return Y_HOOKS_CONTINUE_RETURN_1;
}
hook OnPlayerDisconnect(playerid)
{
    new INI:File = INI_Open(Account_Path(playerid));
    INI_SetTag(File, "data");
    INI_WriteInt(File, "Staff", player_Staff[playerid]);
    INI_WriteInt(File, "Skin", player_Skin[playerid]);
    INI_Close(File);

	DestroyVehicle(staff_vehicle[playerid]);
	staff_vehicle[playerid] = INVALID_PLAYER_ID;

    return Y_HOOKS_CONTINUE_RETURN_1;
}

stock Staff_Rank(s_rank)
{
	static 
        Staff_String[45];
        
	switch(s_rank)
	{
		case 1: Staff_String = "HELPER";
		case 2: Staff_String = "JUNIOR STAFF";
		case 3: Staff_String = "SENIOR STAFF";
		case 4: Staff_String = "HEAD STAFF";
		case 5: Staff_String = "DIAMOND FOUNDER";
		default: Staff_String = "";
	}
	return Staff_String;
}
YCMD:sduty(playerid, const string: params[], help)
{
	if(player_Staff[playerid] < 5) 
        return SendClientMessage(playerid, col_red, "(#): "c_white"Niste ovlasceni za koriscenje te komande!");
	
	static fm_string[256];
	if(StaffDuty[playerid] == false )
 	{
		SetPlayerHealth(playerid, 100);
		SetPlayerArmour(playerid, 99);
		
		format(fm_string, sizeof(fm_string), ""c_orange"Staff %s je sada na duznosti /askq", ReturnPlayerName(playerid));
	  	SendClientMessageToAll(-1, fm_string);	
	}
	else if(StaffDuty[playerid] == true)
	{
	 	StaffDuty[playerid] = false;
		format(fm_string, sizeof(fm_string), ""c_orange"Staff %s vise nije na duznosti", ReturnPlayerName(playerid));
	 	SendClientMessageToAll(-1, fm_string);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}
YCMD:makestaff(playerid, const string: params[], help)
{
    if(player_Staff[playerid] < 5) 
        return SendClientMessage(playerid, col_red, "(#): "c_white"Niste ovlasceni za koriscenje te komande!");
	
    static 
        level, 
        targetid;

	if(sscanf(params, "ud", targetid, level)) 
        return SendClientMessage(playerid, col_blue, "(?): "c_white"/makestaff [targetid] [level]");

	if(!IsPlayerConnected(targetid)) 
        return SendClientMessage(playerid, col_red, "(#): "c_white"Pogresili ste ID igraca");

    if(level < 0 || level > 5 ) 
        return SendClientMessage(playerid, col_red, "(#): "c_white"Maksimalan staff level je 5");

    if (!level && !player_Staff[targetid])
        return SendClientMessage(playerid, col_red, "(#): "c_white"Taj igrac nije u staff timu");

    if(level == player_Staff[targetid])
        return SendClientMessage(playerid, col_red, "(#): "c_white"Taj igrac je vec u staff timu");

	player_Staff[targetid] = level;
    
	if(!level)
	{           
        static fm_string[128];
		format(fm_string, sizeof(fm_string), ""c_diamond"(!): "c_white"Skinuta vam je pozicija staff-a od strane %s", ReturnPlayerName(playerid));
        SendClientMessage(targetid, -1, fm_string);

		format(fm_string, sizeof(fm_string), ""c_diamond"(!): "c_white"Skinuli ste poziciju staff-a igracu %s", ReturnPlayerName(targetid));
        SendClientMessage(playerid, -1, fm_string);

        SetPlayerColor(targetid,  0xFFFFFFFF);
	}
	if(level >= 1 && level <= 5 )
  	{
        new fm_string[128];  
		format(fm_string, sizeof(fm_string), ""c_diamond"(!): "c_white"Postavili ste poziciju staffa igracu %s", ReturnPlayerName(targetid));
        SendClientMessage(playerid, -1, fm_string);

		format(fm_string, sizeof(fm_string), ""c_diamond"(!): "c_white"Postavljena vam je pozicija staffa od strane %s", ReturnPlayerName(playerid));
        SendClientMessage(targetid, -1, fm_string);
	}
    new INI:File = INI_Open(Account_Path(playerid));
	INI_SetTag(File, "data" );
    INI_WriteInt(File, "Staff", player_Staff[playerid]);
	INI_Close(File);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

YCMD:jetpack(playerid, const string: params[], help)
{
	if (player_Staff[playerid] < 1)
		return SendClientMessage(playerid, col_red, "(#): "c_white"Niste ovlasceni za koriscenje te komande!");

    static 
        Float:Position_X,
        Float:Position_Y,
        Float:Position_Z;

	if( GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_USEJETPACK ) 
	{
		GetPlayerPos(playerid, Position_X, Position_Y, Position_Z);
		SetPlayerPos(playerid, Position_X, Position_Y, Position_Z );
		SendClientMessage(playerid, col_diamond, "(!): "c_white"Skinuo si jatpack");
	}
	else 
	{
	    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USEJETPACK);
	    SendClientMessage(playerid, col_diamond, "(!): "c_white"Uzeo si jatpack");
   	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

YCMD:sc(playerid, const string: params[], help)
{
	if(player_Staff[playerid] < 1)
		return SendClientMessage(playerid, col_red, "(#): "c_white"Niste ovlasceni za koriscenje te komande!");

	if(isnull(params))
		return SendClientMessage(playerid, col_blue, "(?): "c_white"/sc [text]");

	static tm_str[256];

	format(tm_str, sizeof(tm_str), "[SC] [%s] [%s]: %s", Staff_Rank(player_Staff[playerid]), ReturnPlayerName(playerid), params);

	foreach (new i: Player)
		if (player_Staff[i])
			SendClientMessage(i, col_diamond, tm_str);
	
    return Y_HOOKS_CONTINUE_RETURN_1;
}

CMD:fv(playerid, const string: params[])
{
    if(player_Staff[playerid] < 1)
		return SendClientMessage(playerid, col_red, "(#): "c_white"Niste ovlasceni za koriscenje te komande!");

	if(!IsPlayerInAnyVehicle(playerid)) 
        return SendClientMessage(playerid, col_red, "(#): "c_white"Moras biti u vozilu");

	RepairVehicle(GetPlayerVehicleID(playerid));

	SendClientMessage(playerid, col_blue, "(?): "c_white"Popravili ste svoje vozilo");

	return Y_HOOKS_CONTINUE_RETURN_1;
}

YCMD:setskin(playerid, const string: params[], help)
{

	if(player_Staff[playerid] < 1)
		return SendClientMessage(playerid, col_red, "(#): "c_white"Niste ovlasceni za koriscenje te komande!");

	static
		targetid,
		skinid;

	if (sscanf(params, "ri", targetid, skinid))
		return SendClientMessage(playerid, col_blue, "(?): "c_white"/setskin [targetid] [skinid]");

	SetPlayerSkin(targetid, skinid);

	player_Skin[targetid] = skinid;

    new INI:File = INI_Open(Account_Path(playerid));
	INI_SetTag( File, "data" );
    INI_WriteInt(File, "Skin", GetPlayerSkin(playerid));
	INI_Close( File );

    return 1;
}

YCMD:so(playerid, const string: params[], help)
{
	if(player_Staff[playerid] < 1)
		return SendClientMessage(playerid, col_red, "(#): "c_white"Niste ovlasceni za koriscenje te komande!");

	if(isnull(params))
		return SendClientMessage(playerid, col_blue, "(?): "c_white"/so [text]");

	static fm_string[256];

	format(fm_string, sizeof(fm_string), "[OBAVJESTENJE]: %s", params);
	SendClientMessageToAll(col_orange, fm_string);
	
    return Y_HOOKS_CONTINUE_RETURN_1;
}

YCMD:veh(playerid, params[], help)
{
	if(!player_Staff[playerid])
		return SendClientMessage(playerid, col_red, "(#): "c_white"Niste ovlasceni za koriscenje te komande!");

	static 
		Float:Position_X, 
		Float:Position_Y, 
		Float:Position_Z;

	GetPlayerPos(playerid, Position_X, Position_Y, Position_Z);
	if(staff_vehicle[playerid] == INVALID_VEHICLE_ID) 
	{
		if(isnull(params))
			return SendClientMessage(playerid, col_blue, "(?): "c_white"/veh [vehicleid]");

		new modelid = strval(params);

		if(400 > modelid > 611)
			return SendClientMessage(playerid, col_red, "(#): "c_white"ID vozila ne moze biti manji od 400 i veci od 600");
		
		new vehicleid = staff_vehicle[playerid] = CreateVehicle(modelid, Position_X, Position_Y, Position_Z, 0.0, 1, 0, -1);
		SetEngineStatus(vehicleid, true);
		staff_label[playerid] = Create3DTextLabel(""c_diamond"STAFF VEHICLE", 0xFFFFFFAA, 0, 0, 0, 25.0, 0, 0);
  		Attach3DTextLabelToVehicle(staff_label[playerid], staff_vehicle[playerid], 0.0, 0.0, 0.0);
		PutPlayerInVehicle(playerid, vehicleid, 0);
	}
	else 
	{
		DestroyVehicle(staff_vehicle[playerid]);
		Delete3DTextLabel(staff_label[playerid]);
		staff_vehicle[playerid] = INVALID_PLAYER_ID;
	}
	
    return Y_HOOKS_CONTINUE_RETURN_1;
}