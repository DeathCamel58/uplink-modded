// -*- tab-width:4 c-file-style:"cc-mode" -*-
/*

  =============================================================================
  =																		      =
  =							 U	 P	 L	 I	 N	 K							  =
  =																			  =
  =							 Started 22nd June '99							  =
  =																			  =
  =							  By Christopher Delay							  =
  =																			  =
  =																			  =
  =============================================================================

  */

#define CRASH_REPORTS

#if defined(WIN32) && defined(CRASH_REPORTS)
#define _WIN32_WINDOWS 0x0500	// for IsDebuggerPresent
#endif

//#include "stdafx.h"

#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#ifdef WIN32
#include <conio.h>
#include <io.h>
#  ifdef CRASH_REPORTS
#include <dbghelp.h>
#include "ExceptionHandler.h"
#  endif

#else
#include <unistd.h>
#include <csignal>
#endif

#include <GL/gl.h>

#include <GL/glu.h>


#include "eclipse.h"
#include "vanbakel.h"
#include "soundgarden.h"
#include "gucci.h"
#include "redshirt.h"
#include "bungle.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/opengl.h"
#include "app/opengl_interface.h"
#include "app/miscutils.h"
#include "app/serialise.h"

#ifndef WIN32
#include "app/binreloc.h"
#endif

#include "options/options.h"

#include "mainmenu/mainmenu.h"

#include "game/scriptlibrary.h"
#include "game/game.h"

#include "mmgr.h"

// ============================================================================
// Initialisation functions

void RunUplink ( int argc, char **argv );
int RunUplinkExceptionHandling ();

void Init_App      ( char *apppath );
void Init_Options  ( int argc, char **argv );

bool Load_Data	   ();
void Init_Game     ();
void Init_Sound    ();
void Init_Music    ();
void Init_Graphics ();
void Init_OpenGL   ( int argc, char **argv );
void Init_Fonts	   ();

void Run_MainMenu  ();
void Run_Game      ();

void Cleanup_Uplink();

#if defined(FULLGAME) || defined(TESTGAME)
#if defined(WIN32)
bool Init_Steam    ();
bool Cleanup_Steam ();
#endif

bool VerifyLegitAndCodeCardCheck();
#endif

// ============================================================================
// Static variables

#if defined(FULLGAME) || defined(TESTGAME)
#if defined(WIN32)
static bool isSteamInit = false;
#endif
#endif

// ============================================================================
// Program entry points


#ifndef WIN32

static FILE *file_stdout = nullptr;

void hSignalSIGSEGV ( int n )
{

	cout << endl << "An Uplink Internal Error has occurred: segmentation violation (SIGSEGV)" << endl;
	if ( file_stdout ) {
		fprintf ( file_stdout, "\nAn Uplink Internal Error has occurred: segmentation violation (SIGSEGV)\n" );
		fflush ( file_stdout );
	}

	RunUplinkExceptionHandling ();

}

void hSignalSIGFPE ( int n )
{

	cout << endl << "An Uplink Internal Error has occurred: erroneous arithmetic operation (SIGFPE)" << endl;
	if ( file_stdout ) {
		fprintf ( file_stdout, "\nAn Uplink Internal Error has occured: erroneous arithmetic operation (SIGFPE)\n" );
		fflush ( file_stdout );
	}

	RunUplinkExceptionHandling ();

}

void hSignalSIGPIPE ( int n )
{

	cout << endl << "An Uplink Internal Error has occurred: write to pipe with no one reading (SIGPIPE)" << endl;
	if ( file_stdout ) {
		fprintf ( file_stdout, "\nAn Uplink Internal Error has occurred: write to pipe with no one reading (SIGPIPE)\n" );
		fflush ( file_stdout );
	}

	RunUplinkExceptionHandling ();

}

#endif


int main ( int argc, char **argv )
{

	// Standard C++ entry point

#if !defined(WIN32) && !defined(_DEBUG)
	signal ( SIGSEGV, hSignalSIGSEGV );  // segmentation violation
	signal ( SIGFPE, hSignalSIGFPE );    // floating point exception: "erroneous arithmetic operation"
	signal ( SIGPIPE, hSignalSIGPIPE );  // write to pipe with no one reading
#endif
	
	RunUplink ( argc, argv );
	return 0;
}


//#ifdef _WINDOWS
#ifdef WIN32

