
#ifndef included_hudinterface_h
#define included_hudinterface_h

// ============================================================================

#include "eclipse.h"

#include "interface/localinterface/localinterfacescreen.h"
#include "interface/localinterface/gateway_interface.h"
#include "interface/localinterface/sw_interface.h"
#include "interface/localinterface/worldmap/worldmap_interface.h"

//! A HUD interface upgrade
struct HUDInterfaceUpgrade
{
    /**
     * Position to show the button in
     */
    char number;
    /**
     * Name of the upgrade
     */
    char *name;
    /**
     * Tooltip to show on upgrade hover
     */
    char *tooltip;
    /**
     * Button name to use for icon
     */
    char *buttonName;
    /**
     * Image filename to use for button
     */
    char *buttonFilename;
    /**
     * Image filename to use for highlighted button
     */
    char *buttonFilename_h;
    /**
     * Image filename to use for clicked button
     */
    char *buttonFilename_c;
    void (*callback) (Button *);
};

// ============================================================================


//! Screen that shows worldmap, main toolbar, services, messages, etc
class HUDInterface : public LocalInterfaceScreen
{
	
public:

	WorldMapInterface wmi;
	SWInterface	      si;
	GatewayInterface  gw;

protected:

  	char *previoushighlight;
	Image *previousimage;

	/**
	 * The currently visible upgrades
	 * @note entries correspond to visual position on screen
	 */
    char visibleUpgrades[8];
    static HUDInterfaceUpgrade hudUpgrades[8];

protected:

    /**
     * Closes the game
     * @param button The X button in top left
     * @note Uses CloseGame
     */
	static void MainMenuClick     ( Button *button );

	/**
	 * Switches to software screen
	 * @param button The software button
	 */
	static void SoftwareClick	  ( Button *button );

    /**
     * Switches to hardware screen
     * @param button The hardware button
     */
	static void HardwareClick     ( Button *button );

    /**
     * Switches to memory screen
     * @param button The memory button
     */
	static void MemoryClick       ( Button *button );

    /**
     * Switches to status screen
     * @param button The status button
     */
	static void StatusClick       ( Button *button );

    /**
     * Switches to finance screen
     * @param button The finance button
     */
	static void FinanceClick	  ( Button *button );

    /**
     * Switches to send mail screen
     * @param button The send mail button
     */
	static void SendMailClick     ( Button *button );

    /**
     * Switches to analyser screen
     * @param button The analyser button
     */
	static void AnalyserClick	  ( Button *button );

    /**
     * Switches to IRC screen
     * @param button The IRC button
     */
    static void IRCClick          ( Button *button );

    /**
     * Switches to LAN screen
     * @param button The LAN button
     */
    static void LANClick          ( Button *button );

    /**
     * Renders the button to the toolbar
     * @param button The button to draw
     * @param highlighted TODO: Document
     * @param clicked TODO: Document
     */
	static void ToolbarButtonDraw ( Button *button, bool highlighted, bool clicked );

    /**
     * Highlight the toolbar button for email
     * @param bname The email button to highlight
     * @note This is used to visually show which screen we have open
     */
	static void EmailHighlight	  ( Button *button );

	/**
	 * Switches to the email view screen and populates fields
	 * @param button The email button clicked on
	 */
	static void EmailClick	      ( Button *button );

    /**
     * Highlight the toolbar button for mission
     * @param bname The mission button to highlight
     * @note This is used to visually show which screen we have open
     */
	static void MissionHighlight  ( Button *button );

    /**
     * Switches to the mission view screen and populates fields
     * @param button The mission button clicked on
     */
	static void MissionClick	  ( Button *button );

	/**
	 * Renders the speed button
	 * @param button The button to draw
	 * @param highlighted TODO: Document
	 * @param clicked TODO: Document
	 */
	static void SpeedButtonDraw   ( Button *button, bool highlighted, bool clicked );

	/**
	 * Sets the game speed based on button
	 * @param button The button that was clicked on
	 */
	static void SpeedButtonClick  ( Button *button );

	/**
	 * Pauses the game
	 * @param button Pause button
	 */
    static void PauseButtonClick  ( Button *button );
    /**
     * Unpauses the game
     * @param button Close button
     */
    static void UnPauseClick      ( Button *button );

    /**
     * Highlight the toolbar button
     * @param bname The button to highlight
     * @note This is used to visually show which screen we have open
     */
	static void HighlightToolbarButton   (const string& bname);

    /**
     * Remove highlight from the toolbar button
     * @param bname The button to remove highlight from
     * @note This is used to visually show which screen we have open
     */
	static void UnHighlightToolbarButton ();

public:

	HUDInterface ();
	~HUDInterface () override;

	/**
	 * Changes the screen to match the button clicked on
	 * @param screenID The screen ID to change to
	 * @param screenindex Normally unused
	 * @note screenindex is only used in some cases when a particular screenID might not be enough to locate the button - for example the emails at the bottom of the screen (all the same screenID, but different buttons based on index)
	 */
	static void MoveSelecter ( int screenID, int screenindex );

    bool IsUpgradeVisible ( char upgrade );

    /**
     * Adds an upgrade to the HUD
     * @param upgrade Upgrade to add
     */
    void AddUpgrade       ( char upgrade );

    /**
     * Removes an upgrade from the HUD
     * @param upgrade Upgrade to remove
     */
    static void RemoveUpgrade    ( char upgrade );

    /**
     * Gets an upgrade by name
     * @param upgrade Name of upgrade
     * @return HUDInterfaceUpGrade
     */
    static HUDInterfaceUpgrade *GetUpgrade ( char upgrade );

	void Create () override;
	void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

	/**
	 * Gets a pointer to the current HUD
	 * @return The HUDInterface
	 */
	static HUDInterface *GetHUD ();	

	/**
	 * Close connections, save game, log off
	 */
	static void CloseGame ();

	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	
	string GetID () override;

};


#endif

