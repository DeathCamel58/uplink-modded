// MemoryInterface.h: interface for the MemoryInterface class.
//
//////////////////////////////////////////////////////////////////////

#ifndef included_memoryinterface_h
#define included_memoryinterface_h


#include "interface/localinterface/localinterfacescreen.h"


class MemoryInterface : public LocalInterfaceScreen  
{

protected:

	static int previousnumfiles;

	static int baseoffset;
	static int currentprogramindex;					// Currently highlighted program

    static int specialHighlight;

protected:

	static void MemoryBlockDraw      ( Button *button, bool highlighted, bool clicked );
	static void MemoryBlockHighlight ( Button *button );
	static void MemoryBlockClick	 ( Button *button );
	
	static void TitleClick ( Button *button );
    static void ScrollChange (const string &scrollname, int newValue );

public:

    static void ForceUpdateAll ();
    static void SpecialHighlight ( int memoryIndex );

	void Create () override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

};

#endif 