/*
    Remember to set subsystem:windows
    instead of subsystem:console
    in the link options
    */

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{


 	
    // Work out the exe app path
    // THANK YOU VERY MUCH WIN32
    // WHATEVER WAS WRONG WITH ARGV[0] YOU FUCKING IDIOTS

	size_t tmppathsize = 256;
	char *tmppath = new char [tmppathsize];
	char *apppath = tmppath;
    UplinkStrncpy ( apppath, GetCommandLine (), tmppathsize );
	// Strip inverted commas from the app path
	// note that it's not guaranteed the path is surrounded
	// by inverted commas (eg. if path does not have a space in it,
	// or you are running under Wine)
	if (apppath[0] == '\"') {
		apppath = (apppath+1);
		char *lastdit = strchr ( apppath, '\"' ); 
		UplinkAssert (lastdit);
		*lastdit = '\x0';
	}

    // Break up lpCmdLine into argc and argv

    char **argv = nullptr;
    int argc = 1;

    if ( *lpCmdLine != '\x0' ) {

	    char *cmdLineCopy = new char [strlen(lpCmdLine)+1];
	    UplinkSafeStrcpy ( cmdLineCopy, lpCmdLine );

	    char *c = cmdLineCopy;
	    while ( c ) {
		    ++argc;
		    c = strchr ( (c+1), ' ' );
	    }

        argv = new char *[argc];
        argv[0] = apppath;
        
	    if ( argc > 1 ) {
		    
		    argv [1] = cmdLineCopy;
		    char *c = strchr ( cmdLineCopy, ' ');
		    int n = 2;
		    while ( c ) {
			    *c = '\x0';
			    argv [n] = (c+1);
			    ++n;
			    c = strchr ( (c+1), ' ' );
		    }
	    }

    }
    else {

        argv = new char *[1];
        argv[0] = apppath;

    }

    // Run the game

/*
	cout << "App path : " << apppath << endl;
	cout << "Command line : " << lpCmdLine << endl;
	cout << "Num arguments : " << argc << endl;
	for ( int i = 0; i < argc; ++i )
		cout << "Arg " << i << " : '" << argv[i] << "'" << endl;
*/

#ifdef CRASH_REPORTS
	__try {
#endif

		RunUplink ( argc, argv );

#ifdef CRASH_REPORTS
	}
	__except( RecordExceptionInfo( GetExceptionInformation(), "WinMain" ) ) {}
#endif

	delete [] tmppath;

	delete [] argv;

	return 0;

}

#endif

// ============================================================================

int RunUplinkExceptionHandling ()
{

#if defined(WIN32) && defined(CRASH_REPORTS)
	if ( IsDebuggerPresent() ) {
		GciRestoreScreenSize ();

		UplinkIntFlush;

		return EXCEPTION_CONTINUE_SEARCH;
	}
#endif

	if ( app && app->options && app->GetOptions ()->GetOption ( "crash_graphicsinit" ) ) {
		int crashInit = app->GetOptions ()->GetOptionValue ( "crash_graphicsinit" );
		if ( crashInit != 0 ) {
			cout << endl << "An Uplink Internal Error has occurred during graphics initialization" << endl;
#ifdef WIN32
			if ( !opengl_isSoftwareRendering () && opengl_setSoftwareRendering ( true ) ) {
				MessageBox ( nullptr, "A Fatal Error occured in Uplink.\n\n"
				                   "Software Rendering is now turn on.\n"
				                   "Please restart Uplink.", 
				                   "Uplink Error", MB_ICONEXCLAMATION | MB_OK );
				exit(255);
			}
			else {
				MessageBox ( nullptr, "A Fatal Error occured in Uplink\n"
				                   "during graphics initialization.\n",
				                   "Uplink Error", MB_ICONEXCLAMATION | MB_OK );
			}
#else
			if ( file_stdout ) {
				fprintf ( file_stdout, "\nAn Uplink Internal Error has occurred during graphics initialization\n" );
				fflush ( file_stdout );
			}
#endif
		}
	}

	// Catch any errors that occured
	// Set the screen size back to normal,
	// then continue with the error - permitting debugging

	cout << endl;
	cout << "An (unrecognised) Uplink Internal Error has occured" << endl;
	cout << "===================================================" << endl;

	App::CoreDump ();

#ifndef WIN32
	if ( file_stdout ) {
		fprintf ( file_stdout, "\nAn (unrecognised) Uplink Internal Error has occured\n" );
		fprintf ( file_stdout, "===================================================\n" );
		if ( app && strncmp ( app->userpath.c_str(), "c:/", sizeof(app->userpath) ) != 0 ) {
			fprintf ( file_stdout, "See the %sdebug.log file for more informations on the error\n", app->userpath.c_str() );
		}
		else {
			fprintf ( file_stdout, "See the debug.log file for more informations on the error\n" );
		}
		fflush ( file_stdout );
	}
#endif

    GciRestoreScreenSize ();

	UplinkIntFlush;

#ifdef WIN32
#  ifdef CRASH_REPORTS
	return EXCEPTION_CONTINUE_SEARCH;
#  else
    throw;
#  endif
#else
#  ifdef TESTGAME
    throw;
#  else
    exit(255);
#  endif
#endif

	return 255;

}

