
//#include "stdafx.h"

#include <app/miscutils.h>
#include "eclipse.h"
#include "soundgarden.h"
#include "redshirt.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/opengl.h"
#include "app/serialise.h"
#include "app/opengl_interface.h"

#include "options/options.h"

#include "game/game.h"
#include "game/gameobituary.h"
#include "game/data/data.h"

#include "mainmenu/mainmenu.h"

#include "view/view.h"

#include "interface/interface.h"

#include "world/world.h"
#include "world/player.h"
#include "world/message.h"
#include "world/generator/worldgenerator.h"
#include "world/generator/numbergenerator.h"
#include "world/scheduler/notificationevent.h"

#include "mmgr.h"


Game::Game ()
{

	ui      = nullptr;
	view    = nullptr;
	world   = nullptr;

	gamespeed = GAMESPEED_PAUSED;
	gob = nullptr;

	loadedSavefileVer = "";

	createdSavefileVer = "";

	winningCodeWon = 0;
	winningCodeDesc = nullptr;
	winningCodeExtra = nullptr;
	winningCodeRandom = 0;

	whichWorldMap = defconworldmap;

}

Game::~Game () 
{

    delete ui;
    delete view;
    delete world;

    delete gob;

	createdSavefileVer = "";

    delete [] winningCodeDesc;
    delete [] winningCodeExtra;

}

void Game::NewGame ()
{
 
    delete ui;
    delete view;
    delete world;
    delete gob;

    createdSavefileVer = SAVEFILE_VERSION;

	// Create a unique winning code
	winningCodeRandom = 0;
	for ( int i = 0; i < sizeof ( winningCodeRandom ); i++ )
		winningCodeRandom |= ( NumberGenerator::RandomNumber ( 254 ) + 1 ) << ( i * 8 );

	gob = nullptr;

	whichWorldMap = defconworldmap;
	if ( app->GetOptions ()->GetOption ("graphics_defaultworldmap") &&
		 app->GetOptions ()->GetOption ("graphics_defaultworldmap")->value != 0 ) {

		 whichWorldMap = defaultworldmap;

	}

	// Initialise the world
	world = new World ();
	WorldGenerator::LoadDynamicsGatewayDefs ();

	NotificationEvent::ScheduleStartingEvents ();
	WorldGenerator::GenerateAll ();
	world->plotgenerator.Initialise ();
    world->demoplotgenerator.Initialise ();

    SgPlaySound ( RsArchiveFileOpen ( "sounds/ringout.wav" ), "sounds/ringout.wav", false );

	// Initialise the view		
	view = new View ();
	view->Initialise ();

	// Initialise the interface
	ui = new Interface ();
	GetInterface ()->Create ();	
	
	// Start the game running
	gamespeed = GAMESPEED_NORMAL;

	// Run until (at least) the game start date
	// Generate a few extra "easy" missions for the player to get started with

	Date gamestart;
	gamestart.SetDate ( GAME_START_DATE );
    
	do {

		GetWorld ()->Update ();
		GetWorld ()->date.AdvanceDay (1);
		GetWorld ()->date.AdvanceHour ( NumberGenerator::RandomNumber ( 12 ) - 6 );
		GetWorld ()->date.AdvanceMinute ( NumberGenerator::RandomNumber ( 60 ) );

		if ( NumberGenerator::RandomNumber (5) == 0 ) 	WorldGenerator::GenerateSimpleStartingMissionA ();	
		if ( NumberGenerator::RandomNumber (5) == 0 ) 	WorldGenerator::GenerateSimpleStartingMissionB ();

	} while ( GetWorld ()->date.Before ( &gamestart ) );

}

void Game::ExitGame ()
{

	opengl_close ();

}

void Game::SetGameSpeed ( int newspeed )
{

	gamespeed = newspeed;

}

int Game::GameSpeed () const
{

	return gamespeed;

}

bool Game::IsRunning () const
{

	return ( gamespeed > GAMESPEED_PAUSED );

}

void Game::GameOver (const string& reason )
{

	gamespeed = GAMESPEED_GAMEOVER;

	if ( !gob ) gob = new GameObituary ();
	gob->SetGameOverReason ( reason );
	gob->Evaluate ();
	
}

void Game::DemoGameOver ()
{

  	gamespeed = GAMESPEED_GAMEOVER;

	if ( !gob ) gob = new GameObituary ();
	gob->SetGameOverReason (  "You reached the maximum permissable Agent rating for a Demo user.\n"
                              "You can visit www.introversion.co.uk to buy the full product." );
	gob->SetDemoGameOver ( true );
    gob->Evaluate ();

}

void Game::WarezGameOver ()
{

    gamespeed = GAMESPEED_GAMEOVER;

    if ( !gob ) gob = new GameObituary ();
    gob->SetGameOverReason (  "You are playing an illegal copy of Uplink.\n"
                              "You can visit www.introversion.co.uk to buy the full product." );
    gob->SetWarezGameOver ( true );
    gob->Evaluate ();

}

