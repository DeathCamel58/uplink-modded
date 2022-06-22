
#ifndef included_laninterface_h
#define included_laninterface_h

// ============================================================================

#include "world/computer/lancomputer.h"
#include "interface/localinterface/localinterfacescreen.h"

/**
 * TODO: Document
 */
struct LanInterfaceObject
{
    char    *name;
    char    *filename;
    int     width;
    int     height;
    char    *description;
};

// ============================================================================


#define LANHIGHLIGHT_NONE               0
#define LANHIGHLIGHT_VALIDSUBNET        1
#define LANHIGHLIGHT_CONTROLS           2

/**
 * TODO: Document
 */
class LanInterfaceHighlight
{
public:
    LanInterfaceHighlight ( int _systemIndex, char *_text ) 
        : systemIndex(_systemIndex) 
        {
            UplinkStrncpy ( text, _text, sizeof ( text ) )
        }
    
    int systemIndex;
    char text[64];
};


// ============================================================================


//! LocalInterfaceScreen that shows LAN of connected machine
/**
 * Local Interface screen
 * Allows you to view a Local Area Network
 */
class LanInterface : public LocalInterfaceScreen
{

protected:

    /**
     * LanInterfaceObject of all objects in LAN
     */
    static LanInterfaceObject lanInterfaceObjects[LANSYSTEM_NUMTYPES];

    /**
     * Renders the title of the screen
     * @param button Button for title
     * @param highlighted
     * @param clicked
     * @note highlighted and clicked are unused
     */
    static void TitleDraw           ( Button *button, bool highlighted, bool clicked );

    /**
     * Renders the background of the screen
     * @param button button for background
     * @param highlighted
     * @param clicked
     * @note highlighted and clicked are unused
     */
    static void PanelBackgroundDraw ( Button *button, bool highlighted, bool clicked );

    /**
     * Renders the LAN system
     * @param button Button of system
     * @param highlighted
     * @param clicked
     * @note highlighted and clicked are unused
     */
    static void LanSystemDraw		( Button *button, bool highlighted, bool clicked );

    /**
     * Sets clicked button to the target of the program, and runs it
     * @param button Button that was clicked
     */
    static void LanSystemClick		( Button *button );

    /**
     * Sets tooltip of hovered LAN item
     * @param button Button that was hovered over
     */
    static void LanSystemMouseMove	( Button *button );

    /**
     * Closes the screen
     * @param button
     * @note button is unused
     */
    static void TitleClick          ( Button *button );

    /**
     * Renders background of LAN system
     * @param button Button of LAN system
     * @param highlighted
     * @param clicked
     * @note highlighted and clicked are unused
     */
    static void LanBackgroundDraw	( Button *button, bool highlighted, bool clicked );
    static void LanBackgroundMouseMove ( Button *button );

    /**
     * Renders a system in LAN
     * @param button Button for system
     * @param highlighted TODO: Document
     * @param clicked TODO: Document
     */
	static void ConnectDraw			( Button *button, bool highlighted, bool clicked );

	/**
	 * Connects player to system clicked on
	 * @param button Button for system clicked on
	 */
	static void ConnectClick		( Button *button );

	/**
	 * Highlights the LAN item currently hovered over
	 * @param button Button hovered over
	 */
	static void ConnectMouseMove	( Button *button );

	/**
	 * TODO: Document
	 * @param button
     * @note button is unused
	 */
	static void CancelClick			( Button *button );

	/**
	 * TODO: Document
	 * @param button
	 * @param highlighted
	 * @param clicked
	 */
    static void BackDraw            ( Button *button, bool highlighted, bool clicked );

    /**
     * TODO: Document
     * @param button
     */
    static void BackMouseMove       ( Button *button );

    /**
     * TODO: Document
     * @param button
     */
    static void BackClick           ( Button *button );

    /**
     * TODO: Document
     * @param button
     */
    static void GenerateClick       ( Button *button );

    /**
     * Renders a link between two points
     * @param link The link to draw
     * @param fromX Starting X coordinate
     * @param fromY Starting Y coordinate
     * @param toX Ending X coordinate
     * @param toY Ending Y coordinate
     */
	static void DrawLink ( LanComputerLink *link, 
							float fromX, float fromY,
							float toX, float toY );

	/**
	 * Moves the displayed LAN within the viewport
	 * @param button The clicked button
	 */
    static void ScrollClick ( Button *button );

protected:

    /**
     * The IP of the system
     */
    char ip[SIZE_VLOCATION_IP];

    /**
     * TODO: Document
     */
    LList <LanInterfaceHighlight *> highlights;

    /**
     * The X offset of the system
     */
    static int offsetX;

    /**
     * The Y offset of the system
     */
    static int offsetY;

public:

    LanInterface ();
    ~LanInterface () override;

    /**
     * Render the layout and center on current connection
     */
    void CreateLayout ();


    /**
     * Remove the LAN from the display
     */
    static void RemoveLayout ();

    /**
     * Repositions all buttons
     */
    void PositionLayout ();

    /**
     * Connect to another system
     * @param systemIndex Index of system to switch to
     */
    void SelectSystem ( int systemIndex );

    /**
     * TODO: Document
     * @param TYPE
     * @return LanInterfaceObject
     */
    static LanInterfaceObject *GetLanInterfaceObject( int TYPE );

	void Create () override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int  ScreenID () override;

};


#endif

