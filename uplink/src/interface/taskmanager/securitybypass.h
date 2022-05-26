
/*

  Security Bypass

	Similar to disable but this software runs continuously, 
	bypassing a single proxy server.

	This is a generic task designed to run on any security system

  */



#ifndef included_proxybypass_h
#define included_proxybypass_h

#include "world/vlocation.h"

#include "interface/taskmanager/uplinktask.h"


class SecurityBypass : public UplinkTask
{

protected:

	static void PauseClick ( Button *button );
	static void CloseClick ( Button *button );
	static void BypassClick ( Button *button );

	static void StatusLightDraw ( Button *button, bool highlighted, bool clicked );
	static void ImageButtonDraw ( Button *button, bool highlighted, bool clicked );

	static bool ShouldDraw ( int pid );

protected:

	char ip [SIZE_VLOCATION_IP];				// Target IP

public:

	int TYPE;												// Security system type
	int status;												// 0 = static, 1 = working

public:

	SecurityBypass ();
	SecurityBypass ( int newTYPE );
	~SecurityBypass () override;

	void SetTYPE ( int newTYPE );

	void PauseCurrentBypass ();
	void ResumeCurrentBypass ();

	void EndCurrentBypass ();

	void SetTarget (UplinkObject *uo, const string &uos, int uoi ) override;
	void MoveTo ( int x, int y, int time_ms ) override;

	void Initialise () override;
	void Tick ( int n ) override;
	
	void CreateInterface () override;
	void RemoveInterface () override;
	void ShowInterface () override;

	bool IsInterfaceVisible () override;


};


#endif


