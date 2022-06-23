
#ifndef included_consolescreeninterface_h
#define included_consolescreeninterface_H

#include "tosser.h"
#include "eclipse.h"

#include "interface/remoteinterface/remoteinterfacescreen.h"

class GenericScreen;
class ConsoleCommand;

//! RemoteInterfaceScreen for computer's "Console" screen
/**
 * Allows extremely low level access to a computer system
 */
class ConsoleScreenInterface : public RemoteInterfaceScreen 
{

protected:

    /**
     * LList of users in the chat
     * @note index 0 is the system, 1 is the player
     */
	LList <string> users;

	/**
	 * LList of commands not yet run
	 */
	LList <ConsoleCommand *> queue;

	/**
	 * true if waiting for commands to "scroll"
	 */
	bool waiting;

	int timesync;									// Used to synchronise things

	/**
	 * The directory the console is currently in
	 */
	string currentdir;

	bool deleted_sys;

protected:

    /**
     * Is the interface currently visible
     * @return true if the title is
     */
	static bool IsVisibleInterface ();

    /**
     * Callback for when message has finished "scrolling"
     */
	static void WaitingCallback ();							// Called when message has finished "scrolling" on

    /**
     * Renders the typing input box
     * @param button Button for input
     * @param highlighted Callback for highlight
     * @param clicked Callback for click
     */
	static void TypeHereDraw ( Button *button, bool highlighted, bool clicked );

    /**
     * Appends inputted message to queue and resets input field
     * @param button Button for input
     */
	static void PostClick ( Button *button );

    /**
     * Renders the Border
     * @param button Button for chat
     * @param highlighted Callback for highlight
     * @param clicked Callback for click
     */
	static void BorderDraw ( Button *button, bool highlighted, bool clicked );

    /**
     * Renders the message
     * @param button Button for message
     * @param highlighted Callback for highlight
     * @param clicked Callback for click
     */
	static void MessageDraw ( Button *button, bool highlighted, bool clicked );

public:

	ConsoleScreenInterface ();
	~ConsoleScreenInterface () override;


    /**
     * Adds a user to the chat
     * @param name Name of the user
     */
	void AddUser (const string& name );

	/**
	 * Sets the directory the console is currently in
	 * @param newcurrentdir New directory to be in
	 */
	void SetCurrentDir (const string &newcurrentdir );

	/**
	 * Adds text to the end of the message queue
	 * @param userid Userid that is providing message
	 * @param text Text to add
	 */
	void PutText (int userid, const string& text );

	/**
	 * Adds text to the start of message queue
	 * @param userid Userid that is providing message
	 * @param text Text to add
	 */
	void PutTextAtStart (int userid, const string& text );

	/**
	 * Parses command into ConsoleCommand object
	 * @param command String to parse
	 */
	void RunCommand				(const string& command );

	/**
	 * Takes a ConsoleCommand, and calls the function for the given command to execute
	 * @param cc ConsoleCommand to run
	 */
	void RunCommand				( ConsoleCommand *cc );

	/**
	 * Echoes the text to console
	 * @param text Text to echo
	 */
	void RunCommand_TEXT		(const string &text );

	/**
	 * Lists the files and directories in the current directory to console
	 */
	void RunCommand_DIR			();

	/**
	 * Prints the help message to the console
	 */
	void RunCommand_HELP		();

	/**
	 * Exits the console
	 */
	void RunCommand_EXIT		();

	/**
	 * Deletes all files in current directory
	 * @param dir Directory to delete
	 */
	void RunCommand_DELETEALL	(const string &dir );

	/**
	 * Runs given program
	 * @param program Program to run
	 * @param actuallyrun true if the command itself is run
	 */
	void RunCommand_RUN			(const string &program, bool actuallyrun );

	/**
	 * Shuts down the system
	 */
	void RunCommand_SHUTDOWN	();

	/**
	 * Disconnects the current user from the console
	 */
	static void RunCommand_DISCONNECT	();

	void Create ( ComputerScreen *cs ) override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	bool ReturnKeyPressed () override;

	int ScreenID () override;

	GenericScreen *GetComputerScreen ();


};


// ============================================================================

#define CMDTYPE_NONE		-1
#define CMDTYPE_TEXT		0				// Data1 = string
#define CMDTYPE_DIR			1
#define CMDTYPE_HELP		2
#define CMDTYPE_EXIT		3
#define CMDTYPE_DELETEALL	4				// Data1 = directory or NULL to use current
#define CMDTYPE_CD			5
#define CMDTYPE_SHUTDOWN	6
#define CMDTYPE_DISCONNECT  7
#define CMDTYPE_RUN			8				// Data1 = program name
#define CMDTYPE_RUNPROGRAM	9				// Data1 = program name, actually runs it

/**
 * Command and data for command on console
 */
class ConsoleCommand
{

public:

    /**
     * The type of the command
     * @note Valid values are CMDTYPE_NONE, CMDTYPE_TEXT, CMDTYPE_DIR, CMDTYPE_HELP, CMDTYPE_EXIT, CMDTYPE_DELETEALL, CMDTYPE_CD, CMDTYPE_SHUTDOWN, CMDTYPE_DISCONNECT, CMDTYPE_RUN, or CMDTYPE_RUNPROGRAM
     */
	int TYPE;

	/**
	 * Arguments for the command
	 */
	string data1;
	int time;

public:

	ConsoleCommand ();
	ConsoleCommand (int newTYPE, const string &newdata1, int newtime );
	~ConsoleCommand ();

};


#endif
