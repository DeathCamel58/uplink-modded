
#ifndef included_consequencegenerator_h
#define included_consequencegenerator_h

// ============================================================================

#include "eclipse.h"

class Person;
class Computer;
class Mission;
class AccessLog;

// ============================================================================

//! Consequence Generator
/**
 * This is responsible for analysing an action that has happened, and generating subsequence "consequence" actions.
 *
 * Example action: A user is caught performing a hack
 * Example consequence: User's Uplink rating is decreased, and they receive a fine
 *
 * @note This class should operate in DATA only. It shouldn't be handling screen flashes, interface changes, etc.
 * Ideally, it should stick to the world module.
 */
class ConsequenceGenerator
{

public:

	static void Initialise ();

    /** @name Event processing for all people
     */
    ///@{

	/**
	 * Processes an event where the user was caught hacking. This adds the crime to the criminal record, decreases their
	 * rating, and triggers the consequence.
	 * @param person Person who was caught
	 * @param comp Computer that was hacked
	 */
	static void CaughtHacking ( Person *person, Computer *comp );

	/**
	 * Creates a news story about the arrest
	 * @param person Person who was arrested
	 * @param comp Computer that was hacked
	 * @param reason Reason they were arrested
	 * @note In future versions: Generate news stories, vendetta's, etc. from this
	 */
	static void Arrested	  (Person *person, Computer *comp, const string& reason );

	/**
	 * Schedules an arrest event or SeizeGateway event
	 * @param person Person who didn't pa the file
	 * @param fine The fine Mission
	 */
	static void DidntPayFine  ( Person *person, Mission *fine  );

	/**
	 * Does nothing
	 * @param person Person who was shot by the feds
	 * @param reason
	 * @note Do something here - news stories, etc.
	 */
	static void ShotByFeds	  (Person *person, const string& reason );

	/**
	 * Does nothing.
	 * @param person Person whose gateway has been seized
	 * @param reason
	 * @note reason is unused
	 * @note In future versions: This person is now disavowed. They may come back as a vengeful new agent.
	 */
	static void SeizeGateway  (Person *person, const string& reason );

	///@}
    /** @name Event processing for company computers
     */
    ///@{

	/**
	 * Checks if computer has been hacked, and sometimes generates a trace hacker mission
	 * @param comp Computer to check for hacks
	 * @param al AccessLog of hack
	 */
	static void ComputerHacked ( Computer *comp, AccessLog *al );

	///@}
    /** @name Event processing for completed missions
     */
    ///@{

	/**
	 * Checks if the Mission has been completed
	 * @param mission The Mission to check completion of
	 * @param person The Person who is working on the Mission
	 */
	static void MissionCompleted				( Mission *mission, Person *person );

    /**
     * Checks if the Mission has been completed
     * @param mission The Mission to check completion of
     * @param person The Person who is working on the Mission
     */
	static void MissionCompleted_StealFile		( Mission *mission, Person *person );

    /**
     * Checks if the Mission has been completed
     * @param mission The Mission to check completion of
     * @param person The Person who is working on the Mission
     */
	static void MissionCompleted_DestroyFile	( Mission *mission, Person *person );

    /**
     * Checks if the Mission has been completed
     * @param mission The Mission to check completion of
     * @param person The Person who is working on the Mission
     */
	static void MissionCompleted_ChangeAccount  ( Mission *mission, Person *person );

    /**
     * Checks if the Mission has been completed
     * @param mission The Mission to check completion of
     * @param person The Person who is working on the Mission
     */
	static void MissionCompleted_TraceUser		( Mission *mission, Person *person );

	///@}

};



#endif

