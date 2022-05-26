
/*
	
  Gatwaye Nuke software driver

	Checks you have gate-nuke hardware,
	then takes down your gateway, destroying
	all evidence

  */

#ifndef included_gatewaynuke_h
#define included_gatewaynuke_h

// ============================================================================

#include "interface/taskmanager/uplinktask.h"

#define GATEWAYNUKE_STATUS_NONE			0
#define GATEWAYNUKE_STATUS_START		1
#define GATEWAYNUKE_STATUS_AREYOUSURE	2
#define GATEWAYNUKE_STATUS_FAILED		3

// ============================================================================


class GatewayNuke : public UplinkTask
{

protected:

	int status;

protected:

	static void TitleDraw ( Button *button, bool highlighted, bool clicked );

	static void YesClick ( Button *button );
	static void NoClick ( Button *button );

	static void NukeGateway ();												// Does the dirty work

public:

	GatewayNuke ();
	~GatewayNuke () override;

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

