// MemoryInterface.h: interface for the MemoryInterface class.
//
//////////////////////////////////////////////////////////////////////

#ifndef included_memoryinterface_h
#define included_memoryinterface_h


#include "interface/localinterface/localinterfacescreen.h"


//! Screen that shows memory usage
class MemoryInterface : public LocalInterfaceScreen  
{

protected:

    /**
     * Number of files in memory
     */
	static int previousnumfiles;

	/**
	 * The offset for the scroll box
	 */
	static int baseoffset;

	/**
	 * Currently highlighted program
	 */
	static int currentprogramindex;					// Currently highlighted program

	/**
	 * Which memory block should be highlighted
	 */
    static int specialHighlight;

protected:

    /**
     * Renders a memory block
     * @param button Memory block to render
     * @param highlighted Is the memory block highlighted
     * @param clicked
	 * @note clicked is unused
     */
	static void MemoryBlockDraw      ( Button *button, bool highlighted, bool clicked );

	/**
	 * Highlights a memory block
	 * @param button Memory block to highlight
	 */
	static void MemoryBlockHighlight ( Button *button );

	/**
	 * Sets clicked element as program target
	 * @param button Button that was clicked on
	 */
	static void MemoryBlockClick	 ( Button *button );

    /**
     * Closes the screen
     * @param button
     * @note button is unused
     */
	static void TitleClick ( Button *button );

	/**
	 * Forces a re-render of the memory interface
	 * @param scrollname
	 * @param newValue
	 */
    static void ScrollChange (const string &scrollname, int newValue );

public:

    /**
     * Forces a re-render of the memory interface
     * @note This is currently only used by the Defrag tool
     */
    static void ForceUpdateAll ();

    /**
     * Highlight memory block at index
     * @param memoryIndex Memory block index
     */
    static void SpecialHighlight ( int memoryIndex );

	void Create () override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

};

#endif 
