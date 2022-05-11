
#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

#include <GL/glu.h>


#include "eclipse.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/opengl_interface.h"
#include "app/serialise.h"

#include "options/options.h"

#include "mainmenu/mainmenu.h"

#include "network/network.h"
#include "network/networkclient.h"
#include "network/interfaces/clientstatusinterface.h"

#include "mmgr.h"



ClientStatusInterface::ClientStatusInterface()
{

	UplinkStrncpy ( hardware, " ", sizeof ( hardware ) )
	UplinkStrncpy ( hudupgrades, " ", sizeof ( hudupgrades ) )
	UplinkStrncpy ( connection, " ", sizeof ( connection ) )

	UplinkStrncpy ( ratings, " ", sizeof ( ratings ) )
	UplinkStrncpy ( criminal, " ", sizeof ( criminal ) )
	UplinkStrncpy ( financial, " ", sizeof ( financial ) )

	news.PutDataAtStart ( " " );

}

ClientStatusInterface::~ClientStatusInterface()
{

    DeleteLListData ( &news );

}

void ClientStatusInterface::BackgroundDraw ( Button *button, bool highlighted, bool clicked )
{
	
	glBegin ( GL_QUADS );		
		glColor3ub ( 8, 20, 0 );		glVertex2i ( button->x, button->y + button->height );
		glColor3ub ( 8, 20, 124 );		glVertex2i ( button->x, button->y );
		glColor3ub ( 8, 20, 0 );		glVertex2i ( button->x + button->width, button->y );
		glColor3ub ( 8, 20, 124 );		glVertex2i ( button->x + button->width, button->y + button->height );
	glEnd ();

	glColor3ub ( 81, 138, 215 );
	border_draw ( button );

}

void ClientStatusInterface::GatewayPanelDraw ( Button *button, bool highlighted, bool clicked )
{

	int screenheight = app->GetOptions ()->GetOptionValue ( "graphics_screenheight" );
	glScissor ( button->x, screenheight - (button->y + button->height), button->width, button->height );	
	glEnable ( GL_SCISSOR_TEST );


	auto *thisint = (ClientStatusInterface *) app->GetNetwork ()->GetClient ()->GetNetworkScreen ();
	UplinkAssert (thisint)

	BackgroundDraw ( button, highlighted, clicked );

	int x = button->x;
	int y = button->y;

	glColor4f ( 1.0f, 1.0f, 1.0f, 1.0f );

	GciDrawText ( x + 55, y + 40, "Hardware", HELVETICA_18 );
	text_draw ( x + 20, y + 70, thisint->hardware, button->width );
	
	GciDrawText ( x + 40, y + 200, "HUD Upgrades", HELVETICA_18 );
	text_draw ( x + 20, y + 230, thisint->hudupgrades, button->width );

	GciDrawText ( x + 50, y + 300, "Connection", HELVETICA_18 );
	text_draw ( x + 20, y + 330, thisint->connection, button->width );
	
	glDisable ( GL_SCISSOR_TEST );

}

void ClientStatusInterface::PersonalPanelDraw ( Button *button, bool highlighted, bool clicked )
{

	int screenheight = app->GetOptions ()->GetOptionValue ( "graphics_screenheight" );
	glScissor ( button->x, screenheight - (button->y + button->height), button->width, button->height );	
	glEnable ( GL_SCISSOR_TEST );


	auto *thisint = (ClientStatusInterface *) app->GetNetwork ()->GetClient ()->GetNetworkScreen ();
	UplinkAssert (thisint)

	BackgroundDraw ( button, highlighted, clicked );

	int x = button->x;
	int y = button->y;

	glColor4f ( 1.0f, 1.0f, 1.0f, 1.0f );

	GciDrawText ( x + 50, y + 40, "Ratings", HELVETICA_18 );
	text_draw ( x + 20, y + 70, thisint->ratings, button->width );

	GciDrawText ( x + 50, y + 160, "Criminal", HELVETICA_18 );
	text_draw ( x + 20, y + 190, thisint->criminal, button->width );

	GciDrawText ( x + 50, y + 280, "Financial", HELVETICA_18 );
	text_draw ( x + 20, y + 310, thisint->financial, button->width );

	glDisable ( GL_SCISSOR_TEST );

}

void ClientStatusInterface::WorldPanelDraw ( Button *button, bool highlighted, bool clicked )
{

	int screenheight = app->GetOptions ()->GetOptionValue ( "graphics_screenheight" );
	glScissor ( button->x, screenheight - (button->y + button->height), button->width, button->height );	
	glEnable ( GL_SCISSOR_TEST );


	auto *thisint = (ClientStatusInterface *) app->GetNetwork ()->GetClient ()->GetNetworkScreen ();
	UplinkAssert (thisint)

	BackgroundDraw ( button, highlighted, clicked );

	int x = button->x;
	int y = button->y;

	glColor4f ( 1.0f, 1.0f, 1.0f, 1.0f );

	GciDrawText ( x + 50, y + 40, "Headlines", HELVETICA_18 );

	for ( int i = 0; i < thisint->news.Size (); ++i ) {
		if ( thisint->news.ValidIndex (i) ) {

			char *newsitem = thisint->news.GetData (i);
			UplinkAssert (newsitem)

			text_draw ( x + 10, y + 70 + i * 55, newsitem, button->width + 20 );

		}
	}

	glDisable ( GL_SCISSOR_TEST );

}

