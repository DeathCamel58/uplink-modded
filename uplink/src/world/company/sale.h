
/*

	Sale class object

	Represents a sellable item -
	such as software or hardware or information

  */

#ifndef _included_sale_h
#define _included_sale_h

// ============================================================================

#include "app/uplinkobject.h"

#define SALETYPE_NONE      0
#define SALETYPE_SOFTWARE  1
#define SALETYPE_HARDWARE  2

#define SIZE_SALE_TITLE	   64

class SaleVersion;

// ============================================================================


class Sale : public UplinkObject
{

public:

	char title [SIZE_SALE_TITLE];
	int saleTYPE;
	int swhwTYPE;

	DArray <SaleVersion *> versions;

public:

	Sale ();
	~Sale () override;

	void SetTitle (const string &newtitle );
	void SetSaleTYPE ( int newTYPE );
	void SetSwhwTYPE ( int newSwhwTYPE );

	void AddVersion (const string &details, int cost, int size, int data );
	SaleVersion *GetVersion ( int index );								// Can return NULL

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

	char *details;

public:

	int cost;
	int size;
	int data;						// SW version or HW speed

public:

	SaleVersion ();
	~SaleVersion () override;

	void Set (const string &newdetails, int newcost, int newsize, int newdata );
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

