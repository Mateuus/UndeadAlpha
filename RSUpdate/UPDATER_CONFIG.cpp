#include "r3dPCH.h"
#include "r3d.h"

bool	UPDATER_UPDATER_ENABLED  = 1;
char	UPDATER_VERSION[512]     = "0.9.3";
char	UPDATER_VERSION_SUFFIX[512] = "";
char	UPDATER_BUILD[512]	 = __DATE__ " " __TIME__;

char	BASE_RESOURSE_NAME[512]  = "WZ";
char	GAME_EXE_NAME[512]       = "WarZ.exe";
char	GAME_TITLE[512]          = "The War Z";

// updater (xml and exe) and game info on our server.
char	UPDATE_DATA_URL[512]     = "https://api1.thewarinc.com/wz/wz.xml";	// url for data update
char	UPDATE_UPDATER_URL[512]  = "https://api1.thewarinc.com/wz/updater/woupd.xml";

// HIGHWIND CDN
char	UPDATE_UPDATER_HOST[512] = "http://arktos-icdn.pandonetworks.com/wz/updater/";

// LOCAL TESTING
//http://arktos.pandonetworks.com/Arktos
//char	UPDATE_DATA_URL[512]     = "http://localhost/wo/wo.xml";	// url for data update
//char	UPDATE_UPDATER_HOST[512] = "http://localhost/wo/updater/";	// url for updater .xml

char	EULA_URL[512]            = "http://arktos-icdn.pandonetworks.com/EULA.rtf";
char	TOS_URL[512]             = "http://arktos-icdn.pandonetworks.com/TOS.rtf";
char	GETSERVERINFO_URL[512]   = "http://www.thewarz.com/api_getserverinfo.xml";



bool	UPDATER_STEAM_ENABLED	 = false;
