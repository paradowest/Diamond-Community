#include <ylib\YSI_Coding\y_hooks>
#include <ylib\YSI_Visual\y_commands>

hook OnPlayerText(playerid, text[])
{
    static fm_string[128];

    format(fm_string, sizeof(fm_string), ""c_greey"%s kaze: "c_white"%s", ReturnPlayerName(playerid), text);
    ProxDetector(playerid, Float:20.0, col_white, fm_string);

    return Y_HOOKS_CONTINUE_RETURN_1;
}

YCMD:b(playerid, const string: params[], help)
{   
    static 
        fm_string[128],
        text[124];
    
    if(sscanf(params, "s[100]", text)) 
        return SendClientMessage(playerid, col_red, "(#): "c_white"/b [text]");
        
    format(fm_string, sizeof(fm_string), ""c_diamond"((OOC | %s: "c_white"%s "c_diamond"))", ReturnPlayerName(playerid), text);
    ProxDetector(playerid, Float:15.0, col_white, fm_string);

    return Y_HOOKS_CONTINUE_RETURN_1;
}

