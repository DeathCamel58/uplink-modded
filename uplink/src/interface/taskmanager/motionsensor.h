
/*

  Motion Sensor Task

	Runs on your Gateway in the background and keeps track of 
	people near your machine.

  */


#ifndef included_motionsensor_h
#define included_motionsensor_h


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

	/**
	 * Doesn't do anything, as the target is gotten by getting the player's gateway
	 * @param uo
	 * @param uos
	 * @param uoi
	 * @note uo, uos, and uoi are unused
	 */
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

