
#ifndef included_accountscreen_h
#define included_accountscreen_h

#include "interface/remoteinterface/remoteinterfacescreen.h"

class ComputerScreen;
class GenericScreen;
class BankAccount;

//! RemoteInterfaceScreen for the bank account's "View account" screen
class AccountScreenInterface : public RemoteInterfaceScreen
{

protected:

	BankAccount *account;
	time_t lastupdate;

protected:

    /**
     * Returns to the previous screen
     * @param button
     * @note button is unused
     */
	static void CloseClick ( Button *button );

public:

	AccountScreenInterface ();
	~AccountScreenInterface () override;

    bool EscapeKeyPressed () override;

	void Create () override;
	void Create ( ComputerScreen *cs ) override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

	GenericScreen *GetComputerScreen ();				// Should override this with correct data type

};


#endif