Interface *Game::GetInterface ()
{

	UplinkAssert ( ui )
	return ui;

}

View *Game::GetView ()
{

	UplinkAssert ( view )
	return view;

}

World *Game::GetWorld ()
{

	UplinkAssert ( world )
	return world;

}

GameObituary *Game::GetObituary ()
{

	UplinkAssert (gob)
	return gob;

}

bool Game::LoadGame ( FILE *file )
{

	// Initialise the random number generator

	srand( (unsigned) time( nullptr ) );

	LoadID ( file );

	// Check the file version is correct

	if ( !FileReadDataInt(loadedSavefileVer, 6, file) ) {
		loadedSavefileVer = "";
	}
    if ( loadedSavefileVer.empty() || loadedSavefileVer.compare(SAVEFILE_VERSION_MIN) < 0 || loadedSavefileVer.compare(SAVEFILE_VERSION) > 0 ) {

        //UplinkAbort ( "This save game file is from an older version of Uplink" );        

		EclReset ( app->GetOptions ()->GetOptionValue ("graphics_screenwidth"),
		           app->GetOptions ()->GetOptionValue ("graphics_screenheight") );
        app->GetMainMenu ()->RunScreen ( MAINMENU_LOGIN );

        string message = "Failed to load user profile\nThe save file is not compatible\n\nSave file is Version [" + loadedSavefileVer + "]\nRequired Version is between [" + SAVEFILE_VERSION_MIN + "] and [" + SAVEFILE_VERSION + "]";
        create_msgbox ( "Error", message );

        return false;
    
    }

	if ( !Load ( file ) ) {

		EclReset ( app->GetOptions ()->GetOptionValue ("graphics_screenwidth"),
		           app->GetOptions ()->GetOptionValue ("graphics_screenheight") );
        app->GetMainMenu ()->RunScreen ( MAINMENU_LOGIN );

        create_msgbox ( "Error", "Failed to load user profile\n"
		                         "The save file is either\n"
		                         "not compatible or\n"
		                         "corrupted" );

        return false;

	}

	LoadID_END ( file );

    return true;

}


bool Game::Load ( FILE *file )
{

	// Reset currently running game

    delete ui;
    delete view;
    delete world;
    delete gob;

	ui	    = new Interface ();
	view    = new View ();
	world   = new World ();
	gob     = nullptr;
	WorldGenerator::LoadDynamicsGatewayDefs ();

	LoadID ( file );

	if ( strcmp( game->GetLoadedSavefileVer(), "SAV62" ) >= 0 ) {

		if ( !FileReadData ( &whichWorldMap, sizeof(whichWorldMap), 1, file ) ) return false;

	}
	else {

		whichWorldMap = defaultworldmap;

	}

	// Is this a "GameOver" game?

	if ( !FileReadData ( &gamespeed, sizeof(gamespeed), 1, file ) ) return false;

	if ( gamespeed != GAMESPEED_GAMEOVER ) {

		// Load each of the modules

		if ( !GetWorld     ()->Load ( file ) ) return false;
		if ( !GetInterface ()->Load ( file ) ) return false;
		if ( !GetView      ()->Load ( file ) ) return false;
		
	}
	else {

		// This is a "game over" game
		gob = new GameObituary ();
		if ( !gob->Load ( file ) ) return false;
		
	}

	if ( strcmp( game->GetLoadedSavefileVer(), "SAV58" ) >= 0 ) {

		if ( !LoadDynamicStringInt( createdSavefileVer, file ) ) return false;

		if ( !FileReadData ( &winningCodeWon, sizeof(winningCodeWon), 1, file ) ) return false;
		if ( !LoadDynamicStringPtr ( &winningCodeDesc, file ) ) return false;
		if ( !LoadDynamicStringPtr ( &winningCodeExtra, file ) ) return false;
		if ( !FileReadData ( &winningCodeRandom, sizeof(winningCodeRandom), 1, file ) ) return false;

	}

	LoadID_END ( file );

	return true;

}

void Game::Save ( FILE *file )
{

	if ( gamespeed != GAMESPEED_PAUSED ) {

		SaveID ( file );

		UplinkAssert ( ui )
		UplinkAssert ( view )
		UplinkAssert ( world )

		fwrite ( SAVEFILE_VERSION, sizeof (SAVEFILE_VERSION), 1, file );

		fwrite ( &whichWorldMap, sizeof(whichWorldMap), 1, file );

		fwrite ( &gamespeed, sizeof(gamespeed), 1, file );

		if (gamespeed != GAMESPEED_GAMEOVER) {

			GetWorld     ()->Save ( file );
			GetInterface ()->Save ( file );		
			GetView      ()->Save ( file );

		}
		else {
	
			UplinkAssert ( gob )
			gob->Save ( file );

		}

		SaveDynamicString ( createdSavefileVer, file );

		fwrite ( &winningCodeWon, sizeof(winningCodeWon), 1, file );
		SaveDynamicString ( winningCodeDesc, file );
		SaveDynamicString ( winningCodeExtra, file );
		fwrite ( &winningCodeRandom, sizeof(winningCodeRandom), 1, file );

		SaveID_END ( file );

	}

}

