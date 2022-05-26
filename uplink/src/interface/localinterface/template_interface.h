// MemoryInterface.h: interface for the MemoryInterface class.
//
//////////////////////////////////////////////////////////////////////

#ifndef included_memoryinterface_h
#define included_memoryinterface_h


#include "localinterfacescreen.h"


class MemoryInterface : public LocalInterfaceScreen  
{

protected:

	static void TitleClick ( Button *button );

public:

	void Create ();
	void Remove ();
	void Update ();
	bool IsVisible ();

	int ScreenID ();

};

#endif 
