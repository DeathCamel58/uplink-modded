
/*

  Proxy Disable software

	Part of the Task Manager sub system

  */


#ifndef _included_proxydisable_h
#define _included_proxydisable_h

#include "eclipse.h"

#include "world/vlocation.h"

#include "interface/taskmanager/uplinktask.h"



class ProxyDisable : public UplinkTask
{

protected:

	static void ProgressDraw ( Button *button, bool highlighted, bool clicked );
	static void BorderDraw   ( Button *button, bool highlighted, bool clicked );

	static void GoClick		 ( Button *button );
	static void CloseClick   ( Button *button );

protected:

	char ip [SIZE_VLOCATION_IP];				// Target IP

	int progress;								// 0 = static, 1 = analysing, 2 = disabling, 3 = unable to proceed, 4 = done
	
	int ticksdone;								//
	int ticksrequired;							// Used to syncronise with task manager

	time_t timersync;								// Used to syncronise events to real time

public:

	ProxyDisable ();
	~ProxyDisable () override;

	void SetTarget ( UplinkObject *uo, char *uos, int uoi ) override;
	void MoveTo ( int x, int y, int time_ms ) override;

	void Initialise () override;
	void Tick ( int n ) override;
	
	void CreateInterface () override;
	void RemoveInterface () override;
	void ShowInterface () override;

	bool IsInterfaceVisible () override;

};

#endif

