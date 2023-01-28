/*
-> Thanks:
	* Southclaws : https://github.com/pawn-lang/compiler
	* Y_Less : https://github.com/pawn-lang/YSI-Includes/tree/v5.1
	* Maddinat0r : https://github.com/maddinat0r/sscanf/releases
	* Awesomdude : https://github.com/Awsomedude/easyDialog
	* samp-incognito : https://github.com/samp-incognito/samp-streamer-plugin/releases
	* NexiusTailer : https://github.com/NexiusTailer/MapFix.git
	* Y_Lees : https://github.com/Y-Less/samp-distance.git
*/ 
#define YSI_YES_HEAP_MALLOC
#define CGEN_MEMORY 60000

//-includes
#include <a_samp>
#include <ylib\YSI_Storage\y_ini>
#include <ylib\YSI_Coding\y_timers>
#include <ylib\YSI_Visual\y_commands>
#include <ylib\YSI_Coding\y_hooks>
#include <ylib\YSI_Data\y_foreach>
#include <ylib\YSI_Coding\y_va>
#include <sscanf\sscanf2>
#include <easy-dialog>
#include <crashdetect>
#include <streamer>
#include <mapfix>
#include <distance>
//-external
#include "external/server_side/server_list.pwn"
#include "external/staff_side/staff_list.pwn"
#include "external/vehicle_side/vehicle_list.pwn"
//-script
main ()
{
	print("-__________________________-");
	print("     Diamond Community	   ");
	print("        by .paradox         ");
	print("          v0.0.1            ");
	print("-__________________________-");
}
public OnGameModeInit()
{
	ManualVehicleEngineAndLights();
    EnableVehicleFriendlyFire();
    DisableInteriorEnterExits();
	SetNameTagDrawDistance(20.0);
	LimitGlobalChatRadius(20.0);
	return 1;
}