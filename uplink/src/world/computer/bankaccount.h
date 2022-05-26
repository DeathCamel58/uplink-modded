
/*

  Bank Account class object

	A specific version of Company	
	Part of the World Sub-system

  */


#ifndef included_bankaccount_h
#define included_bankaccount_h


#include "app/uplinkobject.h"

#include "world/person.h"
#include "world/computer/logbank.h"

class Person;


class BankAccount : public UplinkObject
{

public:
	
	char name [SIZE_PERSON_NAME];
	char password [64];
	int security;
	int accountnumber;
	int balance;
	int loan;

	LogBank log;

public:

	BankAccount ();
	~BankAccount () override;

	void SetOwner		 (const string &newname );
	void SetSecurity	 (const string &newpassword, int newsecurity );
	void SetAccNumber	 ( int newaccnumber );	
	void SetBalance		 ( int newbalance, int newloan );

	void ChangeBalance	 (int amount, const string &description = "" );
	
	// Has this account sent money to target account
	// If partial is true,  it return true if there is a log of the transaction on the target _or_ source account
	// If partial is false, it return true if there is a log of the transaction on the target _and_ source account
	bool HasTransferOccurred (char *s_ip, char *t_ip, int t_accno, int amount, bool partial = false ) const;
	bool HasTransferOccurred (char *person, int amount ) const;								// Has this account received payment from person

	Person *GetPerson ();

	static BankAccount *GetAccount (const string &bankip, const string &accno );
	static bool	TransferMoney (const string &source_ip, const string &source_accno,
                                  const string &target_ip, const string &target_accno,
                                  int amount, Person *person );

	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	
	string GetID () override;
	int   GetOBJECTID () override;

};

#endif 
