
#ifndef included_eventscheduler_h
#define included_eventscheduler_h


#include "tosser.h"

#include "app/uplinkobject.h"

#include "world/scheduler/uplinkevent.h"

//!Event Scheduler Class
/**
 * Responsible for keeping track of all future scheduled events, and running those events at the right time.
 * It is not designed for high accuracy scheduling - events are run within around 10 seconds of their target time.
 */
class EventScheduler : public UplinkObject
{

public:

    /**
     * LList of all events
     */
	LList <UplinkEvent *> events;

public:

	EventScheduler ();
	~EventScheduler () override;

	/**
	 * Inserts an Event into the list of events
	 * @param event Event to add
	 */
	void ScheduleEvent ( UplinkEvent *event );

	/**
	 * Schedules a warning of an upcoming Event
	 * @param event Event to schedule
	 * @param date Date to give the warning at
	 */
	void ScheduleWarning ( UplinkEvent *event, Date *date );

	/**
	 * Gets the Date of the next event
	 * @return The Date of the next event
	 */
	Date *GetDateOfNextEvent ();

	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	void Update () override;

	string GetID () override;

};


#endif
	
