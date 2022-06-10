
#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

#include <sstream>

#include "gucci.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/opengl_interface.h"

#include "options/options.h"

#include "game/game.h"

#include "world/world.h"

#include "interface/interface.h"
#include <interface/scrollbox.h>
#include "interface/localinterface/localinterface.h"
#include "interface/localinterface/evtqueue_interface.h"

#include "mmgr.h"


int EventQueueInterface::baseoffset = 0;
int EventQueueInterface::previousupdate = 0;


void EventQueueInterface::TitleClick ( Button *button )
{

	game->GetInterface ()->GetLocalInterface ()->RunScreen ( SCREEN_NONE );

}

void EventQueueInterface::ScrollUpClick ( Button *button )
{
	
	--baseoffset;	
	if ( baseoffset < 0 ) baseoffset = 0;

	// Invalidate all the buttons

	for ( int i = 0; i < 9; ++i ) {
	
		string name = "evtqueue_event " + to_string( i );
		EclDirtyButton ( name );

	}

}

void EventQueueInterface::ScrollDownClick ( Button *button )
{

	++baseoffset;

	// Invalidate all the buttons

	for ( int i = 0; i < 9; ++i ) {
	
		string name = "evtqueue_event %d" + to_string( i );
		EclDirtyButton ( name );

	}

}

void EventQueueInterface::FullDetailsCloseClick ( Button *button )
{

	EclRemoveButton ( "evtqueue_fulldate" );
	remove_stextbox ( "evtqueue_fulldetails" );

}

void EventQueueInterface::EventDraw ( Button *button, bool highlighted, bool clicked )
{

	int index;
    string unused;
    istringstream stream(button->name);
    stream >> unused >> index;
	index += baseoffset;

	if ( index < game->GetWorld ()->scheduler.events.Size () ) {

		UplinkEvent *e = game->GetWorld ()->scheduler.events.GetData (index);
		UplinkAssert (e)

		// Draw the button background

		button_draw ( button, highlighted, clicked );

		int screenheight = app->GetOptions ()->GetOptionValue ( "graphics_screenheight" );
		glScissor ( button->x, screenheight - (button->y + button->height), button->width, button->height );	
		glEnable ( GL_SCISSOR_TEST );

		// Print the date

		char *date = e->rundate.GetLongString ();
		GciDrawText ( button->x + 5, button->y + 12, date, HELVETICA_10 );

		// Print a short description
		GciDrawText ( button->x + 5, button->y + 24, e->GetShortString (), HELVETICA_10 );

		glDisable ( GL_SCISSOR_TEST );

	}
	else {

		clear_draw ( button->x, button->y, button->width, button->height );

	}

}

void EventQueueInterface::EventClick ( Button *button )
{

	int index;
    string unused;
    istringstream stream(button->name);
    stream >> unused >> index;
	index += baseoffset;

	if ( index < game->GetWorld ()->scheduler.events.Size () ) {

		int screenw = app->GetOptions ()->GetOptionValue ("graphics_screenwidth");
		int screenh = app->GetOptions ()->GetOptionValue ("graphics_screenheight");
		int paneltop = (int)(100.0 * ( screenw * PANELSIZE / 188.0 )) + 30;
		int panelwidth = (int)(screenw * PANELSIZE);

		// Create a more detailed version

		UplinkEvent *e = game->GetWorld ()->scheduler.events.GetData (index);
		UplinkAssert (e)

		char *date = e->rundate.GetLongString ();

		EclRegisterButton ( screenw - panelwidth - 3, paneltop + 20, panelwidth, 15, date, "Click to close", "evtqueue_fulldate" );
		EclRegisterButtonCallback ( "evtqueue_fulldate", FullDetailsCloseClick );
		create_stextbox ( screenw - panelwidth - 3, paneltop + 40, panelwidth, 9 * 30, e->GetLongString (), "evtqueue_fulldetails" );

	}

}

void EventQueueInterface::DeleteEventDraw ( Button *button, bool highlighted, bool clicked )
{

   	int index;
    string unused;
    istringstream stream(button->name);
    stream >> unused >> index;
	index += baseoffset;

	if ( index < game->GetWorld ()->scheduler.events.Size () ) 
        imagebutton_draw ( button, highlighted, clicked );

}

