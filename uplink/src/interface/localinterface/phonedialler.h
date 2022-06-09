
#ifndef included_dialler_h
#define included_dialler_h


#include "interface/localinterface/localinterfacescreen.h"


//! Screen that shows the dialer
/**
 * Dialer screen is used to visually show the IP we're in the process of connecting to
 */
class PhoneDialler : public LocalInterfaceScreen
{

protected:

    /**
     * Stores the names of the numbered buttons
     */
	static char buttonnames [][9];

	/**
	 * The IP/Phone number to dial
	 */
	string ip;

	/**
	 * TODO: Document
	 */
	string infosup;

	/**
	 * The screen to switch to after the dialer finishes
	 */
	int nextscene;

    /**
     * The time the dialer was last updated
     */
	int lastupdate;

	/**
	 * The current number the dialer is dialing
	 */
	int curnumber;

	/**
	 * Re-renders the dialer
	 */
	void UpdateDisplay ();

public:

    PhoneDialler ();
    ~PhoneDialler () override;

    /**
     * Render the dialer at coordinates
     * @param x X coordinate
     * @param y Y coordinate
     */
	void Create ( int x, int y );
	void Remove () override;

	/**
	 * Forces a re-render of the dialer if the last update time was 110ms ago
	 * @return true if a re-render occurred
	 */
	bool UpdateSpecial ();
	bool IsVisible () override;

	/**
	 * Starts the dialer at coordinates
	 * @param x X coordinate
	 * @param y Y coordinate
	 * @param number The IP/Phone number to dial
	 * @param scenario TODO: Document
	 * @param info TODO: Document
	 */
	void DialNumber (int x, int y, const string& number, int scenario, const string& info = "");

};

#endif

