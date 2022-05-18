
/*

   Mission class object
   Part of the World sub-system

   A representation of a mission or task to be completed
	
	*/


#ifndef _included_mission_h
#define _included_mission_h

// ============================================================================

#include "app/uplinkobject.h"
#include "world/date.h"

#define SIZE_MISSION_DESCRIPTION    128
#define SIZE_MISSION_EMPLOYER       64
#define SIZE_MISSION_COMPLETION     64

#define SIZE_PERSON_NAME	 128

#define MISSIONPAYMENT_AFTERCOMPLETION		0
#define MISSIONPAYMENT_HALFATSTART			1
#define MISSIONPAYMENT_ALLATSTART			2

// ============================================================================


class Mission : public UplinkObject
{

protected:

	char *details;						// Listed on bbs boards
	char *fulldetails;					// Listed after acceptance (contains actual target details)
	Date *duedate;						// Can be NULL

	char *whysomuchmoney;				// Answer to these questions
	char *howsecure;					// They can be NULL
	char *whoisthetarget;				//

public:

	int TYPE;												// Defined in MissionGenerator
	char employer    [SIZE_MISSION_EMPLOYER];
	char contact     [SIZE_PERSON_NAME];
	char description [SIZE_MISSION_DESCRIPTION];

	Date createdate;

	char completionA [SIZE_MISSION_COMPLETION];				// String used to determine if this mission is done
	char completionB [SIZE_MISSION_COMPLETION];				// String used to determine if this mission is done
	char completionC [SIZE_MISSION_COMPLETION];				// String used to determine if this mission is done
	char completionD [SIZE_MISSION_COMPLETION];				// String used to determine if this mission is done
	char completionE [SIZE_MISSION_COMPLETION];				// String used to determine if this mission is done

	int payment;
	int maxpayment;						// You can negotiate this if your rating is high enough
	int paymentmethod;					// Eg all now, half now, none now

	int difficulty;
	int minuplinkrating;				// If you have this rating you can view this mission
	int acceptrating;					// If you have this rating you can auto-accept this mission

	bool npcpriority;					// NPC's will take this one first if this bool is true

	LList <char *> links;				// All useful ip addresses (to be added to links)
	BTree <char *> codes;				// Access codes, indexed on ip

public:

	Mission ();
	~Mission () override;

	void SetTYPE			( int newTYPE );
	void SetCompletion		(const string& newA, const string& newB, const string& newC, const string& newD, const string& newE );

	void SetCreateDate		( Date *date );
	void SetEmployer		(const string &newemployer );
	void SetContact			(const string &newcontact );
	void SetPayment			( int newpayment, int newmaxpayment = -1 );
	void SetDifficulty		( int newdifficulty );
	void SetMinRating		( int newrating );
	void SetAcceptRating	( int newrating );
	void SetNpcPriority		( bool newpriority );
	void SetDescription		(const string &newdescription );
	void SetDetails			(const string &newdetails );
	void SetFullDetails		(const string &newdetails );
	
	void SetWhySoMuchMoney	(const string &answer );
	void SetHowSecure		(const string &answer );
	void SetWhoIsTheTarget  (const string &answer );

	void GiveLink			(const string &ip );
	void GiveCode			(const string &ip, const string &code );

	void SetDueDate			( Date *newdate );
	Date *GetDueDate		();							// Can return NULL

	char *GetDetails		();
	char *GetFullDetails	();
	char *GetWhySoMuchMoney ();							// Can return NULL
	char *GetHowSecure		();							// Can return NULL
	char *GetWhoIsTheTarget ();							// Can return NULL

	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	
	string GetID () override;
	int   GetOBJECTID () override;

};


#endif
