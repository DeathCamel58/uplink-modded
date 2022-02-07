// VLocation.cpp: implementation of the VLocation class.
//
//////////////////////////////////////////////////////////////////////

#include <app/miscutils.h>
#include "gucci.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/serialise.h"

#include "game/game.h"

#include "world/world.h"
#include "world/vlocation.h"
#include "world/computer/computer.h"

#include "mmgr.h"


VLocation::VLocation()
{
	
	UplinkStrncpy ( ip, "0.0.0.0", sizeof ( ip ) )
	UplinkStrncpy ( computer, "", sizeof ( computer ) )

	x = y = 0;
	listed = true;
    displayed = true;
	colored = false;

}

VLocation::~VLocation()
= default;

void VLocation::SetIP (const string &newip )
{

	assert( newip.length() < SIZE_VLOCATION_IP );
	UplinkStrncpy ( ip, newip.c_str(), sizeof ( ip ) )

}

bool VLocation::VerifyIP (const string &newip )
{

	if (newip.empty() || newip.length() >= SIZE_VLOCATION_IP) {

		return false;

	}

	return true;

}

void VLocation::SetComputer (const string &newcomputer )
{

	assert( newcomputer.length() < SIZE_COMPUTER_NAME );
	UplinkStrncpy ( computer, newcomputer.c_str(), sizeof ( computer ) )

//	UplinkAssert ( game->GetWorld ()->GetComputer ( computer ) );

}

void VLocation::SetPLocation ( int newx, int newy )
{

	assert( newx >= 0 && newx < VIRTUAL_WIDTH );
	assert( newy >= 0 && newy < VIRTUAL_HEIGHT );

	x = newx;
	y = newy;

}

bool VLocation::VerifyPLocation ( int newx, int newy )
{

	if ( !( newx >= 0 && newx < VIRTUAL_WIDTH ) || 
	     !( newy >= 0 && newy < VIRTUAL_HEIGHT ) ) {

		return false;

	}

	return true;

}

void VLocation::SetListed ( bool newlisted )
{

	listed = newlisted;

}

void VLocation::SetDisplayed ( bool newdisplayed )
{

    displayed = newdisplayed;

}

void VLocation::SetColorDisplayed ( bool newcolored )
{

	colored = newcolored;

}

Computer *VLocation::GetComputer ()
{
/*
	Computer *comp = game->GetWorld ()->GetComputer ( computer );
	
	if ( comp )
		return comp;

	else
		return nullptr;
*/


	BTree <Computer *> *tree = &(game->GetWorld()->computers);
    
    /*  Remember there is no rule that states computer names have to be unique.
     *  eg "CompanyName Access Terminal" - there can be many.
     *  We must now continue searching until we find a computer that matches
     *  both the name and the IP of this VLocation.
     *
     *  Also remember that this might be a Dialup Number - in which case its IP
     * (telephone number) will definately NOT match the IP of the computer.
     *
     */
     
    while(tree)
    {
        tree = tree->LookupTree( computer );

        if( tree )
        {
            Computer *comp = tree->data;
            if( comp && strcmp( comp->ip, ip ) == 0 ) 
            {
                return comp;
            }
            else if( comp && 
                     comp->TYPE == COMPUTER_TYPE_LAN && 
                     GetOBJECTID() == OID_VLOCATIONSPECIAL )
            {
                return comp;
            }
            else 
            {
                // Copies with identical names will always be on the left branch
                tree = tree->Left();
            }
        }
    }

    return nullptr;

}

bool VLocation::Load ( FILE *file )
{

	LoadID ( file );

	if ( !LoadDynamicStringStatic ( ip, SIZE_VLOCATION_IP, file ) ) return false;
	if ( !LoadDynamicStringStatic ( computer, SIZE_COMPUTER_NAME, file ) ) return false;

	if ( !FileReadData ( &x, sizeof (x), 1, file ) ) return false;
	if ( !FileReadData ( &y, sizeof (y), 1, file ) ) return false;
	if ( !FileReadData ( &listed, sizeof(listed), 1, file ) ) return false;
	if ( !FileReadData ( &displayed, sizeof(displayed), 1, file ) ) return false;

	if ( strcmp( game->GetLoadedSavefileVer(), "SAV60" ) >= 0 ) {
		if ( !FileReadData ( &colored, sizeof(colored), 1, file ) ) return false;
	}
    
	LoadID_END ( file );

	return true;

}

void VLocation::Save ( FILE *file )
{

	SaveID ( file );

	SaveDynamicString ( ip, SIZE_VLOCATION_IP, file );
	SaveDynamicString ( computer, SIZE_COMPUTER_NAME, file );

	fwrite ( &x, sizeof (x), 1, file );
	fwrite ( &y, sizeof (y), 1, file );
	fwrite ( &listed, sizeof(listed), 1, file );
    fwrite ( &displayed, sizeof(displayed), 1, file );

    fwrite ( &colored, sizeof(colored), 1, file );

	SaveID_END ( file );

}

void VLocation::Print ()
{

    cout << "VLocation :" << endl;
    PrintValue("IP", ip);
    PrintValue("Computer", computer);
    PrintValue("X", x);
    PrintValue("Y", y);
    PrintValue("Listed", listed);
    PrintValue("Displayed", displayed);
    PrintValue("Colored", colored);

}

string VLocation::GetID ()
{

	return "VIRTLOC";

}

int VLocation::GetOBJECTID ()
{

	return OID_VLOCATION;

}


VLocationSpecial::VLocationSpecial () : VLocation ()
{
    screenIndex = 0;
    securitySystemIndex = -1;
}


VLocationSpecial::~VLocationSpecial ()
= default;

void VLocationSpecial::SetScreenIndex ( int newScreenIndex )
{
    screenIndex = newScreenIndex;
}

void VLocationSpecial::SetSecuritySystemIndex ( int newIndex )
{
    securitySystemIndex = newIndex;
}

bool VLocationSpecial::Load  ( FILE *file )
{
    LoadID ( file );

    if ( !VLocation::Load ( file ) ) return false;

    if ( !FileReadData ( &screenIndex, sizeof(screenIndex), 1, file ) ) return false;
    if ( !FileReadData ( &securitySystemIndex, sizeof(securitySystemIndex), 1, file ) ) return false;

    LoadID_END ( file );

	return true;
}

void VLocationSpecial::Save  ( FILE *file )
{
    SaveID ( file );

    VLocation::Save ( file );

    fwrite ( &screenIndex, sizeof(screenIndex), 1, file );
    fwrite ( &securitySystemIndex, sizeof(securitySystemIndex), 1, file );

    SaveID_END ( file );
}

void VLocationSpecial::Print ()
{
    cout << "VLocationSpecial:" << endl;
    PrintValue("ScreenIndex", screenIndex);
    PrintValue("SecuritySystemIndex", securitySystemIndex);

    VLocation::Print ();
}
	
string VLocationSpecial::GetID ()
{
    return "VLOCSPEC";
}

int VLocationSpecial::GetOBJECTID ()
{
    return OID_VLOCATIONSPECIAL;
}

