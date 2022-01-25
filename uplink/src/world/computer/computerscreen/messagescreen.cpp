// MessageScreen.cpp: implementation of the MessageScreen class.
//
//////////////////////////////////////////////////////////////////////

#include "app/globals.h"
#include "app/serialise.h"

#include "world/computer/computerscreen/messagescreen.h"

#include "mmgr.h"

MessageScreen::MessageScreen()
{
	
	nextpage = -1;
	textmessage = nullptr;
	buttonmessage = nullptr;
	mailthistome = false;

}

MessageScreen::~MessageScreen()
{
	
	delete [] textmessage;
	delete [] buttonmessage;

}

void MessageScreen::SetNextPage ( int newnextpage )
{

	nextpage = newnextpage;

}

void MessageScreen::SetTextMessage ( char *newtextmessage )
{

	delete [] textmessage;
	textmessage = new char [strlen(newtextmessage)+1];
	UplinkSafeStrcpy ( textmessage, newtextmessage )

}

void MessageScreen::SetButtonMessage ( char *newbuttonmessage )
{

	delete [] buttonmessage;
	buttonmessage = new char [strlen(newbuttonmessage)+1];
	UplinkSafeStrcpy ( buttonmessage, newbuttonmessage )

}

void MessageScreen::SetMailThisToMe ( bool newvalue )
{

	mailthistome = newvalue;

}

bool MessageScreen::Load  ( FILE *file )
{

	LoadID ( file );

	if ( !ComputerScreen::Load ( file ) ) return false;

	if ( !FileReadData ( &nextpage, sizeof(nextpage), 1, file ) ) return false;	
	if ( !LoadDynamicStringPtr ( &textmessage, file ) ) return false;
	if ( !LoadDynamicStringPtr ( &buttonmessage, file ) ) return false;
	if ( !FileReadData ( &mailthistome, sizeof(mailthistome), 1, file ) ) return false;

	LoadID_END ( file );

	return true;

}

void MessageScreen::Save  ( FILE *file )
{

	SaveID ( file );

	ComputerScreen::Save ( file );

	fwrite ( &nextpage, sizeof(nextpage), 1, file );
	SaveDynamicString ( textmessage, file );
	SaveDynamicString ( buttonmessage, file );
	fwrite (  &mailthistome, sizeof(mailthistome), 1, file );

	SaveID_END ( file );

}

void MessageScreen::Print ()
{

	printf ( "MessageScreen : \n" );
	ComputerScreen::Print ();
	printf ( "NextPage = %d, TextMessage = %s, ButtonMessage = %s, MailThisToMe = %d\n", 
				nextpage, textmessage, buttonmessage, mailthistome );

}
	
string MessageScreen::GetID ()
{

	return "SCR_MESS";

}

int MessageScreen::GetOBJECTID ()
{

	return OID_MESSAGESCREEN;

}
			

