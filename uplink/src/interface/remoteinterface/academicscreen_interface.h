
/*

  Interface for International academic databasae

  */


#ifndef _included_academicscreeninterface_h
#define _included_academicscreeninterface_h

// ============================================================================

#include "interface/remoteinterface/remoteinterfacescreen.h"

class GenericScreen;

// ============================================================================



class AcademicScreenInterface : public RemoteInterfaceScreen
{

protected:

	string searchname;
	int recordindex;
	int lastupdate;

protected:

	void UpdateScreen ();									// Uses recordindex

	static void DetailsDraw ( Button *button, bool highlighted, bool clicked );

	static void CommitClick			( Button *button );
	static void CloseClick			( Button *button );

public:

	AcademicScreenInterface ();
	~AcademicScreenInterface () override;

    bool EscapeKeyPressed () override;

	void SetSearchName (const string &newsearchname );


	void Create ( ComputerScreen *cs ) override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

	GenericScreen *GetComputerScreen ();

};



#endif

