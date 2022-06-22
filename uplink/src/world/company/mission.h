
#ifndef included_mission_h
#define included_mission_h

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

//! Mission UplinkObject
/**
 * Part of the World sub-system
 * A representation of a mission or task to be completed
 */
class Mission : public UplinkObject
{

protected:

    /**
     * The details listed on BBS boards
     */
	char *details;

	/**
	 * The full details listed after acceptance
	 * @note This contains actual target details
	 */
	char *fulldetails;

	/**
	 * The Date for when the mission is due
	 * @note This can be nullptr
	 */
	Date *duedate;

	/**
	 * Answer for asking why so much money
     * @note This can be nullptr
	 */
	char *whysomuchmoney;

    /**
     * Answer for asking how secure the target is
     * @note This can be nullptr
     */
	char *howsecure;

    /**
     * Answer for asking who is the target
     * @note This can be nullptr
     */
	char *whoisthetarget;

public:

	int TYPE;												// Defined in MissionGenerator

	/**
	 * The employer
	 */
	char employer    [SIZE_MISSION_EMPLOYER];

	/**
	 * The contact
	 */
	char contact     [SIZE_PERSON_NAME];

	/**
	 * The description
	 */
	char description [SIZE_MISSION_DESCRIPTION];

	/**
	 * Date of the creation for this mission
	 */
	Date createdate;

	char completionA [SIZE_MISSION_COMPLETION];				// String used to determine if this mission is done
	char completionB [SIZE_MISSION_COMPLETION];				// String used to determine if this mission is done
	char completionC [SIZE_MISSION_COMPLETION];				// String used to determine if this mission is done
	char completionD [SIZE_MISSION_COMPLETION];				// String used to determine if this mission is done
	char completionE [SIZE_MISSION_COMPLETION];				// String used to determine if this mission is done

	/**
	 * The default payment amount
	 */
	int payment;

	/**
	 * The maximum negotiable payment
	 */
	int maxpayment;
	int paymentmethod;					// Eg all now, half now, none now

	int difficulty;
	int minuplinkrating;				// If you have this rating you can view this mission
	int acceptrating;					// If you have this rating you can auto-accept this mission

	bool npcpriority;					// NPC's will take this one first if this bool is true

	LList <string> links;				// All useful ip addresses (to be added to links)
	BTree <char *> codes;				// Access codes, indexed on ip

public:

	Mission ();
	~Mission () override;

    /** @name Setting Mission values
     */
    ///@{

	/**
	 * Sets the type of the Mission
	 * @param newTYPE New type for Mission
	 */
	void SetTYPE			( int newTYPE );

	/**
	 * Set the completion strings for the Mission
	 * @param newA The first completion string
	 * @param newB The second completion string
	 * @param newC The third completion string
	 * @param newD The fourth completion string
	 * @param newE The fifth completion string
	 */
	void SetCompletion		(const string& newA, const string& newB, const string& newC, const string& newD, const string& newE );

	/**
	 * Sets the Date of the Mission
	 * @param date New Date of Mission
	 */
	void SetCreateDate		( Date *date );

	/**
	 * Sets the employer that made the Mission
	 * @param newemployer New employer for the mission
	 */
	void SetEmployer		(const string &newemployer );

	/**
	 * Sets the contact for the mission
	 * @param newcontact New contact for the mission
	 */
	void SetContact			(const string &newcontact );

	/**
	 * Sets the payment for the Mission
	 * @param newpayment The payment for the Mission
	 * @param newmaxpayment The maximum negotiable payment
	 */
	void SetPayment			( int newpayment, int newmaxpayment = -1 );

	/**
	 * Sets the difficulty of the Mission
	 * @param newdifficulty New difficulty for the Mission
	 */
	void SetDifficulty		( int newdifficulty );

	/**
	 * Sets the minimum rating for an Agent to see this Mission
	 * @param newrating New minimum rating
	 */
	void SetMinRating		( int newrating );

	/**
	 * Sets the minimum automatic acceptance rating for an Agent to get this Mission
	 * @param newrating New minimum rating
	 */
	void SetAcceptRating	( int newrating );

	/**
	 * Sets if NPCs will take this Mission first
	 * @param newpriority true if NPCs should take this first
	 */
	void SetNpcPriority		( bool newpriority );

    /**
     * Sets the description for the Mission
     * @param newdescription New description
     */
	void SetDescription		(const string &newdescription );

    /**
     * Sets the details for the Mission
     * @param newdetails New details
     */
	void SetDetails			(const string &newdetails );

	/**
	 * Sets the full details for the Mission
	 * @param newdetails New full details
	 */
	void SetFullDetails		(const string &newdetails );

	/**
	 * Sets the answer for the question "Why so much money"
	 * @param answer The answer for the question
	 */
	void SetWhySoMuchMoney	(const string &answer );

    /**
     * Sets the answer for the question "How secure is the target"
     * @param answer The answer for the question
     */
	void SetHowSecure		(const string &answer );

    /**
     * Sets the answer for the question "Who is the target"
     * @param answer The answer for the question
     */
	void SetWhoIsTheTarget  (const string &answer );

	/**
	 * Adds ip to the list of links the Agent who accepts this Mission will receive
	 * @param ip The ip to add to the list of links
	 */
	void GiveLink			(const string &ip );

	/**
	 * Adds access code for Computer to list of codes the Agent who accepts this Mission will receive
	 * @param ip The ip of the Computer
	 * @param code The code for the Computer
	 */
	void GiveCode			(const string &ip, const string &code );

	/**
	 * Sets the due Date of the Mission
	 * @param newdate The due Date
	 */
	void SetDueDate			( Date *newdate );

	///@}
    /** @name Getting Mission values
     */
    ///@{

    /**
     * Gets the due Date of the Mission
     * @note Can be nullptr
     */
	Date *GetDueDate		();

	/**
	 * Gets the details of the Mission
	 * @return
	 */
	char *GetDetails		();

	/**
	 * Gets the full details of the Mission
	 * @return The full details
	 */
	char *GetFullDetails	();

    /**
     * Get the answer to "Why so much money"
     * @return The answer, or nullptr
     */
	char *GetWhySoMuchMoney ();

    /**
     * Get the answer to "How secure is the target"
     * @return The answer, or nullptr
     */
	char *GetHowSecure		();

	/**
	 * Get the answer to "Who is the target"
	 * @return The answer, or nullptr
	 */
	char *GetWhoIsTheTarget ();

	///@}

	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	
	string GetID () override;
	int   GetOBJECTID () override;

};


#endif
