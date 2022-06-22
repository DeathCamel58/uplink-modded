
#ifndef included_vlocation_h
#define included_vlocation_h

// ============================================================================

#include <cstdio>

#include "app/uplinkobject.h"
 
#define SIZE_VLOCATION_IP  24
#define SIZE_COMPUTER_NAME   64

#define VIRTUAL_WIDTH     594
#define VIRTUAL_HEIGHT    315

class Computer;

// ============================================================================


//! Location in the uplink world
class VLocation : public UplinkObject
{

public:

    /**
     * The ip of the location
     */
	char ip       [SIZE_VLOCATION_IP];

	/**
	 * The name of the computer at this location
	 */
	string computer;

	/**
	 * X coordinate of physical location on map
	 */
	int x;

    /**
     * Y coordinate of physical location on map
     */
	int y;

	/**
	 * Is the location publicly listed?
	 */
	bool listed;

	/**
	 * Is the location displayed on the world map?
	 */
    bool displayed;

    /**
     * Should the location be displayed in a distinct color on the world map?
     */
	bool colored;

public:

	VLocation();
	~VLocation() override;

	/**
	 * Sets the ip of the location
	 * @param newip The new ip of the location
	 */
	void SetIP (const string &newip );

	/**
	 * Check that the new ip of the location is within size limits and isn't empty
	 * @param newip The ip to check
	 * @return true if it's a valid ip
	 */
	static bool VerifyIP (const string &newip );

	/**
	 * Set the computer name of the location
	 * @param newcomputer The new computer name
	 */
	void SetComputer (const string &newcomputer );

	/**
	 * Sets the physical location of the gateway
	 * @param newx X coordinate
	 * @param newy Y coordinate
	 */
	void SetPLocation ( int newx, int newy );

	/**
	 * Check that the new ip of the location is within size limits
	 * @param newx X coordinate
	 * @param newy Y coordinate
	 * @return true if valid coordinates
	 */
	static bool VerifyPLocation ( int newx, int newy );

	/**
	 * Sets whether the location should be publicly listed
	 * @param newlisted true if it should be
	 */
	void SetListed ( bool newlisted );

	/**
	 * Sets whether the location should be displayed on the world map
	 * @param newdisplayed true if it should be
	 */
    void SetDisplayed ( bool newdisplayed );

    /**
     * Sets whether the location should be colored on the world map
     * @param newcolored true if it should be
     */
	void SetColorDisplayed ( bool newcolored );

	/**
	 * Gets the Computer that exists at this location
	 * @return The Computer at the location, or nullptr if none
	 */
	Computer *GetComputer ();				// Returns NULL if none

	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	
	string GetID () override;
	int   GetOBJECTID () override;

};

//! Special VLocation
/**
 * Used to specify which screen you enter the system at
 * @note Currently used on modems, which can jump into a system at any screen
 */
class VLocationSpecial : public VLocation
{

public:

    /**
     * The index of the only screen at this location
     */
	int screenIndex;
    int securitySystemIndex;

public:

	VLocationSpecial ();
	~VLocationSpecial () override;

	/**
	 * Sets the index of the only screen at this location
	 * @param newScreenIndex The index of the screen
	 */
	void SetScreenIndex ( int newScreenIndex );
    void SetSecuritySystemIndex ( int newIndex );

	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	
	string GetID () override;
	int   GetOBJECTID () override;

};

#endif 
