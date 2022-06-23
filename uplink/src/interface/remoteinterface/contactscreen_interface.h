
#ifndef included_contactscreeninterface_h
#define included_contactscreeninterface_H

#include "tosser.h"
#include "eclipse.h"

#include "interface/remoteinterface/remoteinterfacescreen.h"

class Mission;
class GenericScreen;

#define CONTACT_TYPE_NONE		0
#define CONTACT_TYPE_MISSION	1

//! RemoteInterfaceScreen for uplink internal services mission "Secure Communication Channel open" screen
/**
 * Allows the user to talk to another person through an IRC style interface
 */
class ContactScreenInterface : public RemoteInterfaceScreen 
{

protected:

    /**
     * The type of conference this is
     * @note Valid values are CONTACT_TYPE_NONE, or CONTACT_TYPE_MISSION
     */
	int TYPE;

	/**
	 * LList of users in the chat
	 */
	LList <string> users;

	/**
	 * LList of messages not yet printed
	 */
	LList <string> messagequeue;

	/**
	 * true if waiting for message to "scroll"
	 */
	bool waiting;

	/**
	 * Number of clickable questions
	 */
	int numquestions;

	/**
	 * The Mission being discussed
	 */
	Mission *mission;

	/**
	 * The original payment amount
	 */
	int originalpayment;

	/**
	 * The original payment terms
	 */
	int originalpaymentmethod;

	/**
	 * true if payment amount has already been asked about
	 */
	bool askedpayment;

	/**
	 * true if payment terms have already been asked
	 */
	bool askedpaymentmethod;

protected:

    /**
     * Adds a message to the queue
     * @param userid The userid who sent it
     * @param message The message to put on the queue
     */
	static void PutMessage (int userid, const string& message );

	/**
	 * Handles questions being asked
	 * @param index Index of question to ask
	 */
	static void AskQuestion ( int index );

	/**
	 * Callback for when message has finished "scrolling"
	 */
	static void WaitingCallback ();

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

	/**
	 * Renders the question
	 * @param button Button for question
	 * @param highlighted Callback for highlight
	 * @param clicked Callback for click
	 */
	static void QuestionDraw ( Button *button, bool highlighted, bool clicked );

	/**
	 * Finds the question clicked, prints the question to chat, and queues the response
	 * @param button Button for question
	 */
	static void QuestionClick ( Button *button );

public:

	ContactScreenInterface ();
	~ContactScreenInterface () override;

	/**
	 * Sets the type of the chat
	 * @param newTYPE New type of chat
	 * @note Valid values are CONTACT_TYPE_NONE, or CONTACT_TYPE_MISSION
	 */
	void SetTYPE ( int newTYPE );

	/**
	 * Sets the mission this chat is about
	 * @param newmission Mission to be discussed
	 */
	void SetMission  ( Mission *newmission );

	/**
	 * Adds a user to the chat
	 * @param name Name of the user
	 */
	void AddUser (string name);

	/**
	 * Adds a question to the available ones
	 * @param question Question to add
	 * @param index Index to add the question at
	 */
	void AddQuestion (const string& question, int index = -1 );

	bool ReturnKeyPressed () override;

	void Create ( ComputerScreen *cs ) override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

	GenericScreen *GetComputerScreen ();


};


#endif
