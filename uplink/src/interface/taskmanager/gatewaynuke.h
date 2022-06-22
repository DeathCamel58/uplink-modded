
#ifndef included_gatewaynuke_h
#define included_gatewaynuke_h

// ============================================================================

#include "interface/taskmanager/uplinktask.h"

#define GATEWAYNUKE_STATUS_NONE			0
#define GATEWAYNUKE_STATUS_START		1
#define GATEWAYNUKE_STATUS_AREYOUSURE	2
#define GATEWAYNUKE_STATUS_FAILED		3

// ============================================================================

//! Gateway nuke software UplinkTask
/**
 * Checks if the player has the gateway nuke hardware, then takes down the gateway, destroying all evidence
 */
class GatewayNuke : public UplinkTask
{

protected:

    /**
     * The status of the gateway nuke
     */
	int status;

protected:

	static void TitleDraw ( Button *button, bool highlighted, bool clicked );

	/**
	 * Changes the title to a new message or attempts to nuke gateway
	 * @param button Button clicked
	 */
	static void YesClick ( Button *button );

	/**
	 * Deletes the nuke gateway task
	 * @param button Button clicked
	 */
	static void NoClick ( Button *button );

	/**
	 * Nukes the gateway
	 */
	static void NukeGateway ();

public:

	GatewayNuke ();
	~GatewayNuke () override;

	/**
	 * Doesn't do anything, as the target is gotten by getting the player's gateway
	 * @param uo
	 * @param uos
	 * @param uoi
	 * @note uo, uos, and uoi are unused
	 */
	void SetTarget (UplinkObject *uo, const string &uos, int uoi ) override;
	void MoveTo ( int x, int y, int time_ms ) override;

	void Initialise () override;
	void Tick ( int n ) override;
	
	void CreateInterface () override;
	void RemoveInterface () override;
	void ShowInterface () override;

	bool IsInterfaceVisible () override;

};



#endif

