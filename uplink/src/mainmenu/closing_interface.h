
#ifndef included_closinginterface_h
#define included_closinginterface_h

#include "mainmenu/mainmenuscreen.h"

//! Closing interface MainMenuScreen
/**
 * Shows an advert as Uplink Closes down
 */
class ClosingInterface : public MainMenuScreen
{

protected:

    static void LargeTextDraw ( Button *button, bool highlighted, bool clicked );

public:

    int timesync;
    int status;
    int count;

public:

    ClosingInterface ();
    ~ClosingInterface () override;

	void Create () override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int  ScreenID () override;

};


#endif
