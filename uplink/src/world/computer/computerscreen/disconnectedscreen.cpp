
#include <app/miscutils.h>
#include "app/globals.h"
#include "app/serialise.h"

#include "world/vlocation.h"
#include "world/computer/computerscreen/disconnectedscreen.h"

#include "mmgr.h"

DArray <char *> DisconnectedScreen::loginslost;


DisconnectedScreen::DisconnectedScreen()
{
	
	nextpage = -1;
	textmessage = nullptr;

}

DisconnectedScreen::~DisconnectedScreen()
{
	
	delete [] textmessage;

}

void DisconnectedScreen::SetNextPage ( int newnextpage )
{

	nextpage = newnextpage;

}

void DisconnectedScreen::SetTextMessage (const string &newtextmessage )
{

	delete [] textmessage;
	textmessage = new char [newtextmessage.length()+1];
	UplinkSafeStrcpy ( textmessage, newtextmessage.c_str() )

}

void DisconnectedScreen::AddLoginLost ( char *ip )
{

	size_t ipcopysize = SIZE_VLOCATION_IP;
    char *ipcopy = new char [ipcopysize];
    UplinkStrncpy ( ipcopy, ip, ipcopysize )
    loginslost.PutData ( ipcopy );

}

void DisconnectedScreen::ClearLoginsLost ()
{

    DeleteDArrayData ( &loginslost );
    loginslost.Empty ();

}

bool DisconnectedScreen::Load  ( FILE *file )
{

	LoadID ( file );

	if ( !ComputerScreen::Load ( file ) ) return false;

	if ( !FileReadData ( &nextpage, sizeof(nextpage), 1, file ) ) return false;	
	if ( !LoadDynamicStringPtr ( &textmessage, file ) ) return false;

	LoadID_END ( file );

	return true;

}

void DisconnectedScreen::Save  ( FILE *file )
{

	SaveID ( file );

	ComputerScreen::Save ( file );

	fwrite ( &nextpage, sizeof(nextpage), 1, file );
	SaveDynamicString ( textmessage, file );

	SaveID_END ( file );

}

void DisconnectedScreen::Print ()
{

    cout << "DisconnectedScreen : " << endl;
    ComputerScreen::Print ();
    PrintValue("NextPage", nextpage);
    PrintValue("TextMessage", textmessage);

}
	
string DisconnectedScreen::GetID ()
{

	return "SCR_DISC";

}

int DisconnectedScreen::GetOBJECTID ()
{

	return OID_DISCONNECTEDSCREEN;

}
			