void RunUplink ( int argc, char **argv ) 
{

	// Crash!!!
	//char * nullPtr = nullptr;
	//(*nullPtr) = 'a';
	//UplinkAssert(nullptr);

#if defined(WIN32) && defined(CRASH_REPORTS)
	__try {
#else
	try {
#endif

		if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'v') {
			cout << VERSION_NUMBER << endl;
			return;
		}

			
		// Initialise each of the modules

#ifdef WIN32
		Init_App      ( argv[0] );
#else
		Init_App      ( br_find_exe("uplink") );
#endif
		Init_Options  ( argc, argv );

#if defined(FULLGAME) || defined(TESTGAME)
#if defined(WIN32)
		if ( !Init_Steam () ) {
			Cleanup_Uplink ();
			return;
		}
#endif

//      Make the game start no matter if there's a valid license
//        if ( !VerifyLegitAndCodeCardCheck() ) {
//			Cleanup_Uplink ();
//			return;
//		}
#endif

		if( !Load_Data() ) {
			Cleanup_Uplink ();
			return;
		}

		Init_Game     ();
		Init_Graphics ();
		Init_OpenGL   ( argc, argv );
		Init_Fonts	  ();
		Init_Sound    ();
		Init_Music    ();

		// Run everything

		Run_MainMenu  ();
		Run_Game      ();

		// Clean up

		Cleanup_Uplink();

		UplinkIntFlush;

	}
#if defined(WIN32) && defined(CRASH_REPORTS)
	__except( RunUplinkExceptionHandling () ) {}
#else
	catch ( ... ) {
		RunUplinkExceptionHandling ();
	}
#endif
}

// ============================================================================


void Init_App ( char *argv0 )
{

	// Set up App object

    char *apppath = GetFilePath ( argv0 );
	app = new App ();
	char currenttime [SIZE_APP_DATE];
	UplinkSnprintf ( currenttime, sizeof ( currenttime ), "%s at %s", __DATE__, __TIME__ )
	app->Set ( apppath, VERSION_NUMBER, VERSION_NAME, currenttime, "Uplink" );
    delete [] apppath;

#ifdef CODECARD_ENABLED
    #ifdef CODECARD_CHRIS
        UplinkStrncat ( app->release, sizeof ( app->release ), "(code-C)" );
    #endif
    #ifdef CODECARD_MARK
        UplinkStrncat ( app->release, sizeof ( app->release ), "(code-M)" );
    #endif
    #ifdef CODECARD_TOM
        UplinkStrncat ( app->release, sizeof ( app->release ), "(code-T)" );
    #endif
#endif

	cout << "=============================" << endl;
	cout << "=                           =" << endl;
	cout << "=        U P L I N K        =" << endl;
	cout << "=                           =" << endl;
	cout << "=        Version " << app->version << " =" << endl;

#ifdef FULLGAME
    cout << "=     - R E L E A S E -     =" << endl;
#endif

#ifdef DEMOGAME	
	cout << "=        - D E M O -        =" << endl;
#endif
    
#ifdef TESTGAME
    cout << "=    - I N T E R N A L -    =" << endl;
#endif

	cout << "=                           =" << endl;
	cout << "=============================" << endl;
	cout << endl << app->release << endl;

#ifdef DEBUGLOG_ENABLED

	MakeDirectory ( app->userpath );
	MakeDirectory ( app->usertmppath );
	EmptyDirectory ( app->usertmppath );
	MakeDirectory ( app->userretirepath );

    char debugpath [256];
    UplinkSnprintf ( debugpath, sizeof ( debugpath ), "%sdebug.log", app->userpath );

#ifndef WIN32
	file_stdout = nullptr;
	int fd_stdout = dup ( fileno ( stdout ) );
	if ( fd_stdout != -1 ) {
		file_stdout = fdopen ( fd_stdout, "a" );
	}
#endif

    FILE *stdoutfile = freopen ( debugpath, "a", stdout );
    if ( !stdoutfile ) cout << "WARNING : Failed to open " << debugpath << " for writing stdout" << endl;

    FILE *stderrfile = freopen ( debugpath, "a", stderr );
    if ( !stderrfile ) cout << "WARNING : Failed to open " << debugpath << " for writing stderr" << endl;

    // Print some basic info to the file

    time_t timet = time(nullptr);
    tm *thetime = localtime(&timet);

    cout << endl << endl;
    cout << "===============================================" << endl;
    cout << "NEW GAME     " << thetime->tm_hour << ":" << thetime->tm_min << ", " <<  thetime->tm_mday << "/" << thetime->tm_mon+1 << "/" << thetime->tm_year+1900 << endl;
    cout << "===============================================" << endl;
        
    cout << "Version : " << app->version << endl;

#ifdef FULLGAME
    cout << "RELEASE\" << endl;
#endif
#ifdef DEMOGAME	
	cout << "DEMO" << endl;
#endif
#ifdef TESTGAME
    cout << "INTERNAL" << endl;
#endif
#ifdef WIN32
    cout << "Win32 Build" << endl;
#else
    cout << "Linux Build" << endl;
#endif
    cout << app->release << endl;

#endif

    RsInitialise ( (char *) app->path.c_str() );

	app->Initialise ();

}


