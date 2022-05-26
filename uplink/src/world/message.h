
/*

  Message class object

	Stores the data representing a message (system message, email)

  */


#ifndef included_message_h
#define included_message_h

// ============================================================================

#include "app/uplinkobject.h"

#include "world/person.h"
#include "world/date.h"

class Data;

// ============================================================================


class Message : public UplinkObject  
{

protected:

	char *subject;
	char *body;

	Data *data;							// An attached file/program

public:

	char to		 [SIZE_PERSON_NAME];
	char from    [SIZE_PERSON_NAME];

	LList <char *> links;				// All useful ip addresses (to be added to links)
	BTree <char *> codes;				// Access codes, indexed on ip

	Date date;							// Date of send

public:

	Message();
	~Message() override;

	void SetTo		(const string &newto );
	void SetFrom	(const string &newfrom );
	void SetSubject (const string &newsubject );
	void SetBody	(const string &newbody );
	void SetDate    ( Date *newdate );

	void GiveLink (const string &ip );
	void GiveCode (const string &ip, const string &code );

	void AttachData ( Data *newdata );
	Data *GetData ();

	char *GetSubject ();
	char *GetBody	 ();

	void Send ();


	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	
	string GetID () override;
	int   GetOBJECTID () override;

};

#endif
