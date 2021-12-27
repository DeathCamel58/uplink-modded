
/*

  Warez Game Over screen

    We've detected you are A FUCKING PIRATING MUTHERFUCKER
    SO FUCK OFF 

  */


#ifndef _included_warezgameoverinterface_h
#define _included_warezgameoverinterface_h

#include "mainmenu/mainmenuscreen.h"


class WarezGameOverInterface : public MainMenuScreen
{

protected:

	time_t starttime{};

protected:

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

