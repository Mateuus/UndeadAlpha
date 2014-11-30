#pragma once

#include "../../ServerNetPackets/NetPacketsGameInfo.h"

class CMasterServerConfig
{
  public:
	int		masterPort_;
	int		clientPort_;
	int		serverId_;
	int		masterCCU_;	// max number of connected peers
	float   supervisorCoolDownSeconds_;

	//
	// permanent games groups
	//
	struct permGame_s
	{
	  GBGameInfo	ginfo;
	  
	  permGame_s()
	  {
	  }
	};
	permGame_s	permGames_[4096];
	int		numPermGames_;

	void		LoadConfig();

	void		Temp_Load_WarZGames();

	void		LoadPermGamesConfig();
	void		 ParsePermamentGame(int gameServerId, const char* name, const char* map, const char* data, const char* PasswordGame, const char* MapSettings);
	void		 AddPermanentGame(int gameServerId, const GBGameInfo& ginfo, EGBGameRegion region);
	
  public:
	CMasterServerConfig();
};
extern CMasterServerConfig* gServerConfig;
