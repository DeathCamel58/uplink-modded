
#ifndef included_statusinterface_h
#define included_statusinterface_h


#include "interface/localinterface/localinterfacescreen.h"

//! LocalInterfaceScreen that displays the user's status
class StatusInterface : public LocalInterfaceScreen  
{

protected:

    /**
     * Renders the section titles
     * @param button Button for section titles
     * @param highlighted
     * @param clicked
     * @note highlighted and clicked are unused
     */
	static void MiniTitleDraw ( Button *button, bool highlighted, bool clicked );

    /**
     * Closes the screen
     * @param button Button for the title
     * @note button is unused
     */
	static void TitleClick ( Button *button );

public:

	void Create () override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

};

#endif 
