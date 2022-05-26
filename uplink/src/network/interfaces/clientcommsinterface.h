// ClientCommsInterface.h: interface for the ClientCommsInterface class.
//
//////////////////////////////////////////////////////////////////////

#ifndef included_clientcommsinterface_h
#define included_clientcommsinterface_h

// ============================================================================

#include "eclipse.h"

#include "network/interfaces/networkscreen.h"

class VLocation;
class WorldMapLayout;

// ============================================================================


class ClientCommsInterface : public NetworkScreen  
{

protected:

    static int stipplepattern;							// Used for drawing connection lines
    static void CycleStipplePattern ();					//

    static void MainMenuClick ( Button *button );
    static void LargeMapDraw ( Button *button, bool highlighted, bool clicked );

public:

    static BTree <VLocation *> locations;
    static LList <char *> connection;
    static int traceprogress;
    
protected:

    static WorldMapLayout *layout;
    static void DrawAllObjects();
    static bool InConnection(const string& computerName );

public:

    ClientCommsInterface();
    ~ClientCommsInterface() override;
    
    static void LayoutLabels();
    
    void Create () override;
    void Remove () override;
    void Update () override;
    bool IsVisible () override;

    int  ScreenID () override;

};

#endif 
