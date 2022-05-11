
#ifndef _included_filecopier_h
#define _included_filecopier_h


// ============================================================================

#include "eclipse.h"

#include "interface/taskmanager/uplinktask.h"

class DataBank;

#define FILECOPIER_NOTDOWNLOADING    0
#define FILECOPIER_INPROGRESS        1
#define FILECOPIER_WAITINGFORTARGET  2
#define FILECOPIER_FINISHED          3

// ============================================================================


class FileCopier : public UplinkTask
{

protected:

	DataBank *source;	
	int sourceindex;

	int downloading;				// 0 = not, 1 = in progress, 2 = download complete, 3 = finished
    bool remotefile;                // True if the target is coming in remotely
	
	int numticksrequired;
	int progress;					// 0.0 - numticksrequired

protected:

	static void BorderDraw    ( Button *button, bool highlighted, bool clicked );
	static void ProgressDraw  ( Button *button, bool highlighted, bool clicked );

	static void CloseClick    ( Button *button );
	static void BorderClick   ( Button *button );

public:

	FileCopier ();
	~FileCopier () override;

    int GetState () const;                    // Returns variable 'downloading'

	void SetTarget (UplinkObject *uo, const string &uos, int uoi ) override;
	void MoveTo ( int x, int y, int time_ms ) override;				// Centred on top-right of title image

	void Initialise () override;     // Called at creation time, neccisary
	void Tick ( int n ) override;           // n ticks occur
	
	void CreateInterface () override;
	void RemoveInterface () override;
	void ShowInterface () override;

	bool IsInterfaceVisible () override;

};



#endif
