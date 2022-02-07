// -*- tab-width:4 c-file-style:"cc-mode" -*-
// App source file
// By Christopher Delay

//#include "stdafx.h"

#ifdef WIN32
#include <direct.h>
#include <io.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#endif

#include "gucci.h"
#include "eclipse.h"
#include "redshirt.h"
#include "soundgarden.h"
#include "vanbakel.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/miscutils.h"

#include "options/options.h"

#include "game/game.h"
#include "game/data/data.h"
#include "game/gameobituary.h"

#include "mainmenu/mainmenu.h"
#include "network/network.h"

#include "world/world.h"
#include "world/player.h"
#include "world/generator/worldgenerator.h"

#include "interface/interface.h"
#include "interface/localinterface/localinterface.h"
#include "interface/remoteinterface/remoteinterface.h"
#include "interface/localinterface/phonedialler.h"

#include "mmgr.h"

App :: App ()
{

    path = "c:/";
    userpath = path;
    UplinkStrncpy ( version, "1.31c", sizeof ( version ) )
    UplinkStrncpy ( type, "RELEASE", sizeof ( type ) )
    UplinkStrncpy ( date, "01/01/97", sizeof ( date ) )
    UplinkStrncpy ( title, "NewApp", sizeof ( title ) )
    UplinkStrncpy ( release, "Version 1.0 (RELEASE), Compiled on 01/01/97", sizeof ( release ) )

    starttime = 0;

	options = nullptr;
	network = nullptr;
	mainmenu = nullptr;
	phoneDial = nullptr;

	nextLoadGame = "";

    closed = false;

#ifdef CODECARD_ENABLED
	askCodeCard = true;
#else
	askCodeCard = false;
#endif

#ifdef _DEBUG
//    SlInitialise ();
#endif

}

App :: ~App ()
{
	
	// All destructor information should go in App::Close ()

	if ( !Closed() )
		Close();

}

void App :: Initialise ()
{

	options = new Options ();	
	options->Load ( nullptr );
	options->CreateDefaultOptions ();

    starttime = (int)EclGetAccurateTime ();

	network = new Network ();
	mainmenu = new MainMenu ();

}

void App :: Set ( char *newpath, char *newversion, char *newtype, char *newdate, char *newtitle )
{
                
	assert( strlen ( newpath ) < SIZE_APP_PATH );
	assert( strlen ( newversion ) < SIZE_APP_VERSION );
	assert( strlen ( newtype ) < SIZE_APP_TYPE );
	assert( strlen ( newdate ) < SIZE_APP_DATE );
	assert( strlen ( newtitle ) < SIZE_APP_TITLE );

    path = newpath;
    UplinkStrncpy ( version, newversion, sizeof ( version ) )
    UplinkStrncpy ( type, newtype, sizeof ( type ) )
    UplinkStrncpy ( date, newdate, sizeof ( date ) )
    UplinkStrncpy ( title, newtitle, sizeof ( title ) )
    UplinkSnprintf ( release, sizeof ( release ), "Version %s (%s)\nCompiled on %s\n", version, type, date )

#ifdef WIN32
	// Under Windows, the user-path is %app-path%/users
	UplinkSnprintf( userpath, sizeof ( userpath ), "%susers/", path );
	UplinkSnprintf( usertmppath, sizeof ( usertmppath ), "%suserstmp/", path );
	UplinkSnprintf( userretirepath, sizeof ( userretirepath ), "%susersold/", path );
#else
	// Under Linux, the user-path is ~/.uplink 
	// (or %app-path%/users if no HOME environment variable)
	string homedir = getenv("HOME");
	if (!homedir.empty()) {
		userpath = homedir + "/.uplink/";
		usertmppath = homedir + "/.uplink/userstmp/";
		userretirepath = homedir + "/.uplink/usersold/";
	}
	else {
		userpath = path + "users/";
		usertmppath = path + "userstmp/";
		userretirepath = path + "usersold/";
	}
#endif // WIN32
}

Options *App::GetOptions ()
{

	UplinkAssert (options)
	return options;

}

Network *App::GetNetwork ()
{

	UplinkAssert (network)
	return network;

}

MainMenu *App::GetMainMenu ()
{

	UplinkAssert (mainmenu)
	return mainmenu;

}

