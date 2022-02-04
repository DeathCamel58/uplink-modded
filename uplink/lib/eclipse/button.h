

// Button class
// Part of the Eclipse interface library
// By Christopher Delay

#ifndef _included_button_h
#define _included_button_h


#include "gucci.h"


class Button
{

public:

	int x;
	int y;
	int width;
	int height;
	string caption;
	string name;
	string tooltip;
	int userinfo;								// Used to represent other program data about the button.

	bool dirty;

	Image *image_standard;					// Used when this button is 
	Image *image_highlighted;					// represented by an image
	Image *image_clicked;

protected:

	void (*draw)      ( Button *, bool, bool );   // Draws the button.  highlighted, clicked.
	void (*mousedown) ( Button * );               // Called when mouse down occurs
	void (*mouseup)   ( Button * );               // Called when mouse up occurs
	void (*mousemove) ( Button * );               // Called when moved over

public:

	Button ();
	Button (int newx, int newy, int newwidth, int newheight,
            const string &newcaption, const string &newname );

	virtual ~Button ();

	void SetProperties (int newx, int newy, int newwidth, int newheight,
                        const string &newcaption, const string &newname );

	void SetCaption (const string &newcaption );
	void SetTooltip (const string &newtooltip );
	void SetStandardImage ( Image *newimage );
	void SetImages ( Image *newstandard, Image *newhighlighted, Image *newclicked );	

	void RegisterDrawFunction       ( void (*newdraw)      (Button *, bool, bool) );
	void RegisterMouseUpFunction    ( void (*newmouseup)   (Button *) );
	void RegisterMouseDownFunction  ( void (*newmousedown) (Button *) );	
	void RegisterMouseMoveFunction  ( void (*newmousemove) (Button *) );

	void Dirty ();

	void Draw ( bool highlighted, bool clicked );
	void MouseUp ();
	void MouseDown ();
	void MouseMove ();
	
	void DebugPrint () const;

};

#endif
