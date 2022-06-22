
#ifndef included_worldmapinterface_h
#define included_worldmapinterface_h


#include "eclipse.h"

#include "interface/localinterface/localinterfacescreen.h"
#include "rectangle.h"

#define WORLDMAP_NONE     0
#define WORLDMAP_SMALL    1
#define WORLDMAP_LARGE    2


#define TEXTSIZEX 5						// Size of text grid
#define TEXTSIZEY 20					//

class WorldMapLayout;

//! LocalInterfaceScreen that shows the world map
class WorldMapInterface : public LocalInterfaceScreen
{

protected:

    /**
     * Moving dots pattern
     */
    static int  stipplepattern;

    /**
     * Cycles the stipplepattern
     */
    static void CycleStipplePattern ();

    /**
     * Removes the world map
     */
    static void RemoveWorldMapInterface ();

    /**
     * Maximizes the world map
     * @param button
     */
    static void FullScreenClick ( Button *button );

    /**
     * Adds or removes a location from the connection
     * @param button Button for location
     */
    static void LocationClick   ( Button *button );

    /**
     * Starts the connection
     * @param button
     * @note button is unused
     */
    static void ConnectClick     ( Button *button );

    /**
     * Updates the tooltip when hovering connect button
     * @param button Button hovered over
     */
    static void ConnectMouseMove ( Button *button );

    /**
     * Plays sound and calls button callback
     * @param button Connect button
     */
    static void ConnectMouseDown ( Button *button );

    /**
     * Renders the connect/disconnect button
     * @param button Button to draw
     * @param highlighted Is the button highlighted
     * @param clicked Is the button being clicked
     */
    static void ConnectDraw      ( Button *button, bool highlighted, bool clicked );

    /**
     * Clears any half set up connection, closes screen if no half set up connection exists
     * @param button Cancel button
     * @note button is unused
     */
    static void CancelClick       ( Button *button );
    static void TraceAdvanceClick ( Button *button );

    /**
     * Closes the screen
     * @param button
     * @note button is unused
     */
    static void CloseClick        ( Button *button );

    /**
     * Saves the current connection
     * @param button
     * @note button is unused
     */
    static void SaveConnectionClick ( Button *button );

    /**
     * Load saved connection into current connection
     * @param button
     * @note button is unused
     */
    static void LoadConnectionClick ( Button *button );

    /**
     * Zooms the world map in
     * @param button
     * @note button is unused
     */
    static void ZoomInClick ( Button *button );

    /**
     * Zooms the world map out
     * @param button
     * @note button is unused
     */
    static void ZoomOutClick ( Button *button );

    /**
     * Jumps the zoom to a set zoom
     * @param button The zoom slider
     */
    static void ZoomButtonClick ( Button *button );

    /**
     * Renders the zoom slider
     * @param button The zoom slider
     * @param highlighted Is the button highlighted?
     * @param clicked Is the button clicked?
     */
    static void ZoomButtonDraw ( Button *button, bool highlighted, bool clicked );

    /**
     * Renders the small world map in top right
     * @param button The button that stores the world map
     * @param highlighted Should the button be highlighted?
     * @param clicked Is the button clicked?
     */
    static void DrawWorldMapSmall ( Button *button, bool highlighted, bool clicked );

    /**
     * Renders the large world map
     * @param button The button that stores the world map
     * @param highlighted Should the button be highlighted?
     * @param clicked Is the button clicked?
     */
    static void DrawWorldMapLarge ( Button *button, bool highlighted, bool clicked );

    /**
     * Renders a location
     * @param button Button for location
     * @param highlighted Is the button highlighted?
     * @param clicked Is the button clicked?
     */
    static void DrawLocation ( Button *button, bool highlighted, bool clicked );

    /**
     * Get the X that should be used when rendering for given map size
     * @param x X coordinate of stored location
     * @param SIZE The size of the world map
     * @return The X coordinate that should be used
     * @note SIZE can be either WORLDMAP_SMALL or WORLDMAP_LARGE
     */
    static int GetScaledX ( int x, int SIZE );

    /**
     * Get the Y that should be used when rendering for given map size
     * @param y Y coordinate of stored location
     * @param SIZE The size of the world map
     * @return The Y coordinate that should be used
     * @note SIZE can be either WORLDMAP_SMALL or WORLDMAP_LARGE
     */
    static int GetScaledY ( int y, int SIZE );

