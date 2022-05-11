// -*- tab-width:4; c-file-mode:"cc-mode" -*-
// Options.cpp: implementation of the Options class.
//
//////////////////////////////////////////////////////////////////////

#include <strstream>

#include <fstream>
#include "app/dos2unix.h"

#include "gucci.h"
#include "redshirt.h"

#include "app/globals.h"
#include "app/app.h"
#include "app/miscutils.h"
#include "app/serialise.h"
#include "app/opengl.h"

#include "options/options.h"

#include "mmgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static ColourOption getColourDefault ( 0.0, 0.0, 0.0 );


Options::Options()
{
    themeName = "graphics";
}

Options::~Options()
{

	DeleteBTreeData ( (BTree <UplinkObject *> *) &options );


	DArray <ColourOption *> *cols = colours.ConvertToDArray ();

	for ( int i = 0; i < cols->Size (); ++i )
		if ( cols->ValidIndex (i) )
			if ( cols->GetData (i) )
				delete cols->GetData (i);

	delete cols;

}

Option *Options::GetOption (const string &name )
{

	BTree <Option *> *btree = options.LookupTree ( name );

	if ( btree )
		return btree->data;

	else
		return nullptr;

}

int Options::GetOptionValue (const string &name )
{

	Option *option = GetOption ( name );

	if ( !option ) {
		string msg = "Option " + name + " not found";
		UplinkAbort(msg.c_str())
	}

	return option->value;

}

bool Options::IsOptionEqualTo (const string &name, int value )
{

	Option *option = GetOption ( name );

	if ( option && option->value == value )
		return true;

	else
		return false;

}

void Options::SetOptionValue (const string &name, int newvalue )
{

	BTree <Option *> *btree = options.LookupTree ( name );

	if ( btree ) {

		UplinkAssert ( btree->data )
		Option *option = btree->data;
		option->SetValue ( newvalue );

	}
	else {

		cout << "Tried to set unrecognised option: " << name << endl;

	}

}

void Options::SetOptionValue (const string &name, int newvalue, const string &tooltip, bool yesorno, bool visible )
{

	BTree <Option *> *btree = options.LookupTree ( name );

	if ( btree ) {

		UplinkAssert ( btree->data )
		Option *option = btree->data;
		option->SetValue ( newvalue );
		option->SetTooltip ( tooltip );
		option->SetYesOrNo ( yesorno );
		option->SetVisible ( visible );

	}
	else {

		// Create a new entry
		auto *option = new Option ();
		option->SetName ( name );
		option->SetValue ( newvalue );
		option->SetTooltip ( tooltip );
		option->SetYesOrNo ( yesorno );
		option->SetVisible ( visible );

		options.PutData ( name, option );

	}


}

LList <Option *> *Options::GetAllOptions ( char *searchstring, bool returnhidden )
{

	DArray <Option *> *alloptions = options.ConvertToDArray ();
	auto *result = new LList <Option *> ();

	for ( int i = 0; i < alloptions->Size (); ++i ) {
		if ( alloptions->ValidIndex (i) ) {

			Option *option = alloptions->GetData (i);
			UplinkAssert (option)

			if ( option->visible || returnhidden )
				if ( !searchstring || strstr ( option->name, searchstring ) != nullptr )
					result->PutData ( option );

		}
	}

	delete alloptions;
	return result;

}

