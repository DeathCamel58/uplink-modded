
#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

#include <sstream>

#include "eclipse.h"
#include "gucci.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/opengl_interface.h"
#include "app/miscutils.h"
#include "app/serialise.h"

#include "options/options.h"

#include "game/game.h" 
#include "game/data/data.h"

#include "interface/interface.h"
#include "interface/scrollbox.h"
#include "interface/remoteinterface/remoteinterface.h"
#include "interface/remoteinterface/linksscreen_interface.h"
#include "interface/localinterface/localinterface.h"
#include "interface/localinterface/phonedialler.h"
#include "interface/localinterface/hud_interface.h"

#include "world/world.h"
#include "world/message.h"
#include "world/player.h"
#include "world/computer/computer.h"
#include "world/computer/computerscreen/linksscreen.h"

#include "mmgr.h"


int LinksScreenInterface::baseoffset = 0;

Image *LinksScreenInterface::ilink_tif = nullptr;
Image *LinksScreenInterface::ilink_h_tif = nullptr;
Image *LinksScreenInterface::ilink_c_tif = nullptr;

Image *LinksScreenInterface::iadd_tif = nullptr;
Image *LinksScreenInterface::iadd_h_tif = nullptr;
Image *LinksScreenInterface::iadd_c_tif = nullptr;


LinksScreenInterface::LinksScreenInterface ()
{

	ilink_tif = get_assignbitmap ( "link.tif" );
	ilink_h_tif = get_assignbitmap ( "link_h.tif" );
	ilink_c_tif = get_assignbitmap ( "link_c.tif" );

	iadd_tif = get_assignbitmap ( "add.tif" );
	iadd_h_tif = get_assignbitmap ( "add_h.tif" );
	iadd_c_tif = get_assignbitmap ( "add_c.tif" );

}

LinksScreenInterface::~LinksScreenInterface ()
{
    
    DeleteLListData ( fulllist );
	fulllist.Empty ();
    DeleteLListData ( filteredlist );
	filteredlist.Empty ();

	if ( ilink_tif ) {
		delete ilink_tif;
		ilink_tif = nullptr;
	}
	if ( ilink_h_tif ) {
		delete ilink_h_tif;
		ilink_h_tif = nullptr;
	}
	if ( ilink_c_tif ) {
		delete ilink_c_tif;
		ilink_c_tif = nullptr;
	}

	if ( iadd_tif ) {
		delete iadd_tif;
		iadd_tif = nullptr;
	}
	if ( iadd_h_tif ) {
		delete iadd_h_tif;
		iadd_h_tif = nullptr;
	}
	if ( iadd_c_tif ) {
		delete iadd_c_tif;
		iadd_c_tif = nullptr;
	}

}