#if defined(FULLGAME) || defined(TESTGAME)
#if defined(WIN32)

typedef int (*InitSteamDLL)( int seed, char *errorMsg, int errorMaxSize ); 

bool Init_Steam ()
{

	isSteamInit = false;

	bool debugging = false;
	if ( app->GetOptions ()->IsOptionEqualTo ( "game_debugstart", 1 ) )
		debugging = true;

	if ( debugging ) cout << "Initialising Steam..." << endl;

    HINSTANCE hinstLib; 
    InitSteamDLL ProcInitSteam; 
    BOOL fFreeResult; 
 
    // Get a handle to the DLL module.
 
    hinstLib = LoadLibrary(TEXT("UplinkSteamAuth")); 
 
    // If the handle is valid, try to get the function address.
 
    if (hinstLib != nullptr)
    { 
		ProcInitSteam = (InitSteamDLL) GetProcAddress(hinstLib, TEXT("InitSteam")); 
 
        // If the function address is valid, call the function.
 
        if (nullptr != ProcInitSteam)
        {
			bool errorSteam;
			char errorMsg[512];

			// Initialise the random number generator
			srand( (unsigned int) time( nullptr ) );

			int rand1 = rand();
			int rand2 = rand();
			int rand3 = rand();
			int rand4 = rand();
			int seed = rand1 ^ ( rand2 << 8 ) ^ ( rand2 << 16 ) ^ ( rand2 << 24 );

			int retSeed = (ProcInitSteam) ( seed, errorMsg, sizeof(errorMsg) );
			if ( ( ( retSeed ^ seed ) & 0xFF ) == 'G' && 
				 ( ( ( retSeed ^ seed ) >> 8 ) & 0xFF ) == 'O' && 
				 ( ( ( retSeed ^ seed ) >> 16 ) & 0xFF ) == 'O' && 
				 ( ( ( retSeed ^ seed ) >> 24 ) & 0xFF ) == 'D' ) {

				isSteamInit = true;

				app->askCodeCard = false;

				if ( debugging ) cout << "done." << endl;

				errorSteam = false;
			}
			else {
				if ( errorMsg[0] == '\0' ) {
					UplinkIntSnprintf ( errorMsg, sizeof(errorMsg), "Could not initialize Steam: Unknown Error." );
				}
				errorSteam = true;
			}

#ifdef STEAM_AUTH
			if ( errorSteam ) {
				fFreeResult = FreeLibrary(hinstLib);

				cout << endl << "An error occured in Uplink" << endl;
				cout << errorMsg << endl;

				char message[1024];
				UplinkIntSnprintf ( message, sizeof(message), "An error occured in Uplink.\n\n"
				                                              "%s\n", errorMsg);

				MessageBox ( nullptr, message,
				                   "Uplink Error", MB_ICONEXCLAMATION | MB_OK );
			}
			return !errorSteam;
#else
			if ( errorSteam ) {
				fFreeResult = FreeLibrary(hinstLib);

				cout << errorMsg << endl;
			}
			return true;
#endif
        }
 
        // Free the DLL module.
 
        fFreeResult = FreeLibrary(hinstLib); 
    }

#ifdef STEAM_AUTH
	cout << endl << "An error occured in Uplink" << endl;
	cout << "Could not initialize Steam: Function not found." << endl;

	MessageBox ( nullptr, "An error occured in Uplink.\n\n"
	                   "Could not initialize Steam: Function not found.\n", 
	                   "Uplink Error", MB_ICONEXCLAMATION | MB_OK );
	return false;
#else
	if ( debugging ) cout << "Could not initialize Steam: Function not found." << endl;
	return true;
#endif

}

typedef int (*CleanupSteamDLL)( char *errorMsg, int errorMaxSize ); 

