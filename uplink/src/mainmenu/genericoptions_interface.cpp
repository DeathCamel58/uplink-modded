
#ifdef WIN32
#include <windows.h>
#endif

#include <sstream>

#include "soundgarden.h"
#include "redshirt.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/opengl_interface.h"

#include "options/options.h"

#include "mainmenu/mainmenu.h"
#include "mainmenu/genericoptions_interface.h"

#include "mmgr.h"


void GenericOptionsInterface::ReturnToMainMenuClick ( Button *button )
{

	app->GetMainMenu ()->RunScreen ( MAINMENU_OPTIONS );

}

void GenericOptionsInterface::ApplyClick ( Button *button )
{

	auto *thisint = (GenericOptionsInterface *) app->GetMainMenu ()->GetMenuScreen ();
	UplinkAssert (thisint)

	int index = 0;
	string name1 = "generic_option " + to_string(index);

	while ( EclGetButton ( name1 ) != nullptr ) {

		// Look up the next option button

		string name2 = "generic_value " + to_string(index);

		Button *namebutton = EclGetButton ( name1 );
		Button *valuebutton = EclGetButton ( name2 );

		UplinkAssert (namebutton)
		UplinkAssert (valuebutton)

		// Extract the values

		char optionname [64];
		int newvalue;
		UplinkSnprintf ( optionname, sizeof ( optionname ), "%s_%s", thisint->optionTYPE, namebutton->caption.c_str() )
        string unused;
        istringstream stream(valuebutton->caption);
        stream >> newvalue;
		
		// If they've changed, update them

		Option *thisoption = app->GetOptions ()->GetOption (optionname);

		if ( thisoption && thisoption->value != newvalue ) 
			thisint->ChangeOptionValue ( optionname, newvalue );

		// Next button

		++index;
		name1 = "generic_option " + to_string(index);

	}

}

void GenericOptionsInterface::ToggleBoxDraw ( Button *button, bool highlighted, bool clicked )
{

	UplinkAssert (button)

	if ( button->caption [0] == '0' )
		imagebutton_draw ( button, false, false );

	else
		imagebutton_draw ( button, true, true );

}

void GenericOptionsInterface::ToggleBoxClick ( Button *button )
{

	UplinkAssert (button)

	// Get the button index

	int buttonindex;
    string unused;
    istringstream stream(button->name);
    stream >> unused >> buttonindex;

	// Grab the appropriate button

	string name2 = "generic_value " + to_string(buttonindex);
	Button *button2 = EclGetButton ( name2 );
	UplinkAssert (button2)

	// Read the value from the button
	
	if ( button2->caption [0] == '0' ) 
		button2->SetCaption ( "1" );

	else
		button2->SetCaption ( "0" );

}


GenericOptionsInterface::GenericOptionsInterface ()
= default;

GenericOptionsInterface::~GenericOptionsInterface ()
= default;

void GenericOptionsInterface::ChangeOptionValue ( char *option, int newvalue )
{

	app->GetOptions ()->SetOptionValue ( option, newvalue );

	//
	// Deal with specific options
	//

	if ( strcmp ( option, "sound_musicenabled" ) == 0 ) {

		if ( newvalue == 0 ) 	SgPlaylist_Stop ();
		else					SgPlaylist_Play ("main");    //SgPlaylist_Play ("action");

	}

}

void GenericOptionsInterface::ExitGameClick ( Button *button )
{

	app->Close ();

}

