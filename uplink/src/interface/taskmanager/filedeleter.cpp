	
#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

#include <sstream>


#include "eclipse.h"
#include "vanbakel.h"
#include "gucci.h"

#include "app/globals.h"
#include "app/opengl_interface.h"
#include "app/serialise.h"

#include "game/game.h"
#include "game/data/data.h"

#include "interface/interface.h"
#include "interface/taskmanager/taskmanager.h"
#include "interface/taskmanager/filedeleter.h"

#include "world/world.h"
#include "world/player.h"
#include "world/computer/databank.h"

#include "mmgr.h"


FileDeleter::FileDeleter () : UplinkTask ()
{

	source = nullptr;
	sourceindex = -1;
	status = FILEDELETER_OFF;
	numticksrequired = 0;
	progress = 0;
    remotefile = true;

}

FileDeleter::~FileDeleter ()
= default;

void FileDeleter::MoveTo ( int x, int y, int time_ms )
{

	UplinkTask::MoveTo ( x, y, time_ms );

	int pid = SvbLookupPID ( this );

	string stitle = "filedeleter_title " + to_string(pid);
    string sborder = "filedeleter_border " + to_string(pid);
    string sprogress = "filedeleter_progress " + to_string(pid);
    string sclose = "filedeleter_close " + to_string(pid);

	EclRegisterMovement ( stitle, x, y, time_ms);
	EclRegisterMovement ( sborder, x + 20, y, time_ms );
	EclRegisterMovement ( sprogress, x + 20, y + 1, time_ms );
	EclRegisterMovement ( sclose, x + 140, y + 1, time_ms );

	EclButtonBringToFront ( stitle );
	EclButtonBringToFront ( sborder );
	EclButtonBringToFront ( sprogress );
	EclButtonBringToFront ( sclose );

}

void FileDeleter::SetTarget (UplinkObject *uo, const string &uos, int uoi )
{

	/*

		uo  : DataBank object containing Data to be deleted
		uos : Name of button representing data
		uoi : index of data to be deleted

		*/

	if ( status == FILEDELETER_OFF ) {

		if ( uo->GetOBJECTID () == OID_DATABANK ) {

			// Databank selected

			if ( ((DataBank *) uo)->GetData (uoi) ) {

				source = (DataBank *) uo;
				sourceindex = uoi;

				Data *data = source->GetData (sourceindex);
				UplinkAssert (data)
				
				numticksrequired = TICKSREQUIRED_DELETE * data->size;
				progress = 0;

                remotefile = uos.find( "fileserverscreen") != string::npos;

				Button *button = EclGetButton ( uos );
				UplinkAssert (button)

				MoveTo ( button->x, button->y + button->height + 1, 1000 );

			}

		}

	}

}

void FileDeleter::BorderDraw ( Button *button, bool highlighted, bool clicked )
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

void FileDeleter::ProgressDraw ( Button *button, bool highlighted, bool clicked )
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

void FileDeleter::CloseClick ( Button *button )
{

	int pid;
	char bname [64];
    istringstream stream(button->name);
    stream >> bname >> pid;

	SvbRemoveTask ( pid );

}

void FileDeleter::BorderClick ( Button *button )
{

	int pid;
	char bname [64];
    istringstream stream(button->name);
    stream >> bname >> pid;

	game->GetInterface ()->GetTaskManager ()->SetTargetProgram ( pid );
	
}

void FileDeleter::Initialise ()
{
}

static bool FileDeleter_ReSetTargetProgram ( int pid )
{

	TaskManager *tm = game->GetInterface ()->GetTaskManager ();
	if ( tm->IsTargetProgramLast ( pid ) ) {

		string stitle = "filedeleter_title " + to_string(pid);
        string sborder = "filedeleter_border " + to_string(pid);
        string sprogress = "filedeleter_progress " + to_string(pid);
        string sclose = "filedeleter_close " + to_string(pid);

		int animationid;
		if ( ( animationid = EclIsNoCaptionChangeActive( stitle ) ) != -1 )
			EclRemoveAnimation ( animationid );

		if ( ( animationid = EclIsNoCaptionChangeActive( sborder ) ) != -1 )
			EclRemoveAnimation ( animationid );

		if ( ( animationid = EclIsNoCaptionChangeActive( sprogress ) ) != -1 )
			EclRemoveAnimation ( animationid );

		if ( ( animationid = EclIsNoCaptionChangeActive( sclose ) ) != -1 )
			EclRemoveAnimation ( animationid );

		tm->SetTargetProgram ( pid );
		return true;

	}

	return false;

}

