
#ifndef included_changegatewayscreeninterface_h
#define included_changegatewayscreeninterface_h

#include "interface/remoteinterface/remoteinterfacescreen.h"

class GenericScreen;

//! RemoteInterfaceScreen for the uplink internal services' "Exchange gateway" screen
/**
 * Allows the user to exchange his gateway for a new one
 * @note This screen is used after a gateway has been selected in the list of available gateways, and the user has clicked "Upgrade"
 */
class ChangeGatewayScreenInterface : public RemoteInterfaceScreen
{

protected:

    /**
     * Renders the gateway Button
     * @param button Button
     * @param highlighted Callback for highlight
     * @param clicked
     * @note clicked is unused
     */
	static void GatewayButtonDraw ( Button *button, bool highlighted, bool clicked );

	/**
	 * Renders the info text
	 * @param button Button to render
	 * @param highlighted
	 * @param clicked
	 * @note hightlighted and clicked are unused
	 */
	static void InfoTextDraw ( Button *button, bool highlighted, bool clicked );

	/**
	 * Callback for when gateway button is clicked
	 * @param button
	 */
	static void GatewayButtonClick ( Button *button );

	/**
	 * Returns to previous screen
	 * @param button
	 * @note button is unused
	 */
	static void CloseClick ( Button *button );

	/**
	 * Callback for when "are you sure" "Upgrade" button is clicked
	 * @param button
	 * @note button is unused
	 */
	static void BuyClick ( Button *button );

	/**
	 * Callback for when "Click to go ahead with the exchange" is clicked
	 * @param button
	 * @note button is unused
	 */
	static void BuyConfirmClick ( Button *button );

	/**
	 * Callback for when "Cancel the exchange" is clicked
	 * @param button
	 * @note button is unused
	 */
	static void BuyCancelClick ( Button *button );

	/**
	 * Renders the image and border for the picture of the gateway
	 * @param button Button to render
	 * @param highlighted Callback on highlight
	 * @param clicked Callback on click
	 */
    static void GatewayPictureDraw ( Button *button, bool highlighted, bool clicked );

    /**
     * Changes the scroll index offset and re-renders the ScrollBox
     * @param scrollname
     * @param newIndex index to use for the first element
     * @note scrollname is unused
     */
    static void ScrollChange (const string &scrollname, int newIndex );

protected:

	static int baseOffset;
    int currentselect;

public:

	ChangeGatewayScreenInterface ();
	~ChangeGatewayScreenInterface () override;

	/**
	 * Updates gateway description and image to the one in the index
	 * @param index index of the gateway
	 */
	void ShowGateway ( int index );

	/**
	 * Gets the price of the gateway upgrade
	 * @param index index of the gateway
	 * @return The price of the gateway
	 * @note Takes into account Part exchange value of your gateway
	 */
	static int GetGatewayPrice ( int index );

	void Create () override;
	void Create ( ComputerScreen *cs ) override;

	/**
	 * Creates the "are you sure" screen
	 */
	void CreateAreYouSure ();

	void Remove () override;
	void Update () override;
	bool IsVisible () override;

	bool ReturnKeyPressed () override;
    bool EscapeKeyPressed () override;

	int ScreenID () override;

	GenericScreen *GetComputerScreen ();

};


#endif

