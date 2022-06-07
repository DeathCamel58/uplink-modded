
#ifdef WIN32
#include <windows.h>
#endif

#include <cstring>
#include <cstdio>

#include "soundgarden.h"
#include "redshirt.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/opengl_interface.h"

#include "game/scriptlibrary.h"

#include "interface/localinterface/phonedialler.h"

#include "interface/localinterface/worldmap/worldmap_interface.h"
#include "interface/localinterface/finance_interface.h"
#include "interface/remoteinterface/linksscreen_interface.h"

#include "mmgr.h"

// === Initialise static variables here =======================================


char PhoneDialler::buttonnames [][9] = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };


// ======================================================================================

PhoneDialler::PhoneDialler ()
{

	ip = "";
	infosup = "";
	nextscene = -1;
	lastupdate = 0;
	curnumber = -1;

}

PhoneDialler::~PhoneDialler ()
= default;

void PhoneDialler::Create ( int x, int y )
{

	if ( !IsVisible () ) {

		EclRegisterButton ( x - 10, y - 10, 120, 140, "", "dialler_bg" );
		EclRegisterButtonCallbacks ( "dialler_bg", BackgroundDraw, nullptr, nullptr, nullptr );

		EclRegisterButton (  x +  0,  y +  0,  29,  29, "1", "one" );
		EclRegisterButton (  x + 36,  y +  0,  29,  29, "2", "two" );
		EclRegisterButton (  x + 72,  y +  0,  29,  29, "3", "three" );
		EclRegisterButton (  x +  0,  y + 36,  29,  29, "4", "four" );
		EclRegisterButton (  x + 36,  y + 36,  29,  29, "5", "five" );
		EclRegisterButton (  x + 72,  y + 36,  29,  29, "6", "six" );
		EclRegisterButton (  x +  0,  y + 72,  29,  29, "7", "seven" );
		EclRegisterButton (  x + 36,  y + 72,  29,  29, "8", "eight" );
		EclRegisterButton (  x + 72,  y + 72,  29,  29, "9", "nine" );

		button_assignbitmaps ( "one", "dialler/one.tif", "dialler/one_h.tif", "dialler/one_h.tif" );
		button_assignbitmaps ( "two", "dialler/two.tif", "dialler/two_h.tif", "dialler/two_h.tif" );
		button_assignbitmaps ( "three", "dialler/three.tif", "dialler/three_h.tif", "dialler/three_h.tif" );
		button_assignbitmaps ( "four", "dialler/four.tif", "dialler/four_h.tif", "dialler/four_h.tif" );
		button_assignbitmaps ( "five", "dialler/five.tif", "dialler/five_h.tif", "dialler/five_h.tif" );
		button_assignbitmaps ( "six", "dialler/six.tif", "dialler/six_h.tif", "dialler/six_h.tif" );
		button_assignbitmaps ( "seven", "dialler/seven.tif", "dialler/seven_h.tif", "dialler/seven_h.tif" );
		button_assignbitmaps ( "eight", "dialler/eight.tif", "dialler/eight_h.tif", "dialler/eight_h.tif" );
		button_assignbitmaps ( "nine", "dialler/nine.tif", "dialler/nine_h.tif", "dialler/nine_h.tif" );

		EclRegisterButton ( x, y + 108, 101, 15, "", "dialler_number" );

	}

}

void PhoneDialler::Remove ()
{

	if ( IsVisible () ) {

		EclRemoveButton ( "dialler_bg" );

		EclRemoveButton ( "one" );
		EclRemoveButton ( "two" );
		EclRemoveButton ( "three" );
		EclRemoveButton ( "four" );
		EclRemoveButton ( "five" );
		EclRemoveButton ( "six" );
		EclRemoveButton ( "seven" );
		EclRemoveButton ( "eight" );
		EclRemoveButton ( "nine" );

		EclRemoveButton ( "dialler_number" );

	}

	curnumber = -1;

}

void PhoneDialler::DialNumber (int x, int y, const string& number, int scenario, string info )
{

	UplinkAssert ( !number.empty() )

	ip = number;

	if ( !info.empty() ) {
		infosup = info;
	}

	nextscene = scenario;

	lastupdate = 0;
	curnumber = 0;

	app->RegisterPhoneDialler ( this );

	Create ( x, y );

}

bool PhoneDialler::IsVisible () 
{

	return ( EclGetButton ( "dialler_number" ) != nullptr );

}

void PhoneDialler::UpdateDisplay ()
{

	Button *buttonDiallerNumber = nullptr;

	if ( curnumber != -1 && ( buttonDiallerNumber = EclGetButton ( "dialler_number" ) ) ) {
	
		if ( curnumber < ip.size() ) {

			char ch;
			do {
				ch = ip [ curnumber ];
				curnumber++;
			} while ( curnumber < ip.size() && !( ch >= '1' && ch <= '9' ) );

			if ( ch >= '1' && ch <= '9' ) {

				EclHighlightButton ( buttonnames [ ch - '1' ] );
				
				string caption = ip;
				caption [curnumber] = 0;
				buttonDiallerNumber->SetCaption ( caption );

				char filename [256];
				UplinkSnprintf ( filename, sizeof ( filename ), "sounds/%d.wav", ch - '0' )
				SgPlaySound ( RsArchiveFileOpen ( filename ), filename, true );

			}

		}
		else {

			if ( nextscene == 1 ) {          // scriptlibrary.cpp, void ScriptLibrary::Script31

				ScriptLibrary::RunScript ( 92 );

			}
			else if ( nextscene == 2 ) {     // scriptlibrary.cpp, void ScriptLibrary::Script35

				ScriptLibrary::RunScript ( 93 );

			}
			else if ( nextscene == 3 ) {

				WorldMapInterface::AfterPhoneDialler ( ip, infosup );

			}
			else if ( nextscene == 4 ) {

				FinanceInterface::AfterPhoneDialler ( ip, infosup );

			}
			else if ( nextscene == 5 ) {

				LinksScreenInterface::AfterPhoneDialler ( ip, infosup );

			}
			else {
				char message[ 64 ];
				UplinkSnprintf ( message, sizeof ( message ), "Unrecognised nextscene=%d", nextscene )
				UplinkAbort ( message )
			}

			curnumber = -1;

		}

	}
	else {

		curnumber = -1;

	}

	lastupdate = (int) EclGetAccurateTime ();

}

bool PhoneDialler::UpdateSpecial ()
{

    int timesincelastupdate = (int) ( EclGetAccurateTime () - lastupdate );

    if ( timesincelastupdate >= 110 ) {

		UpdateDisplay ();

	}

	return ( curnumber == -1 );

}

