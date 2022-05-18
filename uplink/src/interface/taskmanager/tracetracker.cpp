// TraceTracker.cpp: implementation of the TraceTracker class.
//
//////////////////////////////////////////////////////////////////////


#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

#include <sstream>

#include "eclipse.h"
#include "gucci.h" 
#include "vanbakel.h"
#include "soundgarden.h"
#include "redshirt.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/opengl_interface.h"

#include "options/options.h"

#include "game/game.h"

#include "interface/taskmanager/tracetracker.h"

#include "world/world.h"
#include "world/player.h"
#include "world/vlocation.h"
#include "world/computer/computer.h"

#include "mmgr.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


void TraceTracker::CloseClick ( Button *button )
{

	int pid;
	char bname [64];
    istringstream stream(button->name);
    stream >> bname >> pid;

	SvbRemoveTask ( pid );

}

void TraceTracker::TraceDraw ( Button *button, bool highlighted, bool clicked )
{

    UplinkAssert (button)

    //
    // Get the task
    //

	int pid;
	char bname [64];
    istringstream stream(button->name);
    stream >> bname >> pid;
    auto *thistask = (TraceTracker *) SvbGetTask (pid);
    UplinkAssert (thistask)

    //
    // Work out the brightness
    //

    float brightness = 0.0;
       
    if ( game->GetWorld ()->GetPlayer ()->connection.TraceInProgress () ) {

        if ( thistask->beepdelay < 100 )
            brightness = 0.8f;

        else {
            int timeoflastbeep = thistask->nextbeep - thistask->beepdelay;
            int timepassedsincelastbeep = (int) ( EclGetAccurateTime () - timeoflastbeep );
            brightness = 0.7f * (((float) timepassedsincelastbeep) / (float) thistask->beepdelay);
        }

    }
    else
        brightness = 0.0f;

    //
	// Draw the button
    //

	int screenheight = app->GetOptions ()->GetOptionValue ( "graphics_screenheight" );
	glScissor ( button->x, screenheight - (button->y + button->height), button->width, button->height );	
	glEnable ( GL_SCISSOR_TEST );
	
	glBegin ( GL_QUADS );

		glColor4f ( brightness, brightness, 0.7f, 0.5f );
		glVertex2i ( button->x, button->y + button->height );

		glColor4f ( brightness, brightness, 0.4f, 0.5f );
		glVertex2i ( button->x, button->y );

		glColor4f ( brightness, brightness, 0.7f, 0.5f );
		glVertex2i ( button->x + button->width, button->y );

		glColor4f ( brightness, brightness, 0.4f, 0.5f );
		glVertex2i ( button->x + button->width, button->y + button->height );

	glEnd ();

	//
    // Draw the text
    //

	int xpos = (button->x + button->width  / 2) - ( GciTextWidth ( button->caption ) / 2 );
	int ypos = (button->y + button->height / 2) + 2;

	if ( highlighted || clicked )	glColor4f ( 1.0f, 1.0f, 1.0f, 1.0f );
	else							glColor4f ( 1.0f, 1.0f, 1.0f, ALPHA );    
    GciDrawText ( xpos, ypos, button->caption );

	glDisable ( GL_SCISSOR_TEST );

}

void TraceTracker::AudioDraw ( Button *button, bool highlighted, bool clicked )
{

	int pid;
	char bname [64];
    istringstream stream(button->name);
    stream >> bname >> pid;
    auto *thistask = (TraceTracker *) SvbGetTask (pid);
    UplinkAssert (thistask)

    if ( thistask->audioon )
        imagebutton_draw ( button, false, false );

    else
        imagebutton_draw ( button, true, true );

}

void TraceTracker::AudioClick ( Button *button )
{

	int pid;
	char bname [64];
    istringstream stream(button->name);
    stream >> bname >> pid;
    auto *thistask = (TraceTracker *) SvbGetTask (pid);
    UplinkAssert (thistask)

    thistask->audioon = !thistask->audioon;

    string audioname = "tracetracker_audio " + to_string(pid);

    if ( thistask->audioon )
        button_assignbitmaps ( audioname, "software/audioon.tif", "software/audioon_h.tif", "software/audioon_c.tif" );

    else
        button_assignbitmaps ( audioname, "software/audiooff.tif", "software/audiooff_h.tif", "software/audiooff_c.tif" );

}

TraceTracker::TraceTracker() : UplinkTask ()
{

    x = 0;
    y = 0;
    traceprogress = -1;
    traceestimate = -1;
    nextbeep = -1;
    beepdelay = -1;
    audioon = true;
	lastupdate = 0;
    
}

TraceTracker::~TraceTracker()
= default;

void TraceTracker::Initialise ()
{
}

void TraceTracker::MoveTo ( int x, int y, int time_ms )
{
	
	if ( IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );

		string displayname = "tracetracker_display " + to_string(pid);
        string closename = "tracetracker_close " + to_string(pid);
        string audioname = "tracetracker_audio " + to_string(pid);

		EclRegisterMovement ( displayname, x, y, time_ms );
		EclRegisterMovement ( closename, x + 100, y, time_ms );
        EclRegisterMovement ( audioname, x - 20, y, time_ms );

	}

}

