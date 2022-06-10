
#ifndef included_task_h
#define included_task_h

//#ifdef _DEBUG
//#include "slasher.h"
//#endif

//! Base Task Class
/**
 * Base Task Class
 * Any task that you wish to run on the VanBakel Task Manager must derive from this class and implement the virtual functions.
 */
class Task  
{

protected:

    /**
     * Called when the task has finished
     */
	void (*callback) ();

public:

	Task();
	virtual ~Task();

	/**
	 * Sets a callback function to run when this task finishes
	 * @param newcallback The callback function
	 */
	void SetCallback ( void (*newcallback) () );

	/**
	 * Initialise the task
	 */
	virtual void Initialise ();

	/**
	 * Called every clock tick
	 */
	void Tick ();

	/**
	 * Does something when a number of ticks occur
	 * @param n The number of ticks that have occurred
	 */
	virtual void Tick ( int n );

	/**
	 * Creates the task interface
	 */
	virtual void CreateInterface ();

	/**
	 * Removes the task interface
	 */
	virtual void RemoveInterface ();

	/**
	 * Bring to front
	 * @note Optional
	 */
	virtual void ShowInterface ();

	/**
	 * Checks if this is visible
	 * @return true if visible
	 */
	virtual bool IsInterfaceVisible ();

	/**
	 * Looks up the PID of this task
	 * @return The PID of the task
	 */
	int GetPID ();

};

#endif 
