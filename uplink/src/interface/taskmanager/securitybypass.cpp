
#include <sstream>
#include "vanbakel.h"
#include "eclipse.h"

#include "app/globals.h"
#include "app/opengl_interface.h"
#include "app/uplinkobject.h"

#include "game/game.h"

#include "world/world.h"
#include "world/player.h"
#include "world/computer/security.h"
#include "world/computer/securitysystem.h"


#include "interface/interface.h"
#include "interface/taskmanager/taskmanager.h"
#include "interface/taskmanager/securitybypass.h"
#include "interface/localinterface/localinterface.h"

#include "mmgr.h"


void SecurityBypass::PauseClick ( Button *button )
{

	UplinkAssert (button)

	char name [32];
	int pid;
    istringstream stream(button->name);
    stream >> name >> pid;

	auto *pb = (SecurityBypass *) SvbGetTask ( pid );
	UplinkAssert (pb)

	if		( pb->status == 1 ) pb->PauseCurrentBypass ();
	else if ( pb->status == 2 ) pb->ResumeCurrentBypass ();

}

void SecurityBypass::CloseClick ( Button *button )
{

	UplinkAssert (button)

	char name [32];
	int pid;
    istringstream stream(button->name);
    stream >> name >> pid;

	auto *pb = (SecurityBypass *) SvbGetTask ( pid );
	UplinkAssert (pb)
	pb->EndCurrentBypass ();

	SvbRemoveTask ( pid );

}

void SecurityBypass::BypassClick ( Button *button )
{

	UplinkAssert (button)

	char name [32];
	int pid;
    istringstream stream(button->name);
    stream >> name >> pid;

	game->GetInterface ()->GetTaskManager ()->SetTargetProgram ( pid );

}

void SecurityBypass::StatusLightDraw ( Button *button, bool highlighted, bool clicked )
{

	char name [32];
	int pid;
    istringstream stream(button->name);
    stream >> name >> pid;

	if ( ShouldDraw ( pid ) ) {

		auto *pb = (SecurityBypass *) SvbGetTask ( pid );
		UplinkAssert (pb)

		if ( pb->status > 0 ) {

			UplinkAssert ( pb->targetobject )
			SecuritySystem *ss = ((Security *) pb->targetobject)->GetSystem (pb->targetint);
			UplinkAssert (ss)

			if ( ss->bypassed || !ss->enabled )													//
				button->image_standard->Draw ( button->x, button->y );							//
																								// C L E V E R ! ! ! 
			else																				//
				button->image_highlighted->Draw ( button->x, button->y );						//

		}
		else {

			button->image_highlighted->Draw ( button->x, button->y );						

		}

	}

}

void SecurityBypass::ImageButtonDraw ( Button *button, bool highlighted, bool clicked )
{

	char name [32];
	int pid;
    istringstream stream(button->name);
    stream >> name >> pid;

	if ( ShouldDraw ( pid ) ) {

		imagebutton_draw ( button, highlighted, clicked );

	}

}

bool SecurityBypass::ShouldDraw ( int pid )
{

	auto *pb = (SecurityBypass *) SvbGetTask ( pid );
	if ( !pb || pb->status == 0 ) // 0 = static, 1 = working
		return true;

	LocalInterface *li = game->GetInterface()->GetLocalInterface();
	UplinkTask *ut;
	if ( ( li && li->InScreen() == SCREEN_ANALYSER && li->IsVisible() ) ||
	     ( game->IsRunning() && ( ut = game->GetInterface()->GetTaskManager()->GetTargetProgram() ) &&
	       ut->GetPID() == pid ) ) {

		return true;

	}

	return false;

}

SecurityBypass::SecurityBypass () : UplinkTask ()
{

	status = 0;
		
}

SecurityBypass::SecurityBypass ( int newTYPE )
{

	status = 0;
	SetTYPE ( newTYPE );

}

SecurityBypass::~SecurityBypass ()
= default;

void SecurityBypass::SetTYPE ( int newTYPE )
{

	TYPE = newTYPE;

}

void SecurityBypass::PauseCurrentBypass ()
{

	if ( status == 1 ) {

		UplinkAssert ( targetobject )
		UplinkAssert ( ((Security *) targetobject)->GetSystem (targetint) )
		((Security *) targetobject)->GetSystem (targetint)->EndBypass ();
		
		status = 2;

	}
	else {

		cout << "WARNING : SecurityBypass::PauseCurrentBypass, no bypass in existence!" << endl;

	}

}

