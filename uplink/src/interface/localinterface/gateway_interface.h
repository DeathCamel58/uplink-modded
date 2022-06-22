
#ifndef included_gatewayinterface_h
#define included_gatewayinterface_h


#include "localinterfacescreen.h"

//! LocalInterfaceScreen that shows the current gateway and upgrades
class GatewayInterface : public LocalInterfaceScreen  
{

protected:


    /**
     * Closes the screen
     * @param button
     * @note button is unused
     */
	static void CloseClick ( Button *button );

	/**
	 * Renders the gateway's background
	 * @param button The background button
	 * @param highlighted TODO: Document
	 * @param clicked TODO: Document
	 */
	static void DrawGatewayBackground ( Button *button, bool highlighted, bool clicked );

	/**
	 * Renders the picture of the gateway
	 * @param button The picture button
	 * @param highlighted TODO: Document
	 * @param clicked TODO: Document
	 */
	static void DrawGatewayPicture    ( Button *button, bool highlighted, bool clicked );

	/**
	 * Renders the window title
	 * @param button The window title button
	 * @param highlighted TODO: Document
	 * @param clicked TODO: Document
	 */
	static void DrawMainTitle ( Button *button, bool highlighted, bool clicked );

	/**
	 * Renders a populated item box
	 * @param button The populated item button
	 * @param highlighted TODO: Document
	 * @param clicked TODO: Document
	 */
	static void DrawPopulatedItem ( Button *button, bool highlighted, bool clicked );

	/**
	 * Renders an empty item box
	 * @param button The empty item button
	 * @param highlighted TODO: Document
	 * @param clicked TODO: Document
	 */
	static void DrawUnPopulatedItem ( Button *button, bool highlighted, bool clicked );

public:

	void Create () override;
	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	int ScreenID () override;

};

#endif 
