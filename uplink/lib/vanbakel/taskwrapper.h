// TaskWrapper.h: interface for the TaskWrapper class.
//
//////////////////////////////////////////////////////////////////////

#ifndef included_taskwrapper_h
#define included_taskwrapper_h

#include "task.h"

//#ifdef _DEBUG
//#include "slasher.h"
//#endif

class TaskWrapper 
{

public:

	int pid;			 // Unique program identifier
	std::string name;
	Task *task;          // The task itself (derived from Task)
	double priority;      // fraction of total CPU time allocated to this task
	double progress;	  // Time accumulated on process

public:

	TaskWrapper();
	virtual ~TaskWrapper();
	
	void SetPID ( int newpid );
	void SetName (const std::string &newname );
	void SetTask ( Task *newtask );
	void SetPriority ( double newpriority );
	
	void DebugPrint () const;

};

#endif 