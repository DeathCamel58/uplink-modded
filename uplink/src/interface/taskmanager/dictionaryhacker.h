
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

    /**
     * Renders the background of the program
     * @param button Button for program
     * @param highlighted TODO: Document
     * @param clicked TODO: Document
     */
	static void DictionaryHackerDraw ( Button *button, bool highlighted, bool clicked );

	/**
	 * Sets the program selected to this one
	 * @param button Button for program
	 */
	static void DictionaryHackerClick ( Button *button );

    /**
     * Closes the program
     * @param button
     * @note button is unused
     */
	static void CloseClick ( Button *button );

public:

	DictionaryHacker();

	~DictionaryHacker() override;

	/**
	 * Sets the target of program
	 * @param uo Target PasswordScreen or UserIDScreen object
	 * @param uos name of button with password in
	 * @param uoi
	 * @note uoi is unused
	 */
	void SetTarget (UplinkObject *uo, const string &uos, int uoi ) override;

	void Initialise () override;     // Called at creation time, neccisary
	void Tick ( int n ) override;           // n ticks occur
	
	void CreateInterface () override;
	void RemoveInterface () override;
	void ShowInterface () override;

	bool IsInterfaceVisible () override;

};

#endif

