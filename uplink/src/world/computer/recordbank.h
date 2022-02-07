

#ifndef _included_recordbank_h
#define _included_recordbank_h

/*

	Record Bank class

	Represents a bank of computer records
	ie a Database

  */

#include "app/uplinkobject.h"

class Record;

/*

  OP CODES

	=			:		Equals
	!			:		Not Equal

	+			:		Contains
	-			:		Does not contain


  */

#define RECORDBANK_NAME         "Name"
#define RECORDBANK_PASSWORD     "Password"
#define RECORDBANK_ACCNO        "Account Number"
#define RECORDBANK_SECURITY     "Security"
#define RECORDBANK_ADMIN        "admin"
#define RECORDBANK_READWRITE    "readwrite"
#define RECORDBANK_READONLY     "readonly"


class RecordBank : public UplinkObject
{

public:

	LList <Record *> records;

public:

	RecordBank ();
	~RecordBank () override;

	// Data access functions

	void AddRecord ( Record *newrecord );
	void AddRecordSorted ( Record *newrecord, char *sortfield = RECORDBANK_NAME );

	Record *GetRecord			  ( int index );							// Returns NULL if not found
	Record *GetRecord			  (const string &query );							// Assumes there is only 1 match
	LList <Record *> *GetRecords  (const string &query );
	Record *GetRandomRecord		  (const string &query );
    
    Record *GetRecordFromName           (const string &name );
    Record *GetRecordFromNamePassword   (const string &name, const string &password );
    Record *GetRecordFromAccountNumber  (const string &accNo );

	int FindNextRecordIndexNameNotSystemAccount ( int curindex = -1);


	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	void Update () override;
	
	string GetID () override;
	int GetOBJECTID () override;

private:
	static string MakeSafeField(const string &fieldval );

};


// ============================================================================

/*
	Computer Record class
	Eg. A criminal record, a bank account, an uplink account etc

	  */


class Record : public UplinkObject  
{

public:

	BTree <char *> fields;

public:

	Record();
	~Record() override;

	void AddField    (const string &name, const string &value );
	void AddField    (const string &name, int value );
	
	void ChangeField (const string &name, const string &newvalue );
	void ChangeField (const string &name, int newvalue );

    char *GetField   (const string &name );
	void DeleteField (const string &name );

	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	void Update () override;
	
	string GetID () override;
	int GetOBJECTID () override;

};

#endif 
