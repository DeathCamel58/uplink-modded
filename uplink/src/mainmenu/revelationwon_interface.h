
#ifndef included_revelationwoninterface_h
#define included_revelationwoninterface_h

#include "mainmenu/mainmenuscreen.h"

//! Revelation won interface MainMenuScreen
/**
 * Revelation has taken over the Internet and totally destroyed it!
 */
class RevelationWonInterface : public MainMenuScreen
{

protected:

	time_t starttime{};

protected:

	static void ReturnToMainMenuClick ( Button *button );

public:

	RevelationWonInterface ();
	~RevelationWonInterface () override;

	void Create () override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int  ScreenID () override;

};




#endif

