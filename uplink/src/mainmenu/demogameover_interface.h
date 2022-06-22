
#ifndef included_demogameoverinterface_h
#define included_demogameoverinterface_h

#include "mainmenu/mainmenuscreen.h"

//! Demo game over interface MainMenuScreen
/**
 * Appears when the demo has come to an end
 */
class DemoGameOverInterface : public MainMenuScreen
{

protected:

	time_t starttime{};

protected:

	static void ReturnToMainMenuClick ( Button *button );

public:

	DemoGameOverInterface ();
	~DemoGameOverInterface ();

	void Create ();
	void Remove ();
	void Update ();
	bool IsVisible ();

	int  ScreenID ();			

};




#endif