void SecurityBypass::ResumeCurrentBypass ()
{

	if ( status == 2 ) {

		UplinkAssert ( targetobject )
		UplinkAssert ( ((Security *) targetobject)->GetSystem (targetint) )
		((Security *) targetobject)->GetSystem (targetint)->Bypass ();
		
		status = 1;

	}
	else {

		cout << "WARNING : SecurityBypass::ResumeCurrentBypass, not paused to begin with!" << endl;

	}

}

void SecurityBypass::EndCurrentBypass ()
{

	if ( status > 0 ) {

		UplinkAssert ( targetobject )
		UplinkAssert ( ((Security *) targetobject)->GetSystem (targetint) )
		((Security *) targetobject)->GetSystem (targetint)->EndBypass ();
		
		targetobject = nullptr;
		targetint = 0;
		status = 0;

	}

}

void SecurityBypass::SetTarget (UplinkObject *uo, const string &uos, int uoi )
{

	/*
		uo  : Security object that belongs to target computer
		uos : Button name representing system in connection analyser
		uoi : index of security system 

		*/

	UplinkAssert (uo)

	if ( uo->GetID () == "SECUR" ) {

		UplinkAssert ( ((Security *) uo)->GetSystem (uoi) )

		if ( ((Security *) uo)->GetSystem (uoi)->TYPE == TYPE ) {

			//
			// If we were already running, stop
			//

			EndCurrentBypass ();

			//
			// Move the button into place
			//

			Button *b = EclGetButton ( uos );
			UplinkAssert (b)
			MoveTo ( b->x, b->y + b->height, 1000 );

			//
			// Store our target
			//

			targetobject = uo;
			targetint = uoi;

			UplinkStrncpy ( ip, game->GetWorld ()->GetPlayer ()->remotehost, sizeof ( ip ) )


			//
			// Start it running
			//

			if ( version >= ((Security *) targetobject)->GetSystem (targetint)->level ) {

				((Security *) targetobject)->GetSystem (targetint)->Bypass ();
				status = 1;

			}
			else {

				status = 0;

			}

		}

	}

}

void SecurityBypass::MoveTo ( int x, int y, int time_ms )
{

	// X and Y of bottom left corner

	if ( IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );

		string main = "sbypass_main " + to_string(pid);
		string pause = "sbypass_pause " + to_string(pid);
		string close = "sbypass_close " + to_string(pid);
		string title = "sbypass_title " + to_string(pid);
		string status = "sbypass_status " + to_string(pid);

		EclButtonBringToFront ( main );
		EclButtonBringToFront ( pause );
		EclButtonBringToFront ( close );
		EclButtonBringToFront ( title );
		EclButtonBringToFront ( status );
		
//		EclRegisterMovement ( main,   x,	  y - 32, 1000, MOVE_STRAIGHTLINE );
///		EclRegisterMovement ( pause,  x + 1,  y - 31, 1000, MOVE_STRAIGHTLINE );
//		EclRegisterMovement ( close,  x + 19, y - 31, 1000, MOVE_STRAIGHTLINE );
//		EclRegisterMovement ( title,  x + 1,  y - 13, 1000, MOVE_STRAIGHTLINE );
//		EclRegisterMovement ( status, x + 19, y - 13, 1000, MOVE_STRAIGHTLINE );

		EclRegisterMovement ( main,   x,	  y - 32, time_ms );
		EclRegisterMovement ( pause,  x + 1,  y - 31, time_ms );
		EclRegisterMovement ( close,  x + 19, y - 31, time_ms );
		EclRegisterMovement ( title,  x + 1,  y - 13, time_ms );
		EclRegisterMovement ( status, x + 19, y - 13, time_ms );

	}

}

void SecurityBypass::Initialise ()
{
}

void SecurityBypass::Tick ( int n )
{

	//
	// Switch off if we have disconnected
	//

	if ( status > 0 ) {

		if ( strcmp ( game->GetWorld ()->GetPlayer ()->remotehost, ip ) != 0 ) {

			EndCurrentBypass ();

			SvbRemoveTask ( SvbLookupPID ( this ) );

		}

	}

}

