
#include <app/miscutils.h>
#include "app/serialise.h"

#include "world/computer/computerscreen/cypherscreen.h"

#include "mmgr.h"


CypherScreen::CypherScreen ()
{

	nextpage = -1;
	difficulty = 1;

}

CypherScreen::~CypherScreen ()
= default;

void CypherScreen::SetNextPage ( int newnextpage )
{
	
	nextpage = newnextpage;

}

void CypherScreen::SetDifficulty ( int newdifficulty )
{

	difficulty = newdifficulty;

}

bool CypherScreen::Load  ( FILE *file )
{

	LoadID ( file );

	if ( !ComputerScreen::Load ( file ) ) return false;

	if ( !FileReadData ( &nextpage, sizeof(nextpage), 1, file ) ) return false;
	if ( !FileReadData ( &difficulty, sizeof(difficulty), 1, file ) ) return false;

	LoadID_END ( file );

	return true;

}

void CypherScreen::Save  ( FILE *file )
{

	SaveID ( file );

	ComputerScreen::Save ( file );

	fwrite ( &nextpage, sizeof(nextpage), 1, file );
	fwrite ( &difficulty, sizeof(difficulty), 1, file );

	SaveID_END ( file );

}

void CypherScreen::Print ()
{

    cout << "CypherScreen : " << endl;
	ComputerScreen::Print ();
    PrintValue("NextPage", nextpage);
    PrintValue("Difficulty", difficulty);

}

string CypherScreen::GetID ()
{

	return "SCR_CYPH";

}

int CypherScreen::GetOBJECTID ()
{

	return OID_CYPHERSCREEN;

}