void ClientStatusInterface::MainMenuClick ( Button *button )
{

	assert( app->GetNetwork ()->STATUS == NETWORK_CLIENT );

	app->GetNetwork ()->GetClient ()->SetClientType ( CLIENT_NONE );
	app->GetMainMenu ()->RunScreen ( MAINMENU_LOGIN );

}

void ClientStatusInterface::SetHardware	(const string &newhw )
{

	assert( newhw.length() < sizeof(hardware) );
	UplinkStrncpy ( hardware, newhw.c_str(), sizeof ( hardware ) )

	EclDirtyButton ( "status_gatewaybg" );

}

void ClientStatusInterface::SetHUDUpgrades (const string &newhud )
{
	
	assert( newhud.length() < sizeof(hudupgrades) );
	UplinkStrncpy ( hudupgrades, newhud.c_str(), sizeof ( hudupgrades ) )

	EclDirtyButton ( "status_gatewaybg" );

}

void ClientStatusInterface::SetConnection	(const string &newconnection )
{

	assert( newconnection.length() < sizeof(connection) );
	UplinkStrncpy ( connection, newconnection.c_str(), sizeof ( connection ) )

	EclDirtyButton ( "status_gatewaybg" );

}

void ClientStatusInterface::SetRating (const string &newrating )
{
	
	assert( newrating.length() < sizeof(ratings) );
	UplinkStrncpy ( ratings, newrating.c_str(), sizeof ( ratings ) )

	EclDirtyButton ( "status_personalbg" );

}

void ClientStatusInterface::SetFinancial (const string &newfinancial )
{

	assert(newfinancial.length() < sizeof(financial) );
	UplinkStrncpy ( financial, newfinancial.c_str(), sizeof ( financial ) )

	EclDirtyButton ( "status_personalbg" );

}

void ClientStatusInterface::SetCriminal  (const string &newcriminal )
{

	assert(newcriminal.length() < sizeof(criminal) );
	UplinkStrncpy ( criminal, newcriminal.c_str(), sizeof ( criminal ) )

	EclDirtyButton ( "status_personalbg" );

}

void ClientStatusInterface::AddNewsStory (const string &newsstory )
{

	char *newscopy = new char [newsstory.length()+1];
	UplinkSafeStrcpy ( newscopy, newsstory.c_str() )

	news.PutDataAtStart ( newscopy );

	while ( news.Size () > 6 )	news.RemoveData (6);

	EclDirtyButton ( "status_worldbg" );

}

void ClientStatusInterface::Create ()
{

	if ( !IsVisible () ) {

		int screenw = app->GetOptions ()->GetOptionValue ( "graphics_screenwidth" );
		int screenh = app->GetOptions ()->GetOptionValue ( "graphics_screenheight" );

		// Create a close button

		EclRegisterButton ( 3, 4, 13, 13, "", "Go to the Main Menu", "status_mainmenu" );
		button_assignbitmaps ( "status_mainmenu","close.tif", "close_h.tif", "close_c.tif" );
		EclRegisterButtonCallback ( "status_mainmenu", MainMenuClick );

		// Create a "gateway" panel

		EclRegisterButton ( 10, 35, 200, 400, "", "", "status_gatewaybg" );
		EclRegisterButtonCallbacks ( "status_gatewaybg", GatewayPanelDraw, nullptr, nullptr, nullptr );
		EclRegisterButton ( 13, 38, 193, 15, "Gateway Status", "Shows the status of your Gateway", "status_gatewaytitle" );
			
		// Create a "world" panel

		EclRegisterButton ( 220, 35, 200, 400, "", "", "status_worldbg" );
		EclRegisterButtonCallbacks ( "status_worldbg", WorldPanelDraw, nullptr, nullptr, nullptr );
		EclRegisterButton ( 223, 38, 193, 15, "World Status", "Shows the status of the world", "status_worldtitle" );

		// Create a "personal" panel

		EclRegisterButton ( 430, 35, 200, 400, "", "", "status_personalbg" );
		EclRegisterButtonCallbacks ( "status_personalbg", PersonalPanelDraw, nullptr, nullptr, nullptr );
		EclRegisterButton ( 433, 38, 193, 15, "Personal Status", "Shows your personal status", "status_personaltitle" );

	}

}

void ClientStatusInterface::Remove ()
{

	if ( IsVisible () ) {

		EclRemoveButton ( "status_mainmenu" );

		EclRemoveButton ( "status_gatewaytitle" );
		EclRemoveButton ( "status_gatewaybg" );

		EclRemoveButton ( "status_personaltitle" );
		EclRemoveButton ( "status_personalbg" );

		EclRemoveButton ( "status_worldtitle" );
		EclRemoveButton ( "status_worldbg" );

	}

}

void ClientStatusInterface::Update ()
{
}

bool ClientStatusInterface::IsVisible ()
{

	return ( EclGetButton ( "status_mainmenu" ) != nullptr );

}

int  ClientStatusInterface::ScreenID ()
{

	return CLIENT_STATUS;

}
