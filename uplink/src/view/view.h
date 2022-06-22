
#ifndef included_view_h
#define included_view_h

// ============================================================================

#include <cstdio>

#include "app/uplinkobject.h"
#include "view/fps.h"

// ============================================================================

//! View class UplinkObject
/**
 * Creates a 3D representation of the game world based on the players current location in that world
 * @note Uses OpenGL
 */
class View : public UplinkObject
{

public:

    /**
     * The FrameRate of the View
     */
	FrameRate fps;

public:


	View();
	~View() override;

	void Initialise ();

	/**
	 * Ticks the fps
	 */
	void Draw ();


	// Common functions

	bool Load   ( FILE *file ) override;
	void Save   ( FILE *file ) override;
	void Print  () override;
	void Update () override;
	
	string GetID () override;

};


#endif 
