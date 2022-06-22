
#ifndef included_scrollbox_h
#define included_scrollbox_h

#include "tosser.h"


#define SIZE_SCROLLBOX_NAME     64

//! Scroll box object
class ScrollBox
{

public:

    /**
     * The name of the scroll box
     */
    string name;

    /**
     * X coordinate of scroll box
     */
    int x;

    /**
     * Y coordinate of scroll box
     */
    int y;

    /**
     * Width of scroll box
     */
    int w;

    /**
     * Height of scroll box
     */
    int h;

    /**
     * The number of items within the scroll box
     */
    int numItems;

    /**
     * The height of the scroll box window
     */
    int windowSize;

    /**
     * The index of the first item to display in the scroll box
     */
    int currentIndex;

protected:

    /**
     * Function callback for when value is changed
     */
    void (*callback) ( const string &, int );

public:

    ScrollBox();
    ~ScrollBox();

    /**
     * Sets the name of the scroll box
     * @param newname The name to set
     */
    void SetName            (const string &newname );

    /**
     * Set the dimensions of the scroll box
     * @param newx X coordinate of scroll box
     * @param newy Y coordinate of scroll box
     * @param neww Width of scroll box
     * @param newh Height of scroll box
     */
    void SetDimensions      ( int newx, int newy, int neww, int newh );

    /**
     * Sets the number of items in the scroll box
     * @param newNumItems The number of items in the scroll box
     */
    void SetNumItems        ( int newNumItems );

    /**
     * Sets the height of the scroll box window
     * @param newWindowSize
     */
    void SetWindowSize      ( int newWindowSize );

    /**
     * Set the index of the first item to display in the scroll box
     * @param newCurrentIndex The index of the first item
     */
    void SetCurrentIndex    ( int newCurrentIndex );

    /**
     * Function callback for when value is changed
     * @param newcallback The callback to use when value is changed
     */
    void SetCallback        ( void (*newcallback) (const string &, int) );

protected:

    /** @name Interface Stuff
     */
    ///@{

    /**
     * Has the interface already been created
     */
	bool interfaceCreated;

	/**
	 * Registers all buttons on the interface
	 */
    void CreateInterface ();

    /**
     * Removes all buttons on the interface
     */
    void RemoveInterface ();

    /**
     * Marks the scroll box for re-render
     */
    void UpdateInterface () const;

    /**
     * Always true
     * @return true
     */
    static bool IsInterfaceVisible ();

    ///@}

    /**
     * Decrements currentIndex to shift list of items
     */
    void ScrollUp ();

    /**
     * Increments currentIndex to shift list of items
     */
    void ScrollDown ();

    static void ClickScrollUp       ( Button *button );
    static void ClickScrollDown     ( Button *button );
    static void ClickScrollBar      ( Button *button );
    static void MouseDownScrollBar  ( Button *button );
    static void MouseMoveScrollBar  ( Button *button );
    static void DrawScrollBar       ( Button *button, bool highlighted, bool clicked );

public:

    /** @name Management of all scroll boxes
     */
    ///@{

    /**
     * Creates a scroll box
     * @param name Name of scroll box
     * @param x X coordinate
     * @param y Y coordinate
     * @param w Width
     * @param h Height
     * @param numItems Number of items in the scroll box
     * @param windowSize The height of the scroll box window
     * @param currentIndex The index of the first item to display
     * @param callback The callback for when values change
     */
    static void CreateScrollBox (const string &name,
                                 int x, int y, int w, int h,
                                 int numItems, int windowSize, int currentIndex,
                                 void (*callback)(const string &, int) = nullptr );

    /**
     * Removes a scroll box by name
     * @param name Name of scroll box
     */
    static void RemoveScrollBox (const string &name );

    /**
     * Gets a scroll box by name
     * @param name Name of scroll box
     * @return ScrollBox
     */
    static ScrollBox *GetScrollBox (const string &name );

    /**
     * Stores scroll boxes
     */
    static LList <ScrollBox *> scrollBoxes;

    ///@}

    /** @name Mouse drag handling
     */
    ///@{

    /**
     * Name of the item currently grabbed
     */
    static string currentGrab;

    /**
     * Sets the name of the item being grabbed
     * @param name Name of item being grabbed
     */
    static void GrabScrollBar   (const string &name );

    /**
     * Sets the current grab to None
     */
    static void UnGrabScrollBar ();

    /**
     * Is a grab currently being done?
     * @return true if it is
     */
    static bool IsGrabInProgress ();

    /**
     * Calculates the new index to display first based on the mouse position during a grab
     */
    static void UpdateGrabScroll ();

    ///@}

};



#endif

