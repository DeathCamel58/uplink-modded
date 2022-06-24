
#include <cstdio>

#include "redshirt.h"

#include "app/globals.h"
#include "app/serialise.h"

#include "game/game.h"

#include "world/world.h"
#include "world/company/company.h"
#include "world/computer/computer.h"
#include "world/generator/namegenerator.h"
#include "world/generator/numbergenerator.h"

#include "mmgr.h"


LList <string> NameGenerator::fornames;
LList <string> NameGenerator::surnames;
LList <string> NameGenerator::agentaliases;
LList <string> NameGenerator::companynamesA;
LList <string> NameGenerator::companynamesB;

void NameGenerator::Initialise ()
{
}

void NameGenerator::Shutdown ()
{

    DeleteLListData ( fornames );
    DeleteLListData ( surnames );
    DeleteLListData ( agentaliases );
    DeleteLListData ( companynamesA );
    DeleteLListData ( companynamesB );

    fornames.Empty ();
    surnames.Empty ();
    agentaliases.Empty ();
    companynamesA.Empty ();
    companynamesB.Empty ();

}

void NameGenerator::LoadNames ()
{

	//
	// Load forenames (only required once)
	//

	char tempName [ 128 ];

	if ( fornames.Size () == 0 ) {

		FILE *file = RsArchiveFileOpen ( "data/fornames.txt", "rt" );
		UplinkAssert ( file )

		while ( !feof ( file ) ) {

			fscanf ( file, "%s\n", tempName );
			string name = tempName;
			fornames.PutData ( name );

		}
		
        string deleteme = fornames.GetData ( fornames.Size () - 1 );
        fornames.RemoveData ( fornames.Size () - 1 );					// Remove last entry

		RsArchiveFileClose ( "data/fornames.txt", file );

	}

	//
	// Load surnames (required when all surnames have been used)
	//

	if ( surnames.Size () == 0 ) {

		FILE *file = RsArchiveFileOpen ( "data/surnames.txt", "rt" );
		UplinkAssert ( file )

		while ( !feof ( file ) ) {

			fscanf ( file, "%s\n", tempName );
			string name = tempName;
			surnames.PutData ( name );

		}

        string deleteme = surnames.GetData ( surnames.Size () - 1 );
		surnames.RemoveData ( surnames.Size () - 1 );					// Remove last entry

		RsArchiveFileClose ( "data/surnames.txt", file );

	}

	//
	// Load Agent aliases
	//

	if ( agentaliases.Size () == 0 ) {

		FILE *file = RsArchiveFileOpen ( "data/agentaliases.txt", "rt" );
		UplinkAssert ( file )

		while ( !feof ( file ) ) {

			fscanf ( file, "%s\n", tempName );
			string name = tempName;
			agentaliases.PutData ( name );

		}

        string deleteme = agentaliases.GetData ( agentaliases.Size () - 1 );
		agentaliases.RemoveData ( agentaliases.Size () - 1 );					// Remove last entry

		RsArchiveFileClose ( "data/agentaliases.txt", file );

	}

	//
	// Load company names (partA)
	// These may run out and will then need to be reloaded
	//

	if ( companynamesA.Size () == 0 ) {

		FILE *file = RsArchiveFileOpen ( "data/companya.txt", "rt" );
		UplinkAssert ( file )

		while ( !feof ( file ) ) {

			fscanf ( file, "%s\n", tempName );
			string name = tempName;
			companynamesA.PutData ( name );

		}

        string deleteme = companynamesA.GetData ( companynamesA.Size () - 1 );
		companynamesA.RemoveData ( companynamesA.Size () - 1 );					// Remove last entry

		RsArchiveFileClose ( "data/companya.txt", file );

	}

	//
	// Load company names (partB)
	//

	if ( companynamesB.Size () == 0 ) {

		FILE *file = RsArchiveFileOpen ( "data/companyb.txt", "rt" );
		UplinkAssert ( file )

		while ( !feof ( file ) ) {

			fscanf ( file, "%s\n", tempName );
			string name = tempName;
			companynamesB.PutData ( name );

		}

        string deleteme = companynamesB.GetData ( companynamesB.Size () - 1 );
		companynamesB.RemoveData ( companynamesB.Size () - 1 );					// Remove last entry

		RsArchiveFileClose ( "data/companyb.txt", file );

	}

}

