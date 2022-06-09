// ============================================================================

#define WORLDMAPOBJECT_NONE			0
#define WORLDMAPOBJECT_LABEL		1
#define WORLDMAPOBJECT_LOCATION		2
#define WORLDMAPOBJECT_GATEWAY		3

#include "rectangle.h"
#include "tosser.h"

/**
 * An individual item to display on the world map
 */
class WorldMapInterfaceObject
{

public:

    /**
     * The machine type
     */
    int TYPE;

    /**
     * TODO: Document
     */
    int x;

    /**
     * TODO: Document
     */
    int y;

    /**
     * X coordinate of object
     */
    int baseX;

    /**
     * Y coordinate of object
     */
    int baseY;

    /**
     * If the connection requires a server that isn't on the map, this will be true
     */
    bool tempForConnection;

public: 

    WorldMapInterfaceObject ();
    virtual ~WorldMapInterfaceObject ();

    /**
     * Sets the type of object
     * @param newTYPE Type of object
     * @note Accepted values of newTYPE: WORLDMAPOBJECT_LABEL, WORLDMAPOBJECT_GATEWAY, WORLDMAPOBJECT_LOCATION
     */
    void SetTYPE ( int newTYPE );
    void SetPosition ( int newx, int newy );
    void SetBasePosition ( int newx, int newy );

    /**
     * Sets the IP of the object
     * @param newip IP to set it to
     */
    void SetIP (const string& newip );

    /**
     * Checks if object needs is part of a mission or needs to be colored based on IP
     */
	void CheckIP ();

    /**
     * Renders the object on the map
     * @param xOffset The X offset to render the object at
     * @param yOffset The Y offset to render the object at
     * @param zoom A multiplier to use when calculating the final render coordinates
     * @note zoom is not used in any calls to this function
     */
    virtual void Draw ( int xOffset = 0, int yOffset = 0, float zoom = 1.0 );

    /**
     * Gets the rectangle for the position and size of this object
     * @return The rectangle for position and size
     */
    virtual MapRectangle GetExtent() const;

    /**
     * Gets the IP of the object
     * @return The IP of the object
     */
    string GetIP() const;

protected:

    /**
     * The IP of the object
     */
    string ip;

    /**
     * Is this object related to a mission
     */
    bool isMission;

    /**
     * Is this object colored
     */
	bool isColored;

};

//! The label for the object on the world map
class WorldMapInterfaceLabel : public WorldMapInterfaceObject {
public:
    
    WorldMapInterfaceLabel(const MapRectangle &mapRect,
                           WorldMapInterfaceObject *newFeaturePoint,
                           const string& newCaption);
    ~WorldMapInterfaceLabel() override;

    /**
     * Sets the text to display in label
     * @param newcaption Text to display
     */
    void SetCaption (const string& newcaption );

    /**
     * Sets the label position
     * @param n Position location index
     */
    void SetLabelPosition(int n);

    /**
     * Sets the label position to a random one
     */
    void SetRandomLabelPosition();

    /**
     * Gets the label position
     * @return Label position
     */
    int GetLabelPosition() const;
    MapRectangle GetExtent() const override;

    /**
     * Check if this label is overlapped by another label
     * @param label Label to check collision with
     * @return true if two labels collide
     */
    bool Overlaps(WorldMapInterfaceObject *label) const;

    void Draw( int xOffset = 0, int yOffset = 0, float zoom = 1.0  ) override;

    /**
     * Gets the text in the label
     * @return The text in the label
     */
    string GetCaption() const;
    
protected:

    /**
     * Calculates the width of the label based on the text in label
     */
    void CalculateWidth();

    /**
     * Calculates all valid label positions
     * @param mapRect The map rectangle to ensure label is fully within
     */
    void CalculatePossibleLabelPositions(const MapRectangle &mapRect);

protected:

    /**
     * Text to display in label
     */
    string caption;

    /**
     * TODO: Document
     */
    WorldMapInterfaceObject *featurePoint; // Does not own featurePoint

    /**
     * Position location index
     * @note 0 is top center, 1 is bottom center, 2 is bottom right, 3 is right, 4 is top right, 5 is top left, 6 is left, 7 is bottom left
     */
    int labelPos;

