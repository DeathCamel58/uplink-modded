
/*

  Bank Robbery Event

    Created when the player attempts to transfer money 
    into his own bank account

    Runs roughly two minutes later

    If the player hasn't covered his tracks, its game over

  */



#ifndef included_bankrobberyevent_h
#define included_bankrobberyevent_h

#include "world/scheduler/eventscheduler.h"


class BankRobberyEvent : public UplinkEvent
{

public:

    string source_ip;
    string source_accno;
    string target_ip;
    string target_accno;
    int amount;
    Date hackdate;

public:

    BankRobberyEvent ();
    ~BankRobberyEvent () override;

	void Run () override;

    void SetDetails (const string &newsourceip, const string &newsourceaccno,
                     const string &newtargetip, const string &newtargetaccno,
                     int newamount, Date *newhackdate );

	string GetShortString () override;
	string GetLongString () override;

	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	
	string GetID () override;
	int   GetOBJECTID () override;

};


#endif
