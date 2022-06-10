
#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

#include <sstream>


#include "eclipse.h"
#include "vanbakel.h"
#include "gucci.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/opengl_interface.h"
#include "app/serialise.h"

#include "game/game.h"
#include "game/data/data.h"

#include "interface/interface.h"
#include "interface/taskmanager/taskmanager.h"
#include "interface/taskmanager/decypher.h"
#include "interface/remoteinterface/remoteinterface.h"
#include "interface/remoteinterface/cypherscreen_interface.h"

#include "world/world.h"
#include "world/player.h"
#include "world/computer/computer.h"
#include "world/computer/computerscreen/cypherscreen.h"
#include "world/generator/numbergenerator.h"

#include "mmgr.h"


Decypher::Decypher () : UplinkTask ()
{
	
	status = DECYPHER_OFF;
	numticksrequired = 0;
	progress = 0;

}

Decypher::~Decypher ()
= default;

void Decypher::MoveTo ( int x, int y, int time_ms )
{

	UplinkTask::MoveTo ( x, y, time_ms );

	int pid = SvbLookupPID ( this );

	string stitle = "decypher_title " + to_string(pid);
    string sborder = "decypher_border " + to_string(pid);
    string sprogress = "decypher_progress " + to_string(pid);
    string sclose = "decypher_close " + to_string(pid);

	EclRegisterMovement ( stitle, x, y, time_ms);
	EclRegisterMovement ( sborder, x + 20, y, time_ms );
	EclRegisterMovement ( sprogress, x + 20, y + 1, time_ms );
	EclRegisterMovement ( sclose, x + 140, y + 1, time_ms );

	EclButtonBringToFront ( stitle );
	EclButtonBringToFront ( sborder );
	EclButtonBringToFront ( sprogress );
	EclButtonBringToFront ( sclose );

}

void Decypher::SetTarget (UplinkObject *uo, const string &uos, int uoi )
{

	if ( status == DECYPHER_OFF ) {

		if ( uo->GetOBJECTID () == OID_CYPHERSCREEN ) {

			auto *cs = (CypherScreen *) uo;
			UplinkAssert (cs)
			Computer *comp = cs->GetComputer ();
			UplinkAssert (comp)

			Button *button = EclGetButton ( uos );
			UplinkAssert (button)

			MoveTo ( button->x + button->width - 300, button->y + button->height, 1000 );

			int security = game->GetInterface ()->GetRemoteInterface ()->security_level;
			int difficulty = (int)(CYPHER_WIDTH * CYPHER_HEIGHT * TICKSREQUIRED_BYPASSCYPHER * cs->difficulty);
			numticksrequired = NumberGenerator::ApplyVariance ( difficulty, (int)(HACKDIFFICULTY_VARIANCE * 100) );
			progress = 0;

			if ( comp->security.IsRunning_Monitor () )
				game->GetWorld ()->GetPlayer ()->GetConnection ()->BeginTrace ();

			int pid = SvbLookupPID ( this );
			string sprogress = "decypher_progress " + to_string(pid);
			EclRegisterCaptionChange ( sprogress, "Decyphering..." );

			status = DECYPHER_INPROGRESS;

		}

	}

}

void Decypher::BorderDraw ( Button *button, bool highlighted, bool clicked )
{

	glBegin ( GL_QUADS );

		if      ( clicked )		glColor4f ( 0.5f, 0.5f, 0.6f, ALPHA );
		else if ( highlighted ) glColor4f ( 0.2f, 0.2f, 0.5f, ALPHA );
		else					glColor4f ( 0.2f, 0.2f, 0.4f, ALPHA );
		glVertex2i ( button->x, button->y );

		if		( clicked )		glColor4f ( 0.7f, 0.7f, 0.6f, ALPHA );
		else if ( highlighted ) glColor4f ( 0.5f, 0.5f, 0.6f, ALPHA );
		else					glColor4f ( 0.3f, 0.3f, 0.5f, ALPHA );
		glVertex2i ( button->x + button->width, button->y );

		if		( clicked )		glColor4f ( 0.5f, 0.5f, 0.6f, ALPHA );
		else if ( highlighted ) glColor4f ( 0.2f, 0.2f, 0.5f, ALPHA );
		else					glColor4f ( 0.2f, 0.2f, 0.4f, ALPHA );
		glVertex2i ( button->x + button->width, button->y + button->height );

		if		( clicked )		glColor4f ( 0.7f, 0.7f, 0.6f, ALPHA );
		else if ( highlighted ) glColor4f ( 0.5f, 0.5f, 0.6f, ALPHA );
		else					glColor4f ( 0.3f, 0.3f, 0.5f, ALPHA );
		glVertex2i ( button->x, button->y + button->height );

	glEnd ();

	if ( highlighted || clicked ) {

		glColor4f ( 0.3f, 0.3f, 0.7f, 1.0f );
		border_draw ( button );

	}

}

