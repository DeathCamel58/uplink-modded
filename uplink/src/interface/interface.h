
/*
	Interface class object

		Responsible for handling all user input 
		including mouse, keyboard, buttons, sounds, animations etc.

		Includes the main HUD, the interface for the current players location
		and the task manager, as well as a list of all possible "tasks" - 
		eg break password, crack code etc.

  */


#ifndef _included_classinterface_h				
#define _included_classinterface_h

#include <cstdio>

// ============================================================================

#include "app/uplinkobject.h"


class LocalInterface;
class RemoteInterface;
class TaskManager;

// ============================================================================


class Interface : public UplinkObject
{

protected:

	LocalInterface  *li;
	RemoteInterface *ri;
	TaskManager     *tm;

public:

	Interface();
	~Interface() override;

	void Create ();


	LocalInterface  *GetLocalInterface  ();				// Asserts li
	RemoteInterface *GetRemoteInterface ();				// Asserts ri
	TaskManager     *GetTaskManager     ();				// Asserts tm
	

	// Common functions

	bool Load   ( FILE *file ) override;
	void Save   ( FILE *file ) override;
	void Print  () override;
	void Update () override;

	string GetID () override;

};


#endif 
