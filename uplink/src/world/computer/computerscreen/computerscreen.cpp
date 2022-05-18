// ComputerScreen.cpp: implementation of the ComputerScreen class.
//
//////////////////////////////////////////////////////////////////////

#include <app/miscutils.h>

#include "game/game.h"

#include "app/globals.h"
#include "app/serialise.h"

#include "world/world.h"
#include "world/computer/computerscreen/computerscreen.h"

#include "mmgr.h"



ComputerScreen::ComputerScreen()
{

	UplinkStrncpy ( maintitle, " ", sizeof ( maintitle ) )
	UplinkStrncpy ( subtitle, " ", sizeof ( subtitle ) )
	UplinkStrncpy ( computer, " ", sizeof ( computer ) )

}

ComputerScreen::~ComputerScreen()
= default;

void ComputerScreen::SetMainTitle (const string &newmaintitle )
{

	assert( newmaintitle.length() < SIZE_COMPUTERSCREEN_MAINTITLE );
	UplinkStrncpy ( maintitle, newmaintitle.c_str(), sizeof ( maintitle ) )

}

void ComputerScreen::SetSubTitle (const string &newsubtitle )
{

	assert( newsubtitle.length() < SIZE_COMPUTERSCREEN_SUBTITLE );
	UplinkStrncpy ( subtitle, newsubtitle.c_str(), sizeof ( subtitle ) )

}

void ComputerScreen::SetComputer (const string &newcomputer )
{

	assert( newcomputer.length() < SIZE_COMPUTER_NAME );
	UplinkStrncpy ( computer, newcomputer.c_str(), sizeof ( computer ) )

}

Computer *ComputerScreen::GetComputer ()
{

	return game->GetWorld ()->GetComputer ( computer );

}

bool ComputerScreen::Load  ( FILE *file )
{

	LoadID ( file );

	if ( !LoadDynamicStringStatic ( maintitle, SIZE_COMPUTERSCREEN_MAINTITLE, file ) ) return false;
	if ( !LoadDynamicStringStatic ( subtitle, SIZE_COMPUTERSCREEN_SUBTITLE, file ) ) return false;
	if ( !LoadDynamicStringStatic ( computer, SIZE_COMPUTER_NAME, file ) ) return false;

	LoadID_END ( file );

	return true;

}

void ComputerScreen::Save  ( FILE *file )
{

	SaveID ( file );

	SaveDynamicString ( maintitle, SIZE_COMPUTERSCREEN_MAINTITLE, file );
	SaveDynamicString ( subtitle, SIZE_COMPUTERSCREEN_SUBTITLE, file );
	SaveDynamicString ( computer, SIZE_COMPUTER_NAME, file );

	SaveID_END ( file );

}

void ComputerScreen::Print ()
{

    cout << "Computer Screen :" << endl;
    PrintValue("MainTitle", maintitle);
    PrintValue("Subtitle", subtitle);
    PrintValue("Computer", computer);

}

void ComputerScreen::Update ()
{
}

string ComputerScreen::GetID ()
{

	return "COMPSCR";

}

int ComputerScreen::GetOBJECTID ()
{
	
	return OID_UNKNOWN;

}

