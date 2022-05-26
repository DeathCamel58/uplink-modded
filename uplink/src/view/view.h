
/*

  View class object

	Creates a 3D representation of the game world
	based on the players current location in that world
	Uses OpenGL

  */


#ifndef included_view_h
#define included_view_h

// ============================================================================

#include <cstdio>

#include "app/uplinkobject.h"
#include "view/fps.h"

// ============================================================================


class View : public UplinkObject
{

public:

	FrameRate fps;

public:


	View();
	~View() override;

	void Initialise ();
	void Draw ();					// This is the big one


	// Common functions

	bool Load   ( FILE *file ) override;
	void Save   ( FILE *file ) override;
	void Print  () override;
	void Update () override;
	
	string GetID () override;

};


#endif 