void App::RegisterPhoneDialler ( PhoneDialler *phoneDiallerScreen )
{

	UplinkAssert ( phoneDiallerScreen )
	assert( phoneDial != phoneDiallerScreen );

	if ( phoneDial )
		UnRegisterPhoneDialler ( phoneDial );

	phoneDial = phoneDiallerScreen;

}

void App::UnRegisterPhoneDialler ( PhoneDialler *phoneDiallerScreen )
{

	if ( phoneDial == phoneDiallerScreen ) {

		phoneDial->Remove ();
		delete phoneDial;
		phoneDial = nullptr;

	}

}

void CopyGame (const string &username, const string &filename )
{

	string filenametmp = app->usertmppath + "curagent.usr";

	//char filenametmpUplink [256];
	//UplinkSnprintf ( filenametmpUplink, sizeof ( filenametmpUplink ), "%scuragent_clear.bin", app->usertmppath );

	EmptyDirectory ( app->usertmppath );
	CopyFilePlain ( filename, filenametmp );
	//CopyFileUplink ( filename, filenametmpUplink );

}

void App::SetNextLoadGame (const string &username )
{

    // TODO: This assertion fails if the user doesn't provide a username on the login screen
	assert( !username.empty() );

	nextLoadGame = username;

}

void App::LoadGame ( )
{

	assert ( !nextLoadGame.empty() );

	LoadGame ( nextLoadGame );

	nextLoadGame = "";

}

void App::LoadGame (const string &username )
{

	UplinkAssert ( game )

	// Try to load from the local dir

	string filename = app->userpath + username + ".usr";

	if ( !RsFileEncrypted ( (char *) filename.c_str() ) ) {
		string filenametmp = app->userpath + username + ".tmp";
		if ( RsFileEncrypted ( (char *) filenametmp.c_str() ) ) {
			filename = filenametmp;
		}
	}

	CopyGame ( username, filename );

	cout << "Loading profile from "  << filename  << "...";
	
	FILE *file = RsFileOpen ( (char *) filename.c_str() );

	if ( file ) {

		GetMainMenu ()->Remove ();

		bool success = game->LoadGame ( file );
		RsFileClose ( (char *) filename.c_str(), file );

        if ( !success ) {
			EmptyDirectory ( app->usertmppath );
            cout << "App::LoadGame, Failed to load user profile" << endl;

			delete game;
			game = new Game();

			return;
        }

		cout << "success" << endl;

	}
	else {

		EmptyDirectory ( app->usertmppath );
		cout << "failed" << endl;
		cout << "App::LoadGame, Failed to load user profile" << endl;

		EclReset ( GetOptions ()->GetOptionValue ("graphics_screenwidth"),
		           GetOptions ()->GetOptionValue ("graphics_screenheight") );
		GetMainMenu ()->RunScreen ( MAINMENU_LOGIN );

		return;

	}
	
	if (game->GameSpeed() != GAMESPEED_GAMEOVER) {

        WorldGenerator::LoadDynamics();

		// Not needed anymore since the Gateway store the GatewayDef
		//WorldGenerator::VerifyPlayerGateway();

		game->GetWorld ()->GetPlayer ()->GetConnection ()->Disconnect ();
		game->GetWorld ()->GetPlayer ()->GetConnection ()->Reset ();

		if ( game->GetWorld ()->GetPlayer ()->gateway.newgatewaydef ) {

			// The player has upgraded his gateway and now wishes to 
			// log into the new system

			game->GetWorld ()->GetPlayer ()->gateway.ExchangeGatewayComplete ();
			
			// "Welcome to your new gateway"

			game->GetInterface ()->GetRemoteInterface ()->RunNewLocation ();
			game->GetInterface ()->GetRemoteInterface ()->RunScreen ( 10 );
			

		}
		else if ( game->GetWorld ()->GetPlayer ()->gateway.nuked ) {

			// The player has nuked his gateway, so set him up
			// with a new one           
            game->GetWorld ()->GetPlayer ()->gateway.nuked = false;

			game->GetWorld ()->GetPlayer ()->GetConnection ()->AddVLocation ( IP_UPLINKPUBLICACCESSSERVER );
			game->GetWorld ()->GetPlayer ()->GetConnection ()->Connect ();

            game->GetInterface ()->GetLocalInterface ()->Remove ();
            EclReset ( app->GetOptions ()->GetOptionValue ("graphics_screenwidth"),
				       app->GetOptions ()->GetOptionValue ("graphics_screenheight") );
			game->GetInterface ()->GetRemoteInterface ()->RunNewLocation ();
			game->GetInterface ()->GetRemoteInterface ()->RunScreen ( 10 );

		}
		else {
			
			// "Welcome back to your gateway"

			game->GetInterface ()->GetRemoteInterface ()->RunNewLocation ();
			game->GetInterface ()->GetRemoteInterface ()->RunScreen ( 3 );

		}

	}
	else {											// This is a Game Over game

		game->SetGameSpeed ( GAMESPEED_PAUSED );
        EclReset ( app->GetOptions ()->GetOptionValue ("graphics_screenwidth"),
				   app->GetOptions ()->GetOptionValue ("graphics_screenheight") );
		mainmenu->RunScreen ( MAINMENU_OBITUARY );

	}

}