void TraceTracker::Tick ( int n )
{

	if ( IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );
		string displayname = "tracetracker_display " + to_string(pid);

		Button *button = EclGetButton ( displayname );
		UplinkAssert ( button )

        Connection *connection = game->GetWorld ()->GetPlayer ()->GetConnection ();

        //
        // Calculate our best guess for the trace time
        //

        if ( connection->TraceInProgress () && 
             connection->traceprogress != traceprogress ) {

            int timeremaining = game->GetWorld ()->GetPlayer ()->TimeRemaining ();

            traceestimate = (int) ( EclGetAccurateTime () + timeremaining * 1000 );
            traceprogress = connection->traceprogress;

        }

        //
        // deal with the bleeping
        //

        if ( connection->TraceInProgress () ) {

            if ( EclGetAccurateTime () >= nextbeep ) {

                if ( audioon )
                    SgPlaySound ( RsArchiveFileOpen ( "sounds/tracebleep.wav" ), "sounds/tracebleep.wav" );
                
                int timeremaining = (int) ( traceestimate - EclGetAccurateTime () );
                beepdelay = (int) ( (float) timeremaining / 20.0 );
                nextbeep = (int) ( EclGetAccurateTime () + beepdelay );

            }

		    string displayname = "tracetracker_display " + to_string(pid);
		    EclDirtyButton ( displayname );

        }

		if ( connection->Traced () ) {

			button->SetCaption ( "Traced" );

		}
		else if ( connection->TraceInProgress () &&
				  connection->traceprogress > 0 ) {
			
			int timesincelastupdate = (int) ( EclGetAccurateTime () - lastupdate );

			if ( timesincelastupdate >= 500 ) {

				if ( version == 1.0 ) {

					if ( connection->traceprogress >= connection->GetSize () - 2 ) 
						button->SetCaption ( "Trace Imminent" );
					
					else
						button->SetCaption ( "In progress" );

				}
				else if ( version == 2.0 ) {

					int percent = (int) ( 100 * (float) connection->traceprogress / (float) (connection->GetSize () - 1) );
					string caption = "Trace: " + to_string(percent) + "%%";
					button->SetCaption ( caption );		

				}
				else if ( version == 3.0 ) {

					int timeremaining = game->GetWorld ()->GetPlayer ()->TimeRemaining ();

					string caption = "Trace: " + to_string(timeremaining) + " secs";
					button->SetCaption ( caption );		
					
				}
				else if ( version == 4.0 ) {

					int timeremaining = game->GetWorld ()->GetPlayer ()->TimeRemaining ();

					int accuratetimeremaining = (int) ( (traceestimate - EclGetAccurateTime ()) / 1000 );
					if ( accuratetimeremaining < 0 ) accuratetimeremaining = 0;
					string caption = "Trace: " + to_string(timeremaining) + "s (" + to_string(accuratetimeremaining) + ")";
					button->SetCaption ( caption );

				}
				else {

					cout << "TraceTracker WARNING : Unrecognised version number" << endl;

				}

				lastupdate = (int) EclGetAccurateTime ();

			}


		}
		else if ( connection->TraceInProgress () &&
				  connection->traceprogress == 0 ) {

			button->SetCaption ( "Trace begun" );			

		}
		else {

			button->SetCaption ( "No Traces" );

		}

	}

}
	
void TraceTracker::CreateInterface ()
{

	if ( !IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );

		string displayname = "tracetracker_display " + to_string(pid);
        string closename = "tracetracker_close " + to_string(pid);
        string audioname = "tracetracker_audio " + to_string(pid);

		EclRegisterButton ( 25, 300, 100, 13, "Trace Tracker", "Trace Tracker", displayname );
		EclRegisterButtonCallbacks ( displayname, TraceDraw, nullptr, nullptr, nullptr );

		EclRegisterButton ( 125, 300, 13, 13, "", "Close the TraceTracker", closename );		
		button_assignbitmaps ( closename, "close.tif", "close_h.tif", "close_c.tif" );
		EclRegisterButtonCallback ( closename, CloseClick );
		
        EclRegisterButton ( 5, 300, 20, 13, "", "Toggle audio beeps", audioname );
        button_assignbitmaps ( audioname, "software/audioon.tif", "software/audioon_h.tif", "software/audioon_c.tif" );
        EclRegisterButtonCallbacks ( audioname, AudioDraw, AudioClick, button_click, button_highlight );
        
        int screenW = app->GetOptions ()->GetOptionValue( "graphics_screenwidth" );
        int screenH = app->GetOptions ()->GetOptionValue( "graphics_screenheight" );

		MoveTo ( screenW - 115, screenH - 15, 500 );

	}

}

void TraceTracker::RemoveInterface ()
{

	if ( IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );

		string displayname = "tracetracker_display " + to_string(pid);
        string closename = "tracetracker_close " + to_string(pid);
        string audioname = "tracetracker_audio " + to_string(pid);

		EclRemoveButton ( displayname );		
		EclRemoveButton ( closename );
        EclRemoveButton ( audioname );

	}

}

void TraceTracker::ShowInterface ()
{

	if ( !IsInterfaceVisible () ) CreateInterface ();

	int pid = SvbLookupPID ( this );

	string displayname = "tracetracker_display " + to_string(pid);
    string closename = "tracetracker_close " + to_string(pid);
    string audioname = "tracetracker_audio " + to_string(pid);

	EclButtonBringToFront ( displayname );		
	EclButtonBringToFront ( closename );
    EclButtonBringToFront ( audioname );

}

bool TraceTracker::IsInterfaceVisible ()
{

	int pid = SvbLookupPID ( this );
	string displayname = "tracetracker_display " + to_string(pid);

	return ( EclGetButton ( displayname ) != nullptr );

}
