
#ifndef included_openglinterface_h
#define included_openglinterface_h

#include "eclipse.h"
#include "gucci.h"

/*

	Generic OpenGL functions for rendering the interface
	(defined by Eclipse)	

  */


void clear_draw       ( int x, int y, int w, int h );

void button_draw      ( Button *button, bool highlighted, bool clicked );
void button_highlight ( Button *button );
void button_click     ( Button *button );

void imagebutton_draw ( Button *button, bool highlighted, bool clicked );
void imagebutton_draw ( Button *button, bool highlighted, bool clicked, Image *standard_i_ref, Image *highlighted_i_ref, Image *clicked_i_ref );
void imagebutton_drawtextured ( Button *button, bool highlighted, bool clicked );
void imagebutton_draw_blend ( Button *button, bool highlighted, bool clicked );

void border_draw ( Button *button );
void textbutton_draw  ( Button *button, bool highlighted, bool clicked );

/**
 * Used to append text to button's caption when user interacts with it
 * @param button The button that received input
 * @param key The keycode of the input
 */
void textbutton_keypress ( Button *button, char key );

/**
 * Goes through the string and divides it up into several smaller strings, taking into account newline characters, and
 * the width of the text area.
 * @param text Text to handle
 * @param linesize The maximum size of the line in pixels
 * @return LList with separate lines in separate elements
 */
LList <string> *wordwraptext ( const string& text, int linesize );

/**
 * Takes input text, splits it into lines that fit, then draws to screen
 * @param x X position of where to draw text
 * @param y Y position of where to draw text
 * @param text The text to process and draw
 * @param linesize The maximum line width of the text
 */
void text_draw		  (int x, int y, const string& text, int linesize );

/**
 * Takes input button, splits the caption into lines that fit, then draws to screen
 * @param button The button to draw
 * @param highlighted Should the button be highlighted
 * @param clicked Is the button clicked
 */
void text_draw		  ( Button *button, bool highlighted, bool clicked );
void textbox_draw     ( Button *button, bool highlighted, bool clicked );

void buttonborder_draw ( Button *button, bool highlighted, bool clicked );

void superhighlight_draw ( Button *button, bool highlighted, bool clicked );

void button_assignbitmap  (const string &name, const string &standard_f );
void button_assignbitmap  (const string &name, Image *standard_i_ref );
void button_assignbitmaps (const string &name, const string &standard_f, const string &highlighted_f, const string &clicked_f );
void button_assignbitmaps (const string &name, Image *standard_i_ref, Image *highlighted_i_ref, Image *clicked_i_ref );

void button_assignbitmap_blend (const string &name, const string &standard_f );
void button_assignbitmaps_blend (const string &name, const string &standard_f, const string &highlighted_f, const string &clicked_f );

Image *get_assignbitmap (const string &filename );


void tooltip_update (const string &newtooltip );

Button *getcurrentbutton ();					// Returns button under mouse or NULL

//
// This set of functions can create and manage a scrollable text box
//

void create_stextbox (int x, int y, int width, int height,
                      const string& caption, const string &name );
void remove_stextbox (const string &name );

void draw_stextbox	 ( Button *button, bool highlighted, bool clicked );
void draw_scrollbox  ( Button *button, bool highlighted, bool clicked );
void stextbox_scroll (const string &name, int newValue );

//
// Functions for modal message boxes
//

void create_msgbox      (const string &title, const string &message, void (*closeclick) (Button *) = nullptr );
void create_yesnomsgbox (const string &title, const string &message, void (*yesclick) (Button *) = nullptr, void (*noclick) (Button *) = nullptr );
void remove_msgbox      ();
bool isvisible_msgbox   ();


//
// Functions to handle screen resize / button repositioning

void SetWindowScaleFactor ( float scaleX, float scaleY );
int GetScaledXPosition ( int xPos );
int GetScaledYPosition ( int yPos );

#define SX(x)       GetScaledXPosition(x)
#define SY(y)       GetScaledYPosition(y)

#endif