bool Cleanup_Steam ()
{

	if ( !isSteamInit ) {
		return true;
	}
	isSteamInit = false;

    HINSTANCE hinstLib; 
    CleanupSteamDLL ProcCleanupSteam; 
    BOOL fFreeResult; 
 
    // Get a handle to the DLL module.
 
    hinstLib = LoadLibrary(TEXT("UplinkSteamAuth")); 
 
    // If the handle is valid, try to get the function address.
 
    if (hinstLib != nullptr)
    { 
        ProcCleanupSteam = (CleanupSteamDLL) GetProcAddress(hinstLib, TEXT("CleanupSteam")); 
 
        // If the function address is valid, call the function.
 
        if (nullptr != ProcCleanupSteam)
        {
			bool errorSteam;
			char errorMsg[512];

			if ( !(ProcCleanupSteam) ( errorMsg, sizeof(errorMsg) ) ) {
				if ( errorMsg[0] != '\0' ) {
					cout << errorMsg << endl;
				}
				else {
					cout << "Could not clean up Steam: Unknown Error." << endl;
				}

		        fFreeResult = FreeLibrary(hinstLib); 
				errorSteam = true;
			}
			else {
				errorSteam = false;
			}

	        fFreeResult = FreeLibrary(hinstLib); 
			return !errorSteam;
        }
 
        // Free the DLL module.
 
        fFreeResult = FreeLibrary(hinstLib); 
    }

	cout << "Could not clean up Steam: Function not found." << endl;
	return false;

}

#endif // defined(WIN32)


static void findGoodFile( char *filename )
{

	if ( !DoesFileExist( filename ) ) {
		int len = (int) strlen ( filename );

		int i;
		for ( i = len - 1; i >= 0; i-- ) {
			if ( filename[i] == '\\' || filename[i] == '/' ) {
				break;
			}
		}

		if ( i >= 4 ) {
			char c1 = filename[ i - 4 ];
			char c2 = filename[ i - 3 ];
			char c3 = filename[ i - 2 ];
			char c4 = filename[ i - 1 ];
			char c5 = filename[ i ];

			if ( ( c1 == '\\' || c1 == '/' ) &&
			     ( c2 == 'l' || c2 == 'L' ) &&
			     ( c3 == 'i' || c3 == 'I' ) &&
			     ( c4 == 'b' || c4 == 'B' ) &&
				 ( c5 == '\\' || c5 == '/' ) ) {

				 for ( i++; i <= len; i++ ) {
					filename[ i - 4 ] = filename[ i ];
				 }
			}
		}
	}

}

bool VerifyLegitAndCodeCardCheck()
{

    //
    // SECURITY CHECK
    // To detect if a dodgy warez kid has put one of our
    // patches over his hacked version to fix it
    // If so, his world.dat will have the realbuffer shown below
    //
    //

	// Generate the file CRC

	char realbuffer [9];
	UplinkStrncpy ( realbuffer, "\xe7\x6b\x7e\x6b\x4c\x4f\x57\x7d", sizeof ( realbuffer ) )
    
	// Ensure the world.dat file is in place and has the right CRC

	char worlddatfilename [256];
	UplinkSnprintf ( worlddatfilename, sizeof ( worlddatfilename ), "%sworld.dat", app->path.c_str() )
	findGoodFile ( worlddatfilename );

	FILE *file = fopen ( worlddatfilename, "rb" );

	bool match = false;
	if ( file ) {

		fseek ( file, 128, SEEK_SET );
		char readbuffer [9];
		fread ( readbuffer, 8, 1, file );
		readbuffer [8] = '\x0';
		match = ( strcmp ( readbuffer, realbuffer ) == 0 );
		fclose ( file );

	}

	// Ensure the filesize is correct

	bool match2 = false;

	if ( match ) {
#ifdef WIN32
		int filehandle = _open ( worlddatfilename, _O_RDONLY );
		if ( filehandle != -1 ) {

			struct _stat filestats;
			_fstat ( filehandle, &filestats );
			match2 = ( filestats.st_size == 14400792 );
			_close ( filehandle );

		}
#else
		int filehandle = open ( worlddatfilename, O_RDONLY );
		if ( filehandle != -1 ) {

			struct stat filestats;
			fstat ( filehandle, &filestats );
			match2 = ( filestats.st_size == 14400792 );
			close ( filehandle );

		}
#endif
	}

	bool match3 = false;

	if ( match && match2 ) {

		UplinkStrncpy ( realbuffer, "\xaa\xab\x15\xdd\xdd\xee\xe9\x2d", sizeof ( realbuffer ) )

		file = fopen ( worlddatfilename, "rb" );

		if ( file ) {

			fseek ( file, 256, SEEK_SET );
			char readbuffer [9];
			fread ( readbuffer, 8, 1, file );
			readbuffer [8] = '\x0';
			match3 = ( strcmp ( readbuffer, realbuffer ) != 0 );
			fclose ( file );

		}

	}

    if ( match && match2 && match3 ) {

		bool match4 = false;

		UplinkStrncpy ( realbuffer, "\xc1\xec\xd6\x8b\x02\x07\x28\xe8", sizeof ( realbuffer ) )

		file = fopen ( worlddatfilename, "rb" );

		if ( file ) {

			fseek ( file, 512 + 64, SEEK_SET );
			char readbuffer [9];
			fread ( readbuffer, 8, 1, file );
			readbuffer [8] = '\x0';
			match4 = ( strcmp ( readbuffer, realbuffer ) == 0 );
			fclose ( file );

		}

		if ( match4 ) {

			app->askCodeCard = false;

		}

		return true;

    }

#ifdef VERIFY_UPLINK_LEGIT

    cout << endl << "An error occured in Uplink" << endl;
    cout << "Files integrity is not verified" << endl;

#ifdef WIN32
	MessageBox ( nullptr, "An error occured in Uplink.\n\n"
	                   "Files integrity is not verified.\n", 
	                   "Uplink Error", MB_ICONEXCLAMATION | MB_OK );
#else
	if ( file_stdout ) {
        cout << endl << "An Uplink Error has occured" << endl;
        cout << "Files integrity is not verified" << endl;
	}
#endif

	return false;

#else

	return true;

#endif

}

