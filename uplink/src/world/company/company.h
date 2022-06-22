
#ifndef included_company_h
#define included_company_h

// ============================================================================

#include "tosser.h"

#include "app/uplinkobject.h"

#include "world/person.h"

#define COMPANYTYPE_UNKNOWN     0

#define COMPANYTYPE_INDUSTRIAL  1
#define COMPANYTYPE_COMMERCIAL  2
#define COMPANYTYPE_FINANCIAL   3
#define COMPANYTYPE_ACADEMIC    4
#define COMPANYTYPE_GOVERNMENT  6


#define SIZE_COMPANY_NAME   64

// ============================================================================

//! Company UplinkObject
/**
 * Part of the world sub-system
 * Represents a company in the game world.  The company could be a corporation, military, government, academic etc.
 */
class Company : public UplinkObject  
{

protected:

    /**
     * Share price history indexed on month
     */
	int sharehistory [12];

	/**
	 * Most recently set value index in sharehistory
	 */
	int lastmonthset;

public:

    /**
     * Name of the company
     */
	string name;

	/**
	 * Name of the boss
	 */
	string boss;

	/**
	 * Name of the administrator
	 */
	string administrator;

	/**
	 * TODO: Document
	 */
	int size;
	
	/**
	 * The type of the company
	 * @note Valid values are COMPANYTYPE_UNKNOWN, COMPANYTYPE_INDUSTRIAL, COMPANYTYPE_COMMERCIAL, COMPANYTYPE_FINANCIAL, COMPANYTYPE_ACADEMIC, or COMPANYTYPE_GOVERNMENT
	 */
	int TYPE;

	/**
	 * The percentage change of the company share price over time
	 */
	int growth;

	/**
	 * Number on the scale of -100 (Satanic), 0 (Neutral), +100 (Angelic)
	 */
	int alignment;

public:

	Company();
	~Company() override;

	/**
	 * Sets the name of the company
	 * @param newname New name of company
	 */
	void SetName (const string &newname );

	/**
	 * Sets the size of the company
	 * @param newsize New size of company
	 */
	void SetSize ( int newsize );

	/**
	 * Set the type of the company
	 * @param newTYPE New type of the company
	 */
	void SetTYPE ( int newTYPE );

	/**
	 * Set the growth of the company
	 * @param newgrowth New growth of company
	 */
	void SetGrowth ( int newgrowth );

	/**
	 * Sets the alignment of the company
	 * @param newalignment New alignment of the company
	 */
	void SetAlignment ( int newalignment );

	/**
	 * Sets the name of the company boss
	 * @param bossname New name of the company boss
	 */
	void SetBoss (const string &bossname );

	/**
	 * Sets the name of the company administrator
	 * @param adminname New name of the company administrator
	 */
	void SetAdmin (const string &adminname );

	/**
	 * Applies growth to the share price
	 * @param dayspast TODO: Document
	 */
	void Grow ( int dayspast );					// Applies growth

	/**
	 * Varies the growth of the company by +/- 20%
	 */
	void VaryGrowth ();

	/**
	 * Gets the share price for given month
	 * @param month Index of month, or -1 for the share price now
	 * @return The share price for month
	 */
	int GetSharePrice ( int month = -1 );

	/**
	 * Gets the percentage change of the share price since the previous month
	 * @return The percentage change
	 */
	int GetShareChange ();

	/**
	 * Gets the index of the most recently set share price
	 * @return The index of the latest share price
	 */
	int GetShareLastMonthSet () const;
	
	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	void Update () override;
	
	string GetID () override;
	int   GetOBJECTID () override;

};


#endif 
