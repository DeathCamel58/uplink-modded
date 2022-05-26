
/*

  Player class object

	Player ISA Agent ISA Person

  */


#ifndef included_player_h
#define included_player_h

// ============================================================================

#include "world/agent.h"
#include "world/computer/gateway.h"

// ============================================================================


class Player : public Agent
{

protected:

	BTree <char *> shares;															// Indexed on company name, values are "numowned totalpricepaid"

public:

	Gateway gateway;	

	int score_peoplefucked;
	int score_systemsfucked;
	int score_highsecurityhacks;

public:

	Player ();
	~Player () override;

	bool IsPlayerAccount   (const string &bankip, const string &accno );							// True if this account is owned by the player
	
	void GiveAllLinks ();
	void GiveMessage ( Message *message ) override;

	int NumSharesOwned	(const string &companyname );
	int SharesPricePaid (const string &companyname );
	void TradeShares	(const string &companyname, int howmany );

    static int TimeToTrace (const string &tracerIP, const string &routerIP );                     // How long for the people at tracerIP to trace through routerIP
    int TimeRemaining ();                                                          // How long remaining in this trace?

	// Common functions

	bool Load   ( FILE *file ) override;
	void Save   ( FILE *file ) override;
	void Print  () override;
	void Update () override;

	string GetID () override;
	int   GetOBJECTID () override;

};


#endif

