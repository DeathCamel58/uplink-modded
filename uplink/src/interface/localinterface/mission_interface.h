
#ifndef included_missioninterface_h
#define included_missioninterface_h

// ============================================================================

#include "interface/localinterface/localinterfacescreen.h"

class Mission;

// ============================================================================

//! LocalInterfaceScreen that shows Mission data
class MissionInterface : public LocalInterfaceScreen  
{

protected:

    /**
     * The index of the mission
     * @note The game shows these in descending order. 0 is on the right in the game.
     */
	int index;

    /**
     * The next time to update the screen at
     */
	time_t timesync;

    /**
     * The message in the current mission
     */
	Mission *mission;

protected:

    /**
     * Closes the screen
     * @param button
     * @note button is unused
     */
	static void TitleClick ( Button *button );

	/**
	 * Abandons the mission currently open
	 * @param button Button clicked to abandon mission
	 */
	static void AbandonClick ( Button *button );

	/**
	 * Replies to the mission currently open
	 * @param button Button clicked to reply to the mission
	 * @note This opens the sendmail screen
	 */
	static void ReplyClick ( Button *button );

public:

	MissionInterface ();
	~MissionInterface () override;

	/**
	 * Sets the currently open mission to the mission at an index
	 * @param index index of mission
	 */
	void SetMission ( int index );

	void Create () override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

};

#endif 
