
#ifndef included_disavowedinterface_h
#define included_disavowedinterface_h

#include "mainmenu/mainmenuscreen.h"

//! Disavowed interface MainMenuScreen
/**
 * Appears after your connection from your home PC to your Gateway is lost because you have been disavowed
 */
class DisavowedInterface : public MainMenuScreen
{

protected:

	time_t starttime{};

protected:

	static void ReturnToMainMenuClick ( Button *button );

public:

	DisavowedInterface ();
	~DisavowedInterface ();

	void Create ();
	void Remove ();
	void Update ();
	bool IsVisible ();

	int  ScreenID ();			

};




#endif

