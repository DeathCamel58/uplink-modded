
/*

  InterfaceScreen class object

	Part of the Interface sub-system
	Base class for defining a screen that will be in the interface
	 for example the InRealLife screen, or a password screen etc

  */


#ifndef included_interfacescreen_h
#define included_interfacescreen_h

#include "app/uplinkobject.h"

#include "tosser.h"

class InterfaceScreen : public UplinkObject
{

protected:
	DArray<char *> *interface_buttons;
	virtual void RegisterButton ( int x, int y, int width, int height,
								  char *caption, char *name );
	virtual void RegisterButton ( int x, int y, int width, int height,
								  char *caption, char *tooltip, char *name );

public:

	InterfaceScreen();
	~InterfaceScreen() override;

	virtual void Create ();
	virtual void Remove ();
	void Update () override;
	virtual bool IsVisible ();

	virtual int  ScreenID ();			

};


#endif 
