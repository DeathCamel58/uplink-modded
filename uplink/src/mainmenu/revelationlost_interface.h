
/*

    Revelation Lost Interface

    The player tried to use Revelation but failed
    He is now arrested, along with all of ARC


  */


#ifndef included_revelationlostinterface_h
#define included_revelationlostinterface_h

#include "mainmenu/mainmenuscreen.h"


class RevelationLostInterface : public MainMenuScreen
{

protected:

	time_t starttime{};

protected:

	static void ReturnToMainMenuClick ( Button *button );

public:

	RevelationLostInterface ();
	~RevelationLostInterface () override;

	void Create () override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int  ScreenID () override;

};




#endif

