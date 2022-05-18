// Message.cpp: implementation of the Message class.
//
//////////////////////////////////////////////////////////////////////

#include <app/miscutils.h>

#include "game/game.h"

#include "app/globals.h"
#include "app/serialise.h"

#include "world/world.h"
#include "world/message.h"
#include "world/computer/databank.h"

#include "mmgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Message::Message()
{
	
	subject = nullptr;
	body = nullptr;

	SetTo ( " " );
	SetFrom ( " " );
	SetSubject ( " " );
	SetBody ( " " );

	data = nullptr;

}
 
Message::~Message()
{

	delete [] subject;
	delete [] body;
	delete data;

    DeleteLListData ( &links );
    DeleteBTreeData ( &codes );

}

void Message::SetTo	(const string &newto )
{

	assert( newto.length() < SIZE_PERSON_NAME );
	UplinkStrncpy ( to, newto.c_str(), sizeof ( to ) )

}

void Message::SetFrom (const string &newfrom )
{

	assert( newfrom.length() < SIZE_PERSON_NAME );
	UplinkStrncpy ( from, newfrom.c_str(), sizeof ( from ) )

}

void Message::SetSubject (const string &newsubject )
{

	delete [] subject;
	subject = new char [ newsubject.length() + 1 ];
	UplinkSafeStrcpy ( subject, newsubject.c_str() )

}

void Message::SetBody (const string &newbody )
{

	delete [] body;
	body = new char [ newbody.length() + 1 ];
	UplinkSafeStrcpy ( body, newbody.c_str() )

}

void Message::SetDate ( Date *newdate )
{

	date.SetDate ( newdate );

}

void Message::GiveLink (const string &ip )
{

	size_t theipsize = SIZE_VLOCATION_IP;
	char *theip = new char [theipsize];
	assert(ip.length() < SIZE_VLOCATION_IP );
	UplinkStrncpy ( theip, ip.c_str(), theipsize )
	links.PutData (theip);

}

void Message::GiveCode (const string &ip, const string &code )
{

	char *thecode = new char [code.length()+1];
	UplinkSafeStrcpy ( thecode, code.c_str() )
    codes.PutData ( ip, thecode );

}

void Message::AttachData ( Data *newdata )
{

	delete data;
	// Copy newdata
	data = new Data ( newdata );	

}

Data *Message::GetData ()
{

	return data;

}

char *Message::GetSubject ()
{

	UplinkAssert ( subject )
	return subject;

}

char *Message::GetBody ()
{

	UplinkAssert ( body )
	return body;

}

void Message::Send ()
{

	SetDate ( &(game->GetWorld ()->date) );

	Person *person = game->GetWorld ()->GetPerson ( to );

	if ( person ) {

		person->GiveMessage ( this );

	}
	else {

		// Send an error message to the sender
		auto *msg = new Message ();
		msg->SetTo ( from );
		msg->SetFrom ( "MAIL SERVER" );
		msg->SetSubject ( "Failed to deliver this email - unknown recipient" );
		msg->SetBody ( body );
		msg->Send ();

	}

}

bool Message::Load ( FILE *file )
{

	LoadID ( file );

	if ( !LoadDynamicStringStatic ( to, SIZE_PERSON_NAME, file ) ) return false;
	if ( !LoadDynamicStringStatic ( from, SIZE_PERSON_NAME, file ) ) return false;

	if ( subject ) {
		delete [] subject;
		subject = nullptr;
	}
	if ( !LoadDynamicStringPtr ( &subject, file ) ) return false;
	if ( body ) {
		delete [] body;
		body = nullptr;
	}
	if ( !LoadDynamicStringPtr ( &body, file ) ) return false;

	if ( !date.Load ( file ) ) return false;

	if ( !LoadLList ( &links, file ) ) return false;
	if ( !LoadBTree ( &codes, file ) ) return false;

	LoadID_END ( file );

	return true;

}

void Message::Save ( FILE *file )
{

	SaveID ( file );

	SaveDynamicString ( to, SIZE_PERSON_NAME, file );
	SaveDynamicString ( from, SIZE_PERSON_NAME, file );

	SaveDynamicString ( subject, file );
	SaveDynamicString ( body, file );

	date.Save ( file );

	SaveLList ( &links, file );
	SaveBTree ( &codes, file );

	SaveID_END ( file );

}

void Message::Print ()
{

    cout << "Message : " << endl;
    PrintValue("To", to);
    PrintValue("From", from);
    PrintValue("Subject", subject);
    PrintValue("Body", body);

	PrintLList ( &links );
	PrintBTree ( &codes );

	date.Print ();

}
	
string Message::GetID ()
{

	return "MESSAGE";

}

int Message::GetOBJECTID ()
{

	return OID_MESSAGE;

}
