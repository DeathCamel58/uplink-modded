

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

#include <GL/glu.h> /*_glu_extention_library_*/


#include "eclipse.h"
#include "vanbakel.h"
#include "soundgarden.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/opengl.h"
#include "app/opengl_interface.h"

#include "mainmenu/mainmenu.h"

#include "options/options.h"

#include "game/game.h"

#include "world/world.h"
#include "world/player.h"

#include "interface/interface.h"
#include "interface/remoteinterface/remoteinterface.h"
#include "interface/taskmanager/gatewaynuke.h"

#include "mmgr.h"


void GatewayNuke::TitleDraw ( Button *button, bool highlighted, bool clicked )
{

	glBegin ( GL_QUADS );

		glColor4f ( 0.2f, 0.2f, 0.4f, ALPHA );			glVertex2i ( button->x, button->y );
		glColor4f ( 0.3f, 0.3f, 0.5f, ALPHA );			glVertex2i ( button->x + button->width, button->y );
		glColor4f ( 0.2f, 0.2f, 0.4f, ALPHA );			glVertex2i ( button->x + button->width, button->y + button->height );
		glColor4f ( 0.3f, 0.3f, 0.5f, ALPHA );			glVertex2i ( button->x, button->y + button->height );

	glEnd ();

	text_draw ( button, highlighted, clicked );

}

void GatewayNuke::YesClick ( Button *button )
{

	int pid;
	sscanf ( button->name, "gatenuke_yes %d", &pid );

	auto *task = (GatewayNuke *) SvbGetTask ( pid );
	UplinkAssert (task);

	char title [32];
	UplinkSnprintf ( title, sizeof ( title ), "gatenuke_title %d", pid );


	switch ( task->status ) {

		case GATEWAYNUKE_STATUS_NONE:
		{

			UplinkAbort ( "Status not set correctly" );
			break;

		}

		case GATEWAYNUKE_STATUS_START:
		{

			// Ask for confirmation

			task->status = GATEWAYNUKE_STATUS_AREYOUSURE;
			EclRegisterCaptionChange ( title, "Are you sure?" );
			break;

		}

		case GATEWAYNUKE_STATUS_AREYOUSURE:
		{

			// Do it!

			if ( game->GetWorld ()->GetPlayer ()->gateway.IsHWInstalled ( "Gateway Self Destruct" ) ) {

				NukeGateway ();	
				SvbRemoveTask ( pid );				

			}
			else {

				EclRegisterCaptionChange ( title, "No explosives found" );
				task->status = GATEWAYNUKE_STATUS_FAILED;

			}

			break;

		}

		case GATEWAYNUKE_STATUS_FAILED:
		{

			SvbRemoveTask ( pid );				
			break;

		}

		default:
			UplinkAbort ( "Unknown Status" );

	}

}

void GatewayNuke::NoClick ( Button *button )
{
	
	// Abort gate nuke

	int pid;
	sscanf ( button->name, "gatenuke_no %d", &pid );

	SvbRemoveTask ( pid );

}

void GatewayNuke::NukeGateway ()
{

	//
	// Reset the hardware
	//

	game->GetWorld ()->GetPlayer ()->gateway.Nuke ();

	//
	// Close any connections
	//

	game->GetWorld ()->GetPlayer ()->GetConnection ()->Disconnect ();
	game->GetWorld ()->GetPlayer ()->GetConnection ()->Reset ();

	game->GetInterface ()->GetRemoteInterface ()->RunNewLocation ();
	game->GetInterface ()->GetRemoteInterface ()->RunScreen ( 0 );		

}

GatewayNuke::GatewayNuke () : UplinkTask ()
{

	status = GATEWAYNUKE_STATUS_NONE;

}

GatewayNuke::~GatewayNuke ()
= default;

void GatewayNuke::SetTarget ( UplinkObject *uo, char *uos, int uoi )
{

	// This doesn't need to be called

}

void GatewayNuke::MoveTo ( int x, int y, int time_ms )
{
}

void GatewayNuke::Initialise ()
{

	status = GATEWAYNUKE_STATUS_START;

}

void GatewayNuke::Tick ( int n )
{

	if ( IsInterfaceVisible () ) {

		// Nothing to do here

	}

}


void GatewayNuke::CreateInterface ()
{

	if ( !IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );

		char title [32];
		char yes [32];
		char no [32];

		UplinkSnprintf ( title, sizeof ( title ), "gatenuke_title %d", pid );
		UplinkSnprintf ( yes, sizeof ( yes ), "gatenuke_yes %d", pid );
		UplinkSnprintf ( no, sizeof ( no ), "gatenuke_no %d", pid );

		EclRegisterButton ( 40, 20, 107, 15, "Nuke Gateway?", "Click yes to destroy your gateway, no to abort", title );
		EclRegisterButton ( 20, 20, 20, 15, "Yes", "Click here to nuke your gateway", yes );
		EclRegisterButton ( 147, 20, 20, 15, "No", "Click here to abort", no );

		EclRegisterButtonCallbacks ( title, TitleDraw, nullptr, nullptr, nullptr );
		EclRegisterButtonCallback ( yes, YesClick );
		EclRegisterButtonCallback ( no, NoClick );

	}

}

void GatewayNuke::RemoveInterface ()
{

	if ( IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );

		char title [32];
		char yes [32];
		char no [32];

		UplinkSnprintf ( title, sizeof ( title ), "gatenuke_title %d", pid );
		UplinkSnprintf ( yes, sizeof ( yes ), "gatenuke_yes %d", pid );
		UplinkSnprintf ( no, sizeof ( no ), "gatenuke_no %d", pid );

		EclRemoveButton ( title );
		EclRemoveButton ( yes );
		EclRemoveButton ( no );

	}

}

void GatewayNuke::ShowInterface ()
{

	if ( !IsInterfaceVisible () ) CreateInterface ();

	int pid = SvbLookupPID ( this );

	char title [32];
	char yes [32];
	char no [32];

	UplinkSnprintf ( title, sizeof ( title ), "gatenuke_title %d", pid );
	UplinkSnprintf ( yes, sizeof ( yes ), "gatenuke_yes %d", pid );
	UplinkSnprintf ( no, sizeof ( no ), "gatenuke_no %d", pid );

	EclButtonBringToFront ( title );
	EclButtonBringToFront ( yes );
	EclButtonBringToFront ( no );

}

bool GatewayNuke::IsInterfaceVisible ()
{

	int pid = SvbLookupPID ( this );
	char title [32];
	UplinkSnprintf ( title, sizeof ( title ), "gatenuke_title %d", pid );

	return ( EclGetButton ( title ) != nullptr );

}
