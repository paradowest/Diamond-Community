#include <ylib\YSI_Storage\y_ini>
#include <ylib\YSI_Coding\y_hooks>
#include <ylib\YSI_Visual\y_commands>
#include <ylib\YSI_Coding\y_va>

GetEngineStatus(vehicleid)
{
    static
        engine,
        lights,
        alarm,
        doors,
        bonnet,
        boot,
        objective;

    GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);

    if(engine != 1)
        return 0;

    return Y_HOOKS_CONTINUE_RETURN_1;
}

SetEngineStatus(vehicleid, status)
{
    static
        engine,
        lights,
        alarm,
        doors,
        bonnet,
        boot,
        objective;

    GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
    return SetVehicleParamsEx(vehicleid, status, lights, alarm, doors, bonnet, boot, objective);
}

YCMD:engine(playerid, const string: params[], help)
{
    new vehicleid = GetPlayerVehicleID(playerid);
    
    if(IsPlayerInAnyVehicle(vehicleid)) 
        return SendClientMessage(playerid, col_red, "(#): "c_white"Niste u vozilu");
    
    if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) 
        return SendClientMessage(playerid, col_red, "(#): "c_white"Samo vozac moze koristiti ovu komandu ");

    switch(GetEngineStatus(vehicleid))
    {
        case false:
        {
            SetEngineStatus(vehicleid, true);
            va_SendClientMessage(playerid, col_diamond, "(!): "c_white"%s pali motor vozila", ReturnPlayerName(playerid));
        }
        case true:
        {
            SetEngineStatus(vehicleid, false);
            va_SendClientMessage(playerid, col_diamond, "(!): "c_white"%s gasi motor vozila", ReturnPlayerName(playerid));
        }
    }
    return 1;
}
