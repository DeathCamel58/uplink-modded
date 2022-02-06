
#include <app/miscutils.h>
#include "app/serialise.h"

#include "interface/remoteinterface/remoteinterface.h"

#include "world/computer/computerscreen/linksscreen.h"

#include "mmgr.h"



LinksScreen::LinksScreen ()
{
	
	nextpage = -1;
	SCREENTYPE = LINKSSCREENTYPE_NONE;

}

LinksScreen::~LinksScreen ()
= default;

void LinksScreen::SetNextPage ( int newnextpage )
{
	
	nextpage = newnextpage;

}

void LinksScreen::SetScreenType ( int newSCREEN_TYPE )
{
	
	SCREENTYPE = newSCREEN_TYPE;

}

bool LinksScreen::Load ( FILE *file )
{

	LoadID ( file );

	if ( !ComputerScreen::Load ( file ) ) return false;

	if ( !FileReadData ( &nextpage, sizeof(nextpage), 1, file ) ) return false;
	if ( !FileReadData ( &SCREENTYPE, sizeof(SCREENTYPE), 1, file ) ) return false;

	LoadID_END ( file );

	return true;

}

void LinksScreen::Save ( FILE *file )
{

	SaveID ( file );

	ComputerScreen::Save ( file );

	fwrite ( &nextpage, sizeof(nextpage), 1, file );
	fwrite ( &SCREENTYPE, sizeof(SCREENTYPE), 1, file );

	SaveID_END ( file );

}

void LinksScreen::Print ()
{

    cout << "LinksScreen :" << endl;
    ComputerScreen::Print ();
    PrintValue("Type", SCREENTYPE);
    PrintValue("NextPage", nextpage);

}
	
string LinksScreen::GetID ()
{
	
	return "SCR_LINK";

}

int LinksScreen::GetOBJECTID ()
{

	return OID_LINKSSCREEN;

}

