
#ifndef included_cypherscreeninterface_h
#define included_cypherscreeninterface_h

#include "world/computer/computerscreen/cypherscreen.h"

#include "interface/remoteinterface/remoteinterfacescreen.h"

#define CYPHER_WIDTH	30
#define CYPHER_HEIGHT	14

//! RemoteInterfaceScreen for "Enter elliptic-curve encryption cypher" screen
/**
 * Asks the user to input a great big number which can only really be done with software
 */
class CypherScreenInterface : public RemoteInterfaceScreen
{

protected:

    /**
     * The array of cypher elements
     */
	char cypher [CYPHER_WIDTH][CYPHER_HEIGHT];

	/**
	 * The array of cypher locks for each position
	 */
	bool cypherlock [CYPHER_WIDTH][CYPHER_HEIGHT];

	/**
	 * The number of locked cypher elements
	 */
	int numlocked;

	/**
	 * The time when the last render occurred
	 */
	int lastupdate;

protected:

    /**
     * Renders the cypher to the screen
     * @param button Button to render the cypher in
     * @param highlighted
     * @param clicked
     */
	static void DrawCypher ( Button *button, bool highlighted, bool clicked );

	/**
	 * Sets the current program's target to the cypher screen
	 * @param button Button that stores the cypher screen
	 */
	static void ClickCypher ( Button *button );

	/**
	 * Sends user to next page
	 * @param button
	 * @note button is unused
	 */
	static void BypassClick ( Button *button );

	/**
	 * Sends user to next page if cypher is solved
	 * @param button
	 * @note button is unused
	 */
	static void ProceedClick ( Button *button );

public:

	CypherScreenInterface ();
	~CypherScreenInterface () override;

	/**
	 * Gets the number of locked numbers
	 * @return The number of locked numbers
	 */
	int NumLocked () const;

    /**
     * Gets the number of unlocked numbers
     * @return The number of unlocked numbers
     */
	int NumUnLocked () const;

	/**
	 * Gets whether number in position is locked
	 * @param x X coordinate
	 * @param y Y coordinate
	 * @return true if locked
	 */
	bool IsLocked ( int x, int y );

	/**
	 * Locks the cypher at given position
	 * @param x X coordinate
	 * @param y Y coordinate
	 */
	void CypherLock ( int x, int y );

	/**
	 * Picks an unlocked number at random and locks it
	 */
	void CypherLock ();

	/**
	 * Changes all numbers to random ones
	 */
	void Cycle ();

	bool ReturnKeyPressed () override;

	void Create ( ComputerScreen *newcs ) override;
	void Remove () override;
	bool IsVisible () override;
	void Update () override;

	int  ScreenID () override;

	CypherScreen *GetComputerScreen ();				// Should override this with correct data type

};


#endif
