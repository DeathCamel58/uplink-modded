
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

//! App UplinkObject
/**
 * Stores data about the app - path, version, etc
 */
class App : public UplinkObject
{

public :

    /**
     * TODO: Document
     * @note This was observed to be `./` on linux
     */
    string path;

    /**
     * The path to the users folder
     */
    string userpath;

    /**
     * The path to the temporary users folder
     * @note This is used by the crash reporter to store the current .usr
     */
    string usertmppath;

    /**
     * The path used to store old agents
     * @note These files end in .tmp and .usr
     */
    string userretirepath;

    /**
     * The version of uplink
     */
    string version;

    /**
     * The value of UPLINK_VERSION_NAME in cmake at build time
     */
    string type;

    /**
     * The date and time the program was compiled
     */
    string date;

    /**
     * Always `Uplink`
     */
    string title;

    /**
     * The release string of the game
     * @note This is always in the format "Version <Version> (<Release Type>)\nCompiled on <Compile Date>\n`
     */
    string release;

    int starttime;
    bool closed;

    Options *options;
	Network *network;
	MainMenu *mainmenu;
	PhoneDialler *phoneDial;

	/**
	 * The user profile to load next
	 */
	string nextLoadGame;

	/**
	 * Whether the game should ask for the code card to validate the copy of the game
	 */
	bool askCodeCard;

public:

    App ();
    ~App () override;

    /**
     * Sets various settings for the game
     * @param newpath The path of the game TODO: Validate that this is accurate
     * @param newversion The version of the game
     * @param newtype The release type
     * @param newdate The date and time of compilation
     * @param newtitle The title of the game
     */
    void Set (string newpath, string newversion, string newtype,
              string newdate, string newtitle );
    
	void Initialise ();

	/**
	 * Shuts down the app
	 */
	void Close ();

	/**
	 * Checks if the app is in the process of shutting down
	 * @return true if in the process of closing
	 */
    bool Closed () const;

    /**
     * Sets the username to load with the next call to LoadGame
     * @param username The username to load
     */
	void SetNextLoadGame (const string &username );

	/**
	 * Loads the game for the username set with SetNextLoadGame
	 */
	void LoadGame ();

	/**
	 * Loads the game for a specific username
	 * @param username The username to load
	 */
	void LoadGame (const string &username );

	/**
	 * Saves the game for a specific username
	 * @param username The username to save
	 */
	void SaveGame (const string &username ) const;

	/**
	 * Retires the game for a specific username
	 * @param username The username to retire
	 */
	void RetireGame (const string &username ) const;

    /**
     * Gets all save games
     * @return DArray of all save game file names
     */
	static DArray <string> *ListExistingGames ();

	/**
	 * Gets all current options in App
	 * @return Options currently set
	 */
	Options *GetOptions ();

	/**
	 * Gets the current Network for the game
	 * @return Network in App
	 */
	Network *GetNetwork ();

	/**
	 * Gets the current MainMenu for the game
	 * @return MainMenu of app
	 */
	MainMenu *GetMainMenu ();

	/**
	 * Registers a new PhoneDialler for the game
	 * @param phoneDiallerScreen The new PhoneDialler
	 */
	void RegisterPhoneDialler ( PhoneDialler *phoneDiallerScreen );

	/**
	 * Removes the PhoneDialler from the game if given PhoneDialler is the one currently in the game
	 * @param phoneDiallerScreen PhoneDialler to remove if set
	 */
	void UnRegisterPhoneDialler ( PhoneDialler *phoneDiallerScreen );

	/**
	 * Generates a core dump for the game
	 * @note On Windows, this opens a new MessageBox
	 * @note On non-Windows systems, this prints core dump to console
	 */
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


