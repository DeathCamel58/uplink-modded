
#include <strstream>
#include <app/miscutils.h>

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

	agentname = "";

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

	agentname = newagentname;

}

string AttemptMissionEvent::GetShortString ()
{

	return "Attempt Mission for " + agentname;

}

string AttemptMissionEvent::GetLongString ()
{

	auto *agent = (Agent *) game->GetWorld ()->GetPerson ( agentname );
	UplinkAssert (agent)

	Mission *m = agent->missions.GetData (0);
	UplinkAssert (m)

	string longstring = "Attempt Mission Event:\nAgent : " + agentname + "\nAgents next mission : ";
	longstring += m->description;

	return longstring;

}


bool AttemptMissionEvent::Load ( FILE *file )
{

	LoadID ( file );

	if ( !UplinkEvent::Load ( file ) ) return false;

	if ( strcmp( game->GetLoadedSavefileVer(), "SAV59" ) >= 0 ) {
		if ( !LoadDynamicStringInt ( agentname, file ) ) return false;
	}
	else {
		if ( !FileReadDataInt ( agentname, SIZE_PERSON_NAME, file ) ) {
			return false;
		}
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