void SecurityBypass::CreateInterface ()
{

	if ( !IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );

		string main = "sbypass_main " + to_string(pid);
		string pause = "sbypass_pause " + to_string(pid);
		string close = "sbypass_close " + to_string(pid);
		string title = "sbypass_title " + to_string(pid);
		string status = "sbypass_status " + to_string(pid);


		/********** Patched by François Gagné **********/
		// To be complete, each type would need a picture just like the proxy bypass (software/proxy.tif)

		string bypassCaption, pauseCaption, closeCaption;
		switch ( TYPE ) {
		  case SECURITY_TYPE_PROXY:
			bypassCaption = "Click to use this proxy bypass";
			pauseCaption = "Pause the proxy bypass";
			closeCaption = "Close the proxy bypass";
			break;
		  case SECURITY_TYPE_FIREWALL:
			bypassCaption = "Click to use this firewall bypass";
			pauseCaption = "Pause the firewall bypass";
			closeCaption = "Close the firewall bypass";
			break;
		  case SECURITY_TYPE_ENCRYPTION:
			bypassCaption = "Click to use this encryption bypass";
			pauseCaption = "Pause the encryption bypass";
			closeCaption = "Close the encryption bypass";
			break;
		  case SECURITY_TYPE_MONITOR:
			bypassCaption = "Click to use this monitor bypass";
			pauseCaption = "Pause the monitor bypass";
			closeCaption = "Close the monitor bypass";
			break;
		  default: // SECURITY_TYPE_NONE
			bypassCaption = "Click to use this bypass";
			pauseCaption = "Pause the bypass";
			closeCaption = "Close the bypass";
		}

		EclRegisterButton ( 100, 100, 32, 32, "Bypass", bypassCaption, main ); 
		button_assignbitmaps ( main, "software/proxybypass.tif", "software/proxybypass.tif", "software/proxybypass.tif" );
		//EclRegisterButtonCallback ( main, BypassClick );
		EclRegisterButtonCallbacks ( main, ImageButtonDraw, BypassClick, nullptr, nullptr );
		
		EclRegisterButton ( 101, 101, 12, 12, "P", pauseCaption, pause );
		button_assignbitmaps ( pause, "software/pause.tif", "software/pause_h.tif", "software/pause_c.tif" );
		//EclRegisterButtonCallback ( pause, PauseClick );
		EclRegisterButtonCallbacks ( pause, ImageButtonDraw, PauseClick, nullptr, nullptr );

		EclRegisterButton ( 119, 101, 12, 12, "", closeCaption, close );
		button_assignbitmaps ( close, "close.tif", "close_h.tif", "close_c.tif" );
		EclGetButton ( close )->image_standard->Scale	 ( 12, 12 );
		EclGetButton ( close )->image_highlighted->Scale ( 12, 12 );
		EclGetButton ( close )->image_clicked->Scale	 ( 12, 12 );
		//EclRegisterButtonCallback ( close, CloseClick );
		EclRegisterButtonCallbacks ( close, ImageButtonDraw, CloseClick, nullptr, nullptr );

		EclRegisterButton ( 101, 119, 12, 12, "", "", title );
		button_assignbitmap ( title, "software/proxy.tif" );
		//EclRegisterButtonCallbacks ( title, imagebutton_draw, nullptr, nullptr, nullptr );
		EclRegisterButtonCallbacks ( title, ImageButtonDraw, nullptr, nullptr, nullptr );

		EclRegisterButton ( 119, 119, 12, 12, "", "", status );
		button_assignbitmaps ( status, "software/greenlight.tif", "software/redlight.tif", "software/redlight.tif" );
		EclRegisterButtonCallbacks ( status, StatusLightDraw, nullptr, nullptr, nullptr );

	}

}

void SecurityBypass::RemoveInterface ()
{

	if ( IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );

		string main = "sbypass_main %d" + to_string(pid);
		string pause = "sbypass_pause %d" + to_string(pid);
		string close = "sbypass_close %d" + to_string(pid);
		string title = "sbypass_title %d" + to_string(pid);
		string status = "sbypass_status %d" + to_string(pid);
		
		EclRemoveButton ( main );
		EclRemoveButton ( pause );
		EclRemoveButton ( close );
		EclRemoveButton ( title );
		EclRemoveButton ( status );
		
	}

}

void SecurityBypass::ShowInterface ()
{

	if ( !IsInterfaceVisible () ) CreateInterface ();

	int pid = SvbLookupPID ( this );

	string main = "sbypass_main " + to_string(pid);
	string pause = "sbypass_pause " + to_string(pid);
	string close = "sbypass_close " + to_string(pid);
	string title = "sbypass_title " + to_string(pid);
	string status = "sbypass_status " + to_string(pid);
	
	/********** Patched by François Gagné **********/
	if ( ShouldDraw ( pid ) ) {

		EclButtonBringToFront ( main );
		EclButtonBringToFront ( pause );
		EclButtonBringToFront ( close );
		EclButtonBringToFront ( title );
		EclButtonBringToFront ( status );

	} else {

		EclButtonSendToBack ( status );
		EclButtonSendToBack ( title );
		EclButtonSendToBack ( close );
		EclButtonSendToBack ( pause );
		EclButtonSendToBack ( main );

	}

}

bool SecurityBypass::IsInterfaceVisible ()
{

	int pid = SvbLookupPID ( this );
	string main = "sbypass_main " + to_string(pid);

	return ( EclGetButton ( main ) != nullptr );

}
