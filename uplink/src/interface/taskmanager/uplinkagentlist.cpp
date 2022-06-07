
#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

#include <cstring>
#include <cstdio>
#include <sstream>

#include "soundgarden.h"
#include "gucci.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/opengl_interface.h"
#include "app/serialise.h"

#include "options/options.h"

#include "game/game.h"
#include "game/scriptlibrary.h"

#include "interface/interface.h"
#include "interface/remoteinterface/remoteinterface.h"
#include "interface/remoteinterface/remoteinterfacescreen.h"
#include "interface/taskmanager/taskmanager.h"
#include "interface/taskmanager/uplinkagentlist.h"

#include "world/world.h"
#include "world/player.h"
#include "world/computer/computer.h"
#include "world/computer/computerscreen/passwordscreen.h"

#include "mmgr.h"


int UplinkAgentList::baseoffset = 0;


UplinkAgentList::UplinkAgentList () : UplinkTask ()
{
}

UplinkAgentList::~UplinkAgentList()
{

    DeleteDArrayData ( &handles );
    DeleteDArrayData ( &names );

}

void UplinkAgentList::Initialise ()
{
}

void UplinkAgentList::Tick ( int n )
{

	if ( IsInterfaceVisible () ) {


	}

}

void UplinkAgentList::UplinkAgentListDraw ( Button *button, bool highlighted, bool clicked )
{

	int pid;
	char bname [64];
    istringstream stream(button->name);
    stream >> bname >> pid;
    auto *ual = (UplinkAgentList *) SvbGetTask ( pid );
    UplinkAssert (ual)

    //
    // Draw the background

	glBegin ( GL_QUADS );		
		glColor3ub ( 8, 20, 0 );		glVertex2i ( button->x, button->y + button->height );
		glColor3ub ( 8, 20, 124 );		glVertex2i ( button->x, button->y );
		glColor3ub ( 8, 20, 0 );		glVertex2i ( button->x + button->width, button->y );
		glColor3ub ( 8, 20, 124 );		glVertex2i ( button->x + button->width, button->y + button->height );
	glEnd ();

	glColor3ub ( 81, 138, 215 );
	border_draw ( button );


	UplinkAssert (button)

	int screenheight = app->GetOptions ()->GetOptionValue ( "graphics_screenheight" );
	glScissor ( button->x, screenheight - (button->y + button->height), button->width, button->height );	
	glEnable ( GL_SCISSOR_TEST );


    //
    // Draw the names
    
    for ( int i = 0; i < 7; ++i ) {

        int index = i + baseoffset;

        if ( ual->handles.ValidIndex (index) &&
            ual->names.ValidIndex (index) ) {

            char *handle = ual->handles.GetData (index);
            char *name = ual->names.GetData (index);

            UplinkAssert (handle)
            UplinkAssert (name)
            
            glColor3f ( 1.0f, 1.0f, 1.0f );
            GciDrawText ( button->x + 10, button->y + 15 + i * 20, handle );
        
            if      ( strcmp ( name, "Unknown" ) == 0 )     glColor3f ( 0.5f, 0.0f, 0.0f );
            else if ( strcmp ( name, "Encrypted" ) == 0 )   glColor3f ( 0.8f, 0.0f, 0.0f );
            else                                            glColor3f ( 1.0f, 0.0f, 0.0f );
            GciDrawText ( button->x + 100, button->y + 15 + i * 20, name );

        }

    }

    glDisable ( GL_SCISSOR_TEST );

}

void UplinkAgentList::SetTarget (UplinkObject *uo, const string &uos, int uoi )
{


}

void UplinkAgentList::CloseClick ( Button *button )
{

	int pid;
	char bname [64];
    istringstream stream(button->name);
    stream >> bname >> pid;

	SvbRemoveTask ( pid );

}

void UplinkAgentList::TitleClick ( Button *button )
{

	int pid;
	char bname [64];
    istringstream stream(button->name);
    stream >> bname >> pid;
    auto *ual = (UplinkAgentList *) SvbGetTask ( pid );
    UplinkAssert (ual)

	ual->followmouse = true;
	game->GetInterface ()->GetTaskManager ()->SetTargetProgram ( pid );

}


void UplinkAgentList::ScrollUpClick ( Button *button )
{

    --baseoffset;
    if ( baseoffset < 0 ) baseoffset = 0;

	int pid;
	char bname [64];
    istringstream stream(button->name);
    stream >> bname >> pid;
    auto *ual = (UplinkAgentList *) SvbGetTask ( pid );
    UplinkAssert (ual)

	string listname = "uplinkagentlist " + to_string(pid);
    EclDirtyButton ( listname );

}

