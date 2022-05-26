
#ifndef included_bankcomputer_h
#define included_bankcomputer_h


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
	int CreateBankAccount (const string &name, const string &password, int security,		//	number assigned
							 int balance, int loan );

    static void CloseAccount ( int accno );

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

