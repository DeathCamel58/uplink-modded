
/*
  LAN Scan software tool

	Run on a LAN viewer
	Does a quick scan for all available systems
	Will show up Terminals, Mail servers etc

  */

#ifndef included_lanscan_h
#define included_lanscan_h

#include "world/vlocation.h"
#include "interface/taskmanager/uplinktask.h"

class LanComputer;

#define LANSCAN_UNUSED			0
#define LANSCAN_SCANNING		1
#define LANSCAN_FINISHED		2


// ============================================================================


class LanScan : public UplinkTask
{

protected:

	int status;
	int progress;
	int numticksrequired;

	char ip [SIZE_VLOCATION_IP];

protected:

	static void ProgressDraw ( Button *button, bool highlighted, bool clicked );
	static void BorderDraw   ( Button *button, bool highlighted, bool clicked );

	static void GoClick		 ( Button *button );

    /**
     * Closes the program
     * @param button
     * @note button is unused
     */
	static void CloseClick   ( Button *button );

public:

	LanScan();
	~LanScan() override;

	/**
	 * Don't do anything. The target is gotten from the current remote host
	 * @param uo
	 * @param uos
	 * @param uoi
	 * @note uo, uos, and uoi are unused
	 */
	void SetTarget (UplinkObject *uo, const string &uos, int uoi ) override;
	void MoveTo ( int x, int y, int time_ms ) override;							// Origin top left

	void Initialise () override;
	void Tick ( int n ) override;
	
	void CreateInterface () override;
	void RemoveInterface () override;
	void ShowInterface () override;

	bool IsInterfaceVisible () override;

};

#endif