void Options::CreateDefaultOptions ()
{

	// Game

	if ( !GetOption ( "game_debugstart" ) )				SetOptionValue ( "game_debugstart", 1, "z", true, false );

#ifndef TESTGAME
	if ( !GetOption ( "game_firsttime" ) ) {
		DArray <char *> *existing = App::ListExistingGames ();
		int lenexisting = existing->Size ();

		if ( lenexisting > 0 ) {
			SetOptionValue ( "game_firsttime", 0, "z", true, false );
		}
		else {
			SetOptionValue ( "game_firsttime", 1, "z", true, false );
		}

		for ( int i = 0; i < lenexisting; i++ )
			if ( existing->ValidIndex ( i ) )
				delete [] existing->GetData (i);

		delete existing;
	}
#else
	if ( !GetOption ( "game_firsttime" ) )				SetOptionValue ( "game_firsttime", 0, "z", true, false );
#endif

    float currentVersion;
    sscanf ( VERSION_NUMBER, "%f", &currentVersion );
    currentVersion *= 100;

    if ( !GetOption ( "game_version" ) )                SetOptionValue ( "game_version", (int) currentVersion, "z", false, false );

	// Graphics

    // TODO: Detect screen resolution on first launch, and match resolution
	if ( !GetOption ( "graphics_screenwidth" ) )		SetOptionValue ( "graphics_screenwidth", 1920, "Sets the width of the screen", false, false );
	if ( !GetOption ( "graphics_screenheight" ) )		SetOptionValue ( "graphics_screenheight", 1080, "Sets the height of the screen", false, false );
    if ( !GetOption ( "graphics_screendepth" ) )        SetOptionValue ( "graphics_screendepth", -1, "Sets the colour depth. -1 Means use desktop colour depth.", false, false );
    if ( !GetOption ( "graphics_screenrefresh" ) )      SetOptionValue ( "graphics_screenrefresh", -1, "Sets the refresh rate. -1 Means use desktop refresh.", false, false );
	if ( !GetOption ( "graphics_fullscreen" ) )			SetOptionValue ( "graphics_fullscreen", 1, "Sets the game to run fullscreen or in a window", true, true );
	if ( !GetOption ( "graphics_buttonanimations" ) )	SetOptionValue ( "graphics_buttonanimations", 1, "Enables or disables button animations", true, true );
	if ( !GetOption ( "graphics_safemode" ) )			SetOptionValue ( "graphics_safemode", 0, "Enables graphical safemode for troubleshooting", true, true );
	if ( !GetOption ( "graphics_softwaremouse" ) )		SetOptionValue ( "graphics_softwaremouse", 0, "Render a software mouse.  Use to correct mouse problems.", true, true );
	if ( !GetOption ( "graphics_fasterbuttonanimations" ) )	SetOptionValue ( "graphics_fasterbuttonanimations", 0, "Increase the speed of button animations.", true, true );
	if ( !GetOption ( "graphics_defaultworldmap" ) )	SetOptionValue ( "graphics_defaultworldmap", 1, "Create agents with the default world map.", true, true );

	Option *optionSoftwareRendering = GetOption ( "graphics_softwarerendering" );
	if ( !optionSoftwareRendering ) {
#ifdef WIN32
		SetOptionValue ( "graphics_softwarerendering", opengl_isSoftwareRendering (), "Enable software rendering.", true, true );
#else
		SetOptionValue ( "graphics_softwarerendering", 0, "Enable software rendering.", true, false );
#endif
	}
	else {
#ifdef WIN32
		optionSoftwareRendering->SetVisible ( true );
		optionSoftwareRendering->SetValue ( opengl_isSoftwareRendering () );
#else
		optionSoftwareRendering->SetVisible ( false );
#endif
	}

	// Sound

	if ( !GetOption ( "sound_musicenabled" ) )			SetOptionValue ( "sound_musicenabled", 1, "Enables or disables music", true, true );

	// Network

    // Theme

    // SetThemeName ( "graphics" );


    // Repair old options files

    app->GetOptions ()->GetOption ( "graphics_screenwidth" )->SetVisible ( false );
    app->GetOptions ()->GetOption ( "graphics_screenheight" )->SetVisible ( false );
    app->GetOptions ()->GetOption ( "graphics_screendepth" )->SetVisible ( false );
    app->GetOptions ()->GetOption ( "graphics_screenrefresh" )->SetVisible ( false );

}

void Options::SetThemeName ( char *newThemeName )
{
    themeName = newThemeName;

    //
    // Parse the theme.txt file

    string themeTextFilename = ThemeFilename ( "theme.txt" );
    string rsFilename = RsArchiveFileOpen ( themeTextFilename );

    if ( !rsFilename.empty() ) {
        idos2unixstream thefile ( rsFilename.c_str() );

        // Header

        char unused [64];
	    thefile >> unused >> ws;
	    getline(thefile, themeTitle);

        thefile >> unused >> ws;
        getline(thefile, themeAuthor);

        thefile >> unused >> ws;
        getline(thefile, themeDescription);

        // Colours

        while ( !thefile.eof() ) {

            char lineBuffer [256];
            thefile.getline ( lineBuffer, 256 );

            if ( strlen(lineBuffer) < 2 )     continue;
		    if ( lineBuffer[0] == ';' )       continue;

            char colourName[64];
            float r, g, b;
    		std::istrstream thisLine ( lineBuffer );
            thisLine >> colourName >> ws >> r >> g >> b >> ws;

            BTree <ColourOption *> *exists = colours.LookupTree( colourName );
            if ( !exists ) {
                colours.PutData( colourName, new ColourOption ( r, g, b ) );
            }
            else {
                delete exists->data;
                exists->data = new ColourOption ( r, g, b );
            }

        }

        thefile.close();
    }


    RsArchiveFileClose ( (char *) themeTextFilename.c_str(), nullptr );

}