string NameGenerator::GenerateCompanyName ()
{

	LoadNames ();

	int size1 = companynamesA.Size ();
	int size2 = companynamesB.Size ();


	// Choose part 1 (primary name part1)
	int index1 = NumberGenerator::RandomNumber ( size1 );

	// Choose part 2 (primary name part2)
	int index2 = NumberGenerator::RandomNumber ( size2 );

	// Build the name 

    string companynameA = companynamesA.GetData (index1);
    string companynameB = companynamesB.GetData (index2);
	string name = companynameA + " " + companynameB;

    companynamesA.RemoveData (index1);

	return name;

}

string NameGenerator::GeneratePersonName ()
{

	LoadNames ();								// Won't do anything if names exist

	int size_f = fornames.Size ();
	int size_s = surnames.Size ();
	
	int index_f = NumberGenerator::RandomNumber ( size_f );
	int index_s = NumberGenerator::RandomNumber ( size_s );

	// Build the name;

    string forname = fornames.GetData (index_f);
    string surname = surnames.GetData (index_s);
	string name = forname + " " + surname;

	surnames.RemoveData (index_s);

    string tempname = name;
	return tempname;

}

string NameGenerator::GenerateAgentAlias ()
{

	LoadNames ();

	int index = NumberGenerator::RandomNumber ( agentaliases.Size () );

	string result = agentaliases.GetData (index);

	agentaliases.RemoveData (index);

	return result;

}

string NameGenerator::GeneratePassword ()
{

	int numpasswords = game->GetWorld ()->passwords.Size ();

	int index = NumberGenerator::RandomNumber ( numpasswords );

	while (!(game->GetWorld ()->passwords.ValidIndex (index)) ) 
		index = NumberGenerator::RandomNumber ( numpasswords );
			
	return game->GetWorld ()->passwords.GetData (index);

}

string NameGenerator::GenerateEasyPassword ()
{

	switch ( NumberGenerator::RandomNumber ( 5 ) ) {

		case 0		:		return "god";
		case 1		:		return "admin";
		case 2		:		return "password";
		case 3		:		return "love";
		case 4		:		return "secure";

		default:	
			UplinkAbort ( "You've fucked up" )

	}

	return "Shut_the_compiler_up";

}

string NameGenerator::GenerateComplexPassword ()
{

	string password = GeneratePassword();

	int numchanges = NumberGenerator::RandomNumber ( 5 );

	for ( int i = 0; i < numchanges; ++i ) {

		int letternumber = NumberGenerator::RandomNumber ( 8 );
		char newcharacter = NumberGenerator::RandomNumber ( 26 ) + 'a';

		password[letternumber] = newcharacter;

	}

	return password;

}

string NameGenerator::GenerateDataName (const string &companyname, int DATATYPE )
{

	assert(!companyname.empty());

	string dataname;
    string firstThree = companyname.substr(0, 3);

	switch ( DATATYPE ) {

		case DATATYPE_NONE:
		{

			dataname = firstThree + "-file-" + to_string(NumberGenerator::RandomNumber ( 99999 ));
			break;

		}
		case DATATYPE_DATA:
		{
			
			dataname = firstThree + "-data-" + to_string(NumberGenerator::RandomNumber ( 99999 ));
			break;

		}
		case DATATYPE_PROGRAM:
		{

			dataname = firstThree + "-prog-" + to_string(NumberGenerator::RandomNumber ( 99999 ));
			break;

		}

	}

	return dataname;

}

string NameGenerator::GeneratePublicAccessServerName(const string &companyname )
{
    return companyname + " Public Access Server";
}

string NameGenerator::GenerateAccessTerminalName(const string &companyname )
{
    return companyname + " Access Terminal";
}

string NameGenerator::GenerateInternalServicesServerName (const string &companyname )
{
    return companyname + " Internal Services Machine";
}

string NameGenerator::GenerateCentralMainframeName(const string &companyname )
{
    return companyname + " Central Mainframe";
}

string NameGenerator::GenerateInternationalBankName(const string &companyname )
{
    return companyname + " International Bank";
}

string NameGenerator::GenerateFileServerName(const string &companyname )
{
    return companyname + " File Server";
}

string NameGenerator::GenerateLANName(const string &companyname )
{
    return companyname + " Local Area Network";
}

string NameGenerator::GeneratePersonalComputerName(const string &personname )
{
    return personname + " Personal Computer";
}

string NameGenerator::GenerateVoicePhoneSystemName(const string &personname )
{
    return personname + " Voice Phone System";
}

