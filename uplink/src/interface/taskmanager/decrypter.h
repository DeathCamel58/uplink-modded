
#ifndef _included_decrypter_h
#define _included_decrypter_h


// ============================================================================

#include "interface/taskmanager/uplinktask.h"

class DataBank;

#define DECRYPTER_OFF		    0
#define DECRYPTER_INPROGRESS    1
#define DECRYPTER_FINISHED      2

// ============================================================================


class Decrypter : public UplinkTask
{

protected:

	DataBank *source;	
	int sourceindex;

	int status;						
    bool remotefile;

	int numticksrequired;
	int progress;					// 0.0 - numticksrequired

protected:

	static void BorderDraw    ( Button *button, bool highlighted, bool clicked );
	static void ProgressDraw  ( Button *button, bool highlighted, bool clicked );

	static void CloseClick    ( Button *button );
	static void BorderClick   ( Button *button );

public:

	Decrypter ();
	~Decrypter () override;
	
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
