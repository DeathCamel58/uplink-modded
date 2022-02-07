
/*

  Dialog Screen class object

	Designed to be a generic Computer screen, capable of everything.
	In theory, all other computer screens could be made up of a specific dialog screen.

  */

#ifndef _included_dialog_screen
#define _included_dialog_screen

// ============================================================================

#include "world/computer/computerscreen/computerscreen.h"


class DialogScreenWidget;

// ============================================================================


class DialogScreen : public ComputerScreen  
{

public:

	LList <DialogScreenWidget *> widgets;

    char *returnkeybutton;							// This is pressed when return key is pressed
    char *escapekeybutton;                          // This is pressed when escape key is pressed

public:

	DialogScreen();
	~DialogScreen() override;

	void AddWidget (const string &name, int WIDGETTYPE, int x, int y, int width, int height, const string &caption, const string &tooltip );
	void AddWidget (const string &name, int WIDGETTYPE, int x, int y, int width, int height, const string &caption, const string &tooltip,
                    int data1, int data2, const string &stringdata1, const string &stringdata2 );

	void RemoveWidget (const string &name );

	void SetReturnKeyButton (const string &name );
    void SetEscapeKeyButton (const string &name );

	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	
	string GetID () override;
	int   GetOBJECTID () override;

};


// ============================================================================


#define WIDGET_NONE				0						// Error occurs (unassigned)
#define WIDGET_BASIC			1						// Standard blue button
#define WIDGET_CAPTION			2						// Text only, locked 
#define WIDGET_TEXTBOX			3						// Text only, editable
#define WIDGET_PASSWORDBOX		4						// Text only, editable, hidden with '*' characters
#define WIDGET_NEXTPAGE      	5						// data1 = nextpageindex
#define WIDGET_IMAGE			6						// stringdata1 = filename (normal), sdata2 = highlighted
#define WIDGET_IMAGEBUTTON		7						// data1 = nextpageindex, stringdata1/2 = filename
#define WIDGET_SCRIPTBUTTON		8						// data1 = script index, data2 = nextpageindex
#define WIDGET_FIELDVALUE		9						// stringdata1 = fieldname


class DialogScreenWidget : public UplinkObject
{
	
protected:

	char *name;
	char *caption;
	char *tooltip;

	char *stringdata1;
	char *stringdata2;

public:

	int x, y;
	int width, height;	
	int WIDGETTYPE;

	int data1;
	int data2;

public:

	DialogScreenWidget ();
	~DialogScreenWidget () override;

	void SetName (const string &newname );
	void SetTYPE ( int NEWWIDGETTYPE );
	void SetPosition ( int newx, int newy );
	void SetSize ( int newwidth, int newheight );
	void SetTooltip (const string &newtooltip );
	void SetCaption (const string &newcaption );
	
	char *GetName ();
	char *GetTooltip ();
	char *GetCaption ();

	// Data access (data and stringdata is unused)

	void SetData ( int newdata1, int newdata2 );
	void SetStringData (const string &newstringdata1, const string &newstringdata2 );

	char *GetStringData1 ();
	char *GetStringData2 ();

	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	
	string GetID () override;
	int   GetOBJECTID () override;

};

#endif 
