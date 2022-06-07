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
	
	subject = body = "";

	SetTo ( " " );
	SetFrom ( " " );
	SetSubject ( " " );
	SetBody ( " " );

	data = nullptr;

}
 
Message::~Message()
{

	subject = body = "";
	delete data;

    DeleteLListData ( links );
    DeleteBTreeData ( &codes );

}

void Message::SetTo	(const string &newto )
{

	to = newto;

}

void Message::SetFrom (const string &newfrom )
{

	from = newfrom;

}

void Message::SetSubject (const string &newsubject )
{

	subject = newsubject;

}

void Message::SetBody (const string &newbody )
{

    body = newbody;

}

void Message::SetDate ( Date *newdate )
{

	date.SetDate ( newdate );

}

void Message::GiveLink (const string &ip )
{

	links.PutData (ip);

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

string Message::GetSubject ()
{

	assert( !subject.empty() );
	return subject;

}

string Message::GetBody ()
{

	assert( !body.empty() );
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

	if ( !LoadDynamicStringInt ( to, file ) ) return false;
	if ( !LoadDynamicStringInt ( from, file ) ) return false;

	if ( !LoadDynamicStringInt( subject, file ) ) return false;
	if ( !LoadDynamicStringInt( body, file ) ) return false;

	if ( !date.Load ( file ) ) return false;

	if ( !LoadLList ( links, file ) ) return false;
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

	SaveLList ( links, file );
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

	PrintLList ( links );
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
