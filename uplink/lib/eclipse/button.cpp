
#define FUDGE 0

// Source file for button class
// Part of the Eclipse interface library
// By Christopher Delay

#include <cstdio>
#include <cstring>

#include "eclipse.h"
#include "button.h"

#include "mmgr.h"


Button::Button ()
{

	x = y = width = height = 0;
	caption = name = tooltip = "";
	draw = nullptr;
	image_standard = image_highlighted = image_clicked = nullptr;
	mouseup = mousedown = mousemove = nullptr;
	SetTooltip ( " " );
	userinfo = 0;

}

Button::Button (int newx, int newy, int newwidth, int newheight,
				const string &newcaption, const string &newname )
{

	caption = name = tooltip = "";
	SetProperties ( newx, newy, newwidth, newheight + FUDGE, newcaption, newname );
	draw = nullptr;
	image_standard = image_highlighted = image_clicked = nullptr;
	mouseup = mousedown = mousemove = nullptr;
	SetTooltip ( " " );
	userinfo = 0;

}

Button::~Button ()
{

	caption = tooltip = name = "";

	// There is a possibility that this button may use the 
	// same image for more than one of these pointers
	// So don't neccisarily delete all three

	bool delete_standard    = ( image_standard    != nullptr );
	bool delete_highlighted = ( image_highlighted != nullptr && image_highlighted != image_standard );
	bool delete_clicked     = ( image_clicked     != nullptr && image_clicked     != image_standard
														  && image_clicked	   != image_highlighted );

	if ( delete_standard )    delete image_standard;
	if ( delete_highlighted ) delete image_highlighted;
	if ( delete_clicked )     delete image_clicked;

}

void Button::SetProperties (int newx, int newy, int newwidth, int newheight,
							const string &newcaption, const string &newname )
{

	x = newx;
	y = newy;
	width = newwidth;
	height = newheight;
	name = newname;
	SetCaption ( newcaption );
	
}

void Button::SetCaption (const string &newcaption )
{

	if ( caption.empty() || newcaption != caption )
		Dirty ();

	caption = newcaption;

}

void Button::SetTooltip (const string &newtooltip )
{

	tooltip = newtooltip;

}

void Button::SetStandardImage ( Image *newimage )
{

	delete image_standard;
	image_standard = newimage;

}

void Button::SetImages ( Image *newstandard, Image *newhighlighted, Image *newclicked )
{

	delete image_standard;
	delete image_highlighted;
	delete image_clicked;

	image_standard = newstandard;
	image_highlighted = newhighlighted;
	image_clicked = newclicked;

}

void Button::RegisterDrawFunction ( void (*newdraw) ( Button *, bool, bool ) )
{

	draw = newdraw;

}

void Button::RegisterMouseUpFunction ( void (*newmouseup) ( Button * ) )
{

	mouseup = newmouseup;

}

void Button::RegisterMouseDownFunction ( void (*newmousedown) ( Button * ) )
{

	mousedown = newmousedown;

}

void Button::RegisterMouseMoveFunction  ( void (*newmousemove) ( Button * ) )
{

	mousemove = newmousemove;

}

void Button::Dirty ()
{

	dirty = true;

}

void Button::Draw ( bool highlighted, bool clicked )
{

	if ( draw ) draw ( this, highlighted, clicked );

}

void Button::MouseUp ()
{

	if ( mouseup ) mouseup ( this );

}

void Button::MouseDown ()
{

	if ( mousedown ) mousedown ( this );

}

void Button::MouseMove ()
{

	if ( mousemove ) mousemove ( this );

}

void Button::DebugPrint () const
{

	cout << "BUTTON : name:'" << name << "', x:" << x << ", y:" << y << ", width:" << width << ", height:" << height << endl;
	cout << "         caption:'" << caption << "', tooltip:'" << tooltip << "'" << endl;

}

