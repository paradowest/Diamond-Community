#include <ylib\YSI_Storage\y_ini>
#include <ylib\YSI_Coding\y_hooks>
#include <easy-dialog>

const PASSWORD_LENGTH_MAX = 64;
const PASSWORD_LENGTH_MIN = 6;
const LOGIN_ATTEMPTS = 	3;

static stock const USER_PATH[64] = "/Users/%s.ini";

enum
{
	TYPE_REGISTER = 1,
    TYPE_LOGIN
};

static  
    player_Password[MAX_PLAYERS][PASSWORD_LENGTH_MAX],
    player_Gender[MAX_PLAYERS][2],
	player_Money[MAX_PLAYERS],
    player_Score[MAX_PLAYERS],
	player_Skin[MAX_PLAYERS],
    player_Ages[MAX_PLAYERS];

static
    server_loginAttempts[MAX_PLAYERS];

hook Account_Load(const playerid, const string: name[], const string: value[]);
hook Account_Load(const playerid, const string: name[], const string: value[])
{
	INI_String("Password", player_Password[playerid]);
	INI_String("Gender", player_Gender[playerid]);
	INI_Int("Money", player_Money[playerid]);
	INI_Int("Score", player_Score[playerid]);
	INI_Int("Skin", player_Skin[playerid]);

	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerConnect(playerid)
{
    if (fexist(Account_Path(playerid)))
	{
		INI_ParseFile(Account_Path(playerid), "Account_Load", true, true, playerid);
		Dialog_Show(playerid, "dialog_login", DIALOG_STYLE_PASSWORD,
			"Login",
			""c_white"Vas nalog je pronadjen u "c_green"databazi\n\n"c_white"Upisite vasu lozinku kako bi ste se ulogovali",
			"Potvrdi", "Izlaz");

		return Y_HOOKS_CONTINUE_RETURN_1;
	}
	Dialog_Show(playerid, "dialog_register", DIALOG_STYLE_PASSWORD,
			"Register",
			""c_white"Vas nalog nije pronadjen u "c_red"databazi\n\n"c_white"Upisite vasu rezeljenu lozinku kako bi ste nastavili dalje",
			"Potvrdi", "Izlaz");

    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerDisconnect(playerid, reason)
{
	new INI:File = INI_Open(Account_Path(playerid));
    INI_SetTag(File,"data");
	INI_WriteInt(File, "Money", GetPlayerMoney(playerid));
    INI_WriteInt(File, "Score",GetPlayerScore(playerid));
    INI_WriteInt(File, "Skin",GetPlayerSkin(playerid));
    INI_Close(File);

    return Y_HOOKS_CONTINUE_RETURN_1;
}

timer SetPlayerSpawn[100](playerid, type)
{
	if(type == TYPE_REGISTER)
	{
		SetSpawnInfo(playerid, 0, player_Skin[playerid], Float: 331.6165, Float: -1338.2167, Float: 14.5078, Float: 204.6341, 0, 0, 0, 0, 0, 0);
		SpawnPlayer(playerid);

		SetPlayerScore(playerid, player_Score[playerid]);
		GivePlayerMoney(playerid, player_Money[playerid]);
		SetPlayerSkin(playerid, player_Skin[playerid]);
	}
	else if(type == TYPE_LOGIN)
	{
		SetSpawnInfo(playerid, 0, player_Skin[playerid], Float: 331.6165, Float: -1338.2167, Float: 14.5078, Float: 204.6341, 0, 0, 0, 0, 0, 0);
		SpawnPlayer(playerid);
			
		SetPlayerScore(playerid, player_Score[playerid]);
		GivePlayerMoney(playerid, player_Money[playerid]);
		SetPlayerSkin(playerid, player_Skin[playerid]);
	}

}

stock Account_Path(const playerid)
{
	new tmp_fmt[64];
	format(tmp_fmt, sizeof(tmp_fmt), USER_PATH, ReturnPlayerName(playerid));
	return tmp_fmt;
}

Dialog: dialog_register(playerid, response, listitem, string: inputtext[])
{
	if (!response)
		return Kick(playerid);

	if (!(PASSWORD_LENGTH_MIN <= strlen(inputtext) <= PASSWORD_LENGTH_MAX))
		return Dialog_Show(playerid, "dialog_register", DIALOG_STYLE_PASSWORD,
			"Register",
			""c_white"Vas nalog nije pronadjen u "c_red"databazi\n\n"c_white"Upisite vasu rezeljenu lozinku kako bi ste nastavili dalje",
			"Potvrdi", "Izlaz");

	strcmp(player_Password[playerid], inputtext);

	Dialog_Show(playerid, "dialog_ages", DIALOG_STYLE_INPUT,
			"Godine",
			"Upisite koliko imate godina od 12 do 60:",
			"Potvrdi", "Izlaz");

	return Y_HOOKS_CONTINUE_RETURN_1;
}

Dialog: dialog_ages(const playerid, response, listitem, string: inputtext[])
{
	if (!response)
		return Kick(playerid);

	if (!(12 <= strval(inputtext) <= 60))
		return Dialog_Show(playerid, "dialog_ages", DIALOG_STYLE_INPUT,
			"Godine",
			"Upisite koliko imate godina od 12 do 60:",
			"Potvrdi", "Izlaz");

	player_Ages[playerid] = strval(inputtext);

	Dialog_Show(playerid, "dialog_gender", DIALOG_STYLE_LIST,
			"Spol",
			"Musko\nZensko",
			"Potvrdi", "Izlaz");

	return Y_HOOKS_CONTINUE_RETURN_1;
}

Dialog: dialog_gender(const playerid, response, listitem, string: inputtext[])
{
	if (!response)
		return Kick(playerid);

	new tmp_int = listitem + 1;

	new INI:File = INI_Open(Account_Path(playerid));
	INI_SetTag(File,"data");
	INI_WriteString(File, "Password", player_Password[playerid]);
	INI_WriteString(File, "Gender", (tmp_int == 1 ? ("Musko") : ("Zensko")));
	INI_WriteInt(File, "Ages", player_Ages[playerid]);
	INI_WriteInt(File, "Score", 1);
	INI_WriteInt(File, "Skin", 240);
	INI_WriteInt(File, "Money", 1000);
	INI_Close(File);

	player_Money[playerid] = 1000;
	player_Skin[playerid] = 240;
	player_Score[playerid] = 1;

	defer SetPlayerSpawn(playerid, 1);
	
	return Y_HOOKS_CONTINUE_RETURN_1;
}

Dialog: dialog_login(const playerid, response, listitem, string: inputtext[])
{
	if (!response)
		return Kick(playerid);

	if (!strcmp(player_Password[playerid], inputtext, false))
		defer SetPlayerSpawn(playerid, 2);
	else
	{
		if (server_loginAttempts[playerid] == LOGIN_ATTEMPTS)
			return Kick(playerid);

		++server_loginAttempts[playerid];
		Dialog_Show(playerid, "dialog_login", DIALOG_STYLE_PASSWORD,
			"Login",
			""c_white"Vas nalog je pronadjen u "c_green"databazi\n\n"c_white"Upisite vasu lozinku kako bi ste se ulogovali",
			"Potvrdi", "Izlaz");
	}

	return Y_HOOKS_CONTINUE_RETURN_1;
}