// View.cpp: implementation of the View class.
//
//////////////////////////////////////////////////////////////////////

#ifdef WIN32
#include <windows.h>
#endif

#include <app/miscutils.h>

#include "view/view.h"

#include "mmgr.h"

View::View()
= default;

View::~View()
= default;

void View::Initialise ()
{

	fps.Start ();

}

void View::Draw ()
{

	fps.Tick ();

}


bool View::Load ( FILE *file )
{

	LoadID ( file );
	LoadID_END ( file );

	Initialise ();

	return true;

}

void View::Save ( FILE *file )
{

	SaveID ( file );
	SaveID_END ( file );

}

void View::Print ()
{

    PrintPadded("V I E W");
    PrintPadded("E N D  O F  V I E W");

}

string View::GetID ()
{

	return "VIEW";

}

void View::Update ()
{

}
