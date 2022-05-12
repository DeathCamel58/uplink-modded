
#ifdef WIN32
#include <windows.h>
#endif

#include <strstream>

#include <GL/gl.h>

#include <GL/glu.h>
#include <sstream>


#include "app/app.h"
#include "app/globals.h"
#include "app/miscutils.h"
#include "app/opengl_interface.h"

#include "game/game.h"
#include "game/data/data.h"

#include "world/world.h"
#include "world/player.h"
#include "world/message.h"
#include "world/computer/gatewaydef.h"
#include "world/computer/computerscreen/genericscreen.h"
#include "world/scheduler/changegatewayevent.h"
#include "world/generator/numbergenerator.h"

#include "interface/interface.h"
#include "interface/scrollbox.h"
#include "interface/remoteinterface/remoteinterface.h"
#include "interface/remoteinterface/changegatewayscreen_interface.h"

#include "mmgr.h"


int ChangeGatewayScreenInterface::baseOffset = 0;

ChangeGatewayScreenInterface::ChangeGatewayScreenInterface ()
{

	currentselect = 0;

}

ChangeGatewayScreenInterface::~ChangeGatewayScreenInterface ()
= default;

void ChangeGatewayScreenInterface::GatewayButtonDraw ( Button *button, bool highlighted, bool clicked )
{

	UplinkAssert (button)

	char unused [64];
	int index;
    istringstream stream(button->name);
    stream >> unused >> index;
    index += baseOffset;

	GatewayDef *gd = nullptr;
	if ( game->GetWorld ()->gatewaydefs.ValidIndex ( index ) )
		gd = game->GetWorld ()->gatewaydefs.GetData ( index );
	if ( !gd ) {
		clear_draw ( button->x, button->y, button->width, button->height );
		return;
	}

	auto *thisint = (ChangeGatewayScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ();
	UplinkAssert (thisint)

	// Fill in the background

//	if ( index == thisint->currentselect ) {
//
//		glBegin ( GL_QUADS );
//			glColor4f ( 0.6f, 0.6f, 0.6f, ALPHA );		glVertex2i ( button->x, button->y );
//			glColor4f ( 0.4f, 0.4f, 0.4f, ALPHA );		glVertex2i ( button->x + button->width, button->y );
//			glColor4f ( 0.6f, 0.6f, 0.6f, ALPHA );		glVertex2i ( button->x + button->width, button->y + button->height );
//			glColor4f ( 0.4f, 0.4f, 0.4f, ALPHA );		glVertex2i ( button->x, button->y + button->height );
//		glEnd ();
//
//  	glColor3ub ( 81, 138, 215 );
//	    border_draw ( button );
//
//	}
//	else {

		if ( index % 2 == 0 ) {

			glBegin ( GL_QUADS );
				SetColour ( "DarkPanelA" );     glVertex2i ( button->x, button->y + button->height );
				SetColour ( "DarkPanelB" );     glVertex2i ( button->x, button->y );
				SetColour ( "DarkPanelA" );     glVertex2i ( button->x + button->width, button->y );
				SetColour ( "DarkPanelB" );     glVertex2i ( button->x + button->width, button->y + button->height );
			glEnd ();

		}
		else {

			glBegin ( GL_QUADS );
				SetColour ( "DarkPanelB" );     glVertex2i ( button->x, button->y + button->height );
				SetColour ( "DarkPanelA" );     glVertex2i ( button->x, button->y );
				SetColour ( "DarkPanelB" );     glVertex2i ( button->x + button->width, button->y );
				SetColour ( "DarkPanelA" );     glVertex2i ( button->x + button->width, button->y + button->height );
			glEnd ();

		}

//	}

	if ( highlighted ) {

		SetColour ( "PanelHighlightBorder" );
		border_draw ( button );

	}
    else if ( index == thisint->currentselect ) {

        SetColour ( "PanelBorder" );
        border_draw ( button );

    }

	// Draw the title and cost

	int price = thisint->GetGatewayPrice(index);
	bool hasmoney = ( game->GetWorld ()->GetPlayer ()->GetBalance () >= price );

	if ( hasmoney ) 	SetColour ( "DefaultText" );
	else				SetColour ( "DimmedText" );

	string cost = to_string(price) + "c";

	GciDrawText ( button->x + 5, button->y + 10, gd->name );
	GciDrawText ( (button->x + button->width - 10 ) - GciTextWidth ( cost ), button->y + 10, cost );

}

void ChangeGatewayScreenInterface::InfoTextDraw ( Button *button, bool highlighted, bool clicked )
{

	textbutton_draw ( button, true, true );

}

void ChangeGatewayScreenInterface::GatewayButtonClick ( Button *button )
{

	UplinkAssert (button)

	char unused [64];
	int index;
    istringstream stream(button->name);
    stream >> unused >> index;
    index += baseOffset;

	auto *thisint = (ChangeGatewayScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ();
	UplinkAssert (thisint)

	thisint->ShowGateway ( index );

}

void ChangeGatewayScreenInterface::CloseClick ( Button *button )
{

	auto *gs= (GenericScreen *) game->GetInterface ()->GetRemoteInterface ()->GetComputerScreen ();
	UplinkAssert (gs)

	if ( gs->nextpage != -1 )
		game->GetInterface ()->GetRemoteInterface ()->RunScreen ( gs->nextpage, gs->GetComputer () );

}

void ChangeGatewayScreenInterface::BuyClick ( Button *button )
{

	auto *thisint = (ChangeGatewayScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ();
	UplinkAssert (thisint)

	if ( !game->GetWorld ()->gatewaydefs.ValidIndex ( thisint->currentselect ) || !game->GetWorld ()->gatewaydefs.GetData ( thisint->currentselect ) )
		return;

	int price = thisint->GetGatewayPrice(thisint->currentselect);
	bool hasmoney = ( game->GetWorld ()->GetPlayer ()->GetBalance () >= price );

	if ( hasmoney ) {

#ifdef DEMOGAME

        if ( thisint->currentselect > DEMO_MAXGATEWAY ) {

            create_msgbox ( "Sorry", "Not available in demo!\n"
                                     "Visit www.introversion.co.uk to buy the full game." );

            return;

        }

#endif


		// Create the message for the player to read
		// Remove the current interface first

		thisint->Remove ();
		thisint->CreateAreYouSure ();

	}

}

void ChangeGatewayScreenInterface::BuyConfirmClick ( Button *button )
{

	auto *thisint = (ChangeGatewayScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ();
	UplinkAssert (thisint)

	if ( !game->GetWorld ()->gatewaydefs.ValidIndex ( thisint->currentselect ) || !game->GetWorld ()->gatewaydefs.GetData ( thisint->currentselect ) )
		return;

	// Take the money and run
	// Take the money and run
	// Here i'm alive
	// Everything all of the time

	int price = thisint->GetGatewayPrice(thisint->currentselect);
	game->GetWorld ()->GetPlayer ()->ChangeBalance ( price * -1, "Uplink Corporation gateway exchange" );


	// Schedule a Change Gateway Event

	Date rundate;
	rundate.SetDate ( &(game->GetWorld ()->date) );
	rundate.AdvanceMinute ( TIME_TOCHANGEGATEWAY );
	rundate.AdvanceMinute ( NumberGenerator::RandomNumber (60) );

	auto *cge = new ChangeGatewayEvent ();
	cge->SetRunDate ( &rundate );
	cge->SetNewGateway ( game->GetWorld ()->gatewaydefs.GetData ( thisint->currentselect ) );
	game->GetWorld ()->scheduler.ScheduleEvent ( cge );

	// Send a confirmation email to the player

	auto *msg = new Message ();
	msg->SetTo ( "PLAYER" );
	msg->SetFrom ( "Uplink Corporation" );
	msg->SetSubject ( "Gateway Upgrade" );
	msg->SetBody ( "Uplink Corporation is now preparing your new gateway computer.\n"
				   "This will take around 24 hours, at which point we will notify you to "
				   "disconnect and reconnect.  This will allow us to re-route you to your "
				   "new gateway.\n"
				   "[END]" );
	msg->Send ();


	// Return to the upgrade screen

	thisint->Remove ();
	thisint->Create();

}

void ChangeGatewayScreenInterface::BuyCancelClick ( Button *button )
{

	auto *thisint = (ChangeGatewayScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ();
	UplinkAssert (thisint)

	thisint->Remove ();
	thisint->Create ();

}


void ChangeGatewayScreenInterface::GatewayPictureDraw ( Button *button, bool highlighted, bool clicked )
{

    imagebutton_draw ( button, highlighted, clicked );

	auto *thisint = (ChangeGatewayScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ();
	UplinkAssert (thisint)

    glColor3f ( 1.0f, 1.0f, 1.0f );
    border_draw ( button );

#ifdef DEMOGAME
    if ( thisint->currentselect > DEMO_MAXGATEWAY ) {
        glColor3f ( 1.0f, 1.0f, 1.0f );
        GciDrawText ( button->x + 50, button->y + button->height/2, "NOT AVAILABLE IN DEMO" );
    }
#endif

}

void ChangeGatewayScreenInterface::ScrollChange (const string &scrollname, int newIndex )
{

    baseOffset = newIndex;

    int numItems = 10;

	for ( int i = 0; i < numItems; ++i ) {

		string bname = "changegateway_title " + to_string(i);
		EclDirtyButton ( bname );

	}

}

void ChangeGatewayScreenInterface::ShowGateway ( int index )
{

	currentselect = index;

	// Dirty the select buttons

    int numItems = 10;

	for ( int i = 0; i < numItems; ++i ) {

		string bname = "changegateway_title " + to_string(i);
		EclDirtyButton ( bname );

	}


	// Get the new gateway

	GatewayDef *gd = nullptr;
	if ( game->GetWorld ()->gatewaydefs.ValidIndex ( currentselect ) )
		gd = game->GetWorld ()->gatewaydefs.GetData ( currentselect );

    if ( gd ) {

	    string newstats1 = "Max CPUs : " + to_string(gd->maxcpus) + "\nMax Memory : " + to_string(gd->maxmemory * 8) + "Gq";
	    string newstats2 = "Max Security : " + to_string(gd->maxsecurity) + "\nBandwidth : " + to_string(gd->bandwidth) + "Gqs";
	    EclRegisterCaptionChange ( "changegateway_stats1", newstats1 );
	    EclRegisterCaptionChange ( "changegateway_stats2", newstats2 );

	    EclRegisterCaptionChange ( "changegateway_text", gd->description, 1500 );


	    // Show the new picture

	    delete (EclGetButton ("changegateway_picture")->image_standard);
	    EclGetButton ("changegateway_picture")->image_standard = nullptr;
	    //char imagename [64];
	    //UplinkSnprintf ( imagename, sizeof ( imagename ), "gateway/gateway_t%d.tif", index );
	    button_assignbitmap ( "changegateway_picture", gd->thumbnail );
    //	EclGetButton ("changegateway_picture")->image_standard->Scale ( 240, 140 );
	    EclRegisterButtonCallbacks ( "changegateway_picture", GatewayPictureDraw, nullptr, nullptr, nullptr );

    }

}

int ChangeGatewayScreenInterface::GetGatewayPrice ( int index )
{

	GatewayDef *oldgateway = game->GetWorld ()->GetPlayer ()->gateway.curgatewaydef;
	UplinkAssert (oldgateway)

	GatewayDef *newgateway = nullptr;
	if ( game->GetWorld ()->gatewaydefs.ValidIndex ( index ) )
		newgateway = game->GetWorld ()->gatewaydefs.GetData ( index );

	if ( newgateway )
		return newgateway->cost - (int)(oldgateway->cost * GATEWAY_PARTEXCHANGEVALUE );
	else
		return 0;

}

void ChangeGatewayScreenInterface::Create ()
{

	UplinkAssert (cs)
	Create ( cs );

}

void ChangeGatewayScreenInterface::Create ( ComputerScreen *newcs )
{

	cs = newcs;

	if ( !IsVisible () ) {

		EclRegisterButton ( 80, 60, 350, 25, GetComputerScreen ()->maintitle, "", "changegateway_maintitle" );
		EclRegisterButtonCallbacks ( "changegateway_maintitle", DrawMainTitle, nullptr, nullptr, nullptr );
		EclRegisterButton ( 80, 80, 350, 20, GetComputerScreen ()->subtitle, "", "changegateway_subtitle" );
		EclRegisterButtonCallbacks ( "changegateway_subtitle", DrawSubTitle, nullptr, nullptr, nullptr );

		EclRegisterButton ( 20, 120, 220, 15, "Gateway Name and Cost", " ", "changegateway_title" );

		// One button for each gateway

        int numRows = 10;

		for ( int i = 0; i < numRows; ++i ) {

			GatewayDef *gd = nullptr;
			if ( game->GetWorld ()->gatewaydefs.ValidIndex ( i ) )
				gd = game->GetWorld ()->gatewaydefs.GetData ( i );

			string bname = "changegateway_title " + to_string(i);

			if ( gd )
				EclRegisterButton ( 20, 140 + i * 20, 220, 15, gd->name, "View this gateway", bname );
			else
				EclRegisterButton ( 20, 140 + i * 20, 220, 15, "", "View this gateway", bname );
			
			EclRegisterButtonCallbacks ( bname, GatewayButtonDraw, GatewayButtonClick, button_click, button_highlight );

		}

        // Scroll bar baby yeah

        int numItems = game->GetWorld ()->gatewaydefs.Size ();
        if ( numItems > numRows )
            ScrollBox::CreateScrollBox( "changegatewayscroll", 5, 140, 15, numRows * 20, numItems, 10, baseOffset, ScrollChange );

		// Picture of the gateway

		EclRegisterButton ( 245, 120, 200, 140, " ", " ", "changegateway_picture" );
		button_assignbitmap ( "changegateway_picture", "gateway/gateway_t0.tif" );
//		EclGetButton ("changegateway_picture")->image_standard->Scale ( 200, 140 );
		EclRegisterButtonCallbacks ( "changegateway_picture", GatewayPictureDraw, nullptr, nullptr, nullptr );

		// Stats

		EclRegisterButton ( 245, 270, 110, 30, "accessing...", " ", "changegateway_stats1" );
		EclRegisterButton ( 355, 270, 110, 30, "accessing...", " ", "changegateway_stats2" );

		EclRegisterButtonCallbacks ( "changegateway_stats1", textbutton_draw, nullptr, nullptr, nullptr );
		EclRegisterButtonCallbacks ( "changegateway_stats2", textbutton_draw, nullptr, nullptr, nullptr );

		EclRegisterButton ( 245, 315, 200, 70, "accessing...", " ", "changegateway_text" );
		EclRegisterButtonCallbacks ( "changegateway_text", InfoTextDraw, nullptr, nullptr, nullptr );

		// Control buttons

		int lowY = 140 + numRows * 20;

		EclRegisterButton ( 20, lowY + 10, 220, 15, "Upgrade", "Click to change your gateway for this one", "changegateway_buy" );
		EclRegisterButtonCallback ( "changegateway_buy", BuyClick );

		EclRegisterButton ( 20, lowY + 30, 220, 15, "Close", "Return to the main menu", "changegateway_close" );
		EclRegisterButtonCallback ( "changegateway_close", CloseClick );


		// Select your gateway

        GatewayDef *gd = game->GetWorld ()->GetPlayer ()->gateway.curgatewaydef;
		UplinkAssert( gd )

		int sizegatewaydefs = game->GetWorld ()->gatewaydefs.Size ();
		for ( int ii = 0; ii < sizegatewaydefs; ii++ )
			if ( game->GetWorld ()->gatewaydefs.ValidIndex ( ii ) )
				if ( strcmp ( game->GetWorld ()->gatewaydefs.GetData ( ii )->name, gd->name ) == 0 ) {
					ShowGateway ( ii );
					break;
				}

	}

}

void ChangeGatewayScreenInterface::CreateAreYouSure ()
{

	if ( !game->GetWorld ()->gatewaydefs.ValidIndex ( currentselect ) || !game->GetWorld ()->gatewaydefs.GetData ( currentselect ) )
		return;

	// Titles

	EclRegisterButton ( 80, 60, 350, 25, GetComputerScreen ()->maintitle, "", "changegateway_maintitle" );
	EclRegisterButtonCallbacks ( "changegateway_maintitle", DrawMainTitle, nullptr, nullptr, nullptr );
	EclRegisterButton ( 80, 80, 350, 20, GetComputerScreen ()->subtitle, "", "changegateway_subtitle" );
	EclRegisterButtonCallbacks ( "changegateway_subtitle", DrawSubTitle, nullptr, nullptr, nullptr );

	// Proceed and cancel

	EclRegisterButton ( 130, 380, 100, 15, "Proceed", "Click to go ahead with the exchange", "changegateway_buy" );
	EclRegisterButtonCallback ( "changegateway_buy", BuyConfirmClick );

	EclRegisterButton ( 300, 380, 100, 15, "Cancel", "Cancel the exchange", "changegateway_close" );
	EclRegisterButtonCallback ( "changegateway_close", BuyCancelClick );

	// Information

	EclRegisterButton ( 40, 130, 400, 250, " ", " ", "changegateway_text" );
	EclRegisterButtonCallbacks ( "changegateway_text", textbutton_draw, nullptr, nullptr, nullptr );

	GatewayDef *oldgateway = game->GetWorld ()->GetPlayer ()->gateway.curgatewaydef;
	UplinkAssert (oldgateway)

	GatewayDef *newgateway = game->GetWorld ()->gatewaydefs.GetData ( currentselect );
	UplinkAssert (newgateway)

	int price = GetGatewayPrice(currentselect);

	std::ostrstream caption;
	caption << "You have chosen to part exchange your current gateway for a " << newgateway->name << ".\n"
			   "If you chose to proceed, the money will be deducted from your account and Uplink Corporation "
			   "will prepare your new gateway.  We will install identical hardware in your new gateway, and "
			   "we will copy your memory banks from your current system to your new system.\n\n"
				"This process will take around 24 hours.  After this time, you will have to log off from the Net "
			   "and log back in again.  You will then be re-routed to your new gateway, and your old one will "
			   "be immediately dismantled.\n\n";

	if ( price < 0 )
		caption << "Because you are downgrading, you may lose some software or hardware during the transfer.\n";

	if ( price > 0 )	caption << "This exchange will cost you " << price << " credits.\n";
	else				caption << "You will be given " << price * -1 << " credits to balance the exchange.\n";

	caption << "Click Proceed to continue or Cancel to abort." << '\x0';

	EclRegisterCaptionChange ( "changegateway_text", caption.str(), 8000 );
	
	caption.rdbuf()->freeze( false );
	//delete [] caption.str();

}

void ChangeGatewayScreenInterface::Remove ()
{

	if ( IsVisible () ) {

		EclRemoveButton ( "changegateway_maintitle" );
		EclRemoveButton ( "changegateway_subtitle" );

		EclRemoveButton ( "changegateway_title" );

        int numRows = 10;

		for ( int i = 0; i < numRows; ++i ) {

			string bname = "changegateway_title " + to_string(i);
			EclRemoveButton ( bname );

		}

        ScrollBox::RemoveScrollBox ( "changegatewayscroll" );

		EclRemoveButton ( "changegateway_picture" );
		EclRemoveButton ( "changegateway_stats1" );
		EclRemoveButton ( "changegateway_stats2" );
		EclRemoveButton ( "changegateway_text" );

		EclRemoveButton ( "changegateway_buy" );
		EclRemoveButton ( "changegateway_close" );

	}

}

void ChangeGatewayScreenInterface::Update ()
{
}

bool ChangeGatewayScreenInterface::IsVisible ()
{

	return ( EclGetButton ( "changegateway_maintitle" ) != nullptr );

}

bool ChangeGatewayScreenInterface::ReturnKeyPressed ()
{

	return false;

}

bool ChangeGatewayScreenInterface::EscapeKeyPressed ()
{

    CloseClick ( nullptr );
    return true;

}

int ChangeGatewayScreenInterface::ScreenID ()
{

	return SCREEN_CHANGEGATEWAY;

}

GenericScreen *ChangeGatewayScreenInterface::GetComputerScreen ()
{

	UplinkAssert (cs)
	return (GenericScreen *) cs;

}