#endif // defined(FULLGAME) || defined(TESTGAME)


void Init_Options ( int argc, char **argv )
{

	// Parse all command line options

	for ( int i = 1; i < argc; ++i ) {
		
		char type;
		//char optionname [SIZE_OPTION_NAME];
		char *optionname;

		//sscanf ( argv [i], "%c%s", &type, optionname );
		type = argv [i][0];
		if ( argv [i][0] )
			optionname = &(argv [i][1]);
		else
			optionname = argv [i];

		if ( type == '+' ) {													// Turn option ON  (1)
			app->GetOptions ()->SetOptionValue ( optionname, true );
#ifdef _DEBUG
            cout << "Parsed command line option : " << argv [i] << endl;
#endif
		}
		else if ( type == '-' ) {												// Turn option OFF (0)
			app->GetOptions ()->SetOptionValue ( optionname, false );
#ifdef _DEBUG
            cout << "Parsed command line option : " << argv [i] << endl;
#endif
		}
		else if ( type == '!' ) {												// Set option value
			++i;
			if ( i < argc ) {
				int newvalue;
				sscanf ( argv [i], "%d", &newvalue );
				app->GetOptions ()->SetOptionValue ( optionname, newvalue );
#ifdef _DEBUG
                cout << "Parsed command line option : " << optionname << newvalue << endl;
#endif
			}
			else {
                cout << "Error parsing command line option : " << argv [i-1] << endl;
			}
		}
		else {
			//char msg [256];
			//UplinkSnprintf ( msg, sizeof ( msg ), "Error parsing command line option : %s", argv [i] );
			//UplinkAbort ( msg );
            cout << "Error parsing command line option : " << argv [i] << endl;
		}

	}


    //
    // Safe mode?
    //

    if ( app->GetOptions ()->GetOptionValue ( "graphics_safemode" ) == 1 ) {
        app->GetOptions ()->SetOptionValue ( "graphics_fullscreen", false );
        app->GetOptions ()->SetOptionValue ( "graphics_screenrefresh", -1 );
        app->GetOptions ()->SetOptionValue ( "graphics_screendepth", -1 );
        app->GetOptions ()->SetOptionValue ( "graphics_softwaremouse", true );
    }

	cout << endl;
	
    SetWindowScaleFactor ( app->GetOptions ()->GetOptionValue("graphics_screenwidth") / 640.0f,
                           app->GetOptions ()->GetOptionValue("graphics_screenheight") / 480.0f );

	if ( app->GetOptions ()->IsOptionEqualTo ( "game_debugstart", 1 ) )
        PrintPadded("DEBUGGING INFORMATION ENABLED");

}

static bool TestRsLoadArchive ( char *filename )
{

	if ( !RsLoadArchive ( filename ) ) {
        cout << endl << "An error occurred in Uplink" << endl;
        cout << "Files integrity is not verified" << endl;
        cout << "Failed loading '" << filename << "'" << endl;

#ifdef WIN32
		char message[512];
		UplinkIntSnprintf( message, sizeof(message), "An error occured in Uplink.\n\n"
		                                             "Files integrity is not verified.\n"
		                                             "Failed loading '%s'\n", filename );

		MessageBox ( nullptr, message,
						   "Uplink Error", MB_ICONEXCLAMATION | MB_OK );
#else
		if ( file_stdout ) {
            cout << endl << "An Uplink Error has occured" << endl;
            cout << "Files integrity is not verified" << endl;
            cout << "Failed loading '" << filename << "'" << endl;
		}
#endif

		return false;
	}

	return true;

}

