
/*

  Client Status Interface

	Shows info on the world, the player and the gateway

  */

#ifndef included_statusinterface_h
#define included_statusinterface_h


#include "network/interfaces/networkscreen.h"



class ClientStatusInterface : public NetworkScreen
{

protected:

	static void BackgroundDraw ( Button *button, bool highlighted, bool clicked );

	static void GatewayPanelDraw ( Button *button, bool highlighted, bool clicked );
	static void PersonalPanelDraw ( Button *button, bool highlighted, bool clicked );
	static void WorldPanelDraw ( Button *button, bool highlighted, bool clicked );

	static void MainMenuClick ( Button *button );

protected:

	char hardware	 [256];					// 
	char hudupgrades [256];					// Gateway data
	char connection  [128];					//

	char ratings	 [256];					//
	char criminal	 [256];					// Personal data
	char financial	 [256];					//

	LList <char *> news;					// World data

public:

	ClientStatusInterface();
	~ClientStatusInterface() override;

	void SetHardware	(const string &newhw );
	void SetHUDUpgrades (const string &newhud );
	void SetConnection	(const string &newconnection );

	void SetRating      (const string &newrating );
	void SetFinancial   (const string &newfinancial );
	void SetCriminal    (const string &newcriminal );
	
	void AddNewsStory   (const string &newsstory );

	void Create () override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int  ScreenID () override;

};



#endif


