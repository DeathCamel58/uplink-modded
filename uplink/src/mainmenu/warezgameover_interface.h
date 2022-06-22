
#ifndef included_warezgameoverinterface_h
#define included_warezgameoverinterface_h

#include "mainmenu/mainmenuscreen.h"

//! Warez game over MainMenuScreen
/**
 * Displays the game over screen on pirated copies of the game
 * @note Original comment was "We've detected you are A FUCKING PIRATING MUTHERFUCKER SO FUCK OFF"
 */
class WarezGameOverInterface : public MainMenuScreen
{

protected:

    /**
     * The time that this screen was opened
     */
	time_t starttime{};

protected:

    /**
     * Returns to main menu
     * @param button
     * @note button is unused
     */
	static void ReturnToMainMenuClick ( Button *button );

public:

	WarezGameOverInterface ();
	~WarezGameOverInterface () override;

	void Create () override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int  ScreenID () override;

};




#endif

