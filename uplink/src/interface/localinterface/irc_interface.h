
#ifndef included_ircinterface_h
#define included_ircinterface_h

// ============================================================================

#pragma warning( disable:4786 )

#include <irc.h>

#include "interface/localinterface/localinterfacescreen.h"

class UplinkIRCMessage;
class UplinkIRCMonitor;
class UplinkIRCUser;

using namespace irc;

#define IRCBUFFERSIZE 200

// ============================================================================


//! Screen for the IRC client
class IRCInterface : public LocalInterfaceScreen
{

protected:

    /**
     * Renders the black background of the IRC client
     * @param button The background button
     * @param highlighted TODO: Document
     * @param clicked TODO: Document
     */
    static void BackBlackoutDraw    ( Button *button, bool highlighted, bool clicked );

    /**
     * Renders the text in the chat window
     * @param button The chat window button
     * @param highlighted TODO: Document
     * @param clicked TODO: Document
     */
	static void MainTextDraw        ( Button *button, bool highlighted, bool clicked );

	/**
	 * Renders the connect button
	 * @param button The connect button
	 * @param highlighted TODO: Document
	 * @param clicked TODO: Document
	 */
    static void ConnectDraw         ( Button *button, bool highlighted, bool clicked );
    static void UserListDraw        ( Button *button, bool highlighted, bool clicked );

    /**
     * Connects to the server
     * @param button The connect button
     */
    static void ConnectClick        ( Button *button );

    /**
     * Sends message to server
     * @param button
     */
	static void PostClick           ( Button *button );

	/**
	 * Forces a re-render of the IRC chat box
	 * @param name The name of the button
	 * @param newValue
     * @note newValue is unused
	 */
    static void TextScrollChange    (const string &name, int newValue );

    /**
     * Forces a re-render of the user list
     * @param name The name of the button
     * @param newValue
     * @note newValue is unused
     */
    static void UserScrollChange    (const string &name, int newValue );

    /**
     * Finds a smiley in a row of text, and replaces it with an emoticon
     * @param row The row the text is on
     * @param smiley The smiley to search for
     * @param imgSmiley The image to draw in place of the smiley text
     */
    static void AddEmoticons        (int row, const string& smiley, Image *imgSmiley );

protected:

    /**
     * Image of happy smiley face
     * @note :)
     */
    static Image *imgSmileyHappy;

    /**
     * Image of sad smiley face
     * @note :(
     */
    static Image *imgSmileySad;

    /**
     * Image of winking smiley face
     * @note ;)
     */
    static Image *imgSmileyWink;

public:

    /**
     * Are we currently connected to a server
     */
	static bool connected;

	/**
	 * TODO: Document
	 */
	static LList <UplinkIRCMessage *> buffer;

	/**
	 * LList of all users in channel
	 */
    static LList <UplinkIRCUser *> users;

    /**
     * Name of the current channel
     */
    static string channelName;

    /**
     * TODO: Document
     */
    static WinsockInit *winSockInit;

    /**
     * TODO: Document
     */
    static CIrcSession *cIrcSession;

    /**
     * TODO: Document
     */
    static UplinkIRCMonitor *uplinkIRCMonitor;
    //static CIrcSessionInfo *cIrcSessionInfo;

public:

	IRCInterface();
	~IRCInterface() override;

	/**
	 * Creates the left hand window that contains the message history, chat input box, and post button
	 */
	void CreateTalkWindow();

    /**
     * Removes the left hand window that contains the message history, chat input box, and post button
     */
	void RemoveTalkWindow();

	/**
	 * Calls PostClick to send the message
	 * @return true under all conditions
	 */
    bool ReturnKeyPressed ();

    /**
     * Breaks given text into word wrapped lines, puts it into chat window, forces a re-draw
     * @param user User that sent the message
     * @param text Text that user sent
     * @param r TODO: Document
     * @param g TODO: Document
     * @param b TODO: Document
     */
	static void AddText (const string& user, const string& text,
                         float r = 1.0, float g = 1.0, float b = 1.0 );

	/**
	 * Removes all users from the user list box
	 */
    static void ResetUsers          ();

    /**
     * Adds a user to the user list box
     * @param name Username to add
     */
    static void AddUser             (string name );

    /**
     * Removes a user from the user list box
     * @param name Username to remove
     */
    static void RemoveUser          (const string& name );

