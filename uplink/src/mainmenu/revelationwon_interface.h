
/*

    Revelation Won Interface

    Revelation has taken over the Internet and totally destroyed it!


  */


#ifndef _included_revelationwoninterface_h
#define _included_revelationwoninterface_h

#include "mainmenu/mainmenuscreen.h"


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

