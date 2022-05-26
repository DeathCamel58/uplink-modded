// StatusInterface.h: interface for the StatusInterface class.
//
//////////////////////////////////////////////////////////////////////

#ifndef included_statusinterface_h
#define included_statusinterface_h


#include "interface/localinterface/localinterfacescreen.h"


class StatusInterface : public LocalInterfaceScreen  
{

protected:

	static void MiniTitleDraw ( Button *button, bool highlighted, bool clicked );

	static void TitleClick ( Button *button );

public:

	void Create () override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

};

#endif 
