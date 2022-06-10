
#include <strstream>

#include "app/globals.h"
#include "app/serialise.h"

#include "world/scheduler/warningevent.h"

#include "mmgr.h"


WarningEvent::WarningEvent ()
{

	event = nullptr;

}

WarningEvent::~WarningEvent ()
= default;


void WarningEvent::SetEvent ( UplinkEvent *newevent )
{

	event = newevent;

}


void WarningEvent::Run ()
{

	if ( event )		event->RunWarning ();

}

string WarningEvent::GetShortString ()
{

	return "Warning Event";

}

string WarningEvent::GetLongString ()
{

	return "Warning Event for :\n" + event->GetLongString ();

}

bool WarningEvent::Load  ( FILE *file )
{

	LoadID ( file );

	event = nullptr;

	if  ( !UplinkEvent::Load ( file ) ) return false;

	bool eventexists;
	if ( !FileReadData ( &eventexists, sizeof(eventexists), 1, file ) ) return false;

	if ( eventexists ) {
		int objectID;
		if ( !FileReadData ( &objectID, sizeof(objectID), 1, file ) ) return false;
		event = (UplinkEvent *) CreateUplinkObject ( objectID );
		if ( !event ) return false;
		if ( !event->Load ( file ) ) return false;
	}

	LoadID_END ( file );

	return true;

}

void WarningEvent::Save  ( FILE *file )
{

	SaveID ( file );

	UplinkEvent::Save ( file );

	bool eventexists = ( event != nullptr );
	fwrite ( &eventexists, sizeof(eventexists), 1, file );

	if ( eventexists ) {
		int objectID = event->GetOBJECTID ();
		fwrite ( &objectID, sizeof(objectID), 1, file );
		event->Save ( file );
	}

	SaveID_END ( file );

}

void WarningEvent::Print ()
{

	cout << "Warning Event : " << endl;

	UplinkEvent::Print ();

	if ( event ) event->Print ();

}

string WarningEvent::GetID ()
{

	return "EVT_WARN";

}


int   WarningEvent::GetOBJECTID ()
{

	return OID_WARNINGEVENT;

}
