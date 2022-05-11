
/*

  Bank Robbery Event

    Created when the player attempts to transfer money 
    into his own bank account

    Runs roughly two minutes later

    If the player hasn't covered his tracks, its game over

  */



#ifndef _included_bankrobberyevent_h
#define _included_bankrobberyevent_h

#include "world/scheduler/eventscheduler.h"


class BankRobberyEvent : public UplinkEvent
{

public:

    char source_ip      [128];
    char source_accno   [128];
    char target_ip      [128];
    char target_accno   [128];
    int amount;
    Date hackdate;

public:

    BankRobberyEvent ();
    ~BankRobberyEvent () override;

	void Run () override;

    void SetDetails (const string &newsourceip, const string &newsourceaccno,
                     const string &newtargetip, const string &newtargetaccno,
                     int newamount, Date *newhackdate );

	char *GetShortString () override;
	char *GetLongString () override;

	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	
	string GetID () override;
	int   GetOBJECTID () override;

};


#endif
