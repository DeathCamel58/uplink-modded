
#ifndef included_cheatinterface_h
#define included_cheatinterface_h


#include "interface/localinterface/localinterfacescreen.h"

//! LocalInterfaceScreen to provide cheats
class CheatInterface : public LocalInterfaceScreen  
{

public:

    /**
     * Closes the screen
     * @param button
     * @note button is unused
     */
	static void TitleClick ( Button *button );

	/**
	 * Give the player all links
	 * @param button
     * @note button is unused
	 */
	static void AllLinksClick		( Button *button );

	/**
	 * Gives the player all software
	 * @param button
     * @note button is unused
	 */
	static void AllSoftwareClick	( Button *button );

	/**
	 * Gives the player all hardware
	 * @param button
     * @note button is unused
	 */
	static void AllHardwareClick	( Button *button );

	/**
	 * Gives the player 10000 credits
	 * @param button
     * @note button is unused
	 */
	static void LotsOfMoneyClick	( Button *button );

	/**
	 * Sets the player's uplink rating to maximum
	 * @param button
     * @note button is unused
	 */
	static void MaxRatingsClick		( Button *button );

	/**
	 * Increases the player's uplink rating by one level
	 * @param button
     * @note button is unused
	 */
	static void NextRatingClick		( Button *button );

	/**
	 * Prints some debug information
	 * @param button
     * @note button is unused
	 */
	static void DebugPrintClick		( Button *button );

	/**
	 * Simulate a "shot by feds" event in 5 seconds
	 * @param button
     * @note button is unused
	 */
	static void EndGameClick		( Button *button );

	/**
	 * Ends any active trace
	 * @param button
     * @note button is unused
	 */
	static void CancelTraceClick    ( Button *button );

	/**
	 * Opens the event queue interface
	 * @param button
     * @note button is unused
	 */
	static void EventQueueClick		( Button *button );

	/**
	 * Runs revelation on 5 computers
	 * @param button
     * @note button is unused
	 */
    static void RevelationClick     ( Button *button );

    /**
     * Shows the LAN interface
     * @param button
     * @note button is unused
     */
	static void ShowLANClick		( Button *button );

public:

	void Create () override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

};

#endif 
