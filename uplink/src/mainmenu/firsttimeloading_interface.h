
#ifndef included_firsttimeloadinginterface_h
#define included_firsttimeloadinginterface_h

#include "mainmenu/mainmenuscreen.h"

//! Loading MainMenuScreen for first time players
class FirstTimeLoadingInterface : public MainMenuScreen
{

public:

	FirstTimeLoadingInterface ();
	~FirstTimeLoadingInterface () override;

	void Create () override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int  ScreenID () override;

};

#endif
