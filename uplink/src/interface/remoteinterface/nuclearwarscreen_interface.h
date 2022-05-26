
/*

  Nuclear War screen interface

	Run from the Protovision system
    Plays as a basic version of Asteroids

  */


#ifndef included_nuclearwarscreeninterface_h
#define included_nuclearwarscreeninterface_h

#include "interface/remoteinterface/remoteinterfacescreen.h"

class GenericScreen;
struct NuclearWar_Nuke;


class NuclearWarScreenInterface : public RemoteInterfaceScreen
{

protected:

	static void DrawBlack ( Button *button, bool highlighted, bool clicked );

	static void DrawLocation ( Button *button, bool highlighted, bool clicked );
	static void ClickLocation ( Button *button );

    static void DrawMainMap ( Button *button, bool highlighted, bool clicked );

    static void CloseClick ( Button *button );

protected:

    LList <NuclearWar_Nuke *> nukes;

public:

	NuclearWarScreenInterface ();
	~NuclearWarScreenInterface () override;

	bool ReturnKeyPressed () override;

	void Create ( ComputerScreen *newcs ) override;
	void Remove () override;
    void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

	GenericScreen *GetComputerScreen ();

};


struct NuclearWar_Nuke
{

    int sx;
    int sy;

    int x;
    int y;
    int time;

    bool sound;
    
};

#endif

