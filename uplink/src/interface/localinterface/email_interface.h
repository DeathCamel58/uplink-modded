
#ifndef included_emailinterface_h
#define included_emailinterface_h

#include "eclipse.h"

// ============================================================================

#include "interface/localinterface/localinterfacescreen.h"

class Message;

// ============================================================================


//! Screen that shows emails
class EmailInterface : public LocalInterfaceScreen  
{

protected:

    /**
     * The index of the email
     * @note The game shows these in descending order. 0 is on the right in the game.
     */
	int index;

	/**
	 * The next time to update the screen at
	 */
	time_t timesync;

	/**
	 * The message in the current email
	 */
	Message *message;

protected:

    /**
     * Closes the screen
     * @param button
     */
	static void TitleClick ( Button *button );

    /**
     * Closes the screen
     * @param button
     */
	static void EmailClose ( Button *button );

	/**
	 * Changes currently open screen to the Send Mail interface with the email set to reply to the current one
	 * @param button
     * @note button is unused
	 */
    static void EmailReply ( Button *button );

    /**
     * Deletes the currently open email
     * @param button The delete button
     */
	static void EmailDelete ( Button *button );	

public:

	EmailInterface();
	~EmailInterface() override;

	/**
	 * Sets the message to the message of email at given index
	 * @param index index of email to get the message of
	 */
	void SetMessage ( int index );

	void Create () override;

	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int  ScreenID () override;

};

#endif
