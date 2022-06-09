// SendMailInterface.h: interface for the SendMailInterface class.
//
//////////////////////////////////////////////////////////////////////

#ifndef included_sendmailinterface_h
#define included_sendmailinterface_h

// ============================================================================

#include "localinterfacescreen.h"

class Message;

// ============================================================================


//! Screen that allows the user to compose an email
class SendMailInterface : public LocalInterfaceScreen  
{

protected:

    /**
     * Closes the screen
     * @param button
     * @note button is unused
     */
	static void TitleClick ( Button *button );

	/**
	 * Toggles the file list
	 * @param button Button used to toggle the file list
	 */
	static void FileClick ( Button *button );

	/**
	 * Attaches the file to email
	 * @param button Button for a file
	 */
	static void AttachFileClick ( Button *button );

	/**
	 * Clears all boxes in the send mail interface
	 * @param button Button to clear interface
	 */
	static void ClearClick ( Button *button );

	/**
	 * Sends the current email
	 * @param button Button to send email
	 */
	static void SendClick ( Button *button );

	/**
	 * Is the file list currently open
	 * @return true if it is
	 */
	static bool IsVisibleFileList ();

	/**
	 * Creates/Destroys the file list window
	 * @param x X coordinate
	 * @param y Y coordinate
	 */
	static void ToggleFileList ( int x, int y );	

	/**
	 * Toggles the address list
	 * @param button Button used to toggle the address click
	 */
	static void ToClick ( Button *button );

	/**
	 * Is the address list currently open
	 * @return true if it is
	 */
	static bool IsVisibleAddressBook ();

	/**
	 * Creates/Destroys the address list window
	 * @param x X coordinates
	 * @param y Y coordinates
	 */
	static void ToggleAddressBook ( int x, int y );

	/**
	 * Sets the email to the clicked address
	 * @param button Button for address
	 */
	static void AddressClick ( Button *button );

protected:

    /**
     * The message being composed
     */
	static Message *m;

	/**
	 * Has the message changed?
	 */
	static bool changed;

public:

	SendMailInterface ();
	~SendMailInterface () override;

	/**
	 * Gets the message
	 * @return The message
	 */
    static Message *sGetMessage ();

	void Create () override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

};

#endif 
