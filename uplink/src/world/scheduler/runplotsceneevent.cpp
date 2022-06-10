
#include "app/globals.h"
#include "app/serialise.h"

#include "game/game.h"

#include "world/world.h"
#include "world/scheduler/runplotsceneevent.h"

#include "mmgr.h"



RunPlotSceneEvent::RunPlotSceneEvent ()
{
	
	act = 0;
	scene = 0;

}

RunPlotSceneEvent::~RunPlotSceneEvent ()
= default;

void RunPlotSceneEvent::SetActAndScene ( int newact, int newscene )
{

	act = newact;
	scene = newscene;

}

void RunPlotSceneEvent::Run ()
{

    if ( act != -1 )
	    game->GetWorld ()->plotgenerator.Run_Scene ( act, scene );

    else
        game->GetWorld ()->demoplotgenerator.RunScene ( scene );

}


string RunPlotSceneEvent::GetShortString ()
{
	
	return GetLongString ();

}

string RunPlotSceneEvent::GetLongString ()
{

	if ( act != -1 ) {
	    return "Run Plot Act " + to_string(act) + ", Scene " + to_string(scene);

	} else {
        return "Run Demo Plot Scene " + to_string(scene);
	}

}


bool RunPlotSceneEvent::Load  ( FILE *file )
{

	LoadID ( file );

    if ( !UplinkEvent::Load ( file ) ) return false;

	if ( !FileReadData ( &act, sizeof(act), 1, file ) ) return false;
	if ( !FileReadData ( &scene, sizeof(scene), 1, file ) ) return false;

	LoadID_END ( file );

	return true;

}

void RunPlotSceneEvent::Save  ( FILE *file )
{

	SaveID ( file );

    UplinkEvent::Save ( file );

	fwrite ( &act, sizeof(act), 1, file );
	fwrite ( &scene, sizeof(scene), 1, file );

	SaveID_END ( file );
}

void RunPlotSceneEvent::Print ()
{

    UplinkEvent::Print ();
	cout << "\t" << GetLongString () << endl;

}

	
string RunPlotSceneEvent::GetID ()
{
	
	return "EVT_PLOT";

}

int  RunPlotSceneEvent::GetOBJECTID ()
{
	
	return OID_RUNPLOTSCENEEVENT;

}


