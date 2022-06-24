
#ifndef included_computer_h
#define included_computer_h

// ============================================================================

#include "app/uplinkobject.h"

#include "world/computer/databank.h"
#include "world/computer/logbank.h"
#include "world/computer/recordbank.h"
#include "world/computer/security.h"
#include "world/company/company.h"
#include "world/vlocation.h"

#define SIZE_COMPUTER_NAME   64

// Computer types

#define COMPUTER_TYPE_UNKNOWN                 0
#define COMPUTER_TYPE_PUBLICACCESSSERVER      1
#define COMPUTER_TYPE_INTERNALSERVICESMACHINE 2
#define COMPUTER_TYPE_CENTRALMAINFRAME        4
#define COMPUTER_TYPE_PUBLICBANKSERVER		  8
#define COMPUTER_TYPE_PERSONALCOMPUTER		  16
#define COMPUTER_TYPE_VOICEPHONESYSTEM		  32
#define COMPUTER_TYPE_LAN                     64


#define COMPUTER_TYPE_ANY     COMPUTER_TYPE_PUBLICACCESSSERVER          | \
                              COMPUTER_TYPE_INTERNALSERVICESMACHINE     | \
                              COMPUTER_TYPE_CENTRALMAINFRAME            | \
                              COMPUTER_TYPE_PUBLICBANKSERVER            | \
                              COMPUTER_TYPE_PERSONALCOMPUTER            | \
                              COMPUTER_TYPE_VOICEPHONESYSTEM            | \
                              COMPUTER_TYPE_LAN


// Trace actions

#define COMPUTER_TRACEACTION_NONE				0
#define COMPUTER_TRACEACTION_DISCONNECT			1
#define COMPUTER_TRACEACTION_WARNINGMAIL		2
#define COMPUTER_TRACEACTION_FINE				4
#define COMPUTER_TRACEACTION_LEGAL				8
#define COMPUTER_TRACEACTION_TACTICAL			16
#define COMPUTER_TRACEACTION_COVERT				32


class ComputerScreen;

// ============================================================================

//! Computer class UplinkObject
/**
 * The computer class describes a computer in the uplink world. This computer belongs to one company and is located at a
 * virtual location.
 * Part of the world sub-system
 */
class Computer : public UplinkObject
{

protected:

    /**
     * The number of times this machine was recently hacked
     */
	int numrecenthacks;

	/**
	 * The number of times this machine was hacked this month
	 */
	int numhacksthismonth;

	/**
	 * The number of times this machine was hacked last month
	 */
	int numhackslastmonth;

public:

    /**
     * The type of the computer
     * @note Valid types are COMPUTER_TYPE_UNKNOWN, COMPUTER_TYPE_PUBLICACCESSSERVER, COMPUTER_TYPE_INTERNALSERVICESMACHINE, COMPUTER_TYPE_CENTRALMAINFRAME, COMPUTER_TYPE_PUBLICBANKSERVER, COMPUTER_TYPE_PERSONALCOMPUTER, COMPUTER_TYPE_VOICEPHONESYSTEM, COMPUTER_TYPE_LAN
     */
	int TYPE;

	/**
	 * The name of this machine
	 */
	string name;

	/**
	 * Name of company that owns this machine
	 */
	string companyname;

	/**
	 * IP of the computer
	 */
	string ip;

	int tracespeed;
	int traceaction;

	/**
	 * Can this computer be targeted by random events
	 */
	bool istargetable;

	/**
	 * Can this computer be connected to from outside its trusted network?
	 */
	bool isexternallyopen;
	bool isrunning;

	/**
	 * The version of revelation on this machine
	 * @note 0.0 means that revelation is not running
	 */
	float isinfected_revelation;

	/**
	 * The Date of the infection from revelation
	 */
    Date infectiondate;

	DArray <ComputerScreen *> screens;

	DataBank   databank;
	LogBank    logbank;
	RecordBank recordbank;
	Security   security;

public:

	Computer();
	~Computer() override;

	/**
	 * Sets the type of the computer
	 * @param newTYPE
	 * @note Possible values for newTYPE are: COMPUTER_TYPE_UNKNOWN, COMPUTER_TYPE_PUBLICACCESSSERVER, COMPUTER_TYPE_INTERNALSERVICESMACHINE, COMPUTER_TYPE_CENTRALMAINFRAME, COMPUTER_TYPE_PUBLICBANKSERVER, COMPUTER_TYPE_PERSONALCOMPUTER, COMPUTER_TYPE_VOICEPHONESYSTEM, COMPUTER_TYPE_LAN
	 */
	void SetTYPE        ( int newTYPE );

	/**
	 * Sets the name of the computer
	 * @param newname The new name of the computer
	 */
	void SetName		(const string &newname );

	/**
	 * Sets the company name that owns the computer
	 * @param newname The company that owns the computer
	 */
	void SetCompanyName (const string &newname );

	/**
	 * Sets the IP of the computer
	 * @param newip The new IP of the computer
	 */
	void SetIP			(const string &newip );
	void SetTraceSpeed  ( int newtracespeed );
	void SetTraceAction ( int newtraceaction );

	void SetIsTargetable		( bool value );
	void SetIsExternallyOpen	( bool value );
	void SetIsRunning			( bool value );

	void DisinfectRevelation ();
	void InfectWithRevelation ( float version );

	/**
	 * Adds a ComputerScreen to this computer
	 * @param cs The ComputerScreen to add
	 * @param index The index to add this computer screen at
	 * @return The index of the added ComputerScreen
	 */
	int				AddComputerScreen ( ComputerScreen *cs, int index = -1 );

	/**
	 * Gets a screen on this computer at an index
	 * @param index The screen index to get
	 * @return The ComputerScreen
	 */
	ComputerScreen *GetComputerScreen ( int index ) const;


	/**
	 * Checks if the machine was hacked, resets the security systems on the machine, and generates a news article for certain hacks
	 * @note Call me frequently
	 */
	void CheckForSecurityBreaches ();

	/**
	 * Deletes old logs
	 * @note Call me frequently
	 */
	void ManageOldLogs ();

	/**
	 * Changes passwords on the machine
	 * @return true if changes were made
	 */
    bool ChangeSecurityCodes ();

    /**
     * Adds a number of recent hacks to this machine
     * @param n Number of recent hacks
     * @note This should be called when the machine is hacked
     */
	void AddToRecentHacks ( int n );

	/**
	 * Updates the number of recent hacks
	 * @note This is called 4 times a month
	 * @note Reduces the number of recent hacks by the number last month / 4 (removing all of last months hacks from the count by the end of the month)
     *
	 * @note If it is the end of the month, process the changeover to a new month
	 * @note Also schedules the next check
	 */
	void UpdateRecentHacks ();

	/**
	 * Gets the number of recent hacks on this machine
	 * @return The number of recent hacks
	 */
	int  NumRecentHacks () const;

    static void GenerateAccessCode(const string &code, string &result);
    static void GenerateAccessCode(const string &name, const string &code, string &result);
    static void GenerateAccessCode(int accNo, const string &code, string &result);

	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	void Update () override;

	string GetID () override;
	int   GetOBJECTID () override;

};

#endif 
