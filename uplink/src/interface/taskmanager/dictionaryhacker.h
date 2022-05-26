
#ifndef included_dictionaryhacker_h
#define included_dictionaryhacker_h


#include "vanbakel.h"

#include "interface/taskmanager/uplinktask.h"


#define DICTIONARYHACKER_WAITING		0
#define DICTIONARYHACKER_INPROGRESS		1


class DictionaryHacker : public UplinkTask  
{

protected:

	int status;
	int currentindex;
	string username;
	string password;

	float numticksrequired;
	int progress;

protected:

	static void DictionaryHackerDraw ( Button *button, bool highlighted, bool clicked );	
	static void DictionaryHackerClick ( Button *button );

	static void CloseClick ( Button *button );

public:

	DictionaryHacker();

	~DictionaryHacker() override;

	void SetTarget (UplinkObject *uo, const string &uos, int uoi ) override;

	void Initialise () override;     // Called at creation time, neccisary
	void Tick ( int n ) override;           // n ticks occur
	
	void CreateInterface () override;
	void RemoveInterface () override;
	void ShowInterface () override;

	bool IsInterfaceVisible () override;

};

#endif

