
#ifdef WIN32
#include <windows.h>
#endif

#include <strstream>

#include <GL/gl.h>

#include <sstream>

#include "eclipse.h"
#include "vanbakel.h"

#include "app/globals.h"
#include "app/opengl_interface.h"

#include "game/game.h"

#include "world/world.h"
#include "world/player.h"
#include "world/vlocation.h"
#include "world/computer/computer.h"

#include "interface/interface.h"
#include "interface/remoteinterface/remoteinterface.h"
#include "interface/taskmanager/taskmanager.h"
#include "interface/taskmanager/revelationtracker.h"

#include "mmgr.h"

RevelationTracker::RevelationTracker ()
{

    timesync = 0;
    height = 0;

}

RevelationTracker::~RevelationTracker ()
= default;

void RevelationTracker::Initialise ()
{
}

void RevelationTracker::Tick ( int n )
{

	if ( IsInterfaceVisible () ) {

		if ( time(nullptr) > timesync + 3 ) {

			int pid = SvbLookupPID ( this );
			string boxname = "revelationtracker_box " + to_string(pid);
			Button *button = EclGetButton ( boxname );
			UplinkAssert(button)

			//
			// How many computers are infected?

			int numinfected = game->GetWorld ()->plotgenerator.infected.Size ();

			if ( numinfected != height ) {

				// Resize the box

				EclRegisterResize ( boxname, button->width, 15 + numinfected * 15, 1000 );
				height = numinfected;

			}

			//
			// Rebuild the caption

			if ( numinfected == 0 ) {

				button->SetCaption ( "No infected systems" );

			}
			else {

				std::ostrstream newcaption;

				newcaption << "Infected systems" << "\n";

				for ( int i = 0; i < game->GetWorld ()->plotgenerator.infected.Size (); ++i ) {

					string ip = game->GetWorld ()->plotgenerator.infected.GetData (i);
					assert(!ip.empty());

					VLocation *vl = game->GetWorld ()->GetVLocation (ip);
					UplinkAssert (vl)

					Computer *comp = vl->GetComputer ();
					UplinkAssert (comp)

					if ( comp->isinfected_revelation > 0.0 ) {

						newcaption << comp->ip;
						newcaption << " at ";
						newcaption << comp->infectiondate.GetHour () << ":" << comp->infectiondate.GetMinute ();
						newcaption << "\n";

					}

				}

				newcaption << '\x0';

				button->SetCaption ( newcaption.str () );

				newcaption.rdbuf()->freeze( false );
				//delete [] newcaption.str ();

			}

		}

	}

}


void RevelationTracker::TitleClick ( Button *button )
{

    UplinkAssert (button)

    char unused [128];
    int pid;
    istringstream stream(button->name);
    stream >> unused >> pid;

    game->GetInterface ()->GetTaskManager ()->SetTargetProgram ( pid );

}

void RevelationTracker::CloseClick ( Button *button )
{

	int pid;
	char bname [128];
    istringstream stream(button->name);
    stream >> bname >> pid;

	SvbRemoveTask ( pid );

}

void RevelationTracker::MainBoxDraw ( Button *button, bool highlighted, bool clicked )
{

	glBegin ( GL_QUADS );
		glColor3ub ( 8, 20, 0 );		glVertex2i ( button->x, button->y + button->height );
		glColor3ub ( 8, 20, 124 );		glVertex2i ( button->x, button->y );
		glColor3ub ( 8, 20, 0 );		glVertex2i ( button->x + button->width, button->y );
		glColor3ub ( 8, 20, 124 );		glVertex2i ( button->x + button->width, button->y + button->height );
	glEnd ();

	glColor3ub ( 81, 138, 215 );
	border_draw ( button );

    text_draw ( button, highlighted, clicked );

}

void RevelationTracker::CreateInterface ()
{

	if ( !IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );

        string titlename = "revelationtracker_title " + to_string(pid);
        string boxname = "revelationtracker_box " + to_string(pid);
        string closename = "revaltiontracker_close " + to_string(pid);

        EclRegisterButton ( 0, 50, 130, 15, "Revelation Tracker", "Shows all currently infected computers", titlename );
        EclRegisterButton ( 130, 51, 13, 13, "X", "Close the Revelation Tracker", closename );
        EclRegisterButton ( 0, 65, 143, 30, "Listening...", " ", boxname );

        button_assignbitmaps ( closename, "close.tif", "close_h.tif", "close_c.tif" );

        EclRegisterButtonCallback ( titlename, TitleClick );
        EclRegisterButtonCallback ( closename, CloseClick );
        EclRegisterButtonCallbacks ( boxname, MainBoxDraw, nullptr, nullptr, nullptr );

		timesync = time(nullptr);
        height = 0;

	}

}

void RevelationTracker::RemoveInterface ()
{

	if ( IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );

        string titlename = "revelationtracker_title " + to_string(pid);
        string boxname = "revelationtracker_box " + to_string(pid);
        string closename = "revaltiontracker_close " + to_string(pid);

        EclRemoveButton ( titlename );
        EclRemoveButton ( boxname );
        EclRemoveButton ( closename );

	}

}

void RevelationTracker::ShowInterface ()
{

	if ( IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );

        string titlename = "revelationtracker_title " + to_string(pid);
        string boxname = "revelationtracker_box " + to_string(pid);
        string closename = "revaltiontracker_close " + to_string(pid);

        EclButtonBringToFront ( titlename );
        EclButtonBringToFront ( boxname );
        EclButtonBringToFront ( closename );

	}

}

void RevelationTracker::MoveTo ( int x, int y, int time_ms )
{

    if ( IsInterfaceVisible () ) {

        int pid = SvbLookupPID ( this );

        string titlename = "revelationtracker_title " + to_string(pid);
        string boxname = "revelationtracker_box " + to_string(pid);
        string closename = "revaltiontracker_close " + to_string(pid);

        EclRegisterMovement ( titlename, x, y, time_ms );
        EclRegisterMovement ( boxname, x, y + 15, time_ms );
        EclRegisterMovement ( closename, x + 130, y + 1, time_ms );

    }

}

bool RevelationTracker::IsInterfaceVisible ()
{

	int pid = SvbLookupPID ( this );
    string titlename = "revelationtracker_title " + to_string(pid);

	return ( EclGetButton ( titlename ) != nullptr );

}
