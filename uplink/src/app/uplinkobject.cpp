// UplinkObject.cpp: implementation of the UplinkObject class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"

#include "app/uplinkobject.h"
#include "app/serialise.h"

#include "mmgr.h"

#ifdef _DEBUG
#define  SAVEFILE_VERBOSE
#define  SAVEFILE_PRINTPROGRESS
#endif

static int indent;							// Used for formatting Load/Save
static void PrintIndent ();					//


UplinkObject::UplinkObject()
= default;

UplinkObject::~UplinkObject()
= default;

bool UplinkObject::Load ( FILE *file )
{
	return true;
}

void UplinkObject::Save ( FILE *file )
{
}

void UplinkObject::Print ()
{
}

void UplinkObject::Update ()
{
}

string UplinkObject::GetID ()
{
	return "UOBJ";
}

string UplinkObject::GetID_END ()
{

	string id_end = GetID() + "_END";
	return id_end;

}

int UplinkObject::GetOBJECTID ()
{

	return OID_UNKNOWN;

}

static void PrintIndent ()
{

	for ( int i = 0; i < indent-1; ++i )
		cout << "\t";

}

void UplinkObject::LoadID ( FILE *file )
{
	
#ifdef SAVEFILE_PRINTPROGRESS
	++indent;
	PrintIndent ();
	cout << "Started  loading  " << GetID () << endl;
#endif

#ifdef SAVEFILE_VERBOSE
	char id [SIZE_SAVEID];
	fread ( id, SIZE_SAVEID, 1, file );
	id [ sizeof(id) - 1 ] = '\0';
	assert( id == GetID () );
#endif

}

void UplinkObject::SaveID ( FILE *file )
{

#ifdef SAVEFILE_PRINTPROGRESS
	++indent;
	PrintIndent ();
	cout << "Started  saving " << GetID () << endl;
#endif

#ifdef SAVEFILE_VERBOSE
	fwrite ( GetID (), SIZE_SAVEID, 1, file );
#endif

}

void UplinkObject::LoadID_END ( FILE *file )
{

#ifdef SAVEFILE_PRINTPROGRESS
	PrintIndent ();
	cout << "Finished loading  " << GetID () << endl;
	--indent;
#endif

#ifdef SAVEFILE_VERBOSE
	char id_end [SIZE_SAVEID_END];
	fread ( id_end, SIZE_SAVEID_END, 1, file );
	id_end [ sizeof(id_end) - 1 ] = '\0';
	assert( id_end == GetID_END () );
#endif

}

void UplinkObject::SaveID_END ( FILE *file )
{

#ifdef SAVEFILE_PRINTPROGRESS
	PrintIndent ();
	cout << "Finished saving " << GetID () << endl;
	--indent;
#endif
	
#ifdef SAVEFILE_VERBOSE
	fwrite ( GetID_END (), SIZE_SAVEID_END, 1, file );
#endif

}




