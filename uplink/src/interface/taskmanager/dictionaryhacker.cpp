
#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

#include <GL/glu.h> /*_glu_extention_library_*/

#include <cstring>
#include <cstdio>
#include <sstream>

#include "soundgarden.h"
#include "gucci.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/opengl_interface.h"
#include "app/serialise.h"

#include "game/game.h"
#include "game/data/data.h"

#include "interface/interface.h"
#include "interface/remoteinterface/remoteinterface.h"
#include "interface/remoteinterface/remoteinterfacescreen.h"
#include "interface/remoteinterface/passwordscreen_interface.h"
#include "interface/taskmanager/taskmanager.h"
#include "interface/taskmanager/dictionaryhacker.h"

#include "world/world.h"
#include "world/computer/computer.h"
#include "world/computer/recordbank.h"
#include "world/computer/computerscreen/passwordscreen.h"
#include "world/computer/computerscreen/useridscreen.h"
#include "world/player.h"

#include "mmgr.h"


DictionaryHacker::DictionaryHacker () : UplinkTask ()
{

	currentindex = 0;	
	password = username = nullptr;
	status = DICTIONARYHACKER_WAITING;

	numticksrequired = 0;
	progress = 0;

	targetstring = "";

}

DictionaryHacker::~DictionaryHacker()
= default;

void DictionaryHacker::Initialise ()
{
}

void DictionaryHacker::Tick ( int n )
{

	if ( IsInterfaceVisible () ) {

		if ( status == DICTIONARYHACKER_WAITING ) {

			// Don't do anything

		}
		else if ( status == DICTIONARYHACKER_INPROGRESS ) {

			for ( int count = 0; count < n; ++count ) {

				// First check to make sure we are still looking at the same screen
				// i.e the player has not logged out / moved / got past it already
			
				if ( !EclGetButton ( targetstring ) ) {				

					// Terminate this task
					SvbRemoveTask ( this );			

					// WARNING - This instance has been deleted - do not access any more data
					return;

				}

				// Test to see if we have checked all possible words
				if ( currentindex >= game->GetWorld ()->passwords.Size () ) {

					// Terminate this task
					SvbRemoveTask ( this );			

					// WARNING - This instance has been deleted - do not access any more data
					return;

				}

				++progress;
				if ( progress >= numticksrequired ) {

					int numwords = (int)((float) progress / numticksrequired);

					for ( int wordno = 0; wordno < numwords; ++wordno ) {

						if ( game->GetWorld ()->GetPassword ( currentindex ) ) {

							// Test the current word against the password to see if it is correct
							if ( game->GetWorld ()->GetPassword ( currentindex ) == password ) {

								// The full word has been discovered - enter it into the password screen

								if ( targetobject->GetOBJECTID () == OID_PASSWORDSCREEN ) {
					
									EclRegisterCaptionChange ( "passwordscreen_password", password );

								}
								else if ( targetobject->GetOBJECTID () == OID_USERIDSCREEN ) {

									EclRegisterCaptionChange ( "useridscreen_code", password );

								}

								// Also add it into the player's code list
								string newcode;
                                if ( !username.empty() ) Computer::GenerateAccessCode(username, password, newcode);
                                else Computer::GenerateAccessCode(password, newcode);

								game->GetWorld ()->GetPlayer ()->GiveCode ( game->GetWorld ()->GetPlayer ()->remotehost, newcode );

								SvbRemoveTask ( this );

								// WARNING - This instance has been deleted - do not access any more data
								return;
									
							}

						}

						// This is not the correct word - move onto the next
						++currentindex;					

					}

					progress = 0;

				}

			}

			int percentage = (int)((float) currentindex / (float) game->GetWorld ()->passwords.Size () * 100);
			string caption = "\nTrying\nWordlist..." + to_string(percentage) + "%%\n-> ";
			for ( int i = currentindex; i < currentindex + 10; ++i ) {
				if ( game->GetWorld ()->GetPassword (i) ) {
					caption += game->GetWorld ()->GetPassword ( i );
					caption += "\n";
				}
			}			
			string bname = "dictionaryhacker " + to_string( SvbLookupPID (this) );

			Button *button = EclGetButton ( bname );
			UplinkAssert(button)
			button->SetCaption ( caption );

		}

	}

}

void DictionaryHacker::DictionaryHackerDraw ( Button *button, bool highlighted, bool clicked )
{
	
	//textbutton_draw ( button, highlighted, clicked );

	glBegin ( GL_QUADS );

		glColor3ub ( 0,  14,  59  );			glVertex2i ( button->x, button->y );
		glColor3ub ( 36, 72,  146 );			glVertex2i ( button->x + button->width, button->y );
		glColor3ub ( 82, 134, 206 );			glVertex2i ( button->x + button->width, button->y + button->height );
		glColor3ub ( 73, 122, 194 );			glVertex2i ( button->x, button->y + button->height );

	glEnd ();

	text_draw ( button, highlighted, clicked );

	if ( highlighted || clicked ) 
		border_draw ( button );

}

void DictionaryHacker::DictionaryHackerClick ( Button *button )
{

	int pid;
	char bname [64];
    istringstream stream(button->name);
    stream >> bname >> pid;

	game->GetInterface ()->GetTaskManager ()->SetTargetProgram ( pid );

}