void UplinkAgentList::ScrollDownClick ( Button *button )
{

	++baseoffset;

	int pid;
	char bname [64];
    istringstream stream(button->name);
    stream >> bname >> pid;
    auto *ual = (UplinkAgentList *) SvbGetTask ( pid );
    UplinkAssert (ual)

	string listname = "uplinkagentlist " + to_string(pid);
    EclDirtyButton ( listname );
    
}

void UplinkAgentList::UpdateLists ()
{

	// Build a list of agents sorted in handle alphabetical order
	//

	DArray <Person *> *allpeople = game->GetWorld ()->people.ConvertToDArray ();
	LList  <Person *> sorted;

	for ( int ip = 0; ip < allpeople->Size (); ++ip ) {
		if ( allpeople->ValidIndex (ip) ) {

			Person *p = allpeople->GetData (ip);
			UplinkAssert (p)

			if ( p->GetOBJECTID () == OID_AGENT ||
				 p->GetOBJECTID () == OID_PLAYER ) {

				bool inserted = false;

				for ( int is = 0; is < sorted.Size (); ++is ) {

					Person *s = sorted.GetData (is);
					UplinkAssert (s)

					if ( strcmp ( ((Agent *) p)->handle, ((Agent *)s)->handle ) <= 0 ) {

						sorted.PutDataAtIndex ( p, is );
						inserted = true;
						break;

					}

				}

				if ( !inserted ) sorted.PutDataAtEnd ( p );

			}

		}
	}

	delete allpeople;

    //
    // How much data does the player have

	int foundsize = 0;
    int numfiles = 10;
    char *datatitle = "Uplink_Agent_Data";
    int *filefound = new int [numfiles];            // 0 = not found, 1 = found, -1 = encrypted
	for ( int fi = 0; fi < numfiles; ++fi )
		filefound [fi] = 0;

    DataBank *databank = &game->GetWorld ()->GetPlayer ()->gateway.databank;
    UplinkAssert (databank)

	for ( int i = 0; i < databank->GetDataSize (); ++i ) {
		if ( databank->GetDataFile (i) ) {

			Data *thisfile = databank->GetDataFile (i);
			UplinkAssert (thisfile)
			
            if ( thisfile->title.find(datatitle) != string::npos ) {

				char unused [64];
				int thisfileindex;

				sscanf ( thisfile->title.c_str(), "%s %d.dat", unused, &thisfileindex );
            
                if ( thisfileindex >= 0 && thisfileindex < numfiles ) {
			
                    if ( thisfile->encrypted == 0 )
					    filefound [thisfileindex] = 1;

                    else
                        filefound [thisfileindex] = -1;

				}

			}

		}
	}


    //
    // Copy the handles and names into place

    handles.SetSize ( sorted.Size () );
    names.SetSize ( sorted.Size () );

    for ( int j = 0; j < sorted.Size (); ++j ) {

        Person *person = sorted.GetData (j);
        UplinkAssert (person)

		size_t handlesize = SIZE_PERSON_NAME;
        char *handle = new char [handlesize];
		size_t namesize = SIZE_PERSON_NAME;
        char *name = new char [namesize];

        UplinkStrncpy ( handle, ((Agent *) person)->handle, handlesize )

        if ( person == game->GetWorld ()->GetPlayer () ) {

            UplinkStrncpy ( name, "Unknown", namesize )

        }
        else { 

            int fileindex = (int) ( 10 * ((float) j / (float) sorted.Size()) );

            if ( fileindex >= 0 && fileindex < numfiles ) {

				if      ( filefound [fileindex] == 0 ) {      UplinkStrncpy ( name, "Unknown", namesize )
				} else if ( filefound [fileindex] == -1 ) {   UplinkStrncpy ( name, "Encrypted", namesize )
				} else if ( filefound [fileindex] == 1 ) {    UplinkStrncpy ( name, person->name, namesize )
				}

            }

        }
         
        handles.PutData ( handle, j );
        names.PutData ( name, j );

    }

}

