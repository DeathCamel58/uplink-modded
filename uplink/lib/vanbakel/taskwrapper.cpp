// TaskWrapper.cpp: implementation of the TaskWrapper class.
//
//////////////////////////////////////////////////////////////////////

#include <cstdio>
#include <string>
#include <iostream>

#include "taskwrapper.h"

#include "mmgr.h"

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TaskWrapper::TaskWrapper()
{

	task = nullptr;
	name = "";
	priority = 0.0;
	progress = 0.0;

}

TaskWrapper::~TaskWrapper()
= default;

void TaskWrapper::SetPID ( int newpid )
{

	pid = newpid;

}

void TaskWrapper::SetName (const string &newname )
{

	name = newname;

}

void TaskWrapper::SetTask ( Task *newtask )
{

	task = newtask;

}

void TaskWrapper::SetPriority ( double newpriority )
{

	priority = newpriority;

}

void TaskWrapper::DebugPrint () const
{

    cout << "TASK : " << name << endl;
    cout << "\tPID: " << pid << endl;
    cout << "\tPriority: " << priority << endl;
    cout << "\tProgress: " << progress << endl;

}


