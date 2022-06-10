
#include <strstream>
#include <app/miscutils.h>

#include "soundgarden.h"

#include "app/globals.h"
#include "app/serialise.h"

#include "game/game.h"

#include "world/world.h"
#include "world/vlocation.h"
#include "world/generator/consequencegenerator.h"
#include "world/scheduler/arrestevent.h"

#include "mmgr.h"



ArrestEvent::ArrestEvent ()
{

	reason = "";
	ip = nullptr;
	name = "";

}

ArrestEvent::~ArrestEvent ()
= default;

void ArrestEvent::SetName (const string &newname )
{

	name = newname;

}

void ArrestEvent::SetReason (const string &newreason )
{

	reason = newreason;

}

void ArrestEvent::SetIP (const string &newip )
{

	delete [] ip;
	ip = new char [newip.length()+1];
	UplinkSafeStrcpy ( ip, newip.c_str() )

}

void ArrestEvent::Run ()
{

	Person *person = game->GetWorld ()->GetPerson ( name );
	UplinkAssert (person)

	person->SetStatus ( PERSON_STATUS_INJAIL );

	// Look up the computer they messed with

	Computer *comp = nullptr;

	if ( ip ) {
		VLocation *vl = game->GetWorld ()->GetVLocation ( ip );
		UplinkAssert (vl)
		comp = vl->GetComputer ();
		UplinkAssert (comp)
	}

	ConsequenceGenerator::Arrested ( person, comp, reason );

}

string ArrestEvent::GetShortString ()
{

	return "Arrest for " + name;
}

string ArrestEvent::GetLongString ()
{

	return "ArrestEvent:\nFor : " + name + "\nReason : " + reason;

}


bool ArrestEvent::Load  ( FILE *file )
{

	LoadID ( file );

	if ( !UplinkEvent::Load ( file ) ) return false;

	if ( strcmp( game->GetLoadedSavefileVer(), "SAV59" ) >= 0 ) {
		if ( !LoadDynamicStringInt ( name, file ) ) return false;
	}
	else {
		if ( !FileReadDataInt ( name, SIZE_PERSON_NAME, file ) ) {
			return false;
		}
	}

	if ( !LoadDynamicStringInt ( reason, file ) ) return false;
	if ( !LoadDynamicStringPtr ( &ip, file) ) return false;

	LoadID_END ( file );

	return true;

}

void ArrestEvent::Save  ( FILE *file )
{

	SaveID ( file );

	UplinkEvent::Save ( file );

	SaveDynamicString ( name, SIZE_PERSON_NAME, file );

	SaveDynamicString ( reason, file );
	SaveDynamicString ( ip, file );

	SaveID_END ( file );

}

void ArrestEvent::Print ()
{

	cout << "Arrest Event : " << endl;
	PrintValue("Name", name);
	UplinkEvent::Print ();

	PrintValue("Reason", reason);
	if ( ip ) PrintValue ( "IP", ip );


}

string ArrestEvent::GetID ()
{

	return "EVT_ARR";

}

int   ArrestEvent::GetOBJECTID ()
{

	return OID_ARRESTEVENT;

}

