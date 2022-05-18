// MenuScreen.cpp: implementation of the MenuScreen class.
//
//////////////////////////////////////////////////////////////////////

#include <app/miscutils.h>

#include "app/app.h"
#include "app/globals.h"
#include "app/serialise.h"

#include "world/computer/computerscreen/menuscreen.h"

#include "mmgr.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MenuScreen::MenuScreen()
= default;

MenuScreen::~MenuScreen()
{
    
    DeleteLListData ( (LList <UplinkObject *> *) &options );

}

void MenuScreen::AddOption (const string &caption, const string &tooltip, int nextpage, int security, int index )
{

	auto *mso = new MenuScreenOption ();
	mso->SetCaption ( caption );
	mso->SetTooltip ( tooltip );
	mso->SetNextPage ( nextpage );
	mso->SetSecurity ( security );

	if ( index == -1 )
		options.PutData ( mso );

	else
		options.PutDataAtIndex ( mso, index );

}

int MenuScreen::NumOptions  ()
{

	return options.Size ();

}

char *MenuScreen::GetCaption ( int index )
{

	UplinkAssert ( options.ValidIndex ( index ) )

	return options.GetData (index)->caption;

}

char *MenuScreen::GetTooltip ( int index )
{

	UplinkAssert ( options.ValidIndex ( index ) )

	return options.GetData (index)->tooltip;
	
}

int MenuScreen::GetNextPage ( int index )
{

	UplinkAssert ( options.ValidIndex ( index ) )

	return options.GetData (index)->nextpage;

}

int MenuScreen::GetSecurity ( int index )
{

	UplinkAssert ( options.ValidIndex ( index ) )

	return options.GetData (index)->security;

}

bool MenuScreen::Load ( FILE *file )
{

	LoadID ( file );

	if ( !ComputerScreen::Load ( file ) ) return false;

	if ( !LoadLList ( (LList <UplinkObject *> *) &options, file ) ) return false;

	LoadID_END ( file );

	return true;

}

void MenuScreen::Save ( FILE *file )
{

	SaveID ( file );

	ComputerScreen::Save ( file );

	SaveLList ( (LList <UplinkObject *> *) &options, file );

	SaveID_END ( file );

}

void MenuScreen::Print ()
{

	cout << "MenuScreen : " << endl;
	
	ComputerScreen::Print ();
	
	PrintLList ( (LList <UplinkObject *> *) &options );

}

string MenuScreen::GetID ()
{

	return "SCR_MENU";

}

int MenuScreen::GetOBJECTID ()
{

	return OID_MENUSCREEN;

}



// ============================================================================



MenuScreenOption::MenuScreenOption ()
{

	UplinkStrncpy ( caption, "", sizeof ( caption ) )
	UplinkStrncpy ( tooltip, "", sizeof ( tooltip ) )
	nextpage = -1;
	security = 10;

}

MenuScreenOption::~MenuScreenOption ()
= default;

void MenuScreenOption::SetCaption (const string &newcaption )
{

	assert( newcaption.length() < SIZE_MENUSCREENOPTION_CAPTION );
	UplinkStrncpy ( caption, newcaption.c_str(), sizeof ( caption ) )

}

void MenuScreenOption::SetTooltip (const string &newtooltip )
{

	assert( newtooltip.length() < SIZE_MENUSCREENOPTION_TOOLTIP );
	UplinkStrncpy ( tooltip, newtooltip.c_str(), sizeof ( tooltip ) )

}

void MenuScreenOption::SetNextPage ( int newnextpage )
{

	nextpage = newnextpage;

}

void MenuScreenOption::SetSecurity ( int newsecurity )
{

	security = newsecurity;

}

bool MenuScreenOption::Load ( FILE *file )
{

	LoadID ( file );

	if ( !LoadDynamicStringStatic ( caption, SIZE_MENUSCREENOPTION_CAPTION, file ) ) return false;
	if ( !LoadDynamicStringStatic ( tooltip, SIZE_MENUSCREENOPTION_TOOLTIP, file ) ) return false;

	if ( !FileReadData ( &nextpage, sizeof(nextpage), 1, file ) ) return false;
	if ( !FileReadData ( &security, sizeof(security), 1, file ) ) return false;

	LoadID_END ( file );

	return true;

}

void MenuScreenOption::Save ( FILE *file )
{

	SaveID ( file );

	SaveDynamicString ( caption, SIZE_MENUSCREENOPTION_CAPTION, file );
	SaveDynamicString ( tooltip, SIZE_MENUSCREENOPTION_TOOLTIP, file );

	fwrite ( &nextpage, sizeof(nextpage), 1, file );
	fwrite ( &security, sizeof(security), 1, file );

	SaveID_END ( file );

}

void MenuScreenOption::Print ()
{

    cout << "MenuScreenOption : " << endl;
    PrintValue("NextPage", nextpage);
    PrintValue("Caption", caption);
    PrintValue("Tooltip", tooltip);
    PrintValue("Security", security);

}

void MenuScreenOption::Update ()
{
}

string MenuScreenOption::GetID ()
{

	return "MNUOPT";

}

int MenuScreenOption::GetOBJECTID ()
{

	return OID_MENUSCREENOPTION;

}