string Options::GetThemeName ()
{
    return themeName;
}

string Options::GetThemeTitle ()
{
    return themeTitle;
}

string Options::GetThemeDescription ()
{
    return themeDescription;
}

ColourOption *Options::GetColour (const string &colourName )
{

    ColourOption *result = colours.GetData (colourName);

    if ( result ) {
        return result;
    }
    else {

        cout << "Options::GetColour WARNING : Couldn't find colour " << colourName << endl;
        return &getColourDefault;

    }

}

string Options::ThemeFilename (const string &filename )
{

    string result;

    if ( themeName == "graphics" ) {

        result = "graphics/" + filename;

    }
    else {

        string fullFilename = app->path + themeName + "/" + filename;
		if ( DoesFileExist ( fullFilename.c_str() ) ) {
            result = themeName + "/" + filename;

		} else {
            result = "graphics/" + filename;
		}

    }

    return result;

}

void Options::RequestShutdownChange ( char *optionName, int newValue )
{

    auto *oc = new OptionChange ();
    UplinkStrncpy ( oc->name, optionName, sizeof ( oc->name ) )
    oc->value = newValue;

    shutdownChanges.PutData( oc );

}

void Options::ApplyShutdownChanges ()
{

    while ( shutdownChanges.GetData(0) ) {

        OptionChange *oc = shutdownChanges.GetData(0);
        shutdownChanges.RemoveData(0);

        SetOptionValue ( oc->name, oc->value );

        delete oc;

    }

}

bool Options::Load ( FILE *file )
{

	// Read from our own options file

	string filename = app->userpath + "options";
	cout << "Loading uplink options from " << filename << "...";

	FILE *optionsfile = nullptr;
	bool encrypted = false;
	if ( RsFileEncryptedNoVerify ( (char *) filename.c_str() ) ) {
		if ( RsFileEncrypted ( (char *) filename.c_str() ) ) {
			optionsfile = RsFileOpen ( (char *) filename.c_str() );
			encrypted = true;
		}
		else {
			cout << "failed" << endl;
			return false;
		}
	}
	else {
		optionsfile = fopen ( filename.c_str(), "rb" );
	}

	if ( optionsfile ) {

	    char version [32];
		if ( !FileReadData ( version, sizeof (SAVEFILE_VERSION), 1, optionsfile ) ) {
			version [ 0 ] = '\0';
		}
		else {
			version [ sizeof(version) - 1 ] = '\0';
		}
        if ( version[0] == '\0' || strcmp ( version, SAVEFILE_VERSION_MIN ) < 0 || strcmp ( version, SAVEFILE_VERSION ) > 0 ) {
            cout << endl << "ERROR : Could not load options due to incompatible version format" << endl;
			if ( encrypted )
				RsFileClose ( (char *) filename.c_str(), optionsfile );
			else
				fclose ( optionsfile );
            return false;
        }

		cout << "success" << endl;

		LoadID ( optionsfile );
		if ( !LoadBTree ( (BTree <UplinkObject *> *) &options, optionsfile ) ) {
			DeleteBTreeData ( (BTree <UplinkObject *> *) &options );
			return false;
		}
		LoadID_END ( optionsfile );

		// 't' in the optionsfile at this point signals
		// theme name comes next

		char newThemeName[themeName.length()];
		size_t newThemeLen;

		if ( fgetc(optionsfile) == 't' )
			if ( fread(&newThemeLen, sizeof(newThemeLen), 1, optionsfile) == 1 )
				if ( 0 <= newThemeLen && newThemeLen + 1 < themeName.length() )
					if ( fread(newThemeName, newThemeLen, 1, optionsfile) == 1 ) {
						  newThemeName[newThemeLen] = '\0';
						  themeName = newThemeName;
					}

		if ( encrypted )
			RsFileClose ( (char *) filename.c_str(), optionsfile );
		else
			fclose ( optionsfile );

	}
	else {

//        float currentVersion;
//        sscanf ( VERSION_NUMBER, "%f", &currentVersion );
//        currentVersion *= 100;
//        SetOptionValue ( "game_version", currentVersion, "z", false, false );

		cout << "failed" << endl;
		return false;

	}

	return true;

}

