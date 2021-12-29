// TaskWrapper.cpp: implementation of the TaskWrapper class.
//
//////////////////////////////////////////////////////////////////////

#include <cstdio>
#include <cstring>

#include "taskwrapper.h"

#include "mmgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TaskWrapper::TaskWrapper()
{

	task = nullptr;
	name = nullptr;
	priority = 0.0;
	progress = 0.0;

}

TaskWrapper::~TaskWrapper()
{

	delete [] name;

}

void TaskWrapper::SetPID ( int newpid )
{

	pid = newpid;

}

void TaskWrapper::SetName ( char *newname )
{

	delete name;
	name = new char [strlen(newname)+1];
	strcpy ( name, newname );

}

void TaskWrapper::SetTask ( Task *newtask )
{

	task = newtask;

}

void TaskWrapper::SetPriority ( double newpriority )
{

	priority = newpriority;

}

void TaskWrapper::DebugPrint ()
{

	printf ( "TASK :   %s : PID:'%d', priority:%f, progress:%f\n", name, pid, (float) priority, (float) progress );

}