void LinksScreenInterface::LinkClick ( Button *button )
{
	// Ignore clicks if large world map is shown
	if ( game->GetInterface ()->GetLocalInterface ()->GetHUD ()->wmi.IsVisibleWorldMapInterface () == WORLDMAP_LARGE )
		return;

	UplinkAssert ( button )

	int fileindex;
    string unused;
    istringstream stream(button->name);
    stream >> unused >> fileindex;
	fileindex += baseoffset;

	LList <string> *filteredlist = &((LinksScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ())->filteredlist;

	if ( filteredlist->ValidIndex (fileindex) ) {

		string ip = filteredlist->GetData (fileindex);

		game->GetWorld ()->GetPlayer ()->GetConnection ()->Disconnect ();
		game->GetWorld ()->GetPlayer ()->GetConnection ()->Reset ();

		auto *pd = new PhoneDialler ();
		pd->DialNumber ( 100, 100, ip, 5 );	
	
	}

}

void LinksScreenInterface::AfterPhoneDialler (string &ip, string &info )
{

	assert( !ip.empty() );

	game->GetWorld ()->GetPlayer ()->GetConnection ()->Disconnect ();
	game->GetWorld ()->GetPlayer ()->GetConnection ()->Reset ();

	game->GetWorld ()->GetPlayer ()->GetConnection ()->AddVLocation ( ip );
	game->GetWorld ()->GetPlayer ()->GetConnection ()->Connect ();

	game->GetInterface ()->GetRemoteInterface ()->RunNewLocation ();

}

void LinksScreenInterface::LinkDraw ( Button *button, bool highlighted, bool clicked )
{

	UplinkAssert ( button )

	clear_draw ( button->x, button->y, button->width, button->height );

	int linkindex;
    string unused;
    istringstream stream(button->name);
    stream >> unused >> linkindex;
	linkindex += baseoffset;

	LList <string> *filteredlist = &((LinksScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ())->filteredlist;
    string link = filteredlist->ValidIndex(linkindex) ? filteredlist->GetData (linkindex) : "";

	/*
		Re above line

		calling getdata like that will be pretty slow, since GetData on a linked list 
		has to iterate accross the entire array.

		But this call of LinkDraw will come after another call to LinkDraw, for the link
		at linkindex-1.  This means that sequential access is occuring - which will be ok
		in terms of speed.

    */

	if ( !link.empty() ) {

		if ( linkindex % 2 == 0 ) {

			glBegin ( GL_QUADS );
				SetColour ( "DarkPanelB" );   glVertex2i ( button->x, button->y );
				SetColour ( "DarkPanelA" );   glVertex2i ( button->x + button->width, button->y );
				SetColour ( "DarkPanelB" );   glVertex2i ( button->x + button->width, button->y + button->height );
				SetColour ( "DarkPanelA" );   glVertex2i ( button->x, button->y + button->height );
			glEnd ();

		}
		else {

			glBegin ( GL_QUADS );
				SetColour ( "DarkPanelA" );   glVertex2i ( button->x, button->y );
				SetColour ( "DarkPanelB" );   glVertex2i ( button->x + button->width, button->y );
				SetColour ( "DarkPanelA" );   glVertex2i ( button->x + button->width, button->y + button->height );
				SetColour ( "DarkPanelB" );   glVertex2i ( button->x, button->y + button->height );
			glEnd ();

		}

		SetColour ( "DefaultText" );

		// Draw the text

		GciDrawText ( button->x + 10, button->y + 10, link );

		string caption;
			
		if ( game->GetWorld ()->GetVLocation ( link ) ) {
			caption = game->GetWorld ()->GetVLocation ( link )->computer;
		} else {
			caption = "This link has expired";
		}

		GciDrawText ( button->x + 120, button->y + 10, caption );

		// Draw a bounding box
		if ( highlighted ) {

			SetColour ( "PanelHighlightBorder" );
			border_draw ( button );

		}

	}
	
}

void LinksScreenInterface::LinkMouseDown ( Button *button )
{

	UplinkAssert ( button )
	
	int linkindex;
    string unused;
    istringstream stream(button->name);
    stream >> unused >> linkindex;
	linkindex += baseoffset;

	LList <string> *filteredlist = &((LinksScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ())->filteredlist;

	if ( filteredlist->ValidIndex (linkindex) ) {

		button_click ( button );

	}

}

void LinksScreenInterface::LinkMouseMove ( Button *button )
{

	UplinkAssert ( button )
	
	int linkindex;
    string unused;
    istringstream stream(button->name);
    stream >> unused >> linkindex;
	linkindex += baseoffset;

	LList <string> *filteredlist = &((LinksScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ())->filteredlist;

	if ( filteredlist->ValidIndex (linkindex) ) {
 
		button_highlight ( button );

	}

}

void LinksScreenInterface::DeleteLinkDraw ( Button *button, bool highlighted, bool clicked )
{

	UplinkAssert ( button )
	
	int linkindex;
    string unused;
    istringstream stream(button->name);
    stream >> unused >> linkindex;
	linkindex += baseoffset;

	LList <string> *filteredlist = &((LinksScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ())->filteredlist;
    string link = filteredlist->ValidIndex(linkindex) ? filteredlist->GetData (linkindex) : "";

	if ( !link.empty() ) 
		imagebutton_draw ( button, highlighted, clicked );

	else
		clear_draw ( button->x, button->y, button->width, button->height );

}

void LinksScreenInterface::DeleteLinkClick ( Button *button )
{
	// Ignore clicks if large world map is shown
	if ( game->GetInterface ()->GetLocalInterface ()->GetHUD ()->wmi.IsVisibleWorldMapInterface () == WORLDMAP_LARGE )
		return;

	/*
		Safe to delete this link
		(This button would not exist if these 
		were not player links)
	
		*/

	UplinkAssert ( button )
	
	int linkindex;
    string unused;
    istringstream stream(button->name);
    stream >> unused >> linkindex;
	linkindex += baseoffset;

	LList <string> *filteredlist = &((LinksScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ())->filteredlist;
	string link = filteredlist->ValidIndex (linkindex) ? filteredlist->GetData (linkindex) : "";

	if ( !link.empty() && game->GetWorld ()->GetPlayer ()->HasLink (link) &&
         link != IP_INTERNIC ) {

		game->GetWorld ()->GetPlayer ()->RemoveLink (link);

		LinksScreenInterface *thisinterface = ((LinksScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ());
		UplinkAssert (thisinterface)
	
		int currentbaseoffset = baseoffset;
		thisinterface->SetFullList ( &(game->GetWorld ()->GetPlayer ()->links) );

		// Re-apply the filter
		
		Button *filterbutton = EclGetButton ( "linksscreen_filtertext" );
		if ( filterbutton ) {
			string filter = filterbutton->caption;
			if ( filter [0] == '\x0' )  thisinterface->ApplyFilter ( "" );
			else						thisinterface->ApplyFilter ( filter );
			baseoffset = currentbaseoffset;
		}
		else
			thisinterface->ApplyFilter ( "" );

	}

}

void LinksScreenInterface::AddLinkDraw ( Button *button, bool highlighted, bool clicked )
{

	UplinkAssert ( button )
	
	int linkindex;
    string unused;
    istringstream stream(button->name);
    stream >> unused >> linkindex;
	linkindex += baseoffset;

	LList <string> *filteredlist = &((LinksScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ())->filteredlist;
    string link = filteredlist->ValidIndex (linkindex) ? filteredlist->GetData (linkindex) : "";

	if ( !link.empty() && !game->GetWorld ()->GetPlayer ()->HasLink (link) ) 
		imagebutton_draw ( button, highlighted, clicked );

	else
		clear_draw ( button->x, button->y, button->width, button->height );

}

void LinksScreenInterface::AddLinkClick ( Button *button )
{
	// Ignore clicks if large world map is shown
	if ( game->GetInterface ()->GetLocalInterface ()->GetHUD ()->wmi.IsVisibleWorldMapInterface () == WORLDMAP_LARGE )
		return;

	UplinkAssert ( button )
	
	int linkindex;
    string unused;
    istringstream stream(button->name);
    stream >> unused >> linkindex;
	linkindex += baseoffset;

	LList <string> *filteredlist = &((LinksScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ())->filteredlist;
    string link = filteredlist->ValidIndex(linkindex) ? filteredlist->GetData (linkindex) : "";

	if ( !link.empty() && !game->GetWorld ()->GetPlayer ()->HasLink (link) )
		game->GetWorld ()->GetPlayer ()->GiveLink ( link );

}

void LinksScreenInterface::ShowLinkDraw ( Button *button, bool highlighted, bool clicked )
{
    
	UplinkAssert ( button )
	
	int linkindex;
    string unused;
    istringstream stream(button->name);
    stream >> unused >> linkindex;
	linkindex += baseoffset;

	LList <string> *filteredlist = &((LinksScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ())->filteredlist;
    string link = filteredlist->ValidIndex(linkindex) ? filteredlist->GetData (linkindex) : "";

    if ( !link.empty() ) {

	    VLocation *vl = game->GetWorld ()->GetVLocation (link);

		if ( vl->colored )
			imagebutton_draw ( button, highlighted, clicked, iadd_tif, iadd_h_tif, iadd_c_tif );

		else if ( vl->displayed )
			imagebutton_draw ( button, highlighted, clicked, ilink_tif, ilink_h_tif, ilink_c_tif );

		else
            clear_draw ( button->x, button->y, button->width, button->height );

    } else {
        clear_draw(button->x, button->y, button->width, button->height);
    }

}

static void UpdateTooltip ( Button *button, VLocation *vl )
{

	if ( vl->colored )
		button->SetTooltip ( "Toggle the visibility of this link on the map" );

	else if ( vl->displayed )
		button->SetTooltip ( "Toggle the color of this link on the map" );

	else
		button->SetTooltip ( "Toggle the visibility and the color of this link on the map" );

}

void LinksScreenInterface::ShowLinkClick ( Button *button )
{
	// Ignore clicks if large world map is shown
	if ( game->GetInterface ()->GetLocalInterface ()->GetHUD ()->wmi.IsVisibleWorldMapInterface () == WORLDMAP_LARGE )
		return;

	UplinkAssert ( button )
	
	int linkindex;
    string unused;
    istringstream stream(button->name);
    stream >> unused >> linkindex;
	linkindex += baseoffset;

	LList <string> *filteredlist = &((LinksScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ())->filteredlist;

	string link = filteredlist->GetData (linkindex);

    if ( !link.empty() ) {

	    VLocation *vl = game->GetWorld ()->GetVLocation (link);

		if ( vl->colored ) {
			vl->SetDisplayed ( false );
			vl->SetColorDisplayed ( false );
		}
		else if ( vl->displayed ) {
			vl->SetColorDisplayed ( true );
		}
		else {
			vl->SetDisplayed ( true );
		}

		UpdateTooltip ( button, vl );
		tooltip_update ( button->tooltip );

    }

}

void LinksScreenInterface::ShowLinkMouseMove ( Button *button )
{

	UplinkAssert ( button )
	
	int linkindex;
	string unused;
    istringstream stream(button->name);
    stream >> unused >> linkindex;
	linkindex += baseoffset;

	LList <string> *filteredlist = &((LinksScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ())->filteredlist;

	string link = filteredlist->GetData (linkindex);

    if ( !link.empty() ) {

	    VLocation *vl = game->GetWorld ()->GetVLocation (link);

		UpdateTooltip ( button, vl );

	}

	button_highlight ( button );

}

void LinksScreenInterface::FilterDraw ( Button *button, bool highlighted, bool clicked )
{

	UplinkAssert (button)

	textbutton_draw ( button, highlighted, clicked );

	glColor3f ( 1.0f, 1.0f, 1.0f );
	border_draw ( button );

}

void LinksScreenInterface::FilterClick ( Button *button )
{

	UplinkAssert (button)

	LinksScreenInterface *thisinterface = ((LinksScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ());
	UplinkAssert (thisinterface)

	string filter = EclGetButton ( "linksscreen_filtertext" )->caption;

	if ( filter [0] == '\x0' )  thisinterface->ApplyFilter ( "" );
	else						thisinterface->ApplyFilter ( filter );

}

void LinksScreenInterface::ScrollChange (const string &scrollname, int newValue )
{

    baseoffset = newValue;

	for ( int i = 0; i < NumLinksOnScreen(); ++i ) {

		string name = "linksscreen_link " + to_string(i);
		EclDirtyButton ( name );

   		name = "linksscreen_deletelink " + to_string(i);
    	EclDirtyButton ( name );

        name = "linksscreen_addlink " + to_string(i);
		EclDirtyButton ( name );

        name = "linksscreen_showlink " + to_string(i);
        EclDirtyButton ( name );

	}

}

void LinksScreenInterface::CloseClick ( Button *button )
{

	auto *lsi = (LinksScreenInterface *) GetInterfaceScreen ( SCREEN_LINKSSCREEN );
	UplinkAssert (lsi)

	int nextpage = lsi->GetComputerScreen ()->nextpage;

	if ( nextpage != -1 ) 
		game->GetInterface ()->GetRemoteInterface ()->RunScreen ( nextpage, lsi->GetComputerScreen ()->GetComputer () );		
	
}

void LinksScreenInterface::SetFullList ( LList <string> *newfulllist )
{

	UplinkAssert (newfulllist)

    DeleteLListData ( fulllist );
    DeleteLListData ( filteredlist );
    
	fulllist.Empty ();
	filteredlist.Empty ();

    for ( int i = 0; i < newfulllist->Size (); ++i ) {

        string copydata = newfulllist->GetData (i);
        fulllist.PutData ( copydata );

    }
	
	ApplyFilter ( "" );

}

void LinksScreenInterface::SetFullList ()
{

	DeleteLListData ( fulllist );
    fulllist.Empty ();

    for ( int i = 0; i < filteredlist.Size (); ++i ) {

		string copydata = filteredlist.GetData (i);
		fulllist.PutData ( copydata );

    }

	ApplyFilter ( "" );

}

void LinksScreenInterface::ApplyFilter (const string &filter )
{

	//
	// Do the filtering
	//

    DeleteLListData ( filteredlist );
    filteredlist.Empty ();

	if ( !filter.empty() ) {

		string lowercasefilter = LowerCaseString ( filter );

		for ( int i = 0; i < fulllist.Size (); ++i ) {

			VLocation *vl = game->GetWorld ()->GetVLocation ( fulllist.GetData (i) );

			if ( !vl ) { 
				UplinkAbortArgs ( "Cannot find location for ip %s", fulllist.GetData (i).c_str() )
			}

			string computername = vl->computer;
			string lowercasename = LowerCaseString ( computername );

            if ( lowercasename.find( lowercasefilter ) != string::npos) {

                string datacopy = fulllist.GetData (i);
				filteredlist.PutData ( datacopy );

            }

            lowercasename = "";

		}

        lowercasefilter = "";

	}
	else {

        for ( int i = 0; i < fulllist.Size (); ++i ) {

			string datacopy = fulllist.GetData (i);
            filteredlist.PutData ( datacopy );

        }

	}

	baseoffset = 0;

	//
	// Dirty the buttons
	//

	int numLinks = NumLinksOnScreen();
	for ( int ib = 0; ib < numLinks; ++ib ) {

		string name = "linksscreen_link " + to_string(ib);
		EclDirtyButton ( name );

		if ( GetComputerScreen ()->SCREENTYPE == LINKSSCREENTYPE_PLAYERLINKS ) {

    		name = "linksscreen_deletelink " + to_string(ib);
	    	EclDirtyButton ( name );

            string displaylink = "linksscreen_showlink " + to_string(ib);
            EclDirtyButton ( displaylink );

        } else {

		    name = "linksscreen_addlink " + to_string(ib);
		    EclDirtyButton ( name );

        }

	}

    //
    // Update the scrollbar
    //

    ScrollBox *sb = ScrollBox::GetScrollBox("linksscreen_scroll");
    if ( sb ) sb->SetNumItems( filteredlist.Size() );

}

int LinksScreenInterface::NumLinksOnScreen ()
{

    int screenheight = app->GetOptions()->GetOptionValue( "graphics_screenheight" );
    int availablePixels = screenheight - 120 - 145;
    return availablePixels/15;

}

void LinksScreenInterface::Create ()
{

	if ( cs ) Create ( cs );
	else cout << "LinksScreenInterface::Create, tried to create when GenericScreen==nullptr" << endl;

}

void LinksScreenInterface::Create ( ComputerScreen *newcs )
{

	UplinkAssert ( newcs )
	cs = newcs;

	if ( !IsVisible () ) {

		// Draw the screen titles

		EclRegisterButton ( 80, 60, 350, 25, GetComputerScreen ()->maintitle, "", "linksscreen_maintitle" );
		EclRegisterButtonCallbacks ( "linksscreen_maintitle", DrawMainTitle, nullptr, nullptr, nullptr );
		EclRegisterButton ( 80, 80, 350, 20, GetComputerScreen ()->subtitle, "", "linksscreen_subtitle" );
		EclRegisterButtonCallbacks ( "linksscreen_subtitle", DrawSubTitle, nullptr, nullptr, nullptr );

		// Create the links

		Image *iclose_tif = get_assignbitmap ( "close.tif" );
		Image *iclose_h_tif = get_assignbitmap ( "close_h.tif" );
		Image *iclose_c_tif = get_assignbitmap ( "close_c.tif" );

        int numLinks = NumLinksOnScreen();

		for ( int li = 0; li < numLinks; ++li ) {

			string name = "linksscreen_link " + to_string(li);
			EclRegisterButton ( 30, 145 + li * 15, SY(375), 14, "", "Connect to this computer", name );
			EclRegisterButtonCallbacks ( name, LinkDraw, LinkClick, LinkMouseDown, LinkMouseMove );

			if ( GetComputerScreen ()->SCREENTYPE == LINKSSCREENTYPE_PLAYERLINKS ) {

				// Give the player the option to delete any link

				name = "linksscreen_deletelink " + to_string(li);
				EclRegisterButton ( 15, 145 + li * 15, 13, 13, "", "Delete this link", name );
				button_assignbitmaps ( name, iclose_tif, iclose_h_tif, iclose_c_tif );
				EclRegisterButtonCallbacks ( name, DeleteLinkDraw, DeleteLinkClick, button_click, button_highlight );

                // Give the player the option to display the link on the map

                string displaylink = "linksscreen_showlink " + to_string(li);
                EclRegisterButton ( 34 + SY(375), 145 + li * 15, 13, 13, "", "", displaylink );
                EclRegisterButtonCallbacks ( displaylink, ShowLinkDraw, ShowLinkClick, button_click, ShowLinkMouseMove );
                
			}
			else {

				// Give the player the option to add this link to his collection

				name = "linksscreen_addlink " + to_string(li);
				EclRegisterButton ( 15, 145 + li * 15, 13, 13, "", "Store this link", name );
				button_assignbitmaps ( name, iadd_tif, iadd_h_tif, iadd_c_tif );
				EclRegisterButtonCallbacks ( name, AddLinkDraw, AddLinkClick, button_click, button_highlight );

			}

		}

		delete iclose_tif;
		delete iclose_h_tif;
		delete iclose_c_tif;
		
		//
		// Set the full list depending on the screen type
		//

		if ( GetComputerScreen ()->SCREENTYPE == LINKSSCREENTYPE_PLAYERLINKS ) {

			SetFullList ( &(game->GetWorld ()->GetPlayer ()->links) );

		}
		else if ( GetComputerScreen ()->SCREENTYPE == LINKSSCREENTYPE_ALLLINKS ) {

			LList <string> alllinks;
			DArray <VLocation *> *locs = game->GetWorld ()->locations.ConvertToDArray ();

			for ( int i = 0; i < locs->Size (); ++i )
				if ( locs->ValidIndex (i) )
					if ( locs->GetData (i)->listed )
						alllinks.PutData ( locs->GetData (i)->ip );

			delete locs;

			SetFullList ( &alllinks );

		}
		else if ( GetComputerScreen ()->SCREENTYPE == LINKSSCREENTYPE_LOCALLINKS ) {

			string companyname = GetComputerScreen ()->GetComputer ()->companyname;

			LList <string> alllinks;
			DArray <VLocation *> *locs = game->GetWorld ()->locations.ConvertToDArray ();

			for ( int i = 0; i < locs->Size (); ++i )
				if ( locs->ValidIndex (i) )		
                    if ( locs->GetData (i)->GetOBJECTID () != OID_VLOCATIONSPECIAL )
					    alllinks.PutData ( locs->GetData (i)->ip );

			delete locs;

			SetFullList ( &alllinks );
			ApplyFilter ( companyname );
			SetFullList ();
	
		}

		if ( fulllist.Size () >= NumLinksOnScreen () ) {

			// Create the scrollbar
   
            ScrollBox::CreateScrollBox( "linksscreen_scroll", 50 + SY(375), 145, 15, 
                                        NumLinksOnScreen() * 15, fulllist.Size(), NumLinksOnScreen(), 0,  ScrollChange );

			EclRegisterButton ( 50 + SY(375), 125, 15, 15, "", "", "linksscreen_topright" );

			// Create the filter buttons

			EclRegisterButton ( 15, 150 + NumLinksOnScreen() * 15, 135, 15, "Filter", "Click here to apply the filter", "linksscreen_filter" );
			EclRegisterButtonCallback ( "linksscreen_filter", FilterClick );
			
			int width = (65 + SY(375)) - 155;
            if ( GetComputerScreen ()->nextpage != -1 ) width -= 50;

            EclRegisterButton ( 155, 150 + NumLinksOnScreen() * 15, width, 15, "", "Enter filter pattern here", "linksscreen_filtertext" );
			EclRegisterButtonCallbacks ( "linksscreen_filtertext", FilterDraw, nullptr, button_click, button_highlight );
			EclMakeButtonEditable ( "linksscreen_filtertext" );

		}

		// Create the exit button

		if ( GetComputerScreen ()->nextpage != -1 ) {

            int width = (65 + SY(375)) - 205;
            EclRegisterButton ( 160 + width, 150 + NumLinksOnScreen() * 15, 45, 15, "Close", "Close this screen", "linksscreen_close" );
			EclRegisterButtonCallback ( "linksscreen_close", CloseClick );

		}

		// Create the title bars if this isn't the player links screen,
		// or if more than 13 links are on display

		if ( GetComputerScreen ()->SCREENTYPE != LINKSSCREENTYPE_PLAYERLINKS ||
			 fulllist.Size () >= NumLinksOnScreen() ) {

			int xpos = 15;
			int width = 150 - xpos;
			EclRegisterButton ( xpos, 125, width, 15, "IP Address", "Shows the IP address of the location", "linksscreen_iptitle" );
			EclRegisterButton ( 155, 125, (48 + SY(375)) - 155, 15, "Location name", "Shows the computer name at that location", "linksscreen_comptitle" );

		}


		// Removed, ApplyFilter is already done in SetFullList
		//ApplyFilter ( nullptr );

	}

}

void LinksScreenInterface::Remove ()
{

	if ( IsVisible () ) {

		EclRemoveButton ( "linksscreen_maintitle" );
		EclRemoveButton ( "linksscreen_subtitle" );

		EclRemoveButton ( "linksscreen_iptitle" );
		EclRemoveButton ( "linksscreen_comptitle" );

		for ( int i = 0; i < NumLinksOnScreen(); ++i ) {

			string name = "linksscreen_link " + to_string(i);
			EclRemoveButton ( name );

			if ( GetComputerScreen ()->SCREENTYPE == LINKSSCREENTYPE_PLAYERLINKS ) {

			    name = "linksscreen_deletelink " + to_string(i);
			    EclRemoveButton ( name );

                string displaylink = "linksscreen_showlink " + to_string(i);
                EclRemoveButton ( displaylink );

            } else {

			    name = "linksscreen_addlink " + to_string(i);
			    EclRemoveButton ( name );

            }

		}

        if ( ScrollBox::GetScrollBox( "linksscreen_scroll" ) != nullptr ) {
		
            ScrollBox::RemoveScrollBox( "linksscreen_scroll" );

			EclRemoveButton ( "linksscreen_topright" );
			EclRemoveButton ( "linksscreen_filter" );
			EclRemoveButton ( "linksscreen_filtertext" );

		}

		EclRemoveButton ( "linksscreen_close" );

	}

}

bool LinksScreenInterface::IsVisible ()
{

	return ( EclGetButton ( "linksscreen_maintitle" ) != nullptr );

}

bool LinksScreenInterface::ReturnKeyPressed ()
{

	Button *filterbutton = EclGetButton ( "linksscreen_filtertext" );
	
	if ( filterbutton ) {
		string filter = filterbutton->caption;
		if ( filter [0] == '\x0' )  ApplyFilter ( "" );
		else						ApplyFilter ( filter );
		return true;
	}

	return false;
}

void LinksScreenInterface::Update ()
{

}

LinksScreen *LinksScreenInterface::GetComputerScreen ()
{

	UplinkAssert (cs)
	return (LinksScreen *) cs;

}

int LinksScreenInterface::ScreenID ()
{

	return SCREEN_LINKSSCREEN;

}
