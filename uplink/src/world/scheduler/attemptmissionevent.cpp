
#include <strstream>
#include <app/miscutils.h>

#include "gucci.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/serialise.h"

#include "game/game.h"

#include "world/world.h"
#include "world/agent.h"
#include "world/company/mission.h"
#include "world/scheduler/attemptmissionevent.h"

#include "mmgr.h"


AttemptMissionEvent::AttemptMissionEvent ()
{

	memset ( agentname, 0, sizeof ( agentname ) );

}

AttemptMissionEvent::~AttemptMissionEvent ()
= default;

void AttemptMissionEvent::Run ()
{

	auto *agent = (Agent *) game->GetWorld ()->GetPerson ( agentname );
	UplinkAssert (agent)

	agent->AttemptMission ();

}

void AttemptMissionEvent::SetAgentName (const string &newagentname )
{

	assert( newagentname.length() < SIZE_PERSON_NAME );
	UplinkStrncpy ( agentname, newagentname.c_str(), sizeof ( agentname ) )

}

char *AttemptMissionEvent::GetShortString ()
{

	size_t shortstringsize = strlen(agentname) + 32;
	char *shortstring = new char [shortstringsize];
	UplinkSnprintf ( shortstring, shortstringsize, "Attempt Mission for %s", agentname )
	return shortstring;

}

char *AttemptMissionEvent::GetLongString ()
{

	auto *agent = (Agent *) game->GetWorld ()->GetPerson ( agentname );
	UplinkAssert (agent)

	Mission *m = agent->missions.GetData (0);
	UplinkAssert (m)

	std::ostrstream longstring;
	longstring << "Attempt Mission Event:\n"
			   << "Agent : " << agentname << "\n"
			   << "Agents next mission : " << m->description
			   << '\x0';

	return longstring.str ();

}


bool AttemptMissionEvent::Load ( FILE *file )
{

	LoadID ( file );

	if ( !UplinkEvent::Load ( file ) ) return false;

	if ( strcmp( game->GetLoadedSavefileVer(), "SAV59" ) >= 0 ) {
		if ( !LoadDynamicStringStatic ( agentname, sizeof(agentname), file ) ) return false;
	}
	else {
		if ( !FileReadData ( agentname, sizeof(agentname), 1, file ) ) {
			agentname [ sizeof(agentname) - 1 ] = '\0';
			return false;
		}
		agentname [ sizeof(agentname) - 1 ] = '\0';
	}

	LoadID_END ( file );

	return true;

}

void AttemptMissionEvent::Save ( FILE *file )
{

	SaveID ( file );

	UplinkEvent::Save ( file );

	SaveDynamicString ( agentname, sizeof(agentname), file );

	SaveID_END ( file );

}

void AttemptMissionEvent::Print ()
{

    cout << "AttemptMissionEvent :" << endl;
    PrintValue("agentname", agentname);

	UplinkEvent::Print ();

}

string AttemptMissionEvent::GetID ()
{

	return "EVT_AME";

}

int AttemptMissionEvent::GetOBJECTID ()
{

	return OID_ATTEMPTMISSIONEVENT;

}
