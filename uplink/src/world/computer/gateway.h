
/*

  Gateway class object

	Represents the player's Gateway computer

  */


#ifndef _included_gateway_h
#define _included_gateway_h

// ============================================================================

#include "app/uplinkobject.h"

#include "world/computer/databank.h"
#include "world/computer/gatewaydef.h"


#define HUDUPGRADE_MAPSHOWSTRACE			1
#define HUDUPGRADE_CONNECTIONANALYSIS		2
#define HUDUPGRADE_IRCCLIENT                4
#define HUDUPGRADE_LANVIEW                  8

#define GATEWAYUPGRADETYPE_CPU				1
#define GATEWAYUPGRADETYPE_MODEM			2
#define GATEWAYUPGRADETYPE_COOLING			3
#define GATEWAYUPGRADETYPE_MEMORY			4
#define GATEWAYUPGRADETYPE_SECURITY			5
#define GATEWAYUPGRADETYPE_POWER			6

// ============================================================================


class Gateway : public UplinkObject
{

protected:

	unsigned char hudupgrades;						// Bitfield - see above

	void SetGatewayStart ();

public:

	DataBank databank;

	GatewayDef* curgatewaydef;				// GatewayDef of the current Gateway

	int id;									// Unique ID number
    GatewayDef* newgatewaydef;				// New gateway recently purchased, awaiting log out.  Or NULL.
	bool nuked;								// Has the player blown it up?
	int proximity;							// Number of people nearby
	
	char modemtype [64];

	int modemspeed;
	int memorysize;

	LList <char *> hardware;				// Names of all hardware items owned

public:

	Gateway ();
	~Gateway () override;

	void GenerateNewID ();					// Call when gateway is changed
	void Nuke ();							// Call when gateway is nuked
	
	void IncrementProximity ( int n = 1 );				// Someone approaches
	void DecrementProximity ( int n = 1 );				// Someone leaves
	

	void SetGateway		  ( GatewayDef *newgd );
	void SetModemType     (const string& newmodem, int newmodemspeed );
	void SetMemorySize    ( int newsize );
	void ChangeMemorySize ( int amount );

	void ExchangeGateway ( GatewayDef *newgd );		// Call this when the gateway upgrade is ready
	void ExchangeGatewayComplete ();					// Does the actual upgrade

	void GiveHUDUpgrade ( char upgrade );
	bool HasHUDUpgrade  ( char upgrade ) const;
	char GetHudUpgrades () const;								// Returns the bitfield - be careful!
	bool HasAnyHUDUpgrade () const;

	void GiveCPU (const string& CPUName );
	int GetNumCPUs ();
	int GetCPUSpeed ();
	LList <string> *GetCPUs ();

	int GetBandwidth () const;								// Returns min ( modemspeed, gatewaydef.bandwidth )

	int GetNumSecurity ();
	LList <string> *GetSecurity ();

	void GiveStartingHardware ();
	void GiveHardware  ( char *newhardware );
	bool IsHWInstalled (string name );

	void GiveStartingSoftware ();

	void GiveAllSoftware ();
	void GiveAllHardware ();

	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	void Update () override;
	
	string GetID () override;

};



#endif

