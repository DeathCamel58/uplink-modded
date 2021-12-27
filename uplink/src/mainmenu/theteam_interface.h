
/*

	Meet the team!
	(Secret page)
	
  */

#ifndef _included_theteaminterface_h
#define _included_theteaminterface_h


#include "mainmenu/mainmenuscreen.h"


class TheTeamInterface : public MainMenuScreen
{

protected:

	static void NameDraw ( Button *button, bool highlighted, bool clicked );
	static void TextDraw ( Button *button, bool highlighted, bool clicked );

	static void TeamMemberHighlight ( Button *button );

	static void ExitClick ( Button *button );

protected:

	int targetindex;			
	int timesync;

protected:

	void MergeCaption ( char *buttonName, char *targetCaption );

public:

	TheTeamInterface ();
	~TheTeamInterface () override;

	void Create () override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int  ScreenID () override;

};

#endif
