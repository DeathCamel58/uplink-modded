
#ifndef included_logdeleter_h
#define included_logdeleter_h


// ============================================================================

#include "interface/taskmanager/uplinktask.h"

class LogBank;

#define LOGDELETER_OFF		    0
#define LOGDELETER_INPROGRESS   1
#define LOGDELETER_REPLACING    2
#define LOGDELETER_FINISHED     3

// ============================================================================


class LogDeleter : public UplinkTask
{

protected:

	LogBank *source;	
	int sourceindex;

	int currentreplaceindex;			// Used by later versions to keep track of searches
	int currentreplaceprogress;			// "" ""

	int status;						// 0 = off, 1 = in progress, 2 = fnished
	int numticksrequired;
	int progress;					// 0.0 - numticksrequired
	
protected:

	static void BorderDraw    ( Button *button, bool highlighted, bool clicked );
	static void ProgressDraw  ( Button *button, bool highlighted, bool clicked );

	static void CloseClick    ( Button *button );
	static void BorderClick   ( Button *button );

public:

	LogDeleter ();
	~LogDeleter () override;

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