void App::SaveGame (const string &username ) const
{

	if ( username == "NEWAGENT" ) return;

	UplinkAssert ( game )

	// Try to save to the local dir

	MakeDirectory ( userpath );

	string filename = userpath + username + ".tmp";
	string filenamereal = userpath + username + ".usr";

	cout << "Saving profile to " << filename << "...";

	FILE *file = fopen ( filename.c_str(), "wb" );

	if ( file ) {

	
		UplinkAssert ( file )

		game->Save ( file );
		fclose ( file );

#ifndef TESTGAME
		RsEncryptFile ( (char *) filename.c_str() );
#endif

		cout << "success. Moving profile to " << filenamereal << "...";

		if ( !CopyFilePlain ( filename, filenamereal ) ) {
		    cout << "failed" << endl;
		    cout << "App::SaveGame, Failed to copy user profile from " << filename << " to " << filenamereal << endl;
		}
		else {
		    cout << "success" << endl;
			CopyGame ( username, filenamereal );
		}

	}
	else {

		cout << "failed" << endl;
		cout << "App::SaveGame, Failed to save user profile" << endl;
	
	}

}

void App::RetireGame (const string &username ) const
{

	string filenamereal = userpath + username + ".usr";
	string filenametmp = userpath + username + ".tmp";

	string filenameretirereal = userretirepath + username + ".usr";
	string filenameretiretmp = userretirepath + username + ".tmp";

	cout << "Retire profile " << username << " ...";

	CopyFilePlain ( filenametmp, filenameretiretmp );
	if ( !CopyFilePlain ( filenamereal, filenameretirereal ) ) {
	    cout << "failed" << endl;
	    cout << "App::RetireGame, Failed to copy user profile from " << filenamereal << " to " << filenameretirereal << endl;
	}
	else {
	    cout << "success" << endl;
		RemoveFile ( filenametmp );
		RemoveFile ( filenamereal );
	}

}

DArray <char *> *App::ListExistingGames ()
{

	auto *existing = new DArray <char *> ();

#ifdef WIN32

	char searchstring [_MAX_PATH + 1];
	UplinkSnprintf ( searchstring, sizeof ( searchstring ), "%s*.usr", app->userpath );

	_finddata_t thisfile;
	intptr_t fileindex = _findfirst ( searchstring, &thisfile );

	int exitmeplease = 0;

	while ( fileindex != -1 && !exitmeplease ) {

		size_t newnamesize = _MAX_PATH + 1;
		char *newname = new char [newnamesize];
		UplinkStrncpy ( newname, thisfile.name, newnamesize );
		char *p = strchr ( newname, '.' );
		if ( p ) *p = '\x0';

		existing->PutData ( newname );
		exitmeplease = _findnext ( fileindex, &thisfile );

	}

	if ( fileindex != -1 )
		_findclose ( fileindex );

#else
	string userdir = app->userpath;
	DIR *dir = opendir( userdir.c_str() );
	if (dir != nullptr) {
	    struct dirent *entry = readdir ( dir );

	    while (entry != nullptr) {
	    
		char *p = strstr(entry->d_name, ".usr");
		if ( p ) {
		    *p = '\x0';
	      
		    size_t newnamesize = 256;
			char *newname = new char [newnamesize];
		    UplinkStrncpy ( newname, entry->d_name, newnamesize )
		    existing->PutData ( newname );
		}
	    
		entry = readdir ( dir );
	    
	    }
	  
	    closedir( dir );
	}
#endif

	return existing;

}