    /**
     * Gets a user by name
     * @param name Username to get
     */
    static UplinkIRCUser *GetUser   (const string& name );

	void Create () override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int  ScreenID () override;

};

//! IRC Message
class UplinkIRCMessage
{

public:

	UplinkIRCMessage ();
	~UplinkIRCMessage ();

	/**
	 * Sets the message to given values
	 * @param newuser User that created the message
	 * @param newtext Text that user sent
	 * @param r TODO: Document
	 * @param g TODO: Document
	 * @param b TODO: Document
	 */
	void Set (const string& newuser, const string& newtext, float r, float g, float b );

public:

    /**
     * User that created the message
     */
	string user;

	/**
	 * Text that user sent
	 */
    string text;

    /**
     * TODO: Document
     */
	float red;

	/**
	 * TODO: Document
	 */
	float green;

	/**
	 * TODO: Document
	 */
	float blue;

};

//! IRC User
class UplinkIRCUser
{

public:

    UplinkIRCUser ();
    ~UplinkIRCUser ();

    /**
     * Set the username
     * @param newname Username
     */
    void Set (const string& newname );

public:

    /**
     * Username of user
     */
    string name;

    /**
     * Status of the user
     * TODO: Check if this is OP status, or away/here status
     */
    int status;

};

class UplinkIRCMonitor : public CIrcDefaultMonitor
{

public:

    /**
     * Gets a parameter at index in CIrcMessage
     * @param pmsg The CIrcMessage to get parameter from
     * @param n The index of the parameter
     * @return The paramater value
     */
    static string GetIrcParam(const CIrcMessage *pmsg, int n);

    void OnIrcDefault               (const CIrcMessage* pmsg) override;

    /**
     * Called when a private message is received
     * @param pmsg The message
     * @return true under all conditions
     */
    bool Received_PRIVMSG           (const CIrcMessage* pmsg);

    /**
     * Called when a TODO message is received
     * @param pmsg The message
     * @return true under all conditions
     */
	bool Received_JOIN              (const CIrcMessage* pmsg);

    /**
     * Called when a TODO message is received
     * @param pmsg The message
     * @return true under all conditions
     */
    bool Received_PART              (const CIrcMessage* pmsg);

    /**
     * Called when a topic message is received
     * @param pmsg The message
     * @return true under all conditions
     */
    bool Received_TOPIC             (const CIrcMessage* pmsg);

    /**
     * Called when a MOTD message is received
     * @param pmsg The message
     * @return true under all conditions
     */
    bool Received_MOTD              (const CIrcMessage* pmsg);

    /**
     * Called when a mode change message is received
     * @param pmsg The message
     * @return true under all conditions
     */
    bool Received_MODE              (const CIrcMessage* pmsg);
    bool Received_NICK              (const CIrcMessage* pmsg);

    /**
     * Called when a quit message is received
     * @param pmsg The message
     * @return true under all conditions
     */
    bool Received_QUIT              (const CIrcMessage* pmsg);

    /**
     * Called when an error message is received
     * @param pmsg The message
     * @return true under all conditions
     */
    bool Received_ERROR             (const CIrcMessage* pmsg);

    /**
     * Called when a banned from channel message is received
     * @param pmsg The message
     * @return true under all conditions
     */
    bool Received_ERR_BANNEDFROMCHAN(const CIrcMessage* pmsg);


    /**
     * Called when a welcome message is received
     * @param pmsg The message
     * @return true under all conditions
     */
    bool Received_RPL_WELCOME       (const CIrcMessage* pmsg);

    /**
     * Called when a TODO message is received
     * @param pmsg The message
     * @return true under all conditions
     */
    bool Received_RPL_TOPIC         (const CIrcMessage* pmsg);

    /**
     * Called when a TODO message is received
     * @param pmsg The message
     * @return true under all conditions
     */
    bool Received_RPL_NAMREPLY      (const CIrcMessage* pmsg);

    /**
     * Called when a TODO message is received
     * @param pmsg The message
     * @return true under all conditions
     */
    bool Received_RPL_ENDOFNAMES    (const CIrcMessage* pmsg);

    /**
     * Called when a TODO message is received
     * @param pmsg The message
     * @return true under all conditions
     */
    bool Received_RPL_LUSER         (const CIrcMessage* pmsg);

    /**
     * TODO: Document
     * @param session
     */
    UplinkIRCMonitor( CIrcSession &session );

	DEFINE_IRC_MAP();

};




#endif