bool Load_Data ()
{

	bool debugging = false;
	if ( app->GetOptions ()->IsOptionEqualTo ( "game_debugstart", 1 ) )
		debugging = true;

	if ( debugging ) cout << "Loading application data" << endl;

    // TODO: `.dat` files are not exported into the build directory, leading to the game closing. Export `.dat`s to build dir
	if ( !TestRsLoadArchive ( "data/data.dat" ) ) return false;
	if ( !TestRsLoadArchive ( "data/graphics.dat" ) ) return false;
    if ( !TestRsLoadArchive ( "data/loading.dat" ) ) return false;
	if ( !TestRsLoadArchive ( "data/sounds.dat" ) ) return false;
	if ( !TestRsLoadArchive ( "data/music.dat" ) ) return false;
	if ( !TestRsLoadArchive ( "data/fonts.dat" ) ) return false;
    if ( !TestRsLoadArchive ( "data/patch.dat" ) ) return false;
    if ( !TestRsLoadArchive ( "data/patch2.dat" ) ) return false;
    if ( !TestRsLoadArchive ( "data/patch3.dat" ) ) return false;

#ifdef _DEBUG
	//DArray<char *> *fnames = RsListArchive("","");
	//int fnames_size = fnames->Size();
	//for ( int i = 0; i < fnames_size; ++i ) {
	//	cout << "File: " << (*fnames)[i] << endl;
	//}
	//delete fnames;
#endif

	if ( debugging ) cout << "Finished loading application data" << endl;

	return true;

}

void Init_Game () 
{

	bool debugging = false;
	if ( app->GetOptions ()->IsOptionEqualTo ( "game_debugstart", 1 ) )
		debugging = true;

	if ( debugging ) cout << "Init_Game called...creating game object" << endl;

	// Initialise the random number generator
	srand( (unsigned int) time( nullptr ) );

	// Set up Game object
	game = new Game ();	
	
	if ( debugging ) cout << "Finished with Init_Game" << endl;

}

void Init_Sound ()
{

	bool debugging = false;
	if ( app->GetOptions ()->IsOptionEqualTo ( "game_debugstart", 1 ) )
		debugging = true;

	if ( debugging ) cout << "Init_Sound called...setting up sound system" << endl;

	SgInitialise ();

	if ( debugging ) cout << "Finished with Init_Sound" << endl;

}

void Init_Music ()
{

	bool debugging = false;
	if ( app->GetOptions ()->IsOptionEqualTo ( "game_debugstart", 1 ) )
		debugging = true;

    if ( debugging ) cout << "Init_Music called...loading modules" << endl;

    SgPlaylist_Initialise ();

	SgSetModVolume ( 20 );

    SgPlaylist_Create ( "main" );

#ifdef DEMOGAME
    SgPlaylist_AddSong ("main", "music/bluevalley.uni" );
    SgPlaylist_AddSong ("main", "music/serenity.uni" );
    SgPlaylist_AddSong ("main", "music/mystique.uni" );
#else
    SgPlaylist_AddSong ("main", "music/bluevalley.uni" );
    SgPlaylist_AddSong ("main", "music/serenity.uni" );
    SgPlaylist_AddSong ("main", "music/mystique.uni" );
    SgPlaylist_AddSong ("main", "music/a94final.uni" );
    SgPlaylist_AddSong ("main", "music/symphonic.uni" );
    SgPlaylist_AddSong ("main", "music/myst2.uni" );
#endif

/*
    SgPlaylist_Create ("action");
    SgPlaylist_Create ("ambient");
    SgPlaylist_Create ("sad");

#ifdef DEMOGAME
    SgPlaylist_AddSong ("action", "music/bluevalley.uni" );
    SgPlaylist_AddSong ("ambient", "music/bluevalley.uni" );
    SgPlaylist_AddSong ("ambient", "music/serenity.uni" );
    SgPlaylist_AddSong ("ambient", "music/mystique.uni" );
    SgPlaylist_AddSong ("sad", "music/serenity.uni" );
#else
    SgPlaylist_AddSong ("action", "music/bluevalley.uni" );
    SgPlaylist_AddSong ("ambient", "music/bluevalley.uni" );
    SgPlaylist_AddSong ("ambient", "music/serenity.uni" );
    SgPlaylist_AddSong ("ambient", "music/mystique.uni" );
    SgPlaylist_AddSong ("ambient", "music/a94final.uni" );
    SgPlaylist_AddSong ("sad", "music/symphonic.uni" );
#endif
*/

    if ( debugging ) cout << "Finished with Init_Music" << endl;

}


void Init_Graphics ()
{
	Options *o = app->GetOptions ();
	o->SetThemeName ( o->GetThemeName() );
}

