
#ifndef included_sale_h
#define included_sale_h

// ============================================================================

#include "app/uplinkobject.h"

#define SALETYPE_NONE      0
#define SALETYPE_SOFTWARE  1
#define SALETYPE_HARDWARE  2

#define SIZE_SALE_TITLE	   64

class SaleVersion;

// ============================================================================

//! Sale UplinkObject
/**
 * This represents a sellable item.
 * @note This can be for either software or hardware
 */
class Sale : public UplinkObject
{

public:

    /**
     * The name of the item
     */
	string title;

	/**
	 * The type of the item
	 * @note This can be SALETYPE_NONE, SALETYPE_SOFTWARE, or SALETYPE_HARDWARE
	 */
	int saleTYPE;

	/**
	 * The type of item in the type category
	 * @note For hardware items this can be GATEWAYUPGRADETYPE_CPU, GATEWAYUPGRADETYPE_MODEM, GATEWAYUPGRADETYPE_COOLING, GATEWAYUPGRADETYPE_MEMORY, GATEWAYUPGRADETYPE_SECURITY, or GATEWAYUPGRADETYPE_POWER
	 * @note For software items this can be SOFTWARETYPE_NONE, SOFTWARETYPE_FILEUTIL, SOFTWARETYPE_HWDRIVER, SOFTWARETYPE_SECURITY, SOFTWARETYPE_CRACKERS, SOFTWARETYPE_BYPASSER, SOFTWARETYPE_LANTOOL, SOFTWARETYPE_HUDUPGRADE, or SOFTWARETYPE_OTHER
	 */
	int swhwTYPE;

	/**
	 * DArray of available SaleVersion
	 */
	DArray <SaleVersion *> versions;

public:

	Sale ();
	~Sale () override;

	/**
	 * Sets the name of this item
	 * @param newtitle The new name of the item
	 */
	void SetTitle (const string &newtitle );

	/**
	 * Sets the type of item
	 * @param newTYPE The type of the item
	 */
	void SetSaleTYPE ( int newTYPE );

	/**
	 * Sets the type of item in the type category
	 * @param newSwhwTYPE The type of the item
	 */
	void SetSwhwTYPE ( int newSwhwTYPE );

	/**
	 * Add a new version of this item
	 * @param details Details of the item
	 * @param cost Cost of the item
	 * @param size Size of the item
	 * @param data Software version or hardware speed
	 */
	void AddVersion (const string &details, int cost, int size, int data );

	/**
	 * Get the version of an item at index
	 * @param index Index to get
	 * @return SaleVersion of item or nullptr
	 */
	SaleVersion *GetVersion ( int index ) const;

	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	void Update () override;
	
	string GetID () override;
	int   GetOBJECTID () override;

};


class SaleVersion : public UplinkObject
{

protected:

    /**
     * The description of this item
     */
	char *details;

public:

    /**
     * The cost of this item
     */
	int cost;
	int size;

	/**
	 * Software version or hardware speed
	 */
	int data;

public:

	SaleVersion ();
	~SaleVersion () override;

	/**
	 * Sets various attributes of the item
	 * @param newdetails The description of the item
	 * @param newcost The cost of the item
	 * @param newsize The size of the item
	 * @param newdata New software version/hardware speed
	 */
	void Set (const string &newdetails, int newcost, int newsize, int newdata );

	/**
	 * Gets the description of the item
	 * @return
	 */
	char *GetDetails ();

	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	void Update () override;
	
	string GetID () override;
	int   GetOBJECTID () override;

};


#endif

