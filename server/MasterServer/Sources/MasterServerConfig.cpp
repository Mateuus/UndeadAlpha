#include "r3dPCH.h"
#include "r3d.h"

#include "MasterServerConfig.h"

	CMasterServerConfig* gServerConfig = NULL;

static const char* configFile = "MasterServer.cfg";

CMasterServerConfig::CMasterServerConfig()
{
  const char* group      = "MasterServer";

  if(_access(configFile, 0) != 0) {
    r3dError("can't open config file %s\n", configFile);
  }

  masterPort_  = r3dReadCFG_I(configFile, group, "masterPort", SBNET_MASTER_PORT);
  clientPort_  = r3dReadCFG_I(configFile, group, "clientPort", GBNET_CLIENT_PORT);
  masterCCU_   = r3dReadCFG_I(configFile, group, "masterCCU",  3000);

  supervisorCoolDownSeconds_ = r3dReadCFG_F(configFile, group, "supervisorCoolDownSeconds",  15.0f);

  #define CHECK_I(xx) if(xx == 0)  r3dError("missing %s value in %s", #xx, configFile);
  #define CHECK_S(xx) if(xx == "") r3dError("missing %s value in %s", #xx, configFile);
  CHECK_I(masterPort_);
  CHECK_I(clientPort_);
  #undef CHECK_I
  #undef CHECK_S

  serverId_    = r3dReadCFG_I(configFile, group, "serverId", 0);
  if(serverId_ == 0)
  {
	MessageBox(NULL, "you must define serverId in MasterServer.cfg", "", MB_OK);
	r3dError("no serverId");
  }
  if(serverId_ > 255 || serverId_ < 1)
  {
	MessageBox(NULL, "bad serverId", "", MB_OK);
	r3dError("bad serverId");
  }
  
  LoadConfig();
  
  return;
}

void CMasterServerConfig::LoadConfig()
{
  r3dCloseCFG_Cur();
  
  numPermGames_ = 0;

  LoadPermGamesConfig();
  Temp_Load_WarZGames();
}

void CMasterServerConfig::Temp_Load_WarZGames()
{
  char group[128];
  sprintf(group, "WarZGames");

  int numGames   = r3dReadCFG_I(configFile, group, "numGames", 0);
  int maxPlayers = r3dReadCFG_I(configFile, group, "maxPlayers", 32);
  
  r3dOutToLog("WarZ %d games, %d players each\n", numGames, maxPlayers);
  
  for(int i=0; i<numGames; i++)
  {
    GBGameInfo ginfo;
    ginfo.mapId      = GBGameInfo::MAPID_WZ_Colorado;
    ginfo.maxPlayers = maxPlayers;

    sprintf(ginfo.name, "US Server %03d", i + 1);
    AddPermanentGame(10000 + i, ginfo, GBNET_REGION_US_West);

    sprintf(ginfo.name, "EU Server %03d", i + 1);
    AddPermanentGame(20000 + i, ginfo, GBNET_REGION_Europe);
  }
}

void CMasterServerConfig::LoadPermGamesConfig()
{
  numPermGames_ = 0;

//#ifdef _DEBUG
//  r3dOutToLog("Permanet games disabled in DEBUG");
//  return;
//#endif
  
  for(int i=0; i<250; i++)
  {
    char group[128];
    sprintf(group, "PermGame%d", i+1);

    char map[512] = "";
    char data[512] = "";
    char name[512];
	char PasswordGame[512] ="";
	char MapSettings[512] ="";

    r3dscpy(map,  r3dReadCFG_S(configFile, group, "map", ""));
    r3dscpy(data, r3dReadCFG_S(configFile, group, "data", ""));
    r3dscpy(name, r3dReadCFG_S(configFile, group, "name", ""));
	r3dscpy(PasswordGame, r3dReadCFG_S(configFile, group, "PasswordGame", ""));
	r3dscpy(MapSettings, r3dReadCFG_S(configFile, group, "MapSettings", ""));

	/*
	if (strcmp(MapSettings,"6") == 0)
	{
		if (strcmp(PasswordGame,"") == 0)
				strcpy(MapSettings,"0");
	}
	else if (strcmp(MapSettings,"6") != 0)
	{
		if (strcmp(PasswordGame,"") != 0)
			strcpy(PasswordGame,"");
	}
	*/

    if(name[0] == 0)
      sprintf(name, "PermGame%d", i+1);

    if(*map == 0)
      continue;
    
    ParsePermamentGame(i, name, map, data, PasswordGame, MapSettings);
  }

  return;  
}

static int StringToGBMapID(char* str)
{
  if(stricmp(str, "MAPID_WZ_Colorado") == 0)
    return GBGameInfo::MAPID_WZ_Colorado;

  if(stricmp(str, "MAPID_Editor_Particles") == 0)
    return GBGameInfo::MAPID_Editor_Particles;
  if(stricmp(str, "MAPID_ServerTest") == 0)
    return GBGameInfo::MAPID_ServerTest;
    
  r3dError("bad GBMapID %s\n", str);
  return 0;
}

static EGBGameRegion StringToGBRegion(const char* str)
{
  if(stricmp(str, "GBNET_REGION_US_West") == 0)
    return GBNET_REGION_US_West;
  if(stricmp(str, "GBNET_REGION_US_East") == 0)
    return GBNET_REGION_US_East;
  if(stricmp(str, "GBNET_REGION_Europe") == 0)
    return GBNET_REGION_Europe;
  if(stricmp(str, "GBNET_REGION_Russia") == 0)
    return GBNET_REGION_Russia;
    
  r3dError("bad GBGameRegion %s\n", str);
  return GBNET_REGION_Unknown;
}

void CMasterServerConfig::ParsePermamentGame(int gameServerId, const char* name, const char* map, const char* data, const char* PasswordGame, const char* MapSettings)
{
  char mapid[128];
  char maptype[128];
  char region[128];
  int minGames;
  int maxGames;
  if(5 != sscanf(map, "%s %s %s %d %d", mapid, maptype, region, &minGames, &maxGames)) {
    r3dError("bad map format: %s\n", map);
  }

  int maxPlayers;
  int minLevel = 0;
  int maxLevel = 0;
  if(3 != sscanf(data, "%d %d %d", &maxPlayers, &minLevel, &maxLevel)) {
    r3dError("bad data format: %s\n", data);
  }

  GBGameInfo ginfo;
  ginfo.mapId        = StringToGBMapID(mapid);
  ginfo.maxPlayers   = maxPlayers;

  r3dscpy(ginfo.name, name);
  r3dscpy(ginfo.PasswordGame,PasswordGame);
  r3dscpy(ginfo.MapSettings,MapSettings);

  r3dOutToLog("permgame: ID:%d, %s, %s - Password: %s\n",
    gameServerId, name, mapid,PasswordGame);
  
  EGBGameRegion eregion = StringToGBRegion(region);
  AddPermanentGame(gameServerId, ginfo, eregion);
}

void CMasterServerConfig::AddPermanentGame(int gameServerId, const GBGameInfo& ginfo, EGBGameRegion region)
{
  r3d_assert(numPermGames_ < R3D_ARRAYSIZE(permGames_));
  permGame_s& pg = permGames_[numPermGames_++];

  r3d_assert(gameServerId);
  pg.ginfo = ginfo;
  pg.ginfo.gameServerId = gameServerId;
  pg.ginfo.region       = region;

  return;
}
