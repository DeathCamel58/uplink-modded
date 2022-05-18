
#include <app/miscutils.h>
#include "app/serialise.h"

#include "world/computer/securitysystem.h"

#include "mmgr.h"


string names [] = {	"Unknown", "Proxy", "Firewall", "Encrypter", "Monitor" };



SecuritySystem::SecuritySystem ()
{

	TYPE = SECURITY_TYPE_NONE;
	enabled = false;
	bypassed = false;
	level = 0;
	
}

SecuritySystem::~SecuritySystem ()
= default;


void SecuritySystem::SetTYPE ( int newTYPE )
{

	TYPE = newTYPE;

}

void SecuritySystem::SetLevel ( int newlevel )
{

	level = newlevel;

}

void SecuritySystem::Enable ()
{

	enabled = true;

}

void SecuritySystem::Disable ()
{

	enabled = false;

}

void SecuritySystem::Bypass ()
{

	bypassed = true;

}

void SecuritySystem::EndBypass ()
{

	bypassed = false;

}

string SecuritySystem::GetName () const
{

	return names [TYPE];

}

bool SecuritySystem::Load  ( FILE *file )
{

	LoadID ( file );

	if ( !FileReadData ( &TYPE,	    sizeof(TYPE),	   1, file ) ) return false;
	if ( !FileReadData ( &enabled,   sizeof(enabled),   1, file ) ) return false;
	if ( !FileReadData ( &bypassed,  sizeof(bypassed),  1, file ) ) return false;
	if ( !FileReadData ( &level,	    sizeof(level),	   1, file ) ) return false;

	LoadID_END ( file );

	return true;
		
}

void SecuritySystem::Save  ( FILE *file )
{
	
	SaveID ( file );

	fwrite ( &TYPE,	     sizeof(TYPE),	    1, file );
	fwrite ( &enabled,   sizeof(enabled),   1, file );
	fwrite ( &bypassed,  sizeof(bypassed),  1, file );
	fwrite ( &level,     sizeof(level),	    1, file );
	
	SaveID_END ( file );
		
}

void SecuritySystem::Print ()
{

    cout <<  "SecuritySystem :" << endl;
    PrintValue("Type", TYPE);
    PrintValue("Enabled", enabled);
    PrintValue("Bypassed", bypassed);
    PrintValue("Level", level);

}

void SecuritySystem::Update ()
{
}


string SecuritySystem::GetID ()
{

	return "SECSYST";
		
}

int SecuritySystem::GetOBJECTID ()
{

	return OID_SECURITYSYSTEM;	

}
