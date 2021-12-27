
/*

  Definition of HUD interface
	
	i.e Worldmap, main toolbar, services, messages etc

  */

#ifndef _included_hudinterface_h
#define _included_hudinterface_h

// ============================================================================

#include "eclipse.h"

#include "interface/localinterface/localinterfacescreen.h"
#include "interface/localinterface/gateway_interface.h"
#include "interface/localinterface/sw_interface.h"
#include "interface/localinterface/worldmap/worldmap_interface.h"

struct HUDInterfaceUpgrade
{
    char number;
    char *name;
    char *tooltip;
    char *buttonName;
    char *buttonFilename;
    char *buttonFilename_h;
    char *buttonFilename_c;
    void (*callback) (Button *);
};

// ============================================================================


class HUDInterface : public LocalInterfaceScreen
{
	
public:

	WorldMapInterface wmi;
	SWInterface	      si;
	GatewayInterface  gw;

protected:

  	char *previoushighlight;
	Image *previousimage;
    
    char visibleUpgrades[8];                                    // Entries corrispond to visual position on screen. 
    static HUDInterfaceUpgrade hudUpgrades[8];

protected:
	
	static void MainMenuClick     ( Button *button );
	static void SoftwareClick	  ( Button *button );
	static void HardwareClick     ( Button *button );
	static void MemoryClick       ( Button *button );
	static void StatusClick       ( Button *button );
	static void FinanceClick	  ( Button *button );
	static void SendMailClick     ( Button *button );
	static void AnalyserClick	  ( Button *button );
    static void IRCClick          ( Button *button );
    static void LANClick          ( Button *button );

	static void ToolbarButtonDraw ( Button *button, bool highlighted, bool clicked );
	static void EmailHighlight	  ( Button *button );
	static void EmailClick	      ( Button *button );
	static void MissionHighlight  ( Button *button );
	static void MissionClick	  ( Button *button );

	static void SpeedButtonDraw   ( Button *button, bool highlighted, bool clicked );
	static void SpeedButtonClick  ( Button *button );
    
    static void PauseButtonClick  ( Button *button );
    static void UnPauseClick      ( Button *button );

	static void HighlightToolbarButton   ( char *bname );
	static void UnHighlightToolbarButton ();

public:

	HUDInterface ();
	~HUDInterface () override;

	static void MoveSelecter ( int screenID, int screenindex );			// Controls which button is "selected"

    bool IsUpgradeVisible ( char upgrade );
    void AddUpgrade       ( char upgrade );
    void RemoveUpgrade    ( char upgrade );
    HUDInterfaceUpgrade *GetUpgrade ( char upgrade );

	void Create () override;
	void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

	static HUDInterface *GetHUD ();	

	static void CloseGame ();

	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	
	char *GetID () override;

};


#endif