void DictionaryHacker::SetTarget (UplinkObject *uo, const string &uos, int uoi )
{

	/*

		uo  : Target PasswordScreen or UserIDScreen object
		uos : name of button with password in
		uoi : unused

		*/

	targetobject = uo;
	if ( !targetstring.empty() ) {
		targetstring = "";
	}
	if ( !uos.empty() ) {
		targetstring = uos;
	}
	targetint    = uoi;

	Computer *comp = nullptr;

	if ( uo->GetOBJECTID () == OID_PASSWORDSCREEN ) {

		auto *ps = (PasswordScreen *) uo;
        comp = ps->GetComputer ();
	    UplinkAssert (comp)

		size_t length = strlen ( ps->password );
		size_t passwordsize = length+1;
		password = ps->password;
		status = DICTIONARYHACKER_INPROGRESS;

		username = "";

		numticksrequired = TICKSREQUIRED_DICTIONARYHACKER;
		progress = 0;

		if ( comp->security.IsRunning_Monitor () ) 	
			game->GetWorld ()->GetPlayer ()->GetConnection ()->BeginTrace ();

	}
	else if ( uo->GetOBJECTID () == OID_USERIDSCREEN ) {

		auto *uid = (UserIDScreen *) uo;
        comp = uid->GetComputer ();
	    UplinkAssert (comp)

		// If the player has not entered a UserID then do this for admin
		// Then grab the target name from the useridscreen box

		UplinkAssert ( EclGetButton ( "useridscreen_name" ) )
		if ( EclGetButton ( "useridscreen_name" )->caption.empty() )
			EclGetButton ( "useridscreen_name" )->SetCaption ( "admin" );
		string name = EclGetButton ( "useridscreen_name" )->caption;

		username = name;

		// Look up this user name in this computer's records
			
		Record *rec = comp->recordbank.GetRecordFromName ( name );
		char *passwd = nullptr;

		// It's possible that RECORDBANK_PASSWORD doesn't exists,
		// the accounts in the Global Criminal Database don't have it.

		if ( rec ) {
			passwd = rec->GetField ( RECORDBANK_PASSWORD );
		}

		if ( rec && passwd ) {

			size_t length = strlen ( passwd );
			size_t passwordsize = length+1;
			password = passwd;

			numticksrequired = TICKSREQUIRED_DICTIONARYHACKER;
			progress = 0;

			status = DICTIONARYHACKER_INPROGRESS;

			if ( comp->security.IsRunning_Monitor () ) 	
				game->GetWorld ()->GetPlayer ()->GetConnection ()->BeginTrace ();

		}
		else {		

			// The user name is not recognised
			SvbRemoveTask ( this );
			return;

		}		

	}

}

void DictionaryHacker::CloseClick ( Button *button )
{

	int pid;
	char bname [64];
    istringstream stream(button->name);
    stream >> bname >> pid;

	SvbRemoveTask ( pid );

}


void DictionaryHacker::CreateInterface ()
{

	if ( !IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );

		string bname = "dictionaryhacker " + to_string( pid );
	
		string border1 = "dictionaryhacker_b1 " + to_string( pid );
		string border2 = "dictionaryhacker_b2 " + to_string( pid );
		string border3 = "dictionaryhacker_b3 " + to_string( pid );

		EclRegisterButton ( 0, 50, 100, 200, "", "", bname );
		EclRegisterButtonCallbacks ( bname, DictionaryHackerDraw, DictionaryHackerClick, button_click, button_highlight );
		EclRegisterCaptionChange ( bname, "Select target" );
//		EclButtonSendToBack ( bname );

		EclRegisterButton ( 0, 45, 105, 4, "", "", border1 );
		EclRegisterButtonCallbacks ( border1, buttonborder_draw, nullptr, nullptr, nullptr );

		EclRegisterButton ( 101, 45, 4, 210, "", "", border2 );
		EclRegisterButtonCallbacks ( border2, buttonborder_draw, nullptr, nullptr, nullptr );

		EclRegisterButton ( 0, 251, 105, 4, "", "", border3 );
		EclRegisterButtonCallbacks ( border3, buttonborder_draw, nullptr, nullptr, nullptr );


		string closename = "dictionaryhacker_close " + to_string( pid );

		EclRegisterButton ( 85, 51, 13, 13, "X", "Close the dictionary hacker", closename );
		button_assignbitmaps ( closename, "close.tif", "close_h.tif", "close_c.tif" );
		EclRegisterButtonCallback ( closename, CloseClick );
		
	}

}

void DictionaryHacker::RemoveInterface ()
{

	if ( IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );

		string bname = "dictionaryhacker " + to_string( pid );

		string border1 = "dictionaryhacker_b1 " + to_string( pid );
        string border2 = "dictionaryhacker_b2 " + to_string( pid );
        string border3 = "dictionaryhacker_b3 " + to_string( pid );

		EclRemoveButton ( bname );
		EclRemoveButton ( border1 );
		EclRemoveButton ( border2 );
		EclRemoveButton ( border3 );


		string closename = "dictionaryhacker_close " + to_string( pid );
		EclRemoveButton ( closename );

	}

}

void DictionaryHacker::ShowInterface ()
{

	if ( !IsInterfaceVisible () ) CreateInterface ();

	int pid = SvbLookupPID ( this );

	string bname = "dictionaryhacker " + to_string( pid );

	string border1 = "dictionaryhacker_b1 " + to_string( pid );
    string border2 = "dictionaryhacker_b2 " + to_string( pid );
    string border3 = "dictionaryhacker_b3 " + to_string( pid );

	EclButtonBringToFront ( bname );
	EclButtonBringToFront ( border1 );
	EclButtonBringToFront ( border2 );
	EclButtonBringToFront ( border3 );

	string closename = "dictionaryhacker_close " + to_string( pid );
	EclButtonBringToFront ( closename );

}

bool DictionaryHacker::IsInterfaceVisible ()
{

	string bname = "dictionaryhacker " + to_string( SvbLookupPID (this) );

	return ( EclGetButton ( bname ) != nullptr );

}

