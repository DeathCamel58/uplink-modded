// DialogScreen.cpp: implementation of the DialogScreen class.
//
//////////////////////////////////////////////////////////////////////

#include <app/miscutils.h>
#include "gucci.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/serialise.h"

#include "game/game.h"

#include "interface/interface.h"
#include "interface/remoteinterface/remoteinterface.h"
#include "interface/remoteinterface/dialogscreen_interface.h"

#include "world/computer/computerscreen/dialogscreen.h"

#include "mmgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DialogScreen::DialogScreen()
{

	returnkeybutton = nullptr;
    escapekeybutton = nullptr;

}

DialogScreen::~DialogScreen()
{
	
	DeleteLListData ( (LList <UplinkObject *> *) &widgets );

    delete [] returnkeybutton;
    delete [] escapekeybutton;

}

void DialogScreen::AddWidget (const string &name, int WIDGETTYPE, int x, int y, int width, int height, const string &caption, const string &tooltip )
{

	auto *dsw = new DialogScreenWidget ();
	
	dsw->SetName ( name );
	dsw->SetTYPE ( WIDGETTYPE );
	dsw->SetPosition ( x, y );
	dsw->SetSize ( width, height );
	dsw->SetCaption ( caption );
	dsw->SetTooltip ( tooltip );

	widgets.PutData ( dsw );

}

void DialogScreen::AddWidget (const string &name, int WIDGETTYPE, int x, int y, int width, int height, const string &caption, const string &tooltip,
                              int data1, int data2, const string &stringdata1, const string &stringdata2 )
{

	auto *dsw = new DialogScreenWidget ();
	
	dsw->SetName ( name );
	dsw->SetTYPE ( WIDGETTYPE );
	dsw->SetPosition ( x, y );
	dsw->SetSize ( width, height );
	dsw->SetCaption ( caption );
	dsw->SetTooltip ( tooltip );

	dsw->SetData ( data1, data2 );
	dsw->SetStringData ( stringdata1, stringdata2 );

	widgets.PutData ( dsw );

}

void DialogScreen::RemoveWidget (const string &name )
{

	for ( int i = 0; i < widgets.Size (); ++i ) {
		
		DialogScreenWidget *dsw = widgets.GetData (i);
		UplinkAssert (dsw)

		if ( dsw->GetName () == name ) {

			ComputerScreen *cs = game->GetInterface ()->GetRemoteInterface ()->GetComputerScreen ();

			if ( cs == this )
				DialogScreenInterface::RemoveWidget ( dsw, GetComputer () );

			widgets.RemoveData (i);
            delete dsw;
			return;
		}

	}

}

void DialogScreen::SetReturnKeyButton (const string &name )
{

	delete [] returnkeybutton;
	returnkeybutton = new char [name.length()+1];
	UplinkSafeStrcpy ( returnkeybutton, name.c_str() )
	
}

void DialogScreen::SetEscapeKeyButton (const string &name )
{

    delete [] escapekeybutton;
    escapekeybutton = new char [name.length()+1];
    UplinkSafeStrcpy ( escapekeybutton, name.c_str() )

}

bool DialogScreen::Load ( FILE *file )
{
	
	LoadID ( file );

	if ( !ComputerScreen::Load ( file ) ) return false;

	if ( !LoadLList ( (LList <UplinkObject *> *) &widgets, file ) ) return false;

	if ( !LoadDynamicStringPtr ( &returnkeybutton, file ) ) return false;
    if ( !LoadDynamicStringPtr ( &escapekeybutton, file ) ) return false;

	LoadID_END ( file );

	return true;

}

void DialogScreen::Save ( FILE *file )
{

	SaveID ( file );

	ComputerScreen::Save ( file );

	SaveLList ( (LList <UplinkObject *> *) &widgets, file );

	SaveDynamicString ( returnkeybutton, file );
    SaveDynamicString ( escapekeybutton, file );

	SaveID_END ( file );

}

void DialogScreen::Print ()
{

	cout << "DialogScreen : " << endl;
	ComputerScreen::Print ();
	PrintLList ( (LList <UplinkObject *> *) &widgets );
	PrintValue( "ReturnKeyButton", returnkeybutton );
    PrintValue( "EscapeKeyButton", escapekeybutton );

}
	
string DialogScreen::GetID ()
{
	
	return "SCR_DLG";

}

int DialogScreen::GetOBJECTID ()
{

	return OID_DIALOGSCREEN;

}			


// ============================================================================
//  Dialog Widget


DialogScreenWidget::DialogScreenWidget ()
{

	x = y = width = height = 0;
	name = caption = tooltip = nullptr;
	WIDGETTYPE = WIDGET_NONE;

	data1 = data2 = 0;
	stringdata1 = stringdata2 = nullptr;

}

DialogScreenWidget::~DialogScreenWidget ()
{

    delete [] name;
	delete [] caption;
	delete [] tooltip;

	delete [] stringdata1;
	delete [] stringdata2;

}

