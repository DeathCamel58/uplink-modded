
/*
   ============================================================================

			E C L I P S E

		  Interface Library
		By Christopher Delay

		Started 23rd May '99

   ============================================================================
*/

#ifndef included_eclipse_h
#define included_eclipse_h

/*
#ifdef DMALLOC
#include "dmalloc.h"
#endif
*/

/*
#ifdef _DEBUG
#include "slasher.h"
#endif
*/

#include "button.h"
#include "animation.h"

#include "tosser.h"

#define local static



// Initialisation functions ===================================================


void EclReset ( int width, int height );

void EclRegisterButton (int x, int y, int width, int height,
                        const string &caption, const string &name );

void EclRegisterButton (int x, int y, int width, int height,
                        const string &caption, const string &tooltip, const string &name );

void EclRegisterImageButton (int x, int y, int width, int height,
                             const string &caption, const string &tooltip, const string &name );

void EclRemoveButton (const string &name );

void EclButtonBringToFront (const string &name );
void EclButtonSendToBack   (const string &name );

void EclRegisterDefaultButtonCallbacks ( void (*draw) (Button *, bool, bool),
										 void (*mouseup) (Button *),
										 void (*mousedown) (Button *),
										 void (*mousemove) (Button *) );

void EclRegisterButtonCallbacks (const string &name,
                                 void (*draw) (Button *, bool, bool),
                                 void (*mouseup) (Button *),
                                 void (*mousedown) (Button *),
                                 void (*mousemove) (Button *) );

void EclRegisterButtonCallback (const string &name,
                                void (*mouseup) (Button *) );


void EclMakeButtonEditable (const string &name );
void EclMakeButtonUnEditable (const string &name );
bool EclIsButtonEditable (const string &name );
void EclHighlightNextEditableButton ();

// Graphical functions ========================================================

void EclDirtyButton		  (const string &name );					// Tells eclipse that this needs re-drawing
void EclDirtyRectangle	  ( int x, int y, int w, int h );	// Tells eclipse to blank this area
void EclDirtyClear        ();								// Clears all dirty buttons / rectangles
void EclClearRectangle    ( int x, int y, int w, int h );   // Tells eclipse to blank this area (now)

bool EclIsOccupied        ( int x, int y, int w, int h );	// True if there is a button here

void EclDrawAllButtons    ();
void EclDrawButton        (const string &name );
void EclDrawButton        ( int index );

void EclHighlightButton   (const string &name );
bool EclIsHighlighted     (const string &name );
void EclUnHighlightButton ();

void EclClickButton       (const string &name );
bool EclIsClicked         (const string &name );
void EclUnClickButton     ();

void EclSuperHighlight        (const string &name );
bool EclIsSuperHighlighted    (const string &name );
void EclSuperUnHighlight      (const string &name );
void EclSuperUnHighlightAll	  ();
void EclUpdateSuperHighlights (const string &name );

void EclRegisterClearDrawFunction ( void (*draw) (int, int, int, int) );
void EclRegisterSuperHighlightFunction ( int borderwidth, void (*draw) (Button *, bool, bool) );


// Lookup functions ===========================================================


int  EclLookupIndex (const string &name );				// Can change
string EclGetButtonAtCoord (int x, int y );
string EclGetHighlightedButton ();
Button *EclGetButton (const string &name );


// Animation functions ========================================================


#define MOVE_STRAIGHTLINE    1
#define MOVE_XFIRST          2
#define MOVE_YFIRST          3

void EclEnableAnimations ();				// Default
void EclDisableAnimations ();

#define ECL_FASTER_ANIMATION_SPEED 2.0

void EclEnableFasterAnimations ( double speed = ECL_FASTER_ANIMATION_SPEED );
void EclDisableFasterAnimations ();			// Default


int  EclRegisterMovement  (const string &bname, int targetX, int targetY, int time_ms,
                           void (*callback) () = nullptr);
int  EclRegisterMovement  (const string &bname, int targetX, int targetY, int time_ms, int MOVETYPE,
                           void (*callback) () = nullptr);
int  EclRegisterResize    (const string &bname, int targetW, int targetH, int time_ms,
                           void (*callback) () = nullptr);
int  EclRegisterAnimation (const string &bname, int targetX, int targetY,
                           int targetW, int targetH, int time_ms,
                           void (*callback) () = nullptr);

int  EclRegisterCaptionChange (const string &bname, const string &targetC, int time_ms,
                               void (*callback) () = nullptr);
int  EclRegisterCaptionChange (const string &bname, const string &targetC,				// Determines time_ms from
							    void (*callback) () = nullptr);			// length of caption

int  EclRegisterAnimation (const string &bname, int targetX, int targetY, int MOVETYPE,
                           int targetW, int targetH, const string &targetC, int time_ms,
                           void (*callback) () = nullptr);

int EclIsAnimationActive (const string &bname );					// Returns ID number of anim, or -1
int EclIsCaptionChangeActive (const string &bname );
int EclIsNoCaptionChangeActive (const string &bname );

void EclRemoveAnimation ( int idnumber );


int EclAnimationsRunning();
void EclUpdateAllAnimations ();

double EclGetAccurateTime ();				// Returns the time in millisecond accuracy

// ============================================================================


void EclDebugPrint ();					// Prints out all state information


#endif

