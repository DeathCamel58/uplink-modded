
/*

  Interface definition for Connection Analyser

  */



#ifndef included_analyserinterface_h
#define included_analyserinterface_h


#include "world/vlocation.h"

#include "interface/localinterface/localinterfacescreen.h"



class AnalyserInterface : public LocalInterfaceScreen
{

protected:

	static char remotehost [SIZE_VLOCATION_IP];						
	
	static int moving_stipplepattern;
	static int static_stipplepattern;

	time_t lastupdate;
	int stippleupdate;

protected:
	
	static void CycleStipplePattern ();

	static void TitleClick ( Button *button );
	static void SystemClick ( Button *button );

	static void ConnectionDraw ( Button *button, bool highlighted, bool clicked );

public:

	AnalyserInterface ();
	~AnalyserInterface () override;

	void Create () override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

};


#endif

