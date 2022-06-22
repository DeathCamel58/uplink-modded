
#ifndef included_filedeleter_h
#define included_filedeleter_h


// ============================================================================

#include "interface/taskmanager/uplinktask.h"

class DataBank;

#define FILEDELETER_OFF		    0
#define FILEDELETER_INPROGRESS  1
#define FILEDELETER_FINISHED    2

// ============================================================================


class FileDeleter : public UplinkTask
{

protected:

	DataBank *source;	
	int sourceindex;

	int status;						// 0 = off, 1 = in progress, 2 = finished
    bool remotefile;

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

	FileDeleter ();
	~FileDeleter () override;

	/**
	 * Sets the target of program
	 * @param uo DataBank object containing data to be deleted
	 * @param uos Name of button representing data
	 * @param uoi Index of data to be deleted
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