void Options::Save ( FILE *file )
{

	// Write to our own options file

	MakeDirectory ( app->userpath );

	string filename = app->userpath + "options";

	cout << "Saving uplink options to " << filename << "...";

	FILE *optionsfile = fopen ( filename.c_str(), "wb" );

	if ( optionsfile ) {

		cout << "success" << endl;

		fwrite ( SAVEFILE_VERSION, sizeof (SAVEFILE_VERSION), 1, optionsfile );

		SaveID ( optionsfile );
		SaveBTree ( (BTree <UplinkObject *> *) &options, optionsfile );
		SaveID_END ( optionsfile );

		fputc('t', optionsfile);
		size_t themeLen = themeName.length();
		fwrite(&themeLen, sizeof(themeLen), 1, optionsfile);
		fwrite(themeName.c_str(), themeLen, 1, optionsfile);

		fclose ( optionsfile );

		RsEncryptFile ( (char *) filename.c_str() );

	}
	else {

		cout << "failed" << endl;

	}

}

void Options::Print ()
{

    PrintPadded("O P T I O N S");

	PrintBTree ( (BTree <UplinkObject *> *) &options );

    PrintPadded("E N D  O P T I O N S");

}

void Options::Update ()
{
}

string Options::GetID ()
{

	return "OPTIONS";

}

Options *options;


// ============================================================================
// Option class


Option::Option()
{

	UplinkStrncpy ( name, "", sizeof ( name ) )
	UplinkStrncpy ( tooltip, "", sizeof ( tooltip ) )
	yesorno = false;
	visible = true;
	value = 0;

}

Option::~Option()
= default;

void Option::SetName (const string &newname )
{

	assert( newname.length() < SIZE_OPTION_NAME );
	UplinkStrncpy ( name, newname.c_str(), sizeof ( name ) )

}

void Option::SetTooltip (const string &newtooltip )
{

	assert( newtooltip.length() < SIZE_OPTION_TOOLTIP );
	UplinkStrncpy ( tooltip, newtooltip.c_str(), sizeof ( tooltip ) )

}

void Option::SetYesOrNo	( bool newyesorno )
{

	yesorno = newyesorno;

}

void Option::SetVisible ( bool newvisible )
{

	visible = newvisible;

}

void Option::SetValue ( int newvalue )
{

	value = newvalue;

}

bool Option::Load ( FILE *file )
{

	LoadID ( file );

	if ( !FileReadData ( name, sizeof(name), 1, file ) ) {
		name [ 0 ] = '\0';
		return false;
	}
	name [ sizeof(name) - 1 ] = '\0';

	if ( !FileReadData ( tooltip, sizeof(tooltip), 1, file ) ) {
		tooltip [ 0 ] = '\0';
		return false;
	}
	tooltip [ sizeof(tooltip) - 1 ] = '\0';

	if ( !FileReadData ( &yesorno, sizeof(yesorno), 1, file ) ) return false;
	if ( !FileReadData ( &visible, sizeof(visible), 1, file ) ) return false;
	if ( !FileReadData ( &value, sizeof(value), 1, file ) ) return false;

	LoadID_END ( file );

	return true;

}

void Option::Save ( FILE *file )
{

	SaveID ( file );

	fwrite ( name, sizeof(name), 1, file );
	fwrite ( tooltip, sizeof(tooltip), 1, file );
	fwrite ( &yesorno, sizeof(yesorno), 1, file );
	fwrite ( &visible, sizeof(visible), 1, file );
	fwrite ( &value, sizeof(value), 1, file );

	SaveID_END ( file );

}

void Option::Print ()
{

    cout << "Option :" << endl;
    PrintValue("Name", name);
    PrintValue("Value", value);
    PrintValue("YesOrNo", yesorno);
    PrintValue("Visible", visible);

}

void Option::Update ()
{
}

string Option::GetID ()
{

	return "OPTION";

}

int Option::GetOBJECTID ()
{

	return OID_OPTION;

}
