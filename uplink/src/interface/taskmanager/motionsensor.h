
/*

  Motion Sensor Task

	Runs on your Gateway in the background and keeps track of 
	people near your machine.

  */


#ifndef _included_motionsensor_h
#define _included_motionsensor_h


#include "interface/taskmanager/uplinktask.h"



class MotionSensor : public UplinkTask
{

protected:

	static void SensorDraw ( Button *button, bool highlighted, bool clicked );
	static void SensorClose ( Button *button );

protected:

	int oldnumpeople;

public:

	MotionSensor ();
	~MotionSensor () override;

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

