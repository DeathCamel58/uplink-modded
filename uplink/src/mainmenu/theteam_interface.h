
#ifndef included_theteaminterface_h
#define included_theteaminterface_h

#include "mainmenu/mainmenuscreen.h"

//! Team interface MainMenuScreen
/**
 * Secret page that introduces the team of developers
 */
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

	static void MergeCaption (const string& buttonName, string targetCaption );

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