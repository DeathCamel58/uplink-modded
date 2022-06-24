
#ifdef WIN32
#include <windows.h>
#endif

#include <sstream>

#include "eclipse.h"
#include "gucci.h"
#include "soundgarden.h"

#include "app/globals.h"
#include "app/miscutils.h"
#include "app/opengl_interface.h"

#include "game/game.h"

#include "interface/interface.h"
#include "interface/remoteinterface/remoteinterface.h"
#include "interface/remoteinterface/menuscreen_interface.h"

#include "world/world.h"
#include "world/computer/computer.h"
#include "world/computer/computerscreen/menuscreen.h"

#include "mmgr.h"


void MenuScreenInterface::DrawMenuOption ( Button *button, bool highlighted, bool clicked )
{

    SetColour ( "MenuText" );
    GciDrawText ( button->x, button->y + 20, button->caption, HELVETICA_18 );

}

void MenuScreenInterface::DrawMenuOptionDimmed ( Button *button, bool highlighted, bool clicked )
{

	SetColour ( "DimmedText" );
	GciDrawText ( button->x, button->y + 20, button->caption, HELVETICA_18 );

}

void MenuScreenInterface::ClickMenuScreenOption ( Button *button )
{

	//Removed, wasn't heard when using mikmod
    //SgPlaySound ( RsArchiveFileOpen ("sounds/menuitemclick.wav"), "sounds/menuitemclick.wav", false );

	char text [32];
	int nextpage, unused;
	char ip [ SIZE_VLOCATION_IP ] = {0};
    istringstream stream(button->name);
    stream >> text >> unused >> nextpage >> ip;

	Computer *comp = nullptr;
    VLocation *loc = game->GetWorld()->GetVLocation(ip);
    if (loc)
        comp = loc->GetComputer();
    if ( nextpage != -1 ) game->GetInterface ()->GetRemoteInterface ()->RunScreen ( nextpage, comp );

}

void MenuScreenInterface::Create ( ComputerScreen *newcs )
{

	UplinkAssert ( newcs )
	cs = newcs;

	if ( !IsVisible () ) {
		
		EclRegisterButton ( 0, 0, 0, 0, "", "", "menuscreeninterface" );
		EclRegisterButtonCallbacks ( "menuscreeninterface", nullptr, nullptr, nullptr, nullptr );

		EclRegisterButton ( 80, 60, 350, 25, GetComputerScreen ()->maintitle, "", "menuscreen_maintitle" );
		EclRegisterButtonCallbacks ( "menuscreen_maintitle", DrawMainTitle, nullptr, nullptr, nullptr );
		EclRegisterButton ( 80, 80, 350, 20, GetComputerScreen ()->subtitle, "", "menuscreen_subtitle" );
		EclRegisterButtonCallbacks ( "menuscreen_subtitle", DrawSubTitle, nullptr, nullptr, nullptr );

		int timeinterval = (int) ( 1000.0 / GetComputerScreen ()->NumOptions () );

		Computer *comp = GetComputerScreen ()->GetComputer ();

		for ( int i = 0; i < GetComputerScreen ()->NumOptions (); ++i ) {

			if ( game->GetInterface ()->GetRemoteInterface ()->security_level <= GetComputerScreen ()->GetSecurity (i) ) {

				// You have clearance to view this option

				string bname1 = "menuscreenimage " + to_string(i) + " " + to_string(GetComputerScreen ()->GetNextPage ( i )) + " " + comp->ip;
				string bname2 = "menuscreentext " + to_string(i) + " " + to_string(GetComputerScreen ()->GetNextPage ( i )) + " " + comp->ip;

				EclRegisterButton ( -350, 120 + i * 30, 16, 16, "", GetComputerScreen ()->GetTooltip ( i ), bname1 );
				button_assignbitmaps ( bname1, "menuscreenoption.tif", "menuscreenoption_h.tif", "menuscreenoption_c.tif" );
				EclRegisterButtonCallback ( bname1, ClickMenuScreenOption );

				EclRegisterButton ( -300, 120 + i * 30 - 5, 300, 32, GetComputerScreen ()->GetCaption ( i ), GetComputerScreen ()->GetTooltip ( i ), bname2 );
				EclRegisterButtonCallbacks ( bname2, DrawMenuOption, ClickMenuScreenOption, button_click, button_highlight );

				EclRegisterMovement ( bname1, 100, 130 + i * 30, (i+1) * timeinterval );
				EclRegisterMovement ( bname2, 140, 130 + i * 30 - 5, (i+1) * timeinterval );

			}
			else {

				// You don't have clearance to view this option

				string bname1 = "menuscreenimage " + to_string(i) + " " + to_string(GetComputerScreen ()->GetNextPage ( i )) + " " + comp->ip;
				string bname2 = "menuscreentext " + to_string(i) + " " + to_string(GetComputerScreen ()->GetNextPage ( i )) + " " + comp->ip;

				EclRegisterButton ( -350, 120 + i * 30, 16, 16, "", GetComputerScreen ()->GetTooltip ( i ), bname1 );
				button_assignbitmap ( bname1, "menuscreenoption.tif" );
				EclGetButton ( bname1 )->image_standard->SetAlpha ( ALPHA_DISABLED );
				EclRegisterButtonCallbacks ( bname1, imagebutton_draw, nullptr, nullptr, nullptr );

				EclRegisterButton ( -300, 120 + i * 30 - 5, 300, 32, GetComputerScreen ()->GetCaption ( i ), GetComputerScreen ()->GetTooltip ( i ), bname2 );
				EclRegisterButtonCallbacks ( bname2, DrawMenuOptionDimmed, nullptr, nullptr, nullptr );

				EclRegisterMovement ( bname1, 100, 130 + i * 30, (i+1) * timeinterval );
				EclRegisterMovement ( bname2, 140, 130 + i * 30 - 5, (i+1) * timeinterval );


			}

		}
		
	}

}

void MenuScreenInterface::Remove ()
{

	if ( IsVisible () ) {

		EclRemoveButton ( "menuscreeninterface" );

		EclRemoveButton ( "menuscreen_maintitle" );
		EclRemoveButton ( "menuscreen_subtitle" );

		Computer *comp = GetComputerScreen ()->GetComputer ();

		for ( int i = 0; i < GetComputerScreen ()->NumOptions (); ++i ) {

			string bname1 = "menuscreenimage " + to_string(i) + " " + to_string(GetComputerScreen ()->GetNextPage ( i )) + " " + comp->ip;
			string bname2 = "menuscreentext " + to_string(i) + " " + to_string(GetComputerScreen ()->GetNextPage ( i )) + " " + comp->ip;
			
			EclRemoveButton ( bname1 );
			EclRemoveButton ( bname2 );

		}

	}

}

bool MenuScreenInterface::IsVisible ()
{

	return ( EclGetButton ( "menuscreeninterface" ) != nullptr );

}

int MenuScreenInterface::ScreenID ()
{

	return SCREEN_MENUSCREEN;

}

MenuScreen *MenuScreenInterface::GetComputerScreen ()
{

	UplinkAssert (cs)
	return (MenuScreen *) cs;

}

