
/*

  Rating class object

	TJ is going to cane Sega Rally!  Merry Xmas!

	Stores the world rating of a person or the player	
	i.e how well qualified they are

  */

#ifndef included_rating_h
#define included_rating_h


#include "app/uplinkobject.h"

#define SIZE_PERSON_NAME 128



class Rating : public UplinkObject
{

public:

	char owner [SIZE_PERSON_NAME];

	int uplinkrating;
	int uplinkscore;

	int neuromancerrating;
	int neuromancerscore;

	int creditrating;

public:

	Rating ();
	~Rating ();

	void SetOwner ( char *newowner );
	
	void SetUplinkRating	  ( int rating );
	void SetNeuromancerRating ( int rating );
	
	void ChangeUplinkScore		( int amount );
	void ChangeNeuromancerScore ( int amount );

	string GetUplinkRating	   () const;
	string GetNeuromancerRating () const;

	void SetCreditRating ( int rating );
    void ChangeCreditRating ( int amount );

	static string GetUplinkRatingString (int urating );
	static string GetNeuromancerRatingString (int nrating );

	// Common functions

	bool Load  ( FILE *file );
	void Save  ( FILE *file );
	void Print ();
	void Update ();
	
	string GetID ();

};



#endif