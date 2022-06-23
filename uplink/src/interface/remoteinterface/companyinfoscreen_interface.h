
#ifndef includedcompanyinfoscreeninterface_h
#define includedcompanyinfoscreeninterface_h

#include "interface/remoteinterface/remoteinterfacescreen.h"

class GenericScreen;

//! RemoteInterfaceScreen for company's "Company data" screen
/**
 * Shows basic data on a company including size, growth rate, key personnel etc
 * @note This is found by clicking "About us" in a bank
 */
class CompanyInfoScreenInterface : public RemoteInterfaceScreen 
{

protected:


    /**
     * Adds the managing director link to the user's favorites
     * @param button Button clicked
     */
	static void MdAddClick ( Button *button );

	/**
	 * Adds the admin link to the user's favorites
	 * @param button Button clicked
	 */
	static void AdminAddClick ( Button *button );

	/**
	 * Returns to the previous screen
	 * @param button
	 */
    static void BackClick ( Button *button );

public:

	CompanyInfoScreenInterface ();
	~CompanyInfoScreenInterface () override;

    bool EscapeKeyPressed () override;

	void Create () override;
	void Create ( ComputerScreen *newcs ) override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

	GenericScreen *GetComputerScreen ();

};


#endif

