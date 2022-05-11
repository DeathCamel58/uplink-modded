
/*

	Record Generator Class

  Responsible for generating records for a person - 
  such as criminal records, social security etc.

	*/

#ifndef _included_recordgenerator_h
#define _included_recordgenerator_h

class Record;
class BankAccount;


class RecordGenerator
{

public:

	static void Initialise ();

	// Top level functions for generating an entire set of records for a person

	static void GenerateRecords (const string &personname );
	static void GenerateRecords_Player (const string &playername );

	// Low level functions for generating specific records
	
	static void GenerateRecord_SocialSecurity	(const string &personname, int age );
	static void GenerateRecord_Criminal			(const string &personname, int age );
	static void GenerateRecord_Medical			(const string &personname, int age );
	static void GenerateRecord_Academic			(const string &personname, int age );
	static void GenerateRecord_Financial		(const string &personname, int age );
	
	// Access methods for looking up specific records
	// All return the first record found for that name
	// All return NULL on failure

	static Record *GetSocialSecurity			( char *personname );
	static Record *GetCriminal					(const string &personname );
	static Record *GetMedical					( char *personname );
	static Record *GetAcademic					( char *personname );
	static BankAccount *GetFinancial			(const string &personname );

};


#endif