void Decypher::ProgressDraw ( Button *button, bool highlighted, bool clicked )
{

	UplinkAssert ( button )

	float scale = (float) button->width / 100.0f;
	if ( highlighted ) scale *= 2;

	glBegin ( GL_QUADS );

		glColor4f ( 0.0f, 1.5f - scale, scale, 0.6f );
		glVertex3i ( button->x, button->y, 0 );

		glColor4f ( 0.0f, 1.5f - scale, scale, 0.6f );
		glVertex3i ( button->x + button->width, button->y, 0 );

		glColor4f ( 0.0f, 1.5f - scale, scale, 0.6f );
		glVertex3i ( button->x + button->width, button->y + button->height, 0 );

		glColor4f ( 0.0f, 1.5f - scale, scale, 0.6f );
		glVertex3i ( button->x, button->y + button->height, 0 );

	glEnd ();

	int xpos = button->x + 5;
	int ypos = (button->y + button->height / 2) + 3;
		
	glColor4f ( 1.0f, 1.0f, 1.0f, 1.0f );    
    GciDrawText ( xpos, ypos, button->caption, HELVETICA_10 );

}

void Decypher::CloseClick ( Button *button )
{

	int pid;
	char bname [64];
    istringstream stream(button->name);
    stream >> bname >> pid;

	SvbRemoveTask ( pid );

}

void Decypher::BorderClick ( Button *button )
{

	int pid;
	char bname [64];
    istringstream stream(button->name);
    stream >> bname >> pid;

	game->GetInterface ()->GetTaskManager ()->SetTargetProgram ( pid );

}

void Decypher::Initialise ()
{
}

void Decypher::Tick ( int n )
{

	if ( IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );
		string sprogress = "decypher_progress " + to_string(pid);

		if ( status == DECYPHER_OFF ) {

			// Not decyphering - look for a new target


		}
		else if ( status == DECYPHER_INPROGRESS ) {

			// Make sure we are still looking at the
			// same interface screen

			RemoteInterfaceScreen *ris = game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ();
			UplinkAssert (ris)

			if ( ris->ScreenID () != SCREEN_CYPHERSCREEN ) {
				SvbRemoveTask ( this );
				// WARNING - this has now been deleted
				return;
			}

			auto *csi = (CypherScreenInterface *) ris;
				
			progress += (int)(n * version);

			UplinkAssert ( EclGetButton ( sprogress ) )
			EclGetButton ( sprogress )->width = (int)(120 * ( (float) progress / (float) numticksrequired ));
			EclDirtyButton ( sprogress );

			if ( progress >= numticksrequired ) {

				// Finished

				while ( csi->NumUnLocked () > 0 )
					csi->CypherLock ();

				status = DECYPHER_FINISHED;

			}
			else {

				int percentagedone = (int)(100.0f * (float) progress / (float) numticksrequired);

				while ( 100.0 * (float) csi->NumLocked () / (float) (CYPHER_WIDTH*CYPHER_HEIGHT) <= percentagedone )
					csi->CypherLock ();

			}
			
		}
		else if ( status == DECYPHER_FINISHED ) {

			SvbRemoveTask ( this );
			return;

		}

	}

}

void Decypher::CreateInterface ()
{

	if ( !IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );
		
		string stitle = "decypher_title " + to_string(pid);
        string sborder = "decypher_border " + to_string(pid);
        string sprogress = "decypher_progress " + to_string(pid);
        string sclose = "decypher_close " + to_string(pid);

		EclRegisterButton ( 265, 450, 20, 15, "", "Decypher", stitle );
		button_assignbitmap ( stitle, "software/dec.tif" );

		// Fix for decypher unpickability by NeoThermic

		EclRegisterButton ( 285, 450, 120, 15, "", "", sborder );
		EclRegisterButtonCallbacks ( sborder, BorderDraw, BorderClick, button_click, button_highlight );

		EclRegisterButton ( 285, 450, 120, 13, "Select target", "Shows the progress of the decypher", sprogress );
		EclRegisterButtonCallbacks ( sprogress, ProgressDraw, BorderClick, button_click, button_highlight );		

		EclRegisterButton ( 405, 450, 13, 13, "", "Close the Decypher (and stop)", sclose );		
		button_assignbitmaps ( sclose, "close.tif", "close_h.tif", "close_c.tif" );
		EclRegisterButtonCallback ( sclose, CloseClick );
		
	}

}

void Decypher::RemoveInterface ()
{

	if ( IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );

		string stitle = "decypher_title " + to_string(pid);
        string sborder = "decypher_border " + to_string(pid);
        string sprogress = "decypher_progress " + to_string(pid);
        string sclose = "decypher_close " + to_string(pid);

		EclRemoveButton ( stitle );
		EclRemoveButton ( sborder );
		EclRemoveButton ( sprogress );
		EclRemoveButton ( sclose );

	}

}

void Decypher::ShowInterface ()
{

	if ( !IsInterfaceVisible () ) CreateInterface ();

	int pid = SvbLookupPID ( this );

	string stitle = "decypher_title " + to_string(pid);
    string sborder = "decypher_border " + to_string(pid);
    string sprogress = "decypher_progress " + to_string(pid);
    string sclose = "decypher_close " + to_string(pid);

	EclButtonBringToFront ( stitle );
	EclButtonBringToFront ( sborder );
	EclButtonBringToFront ( sprogress );
	EclButtonBringToFront ( sclose );

}

bool Decypher::IsInterfaceVisible ()
{

	int pid = SvbLookupPID ( this );

	string stitle = "decypher_border " + to_string(pid);
	
	return ( EclGetButton (stitle) != nullptr );

}