void App::Close ()
{

	UplinkAssert ( !closed )

    closed = true;

    EclReset ( app->GetOptions ()->GetOptionValue ("graphics_screenwidth"),
			   app->GetOptions ()->GetOptionValue ("graphics_screenheight") );

	if ( game ) game->ExitGame ();

    options->ApplyShutdownChanges ();
    options->Save ( nullptr );
    
    SvbReset ();
    GciDeleteAllTrueTypeFonts ();
    RsCleanUp ();
	SgShutdown ();

	delete mainmenu;
	mainmenu = nullptr;
	delete options;
	options = nullptr;
	delete network;
	network = nullptr;
	if ( game ) {
		delete game;
		game = nullptr;
	}
	if ( phoneDial ) {
		delete phoneDial;
		phoneDial = nullptr;
	}

	if ( !nextLoadGame.empty() ) {
		nextLoadGame = "";
	}

#ifdef _DEBUG
    char filename [256];
    UplinkSnprintf ( filename, sizeof ( filename ), "%smemtemp", app->userpath );
//    SlPrintMemoryLeaks ( filename );
#endif

	//exit(0);

}

bool App::Closed () const
{

    return closed;

}

bool App::Load ( FILE *file )
{
	return true;
}

void App::Save ( FILE *file )
{
}

void App::CoreDump ()
{

#ifdef WIN32
    MessageBox ( nullptr, "A Fatal Error occurred in Uplink.\n\n"
                       "Please report this on the Uplink forums at\n"
					   "http://www.introversion.co.uk/\n\n"
                       "Uplink will now shut down.", 
                       "Uplink Error", MB_ICONEXCLAMATION | MB_OK );    

#endif

    PrintPadded("B E G I N  C O R E  D U M P");
    PrintStackTrace ();
    PrintPadded("E N D  C O R E  D U M P");

}

void App::Print ()
{

    PrintPadded("A P P");

	if ( game )     game->Print ();     else cout << "game == nullptr" << endl;
    if ( mainmenu ) mainmenu->Print (); else cout <<"mainmenu == nullptr" << endl;
	if ( options )  options->Print ();  else cout << "options == nullptr" << endl;
	if ( network )  network->Print ();  else cout << "network == nullptr" << endl;

    PrintPadded("E N D  O F  A P P");

}

void App::Update ()
{

	UplinkAssert ( game )

	if ( game->GameSpeed () == GAMESPEED_GAMEOVER || 
		( game->IsRunning () &&
		  game->GetWorld ()->GetPlayer ()->gateway.nuked ) ) {

		if ( phoneDial )
			UnRegisterPhoneDialler ( phoneDial );

		// The game has ended and now needs shutting down
		
		app->SaveGame ( game->GetWorld ()->GetPlayer ()->handle );				
		game->SetGameSpeed ( GAMESPEED_PAUSED );

        EclReset ( app->GetOptions ()->GetOptionValue ("graphics_screenwidth"),
				   app->GetOptions ()->GetOptionValue ("graphics_screenheight") );

		if ( game->GetWorld ()->GetPlayer ()->gateway.nuked )
			mainmenu->RunScreen ( MAINMENU_CONNECTIONLOST );

        else if ( game->GetObituary ()->demogameover ) 
            mainmenu->RunScreen ( MAINMENU_DEMOGAMEOVER );

        else if ( game->GetObituary ()->warezgameover )
            mainmenu->RunScreen ( MAINMENU_WAREZGAMEOVER );

        else if ( game->GetWorld ()->plotgenerator.revelation_releaseuncontrolled )
            mainmenu->RunScreen ( MAINMENU_REVELATIONWON );

        else if ( game->GetWorld ()->plotgenerator.revelation_releasefailed )
            mainmenu->RunScreen ( MAINMENU_REVELATIONLOST );

		else
			mainmenu->RunScreen ( MAINMENU_DISAVOWED );

	}

	if ( !Closed () && game->IsRunning () ) {	
		game->Update ();
	}

	if ( !Closed () && mainmenu->IsVisible () ) {
		mainmenu->Update ();
	}

	if ( !Closed () && ( game->IsRunning () || mainmenu->InScreen () == MAINMENU_FIRSTLOAD ) )
		if ( phoneDial )
			if ( phoneDial->UpdateSpecial () )
				UnRegisterPhoneDialler ( phoneDial );

	if ( !Closed () && network->IsActive () ) {
		network->Update ();
	}

}

string App::GetID ()
{

	return "APP";

}


App *app = nullptr;



