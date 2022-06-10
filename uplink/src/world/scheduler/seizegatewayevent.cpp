
#include <strstream>
#include <app/miscutils.h>

#include "soundgarden.h"

#include "app/globals.h"
#include "app/serialise.h"

#include "game/game.h"
#include "game/data/data.h"

#include "world/world.h"
#include "world/player.h"
#include "world/message.h"
#include "world/generator/consequencegenerator.h"
#include "world/scheduler/seizegatewayevent.h"

#include "mmgr.h"



SeizeGatewayEvent::SeizeGatewayEvent ()
{

	reason = "";
	gateway_id = 0;
	name = "";

}

SeizeGatewayEvent::~SeizeGatewayEvent ()
= default;

void SeizeGatewayEvent::SetName (const string &newname )
{

	name = newname;

}

void SeizeGatewayEvent::SetReason (const string &newreason )
{

    reason = newreason;

}

void SeizeGatewayEvent::SetGatewayID ( int newid )
{

	gateway_id = newid;

}

void SeizeGatewayEvent::Run ()
{

	Person *person = game->GetWorld ()->GetPerson ( name );
	UplinkAssert (person)

	ConsequenceGenerator::SeizeGateway ( person, reason );

	//
	// If this is the player, then it's GAME OVER
	//

	if ( name == "PLAYER" ) {

		if ( gateway_id == game->GetWorld ()->GetPlayer ()->gateway.id ) {

			string deathmsg = "Disavowed by Uplink Corporation at ";
			deathmsg += game->GetWorld ()->date.GetLongString ();
			deathmsg += "\nFor " + reason + "\n";

			game->GameOver ( deathmsg );

		}
		else {

			// The player's old gateway was seized
			// Not quite as severe - not game over

			auto *m = new Message ();
			m->SetTo ( "PLAYER" );
			m->SetFrom ( "Uplink Corporation" );
			m->SetSubject ( "Gateway seizure" );
			m->SetBody ( "Your old gateway was recently seized by federal agents, shortly after you blew it up.\n"
						 "It seems that no evidence remains of the crime that you didn't commit.\n\n"
						 "A very lucky escape, if you ask us.\n" );
			m->Send ();

			// Uplink Corporation won't like you for this (it's reckless after all)
			// Other hackers love this - everybody knows you did it but
			// you got away with it scott free!

			game->GetWorld ()->GetPlayer ()->rating.ChangeNeuromancerScore ( NEUROMANCERCHANGE_GATEWAYNEARMISS );
			game->GetWorld ()->GetPlayer ()->rating.ChangeUplinkScore ( UPLINKCHANGE_GATEWAYNEARMISS );

		}

	}

}

void SeizeGatewayEvent::RunWarning ()
{

	//
	// Agents are near (2 of em!)
	//

	game->GetWorld ()->GetPlayer ()->gateway.IncrementProximity ( 2 );

}

string SeizeGatewayEvent::GetShortString ()
{

	return "Seize Gateway of " + name;

}

string SeizeGatewayEvent::GetLongString ()
{

	return "Seize Gateway Event\nFor : " + name + "\nGatewayID : " + to_string(gateway_id) + "\nReason : " + reason;

}


bool SeizeGatewayEvent::Load  ( FILE *file )
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
	if ( !FileReadData ( &gateway_id, sizeof(gateway_id), 1, file ) ) return false;

	LoadID_END ( file );

	return true;

}

void SeizeGatewayEvent::Save  ( FILE *file )
{

	SaveID ( file );

	UplinkEvent::Save ( file );

	SaveDynamicString ( name, sizeof(name), file );
	SaveDynamicString ( reason, file );
	fwrite ( &gateway_id, sizeof(gateway_id), 1, file );

	SaveID_END ( file );

}

void SeizeGatewayEvent::Print ()
{

    cout << "Seize Gateway Event : " << endl;
    PrintValue("Name", name);
    PrintValue("Gateway ID", gateway_id);
	UplinkEvent::Print ();
    PrintValue("Reason", reason);


}

string SeizeGatewayEvent::GetID ()
{

	return "EVT_SEZ";

}

int   SeizeGatewayEvent::GetOBJECTID ()
{

	return OID_SEIZEGATEWAYEVENT;

}

