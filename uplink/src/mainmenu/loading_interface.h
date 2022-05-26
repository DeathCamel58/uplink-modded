
/*

	Loading Screen Interface
	
  */

#ifndef included_loadinginterface_h
#define included_loadinginterface_h


#include "mainmenu/mainmenuscreen.h"


class LoadingInterface : public MainMenuScreen
{

public:

	LoadingInterface ();
	virtual ~LoadingInterface ();

	void Create ();
	void Remove ();
	void Update ();
	bool IsVisible ();

	int  ScreenID ();			

};

#endif