    /**
     * Width of the text in the label
     */
    int labelWidth;

    /**
     * Number of valid, possible label placements
     */
    int numPossLabelPos;

    /**
     * All valid label positions that are fully within the world map window
     */
    int possLabelPos[8];
};

/**
 * TODO: Document
 */
class WorldMapObjectiveFunction {
public:
    
    WorldMapObjectiveFunction(const MapRectangle &mapRect);
    ~WorldMapObjectiveFunction();

    /**
     * Gets the cost
     * @return The cost
     */
    int GetCost() const;

    /**
     * TODO: Document
     * @param m
     */
    void AddObject( const WorldMapInterfaceObject *m );

    /**
     * TODO: Document
     * @param m
     */
    void SubObject( const WorldMapInterfaceObject *m );

    /**
     * TODO: Document
     * @param r
     */
    void AddRect( const MapRectangle& r );

    /**
     * TODO: Document
     * @param r
     */
    void SubRect( const MapRectangle& r );

    /**
     * Resets the cost and shadeScreen to 0
     */
    void Reset();
    
protected:

    /**
     * TODO: Document
     */
    int cost;

    /**
     * TODO: Document
     */
    int mapWidth, mapHeight;

    /**
     * TODO: Document
     */
    int *shadeScreen;

    /**
     * TODO: Document
     */
    MapRectangle clipRect;
    
};

//! The world map layout
/**
 * This world map layout is rendered to the screen in the maximized world map view
 */
class WorldMapLayout {
public:
    
    WorldMapLayout(const MapRectangle &newMapRectangle);
    ~WorldMapLayout();

    /**
     * Resets the world map
     */
    void Reset ();

    /**
     * Does nothing
     */
    void ResetTemp ();

    /**
     * Adds a location to the world map
     * @param x X coordinate of location
     * @param y Y coordinate of location
     * @param name The label text
     * @param ip The IP of the location
     * @param tempForConnection Should this location only exist for the current connection?
     */
    void AddLocation (int x, int y, const string& name, const string &ip = "", bool tempForConnection = false );

    /**
     * Deletes temporary locations
     */
    void DeleteLocationsTemp ();

    /**
     * TODO: Document
     */
    void PartialLayoutLabels();

    /**
     * TODO: Document
     */
    void FullLayoutLabels();

    /**
     * Is the layout done being rendered
     * @return true if layout is fully rendered
     */
    bool IsLayoutComplete() const;

    /**
     * Gets all locations on the map
     * @return All locations on map
     */
    LList <WorldMapInterfaceObject *> & GetLocations();

    /**
     * Gets all labels on the map
     * @return All labels on map
     */
    LList <WorldMapInterfaceLabel *> & GetLabels();

    /**
     * Gets the number of locations that only exist for current connection on the map
     * @return Number of temporary locations
     */
    int GetCountLocationTemp();

    /**
     * Check if any locations are part of a mission or are colored
     */
	void CheckIPs();
    
protected:

    /**
     * Renders all locations and labels
     */
    void StartLayout();

    /**
     * Deletes all locations and labels on map
     */
    void DeleteLocations();

    /**
     * Resets the map
     */
    void ResetLayoutParameters();
    
protected:

    /**
     * TODO: Document
     */
    int iteration;

    /**
     * TODO: Document
     */
    int moveNumber;

    /**
     * TODO: Document
     */
    int numGoodMoves;

    /**
     * The temperature
     */
    float T;

    /**
     * Current value of the objective function
     */
    float E;

    /**
     * Is the layout fully rendered
     */
    bool layoutComplete;

    /**
     * Has the rendering of the layout started?
     */
    bool layoutStarted;

    /**
     * The rectangle of the map
     */
    MapRectangle mapRectangle;

    /**
     * Locations in the map
     */
    LList <WorldMapInterfaceObject *> locations;

    /**
     * Labels in the map
     */
    LList <WorldMapInterfaceLabel *> labels;

    /**
     * TODO: Document
     */
    WorldMapObjectiveFunction objective;
    
};