void UplinkAgentList::CreateInterface ()
{

	if ( !IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );

        int x = 100;
        int y = 100;
        int width = 250;
        int height = 150;

        //
        // Create the list

		string bname = "uplinkagentlist " + to_string(pid);
	
		EclRegisterButton ( x, y, width, height, "", "", bname );
		EclRegisterButtonCallbacks ( bname, UplinkAgentListDraw, nullptr, button_click, button_highlight );
		EclRegisterCaptionChange ( bname, "Uplink Agent List" );

        //
        // Title bar

        string titlename = "uplinkagentlist_title " + to_string(pid);
        EclRegisterButton ( x, y - 14, width, 14, "Uplink Agent List", "Click to move this tool", titlename );
        EclRegisterButtonCallback ( titlename, TitleClick );

        
        //
        // Create the close button

		string closename = "uplinkagentlist_close " + to_string(pid);

		EclRegisterButton ( width + x + 1, y - 14, 13, 13, "X", "Close the Uplink Agent List", closename );
		button_assignbitmaps ( closename, "close.tif", "close_h.tif", "close_c.tif" );
		EclRegisterButtonCallback ( closename, CloseClick );
		
        //
        // Scroll bar

	    string name_up = "uplinkagentlist_scrollup " + to_string(pid);
        string name_down = "uplinkagentlist_scrolldown " + to_string(pid);
        string name_bar = "uplinkagentlist_scrollbar " + to_string(pid);
        
	    EclRegisterButton ( width + x, y, 15, 15, "^", "Scroll up", name_up );
	    EclRegisterButton ( width + x, y + height - 15, 15, 15, "v", "Scroll down", name_down );
	    EclRegisterButton ( width + x, y + 15, 15, height - 30, "", name_bar );

	    button_assignbitmaps ( name_up, "up.tif", "up_h.tif", "up_c.tif" );
	    button_assignbitmaps ( name_down, "down.tif", "down_h.tif", "down_c.tif" );

	    EclRegisterButtonCallback  ( name_up, ScrollUpClick );
	    EclRegisterButtonCallback  ( name_down, ScrollDownClick );
	    EclRegisterButtonCallbacks ( name_bar, button_draw, nullptr, nullptr, nullptr );
        
        //
        // Update the lists

        UpdateLists ();


        //
        // Trigger the offer of money in 7 days

        ScriptLibrary::RunScript ( 70 );
        
	}

}

void UplinkAgentList::MoveTo ( int x, int y, int time_ms )
{

	int pid = SvbLookupPID ( this );

    int width = 250;
    int height = 150;

	string bname = "uplinkagentlist " + to_string(pid);
	EclRegisterMovement ( bname, x, y, time_ms );

	string closename = "uplinkagentlist_close " + to_string(pid);
    EclRegisterMovement ( closename, x + width + 1, y - 14, time_ms );
	
    string titlename = "uplinkagentlist_title " + to_string(pid);
    EclRegisterMovement ( titlename, x, y - 14, time_ms );

    string name_up =   "uplinkagentlist_scrollup " + to_string(pid);
    string name_down = "uplinkagentlist_scrolldown " + to_string(pid);
    string name_bar =  "uplinkagentlist_scrollbar " + to_string(pid);
    
    EclRegisterMovement ( name_up, x + width, y, time_ms );
    EclRegisterMovement ( name_down, x + width, y + height - 15, time_ms );
    EclRegisterMovement ( name_bar, x + width, y + 15, time_ms );

}

void UplinkAgentList::RemoveInterface ()
{

	if ( IsInterfaceVisible () ) {

		int pid = SvbLookupPID ( this );

		string bname = "uplinkagentlist " + to_string(pid);
		EclRemoveButton ( bname );

		string closename = "uplinkagentlist_close " + to_string(pid);
		EclRemoveButton ( closename );

        string titlename = "uplinkagentlist_title " + to_string(pid);
        EclRemoveButton ( titlename );

        string name_up =   "uplinkagentlist_scrollup " + to_string(pid);
        string name_down = "uplinkagentlist_scrolldown " + to_string(pid);
        string name_bar =  "uplinkagentlist_scrollbar " + to_string(pid);
                
        EclRemoveButton ( name_up );
        EclRemoveButton ( name_down );
        EclRemoveButton ( name_bar );

	}

}

void UplinkAgentList::ShowInterface ()
{

	if ( !IsInterfaceVisible () ) CreateInterface ();

	int pid = SvbLookupPID ( this );

	string bname = "uplinkagentlist " + to_string(pid);
	EclButtonBringToFront ( bname );

	string closename = "uplinkagentlist_close " + to_string(pid);
	EclButtonBringToFront ( closename );

    string titlename = "uplinkagentlist_title " + to_string(pid);
    EclButtonBringToFront ( titlename );

    string name_up =   "uplinkagentlist_scrollup " + to_string(pid);
    string name_down = "uplinkagentlist_scrolldown " + to_string(pid);
    string name_bar =  "uplinkagentlist_scrollbar " + to_string(pid);

    EclButtonBringToFront ( name_up );
   	EclButtonBringToFront ( name_down );
    EclButtonBringToFront ( name_bar );

}

bool UplinkAgentList::IsInterfaceVisible ()
{

	string bname = "uplinkagentlist " + to_string( SvbLookupPID (this) );

	return ( EclGetButton ( bname ) != nullptr );

}