    /**
     * Get the rectangle of the large map
     * @return The rectangle of the large map
     */
    static MapRectangle GetLargeMapRect();

    /**
     * Gets the top left X coordinate of the large map rectangle
     * @return The top left X coordinate
     */
    static int GetLargeMapX1();

    /**
     * Gets the top left Y coordinate of the large map rectangle
     * @return The top left Y coordinate
     */
    static int GetLargeMapY1();

    /**
     * Gets the width of the large map rectangle
     * @return width of large map rectangle
     */
    static int GetLargeMapWidth();

    /**
     * Gets the height of the large map rectangle
     * @return height of large map rectangle
     */
    static int GetLargeMapHeight();

    /**
     * Updates all locations with whether or not user has an account on that system
     */
	static void UpdateAccessLevel();

protected:

    // This stuff handles the pre-cached text and lines

    /**
     * The layout of the view
     */
    WorldMapLayout *layout;

    /**
     * Loads all links and locations into the layout
     */
    void ProgramLayoutEngine();

    /**
     * Loads in temporary links and locations into the layout
     */
	void ProgramLayoutEnginePartial();

	/**
	 * Renders all objects in the layout
	 */
    void DrawAllObjects ();

    /**
     * If the number of links in connection changed, re-render all objects in layout
     */
    void CheckLinksChanged();

    /**
     * Removes all temporary links from layout
     */
	static void RemoveTempConnectionButton();

protected:

    /**
     * The saved connection
     */
    LList <string> savedconnection;

    /**
     * Save the current connection
     */
    void SaveCurrentConnection ();

    /**
     * Load the saved connection
     */
    void LoadConnection ();

protected:

    // This stuff handles map zooming and scrolling

    /**
     * The X position we're at in the world map
     */
    static float scrollX;

    /**
     * The Y position we're at in the world map
     */
    static float scrollY;

    /**
     * The zoom level
     */
    static float zoom;

    /**
     * Change the position of the map at the current zoom level
     * @param x How far to move the map on the X axis
     * @note This is called when cursor is in the left or right section of the world map
     */
    static void ScrollX ( float x );

    /**
     * Change the position of the map at the current zoom level
     * @param y How far to move the map on the Y axis
     * @note This is called when cursor is in the top or bottom section of the world map
     */
    static void ScrollY ( float y );

    /**
     * Re-renders the world map at certain zoom level
     * @param z Zoom level
     */
    static void SetZoom ( float z );

    /**
     * Changes the zoom level
     * @param z Zoom level to go to
     */
    static void ChangeZoom ( float z );

public:

    /**
     * Re-renders the world map in certain size
     * @param SIZE Size of world map
     * @note Possible values for SIZE are WORLDMAP_SMALL and WORLDMAP_LARGE
     */
    static void CreateWorldMapInterface ( int SIZE );

    /**
     * Renders the small world map
     */
    static void CreateWorldMapInterface_Small ();

    /**
     * Renders the large world map
     */
    static void CreateWorldMapInterface_Large ();

    /**
     * Closes the large world map, and renders the small one
     */
    static void CloseWorldMapInterface_Large ();

    /**
     * Returns the size of the map visible
     * @return The size of the map visible
     * @note Possible return values are WORLDMAP_SMALL and WORLDMAP_LARGE
     */
    static int IsVisibleWorldMapInterface ();

    /**
     * Connects to computer after the phone dialer
     * @param ip
     * @param info
     */
	static void AfterPhoneDialler (string &ip, string &info );
    
    WorldMapInterface ();
    ~WorldMapInterface () override;

    void Create () override;

    /**
     * Renders the world map in certain size
     * @param SIZE Size of world map
     * @note Possible values for SIZE are WORLDMAP_SMALL and WORLDMAP_LARGE
     */
    void Create ( int SIZE );
    void Update () override;
    void Remove () override;
    bool IsVisible () override;

    int ScreenID () override;

	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	
	string GetID () override;

protected:

	// Used for the coloring of the servers

	/**
	 * Number of missions
	 */
	int nbmissions;

	/**
	 * Number of messages
	 */
	int nbmessages;

	/**
	 * Number of colored links
	 */
	int nbcolored;

};



#endif