void DialogScreenWidget::SetName (const string &newname )
{

	delete [] name;
	name = new char [newname.length()+1];
	UplinkSafeStrcpy ( name, newname.c_str() )

}

void DialogScreenWidget::SetTYPE ( int NEWWIDGETTYPE )
{

	WIDGETTYPE = NEWWIDGETTYPE;

}

void DialogScreenWidget::SetPosition ( int newx, int newy )
{
	
	x = newx;
	y = newy;

}

void DialogScreenWidget::SetSize ( int newwidth, int newheight )
{

	width = newwidth;
	height = newheight;

}

void DialogScreenWidget::SetTooltip (const string &newtooltip )
{

	delete [] tooltip;
	tooltip = new char [newtooltip.length() + 1];
	UplinkSafeStrcpy ( tooltip, newtooltip.c_str() )

}

void DialogScreenWidget::SetCaption (const string &newcaption )
{

	delete [] caption;
	caption = new char [newcaption.length() + 1];
	UplinkSafeStrcpy ( caption, newcaption.c_str() )

}
	
char *DialogScreenWidget::GetName ()
{

	return name;

}

char *DialogScreenWidget::GetTooltip ()
{

	return tooltip;

}

char *DialogScreenWidget::GetCaption ()
{

	return caption;

}


void DialogScreenWidget::SetData ( int newdata1, int newdata2 )
{

	data1 = newdata1;
	data2 = newdata2;

}

void DialogScreenWidget::SetStringData (const string &newstringdata1, const string &newstringdata2 )
{

	delete [] stringdata1;
	stringdata1 = nullptr;
	
	if ( !newstringdata1.empty() ) {
		stringdata1 = new char [newstringdata1.length() + 1];
		UplinkSafeStrcpy ( stringdata1, newstringdata1.c_str() )
	}
	
	delete [] stringdata2;
	stringdata2 = nullptr;

	if ( !newstringdata2.empty() ) {
		stringdata2 = new char [newstringdata2.length() + 1];
		UplinkSafeStrcpy ( stringdata2, newstringdata2.c_str() )
	}

}

char *DialogScreenWidget::GetStringData1 ()
{

	return stringdata1;

}

char *DialogScreenWidget::GetStringData2 ()
{

	return stringdata2;

}

bool DialogScreenWidget::Load ( FILE *file )
{
	
	LoadID ( file );

	if ( !LoadDynamicStringPtr ( &name, file ) ) return false;

	if ( !FileReadData ( &x, sizeof(x), 1, file ) ) return false;
	if ( !FileReadData ( &y, sizeof(y), 1, file ) ) return false;
	if ( !FileReadData ( &width, sizeof(width), 1, file ) ) return false;
	if ( !FileReadData ( &height, sizeof(height), 1, file ) ) return false;
	if ( !FileReadData ( &WIDGETTYPE, sizeof(WIDGETTYPE), 1, file ) ) return false;

	if ( !LoadDynamicStringPtr ( &caption, file ) ) return false;
	if ( !LoadDynamicStringPtr ( &tooltip, file ) ) return false;

	if ( !FileReadData ( &data1, sizeof(data1), 1, file ) ) return false;
	if ( !FileReadData ( &data2, sizeof(data2), 1, file ) ) return false;

	if ( !LoadDynamicStringPtr ( &stringdata1, file ) ) return false;
	if ( !LoadDynamicStringPtr ( &stringdata2, file ) ) return false;
	
	LoadID_END ( file );

	return true;

}

void DialogScreenWidget::Save ( FILE *file )
{

	SaveID ( file );

	SaveDynamicString ( name, file );

	fwrite ( &x, sizeof(x), 1, file );
	fwrite ( &y, sizeof(y), 1, file );
	fwrite ( &width, sizeof(width), 1, file );
	fwrite ( &height, sizeof(height), 1, file );
	fwrite ( &WIDGETTYPE, sizeof(WIDGETTYPE), 1, file );

	SaveDynamicString ( caption, file );
	SaveDynamicString ( tooltip, file );

	fwrite ( &data1, sizeof(data1), 1, file );
	fwrite ( &data2, sizeof(data2), 1, file );

	SaveDynamicString ( stringdata1, file );
	SaveDynamicString ( stringdata2, file );
		
	SaveID_END ( file );

}

void DialogScreenWidget::Print ()
{

	cout << "DialogScreenWidget : " << endl;
    PrintValue("Name", name );
    PrintValue("TYPE", WIDGETTYPE );
    PrintValue("X", x );
    PrintValue("Y", y );
    PrintValue("Width", width );
    PrintValue("Height", height );
    PrintValue("Caption", caption );
    PrintValue("Tooltip", tooltip );
    PrintValue("Data1", data1 );
    PrintValue("Data2", data2 );
    PrintValue("Stringdata1", stringdata1 );
    PrintValue("Stringdata2", stringdata2 );

}
	
string DialogScreenWidget::GetID ()
{

	return "DLG_WIDG";

}

int DialogScreenWidget::GetOBJECTID ()
{

	return OID_DIALOGSCREENWIDGET;

}
			