void GenericOptionsInterface::SetOptionTYPE ( char *newtype )
{

	UplinkStrncpy ( optionTYPE, newtype, sizeof ( optionTYPE ) )

	//
	// Build a list of all options of this type
	//

	LList <Option *> *options = app->GetOptions ()->GetAllOptions ( newtype, false );
	
	//
	// Create Title
	//

	int screenw = app->GetOptions ()->GetOptionValue ("graphics_screenwidth");
	int screenh = app->GetOptions ()->GetOptionValue ("graphics_screenheight");

	char title [64];
	UplinkSnprintf ( title, sizeof ( title ), "%s options", optionTYPE )
	EclRegisterButton ( screenw - 210, screenh - 40, 200, 15, title, "Close this options screen", "generic_title" );
	EclRegisterButtonCallback ( "generic_title", ReturnToMainMenuClick );
	EclRegisterMovement ( "generic_title", screenw - 210, screenh - 100 - options->Size () * 20, 500 );

	//
	// Create Each option name and text edit box
	//

	for ( int i = 0; i < options->Size (); ++i ) {
	
		Option *option = options->GetData (i);
		UplinkAssert (option)

		string name1 = "generic_option " + to_string(i);
        string name2 = "generic_value " + to_string(i);

		char *optionname = strchr ( option->name, '_' ) + 1;
		
		string value = to_string( option->value );

		EclRegisterButton ( screenw - 210, screenh - 40, 150, 15, optionname, option->tooltip, name1 );
		EclRegisterButtonCallbacks ( name1, button_draw, ToggleBoxClick, button_click, button_highlight );
        
		if ( option->yesorno ) {

			EclRegisterButton ( screenw - 60, screenh - 40, 15, 15, value, "click to Enable or Disable this option", name2 );
			button_assignbitmaps ( name2, "mainmenu/optionno.tif", "mainmenu/optionyes.tif", "mainmenu/optionyes.tif" );
			EclRegisterButtonCallbacks ( name2, ToggleBoxDraw, ToggleBoxClick, button_click, button_highlight );

		}
		else {

			EclRegisterButton ( screenw - 60, screenh - 40, 50, 15, value, "enter the new value here", name2 );
			EclRegisterButtonCallbacks ( name2, textbutton_draw, nullptr, button_click, button_highlight );
            EclMakeButtonEditable ( name2 );

		}

		int timems = (int) ( 500 * ((float) (i + 1) / (float) options->Size ()) );
		EclRegisterMovement ( name1, screenw - 210, screenh - 90 - i * 20, timems );
		EclRegisterMovement ( name2, screenw - 60, screenh - 90 - i * 20, timems );

	}

	delete options;

	// 
	// Create "Apply" button
	//

	EclRegisterButton ( screenw - 210, screenh - 40, 145, 15, "Apply Changes", "Apply all changes", "generic_apply" );
	EclRegisterButtonCallback ( "generic_apply", ApplyClick );

}

void GenericOptionsInterface::Create ()
{
	
	if ( !IsVisible () ) {

		MainMenuScreen::Create ();

		int screenw = app->GetOptions ()->GetOptionValue ("graphics_screenwidth");
		int screenh = app->GetOptions ()->GetOptionValue ("graphics_screenheight");

		// Close button

		EclRegisterButton ( screenw - 60, screenh - 40, 50, 15, "Close", "Return to Options menu", "generic_return" );
		EclRegisterButtonCallback ( "generic_return", ReturnToMainMenuClick );
		
	}

}

void GenericOptionsInterface::Remove ()
{

	if ( IsVisible () ) {

		EclRemoveButton ( "generic_return" );
		EclRemoveButton ( "generic_title" );

		int index = 0;
		string name1 = "generic_option " + to_string(index);

		while ( EclGetButton ( name1 ) != nullptr ) {

			string name2 = "generic_value " + to_string(index);

			EclRemoveButton ( name1 );
			EclRemoveButton ( name2 );

			++index;
			name1 = "generic_option " + to_string(index);

		}

		EclRemoveButton ( "generic_apply" );

	}

}

void GenericOptionsInterface::Update ()
{
}

bool GenericOptionsInterface::IsVisible ()
{

	return ( EclGetButton ( "generic_return" ) != nullptr );

}


int  GenericOptionsInterface::ScreenID ()
{

	return MAINMENU_GENERICOPTIONS;

}

