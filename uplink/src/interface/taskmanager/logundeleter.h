
#ifndef included_logundeleter_h
#define included_logundeleter_h


// ============================================================================

#include "interface/taskmanager/uplinktask.h"

class LogBank;

#define LOGUNDELETER_OFF		    0
#define LOGUNDELETER_INPROGRESS     1
#define LOGUNDELETER_FINISHED		2

// ============================================================================


class LogUnDeleter : public UplinkTask
{

protected:

	LogBank *source;	
	int sourceindex;

	int status;						
	int numticksrequired;
	int progress;					// 0.0 - numticksrequired
	
protected:

	static void BorderDraw    ( Button *button, bool highlighted, bool clicked );
	static void ProgressDraw  ( Button *button, bool highlighted, bool clicked );

    /**
     * Closes the program
     * @param button
     * @note button is unused
     */
	static void CloseClick    ( Button *button );
	static void BorderClick   ( Button *button );

public:

	LogUnDeleter ();
	~LogUnDeleter () override;

	/**
	 * Sets the target of program
	 * @param uo LogBank object containing AccessLog to be un-deleted
	 * @param uos Name of button representing data
	 * @param uoi Index of log in LogBank
	 */
	void SetTarget (UplinkObject *uo, const string &uos, int uoi ) override;
	void MoveTo ( int x, int y, int time_ms ) override;				// Centred on top-right of title image

	void Initialise () override;
	void Tick ( int n ) override;
	
	void CreateInterface () override;
	void RemoveInterface () override;
	void ShowInterface () override;

	bool IsInterfaceVisible () override;

};



#endif