void FileDeleter::Tick ( int n )
{

	if ( IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );
		string sprogress = "filedeleter_progress " + to_string(pid);

		if ( status == FILEDELETER_OFF ) {

			// Not downloading - look for a new target

			if ( source ) {

				// A new source file has been specified - start downloading it
				status = FILEDELETER_INPROGRESS;

				EclRegisterCaptionChange ( sprogress, "Deleting...", 0 );

			}
			else {

				FileDeleter_ReSetTargetProgram ( pid );

			}

		}
		else if ( status == FILEDELETER_INPROGRESS ) {

			UplinkAssert (source)

            // Check we are still connected
            if ( remotefile && !game->GetWorld ()->GetPlayer ()->IsConnected () ) {
                SvbRemoveTask (pid);
                return;
            }

			Data *data = source->GetData (sourceindex);
            if ( !data ) {
                SvbRemoveTask(pid);
                return;
            }

			// Delete in progress

			for ( int count = 0; count < n; ++count ) {

				++progress;
				UplinkAssert ( EclGetButton ( sprogress ) )
				EclGetButton ( sprogress )->width = (int) ( 120 * ( (float) progress / numticksrequired ) );
				EclDirtyButton ( sprogress );

				if ( progress >= numticksrequired ) {

					// Finished deleting now
					status = FILEDELETER_FINISHED;
					EclRegisterCaptionChange ( sprogress, "Finished" );

					// Delete the file			
					source->RemoveData (sourceindex);

					FileDeleter_ReSetTargetProgram ( pid );

					break;

				}
			
			}

		}
		else if ( status == FILEDELETER_FINISHED ) {

			//status = FILEDELETER_OFF;
			//SvbRemoveTask ( this );

			// Don't close it, put it back in it's initial state

			source = nullptr;
			sourceindex = -1;
			status = FILEDELETER_OFF;
			numticksrequired = 0;
			progress = 0;
			remotefile = true;

			UplinkAssert ( EclGetButton ( sprogress ) )
			EclGetButton ( sprogress )->width = 120;
			EclRegisterCaptionChange ( sprogress, "Select target" );
			EclDirtyButton ( sprogress );

			FileDeleter_ReSetTargetProgram ( pid );

		}

	}

}

void FileDeleter::CreateInterface ()
{

	if ( !IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );

		string stitle = "filedeleter_title " + to_string(pid);
        string sborder = "filedeleter_border " + to_string(pid);
        string sprogress = "filedeleter_progress " + to_string(pid);
        string sclose = "filedeleter_close " + to_string(pid);

		EclRegisterButton ( 265, 450, 20, 15, "", "File Deleter", stitle );
		button_assignbitmap ( stitle, "software/del.tif" );

		EclRegisterButton ( 285, 450, 120, 15, "", "", sborder );
		EclRegisterButtonCallbacks ( sborder, BorderDraw, BorderClick, button_click, button_highlight );

		EclRegisterButton ( 285, 450, 120, 13, "Select target", "Shows the progress of the delete", sprogress );
		EclRegisterButtonCallbacks ( sprogress, ProgressDraw, BorderClick, button_click, button_highlight );		

		EclRegisterButton ( 405, 450, 13, 13, "", "Close the FileDeleter (and stop deleting)", sclose );		
		button_assignbitmaps ( sclose, "close.tif", "close_h.tif", "close_c.tif" );
		EclRegisterButtonCallback ( sclose, CloseClick );
		
	}

}

void FileDeleter::RemoveInterface ()
{

	if ( IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );

		string stitle = "filedeleter_title " + to_string(pid);
        string sborder = "filedeleter_border " + to_string(pid);
        string sprogress = "filedeleter_progress " + to_string(pid);
        string sclose = "filedeleter_close " + to_string(pid);

		EclRemoveButton ( stitle );
		EclRemoveButton ( sborder );
		EclRemoveButton ( sprogress );
		EclRemoveButton ( sclose );

	}

}

void FileDeleter::ShowInterface ()
{

	if ( !IsInterfaceVisible () ) CreateInterface ();

	int pid = SvbLookupPID ( this );

	string stitle = "filedeleter_title " + to_string(pid);
    string sborder = "filedeleter_border " + to_string(pid);
    string sprogress = "filedeleter_progress " + to_string(pid);
    string sclose = "filedeleter_close " + to_string(pid);

	EclButtonBringToFront ( stitle );
	EclButtonBringToFront ( sborder );
	EclButtonBringToFront ( sprogress );
	EclButtonBringToFront ( sclose );

}

bool FileDeleter::IsInterfaceVisible ()
{

	int pid = SvbLookupPID ( this );

	string stitle = "filedeleter_border " + to_string(pid);
	
	return ( EclGetButton (stitle) != nullptr );

}
