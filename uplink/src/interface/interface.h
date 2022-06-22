
#ifndef _included_classinterface_h				
#define included_classinterface_h

#include <cstdio>

// ============================================================================

#include "app/uplinkobject.h"


class LocalInterface;
class RemoteInterface;
class TaskManager;

// ============================================================================

//! Interface class UplinkObject
/**
 * Responsible for handling all user input including mouse, keyboard, buttons, sounds, animations etc.
 * Includes the main HUD, the interface for the current players location and the task manager, as well as a list of all possible "tasks" - eg break password, crack code etc.
 */
class Interface : public UplinkObject
{

protected:

    /**
     * LocalInterface
     */
	LocalInterface  *li;

	/**
	 * RemoteInterface
	 */
	RemoteInterface *ri;

	/**
	 * TaskManager
	 */
	TaskManager     *tm;

public:

	Interface();
	~Interface() override;

	/**
	 * Resets and creates the Interface
	 */
	void Create ();

	/**
	 * Asserts that LocalInterface exists and returns it
	 * @return LocalInterface
	 */
	LocalInterface  *GetLocalInterface  ();

    /**
     * Asserts that RemoteInterface exists and returns it
     * @return RemoteInterface
     */
	RemoteInterface *GetRemoteInterface ();

    /**
     * Asserts that TaskManager exists and returns it
     * @return TaskManager
     */
	TaskManager     *GetTaskManager     ();
	

	// Common functions

	bool Load   ( FILE *file ) override;
	void Save   ( FILE *file ) override;
	void Print  () override;
	void Update () override;

	string GetID () override;

};


#endif 
