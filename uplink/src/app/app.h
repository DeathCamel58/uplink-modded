// -*- tab-width:4 c-file-style:"cc-mode" -*-

/*
	App object

	Stores data about the app - eg path, version, etc

  */

#ifndef included_app_h
#define included_app_h

// ============================================================================

#include "app/uplinkobject.h"


class Options;
class Network;
class MainMenu;
class PhoneDialler;


#define SIZE_APP_PATH    256
#define SIZE_APP_VERSION 32
#define SIZE_APP_TYPE    32
#define SIZE_APP_DATE    32
#define SIZE_APP_TITLE   64
#define SIZE_APP_RELEASE 256

// ============================================================================


class App : public UplinkObject
{

public :

    string path;
    string userpath;
    string usertmppath; // Used by the crash reporter to store the current .usr
    string userretirepath; // Used to store old agents ( .usr / .tmp )
    string version;
    string type;
    string date;
    string title;
    string release;

    int starttime;
    bool closed;

    Options *options;
	Network *network;
	MainMenu *mainmenu;
	PhoneDialler *phoneDial;

	string nextLoadGame;

	bool askCodeCard;

public:

    App ();
    ~App () override;
    
    void Set (string newpath, string newversion, string newtype,
              string newdate, string newtitle );
    
	void Initialise ();	
	void Close ();			// Shuts down the app	
    bool Closed () const;         // True if in the process of shutting down

	void SetNextLoadGame (const string &username );       // Set the username to load with the next call to LoadGame
	void LoadGame ();                                    // Use the username set with SetNextLoadGame
	void LoadGame (const string &username );
	void SaveGame (const string &username ) const;
	void RetireGame (const string &username ) const;

    /**
     * Gets all save games
     * @return DArray of all save game file names
     */
	static DArray <string> *ListExistingGames ();

	Options *GetOptions ();
	Network *GetNetwork ();
	MainMenu *GetMainMenu ();
	void RegisterPhoneDialler ( PhoneDialler *phoneDiallerScreen );
	void UnRegisterPhoneDialler ( PhoneDialler *phoneDiallerScreen );

    static void CoreDump ();             

	// Common functions

	bool Load   ( FILE *file ) override;
	void Save   ( FILE *file ) override;
	void Print  () override;
	void Update () override;
	string GetID () override;

};

extern App *app;

#endif


