
#ifndef _included_bankcomputer_h
#define _included_bankcomputer_h


#include "world/computer/computer.h"
#include "world/computer/bankaccount.h"


class BankComputer : public Computer
{

protected:

	int previousaccno;

public:

	BTree <BankAccount *> accounts;

public:

	BankComputer ();
	~BankComputer () override;

	int GenerateUniqueAccountNumber ();

	int CreateBankAccount ( BankAccount *newaccount );						//  These return the account 
	int CreateBankAccount ( char *name, char *password, int security,		//	number assigned
							 int balance, int loan );

    void CloseAccount ( int accno );

	BankAccount *GetRandomAccount ();

	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	void Update () override;
	
	string GetID () override;
	int   GetOBJECTID () override;

};


#endif