void Game::Print ()
{

    PrintPadded("G A M E");

    PrintValue("Game speed", gamespeed);

	if ( ui )      ui     ->Print ();		else cout << "UI is nullptr" << endl;
	if ( view )    view   ->Print ();		else cout << "View is nullptr" << endl;
	if ( world )   world  ->Print ();		else cout << "World is nullptr" << endl;

    PrintPadded("E N D  O F  G A M E");

}

string Game::GetID ()
{

	return "GAME";

}

void Game::Update ()
{

	//
	// Update the modules
	//

	if ( gamespeed > GAMESPEED_PAUSED ) {

		GetWorld     ()->Update ();
		GetView      ()->Update ();
		GetInterface ()->Update ();

	}

	//
	// Autosave every minute
	//

	if ( time(nullptr) > lastsave + 1 * 60 ) {

		app->SaveGame ( GetWorld ()->GetPlayer ()->handle );
		lastsave = time(nullptr);

	}

}

const char *Game::GetLoadedSavefileVer () const
{

	if ( !loadedSavefileVer.empty() )
		return loadedSavefileVer.c_str();
	else
		return SAVEFILE_VERSION;

}

void Game::WinCode ( const char *desc, const char *codeExtra )
{

	UplinkAssert ( desc )

	if ( !createdSavefileVer.empty() && createdSavefileVer.compare( "SAV58" ) >= 0 )
		winningCodeWon = 1;

	delete [] winningCodeDesc;

	winningCodeDesc = new char [ strlen ( desc ) + 1 ];
	UplinkSafeStrcpy ( winningCodeDesc, desc )

	if ( winningCodeExtra ) {
		delete [] winningCodeExtra;
		winningCodeExtra = nullptr;
	}

	if ( codeExtra ) {
		winningCodeExtra = new char [ strlen ( codeExtra ) + 1 ];
		UplinkSafeStrcpy ( winningCodeExtra, codeExtra )
	}

}

bool Game::IsCodeWon () const
{

	return ( winningCodeWon != 0 );

}

char *Game::GetWinningCodeDesc ()
{

	if ( winningCodeDesc ) {

		char *retDesc = new char [ strlen ( winningCodeDesc ) + 1 ];
		UplinkSafeStrcpy ( retDesc, winningCodeDesc )
		return retDesc;

	}
	else {

		char *retDesc = new char [ 1 ];
		UplinkSafeStrcpy ( retDesc, "" )
		return retDesc;

	}

}

char *Game::GetWinningCode ()
{

	if ( !winningCodeWon ) {
		char *retCode = new char [ 1 ];
		UplinkSafeStrcpy ( retCode, "" )
		return retCode;
	}

	size_t lenExtra = 0;
	if ( winningCodeExtra )
		lenExtra = strlen ( winningCodeExtra );

	size_t lenRandom = sizeof ( winningCodeRandom );

	size_t lenfinal = lenExtra + lenRandom;
	auto *final = new unsigned char [ lenfinal + 4 + 1 ];

	size_t lenMask = 6;
	unsigned char mask[] = "UPLINK";

	size_t i;
	for ( i = 0; i < lenExtra; i++ )
		final [ i ] = winningCodeExtra [ i ];

	for ( i = 0; i < lenRandom; i++ )
		final [ i + lenExtra ] = mask [ i % lenMask ] ^ ( unsigned char ) ( ( winningCodeRandom >> ( i * 8 ) ) & 0xff );

	for ( i = 0; i < 4; i++ ) {
		final [ i + lenfinal ] = 0;
		size_t index = i;
		size_t incr = 1;
		while ( index < lenfinal + i ) {
			final [ i + lenfinal ] ^= final [ index ] >> i;
			index += incr;
			incr <<= 1;
			if ( incr > 4 )
				incr = 4;
		}
	}

	size_t lenFinalhex = lenExtra + 2 * ( lenRandom + 4 );
	char *finalhex = new char [ lenFinalhex + 1 ];

	for ( i = 0; i < lenExtra; i++ )
		finalhex [ i ] = final [ i ];

	size_t indexHex = lenExtra;
	for ( i = 0; i < lenRandom + 4; i++ ) {
		finalhex [ indexHex++ ] = 'A' + ( final [ i + lenExtra ]  >> 4 );
		finalhex [ indexHex++ ] = 'A' + ( final [ i + lenExtra ] & 0xf );
	}

	finalhex [ lenFinalhex ] = '\0';

	delete [] final;

	return finalhex;

}

enum Game::WorldMapType Game::GetWorldMapType ()
{

	return whichWorldMap;

}


Game *game = nullptr;

