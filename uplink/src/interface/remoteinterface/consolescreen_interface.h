
/*

  Console Screen Interface

	Allows extremely low level access to 
	a computer system

  */

#ifndef included_consolescreeninterface_h
#define included_consolescreeninterface_H

// ============================================================================

#include "tosser.h"
#include "eclipse.h"

#include "interface/remoteinterface/remoteinterfacescreen.h"

class GenericScreen;
class ConsoleCommand;

// ============================================================================


class ConsoleScreenInterface : public RemoteInterfaceScreen 
{

protected:
	
	LList <char *> users;							// 0 is system, 1 is player
	
	LList <ConsoleCommand *> queue;					// Commands NOT YET RUN
	bool waiting;									// True if waiting for commands to "scroll" on

	int timesync;									// Used to syncronise things
	string currentdir;

	bool deleted_sys;

protected:

	static bool IsVisibleInterface ();

	static void WaitingCallback ();							// Called when messagehas finished "scrolling" on
	
	static void TypeHereDraw ( Button *button, bool highlighted, bool clicked );
	static void PostClick ( Button *button );

	static void BorderDraw ( Button *button, bool highlighted, bool clicked );
	static void MessageDraw ( Button *button, bool highlighted, bool clicked );

public:

	ConsoleScreenInterface ();
	~ConsoleScreenInterface () override;

	void AddUser (const string& name );

	void SetCurrentDir (const string &newcurrentdir );
	
	void PutText (int userid, const string& text );					// Adds text to end of queue
	void PutTextAtStart (int userid, const string& text );
	
	void RunCommand				(const string& command );				// Parses command into ConsoleCommand object
	void RunCommand				( ConsoleCommand *cc );

	void RunCommand_TEXT		(const string &text );
	void RunCommand_DIR			();
	void RunCommand_HELP		();
	void RunCommand_EXIT		();
	void RunCommand_DELETEALL	(const string &dir );
	void RunCommand_RUN			(const string &program, bool actuallyrun );				// If true, the command itself is run
	void RunCommand_SHUTDOWN	();
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


class ConsoleCommand
{

public:

	int TYPE;
	string data1;
	int time;

public:

	ConsoleCommand ();
	ConsoleCommand (int newTYPE, const string &newdata1, int newtime );
	~ConsoleCommand ();

};


#endif
