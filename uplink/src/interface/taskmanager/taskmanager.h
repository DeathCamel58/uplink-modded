
/* 
	
  Task Manager class object

	Part of the Interface sub-system
	Handles your local computer's task list which may or may not
	 be visible at any point.  Note that this class is mostly a
	 mapping into the Svb library, which does the real work.

  */

// ============================================================================

#ifndef included_tasks_h
#define included_tasks_h


#include "vanbakel.h"

#include "app/uplinkobject.h"

class UplinkTask;

// ============================================================================



class TaskManager : public UplinkObject
{

protected:

	int			  targetprogram;						// PID of running program targetted
	int			  targetprogramreason;					// Reason the target program was changed
	int			  lasttargetprogram;					// PID of last running program targetted
	UplinkObject *programtarget;						// Object to be targeted by program

	int	lastcycle;									// Used to regulate cycle speed

public:

	TaskManager ();
	~TaskManager () override;

	void RunSoftware (const string &name, float version );
												
	void SetProgramTarget (UplinkObject *newprogramtarget, const string &targetstring, int targetint );
	void SetTargetProgram ( int newtargetprogram );

	bool IsTargetProgramLast ( int newtargetprogram ) const;

	UplinkTask *GetTargetProgram () const;					// Can return NULL
    string GetTargetProgramName () const;                      // Can return NULL

	// Common functions

	bool Load   ( FILE *file ) override;
	void Save   ( FILE *file ) override;
	void Print  () override;
	void Update () override;

	string GetID () override;

};


#endif

