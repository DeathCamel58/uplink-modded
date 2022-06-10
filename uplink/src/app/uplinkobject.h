
#ifndef included_uplinkobject_h
#define included_uplinkobject_h

#include <cstdio>

#include "tosser.h"

#define SIZE_SAVEID     9
#define SIZE_SAVEID_END 13

//! UplinkObject base class
/**
 * All uplink classes should be derived from this
 * @note All classes derived from this should implement Load, Save and Print
 * @note Implementing Update is optional
 */
class UplinkObject  
{

public:

	UplinkObject();
	virtual ~UplinkObject();

	/**
	 * Loads data to this object from file
	 * @param file File to load from
	 * @return true if loading was successful
	 */
	virtual bool Load   ( FILE *file );

	/**
	 * Saves data from this object to file
	 * @param file File to save to
	 */
	virtual void Save   ( FILE *file );

	/**
	 * Prints all data about this object
	 * @note This is for debugging
	 */
	virtual void Print  ();

	/**
	 * Processes any necessary updates on the object.
	 */
	virtual void Update ();

	/**
	 * Gets the ID of the object
	 * @return The ID of the object
	 * @note The ID is the type of object this is
	 */
	virtual string GetID ();

	/**
	 * Gets the object type ID used in Tosser structures
	 * @return The object ID
	 */
	virtual int   GetOBJECTID ();

	/**
	 * Gets the ID of the object, and appends `_END` to it
	 * @return The ID + `_END`
	 */
	string GetID_END ();

	/**
	 * Prints debug information or writes to debug file
	 * @param file File to write to
	 * @note If `SAVEFILE_PRINTPROGRESS` is enabled, it will print information to console
	 * @note If `SAVEFILE_VERBOSE` is enabled, it will output debug information to file
	 */
	void LoadID ( FILE *file );

    /**
     * Prints debug information or writes to debug file
     * @param file File to write to
     * @note If `SAVEFILE_PRINTPROGRESS` is enabled, it will print information to console
     * @note If `SAVEFILE_VERBOSE` is enabled, it will output debug information to file
     */
	void SaveID ( FILE *file );

    /**
     * Prints debug information or writes to debug file
     * @param file File to write to
     * @note If `SAVEFILE_PRINTPROGRESS` is enabled, it will print information to console
     * @note If `SAVEFILE_VERBOSE` is enabled, it will output debug information to file
     */
	void LoadID_END ( FILE *file );

    /**
     * Prints debug information or writes to debug file
     * @param file File to write to
     * @note If `SAVEFILE_PRINTPROGRESS` is enabled, it will print information to console
     * @note If `SAVEFILE_VERBOSE` is enabled, it will output debug information to file
     */
	void SaveID_END ( FILE *file );

};


#endif 
