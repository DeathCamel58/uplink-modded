
#ifndef included_financeinterface_h
#define included_financeinterface_h


#include "interface/localinterface/localinterfacescreen.h"


//! Screen that shows finances
class FinanceInterface : public LocalInterfaceScreen  
{

protected:

	int oldbalance;

	/**
	 * The number of accounts the user has
	 */
	int previousnumaccounts;

    /**
     * The time the list was last updated
     */
	time_t lastupdate;

protected:

    /**
     * Closes the screen
     * @param button
     * @note button is unused
     */
	static void TitleClick ( Button *button );

	/**
	 * Renders the title row of the data
	 * @param button The top row of the data
	 * @param highlighted TODO: Document
	 * @param clicked  TODO: Document
	 */
	static void DrawAccountsTitle ( Button *button, bool highlighted, bool clicked );

	/**
	 * Gets the account information, connects to the bank IP
	 * @param button
	 */
	static void ClickAccountButton ( Button *button );

	/**
	 * Renders the account rows
	 * @param button The account row
	 * @param highlighted TODO: Document
	 * @param clicked TODO: Document
	 */
	static void DrawAccountButton ( Button *button, bool highlighted, bool clicked );

public:

    /**
     * Logs user into bank account
     * @param ip The bank IP
     * @param info The bank account number to log in to
     */
	static void AfterPhoneDialler (string &ip, string &info );

	FinanceInterface ();
	~FinanceInterface () override;

	void Create () override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

};

#endif 
