
/*

  Interface for International Social Security database

  */


#ifndef _included_socialsecurityscreeninterface_h
#define _included_socialsecurityscreeninterface_h

// ============================================================================

#include "interface/remoteinterface/remoteinterfacescreen.h"

class GenericScreen;

// ============================================================================



class SocialSecurityScreenInterface : public RemoteInterfaceScreen
{

protected:

	char *searchname;
	int recordindex;
	int lastupdate;

protected:

	void UpdateScreen ();									// Uses recordindex

	static void DetailsDraw ( Button *button, bool highlighted, bool clicked );

	static void CommitClick			( Button *button );
	static void CloseClick			( Button *button );

public:

	SocialSecurityScreenInterface ();
	~SocialSecurityScreenInterface () override;

	void SetSearchName ( char *newsearchname );


	void Create ( ComputerScreen *cs ) override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

//	bool ReturnKeyPressed ();				// Can't do this - need to be able to press return
    bool EscapeKeyPressed () override;

	GenericScreen *GetComputerScreen ();

};



#endif

