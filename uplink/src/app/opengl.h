/*! \file opengl.h
 * \brief Generic OpenGL functions
 * This file contains all generic OpenGL code. This includes  the mouse/keyboard handlers, idle functions, etc
 */


#ifndef included_opengl_h
#define included_opengl_h

/**
 * Initializes the window
 * @param argc TODO: Document
 * @param argv TODO: Document
 */
void opengl_initialise ( int argc, char **argv );

/**
 * Calls the main loop of GUCCI
 */
void opengl_run ();

/**
 * Calls GUCCI's application closing function
 */
void opengl_close ();

#ifdef WIN32
bool opengl_isSoftwareRendering ();
bool opengl_setSoftwareRendering ( bool softwareRendering );
#endif

/**
 * Gets the X coordinate of the mouse
 * @return X coordinate of mouse
 */
int get_mouseX ();

/**
 * Gets the Y coordinate of the mouse
 * @return Y coordinate of mouse
 */
int get_mouseY ();

/**
 * Sets all the callbacks that should occur on various interactions
 */
void setcallbacks ();

void display ();
//void keyboard(unsigned char key, int x, int y);
//void specialkeyboard (int key, int x, int y);
//void idle ();
//void mouse ( int button, int state, int x, int y );
//void passivemouse ( int x, int y );
//void resize(int width, int height);


#endif
