
#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

#include <sstream>


#include "vanbakel.h"
#include "eclipse.h"

#include "app/globals.h"
#include "app/opengl_interface.h"
#include "app/miscutils.h"

#include "game/game.h"

#include "interface/taskmanager/iplookup.h"

#include "world/world.h"
#include "world/vlocation.h"
#include "world/player.h"

#include "mmgr.h"

void IPLookup::DisplayDraw ( Button *button, bool highlighted, bool clicked )
{

	// Draw a grey background

	glBegin ( GL_QUADS );
		glColor4f ( 0.6f, 0.6f, 0.6f, ALPHA );		glVertex2i ( button->x, button->y );
		glColor4f ( 0.4f, 0.4f, 0.4f, ALPHA );		glVertex2i ( button->x + button->width, button->y );
		glColor4f ( 0.6f, 0.6f, 0.6f, ALPHA );		glVertex2i ( button->x + button->width, button->y + button->height );
		glColor4f ( 0.4f, 0.4f, 0.4f, ALPHA );		glVertex2i ( button->x, button->y + button->height );
	glEnd ();

	// Draw ordinary text

	text_draw ( button, highlighted, clicked );

	// Draw a border

	if ( highlighted || clicked ) border_draw ( button );

}

void IPLookup::CloseClick ( Button *button )
{

	int pid;
    string unused;
    istringstream stream(button->name);
    stream >> unused >> pid;

	SvbRemoveTask ( pid );

}

void IPLookup::GoClick ( Button *button )
{

	int pid;
    string unused;
    istringstream stream(button->name);
    stream >> unused >> pid;

	auto *task = (IPLookup *) SvbGetTask ( pid );

	if ( task->status == IPLOOKUP_IDLE )
		task->status = IPLOOKUP_INPROGRESS;

	else
		task->status = IPLOOKUP_IDLE;

}

IPLookup::IPLookup () : UplinkTask ()
{

	status = IPLOOKUP_IDLE;

}

IPLookup::~IPLookup ()
= default;

void IPLookup::Initialise ()
{
}

void IPLookup::Tick ( int n )
{

	if ( IsInterfaceVisible () ) {

		if ( status == IPLOOKUP_IDLE ) {

			// Do nothing

		}
		else if ( status == IPLOOKUP_INPROGRESS ) {

			// Try to lookup the IP in the box

			int pid = SvbLookupPID ( this );
			string name_display = "iplookup_display " + to_string(pid);
			
			Button *button = EclGetButton ( name_display );
			UplinkAssert ( button )
			string ip = StripCarriageReturns (button->caption);

			VLocation *vl = game->GetWorld ()->GetVLocation ( ip );

			if ( vl ) {

				// Found - add to links
				EclRegisterCaptionChange ( name_display, vl->computer );
				game->GetWorld ()->GetPlayer ()->GiveLink ( ip );

			}
			else {

				// Not found
				EclRegisterCaptionChange ( name_display, "IP Not found" );

			}

			timeout = time(nullptr);
			status = IPLOOKUP_FINISHED;


		}
		else if ( status == IPLOOKUP_FINISHED ) {

			// Revert to idle after 5 secs

			if ( time (nullptr) > timeout + 5 ) {

				int pid = SvbLookupPID ( this );
				string name_display = "iplookup_display " + to_string(pid);

				EclRegisterCaptionChange ( name_display, "Enter IP" );
				status = IPLOOKUP_IDLE;
				
			}

		}
		else {

			UplinkAbort ( "Unkown Status" )

		}

	}

}

void IPLookup::CreateInterface ()
{
	
	if ( !IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );

		string name_go = "iplookup_go " + to_string(pid);
        string name_display = "iplookup_display " + to_string(pid);
        string name_close = "iplookup_close " + to_string(pid);

		EclRegisterButton ( 305, 444, 20, 15, "", "Perform the lookup", name_go );
		EclRegisterButton ( 325, 444, 185, 14, "Enter IP", "Type your IP here", name_display );
		EclRegisterButton ( 510, 445, 13, 13, "", "Close the IP Lookup program", name_close );

		button_assignbitmap ( name_go, "software/go.tif" );
		EclRegisterButtonCallback ( name_go, GoClick );
		EclRegisterButtonCallbacks ( name_display, DisplayDraw, nullptr, button_click, button_highlight );
		button_assignbitmaps ( name_close, "close.tif", "close_h.tif", "close_c.tif" );
		EclRegisterButtonCallback ( name_close, CloseClick );

		EclMakeButtonEditable ( name_display );

	}

}

void IPLookup::RemoveInterface ()
{

	if ( IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );

		string name_go = "iplookup_go " + to_string(pid);
        string name_display = "iplookup_display " + to_string(pid);
        string name_close = "iplookup_close " + to_string(pid);

		EclRemoveButton ( name_go );
		EclRemoveButton ( name_display );
		EclRemoveButton ( name_close );

	}

}

void IPLookup::ShowInterface ()
{

	if ( !IsInterfaceVisible () ) CreateInterface ();

	int pid = SvbLookupPID ( this );

	string name_go = "iplookup_go " + to_string(pid);
    string name_display = "iplookup_display " + to_string(pid);
    string name_close = "iplookup_close " + to_string(pid);

	EclButtonBringToFront ( name_go );
	EclButtonBringToFront ( name_display );
	EclButtonBringToFront ( name_close );


}

bool IPLookup::IsInterfaceVisible ()
{

	int pid = SvbLookupPID ( this );
	string name_display = "iplookup_display " + to_string(pid);

	return ( EclGetButton ( name_display ) != nullptr );

}