void EventQueueInterface::DeleteEventClick ( Button *button )
{

	int index;
    string unused;
    istringstream stream(button->name);
    stream >> unused >> index;
	index += baseoffset;

    if ( index < game->GetWorld ()->scheduler.events.Size () ) {

        game->GetWorld ()->scheduler.events.RemoveData (index);
    
    }

}

int EventQueueInterface::NumItemsOnScreen(int height)
{
    int screenheight = app->GetOptions()->GetOptionValue( "graphics_screenheight" );
    return height/30;
}

void EventQueueInterface::ScrollChange (const string &scrollname, int newValue )
{

    baseoffset = newValue;

    for (int i = 0; i < NumItemsOnScreen(0); ++i ) {

        string name = "evtqueue_event " + to_string(i);
        EclDirtyButton ( name );

    }

}

void EventQueueInterface::Create ()
{

	if ( !IsVisible () ) {

		int screenw = app->GetOptions ()->GetOptionValue ("graphics_screenwidth");
		int screenh = app->GetOptions ()->GetOptionValue ("graphics_screenheight");
		int paneltop = (int)(100.0 * ( screenw * PANELSIZE / 188.0 )) + 30;
		int panelwidth = (int)(screenw * PANELSIZE);

        int boxheight = screenh - (paneltop + 85) - 90;

		// Title

		EclRegisterButton ( screenw - panelwidth - 3, paneltop, panelwidth, 15, "EVENT QUEUE", "Remove the screen", "evtqueue_title" );
		EclRegisterButtonCallback ( "evtqueue_title", TitleClick );

		for (int i = 0; i < NumItemsOnScreen(boxheight); ++i ) {

			string name = "evtqueue_event " + to_string( i );
			EclRegisterButton (  screenw - panelwidth - 3, paneltop + 20 + i * 30, panelwidth - 20, 28, "", "Click to enlarge this event", name );
			EclRegisterButtonCallbacks ( name, EventDraw, EventClick, button_click, button_highlight );

            string deletename = "evtqueue_deleteevent " + to_string( i );
            EclRegisterButton (  (screenw - panelwidth - 3) + panelwidth - 35, paneltop + 20 + i * 30, 13, 13, " ", "Click to delete this event", deletename );
            button_assignbitmaps ( deletename, "close.tif", "close_h.tif", "close_c.tif" );
            EclRegisterButtonCallbacks ( deletename, DeleteEventDraw, DeleteEventClick, button_click, button_highlight );

		}

        ScrollBox::CreateScrollBox ("evtqueue_scroll", screenw - 7 - 15, paneltop + 20, 15, NumItemsOnScreen(boxheight) * 30, game->GetWorld ()->scheduler.events.Size(),
                                    NumItemsOnScreen(
                                            0), 0, ScrollChange );

	}

}

void EventQueueInterface::Remove ()
{

	if ( IsVisible () ) {

		EclRemoveButton ( "evtqueue_title" );

        int screenw = app->GetOptions ()->GetOptionValue ("graphics_screenwidth");
        int screenh = app->GetOptions ()->GetOptionValue ("graphics_screenheight");
        int paneltop = (int)(100.0 * ( screenw * PANELSIZE / 188.0 )) + 30;
        int boxheight = screenh - (paneltop + 85) - 90;

        for (int i = 0; i < NumItemsOnScreen(boxheight); ++i ) {

            string name = "evtqueue_event " + to_string( i );
            EclRemoveButton ( name );

            string deletename = "evtqueue_deleteevent " + to_string( i );
            EclRemoveButton ( deletename );

        }

        ScrollBox::RemoveScrollBox( "evtqueue_scroll" );

	}

}

void EventQueueInterface::Update ()
{

	// Update every 5 seconds

	if ( time(nullptr) > previousupdate + 5 ) {

		// Invalidate all the buttons

		for ( int i = 0; i < 9; ++i ) {
		
			string name = "evtqueue_event " + to_string( i );
			EclDirtyButton ( name );

		}

	}

}

bool EventQueueInterface::IsVisible ()
{

	return ( EclGetButton ( "evtqueue_title" ) != nullptr );

}

int EventQueueInterface::ScreenID ()
{

	return SCREEN_EVTQUEUE;

}
