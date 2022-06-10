
#include "gucci.h"

#include "app/globals.h"

#include "world/scheduler/uplinkevent.h"

#include "mmgr.h"


UplinkEvent::UplinkEvent ()
= default;

UplinkEvent::~UplinkEvent ()
= default;
	
void UplinkEvent::SetRunDate ( Date *newrundate )
{

	UplinkAssert (newrundate)
	rundate.SetDate ( newrundate );

}
	
void UplinkEvent::Run ()
{

	UplinkAbort ( "This MUST be overridden" )

}

void UplinkEvent::RunWarning ()
{
}

string UplinkEvent::GetShortString ()
{

	return "Uplink Event";
		
}

string UplinkEvent::GetLongString ()
{

	return "Uplink Event";

}

bool UplinkEvent::Load  ( FILE *file )
{

	LoadID ( file );
	
	if ( !rundate.Load ( file ) ) return false;

	LoadID_END ( file );

	return true;
		
}

void UplinkEvent::Save  ( FILE *file )
{

	SaveID ( file );

	rundate.Save ( file );

	SaveID_END ( file );

}

void UplinkEvent::Print ()
{

	cout << "UplinkEvent : " << endl;
	rundate.Print ();

}
	
string UplinkEvent::GetID ()
{

	UplinkAbort ( "This MUST be overridden" )
	return "";

}

int UplinkEvent::GetOBJECTID ()
{

	UplinkAbort ( "This MUST be overridden" )
	return -1;

}