void Init_Fonts	()
{

	bool debugging = false;
	if ( app->GetOptions ()->IsOptionEqualTo ( "game_debugstart", 1 ) )
		debugging = true;

	if ( debugging ) cout << "Init_Fonts called...setting up system fonts" << endl;

	// Set up Gucci graphics library	

	GciEnableTrueTypeSupport ();

	bool success = false;
	char *dungeonFontFilename = RsArchiveFileOpen ( "fonts/dungeon.ttf" );
	if ( !dungeonFontFilename ) {
        cout << "Unable to open font : fonts/dungeon.ttf" << endl;
	}
	else {
		if ( debugging ) cout << "Registering fonts...";

		bool dungeonFontAdded = GciRegisterTrueTypeFont( dungeonFontFilename );
	        
		if ( debugging ) cout << "done" << endl;
		if ( debugging ) cout << "Loading system fonts into memory..." << endl;

		success =
		  (GciLoadTrueTypeFont ( HELVETICA_10, "Dungeon", dungeonFontFilename, 9  ) &
		   GciLoadTrueTypeFont ( HELVETICA_12, "Dungeon", dungeonFontFilename, 11 ) &
		   GciLoadTrueTypeFont ( HELVETICA_18, "Dungeon", dungeonFontFilename, 16 ));

		if ( debugging ) cout << "done" << endl;

		if ( debugging ) cout << "Unregistering fonts...";
		if ( dungeonFontAdded ) GciUnregisterTrueTypeFont( dungeonFontFilename );
		if ( debugging ) cout << "done" << endl;
	}

	if ( !success ) {
		GciDisableTrueTypeSupport ();
        cout << "True type font support is DISABLED" << endl;
	}
	
	GciSetDefaultFont ( HELVETICA_10 );

	if ( debugging ) cout << "Finished with Init_Fonts" << endl;

}

void Init_OpenGL ( int argc, char **argv )
{

#ifdef WIN32
	if ( app->GetOptions ()->GetOption ( "crash_graphicsinit" ) ) {
		int crashInit = app->GetOptions ()->GetOptionValue ( "crash_graphicsinit" );
		if ( crashInit != 0 ) {
			if ( opengl_setSoftwareRendering ( true ) ) {
				if ( app->GetOptions ()->GetOption ( "graphics_softwarerendering" ) ) {
					app->GetOptions ()->SetOptionValue ( "graphics_softwarerendering", opengl_isSoftwareRendering () );
				}
			}
		}
	}
#endif

	app->GetOptions ()->SetOptionValue ( "crash_graphicsinit", 1, "", true, false );
	app->GetOptions ()->Save ( nullptr );

	// Set up OpenGL

	opengl_initialise ( argc, argv );


	app->GetOptions ()->SetOptionValue ( "crash_graphicsinit", 0, "", true, false );
	app->GetOptions ()->Save ( nullptr );

}


void Run_MainMenu ()
{

	if ( app->GetOptions ()->IsOptionEqualTo ( "game_debugstart", 1 ) ) {
        cout << "Creating main menu." << endl;
        PrintPadded("END OF DEBUGGING INFORMATION");
	}
	

	// Set up the game (mainmenu if not first time, or straight into the game );
    
    float currentVersion;
    sscanf ( VERSION_NUMBER, "%f", &currentVersion );
    currentVersion *= 100;

    int oldVersion = app->GetOptions ()->GetOptionValue( "game_version" );

    if ( (int) currentVersion != oldVersion ) {

        // A new patch has been installed
        cout << "New patch Detected!" << endl;
        cout << "Old version = " << oldVersion << endl;
        cout << "New version = " << currentVersion << endl << endl;

        app->GetOptions()->SetOptionValue ( "game_version", (int) currentVersion, "z", false, false );

        if ( oldVersion < 120 )
        {
            // Ugrading the game from a version before Nakatomi
            app->GetMainMenu ()->RunScreen ( MAINMENU_FIRSTLOAD );
            ScriptLibrary::RunScript( 45 );
        }
        else
        {
    		app->GetMainMenu ()->RunScreen ( MAINMENU_LOGIN );
        }

    }
    else if ( app->GetOptions ()->IsOptionEqualTo ( "game_firsttime", 1 ) ) {

        // This is a brand new installation

		app->GetMainMenu ()->RunScreen ( MAINMENU_FIRSTLOAD );
		GciTimerFunc ( 2000, ScriptLibrary::RunScript, 30 );

	}
	else {
	
        // Normal startup

		app->GetMainMenu ()->RunScreen ( MAINMENU_LOGIN );
	
	}

}

void Run_Game ()
{

	// Run the game!

	opengl_run ();

}

void Cleanup_Uplink() {

#if defined(FULLGAME) || defined(TESTGAME)
#if defined(WIN32)
	Cleanup_Steam ();
#endif
#endif

	if ( app ) {
		delete app;
		app = nullptr;
	}

	//if ( game ) {
	//	delete game;
	//	game = nullptr;
	//}

}
