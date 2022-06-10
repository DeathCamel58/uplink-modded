
#include <strstream>
#include <app/miscutils.h>

#include "app/serialise.h"
#include "app/globals.h"

#include "game/game.h"

#include "world/world.h"
#include "world/scheduler/shotbyfedsevent.h"
#include "world/generator/consequencegenerator.h"

#include "mmgr.h"


ShotByFedsEvent::ShotByFedsEvent ()
{

	reason = "";
	name = "";

}

ShotByFedsEvent::~ShotByFedsEvent ()
= default;

void ShotByFedsEvent::SetName (const string &newname )
{

	name = newname;

}

void ShotByFedsEvent::SetReason (const string &newreason )
{

	reason = newreason;

}

void ShotByFedsEvent::Run ()
{

	Person *person = game->GetWorld ()->GetPerson ( name );
	UplinkAssert (person)

	person->SetStatus ( PERSON_STATUS_DEAD );

	ConsequenceGenerator::ShotByFeds ( person, reason );

}

string ShotByFedsEvent::GetShortString ()
{

	return "ShotByFeds for " + name;

}

string ShotByFedsEvent::GetLongString ()
{

	return "Shot By Feds Event\nFor : " + name + "\nReason : " + reason;

}

bool ShotByFedsEvent::Load  ( FILE *file )
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

	LoadID_END ( file );

	return true;

}

void ShotByFedsEvent::Save  ( FILE *file )
{

	SaveID ( file );

	UplinkEvent::Save ( file );

	SaveDynamicString ( name, SIZE_PERSON_NAME, file );
	SaveDynamicString ( reason, file );

	SaveID_END ( file );

}

void ShotByFedsEvent::Print ()
{

    cout << "ShotByFedsEvent : " << endl;
    PrintValue("Name", name);
	UplinkEvent::Print ();
    PrintValue("Reason", reason);

}

string ShotByFedsEvent::GetID ()
{

	return "EVT_SHOT";

}

int   ShotByFedsEvent::GetOBJECTID ()
{

	return OID_SHOTBYFEDSEVENT;

}
