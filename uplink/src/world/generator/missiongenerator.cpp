// MissionGenerator.cpp: implementation of the MissionGenerator class.
//
//////////////////////////////////////////////////////////////////////

#ifdef WIN32
#include <windows.h>
//#include <vfw.h>
#endif

#include <strstream>
#include <sstream>

#include "soundgarden.h"
#include "redshirt.h"

#include "app/globals.h"
#include "app/probability.h"
#include "app/serialise.h"
#include "app/miscutils.h"

#include "game/game.h"
#include "game/data/data.h"

#include "world/world.h"
#include "world/person.h"
#include "world/player.h"
#include "world/message.h"
#include "world/computer/recordbank.h"
#include "world/computer/computer.h"
#include "world/computer/bankcomputer.h"
#include "world/computer/databank.h"
#include "world/company/company.h"
#include "world/company/companyuplink.h"
#include "world/company/mission.h"
#include "world/company/news.h"

#include "world/generator/worldgenerator.h"
#include "world/generator/missiongenerator.h"
#include "world/generator/namegenerator.h"
#include "world/generator/numbergenerator.h"
#include "world/generator/consequencegenerator.h"

#include "world/scheduler/notificationevent.h"

#include "mmgr.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


Probability *MissionGenerator::prob_missiontype [NUM_UPLINKRATINGS];



void MissionGenerator::Initialise()
{

	// Assign all probabilities here
/*
	prob_missiontype.InputProbability (		MISSION_SPECIAL,			PROB_MISSION_SPECIAL			);
	prob_missiontype.InputProbability (		MISSION_STEALFILE,			PROB_MISSION_STEALFILE			);
	prob_missiontype.InputProbability (		MISSION_DESTROYFILE,		PROB_MISSION_DESTROYFILE		);
	prob_missiontype.InputProbability (		MISSION_FINDDATA,			PROB_MISSION_FINDDATA			);
	prob_missiontype.InputProbability (		MISSION_CHANGEDATA,			PROB_MISSION_CHANGEDATA			);
	prob_missiontype.InputProbability (		MISSION_FRAMEUSER,			PROB_MISSION_FRAMEUSER			);
	prob_missiontype.InputProbability (		MISSION_TRACEUSER,			PROB_MISSION_TRACEUSER			);
	prob_missiontype.InputProbability (		MISSION_CHANGEACCOUNT,		PROB_MISSION_CHANGEACCOUNT		);
	prob_missiontype.InputProbability (		MISSION_REMOVECOMPUTER,		PROB_MISSION_REMOVECOMPUTER		);
	prob_missiontype.InputProbability (		MISSION_REMOVECOMPANY,		PROB_MISSION_REMOVECOMPANY		);
	prob_missiontype.InputProbability (		MISSION_REMOVEUSER,			PROB_MISSION_REMOVEUSER			);

	UplinkAssert ( prob_missiontype.Validate () );
*/

    for ( int i = 0; i < NUM_UPLINKRATINGS; ++i ) {

        prob_missiontype [i] = new Probability ();

	    prob_missiontype [i]->InputProbability (		MISSION_SPECIAL,			PROB_MISSION_SPECIAL [i]		);
	    prob_missiontype [i]->InputProbability (		MISSION_STEALFILE,			PROB_MISSION_STEALFILE [i]		);
	    prob_missiontype [i]->InputProbability (		MISSION_DESTROYFILE,		PROB_MISSION_DESTROYFILE [i]	);
	    prob_missiontype [i]->InputProbability (		MISSION_FINDDATA,			PROB_MISSION_FINDDATA [i]		);
	    prob_missiontype [i]->InputProbability (		MISSION_CHANGEDATA,			PROB_MISSION_CHANGEDATA	[i] 	);
	    prob_missiontype [i]->InputProbability (		MISSION_FRAMEUSER,			PROB_MISSION_FRAMEUSER [i]		);
	    prob_missiontype [i]->InputProbability (		MISSION_TRACEUSER,			PROB_MISSION_TRACEUSER [i]		);
	    prob_missiontype [i]->InputProbability (		MISSION_CHANGEACCOUNT,		PROB_MISSION_CHANGEACCOUNT [i]	);
	    prob_missiontype [i]->InputProbability (		MISSION_REMOVECOMPUTER,		PROB_MISSION_REMOVECOMPUTER	[i]	);
	    prob_missiontype [i]->InputProbability (		MISSION_REMOVECOMPANY,		PROB_MISSION_REMOVECOMPANY [i]	);
	    prob_missiontype [i]->InputProbability (		MISSION_REMOVEUSER,			PROB_MISSION_REMOVEUSER	[i]		);

	    UplinkAssert ( prob_missiontype [i]->Validate () )

    }

}

void MissionGenerator::Shutdown ()
{

    for ( int i = 0; i < NUM_UPLINKRATINGS; ++i ) {

        Probability *p = prob_missiontype [i];
        delete p;

    }

}

Mission *MissionGenerator::GenerateMission ()
{

    int playerRating = game->GetWorld ()->GetPlayer ()->rating.uplinkrating;
    assert( playerRating >= 0 );
    assert( playerRating < NUM_UPLINKRATINGS );

	return GenerateMission ( prob_missiontype [playerRating]->GetValue () );

}

Mission *MissionGenerator::GenerateMission ( int type )
{

	Company *employer = WorldGenerator::GetRandomCompany ();

	return GenerateMission ( type, employer );

}

Mission *MissionGenerator::GenerateMission ( int type, Company *employer )
{

	Computer *source = nullptr;
	Computer *target = nullptr;

	switch ( type ) {

		case MISSION_STEALFILE		:

			return Generate_StealFile ( employer );
			break;

		case MISSION_DESTROYFILE    :

			return Generate_DestroyFile ( employer );
			break;

		case MISSION_FINDDATA		:

			return Generate_FindData ( employer );
			break;

		case MISSION_CHANGEDATA		:

			return Generate_ChangeData ( employer );
			break;

#ifndef DEMOGAME

		case MISSION_CHANGEACCOUNT  :

			source = WorldGenerator::GetRandomComputer ( COMPUTER_TYPE_PUBLICBANKSERVER );
			target = WorldGenerator::GetRandomComputer ( COMPUTER_TYPE_PUBLICBANKSERVER );
			return Generate_ChangeAccount ( employer, source, target );
			break;

		case MISSION_FRAMEUSER		:

			return Generate_FrameUser ( employer );
			break;

		case MISSION_TRACEUSER		:

			return Generate_TraceUser ( employer );
			break;

		case MISSION_REMOVECOMPUTER :

			target = WorldGenerator::GetRandomComputer ( COMPUTER_TYPE_CENTRALMAINFRAME );
			return Generate_RemoveComputer ( employer, target );
			break;

		case MISSION_REMOVEUSER		:

			return Generate_RemoveUser ( employer );
			break;

#endif

		default:

			return nullptr;
			break;

	}

}

Mission *MissionGenerator::Generate_StealFile ( Company *employer )
{

	UplinkAssert (employer)

	int type = NumberGenerator::RandomNumber ( 2 ) + 1;

	Mission *m = nullptr;

	switch ( type ) {

		case 1 :
		{
			Computer *target = WorldGenerator::GetRandomLowSecurityComputer ( COMPUTER_TYPE_INTERNALSERVICESMACHINE );
			UplinkAssert (target)
			if ( employer->name == target->companyname ) return nullptr;
			m = Generate_StealSingleFile ( employer, target );
			break;
		}

		case 2 :
		{
			Computer *target = WorldGenerator::GetRandomComputer ( COMPUTER_TYPE_INTERNALSERVICESMACHINE |
                                                                   COMPUTER_TYPE_CENTRALMAINFRAME |
                                                                   COMPUTER_TYPE_LAN );
			UplinkAssert (target)
			if ( employer->name == target->companyname ) return nullptr;
			m = Generate_StealAllFiles   ( employer, target );
			break;
		}

	}

	return m;

}

Mission *MissionGenerator::Generate_StealSingleFile ( Company *employer, Computer *target )
{

	UplinkAssert (employer)
	UplinkAssert (target)


	if ( employer->name == target->companyname ) return nullptr;

	int difficulty = (int) NumberGenerator::RandomNormalNumber ( MINDIFFICULTY_MISSION_STEALFILE, DIFFICULTY_MISSION_VARIANCE );
	if ( difficulty < MINDIFFICULTY_MISSION_STEALFILE ) difficulty = MINDIFFICULTY_MISSION_STEALFILE;

	// Set up the basic variables of the mission

	int payment			= (int) NumberGenerator::RandomNormalNumber ( (float) ( difficulty * PAYMENT_MISSION_STEALFILE ),
	                                                                  (float) ( difficulty * PAYMENT_MISSION_STEALFILE
	                                                                                       * PAYMENT_MISSION_VARIANCE ) );
	int minrating		= difficulty;

	int acceptrating	= minrating + NumberGenerator::RandomNumber ( 2 );
	int datasize		= (int) ( NumberGenerator::RandomNormalNumber ( (float) difficulty, difficulty / 3.0f ) + 1 );

	if ( minrating > 10 ) minrating = 10;
	if ( acceptrating > 10 ) acceptrating = 10;

	payment = payment / 100 * 100;				// Rounds payment to the nearest 100


	// Create the data to be stolen and add it to the target computer's memory

	string datatitle = NameGenerator::GenerateDataName ( target->name, DATATYPE_DATA );

	int encrypted = difficulty < 3 ? 0 : (int) NumberGenerator::RandomNormalNumber ( (float) difficulty, (float) ( difficulty / 2 ) );
    if ( encrypted > 7 ) encrypted = 7;

	int compressed = 0;

	Data *data = new Data ();
	data->SetTitle ( datatitle );
	data->SetDetails ( DATATYPE_DATA, datasize, encrypted, compressed );

	if ( target->databank.FindValidPlacement ( data ) == -1 )
		target->databank.SetSize ( target->databank.GetSize () + data->size + 2 );

    target->databank.InsertData ( data );
	//target->databank.PutData ( data );

	if ( game->GetWorld ()->GetPlayer ()->GetRemoteHost ()->ip != target->ip )
		target->databank.RandomizeDataPlacement ();

	// Infer the internal services contact address

	string personname = "internal@" + employer->name + ".net";


	// Generate the fields of the mission

	string description;
	std::ostrstream details;
	std::ostrstream fulldetails;

	switch ( NumberGenerator::RandomNumber ( 3 ) + 1 ) {

		case 1 : description = "Steal important data from a rival company's file server"; break;
		case 2 : description = "Access a remote system and copy important data files"; break;
		case 3 : description = "Gain access to a rival system and steal research files"; break;

	}

	details << "Payment for this job is " << payment << " credits.\n"
			<< "You will require " << datasize << " GigaQuads of storage space.\n"
			<< "This job has been assigned an Uplink difficulty of " << difficulty << ".\n"
			<< "An UplinkRating of " << Rating::GetUplinkRatingString ( acceptrating ) << " or above will be sufficient for automatic acceptance.\n\n"
			<< '\x0';


	fulldetails << "Thank you for working for " << employer->name << ".\n"
				<< "\n"
				<< "TARGET COMPUTER DATA :\n"
				<< "   LOCATION: " << target->name << "\n"
				<< "   IP      : " << target->ip << "\n"
				<< "   FILENAME: " << datatitle << "\n"
				<< "   FILESIZE: " << datasize << " GigaQuads\n"
				<< "\n";

	if ( encrypted > 0 && encrypted < 5 )	fulldetails << "\nThe file will be encrypted.\n";
	else if ( encrypted >= 5 )				fulldetails << "\nThe file will be heavily encrypted.\n";

	if ( compressed > 0 && compressed < 5 )	fulldetails << "\nThe file will be compressed.\n";
	else if ( compressed >= 5 )				fulldetails << "\nThe file will be heavily compressed.\n";


	fulldetails	<< "\nReturn the file via email to\n"
				<< personname << "\n"
				<< "\n"
				<< "END"
				<< '\x0';

	Date postdate;
	postdate.SetDate ( &(game->GetWorld ()->date) );
	postdate.AdvanceHour ( NumberGenerator::RandomNumber ( 96 ) * -1 );
	postdate.AdvanceMinute ( NumberGenerator::RandomNumber ( 60 ) * -1 );

	// Insert the mission
	auto *mission = new Mission ();
	mission->SetTYPE		 ( MISSION_STEALFILE );
	mission->SetCompletion   ( target->ip, datatitle, "", "", "" );
	mission->SetEmployer     ( employer->name );
	mission->SetContact      ( personname );
	mission->SetPayment      ( payment, (int) ( payment * 1.1 ) );
	mission->SetDifficulty   ( difficulty );
	mission->SetMinRating    ( minrating );
	mission->SetAcceptRating ( acceptrating );
	mission->SetDescription  ( description );
	mission->SetDetails		 ( details.str () );
	mission->SetFullDetails  ( fulldetails.str () );
	if ( difficulty > 3 )		mission->SetWhySoMuchMoney ( "The files contain sensitive data." );
	if ( !game->IsRunning () )	mission->SetCreateDate   ( &postdate );
	mission->GiveLink ( target->ip );

	details.rdbuf()->freeze( false );
	fulldetails.rdbuf()->freeze( false );
	//delete [] details.str ();
	//delete [] fulldetails.str ();

	CompanyUplink *cu = (CompanyUplink *) game->GetWorld ()->GetCompany ( "Uplink" );
	UplinkAssert ( cu )
	cu->CreateMission ( mission );

	return mission;

}

Mission *MissionGenerator::Generate_StealAllFiles ( Company *employer, Computer *target )
{

	UplinkAssert (employer)
	UplinkAssert (target)

	int difficulty = (int) NumberGenerator::RandomNormalNumber ( MINDIFFICULTY_MISSION_STEALALLFILES, DIFFICULTY_MISSION_VARIANCE );
	if ( difficulty < MINDIFFICULTY_MISSION_STEALALLFILES ) difficulty = MINDIFFICULTY_MISSION_STEALALLFILES;

    if ( target->TYPE == COMPUTER_TYPE_CENTRALMAINFRAME )   difficulty += 1;
    if ( target->TYPE == COMPUTER_TYPE_LAN )                difficulty += 2;

	// Set up the basic variables of the mission

	int payment			= (int) NumberGenerator::RandomNormalNumber ( (float) ( difficulty * PAYMENT_MISSION_STEALALLFILES ),
	                                                                  (float) ( difficulty * PAYMENT_MISSION_STEALALLFILES
	                                                                                       * PAYMENT_MISSION_VARIANCE ) );

	int minrating		= difficulty;

	int acceptrating	= minrating + NumberGenerator::RandomNumber ( 2 );

	if ( minrating > 10 ) minrating = 10;
	if ( acceptrating > 10 ) acceptrating = 10;

	payment = payment / 100 * 100;				// Rounds payment to the nearest 100


	// Infer the internal services contact address

	string personname = "internal@" + employer->name + ".net";

	//
	// Generate a new computer to dump the files to
	//

	Computer *ourcomp = WorldGenerator::GenerateEmptyFileServer ( employer->name );
	UplinkAssert (ourcomp)

	//
	// Add in a new account for the player to use
	//

	string password = NameGenerator::GeneratePassword ();
	char username [12];
	UplinkSnprintf ( username, sizeof ( username ), "temp%c%c%c%c", 'a' + NumberGenerator::RandomNumber ( 26 ),
																	'a' + NumberGenerator::RandomNumber ( 26 ),
																	'a' + NumberGenerator::RandomNumber ( 26 ),
																	'a' + NumberGenerator::RandomNumber ( 26 ) )

	auto *record = new Record ();
	record->AddField ( RECORDBANK_NAME, username );
	record->AddField ( RECORDBANK_PASSWORD, password );
	record->AddField ( RECORDBANK_SECURITY, "3" );
	ourcomp->recordbank.AddRecord ( record );

	string code;
    Computer::GenerateAccessCode(username, password, code);

	int missiontype = NumberGenerator::RandomNumber ( 4 ) + 1;

	/*  ===================================================
		type 1		:		Steal all scientific research
		type 2		:		Steal all corporate data
		type 3		:		Steal customer records
		type 4		:		Steal software in development
		===================================================

		*/

	string missiontypestring [4] = { "rsrch", "corp", "custmr", "softw" };

	//
	// Generate around 10 files to be stolen,
	// All fairly large
	//

	int numfiles = (int) NumberGenerator::RandomNormalNumber ( 10, 5 );
	int encrypted = (int) NumberGenerator::RandomNormalNumber ( (float) difficulty, (float) difficulty );
    if ( encrypted > 7 ) encrypted = 7;
	int totalsize = 0;

	for ( int i = 0; i < numfiles; ++i ) {

		char datatitle [64];
		UplinkSnprintf ( datatitle, sizeof ( datatitle ), "%c%c%c%c-%s %d.dat", target->companyname [0], target->companyname [1],
																	   target->companyname [2], target->companyname [3],
																		missiontypestring [missiontype-1].c_str(), i )

		int size = (int) NumberGenerator::RandomNormalNumber ( 10, 5 );
		totalsize += size;

		Data *file = new Data ();
		file->SetTitle ( datatitle );
		file->SetDetails ( DATATYPE_DATA, size, encrypted, 0 );

		while ( target->databank.FindValidPlacement (file) == -1 )
			target->databank.SetSize ( target->databank.GetSize () + file->size );

		target->databank.PutData ( file );

	}

	if ( game->GetWorld ()->GetPlayer ()->GetRemoteHost ()->ip != target->ip )
		target->databank.RandomizeDataPlacement ();

	/********** Patch by François Gagné **********/
    //
    // Make sure there is enough space on the new computer to store the mission files

    if ( totalsize > ourcomp->databank.GetSize() ) {
        ourcomp->databank.SetSize( totalsize );
    }

    //
	// Generate the fields of the mission

	string description;
	std::ostrstream details;
	std::ostrstream fulldetails;


	details << "Payment for this job is " << payment << " credits.\n"
            << "This job has been assigned an Uplink difficulty of " << difficulty << ".\n"
            << "An UplinkRating of " << Rating::GetUplinkRatingString ( acceptrating )
            << " or above will be sufficient for automatic acceptance.\n"
            << "You will need around " << 10 * (totalsize / 10) << " gigaquads of space.";

    if ( target->TYPE == COMPUTER_TYPE_LAN )
        details << "\nThe data is stored on a LAN.\n";

    details << '\x0';


    string completionA = target->ip;								// IP
	string completionB = "ALL";										// Data title or ALL
    string completionC = ourcomp->ip;								// IP of our dump machine
	string completionD = to_string(numfiles) + " " + to_string(totalsize); // Numfiles TotalSize
	string completionE;												// TYPE of data

	string whoisthetarget = "The data is owned by " + target->companyname;

	if ( missiontype == 1 ) {

		description = "Steal valuable scientific research documents";

		completionE = "SCIENTIFIC";

		fulldetails << "Due to a lack of enthusiasm on the part of our recruitment department, "
					   "one of our market competitors has pulled several months ahead with "
					   "their research program.  Given our current situation, we have concluded "
					   "that the best way to close this gap is for us to steal the research data "
					   "they have gathered.  This will be your mission.\n\n"
					   "Their research files are stored on the system below, and will be "
					   "heavily protected.  You will need to copy the files, break the security "
					   "on them in any way you see fit, then copy the files onto our own file server.\n\n";

	}
	else if ( missiontype == 2 ) {

		description = "Copy large and secure corporate database";

		fulldetails << "Our establishment is very anxious to find out more about the inner workings "
					   "of one of our rivals, and we have determined after several weeks of investigation "
					   "that their primary corporate database is stored at the location shown below.  We "
					   "want you to enter this system and steal all of the data files.\n\n"
					   "These files will be well protected and encrypted - you will need to break this "
					   "security, before copying the stolen database to our own file server, location below.\n\n";

		completionE = "CORPORATE";

	}
	else if ( missiontype == 3 ) {

		completionE = "CUSTOMER";

		description = "Break into High Security System and steal customer records";

		fulldetails << "It has come to our attention that one of our competitors has amassed an extensive "
					   "customer database which would prove to be of great help to our market research team.  "
					   "We want you to compromise the security of the system below, copy all of the customer data "
					   "and decrypt it.  Then copy the files onto our own temporary file server.\n\n";

	}
	else if ( missiontype == 4 ) {

		completionE = "SOFTWARE";

		description = "Copy proprietary source code database";

		fulldetails << "As you may be aware we are currently in direct competition with another rival corporation "
					   "to develop a software product which will no doubt dominate the market once released.  "
					   "Unfortunately, due to circumstances beyond our control, they have managed to pull several months "
					   "ahead.  We believe the best course of action is for us to copy their program source code and use "
					   "it to help our product provide adequate competition.\n"
					   "Break into the computer system below, copy anything that looks like source code and break any "
					   "security systems in place on the data files.  Then copy those files onto our own file server.\n\n";

	}

	fulldetails << "Target computer : " << target->name << "\n"
				<< "IP : " << target->ip << "\n\n"
				<< "Once complete, dump the files you have to this computer:\n"
				<< ourcomp->name << "\n"
				<< "IP : " << ourcomp->ip << "\n\n"
				<< "USERNAME : " << username << "\n"
				<< "PASSWORD : " << password << "\n\n"
				<< "END"
				<< '\x0';


	Date postdate;
	postdate.SetDate ( &(game->GetWorld ()->date) );
	postdate.AdvanceHour ( NumberGenerator::RandomNumber ( 96 ) * -1 );
	postdate.AdvanceMinute ( NumberGenerator::RandomNumber ( 60 ) * -1 );

	// Insert the mission
	auto *mission = new Mission ();
	mission->SetTYPE		 ( MISSION_STEALFILE );
	mission->SetCompletion   ( completionA, completionB, completionC, completionD, completionE );
	mission->SetEmployer     ( employer->name );
	mission->SetContact      ( personname );
	mission->SetPayment      ( payment, (int) ( payment * 1.1 ) );
	mission->SetDifficulty   ( difficulty );
	mission->SetMinRating    ( minrating );
	mission->SetAcceptRating ( acceptrating );
	mission->SetDescription  ( description );
	mission->SetDetails		 ( details.str () );
	mission->SetFullDetails  ( fulldetails.str () );
	mission->SetWhySoMuchMoney ( "There will be a lot of data to copy" );
	if ( !game->IsRunning () )	mission->SetCreateDate   ( &postdate );
	mission->SetWhoIsTheTarget ( whoisthetarget );
	mission->GiveLink ( target->ip );
	mission->GiveLink ( ourcomp->ip );
	mission->GiveCode ( ourcomp->ip, code );

	details.rdbuf()->freeze( false );
	fulldetails.rdbuf()->freeze( false );
	//delete [] details.str ();
	//delete [] fulldetails.str ();

	CompanyUplink *cu = (CompanyUplink *) game->GetWorld ()->GetCompany ( "Uplink" );
	UplinkAssert ( cu )
	cu->CreateMission ( mission );

	return mission;

}

Mission *MissionGenerator::Generate_DestroyFile ( Company *employer )
{

	UplinkAssert (employer)

	int type = NumberGenerator::RandomNumber ( 2 ) + 1;

	Mission *m = nullptr;

	switch ( type ) {

		case 1 :
		{
			Computer *target = WorldGenerator::GetRandomLowSecurityComputer ( COMPUTER_TYPE_INTERNALSERVICESMACHINE );
			UplinkAssert (target)
			if ( employer->name == target->companyname ) return nullptr;
			m = Generate_DestroySingleFile ( employer, target );
			break;
		}

		case 2 :
		{
			Computer *target = WorldGenerator::GetRandomComputer ( COMPUTER_TYPE_INTERNALSERVICESMACHINE |
                                                                   COMPUTER_TYPE_CENTRALMAINFRAME |
                                                                   COMPUTER_TYPE_LAN );
			UplinkAssert (target)
			if ( employer->name == target->companyname ) return nullptr;
			m = Generate_DestroyAllFiles ( employer, target );
			break;
		}

	}

	return m;

}

Mission *MissionGenerator::Generate_DestroySingleFile ( Company *employer, Computer *target )
{

	UplinkAssert (employer)
	UplinkAssert (target)

	int difficulty = (int) NumberGenerator::RandomNormalNumber ( MINDIFFICULTY_MISSION_DESTROYFILE, DIFFICULTY_MISSION_VARIANCE );
	if ( difficulty < MINDIFFICULTY_MISSION_DESTROYFILE ) difficulty = MINDIFFICULTY_MISSION_DESTROYFILE;

    if ( target->TYPE == COMPUTER_TYPE_CENTRALMAINFRAME ) ++difficulty;


	// Set up the basic variables of the mission

	int payment			= (int) NumberGenerator::RandomNormalNumber ( (float) ( difficulty * PAYMENT_MISSION_DESTROYFILE ),
	                                                                  (float) ( difficulty * PAYMENT_MISSION_DESTROYFILE
	                                                                                       * PAYMENT_MISSION_VARIANCE ) );

	int minrating		= difficulty;
	int acceptrating	= minrating + NumberGenerator::RandomNumber ( 2 );
	int datasize		= (int) NumberGenerator::RandomNormalNumber ( (float) difficulty, difficulty / 3.0f ) + 1;

	if ( minrating > 10 ) minrating = 10;
	if ( acceptrating > 10 ) acceptrating = 10;

	payment = payment / 100 * 100;				// Rounds payment to the nearest 100


	// Create the data to be stolen and add it to the target computer's memory

	string datatitle = NameGenerator::GenerateDataName ( target->name, DATATYPE_DATA );

	Data *data = new Data ();
	data->SetTitle ( datatitle );
	data->SetDetails ( DATATYPE_DATA, datasize, 0, 0 );

	if ( target->databank.FindValidPlacement ( data ) == -1 )
		target->databank.SetSize ( target->databank.GetSize () + data->size + 2 );

    target->databank.InsertData ( data );
	//target->databank.PutData ( data );

	if ( game->GetWorld ()->GetPlayer ()->GetRemoteHost ()->ip != target->ip )
		target->databank.RandomizeDataPlacement ();

	// Infer the internal services contact address

	string personname = "internal@" + employer->name + ".net";


	// Generate the fields of the mission

	string description;
	std::ostrstream details;
	std::ostrstream fulldetails;


	switch ( NumberGenerator::RandomNumber ( 3 ) + 1 ) {

		case 1 : description = "Find and destroy crucial data on a mainframe";					break;
		case 2 : description = "Break into a rival computer system and sabotage files";		break;
		case 3 : description = "Hack into a computer and delete key files";					break;

	}

	details << "Payment for this job is " << payment << " credits.\n"
			<< "This job has been assigned an Uplink difficulty of " << difficulty << ".\n"
			<< "An UplinkRating of " << Rating::GetUplinkRatingString ( acceptrating ) << " or above will be sufficient for automatic acceptance.\n\n"
			<< '\x0';


	fulldetails << "Thank you for working for " << employer->name << ".\n"
				<< "\n"
				<< "TARGET COMPUTER DATA :\n"
				<< "   LOCATION: " << target->name << "\n"
				<< "   IP      : " << target->ip << "\n"
				<< "   FILENAME: " << datatitle << "\n"
				<< "   FILESIZE: " << datasize << " GigaQuads\n"
				<< "\n"
				<< "Destroy this data and all backups.\n"
				<< "Send a notice of completion to\n"
				<< personname << "\n"
				<< "\n"
				<< "END"
				<< '\x0';

	string completionA = target->ip;
    string completionB = datatitle;

	Date postdate;
	postdate.SetDate ( &(game->GetWorld ()->date) );
	postdate.AdvanceHour ( NumberGenerator::RandomNumber ( 96 ) * -1 );
	postdate.AdvanceMinute ( NumberGenerator::RandomNumber ( 60 ) * -1 );

	// Insert the mission
	auto *mission = new Mission ();
	mission->SetTYPE		 ( MISSION_DESTROYFILE );
	mission->SetCompletion   ( completionA, completionB, "", "", "" );
	mission->SetEmployer     ( employer->name );
	mission->SetContact      ( personname );
	mission->SetPayment      ( payment, (int) ( payment * 1.1 ) );
	mission->SetDifficulty   ( difficulty );
	mission->SetMinRating    ( minrating );
	mission->SetAcceptRating ( acceptrating );
	mission->SetDescription  ( description );
	mission->SetDetails		 ( details.str () );
	mission->SetFullDetails  ( fulldetails.str () );
	if ( difficulty > 4 )		mission->SetWhySoMuchMoney ( "The data will be well protected." );
	if ( !game->IsRunning () )	mission->SetCreateDate   ( &postdate );
	mission->GiveLink ( target->ip );

	details.rdbuf()->freeze( 0 );
	fulldetails.rdbuf()->freeze( 0 );
	//delete [] details.str ();
	//delete [] fulldetails.str ();

	CompanyUplink *cu = (CompanyUplink *) game->GetWorld ()->GetCompany ( "Uplink" );
	UplinkAssert ( cu )
	cu->CreateMission ( mission );

	return mission;

}

Mission *MissionGenerator::Generate_DestroyAllFiles ( Company *employer, Computer *target )
{

	UplinkAssert (employer)
	UplinkAssert (target)

	int difficulty = (int) NumberGenerator::RandomNormalNumber ( MINDIFFICULTY_MISSION_DESTROYALLFILES, DIFFICULTY_MISSION_VARIANCE );
	if ( difficulty < MINDIFFICULTY_MISSION_DESTROYALLFILES ) difficulty = MINDIFFICULTY_MISSION_DESTROYALLFILES;

    if ( target->TYPE == COMPUTER_TYPE_CENTRALMAINFRAME )   difficulty += 1;
    if ( target->TYPE == COMPUTER_TYPE_LAN )                difficulty += 2;

	// Set up the basic variables of the mission

	int payment			= (int) NumberGenerator::RandomNormalNumber ( (float) ( difficulty * PAYMENT_MISSION_DESTROYALLFILES ),
	                                                                  (float) ( difficulty * PAYMENT_MISSION_DESTROYALLFILES
	                                                                                       * PAYMENT_MISSION_VARIANCE ) );

	int minrating		= difficulty;

	int acceptrating	= minrating + NumberGenerator::RandomNumber ( 2 );

	if ( minrating > 10 ) minrating = 10;
	if ( acceptrating > 10 ) acceptrating = 10;

	payment = payment / 100 * 100;				// Rounds payment to the nearest 100


	// Infer the internal services contact address

	string personname = "internal@" + employer->name + ".net";


	// Generate the fields of the mission

	string description;
	std::ostrstream details;
	std::ostrstream fulldetails;


	details << "Payment for this job is " << payment << " credits.\n"
			<< "This job has been assigned an Uplink difficulty of " << difficulty << ".\n"
			<< "An UplinkRating of " << Rating::GetUplinkRatingString ( acceptrating ) << " or above will be sufficient for automatic acceptance.\n\n";

    if ( target->TYPE == COMPUTER_TYPE_LAN )
        details << "\nThe data is stored on a LAN.\n";

	details	<< '\x0';


	string completionA = target->ip;									// IP
	string completionB = "ALL";									        // Data title or ALL
	string completionC;													// Type of file

	string whoisthetarget = "The data is owned by " + target->companyname;

	int type = NumberGenerator::RandomNumber ( 4 ) + 1;


	//
	// Generate around 10 files to be stolen,
	// All fairly large
	//

	string missiontypestring [4] = { "rsrch", "corp", "custmr", "softw" };
	int numfiles = (int) NumberGenerator::RandomNormalNumber ( 10, 5 );
	int encrypted = (int) NumberGenerator::RandomNormalNumber ( (float) difficulty, (float) difficulty );
	int totalsize = 0;

	for ( int i = 0; i < numfiles; ++i ) {

		char datatitle [64];
		UplinkSnprintf ( datatitle, sizeof ( datatitle ), "%c%c%c%c-%s %d.dat", target->companyname [0], target->companyname [1],
																			   target->companyname [2], target->companyname [3],
																				missiontypestring [type-1].c_str(), i )

		int size = (int) NumberGenerator::RandomNormalNumber ( 10, 5 );
		totalsize += size;

		Data *file = new Data ();
		file->SetTitle ( datatitle );
		file->SetDetails ( DATATYPE_DATA, size, encrypted, 0 );

		while ( target->databank.FindValidPlacement (file) == -1 )
			target->databank.SetSize ( target->databank.GetSize () + file->size );

		target->databank.PutData ( file );

	}

	if ( game->GetWorld ()->GetPlayer ()->GetRemoteHost ()->ip != target->ip )
		target->databank.RandomizeDataPlacement ();

	/*
		type 1		:		Destroy all scientific research
		type 2		:		Destroy all corporate data
		type 3		:		Destroy customer records
		type 4		:		Destroy software in development

		*/

	if ( type == 1 ) {

		description = "Destroy scientific research currently in progress";

		fulldetails << "One of our competitors has unfortunately gained a lead "
					   "and has nearly completed some scientific research which will "
					   "be damaging to our corporate agenda.\n"
					   "Break into the computer system below and destroy ALL files "
					   "you find on their file server.  You must also ensure all backups "
					   "are totally wiped out.\n";

		completionC = "SCIENTIFIC";

	}
	else if ( type == 2 ) {

		description = "Destroy corporate data of a rival company";

		fulldetails << "We are becoming tired of the competition provided by a "
					   "rival firm and we have decided to put an end to their activities.\n"
					   "Break into their main computer system (ip below) and destroy ALL files "
					   "on their system.\n";

		completionC = "CORPORATE";

	}
	else if ( type == 3 ) {

		description = "Destroy customer records on a sales system";

		fulldetails << "We are in direct competition with another rival sales system and "
					   "wish to bring our conflict to a quick end.  Break into their primary "
					   "sales system shown below and delete all files on their computer.\n";

		completionC = "CUSTOMER";

	}
	else if ( type == 4 ) {

		description = "Destroy rival software in development";

		fulldetails << "As you may be aware we are trying to promote our new software system "
					   "but another corporation has developed a rival tool which we feel may "
					   "harm our sales.  Gain access to their main development server (below) "
					   "and destroy ALL files you find.  You must also ensure all backups are "
					   "totally destroyed.\n";

		completionC = "SOFTWARE";

	}

	fulldetails <<	"It would be in your interests to remove any traces of your actions, "
					"as the company will not be pleased when they discover what you have done."
					"\n\n"
					"TARGET COMPUTER : \n"
				 << "LOCATION: " << target->name << "\n"
				 << "IP      : " << target->ip << "\n"
				 << "\n"
				 << "Send a notice of completion to\n"
				 << personname << "\n"
				 << "\n"
				 << "END"
				 << '\x0';


	Date postdate;
	postdate.SetDate ( &(game->GetWorld ()->date) );
	postdate.AdvanceHour ( NumberGenerator::RandomNumber ( 96 ) * -1 );
	postdate.AdvanceMinute ( NumberGenerator::RandomNumber ( 60 ) * -1 );

	// Insert the mission
	auto *mission = new Mission ();
	mission->SetTYPE		 ( MISSION_DESTROYFILE );
	mission->SetCompletion   ( completionA, completionB, completionC, "", "" );
	mission->SetEmployer     ( employer->name );
	mission->SetContact      ( personname );
	mission->SetPayment      ( payment, (int) ( payment * 1.1 ) );
	mission->SetDifficulty   ( difficulty );
	mission->SetMinRating    ( minrating );
	mission->SetAcceptRating ( acceptrating );
	mission->SetDescription  ( description );
	mission->SetDetails		 ( details.str () );
	mission->SetFullDetails  ( fulldetails.str () );
	mission->SetWhySoMuchMoney ( "The data will be well protected." );
	if ( !game->IsRunning () )	mission->SetCreateDate   ( &postdate );
	mission->SetWhoIsTheTarget ( whoisthetarget );
	mission->GiveLink ( target->ip );

	details.rdbuf()->freeze( false );
	fulldetails.rdbuf()->freeze( false );
	//delete [] details.str ();
	//delete [] fulldetails.str ();

	CompanyUplink *cu = (CompanyUplink *) game->GetWorld ()->GetCompany ( "Uplink" );
	UplinkAssert ( cu )
	cu->CreateMission ( mission );

	return mission;

}

Mission *MissionGenerator::Generate_FindData ( Company *employer )
{

	int missiontype = 0;

	Mission *mission = nullptr;
	Computer *target = nullptr;

	if (missiontype == 0) {
	    target = WorldGenerator::GetRandomComputer ( COMPUTER_TYPE_PUBLICBANKSERVER );
        mission = Generate_FindData_FinancialRecord ( employer, target );
	}

	return mission;

}

Mission *MissionGenerator::Generate_FindData_FinancialRecord ( Company *employer, Computer *target )
{

	UplinkAssert ( employer )
	UplinkAssert ( target )
	assert( target->GetOBJECTID () == OID_BANKCOMPUTER );

	int difficulty = (int) NumberGenerator::RandomNormalNumber ( MINDIFFICULTY_MISSION_FINDDATA_FINANCIAL, DIFFICULTY_MISSION_VARIANCE );
	if ( difficulty < MINDIFFICULTY_MISSION_FINDDATA_FINANCIAL ) difficulty = MINDIFFICULTY_MISSION_FINDDATA_FINANCIAL;

	//
	// Set up the basic variables of the mission
	//

	int payment			= (int) NumberGenerator::RandomNormalNumber ( (float) ( difficulty * PAYMENT_MISSION_FINDDATA_FINANCIAL ),
	                                                                  (float) ( difficulty * PAYMENT_MISSION_FINDDATA_FINANCIAL
	                                                                                       * PAYMENT_MISSION_VARIANCE ) );

	int minrating		= difficulty;
	int acceptrating	= minrating + NumberGenerator::RandomNumber ( 2 );

	if ( minrating > 10 ) minrating = 10;
	if ( acceptrating > 10 ) acceptrating = 10;

	payment = payment / 100 * 100;				// Rounds payment to the nearest 100


	string personname = "internal@" + employer->name + ".net"; // Person to send completion email to

	//
	// Get the account of a person at this bank computer
	//

	BankAccount *taccount = ((BankComputer *) target)->GetRandomAccount ();
	UplinkAssert ( taccount )

	// Make sure we don't target the player's Uplink bank account

	if ( taccount->name == game->GetWorld ()->GetPlayer ()->handle ) return nullptr;

	//
	// Fill in the fields of the mission
	//

	string completionA = target->ip + " " + to_string(taccount->accountnumber);	// bankIP accountNum
    string completionB;                             // Field required
    string completionC = "Unused";                  // Name of person responsible (only type3)

	string description;
	std::ostrstream fulldetails;

	string details = "Payment for this job is " + to_string(payment) + " credits.\n"
                     "This job has been assigned an Uplink difficulty of " + to_string(difficulty) + ".\n"
                     "An UplinkRating of " + Rating::GetUplinkRatingString ( acceptrating ) + " or above will be sufficient for automatic acceptance.\n\n";

	fulldetails << "Break into the following well known financial institute:\n"
				<< "TARGET COMPUTER DATA :\n"
				<< "   LOCATION: " << target->name << "\n"
				<< "   IP      : " << target->ip << "\n"
				<< "\n\n";

	int missiontype = NumberGenerator::RandomNumber ( 3 ) + 1;

		/* =========================================================================
				type 1		:		Find bank balance
				type 2		:		Find size of loan
				type 3		:		Find receiver of recent payment
		   =========================================================================	*/

	if ( missiontype == 1 ) {

		description = "Find financial details of one of our associates";

		fulldetails << "We are interested in this man's bank balance.\n"
					<< "Open his account and send his balance to us.\n"
					<< "NAME : " << taccount->name << "\n"
					<< "ACCNO: " << taccount->accountnumber << "\n";

//		char *balance = nullptr;
//		balance = rec->GetField ( "Balance" );
//		if ( !balance ) return nullptr;

		completionB = "Balance";

	}
	else if ( missiontype == 2 ) {

		description = "Client interested in financial data on an enemy";

		fulldetails << "We are interested in this man's financial situation.\n"
					<< "Open his account and determine the size of his loan. Send this information to us.\n"
					<< "NAME : " << taccount->name << "\n"
					<< "ACCNO: " << taccount->accountnumber << "\n";

//		char *loan = nullptr;
//		loan = rec->GetField ( "Loan" );
//		if ( !loan ) return nullptr;

		completionB = "Loan";

	}
	else if ( missiontype == 3 ) {

		// Perform the transfer
		auto *receiver = (BankComputer *) WorldGenerator::GetRandomComputer ( COMPUTER_TYPE_PUBLICBANKSERVER );
		UplinkAssert ( receiver )
		BankAccount *raccount = receiver->GetRandomAccount ();
		UplinkAssert ( raccount )

		if ( raccount == taccount )		return nullptr;				// That would be silly ;)

		string taccountno = to_string(taccount->accountnumber);
		string raccountno = to_string(raccount->accountnumber);

		if ( game->GetWorld ()->GetPlayer ()->IsPlayerAccount (receiver->ip, raccountno) ) return nullptr;
		if ( game->GetWorld ()->GetPlayer ()->IsPlayerAccount (target->ip, taccountno) ) return nullptr;

		int amount = (int) NumberGenerator::RandomNormalNumber ( 1000000, 700000 );
		amount = 100000 * ( amount / 100000 );
        taccount->balance += amount;

		Person *transferer = game->GetWorld ()->GetPerson ( taccount->name );
		UplinkAssert (transferer)

		bool success = taccount->TransferMoney ( target->ip, taccountno, receiver->ip, raccountno, amount, transferer );

		description = "Trace a recent balance transfer";

		fulldetails << "This individual is the subject of an internal investigation into corruption:\n"
					<< "NAME : " << taccount->name << "\n"
					<< "ACCNO: " << taccount->accountnumber << "\n"
					<< "He recently transfered a sizable amount of money to another account."
					<< "Trace this account and find the name of the receiver of this money.";

        completionB = "TraceTransfer";
		completionC = raccount->name;

	}
	else {

		UplinkAbort ( "Unrecognised mission type" )

	}

	//
	// Generate the remaining fields of the mission
	//

	fulldetails	<< "\nSend a notice of completion to\n"
				<< personname << "\n"
				<< "\n"
				<< "END"
				<< '\x0';

	string whoisthetarget = "Nobody you would know.";

	Date postdate;
	postdate.SetDate ( &(game->GetWorld ()->date) );
	postdate.AdvanceHour ( NumberGenerator::RandomNumber ( 96 ) * -1 );
	postdate.AdvanceMinute ( NumberGenerator::RandomNumber ( 60 ) * -1 );

	//
	// Insert the mission
	//

	auto *mission = new Mission ();
	mission->SetTYPE		 ( MISSION_FINDDATA );
	mission->SetCompletion   ( completionA, completionB, completionC, "", "" );
	mission->SetEmployer     ( employer->name );
	mission->SetContact      ( personname );
	mission->SetPayment      ( payment, (int) ( payment * 1.1 ) );
	mission->SetDifficulty   ( difficulty );
	mission->SetMinRating    ( minrating );
	mission->SetAcceptRating ( acceptrating );
	mission->SetDescription  ( description );
	mission->SetDetails		 ( details );
	mission->SetFullDetails  ( fulldetails.str () );
	mission->SetWhySoMuchMoney ( "The data is highly personal, and well protected" );
	if ( !game->IsRunning () ) mission->SetCreateDate   ( &postdate );
	mission->SetWhoIsTheTarget ( whoisthetarget );
	mission->GiveLink ( target->ip );

	fulldetails.rdbuf()->freeze( false );
	//delete [] fulldetails.str ();

	CompanyUplink *cu = (CompanyUplink *) game->GetWorld ()->GetCompany ( "Uplink" );
	UplinkAssert ( cu )
	cu->CreateMission ( mission );

	return mission;

}

Mission *MissionGenerator::Generate_ChangeData ( Company *employer )
{

	// Decide which type of changedata mission to make this

	int missiontype = NumberGenerator::RandomNumber ( 3 );

	Mission *mission = nullptr;

	switch ( missiontype ) {
		case 0	:		mission = Generate_ChangeData_AcademicRecord ( employer );		break;
		case 1  :		mission = Generate_ChangeData_SocialSecurity ( employer );		break;
		case 2  :		mission = Generate_ChangeData_CriminalRecord ( employer );		break;
	}

	return mission;

}

Mission *MissionGenerator::Generate_ChangeData_AcademicRecord ( Company *employer )
{

	UplinkAssert ( employer )

	int difficulty = (int) NumberGenerator::RandomNormalNumber ( MINDIFFICULTY_MISSION_CHANGEDATA_ACADEMIC, DIFFICULTY_MISSION_VARIANCE );
	if ( difficulty < MINDIFFICULTY_MISSION_CHANGEDATA_ACADEMIC ) difficulty = MINDIFFICULTY_MISSION_CHANGEDATA_ACADEMIC;

	Computer *target = game->GetWorld ()->GetComputer ( "International Academic Database" );
	UplinkAssert (target)

	//
	// Set up the basic variables of the mission
	//

	int payment			= (int) NumberGenerator::RandomNormalNumber ( (float) ( difficulty * PAYMENT_MISSION_CHANGEDATA_ACADEMIC ),
	                                                                  (float) ( difficulty * PAYMENT_MISSION_CHANGEDATA_ACADEMIC
	                                                                                       * PAYMENT_MISSION_VARIANCE ) );

	int minrating		= difficulty;
	int acceptrating	= minrating + NumberGenerator::RandomNumber ( 2 );

	if ( minrating > 10 ) minrating = 10;
	if ( acceptrating > 10 ) acceptrating = 10;

	payment = payment / 100 * 100;				// Rounds payment to the nearest 100


	string personname = "internal@" + employer->name + ".net";

	string description;
	std::ostrstream details;
	std::ostrstream fulldetails;

	details << "Payment for this job is " << payment << " credits.\n"
			<< "This job has been assigned an Uplink difficulty of " << difficulty << ".\n"
			<< "An UplinkRating of " << Rating::GetUplinkRatingString ( acceptrating ) << " or above will be sufficient for automatic acceptance.\n\n"
			<< '\x0';

	fulldetails << "Gain access to the International Academic Database:\n"
				<< "TARGET COMPUTER DATA :\n"
				<< "   LOCATION: " << target->name << "\n"
				<< "   IP      : " << target->ip << "\n"
				<< "\n\n";

	Person *person = nullptr;							// Person whom mission affects

	string completionA = target->ip;			// IP
	string completionB = person->name;			// Target person
	string completionC;							// Field to be changed
	string completionD;							// Word that must appear in the field
	string completionE;							// Word that must appear in the field

    string whysomuchmoney;

	int missiontype = NumberGenerator::RandomNumber ( 4 ) + 1;

		/* =========================================================================
			type 1		:		Add a degree
			type 2		:		Improve a degree rating
			type 3		:		Decrease a degree rating
			type 4		:		Add a random qualification
		   ========================================================================= */

	if ( missiontype == 1 ) {		// Add a degree =====================================================

		// Find someone without a degree

		Record *record = target->recordbank.GetRandomRecord ( "University = None" );
		if ( !record ) return nullptr;

		char *personname = record->GetField ( RECORDBANK_NAME );
		UplinkAssert (personname)

		if ( strcmp ( personname, game->GetWorld ()->GetPlayer ()->handle ) == 0 ) return nullptr;

		person = game->GetWorld ()->GetPerson ( personname );
		UplinkAssert (person)

		// Generate a University degree that this person wants

		int degreelevel = 1;
		string degreesubject;

		switch ( NumberGenerator::RandomNumber ( 10 ) ) {

			case 0 : degreesubject = "Computing"; break;
			case 1 : degreesubject = "Physics"; break;
			case 2 : degreesubject = "Electrical Engineering"; break;
			case 3 : degreesubject = "Mechanical Engineering"; break;
			case 4 : degreesubject = "Software Engineering"; break;
			case 5 : degreesubject = "Systems Engineering"; break;
			case 6 : degreesubject = "Management"; break;
			case 7 : degreesubject = "Artificial Intelligence"; break;
			case 8 : degreesubject = "Philosophy"; break;
			case 9 : degreesubject = "Media studies"; break;

			default:
				UplinkAbort ( "MissionGenerator::Generate_ChangeData_EmploymentRecord, error in Degree Type" )

		}

		description = "Generate a University Degree for a friend";

		fulldetails	<< "Provide this person with the following University Degree:\n"
					<< "UNIVERSITY DEGREE :\n"
					<< "   SUBJECT : " << degreesubject << "\n"
					<< "   CLASS   : " << degreelevel << "\n";
		completionC = "University";
		completionD = degreesubject;
		completionE = to_string(degreelevel);

        whysomuchmoney = "We really need this qualification";

	}
	else if ( missiontype == 2 ) {	// Improve a degree rating ==========================================

		// Find someone with a degree that is not a 1st

		Record *record = target->recordbank.GetRandomRecord ( "University ! None ; University - 1" );
		if ( !record ) return nullptr;

		char *personname = record->GetField ( RECORDBANK_NAME );
		UplinkAssert (personname)

		if ( strcmp ( personname, game->GetWorld ()->GetPlayer ()->handle ) == 0 ) return nullptr;

		person = game->GetWorld ()->GetPerson ( personname );
		UplinkAssert (person)

		description = "Improve our associates' academic standing";
		fulldetails << "This person requires a first class degree.\n";
		completionC = "University";
		completionD = "1";
		completionE = "1";						// Always needs a 1st

        whysomuchmoney = "This qualification will be very useful to us";

	}
	else if ( missiontype == 3 ) {	// Decrease a degree rating ==========================================

		// Find someone with a 1st

		Record *record = target->recordbank.GetRandomRecord ( "University ! None ; University + 1" );
		if ( !record ) return nullptr;

		char *personname = record->GetField ( RECORDBANK_NAME );
		UplinkAssert (personname)

		if ( strcmp ( personname, game->GetWorld ()->GetPlayer ()->handle ) == 0 ) return nullptr;

		person = game->GetWorld ()->GetPerson ( personname );
		UplinkAssert (person)

		description = "Help us prove a fellow employee is over-rated";
		fulldetails << "We feel this person scored slightly too highly at his degree."
					<< "Give him a 3rd class replacement.\n";
		completionC = "University";
		completionD = "3";
		completionE = "3";						// Always needs a 3rd

        whysomuchmoney = "We really dont like the target";

	}
	else if ( missiontype == 4 ) {	// Add a random qualification =======================================

		string qualification;

		switch ( NumberGenerator::RandomNumber ( 5 ) ) {

			case 0 : qualification = "Diploma in Computing";		break;
			case 1 : qualification = "IEEE Accreditation";			break;
			case 2 : qualification = "Certified systems engineer";	break;
			case 3 : qualification = "Btec in IT";					break;
			case 4 : qualification = "Registered Uplink Agent";	break;

			default:
				UplinkAbort ( "MissionGenerator::Generate_ChangeData_EmploymentRecord, error in Qualification Type" )

		}

		// Find someone who doesn't have this qualification

		string query = "Other - " + qualification;
		Record *record = target->recordbank.GetRandomRecord ( query );
		if ( !record ) return nullptr;

		char *personname = record->GetField ( RECORDBANK_NAME );
		UplinkAssert (personname)

		if ( strcmp ( personname, game->GetWorld ()->GetPlayer ()->handle ) == 0 ) return nullptr;

		person = game->GetWorld ()->GetPerson ( personname );
		UplinkAssert (person)

		description = "Qualifications required for wealthy professional";
		fulldetails << "This individual urgently requires a \n"
					<< "'" << qualification << "' Qualification adding to his record.";
		completionC = "Other";
		completionD = qualification;
		completionE = qualification;

        whysomuchmoney = "This qualification is needed to gain entry";

	}
	else {

		UplinkAbort ( "Unrecognised mission type" )

	}


	//
	// Generate the remaining fields of the mission
	//

	fulldetails	<< "\nTARGET INDIVIDUAL :\n"
				<< "   NAME    : " << person->name << "\n"
				<< "\n\n"
				<< "Send a notice of completion to\n"
				<< personname << "\n"
				<< "\n"
				<< "END"
				<< '\x0';

	string whoisthetarget = "He's just a friend of ours.";

	Date postdate;
	postdate.SetDate ( &(game->GetWorld ()->date) );
	postdate.AdvanceHour ( NumberGenerator::RandomNumber ( 96 ) * -1 );
	postdate.AdvanceMinute ( NumberGenerator::RandomNumber ( 60 ) * -1 );

	//
	// Insert the mission
	//

	auto *mission = new Mission ();
	mission->SetTYPE		 ( MISSION_CHANGEDATA );
	mission->SetCompletion   ( completionA, completionB, completionC, completionD, completionE );
	mission->SetEmployer     ( employer->name );
	mission->SetContact      ( personname );
	mission->SetPayment      ( payment, (int) ( payment * 1.1 ) );
	mission->SetDifficulty   ( difficulty );
	mission->SetMinRating    ( minrating );
	mission->SetAcceptRating ( acceptrating );
	mission->SetDescription  ( description );
	mission->SetDetails		 ( details.str () );
	mission->SetFullDetails  ( fulldetails.str () );
	mission->SetWhySoMuchMoney ( whysomuchmoney );
	if ( !game->IsRunning () )	mission->SetCreateDate   ( &postdate );
	mission->SetWhoIsTheTarget ( whoisthetarget );
	mission->GiveLink ( target->ip );

	details.rdbuf()->freeze( false );
	fulldetails.rdbuf()->freeze( false );
	//delete [] details.str ();
	//delete [] fulldetails.str ();

	CompanyUplink *cu = (CompanyUplink *) game->GetWorld ()->GetCompany ( "Uplink" );
	UplinkAssert ( cu )
	cu->CreateMission ( mission );

	return mission;

}

Mission *MissionGenerator::Generate_ChangeData_SocialSecurity ( Company *employer )
{

	UplinkAssert ( employer )

	int difficulty = (int) NumberGenerator::RandomNormalNumber ( MINDIFFICULTY_MISSION_CHANGEDATA_SOCIAL, DIFFICULTY_MISSION_VARIANCE );
	if ( difficulty < MINDIFFICULTY_MISSION_CHANGEDATA_SOCIAL ) difficulty = MINDIFFICULTY_MISSION_CHANGEDATA_SOCIAL;

	Computer *target = game->GetWorld ()->GetComputer ( "International Social Security Database" );
	UplinkAssert (target)

	//
	// Set up the basic variables of the mission
	//

	int payment			= (int) NumberGenerator::RandomNormalNumber ( (float) ( difficulty * PAYMENT_MISSION_CHANGEDATA_SOCIAL ),
	                                                                  (float) ( difficulty * PAYMENT_MISSION_CHANGEDATA_SOCIAL
	                                                                                       * PAYMENT_MISSION_VARIANCE ) );

	int minrating		= difficulty;
	int acceptrating	= minrating + NumberGenerator::RandomNumber ( 2 );

	if ( minrating > 10 ) minrating = 10;
	if ( acceptrating > 10 ) acceptrating = 10;

	payment = payment / 100 * 100;				// Rounds payment to the nearest 100


	string personname = "internal@" + employer->name + ".net";

	string description;
	std::ostrstream details;
	std::ostrstream fulldetails;

	details << "Payment for this job is " << payment << " credits.\n"
			<< "This job has been assigned an Uplink difficulty of " << difficulty << ".\n"
			<< "An UplinkRating of " << Rating::GetUplinkRatingString ( acceptrating ) << " or above will be sufficient for automatic acceptance.\n\n"
			<< '\x0';

	fulldetails << "Break into the International Social Security Database:\n"
				<< "TARGET COMPUTER DATA :\n"
				<< "   LOCATION: " << target->name << "\n"
				<< "   IP      : " << target->ip << "\n"
				<< "\n\n";

	Person *person = nullptr;							// Person whom mission affects

	string completionA = target->ip;				// IP
	char completionB [SIZE_PERSON_NAME];			// Target person
	string completionC;								// Field to be changed
	string completionD;								// Word that must appear in the field
	string completionE;								// Word that must appear in the field


	int missiontype = NumberGenerator::RandomNumber ( 2 ) + 1;

	/* =========================================================================
		type 1		:		Make someone deceased
		type 2		:		Resurrect a dead person
	   ========================================================================= */

	if ( missiontype == 1 ) {

		// Find someone who isn't dead

		Record *record = target->recordbank.GetRandomRecord ( "Personal Status ! Deceased" );
		if ( !record ) return nullptr;

		char *personname = record->GetField ( RECORDBANK_NAME );
		UplinkAssert (personname)

		if ( strcmp ( personname, game->GetWorld ()->GetPlayer ()->handle ) == 0 ) return nullptr;

		person = game->GetWorld ()->GetPerson ( personname );
		UplinkAssert (person)

		description = "Falsify a Social Security document";
		fulldetails << "The man below is living too easily.  Change his Personal Status to read 'Deceased'.\n";
		completionC = "Personal Status";
		completionD = "Deceased";
		completionE = "Deceased";

	}
	else if ( missiontype == 2 ) {

		// Find someone who is dead

		Record *record = target->recordbank.GetRandomRecord ( "Personal Status = Deceased" );
		if ( !record ) return nullptr;

		char *personname = record->GetField ( RECORDBANK_NAME );
		UplinkAssert (personname)

		if ( strcmp ( personname, game->GetWorld ()->GetPlayer ()->handle ) == 0 ) return nullptr;

		person = game->GetWorld ()->GetPerson ( personname );
		UplinkAssert (person)

		description = "Create part of a new identity";
		fulldetails << "The man below recently passed away and we wish to use his identity ourselves. "
					   "Change his Personal Status to 'Employed'.\n";
		completionC = "Personal Status";
		completionD = "Employed";
		completionE = "Employed";

	}

	//
	// Generate the remaining fields of the mission
	//

	UplinkStrncpy ( completionB, person->name, sizeof ( completionB ) )

	fulldetails	<< "\nTARGET INDIVIDUAL :\n"
				<< "   NAME    : " << person->name << "\n"
				<< "\n\n"
				<< "Send a notice of completion to\n"
				<< personname << "\n"
				<< "\n"
				<< "END"
				<< '\x0';

	string whoisthetarget = "You wouldn't recognise the name.";

	Date postdate;
	postdate.SetDate ( &(game->GetWorld ()->date) );
	postdate.AdvanceHour ( NumberGenerator::RandomNumber ( 96 ) * -1 );
	postdate.AdvanceMinute ( NumberGenerator::RandomNumber ( 60 ) * -1 );

	//
	// Insert the mission
	//

	auto *mission = new Mission ();
	mission->SetTYPE		 ( MISSION_CHANGEDATA );
	mission->SetCompletion   ( completionA, completionB, completionC, completionD, completionE );
	mission->SetEmployer     ( employer->name );
	mission->SetContact      ( personname );
	mission->SetPayment      ( payment, (int) ( payment * 1.1 ) );
	mission->SetDifficulty   ( difficulty );
	mission->SetMinRating    ( minrating );
	mission->SetAcceptRating ( acceptrating );
	mission->SetDescription  ( description );
	mission->SetDetails		 ( details.str () );
	mission->SetFullDetails  ( fulldetails.str () );
	mission->SetWhySoMuchMoney ( "We need to use the Social Security record very soon" );
	mission->SetHowSecure ( "You will need to bypass the Proxy server." );
	if ( !game->IsRunning () )	mission->SetCreateDate   ( &postdate );
	mission->SetWhoIsTheTarget ( whoisthetarget );
	mission->GiveLink ( target->ip );

	details.rdbuf()->freeze( false );
	fulldetails.rdbuf()->freeze( false );
	//delete [] details.str ();
	//delete [] fulldetails.str ();

	CompanyUplink *cu = (CompanyUplink *) game->GetWorld ()->GetCompany ( "Uplink" );
	UplinkAssert ( cu )
	cu->CreateMission ( mission );

	return mission;

}

Mission *MissionGenerator::Generate_ChangeData_CriminalRecord ( Company *employer )
{

	UplinkAssert ( employer )

	int difficulty = (int) NumberGenerator::RandomNormalNumber ( MINDIFFICULTY_MISSION_CHANGEDATA_CRIMINAL, DIFFICULTY_MISSION_VARIANCE );
	if ( difficulty < MINDIFFICULTY_MISSION_CHANGEDATA_CRIMINAL ) difficulty = MINDIFFICULTY_MISSION_CHANGEDATA_CRIMINAL;

	Computer *target = game->GetWorld ()->GetComputer ( "Global Criminal Database" );
	UplinkAssert (target)

	//
	// Set up the basic variables of the mission
	//

	int payment			= (int) NumberGenerator::RandomNormalNumber ( (float) ( difficulty * PAYMENT_MISSION_CHANGEDATA_CRIMINAL ),
	                                                                  (float) ( difficulty * PAYMENT_MISSION_CHANGEDATA_CRIMINAL
	                                                                                       * PAYMENT_MISSION_VARIANCE ) );

	int minrating		= difficulty;
	int acceptrating	= minrating + NumberGenerator::RandomNumber ( 2 );

	if ( minrating > 10 ) minrating = 10;
	if ( acceptrating > 10 ) acceptrating = 10;

	payment = payment / 100 * 100;				// Rounds payment to the nearest 100



	string personname = "internal@" + employer->name + ".net";

	string description;
	std::ostrstream details;
	std::ostrstream fulldetails;

	details << "Payment for this job is " << payment << " credits.\n"
			<< "This job has been assigned an Uplink difficulty of " << difficulty << ".\n"
			<< "An UplinkRating of " << Rating::GetUplinkRatingString ( acceptrating ) << " or above will be sufficient for automatic acceptance.\n\n"
			<< '\x0';

	fulldetails << "Break into the Global Criminal Database:\n"
				<< "TARGET COMPUTER DATA :\n"
				<< "   LOCATION: " << target->name << "\n"
				<< "   IP      : " << target->ip << "\n"
				<< "\n\n";

	Person *person = nullptr;							// Person whom mission affects

	string completionA = target->ip;				// IP
	char completionB [SIZE_PERSON_NAME];			// Target person
	string completionC;								// Field to be changed
	string completionD;								// Word that must appear in the field
	string completionE;								// Word that must appear in the field

	string whoisthetarget;

	int missiontype = NumberGenerator::RandomNumber ( 3 ) + 1;

	/* =========================================================================
		type 1		:		Give standard conviction
		type 2		:		Give computer-related conviction
		type 3		:		Remove a computer-related conviction
	   ========================================================================= */

	if ( missiontype == 1 ) {

		description = "Help to discredit one of our rivals";

		Record *record = target->recordbank.GetRandomRecord ( "Convictions = None" );
		if ( !record ) return nullptr;

		char *personname = record->GetField ( RECORDBANK_NAME );
		UplinkAssert (personname)

		if ( strcmp ( personname, game->GetWorld ()->GetPlayer ()->handle ) == 0 ) return nullptr;

		person = game->GetWorld ()->GetPerson ( personname );
		UplinkAssert (person)

		fulldetails << "We need " << personname << " discredited so that any "
					<< "future checks on his background will come up positive.\n"
					<< "Open his Criminal Record and give him a conviction for "
					<< "Robbery.";

		completionC = "Convictions";
		completionD = "Robbery";
		completionE = "Robbery";

		whoisthetarget = "We wouldn't want to give his name out just yet.";

	}
	else if ( missiontype == 2 ) {

		description = "Help to stop a hacker from ever working again";

		Record *record = target->recordbank.GetRandomRecord ( "Convictions = None" );
		if ( !record ) return nullptr;

		char *personname = record->GetField ( RECORDBANK_NAME );
		UplinkAssert (personname)

		if ( strcmp ( personname, game->GetWorld ()->GetPlayer ()->handle ) == 0 ) return nullptr;

		person = game->GetWorld ()->GetPerson ( personname );
		UplinkAssert (person)

		fulldetails << "We have suspected that " << personname
					<< " was guilty of hacking one of our systems for some time now, "
					<< "but have been unable to aquire sufficient proof.\n"
					<< "We wish to discredit him by giving him a computer related "
					<< "criminal record, preventing him from ever working in the industry again.\n"
					<< "Open his Criminal Record and give him a new conviction for "
					<< "High tech fraud.";

		completionC = "Convictions";
		completionD = "High tech fraud";
		completionE = "High tech fraud";

		whoisthetarget = "A hacker who has attacked us before.";

	}
	else if ( missiontype == 3 ) {

		description = "Clear a criminal record";

		Record *record = target->recordbank.GetRandomRecord ( "Convictions ! None" );
		if ( !record ) return nullptr;

		char *personname = record->GetField ( RECORDBANK_NAME );
		UplinkAssert (personname)

		if ( strcmp ( personname, game->GetWorld ()->GetPlayer ()->handle ) == 0 ) return nullptr;

		person = game->GetWorld ()->GetPerson ( personname );
		UplinkAssert (person)

		fulldetails << "One of our friends in the industry is having some trouble "
					<< "finding employment, due to his criminal record.  We would be "
					<< "grateful if you could remove all record of his criminal past "
					<< "\n\n"
					<< "Target individual : " << personname;

		completionC = "Convictions";
		completionD = "None";
		completionE = "None";

		whoisthetarget = "Just a friend of ours who needs help.";

	}

	//
	// Generate the remaining fields of the mission
	//

	UplinkStrncpy ( completionB, person->name, sizeof ( completionB ) )

	fulldetails	<< "\nSend a notice of completion to\n"
				<< personname << "\n"
				<< "\n"
				<< "END"
				<< '\x0';

	Date postdate;
	postdate.SetDate ( &(game->GetWorld ()->date) );
	postdate.AdvanceHour ( NumberGenerator::RandomNumber ( 96 ) * -1 );
	postdate.AdvanceMinute ( NumberGenerator::RandomNumber ( 60 ) * -1 );

	//
	// Insert the mission
	//

	auto *mission = new Mission ();
	mission->SetTYPE		 ( MISSION_CHANGEDATA );
	mission->SetCompletion   ( completionA, completionB, completionC, completionD, completionE );
	mission->SetEmployer     ( employer->name );
	mission->SetContact      ( personname );
	mission->SetPayment      ( payment, (int) ( payment * 1.1 ) );
	mission->SetDifficulty   ( difficulty );
	mission->SetMinRating    ( minrating );
	mission->SetAcceptRating ( acceptrating );
	mission->SetDescription  ( description );
	mission->SetDetails		 ( details.str () );
	mission->SetFullDetails  ( fulldetails.str () );
	if ( missiontype == 1 || missiontype == 2 ) mission->SetWhySoMuchMoney ( "The target may have friends." );
	if ( missiontype == 3 ) mission->SetWhySoMuchMoney ( "The police may come after you." );
	mission->SetHowSecure ( "The GCD uses a Proxy security system." );
	if ( !game->IsRunning () ) mission->SetCreateDate   ( &postdate );
	mission->SetWhoIsTheTarget ( whoisthetarget );
	mission->GiveLink ( target->ip );

	details.rdbuf()->freeze( false );
	fulldetails.rdbuf()->freeze( false );
	//delete [] details.str ();
	//delete [] fulldetails.str ();

	CompanyUplink *cu = (CompanyUplink *) game->GetWorld ()->GetCompany ( "Uplink" );
	UplinkAssert ( cu )
	cu->CreateMission ( mission );

	return mission;

}

Mission *MissionGenerator::Generate_FrameUser ( Company *employer )
{

	UplinkAssert (employer)

	//
	// Choose a random person to frame
	//

	Person *poorsod = WorldGenerator::GetRandomPerson ();

	return Generate_FrameUser ( employer, poorsod );

}

Mission *MissionGenerator::Generate_TraceUser ( Company *employer )
{

	/*
		This mission is never randomly generated.
		It always occurs as a consequence of somebody doing something like
		hacking a bank etc.

		*/

	return nullptr;

}

Mission *MissionGenerator::Generate_TraceUser_MoneyTransfer ( Company *employer, BankComputer *source, BankComputer *target,
															 BankAccount *sacc, BankAccount *tacc, int amount, Person *hacker )
{

	UplinkAssert (employer)
	UplinkAssert (source)
	UplinkAssert (target)
	UplinkAssert (sacc)
	UplinkAssert (tacc)
	UplinkAssert (hacker)

	//
	// Set up the basic variables of the mission
	//

	int difficulty = (int) NumberGenerator::RandomNormalNumber ( MINDIFFICULTY_MISSION_TRACEUSER_BANKFRAUD, DIFFICULTY_MISSION_VARIANCE );
	if ( difficulty < MINDIFFICULTY_MISSION_TRACEUSER_BANKFRAUD ) difficulty = MINDIFFICULTY_MISSION_TRACEUSER_BANKFRAUD;

	int payment			= (int) NumberGenerator::RandomNormalNumber ( (float) ( difficulty * PAYMENT_MISSION_TRACEUSER ),
	                                                                  (float) ( difficulty * PAYMENT_MISSION_TRACEUSER
	                                                                                       * PAYMENT_MISSION_VARIANCE ) );

	int minrating		= difficulty;
	int acceptrating	= minrating + NumberGenerator::RandomNumber ( 3 );

	if ( minrating > 10 ) minrating = 10;
	if ( acceptrating > 10 ) acceptrating = 10;

	payment = payment / 100 * 100;				// Rounds payment to the nearest 100


	string personname = "internal@" + employer->name + ".net";


	string description = "Trace an unauthorised money transfer";
	std::ostrstream details;
	std::ostrstream fulldetails;


	details << "Payment for this job is " << payment << " credits.\n"
			<< "This job has been assigned an Uplink difficulty of " << difficulty << ".\n"
			<< "An UplinkRating of " << Rating::GetUplinkRatingString ( acceptrating ) << " or above will be sufficient for automatic acceptance.\n\n"
			<< '\x0';

	fulldetails << "One of our bank accounts was recently hacked, and funds were transfered to an unknown account."
				<< "Use any methods necessary to trace this hacker.\n"
				<< "Our compromised account is at:\n"
				<< source->name << "\n"
				<< " IP    : " << source->ip << "\n"
				<< " ACCNO : " << sacc->accountnumber << "\n"
				<< "The access code to the hacked account is " << sacc->password << "\n"
				<< "(The username is the account number)\n"
				<< "The amount transfered was " << amount << " credits.\n"
				<< "This occurred around " << game->GetWorld ()->date.GetShortString ()
				<< "\n\n"
				<< "You are authorised to access the compromised account."
				<< "\n\n"
				<< "Once you have traced the hacker, send his name to us at\n"
				<< personname << " and we will take matters further if necessary.\n"
				<< "\n"
				<< "END"
				<< '\x0';

	string accesscode;
    Computer::GenerateAccessCode(sacc->accountnumber, sacc->password, accesscode);

	Date postdate;
	postdate.SetDate ( &(game->GetWorld ()->date) );
	postdate.AdvanceHour ( NumberGenerator::RandomNumber ( 96 ) * -1 );
	postdate.AdvanceMinute ( NumberGenerator::RandomNumber ( 60 ) * -1 );

	string whoisthetarget = "That's for you to find out.";

	//
	// Insert the mission
	//

	auto *mission = new Mission ();
	mission->SetTYPE		 ( MISSION_TRACEUSER );
	mission->SetCompletion   ( hacker->name, source->ip, tacc->name, "", "" );
	mission->SetEmployer     ( employer->name );
	mission->SetContact      ( personname );
	mission->SetPayment      ( payment, (int) ( payment * 1.1 ) );
	mission->SetDifficulty   ( difficulty );
	mission->SetMinRating    ( minrating );
	mission->SetAcceptRating ( acceptrating );
	mission->SetDescription  ( description );
	mission->SetDetails		 ( details.str () );
	mission->SetFullDetails  ( fulldetails.str () );
	if ( !game->IsRunning () ) mission->SetCreateDate   ( &postdate );
	mission->GiveLink ( source->ip );
	mission->GiveCode ( source->ip, accesscode );
	mission->SetWhySoMuchMoney ( "Bank records are notoriously hard to crack" );
    mission->SetHowSecure ( "Banks are always well defended." );
	mission->SetWhoIsTheTarget ( whoisthetarget );

	if ( hacker == game->GetWorld ()->GetPlayer () ) mission->SetNpcPriority ( true );

	details.rdbuf()->freeze( false );
	fulldetails.rdbuf()->freeze( false );
	//delete [] details.str ();
	//delete [] fulldetails.str ();

	CompanyUplink *cu = (CompanyUplink *) game->GetWorld ()->GetCompany ( "Uplink" );
	UplinkAssert ( cu )
	cu->CreateMission ( mission );

	return mission;

}

Mission *MissionGenerator::Generate_PayFine (Person *person, Company *company, int amount, Date *duedate, const string &reason )
{

	UplinkAssert (person)
	UplinkAssert (company)
	UplinkAssert (duedate)

	//
	// Locate a target bank account
	//

	auto *bank = (BankComputer *) WorldGenerator::GetRandomComputer ( COMPUTER_TYPE_PUBLICBANKSERVER );
	UplinkAssert (bank)
	BankAccount *account = bank->GetRandomAccount ();
	UplinkAssert (account)

	//
	// Look up a contact at the company
	//

	string personname = "internal@" + company->name + ".net";

	//
	// Fill in the fields of the mission
	//

	char completionA [64];							// Person name who pays the fine
	string completionB = bank->ip + " " + to_string(account->accountnumber);	// (Target account) IP ACCNO
	string completionC = to_string(amount);			// Amount to transfer

	UplinkStrncpy ( completionA, person->name, sizeof ( completionA ) )
	if ( strcmp ( person->name, "PLAYER" ) == 0 ) UplinkStrncpy ( completionA, game->GetWorld ()->GetPlayer ()->handle, sizeof ( completionA ) )

	//
	// Create the descriptive text
	//

	string description = "Pay a " + to_string(amount) + "c fine";

	std::ostrstream fulldetails;
	fulldetails << reason
				<< "\n\n"
				<< "As a result of this action you are legally required to pay a fine to the following account. "
				<< "Failure to pay this fine before the due date could result in imprisonment."
				<< "\n\n"
				<< "BANK : " << bank->name << "\n"
				<< "IP : " << bank->ip << "\n"
				<< "ACCNO : " << account->accountnumber << "\n"
				<< "FINE : " << amount << " credits" << "\n"
				<< "BEFORE : " << duedate->GetLongString () << "\n"
				<< "\n\n"
                << "To pay this fine, access your financial screen and click on the account you wish to transfer from.\n"
                << "This will connect you to that account.\n"
                << "Then select 'transfer' from the menu and fill in the details as above.\n\n"
				<< "Once you have paid this fine, send a mail to "
				<< personname << " to confirm receipt.\n"
				<< "END." << '\x0';

	Date postdate;
	postdate.SetDate ( &(game->GetWorld ()->date) );
	postdate.AdvanceHour ( NumberGenerator::RandomNumber ( 96 ) * -1 );
	postdate.AdvanceMinute ( NumberGenerator::RandomNumber ( 60 ) * -1 );

	//
	// Build the mission
	//


	auto *mission = new Mission ();
	mission->SetTYPE		 ( MISSION_PAYFINE );
	mission->SetCompletion   ( completionA, completionB, completionC, "", "" );
	mission->SetDueDate		 ( duedate );
	mission->SetEmployer     ( company->name );
	mission->SetContact      ( personname );
	mission->SetPayment      ( 0 );
	mission->SetDifficulty   ( 0 );
	mission->SetMinRating    ( 0 );
	mission->SetAcceptRating ( 0 );
	mission->SetDescription  ( description );
	mission->SetDetails		 ( reason );
	mission->SetFullDetails  ( fulldetails.str () );
	if ( !game->IsRunning () ) mission->SetCreateDate   ( &postdate );

	fulldetails.rdbuf()->freeze( false );
	//delete [] fulldetails.str ();

	return mission;

}

Mission *MissionGenerator::Generate_FrameUser ( Company *employer, Person *person )
{

	UplinkAssert (employer)
	UplinkAssert (person)

	int difficulty = (int) NumberGenerator::RandomNormalNumber ( MINDIFFICULTY_MISSION_FRAMEUSER, DIFFICULTY_MISSION_VARIANCE );
	if ( difficulty < MINDIFFICULTY_MISSION_FRAMEUSER )
		difficulty = MINDIFFICULTY_MISSION_FRAMEUSER;
	else if ( difficulty > 10 )
		difficulty = 10;


	//
	// Set up the basic variables of the mission
	//

	int payment			= (int) NumberGenerator::RandomNormalNumber ( (float) ( difficulty * PAYMENT_MISSION_FRAMEUSER ),
	                                                                  (float) ( difficulty * PAYMENT_MISSION_FRAMEUSER
	                                                                                       * PAYMENT_MISSION_VARIANCE ) );

	int minrating		= difficulty;
	int acceptrating	= minrating + NumberGenerator::RandomNumber ( 3 );

	if ( minrating > 10 ) minrating = 10;
	if ( acceptrating > 10 ) acceptrating = 10;

	payment = payment / 100 * 100;				// Rounds payment to the nearest 100


	string personname = "internal@" + employer->name + ".net";


	string description;
	std::ostrstream details;
	std::ostrstream fulldetails;

	char completionA [SIZE_MISSION_COMPLETION];									// Target name
	string completionB;															// Type of crime
	string completionC;															// IP of target (if applicable)


	UplinkStrncpy ( completionA, person->name, sizeof ( completionA ) )

	details << "Payment for this job is " << payment << " credits.\n"
			<< "This job has been assigned an Uplink difficulty of " << difficulty << ".\n"
			<< "An UplinkRating of " << Rating::GetUplinkRatingString ( acceptrating ) << " or above will be sufficient for automatic acceptance.\n\n"
			<< '\x0';


	int missiontype = NumberGenerator::RandomNumber ( 3 ) + 1;

	/* =================================================================
		  type 1	:		Frame a user for hacking
		  type 2	:		Frame a user for financial fraud
		  type 3	:		Frame a user for destruction of data
       ================================================================= */

	if ( missiontype == 1 ) {									// Frame a user for hacking

		description = "Frame a user for computer crimes";

		fulldetails << "A member of the public has been interfering with our plans recently "
					   "and we believe his arrest and conviction for computer crimes will put "
					   "his operation at an end.\n"
					   "Perform a break in on a high-priority computer system and ensure that "
					   "this man is charged with the crime.  He must be jailed for several years.\n";

		completionB = "HACKING";

	}
	else if ( missiontype == 2 ) {

		description = "Frame a man for bank fraud";

		auto *bank = (BankComputer *) WorldGenerator::GetRandomComputer ( COMPUTER_TYPE_PUBLICBANKSERVER );
		UplinkAssert (bank)

		fulldetails << "We need this man to be arrested and charged for breaking into a bank and "
					   "attempting some kind of financial fraud.\n"
					   "We would like you to use the bank server specified below.\n\n"
					   "Perform some form of illegal operation (such as disabling some of their security systems "
                       "and making sure they notice you) then modify the records "
					   "so that he is incriminated.\n"
					   "\n\nTARGET BANK:\n"
					   << bank->name << "\n"
					   "IP: " << bank->ip << "\n";

		completionB = "FINANCIAL";
		completionC = bank->ip;

		payment = (int) ( payment * 1.2 );

	}
	else if ( missiontype == 3 ) {

		description = "Frame a user for destruction of data.";

		Computer *target = WorldGenerator::GetRandomComputer ( COMPUTER_TYPE_INTERNALSERVICESMACHINE );
		UplinkAssert (target)

		fulldetails << "Our corporation has recently found a way to kill two birds with one "
					   "proverbial stone.  We wish to destroy some valuable research data, "
					   "whilst at the same time incriminating one of our long standing enemies for the crime.\n\n"
					   "Break into the computer below and destroy all data files.  Then modify the logs "
					   "so that this man is charged with the crime and sent to jail.\n"
					   "\n\nTARGET COMPUTER:\n"
					   << target->name << "\n"
					   "IP: " << target->ip << "\n";

		completionB = "DESTROYALL";
		completionC = target->ip;

		payment = (int) ( payment * 1.2 );

	}

	//
	// Fill in the rest of the mission
	//

	fulldetails << "\n"
				<< "Target individual :\n"
				<< person->name << "\n"
                << "His local system : \n"
                << person->localhost << "\n\n"
				<< "Once you have completed your mission and his arrest is in the news, send a mail "
				<< "to " << personname << " and we will transfer the payment.\n"
				<< "You will only receive payment if he is charged for the crime we have outlined.\n"
				<< "Good luck.\n"
				<< '\x0';


	Date postdate;
	postdate.SetDate ( &(game->GetWorld ()->date) );
	postdate.AdvanceHour ( NumberGenerator::RandomNumber ( 96 ) * -1 );
	postdate.AdvanceMinute ( NumberGenerator::RandomNumber ( 60 ) * -1 );

	//
	// Build the mission
	//

	auto *m = new Mission ();
	m->SetTYPE		   ( MISSION_FRAMEUSER );
	// TODO: Check if this will work, as it seems like `1` may be passed
	m->SetCompletion   ( completionA, completionB, missiontype == 1 ? "" : completionC, "", "" );
	m->SetEmployer     ( employer->name );
	m->SetContact      ( personname );
	m->SetPayment      ( payment, (int) ( payment * 1.1 ) );
	m->SetDifficulty   ( difficulty );
	m->SetMinRating    ( minrating );
	m->SetAcceptRating ( acceptrating );
	m->SetDescription  ( description );
	m->SetDetails	   ( details.str () );
	m->SetFullDetails  ( fulldetails.str () );
	m->SetWhySoMuchMoney ( "His employer and friends may come after you." );
	m->SetWhoIsTheTarget ( "Someone we don't like very much" );

	if ( missiontype == 1 ) m->SetHowSecure ( "That depends on what you frame him for." );
	else if ( missiontype == 2 ) m->SetHowSecure ( "Banks always have very high security." );

    m->GiveLink ( person->localhost );
	if ( missiontype == 2 || missiontype == 3 ) m->GiveLink ( completionC );

	details.rdbuf()->freeze( false );
	fulldetails.rdbuf()->freeze( false );
	//delete [] details.str ();
	//delete [] fulldetails.str ();

	CompanyUplink *cu = (CompanyUplink *) game->GetWorld ()->GetCompany ( "Uplink" );
	UplinkAssert ( cu )
	cu->CreateMission ( m );

	return m;

}

Mission *MissionGenerator::Generate_TraceHacker ( Mission *completedmission, Person *hacker )
{

	UplinkAssert ( completedmission )
	UplinkAssert ( hacker )

	//
	// Look up the computer that was hacked
	//

	string hackedip = completedmission->links.GetData (0);
	VLocation *vl = game->GetWorld ()->GetVLocation ( hackedip );
	UplinkAssert (vl)
	Computer *comp = vl->GetComputer ();
	UplinkAssert (comp)

	/*

		Generate a follow up mission to trace the bastard that
		did the hack

		*/

	return Generate_TraceHacker ( comp, hacker );

}

Mission *MissionGenerator::Generate_TraceHacker	( Computer *hacked, Person *hacker )
{

	UplinkAssert (hacked)
	UplinkAssert (hacker)

	int difficulty = (int) NumberGenerator::RandomNormalNumber ( MINDIFFICULTY_MISSION_TRACEUSER, DIFFICULTY_MISSION_VARIANCE );
	if ( difficulty < MINDIFFICULTY_MISSION_TRACEUSER ) difficulty = MINDIFFICULTY_MISSION_TRACEUSER;

	int payment			= (int) NumberGenerator::RandomNormalNumber ( (float) ( difficulty * PAYMENT_MISSION_TRACEUSER ),
	                                                                  (float) ( difficulty * PAYMENT_MISSION_TRACEUSER
	                                                                                       * PAYMENT_MISSION_VARIANCE ) );

	int minrating = difficulty;
	int acceptrating	= minrating + NumberGenerator::RandomNumber ( 3 );

	if ( minrating > 10 ) minrating = 10;
	if ( acceptrating > 10 ) acceptrating = 10;

	payment = payment / 100 * 100;				// Rounds payment to the nearest 100

	bool provideaccount = (hacked->TYPE == COMPUTER_TYPE_INTERNALSERVICESMACHINE);

	string password;
	char username [12];

	if ( provideaccount ) {

		//
		// Add in a new account for the player to use
		//

        password = NameGenerator::GeneratePassword ();
		UplinkSnprintf ( username, sizeof ( username ), "temp%c%c%c%c", 'a' + NumberGenerator::RandomNumber ( 26 ),
																		'a' + NumberGenerator::RandomNumber ( 26 ),
																		'a' + NumberGenerator::RandomNumber ( 26 ),
																		'a' + NumberGenerator::RandomNumber ( 26 ) )

		auto *record = new Record ();
		record->AddField ( RECORDBANK_NAME, username );
		record->AddField ( RECORDBANK_PASSWORD, password );
		record->AddField ( RECORDBANK_SECURITY, "3" );
		hacked->recordbank.AddRecord ( record );

	}

	//
	// Look up the owner
	//

	string companyname = hacked->companyname;

	string contact = "internal@" + companyname + ".net";

	//
	// Fill in the text fields
	//

	string description = "Trace a hacker who recently broke into our systems";
	std::ostrstream details;
	std::ostrstream fulldetails;

	details << "Payment for this job is " << payment << " credits.\n"
			<< "This job has been assigned an Uplink difficulty of " << difficulty << ".\n"
			<< "An UplinkRating of " << Rating::GetUplinkRatingString ( acceptrating ) <<
			   " or above will be sufficient for automatic acceptance.\n\n"
			<< '\x0';

	fulldetails << "Our computer systems were recently compromised by a computer hacker.\n"
				   "Find this hacker and send his name to us.  We will deal with him in our "
				   "own way.\n\n"
				   "The compromised system is :\n"
				   << hacked->name << "\n"
				   << "IP : " << hacked->ip << "\n\n"
				   "The hack occurred around " << game->GetWorld ()->date.GetShortString () << ".\n";

	if ( provideaccount ) {

		   fulldetails <<	"We have provided you with level 3 clearence to our system.\n"
							"Your username is " << username << "\n"
							"And your password is " << password << "\n\n";

    }

    fulldetails << "Once completed, send the name of the hacker to "
				<< contact << " and payment will be delivered.\n"
				   "END"
				<< '\x0';

	auto *m = new Mission ();
	m->SetTYPE		 ( MISSION_TRACEUSER );
	m->SetCompletion   ( hacker->name, hacked->ip, "", "", "" );
	m->SetEmployer     ( companyname );
	m->SetContact      ( contact );
	m->SetPayment      ( payment, (int) ( payment * 1.1 ) );
	m->SetDifficulty   ( difficulty );
	m->SetMinRating    ( minrating );
	m->SetAcceptRating ( acceptrating );
	m->SetDescription  ( description );
	m->SetDetails	   ( details.str () );
	m->SetFullDetails  ( fulldetails.str () );
	m->GiveLink        ( hacked->ip );

	m->SetWhySoMuchMoney ( "The hacker will certainly have covered his tracks" );
    m->SetHowSecure ( "You may have to hack several targets." );
	m->SetWhoIsTheTarget ( "That is for you to determine." );

    if ( provideaccount ) {
	    string code;
        Computer::GenerateAccessCode(username, password, code);
	    m->GiveCode ( hacked->ip, code );
    }

	if ( hacker == game->GetWorld ()->GetPlayer () ) m->SetNpcPriority ( true );

	details.rdbuf()->freeze( false );
	fulldetails.rdbuf()->freeze( false );
	//delete [] details.str ();
	//delete [] fulldetails.str ();

	CompanyUplink *cu = (CompanyUplink *) game->GetWorld ()->GetCompany ( "Uplink" );
	UplinkAssert ( cu )
	cu->CreateMission ( m );

	return m;

}

Mission *MissionGenerator::Generate_ChangeAccount ( Company *employer, Computer *source, Computer *target )
{

	int difficulty = (int) NumberGenerator::RandomNormalNumber ( MINDIFFICULTY_MISSION_CHANGEACCOUNT, DIFFICULTY_MISSION_VARIANCE );
	if ( difficulty < MINDIFFICULTY_MISSION_CHANGEACCOUNT ) difficulty = MINDIFFICULTY_MISSION_CHANGEACCOUNT;

	//
	// The employer wants you to transfer money between the two bank computers;
	// source and target.  Both account numbers are random, but obviously can't be the same.
	//

	UplinkAssert ( employer )
	UplinkAssert ( source )
	UplinkAssert ( target )
	assert( source->GetOBJECTID () == OID_BANKCOMPUTER );
	assert( target->GetOBJECTID () == OID_BANKCOMPUTER );

	//
	// Set up the basic variables of the mission
	//

	int payment			= (int) NumberGenerator::RandomNormalNumber ( (float) ( difficulty * PAYMENT_MISSION_CHANGEACCOUNT ),
	                                                                  (float) ( difficulty * PAYMENT_MISSION_CHANGEACCOUNT
	                                                                                       * PAYMENT_MISSION_VARIANCE ) );

	int minrating		= difficulty;
	int acceptrating	= minrating + NumberGenerator::RandomNumber ( 3 );

	if ( minrating > 10 ) minrating = 10;
	if ( acceptrating > 10 ) acceptrating = 10;

	payment = payment / 100 * 100;				// Rounds payment to the nearest 100


	string personname = "internal@" + employer->name + ".net";

	//
	// Choose random bank accounts on the
	// source and target machines
	//

	BankAccount *sourceaccount = ((BankComputer *) source)->GetRandomAccount ();
	BankAccount *targetaccount = ((BankComputer *) target)->GetRandomAccount ();

	UplinkAssert ( sourceaccount )
	UplinkAssert ( targetaccount )

	//
	// Make sure they aren't player accounts
	//

	if ( sourceaccount->name == game->GetWorld ()->GetPlayer ()->handle ) return nullptr;
	if ( targetaccount->name == game->GetWorld ()->GetPlayer ()->handle ) return nullptr;


	int amount_to_transfer = (int) NumberGenerator::RandomNormalNumber ( sourceaccount->balance / 2.0f, sourceaccount->balance * 0.4f );
	amount_to_transfer = amount_to_transfer / 500 * 500;

    if ( amount_to_transfer <= 0 ) return nullptr;
    if ( amount_to_transfer > sourceaccount->balance ) amount_to_transfer = (int) ( sourceaccount->balance * 0.9 );

	//
	// Fill in the fields of the mission
	//

	string completionA = source->ip + " " + to_string(sourceaccount->accountnumber); // (source account) IP AccNo
	string completionB = target->ip + " " + to_string(targetaccount->accountnumber); // (Target account) IP ACCNO
	string completionC = to_string(amount_to_transfer);							// Amount to transfer
	string completionD = to_string(sourceaccount->balance);						// Amount in source account
	string completionE = to_string(targetaccount->balance);						// Amount in target account
	
	string description = "Our esteemed colleague wishes to make a donation.";
	std::ostrstream fulldetails;

	string details = "Payment for this job is " + to_string(payment) + " credits.\n"
                     "This job has been assigned an Uplink difficulty of " + to_string(difficulty) + ".\n"
                     "An UplinkRating of " + Rating::GetUplinkRatingString ( acceptrating ) + " or above will be sufficient for automatic acceptance.\n\n";

	fulldetails << "Break into the following well known financial institute:\n"
				<< "TARGET COMPUTER DATA :\n"
				<< "   LOCATION: " << source->name << "\n"
				<< "   IP      : " << source->ip << "\n"
				<< "\n\n"
				<< "Gain access to account number " << sourceaccount->accountnumber << ",\n"
				<< "which is in the name of " << sourceaccount->name << ".\n"
				<< "Transfer " << amount_to_transfer << " credits to the following account:\n"
				<< "   LOCATION: " << target->name << "\n"
				<< "   IP      : " << target->ip << "\n"
				<< "   ACCNO   : " << targetaccount->accountnumber << "\n"
				<< "\n\n"
				<< "Send a notice of completion to\n"
				<< personname << "\n"
				<< "\n"
				<< "END"
				<< '\x0';

	Date postdate;
	postdate.SetDate ( &(game->GetWorld ()->date) );
	postdate.AdvanceHour ( NumberGenerator::RandomNumber ( 96 ) * -1 );
	postdate.AdvanceMinute ( NumberGenerator::RandomNumber ( 60 ) * -1 );

	string whoisthetarget = source->name;

	//
	// Insert the mission
	//

	auto *mission = new Mission ();
	mission->SetTYPE		 ( MISSION_CHANGEACCOUNT );
	mission->SetCompletion   ( completionA, completionB, completionC, completionD, completionE );
	mission->SetEmployer     ( employer->name );
	mission->SetContact      ( personname );
	mission->SetPayment      ( payment, (int) ( payment * 1.1 ) );
	mission->SetDifficulty   ( difficulty );
	mission->SetMinRating    ( minrating );
	mission->SetAcceptRating ( acceptrating );
	mission->SetDescription  ( description );
	mission->SetDetails		 ( details );
	mission->SetFullDetails  ( fulldetails.str () );
	mission->SetWhySoMuchMoney ( "Financial systems are always well guarded." );
	mission->SetHowSecure ( "The bank will be using Proxys and Monitors." );
	if ( !game->IsRunning () ) mission->SetCreateDate   ( &postdate );
	mission->GiveLink ( source->ip );
	mission->SetWhoIsTheTarget ( whoisthetarget );

	fulldetails.rdbuf()->freeze( false );
	//delete [] fulldetails.str ();

	CompanyUplink *cu = (CompanyUplink *) game->GetWorld ()->GetCompany ( "Uplink" );
	UplinkAssert ( cu )
	cu->CreateMission ( mission );

	return mission;

}

Mission *MissionGenerator::Generate_RemoveComputer ( Company *employer, Computer *target )
{

	int difficulty = (int) NumberGenerator::RandomNormalNumber ( MINDIFFICULTY_MISSION_REMOVECOMPUTER, DIFFICULTY_MISSION_VARIANCE );
	if ( difficulty < MINDIFFICULTY_MISSION_REMOVECOMPUTER ) difficulty = MINDIFFICULTY_MISSION_REMOVECOMPUTER;

	UplinkAssert ( employer )
	UplinkAssert ( target )

	//
	// Set up the basic variables of the mission
	//

	int payment			= (int) NumberGenerator::RandomNormalNumber ( (float) ( difficulty * PAYMENT_MISSION_REMOVECOMPUTER ),
	                                                                  (float) ( difficulty * PAYMENT_MISSION_REMOVECOMPUTER
	                                                                                       * PAYMENT_MISSION_VARIANCE ) );

	int minrating		= difficulty;
	int acceptrating	= minrating + NumberGenerator::RandomNumber ( 3 );

	if ( minrating > 10 ) minrating = 10;
	if ( acceptrating > 10 ) acceptrating = 10;

	payment = payment / 100 * 100;				// Rounds payment to the nearest 100


	string personname = "internal@" + employer->name + ".net";

	//
	// Fill in the fields
	//

	string description = "Elite agent required for destruction of computer system";
	std::ostrstream details;
	std::ostrstream fulldetails;

	details << "Payment for this job is " << payment << " credits.\n"
			<< "This job has been assigned an Uplink difficulty of " << difficulty << ".\n"
			<< "An UplinkRating of " << Rating::GetUplinkRatingString ( acceptrating ) << " or above will be sufficient for automatic acceptance.\n\n"
			<< '\x0';

	fulldetails << "One of our competitors has made the critical mistake of becoming over dependent "
				   "on one of their primary computer systems, and we intend to take advantage of this oversight.\n"
				   "We want you to put the computer system below out of action, permenantly.\n"
				   "Use any means necessary, including virus attacks or system resets, but all data must be "
				   "destroyed and the system itself must be shut down.\n\n"
				   "Target : " << target->name << "\n"
				   "IP : " << target->ip << "\n"
				   "Once you have completed the task, send a mail to "
				   << personname << " and we will determine if you have "
				   "been successful.\n"
				   "END" <<
				   '\x0';


	string completionA = target->ip;

	Date postdate;
	postdate.SetDate ( &(game->GetWorld ()->date) );
	postdate.AdvanceHour ( NumberGenerator::RandomNumber ( 96 ) * -1 );
	postdate.AdvanceMinute ( NumberGenerator::RandomNumber ( 60 ) * -1 );

	string whoisthetarget = "The system is owned by " + target->companyname;

	//
	// Insert the mission
	//

	auto *mission = new Mission ();
	mission->SetTYPE		 ( MISSION_REMOVECOMPUTER );
    mission->SetCompletion   ( completionA, "", "", "", "" );
	mission->SetEmployer     ( employer->name );
	mission->SetContact      ( personname );
	mission->SetPayment      ( payment, (int) ( payment * 1.1 ) );
	mission->SetDifficulty   ( difficulty );
	mission->SetMinRating    ( minrating );
	mission->SetAcceptRating ( acceptrating );
	mission->SetDescription  ( description );
	mission->SetDetails		 ( details.str () );
	mission->SetFullDetails  ( fulldetails.str () );
	mission->SetWhySoMuchMoney ( "The computer system will be heavily defended." );
	mission->GiveLink ( target->ip );
	mission->SetWhoIsTheTarget ( whoisthetarget );
	if ( !game->IsRunning () ) mission->SetCreateDate   ( &postdate );

	details.rdbuf()->freeze( false );
	fulldetails.rdbuf()->freeze( false );
	//delete [] details.str ();
	//delete [] fulldetails.str ();

	CompanyUplink *cu = (CompanyUplink *) game->GetWorld ()->GetCompany ( "Uplink" );
	UplinkAssert ( cu )
	cu->CreateMission ( mission );

	return mission;


}

Mission *MissionGenerator::Generate_RemoveCompany ( Company *employer )
{
	return nullptr;
}

Mission *MissionGenerator::Generate_RemoveUser ( Company *employer )
{

	int difficulty = (int) NumberGenerator::RandomNormalNumber ( MINDIFFICULTY_MISSION_REMOVEUSER, DIFFICULTY_MISSION_VARIANCE );
	if ( difficulty < MINDIFFICULTY_MISSION_REMOVEUSER ) difficulty = MINDIFFICULTY_MISSION_REMOVEUSER;


	UplinkAssert ( employer )

	//
	// Set up the basic variables of the mission
	//

	int payment			= (int) NumberGenerator::RandomNormalNumber ( (float) ( difficulty * PAYMENT_MISSION_REMOVEUSER ),
	                                                                  (float) ( difficulty * PAYMENT_MISSION_REMOVEUSER
	                                                                                       * PAYMENT_MISSION_VARIANCE ) );

	int minrating		= difficulty;
	int acceptrating	= minrating + NumberGenerator::RandomNumber ( 3 );

	if ( minrating > 10 ) minrating = 10;
	if ( acceptrating > 10 ) acceptrating = 10;

	payment = payment / 100 * 100;				// Rounds payment to the nearest 100


	string personname = "internal@" + employer->name + ".net";

	//
	// Find the target person
	// (Never the player)
	//

	Person *target = WorldGenerator::GetRandomPerson ();
	if ( target == game->GetWorld ()->GetPlayer () ) return nullptr;

	//
	// Fill in the fields
	//

	string description = "Highly skilled Agent required for removal job";
	std::ostrstream details;
	std::ostrstream fulldetails;

	details << "Payment for this job is " << payment << " credits.\n"
			<< "This job has been assigned an Uplink difficulty of " << difficulty << ".\n"
			<< "An UplinkRating of " << Rating::GetUplinkRatingString ( acceptrating ) << " or above will be sufficient for automatic acceptance.\n\n"
			<< '\x0';

	fulldetails << "Recently this man has become quite an annoyance and "
				   "we would like him removed.  We do not care how you do it, "
				   "as long as his life is destroyed by the time you have "
				   "finished.  \n\n"
				   "TARGET : " << target->name << "\n\n"
				   "Once you have completed the task, send a mail to "
				   << personname << " and we will determine if you have "
				   "been successful.\n"
				   "END" <<
				   '\x0';


	char completionA [SIZE_MISSION_COMPLETION];
	UplinkStrncpy ( completionA, target->name, sizeof ( completionA ) )

	Date postdate;
	postdate.SetDate ( &(game->GetWorld ()->date) );
	postdate.AdvanceHour ( NumberGenerator::RandomNumber ( 96 ) * -1 );
	postdate.AdvanceMinute ( NumberGenerator::RandomNumber ( 60 ) * -1 );

	//
	// Insert the mission
	//

	auto *mission = new Mission ();
	mission->SetTYPE		 ( MISSION_REMOVEUSER );
	mission->SetCompletion   ( completionA, "", "", "", "" );
	mission->SetEmployer     ( employer->name );
	mission->SetContact      ( personname );
	mission->SetPayment      ( payment, (int) ( payment * 1.1 ) );
	mission->SetDifficulty   ( difficulty );
	mission->SetMinRating    ( minrating );
	mission->SetAcceptRating ( acceptrating );
	mission->SetDescription  ( description );
	mission->SetDetails		 ( details.str () );
	mission->SetFullDetails  ( fulldetails.str () );
	mission->SetWhySoMuchMoney ( "A great deal of skill will be required." );
	mission->SetHowSecure ( "That depends on the method you use." );
	if ( !game->IsRunning () ) mission->SetCreateDate   ( &postdate );

	details.rdbuf()->freeze( false );
	fulldetails.rdbuf()->freeze( false );
	//delete [] details.str ();
	//delete [] fulldetails.str ();

	CompanyUplink *cu = (CompanyUplink *) game->GetWorld ()->GetCompany ( "Uplink" );
	UplinkAssert ( cu )
	cu->CreateMission ( mission );

	return mission;

}

bool MissionGenerator::IsMissionComplete ( Mission *mission, Person *person, Message *message )
{

	UplinkAssert (mission)

	switch ( mission->TYPE ) {

		case MISSION_STEALFILE:				return IsMissionComplete_StealFile	    ( mission, person, message );		break;
		case MISSION_DESTROYFILE:			return IsMissionComplete_DestroyFile    ( mission, person, message );		break;
		case MISSION_FINDDATA:				return IsMissionComplete_FindFinancial  ( mission, person, message );		break;
		case MISSION_CHANGEDATA:			return IsMissionComplete_ChangeData		( mission, person, message );		break;
		case MISSION_FRAMEUSER:				return IsMissionComplete_FrameUser		( mission, person, message );		break;
		case MISSION_TRACEUSER:				return IsMissionComplete_TraceUser		( mission, person, message );		break;
		case MISSION_CHANGEACCOUNT:			return IsMissionComplete_ChangeAccount  ( mission, person, message );		break;
		case MISSION_REMOVECOMPUTER:		return IsMissionComplete_RemoveComputer ( mission, person, message );		break;
		case MISSION_REMOVEUSER:			return IsMissionComplete_RemoveUser		( mission, person, message );		break;
		case MISSION_PAYFINE:				return IsMissionComplete_PayFine		( mission, person, message );		break;

        case MISSION_SPECIAL:               return IsMissionComplete_Special        ( mission, person, message );       break;

		default:
			UplinkAbort ( "Unknown Mission Type" )

	}

	return false;

}

bool MissionGenerator::IsMissionComplete_StealFile ( Mission *mission, Person *person, Message *message )
{

	UplinkAssert (mission)

	string ip		= mission->completionA;
	string datatitle = mission->completionB;					// Required data title

	if ( datatitle == "ALL" )
		return IsMissionComplete_StealAllFiles ( mission, person, message );


	// Data must be uncompressed and not encrypted

	Data *msgdata = message->GetData ();

	if ( msgdata
		 && msgdata->title == datatitle
		 && msgdata->encrypted == 0 ) {

		MissionCompleted ( mission, person, message );
		return true;

	}
	else if ( msgdata
		 && msgdata->title == datatitle
		 && msgdata->encrypted > 0 ) {

		// Message is encrypted

		string reason = "Your mission is not yet complete.\n"
                        "The data you sent us is encrypted with level " + to_string(msgdata->encrypted) +
                        " encryption, which we are unable to break.\n\n"
                        "Please break the encryption on the file and send it too us again.\n"
                        "You will not be paid until this has been accomplished.";

		MissionNotCompleted ( mission, person, message, reason );

		return false;

	}
/*
	else if ( msgdata
		 && strcmp ( msgdata->title, datatitle ) == 0
		 && msgdata->compressed > 0 ) {

		// Message is compressed

		std::ostrstream reason;
		reason << "Your mission is not yet complete.\n"
			   << "The data you sent us is compressed with level " << msgdata->compressed
			   << " compression, which we are unable to decode.\n"
			   << "\nPlease decompress the file and send it too us again.\n"
			   << "You will not be paid until this has been accomplished."
			   << '\x0';

		MissionNotCompleted ( mission, person, message, reason.str () );

		reason.rdbuf()->freeze( 0 );
		//delete [] reason.str ();
		return false;

	}
*/
	else {

		MissionNotCompleted ( mission, person, message, "You don't appear to have attached the file." );
		return false;

	}

}

bool MissionGenerator::IsMissionComplete_StealAllFiles ( Mission *mission, Person *person, Message *message )
{

	UplinkAssert (mission)
	UplinkAssert (person)
	UplinkAssert (message)

	string ip		= mission->completionA;
	string datatitle = mission->completionB;					// ALL
	string ourcompip = mission->completionC;
	int numfiles, datasize;
	sscanf ( mission->completionD, "%d %d", &numfiles, &datasize );
	char *datatype	= mission->completionE;

	//
	// Look up the target computer
	//

	VLocation *vltarget = game->GetWorld ()->GetVLocation ( ip );
	UplinkAssert ( vltarget )
	Computer *target = vltarget->GetComputer ();
	UplinkAssert (target)

	//
	// Work out the filenames that should be here
	//

	string missiontypestring;

	if      ( datatype == "SCIENTIFIC" ) {   missiontypestring = "rsrch";
	} else if	( datatype == "RESEARCH" ) { missiontypestring = "rsrch";         // Not needed anymore, but older user files may have this
	} else if ( datatype == "CORPORATE" ) {  missiontypestring = "corp";
	} else if ( datatype == "CUSTOMER" ) {   missiontypestring = "custmr";
	} else if ( datatype == "SOFTWARE" ) {   missiontypestring = "softw";
	} else {	UplinkAbort ( "Unrecognised data type" )
	}

	string stolendatatitle = target->companyname.substr(4) + "-" + missiontypestring;

	//
	// Lookup the dump computer
	//

	VLocation *vlourcomp = game->GetWorld ()->GetVLocation ( ourcompip );
	UplinkAssert (vlourcomp)
	Computer *ourcomp = vlourcomp->GetComputer ();
	UplinkAssert (ourcomp)

	//
	// Work out how much of the data has been successfully stolen
	// Ignore duplicate files or anything that doesn't have the
	// correct filename
	//

	int foundsize = 0;
	bool foundencrypted = false;

	bool *filefound = new bool [numfiles];
	for ( int fi = 0; fi < numfiles; ++fi )
		filefound [fi] = false;

	for ( int i = 0; i < ourcomp->databank.GetDataSize (); ++i ) {
		if ( ourcomp->databank.GetDataFile (i) ) {

			Data *thisfile = ourcomp->databank.GetDataFile (i);
			UplinkAssert (thisfile)

			if ( thisfile->title.find(stolendatatitle) != string::npos ) {

				if ( thisfile->encrypted == 0 ) {

					int thisfileindex;

                    size_t lastspace = thisfile->title.find_last_of(' ');
                    if ( lastspace != string::npos ) {

                        string tmp = thisfile->title.substr(lastspace + 1);
                        tmp = tmp.substr(0, tmp.length()-4);
                        thisfileindex = stoi(tmp);

						if ( thisfileindex >= 0 && thisfileindex < numfiles &&
                             !filefound[thisfileindex]) {

							foundsize += thisfile->size;
							filefound [thisfileindex] = true;

						}

					}

				}
				else {

					foundencrypted = true;

				}

			}

		}
	}

	//
	// If they've got less than half, they've failed
	// Otherwise, base their remaining payment on the
	// fraction that they successfully downloaded.
	//

	if ( foundsize == 0 ) {

		if ( foundencrypted )
			MissionNotCompleted ( mission, person, message, "We can see some files on the fileserver but they "
															"appear to be encrypted.  You need to decrypt them "
															"before we make payment." );

		else
			MissionNotCompleted ( mission, person, message, "We can't find any of the files on the "
															"file server we specified." );
		return false;

	}
	else if ( foundsize < datasize / 2 ) {

		if ( foundencrypted )
			MissionNotCompleted ( mission, person, message, "You haven't recovered very much data and a lot of it "
															"is encrypted - please copy more and decrypt all of it." );

		else
			MissionNotCompleted ( mission, person, message, "You haven't copied very much of the data - "
															"we'll need more before we pay you." );

		return false;

	}
	else if ( foundsize == datasize ) {

		MissionCompleted ( mission, person, message );

		// Shut down the file server
		// TODO

		return true;

	}
	else {

		float fractionfound = (float) foundsize / (float) datasize;
		mission->payment = int (fractionfound * mission->payment);
		mission->payment = 100 * (mission->payment / 100);
		if ( mission->payment < 100 ) mission->payment = 100;

		string newmessage = "We've reviewed your recent mission to copy data files.\n"
                            "It looks like you got most of the files but some are missing.\n"
                            "We've decided to reduce your remaining payment to " + to_string(mission->payment) + "c.";

		auto *m = new Message ();
		m->SetTo ( "PLAYER" );
		m->SetFrom ( person->name );
		m->SetSubject ( "Your recent mission" );
		m->SetBody ( newmessage );
		m->Send ();

		MissionCompleted ( mission, person, message );

		// Shut down the file server
		// TODO

		return true;

	}

}

bool MissionGenerator::IsMissionComplete_DestroyFile ( Mission *mission, Person *person, Message *message )
{

	UplinkAssert (mission)

	string ip = mission->completionA;
	string filename = mission->completionB;

	VLocation *vl = game->GetWorld ()->GetVLocation (ip);
	UplinkAssert (vl)

	if ( filename == "ALL" ) {

		CompanyUplink *uplink = (CompanyUplink *) game->GetWorld ()->GetCompany ( "Uplink" );
		UplinkAssert (uplink)

		for ( int in = 0; in < uplink->news.Size (); ++in ) {

			News *news = uplink->news.GetData (in);
			UplinkAssert (news)

			// method 1 - Delete all files

			if ( news->NEWSTYPE == NEWS_TYPE_DELETED && news->data1 && news->data1 == ip ) {

				MissionCompleted ( mission, person, message );
				return true;

			}

			// method 2 - Destroy the entire computer

			if ( news->NEWSTYPE == NEWS_TYPE_COMPUTERDESTROYED && news->data1 && news->data1 == ip ) {

				MissionCompleted ( mission, person, message );
				return true;

			}

		}

		MissionNotCompleted ( mission, person, message, "We have not heard word yet that the damage has been done." );
		return false;

	}
	else {

		Data *data = vl->GetComputer ()->databank.GetData ( filename );

		if ( !data ) {

			MissionCompleted ( mission, person, message );
			return true;

		}
		else {

			MissionNotCompleted ( mission, person, message, "According to our sources, the file has not yet been deleted." );
			return false;

		}

	}

}

bool MissionGenerator::IsMissionComplete_FindData ( Mission *mission, Person *person, Message *message )
{

	UplinkAssert (mission)

	string value1 = LowerCaseString ( mission->completionA );
	string value2 = LowerCaseString ( mission->completionB );
	string value3 = LowerCaseString ( mission->completionC );
	string value4 = LowerCaseString ( mission->completionD );
	string value5 = LowerCaseString ( mission->completionE );

	// All 5 values need to be in the message
	// (Unless they are nullptr)

	string msgbody = LowerCaseString ( message->GetBody () );
	bool success = true;

	if ( !value1.empty() && msgbody.find( value1 ) == string::npos ) success = false;
	if ( !value2.empty() && msgbody.find( value2 ) == string::npos ) success = false;
	if ( !value3.empty() && msgbody.find( value3 ) == string::npos ) success = false;
	if ( !value4.empty() && msgbody.find( value4 ) == string::npos ) success = false;
	if ( !value5.empty() && msgbody.find( value5 ) == string::npos ) success = false;

	value1 = value2 = value3 = value4 = value5 = msgbody = "";

	if ( success ) {

		MissionCompleted ( mission, person, message );
		return true;

	}
	else {

		MissionNotCompleted ( mission, person, message, "This data appears to be invalid.  Please verify it." );
		return false;

	}

}

bool MissionGenerator::IsMissionComplete_FindFinancial  ( Mission *mission, Person *person, Message *message )
{

    UplinkAssert (mission)
    UplinkAssert (message)

    // CompletionA = sourceIP accountNum
    // CompletionB = Field (balance, loan etc)
    // CompletionC = name of person responsible (if type == find source of transfer)

	char source_ip [SIZE_VLOCATION_IP];
	int source_acc;
	sscanf ( mission->completionA, "%s %d", source_ip, &source_acc );
	string source_acc_s = to_string(source_acc);

    char *field = mission->completionB;

	BankAccount *source_account = BankAccount::GetAccount ( source_ip, source_acc_s );
	UplinkAssert (source_account)

    if ( strcmp ( field, "Balance" ) == 0 ) {

        int actualbalance = source_account->balance;
        string sbalance = to_string(actualbalance);
        if ( message->GetBody().find(sbalance) != string::npos ) {

            MissionCompleted ( mission, person, message );
            return true;

        }
        else {

            MissionNotCompleted ( mission, person, message, "Could you please verify your results.  We have reason to believe they are not accurate." );
            return false;

        }

    }
    else if ( strcmp ( field, "Loan" ) == 0 ) {

        int actualloan = source_account->loan;
        string sloan = to_string(actualloan);
        if ( message->GetBody().find(sloan) != string::npos ) {

            MissionCompleted ( mission, person, message );
            return true;

        }
        else {

            MissionNotCompleted ( mission, person, message, "Could you please verify your results.  We have reason to believe they are not accurate." );
            return false;

        }

    }
    else if ( field == "TraceTransfer" ) {

        string personresponsible = LowerCaseString (mission->completionC);
        assert(!personresponsible.empty());

		string messagebody = LowerCaseString(message->GetBody ());

		bool ispersonresponsible = ( messagebody.find( personresponsible ) != string::npos );

		personresponsible = messagebody = "";

		if ( ispersonresponsible ) {

            MissionCompleted ( mission, person, message );
            return true;

        } else {

            MissionNotCompleted ( mission, person, message, "Could you please verify your results.  We have reason to believe that person is not responsible." );
            return false;

        }

    }
    else {

        UplinkAbort ( "Error in IsMissionComplete_FindFinancial" )
        return false;

    }

}

bool MissionGenerator::IsMissionComplete_ChangeData	( Mission *mission, Person *person, Message *message )
{

	UplinkAssert (mission)

	string ip			= mission->completionA;
	string personname	= mission->completionB;
	string fieldname	= mission->completionC;

	VLocation *vl = game->GetWorld ()->GetVLocation (ip);
	UplinkAssert (vl)
	Computer *comp = vl->GetComputer ();
	UplinkAssert (comp)

	Record *rec = comp->recordbank.GetRecordFromName ( personname );

	if ( rec ) {

		// This is the record that should have been jimmied
		bool foundStrings = false;
		char *fieldvalue = rec->GetField ( fieldname );

		if ( fieldvalue ) {

			string string1 = mission->completionD ? LowerCaseString ( mission->completionD ) : "";
			string string2 = mission->completionE ? LowerCaseString ( mission->completionE ) : "";

			string fieldvaluelower = LowerCaseString ( fieldvalue );
			foundStrings = ( fieldvaluelower.find( string1 ) != string::npos && fieldvaluelower.find( string2 ) != string::npos );
			fieldvaluelower = "";

			string1 = string2 = "";

		}

		if ( foundStrings ) {

			// Mission Accomplished
			MissionCompleted ( mission, person, message );
			return true;

		}
		else {

			MissionNotCompleted ( mission, person, message, "You haven't changed the data yet" );
			return false;

		}

	}

	// Oh dear.  The record hasn't been found, but it should be there

	UplinkAbort ( "MissionGenerator::IsMissionComplete_ChangeData - could not find record" )
	return false;				// Keeps the compiler happy

}

bool MissionGenerator::IsMissionComplete_FrameUser ( Mission *mission, Person *person, Message *message )
{

	UplinkAssert (mission)
	UplinkAssert (person)
	UplinkAssert (message)

	string personname	= mission->completionA;
	string framedfor	= mission->completionB;
	string ip			= mission->completionC;

	//
	// Has the person been arrested?
	// Look through recent news articles
	//

	bool arrested = false;

	CompanyUplink *uplink = (CompanyUplink *) game->GetWorld ()->GetCompany ( "Uplink" );
	UplinkAssert (uplink)

	for ( int i = 0; i < uplink->news.Size (); ++i ) {

		News *news = uplink->news.GetData (i);
		UplinkAssert (news)

		if ( news->NEWSTYPE == NEWS_TYPE_ARREST && news->data1 && news->data1 == personname ) {

			if ( framedfor == "FINANCIAL" || framedfor == "DESTROYALL" ) {

				// Make sure he was arrested for hacking this computer
				arrested = (news->data2 && !ip.empty() && news->data2 == ip);
				break;

			}
			else {

				// Could have hacked anywhere - ip not important
				arrested = true;
				break;

			}

		}

	}

	if ( !arrested ) {

		MissionNotCompleted ( mission, person, message, "That person has not yet been arrested.  We will not make payment until then." );
		return false;

	}
	else {

		// They have been arrested - but was the crime commited?

		if ( framedfor == "HACKING" ) {								// Bog standard hack

			if ( message->from == "PLAYER" )
				game->GetWorld ()->GetPlayer ()->score_peoplefucked ++;

			MissionCompleted ( mission, person, message );
			return true;

		}
		else if ( framedfor == "FINANCIAL" ) {						// Financial fraud

			for ( int in = 0; in < uplink->news.Size (); ++in ) {

				News *news = uplink->news.GetData (in);
				UplinkAssert (news)

				if ( news->NEWSTYPE == NEWS_TYPE_HACKED && news->data1 && !ip.empty() && news->data1 == ip ) {

					if ( message->from == "PLAYER" )
						game->GetWorld ()->GetPlayer ()->score_peoplefucked ++;

					MissionCompleted ( mission, person, message );
					return true;

				}

			}

		}
		else if ( framedfor == "DESTROYALL" ) {						// Destroying all files

			for ( int in = 0; in < uplink->news.Size (); ++in ) {

				News *news = uplink->news.GetData (in);
				UplinkAssert (news)

				if ( news->NEWSTYPE == NEWS_TYPE_DELETED && news->data1 && !ip.empty() && news->data1 == ip ) {

					if ( message->from == "PLAYER" ) {
						game->GetWorld ()->GetPlayer ()->score_peoplefucked ++;
						game->GetWorld ()->GetPlayer ()->score_systemsfucked ++;
					}

					MissionCompleted ( mission, person, message );
					return true;

				}

			}

		}

		// If we get here then the crime wasn't committed

		MissionFailed ( mission, person, "The person was arrested but not for the crimes we specifically asked for.  As such we will not be making payment." );
		return false;

	}

}

bool MissionGenerator::IsMissionComplete_TraceUser ( Mission *mission, Person *person, Message *message )
{

	string personname    = LowerCaseString (mission->completionA);
	string targetaccount = mission->completionC ?
                            LowerCaseString (mission->completionC) :
                            "";                                       // empty string unless this is a TraceUser_MoneyTransfer
    string msgbody       = LowerCaseString ( message->GetBody () );

	// Check the person's name is in the mail

	bool success = false;

	if ( msgbody.find( personname ) != string::npos ) {

		if ( message->from == "PLAYER" )
			game->GetWorld ()->GetPlayer ()->score_peoplefucked ++;

		MissionCompleted ( mission, person, message );
		success = true;

	}
	else {

        if ( !targetaccount.empty() && msgbody.find( targetaccount ) != string::npos ) {

            MissionNotCompleted ( mission, person, message, "We've determined that the money was transferred into "
                                                            "an account owned by that person, but we do not believe "
                                                            "he was personally responsible.\n\n"
                                                            "We want the name of the Hacker who performed the transfer." );
        }
        else
		    MissionNotCompleted ( mission, person, message, "This person was not responsible." );

		success = false;

	}

	personname = targetaccount = msgbody = "";

	return success;

}

bool MissionGenerator::IsMissionComplete_ChangeAccount ( Mission *mission, Person *person, Message *message )
{

	char source_ip [SIZE_VLOCATION_IP];
	char target_ip [SIZE_VLOCATION_IP];
	int source_acc;
	int target_acc;
	int amount = stoi(mission->completionC);
	int source_balance = stoi(mission->completionD);
	int target_balance = stoi(mission->completionE);

	UplinkAssert (mission)

	sscanf ( mission->completionA, "%s %d", source_ip, &source_acc );
	sscanf ( mission->completionB, "%s %d", target_ip, &target_acc );

	string source_acc_s = to_string(source_acc);
	string target_acc_s = to_string(target_acc);

	// Check the money has been transferred

	BankAccount *source_account = BankAccount::GetAccount ( source_ip, source_acc_s );
	UplinkAssert (source_account)

	BankAccount *target_account = BankAccount::GetAccount ( target_ip, target_acc_s );
	UplinkAssert (target_account)

	bool hasLogs = source_account->HasTransferOccurred(source_ip, target_ip, target_acc, amount, true);

	if ( hasLogs || ( ( source_balance - amount ) >= source_account->balance && ( target_balance + amount ) <= target_account->balance ) ) {

		MissionCompleted ( mission, person, message );
		return true;

	}
	else if ( source_balance != source_account->balance && target_balance != target_account->balance ) {

		MissionNotCompleted ( mission, person, message, "A transfer seem to have occurred, verify the amount transfered." );

	}
	else {

		MissionNotCompleted ( mission, person, message, "The transfer has not occurred." );

	}

	return false;

}

bool MissionGenerator::IsMissionComplete_RemoveComputer ( Mission *mission, Person *person, Message *message )
{

	UplinkAssert (mission)
	UplinkAssert (person)
	UplinkAssert (message)

	string sourceip = mission->completionA;
	VLocation *vl = game->GetWorld ()->GetVLocation ( sourceip );
	UplinkAssert (vl)
	Computer *comp = vl->GetComputer ();
	UplinkAssert (comp)


	//
	// look for word that the system was shut down
	//

	CompanyUplink *uplink = (CompanyUplink *) game->GetWorld ()->GetCompany ( "Uplink" );
	UplinkAssert (uplink)

	for ( int in = 0; in < uplink->news.Size (); ++in ) {

		News *news = uplink->news.GetData (in);
		UplinkAssert (news)

		if ( news->NEWSTYPE == NEWS_TYPE_COMPUTERDESTROYED &&
			 news->data1 && news->data1 == sourceip ) {

			// Data bank formatted
			// System shut down

			if ( message->from == "PLAYER" )
				game->GetWorld ()->GetPlayer ()->score_systemsfucked ++;

			MissionCompleted ( mission, person, message );
			return true;

		}
		else if ( news->NEWSTYPE == NEWS_TYPE_COMPUTERSHUTDOWN &&
			  	  news->data1 && news->data1 == sourceip ) {

			// System shut down
			// But files not deleted

			auto *msg = new Message ();
			msg->SetTo ( message->from );
			msg->SetFrom ( person->name );
			msg->SetSubject ( "Problems with mission completion" );
			msg->SetBody ( "It appears the target system was shut down, but the files were not deleted.\n"
						   "This means the company will most likely be able to recover the data.\n\n"
						   "As such, we will only be paying you half the remaining money.\n\n"
						   "In future, you should delete all files before shutting the system down." );
			msg->Send ();

			mission->payment /= 2;
			MissionCompleted ( mission, person, message );
			return true;

		}

	}

	MissionNotCompleted ( mission, person, message, "We have not heard word yet that the system has been destroyed." );
	return false;

}

bool MissionGenerator::IsMissionComplete_RemoveCompany ( Mission *mission, Person *person, Message *message )
{
	return false;
}

bool MissionGenerator::IsMissionComplete_RemoveUser	( Mission *mission, Person *person, Message *message )
{

	UplinkAssert (mission)
	UplinkAssert (person)
	UplinkAssert (message)

	string personname = mission->completionA;

	//
	// Look through recent news articles
	//

	CompanyUplink *uplink = (CompanyUplink *) game->GetWorld ()->GetCompany ( "Uplink" );
	UplinkAssert (uplink)

	for ( int i = 0; i < uplink->news.Size (); ++i ) {

		News *news = uplink->news.GetData (i);
		UplinkAssert (news)

		if ( news->NEWSTYPE == NEWS_TYPE_ARREST && news->data1 && news->data1 == personname ) {

			if ( message->from == "PLAYER" )
				game->GetWorld ()->GetPlayer ()->score_peoplefucked ++;

			MissionCompleted ( mission, person, message );
			return true;

		}

	}

	MissionNotCompleted ( mission, person, message, "We don't believe you have done sufficient damage yet." );
	return false;

}

bool MissionGenerator::IsMissionComplete_PayFine ( Mission *mission, Person *person, Message *message )
{

	UplinkAssert (mission)
	UplinkAssert (person)
	UplinkAssert (message)

	string finedperson = mission->completionA;

	char target_ip [SIZE_VLOCATION_IP];
	char target_accno [16];
	sscanf ( mission->completionB, "%s %s", target_ip, target_accno );

	int amount;
	sscanf ( mission->completionC, "%d", &amount );

	//
	// Get the target account that should have the money in
	//

	BankAccount *target_account = BankAccount::GetAccount ( target_ip, target_accno );
	UplinkAssert (target_account)

	//
	// Check the money has been transfered
	//

	bool success = target_account->HasTransferOccurred(finedperson, amount);
	if ( !success ) {
		Player *pl = game->GetWorld()->GetPlayer();
		if ( finedperson == pl->handle ) {
			for ( int i = 0; i < pl->accounts.Size (); ++i ) {
				string ip, accno;
				stringstream stream(pl->accounts.GetData(i));
				stream >> ip >> accno;

				BankAccount *ba = BankAccount::GetAccount ( ip, accno );
				if ( ba && ( success = target_account->HasTransferOccurred(ba->name, amount) ) )
					break;
			}
		}
	}

	if ( success ) {

		std::ostrstream body;

		//body << "Payment of your fine has been received for the following offence: \n"
		//	 << mission->GetDetails () << "\n\n"
		//	 << "We hope this has been a sufficient deterent to hacking "
		//		"into our systems in future." << '\x0';

		auto *m = new Message ();
		m->SetTo ( message->from );
		m->SetFrom ( person->name );
		m->SetSubject ( "Fine paid" );
		m->SetBody ( body.str () );
		m->Send ();

		body.rdbuf()->freeze( false );
		//delete [] body.str ();


	}
	else {

		std::ostrstream body;
		body << "We have still not received payment for the following fine: \n"
			 << mission->GetDetails () << "\n\n"
			 << "Please ensure this payment reaches us soon to avoid further action."
			 << '\x0';

		auto *m = new Message ();
		m->SetTo ( message->from );
		m->SetFrom ( person->name );
		m->SetSubject ( "Outstanding fine still not paid" );
		m->SetBody ( body.str () );
		m->Send ();

		body.rdbuf()->freeze( false );
		//delete [] body.str ();

	}

	return success;

}

bool MissionGenerator::IsMissionComplete_Special ( Mission *mission, Person *person, Message *message )
{

    UplinkAssert (mission)

    if ( mission->description == PlotGenerator::SpecialMissionDescription ( SPECIALMISSION_MOLE ) ) {

        // First - has the player already done this?

        if ( game->GetWorld ()->plotgenerator.PlayerCompletedSpecialMission (SPECIALMISSION_MOLE) ) {

	        auto *m = new Message ();
	        m->SetTo ( message->from );
	        m->SetFrom ( person->name );
	        m->SetSubject ( "Mission withdrawn" );
	        m->SetBody ( "It appears you have already sold the information.\n\n"
                         "Since it is now in the public domain, we have withdrawn the mission offer." );
	        m->Send ();

            return true;

        }

        string ourcompip = mission->completionA;

	    //
	    // Lookup the dump computer
	    //

	    VLocation *vlourcomp = game->GetWorld ()->GetVLocation ( ourcompip );
	    UplinkAssert (vlourcomp)
	    Computer *ourcomp = vlourcomp->GetComputer ();
	    UplinkAssert (ourcomp)

	    //
	    // Work out how much of the data has been successfully stolen
	    // Ignore duplicate files or anything that doesn't have the
	    // correct filename
	    //

	    int foundsize = 0;
        bool foundprogram = false;
        int numfiles = 10;

	    bool *filefound = new bool [numfiles];
	    for ( int fi = 0; fi < numfiles; ++fi )
		    filefound [fi] = false;

	    for ( int i = 0; i < ourcomp->databank.GetDataSize (); ++i ) {
		    if ( ourcomp->databank.GetDataFile (i) ) {

			    Data *thisfile = ourcomp->databank.GetDataFile (i);
			    UplinkAssert (thisfile)

			    if ( thisfile->title .find("Uplink_Agent_Data") != string::npos &&
				     thisfile->encrypted == 0 ) {

				    char unused [64];
				    int thisfileindex;

				    sscanf ( thisfile->title.c_str(), "%s %d.dat", unused, &thisfileindex );

				    if ( thisfileindex >= 0 && thisfileindex < numfiles &&
                         !filefound[thisfileindex]) {

					    foundsize += thisfile->size;
					    filefound [thisfileindex] = true;

				    }

			    }

                if ( thisfile->title == "Uplink_Agent_List" &&
                    thisfile->encrypted == 0 ) {

                    foundprogram = true;

                }

		    }
	    }

        //
        // Respond based on the data size found
        //

        if ( foundsize == 0 && !foundprogram) {

            MissionNotCompleted ( mission, person, message, "We don't see any data on our fileserver that contains "
                                                            "the names of any Uplink Agents.  Did you upload and "
                                                            "decrypt the data?" );
            return false;

        }
        else if ( foundsize == 0 && foundprogram) {

            MissionNotCompleted ( mission, person, message, "Looks like you've uploaded the Uplink Agent List program, "
                                                            "but we don't see any data on our fileserver that contains "
                                                            "the names of any Uplink Agents.  The program is useless "
                                                            "without the data.\n\n"
                                                            "Did you upload and decrypt the data?" );
            return false;

        }
        else if ( foundsize > 0 ) {

            int foundpercent = (int) ( 100 * ((float) foundsize / (float) 50) );
            int payment = (int) ( PAYMENT_SPECIALMISSION_MOLE * 0.75 * ((float) foundpercent / (float) 100) );
            if ( foundprogram ) payment += (int) ( PAYMENT_SPECIALMISSION_MOLE * 0.25 );
            payment = 100 * (payment / 100);

            std::ostrstream body;
            body << "Weve just checked our fileserver, and we can see lots of data containing the handles of "
                    "Agents and their real-world names.  This data will come in very handy.\n";

            if ( foundpercent == 100 )
                body << "In the end we were able to achieve almost all of the Uplink Agent names.\n";

            else
                body << "In the end we were able to achieve roughly " << 10 * (foundpercent / 10)
                     << " percent of the Uplink Agent names.\n";

            if ( foundprogram )
                body << "We also noticed you uploaded the List program used by Uplink Corporation.\n";
            else
                body << "Unfortunately you didnt upload the List program used to access the data. This will make "
                        "recovery of the source data more difficult.\n";

            if ( payment == PAYMENT_SPECIALMISSION_MOLE )
                body << "We will therefore be paying you the full " << payment << " credits.\n\n";

            else
                body << "Based on this, we will only be paying you " << payment << " credits for the mission, "
                        "out of an available " << PAYMENT_SPECIALMISSION_MOLE << ".\n\n";

            body << "Once again, congratulations on an incredible hack.\n\nEND";
            body << '\x0';

	        auto *m = new Message ();
	        m->SetTo ( message->from );
	        m->SetFrom ( person->name );
	        m->SetSubject ( "Mission completed" );
	        m->SetBody ( body.str() );
	        m->Send ();

			body.rdbuf()->freeze( false );
			//delete [] body.str ();

            //
            // Money, ratings etc
            //

            game->GetWorld ()->GetPlayer ()->ChangeBalance ( payment, "Services Rendered" );
            game->GetWorld ()->GetPlayer ()->rating.ChangeNeuromancerScore ( -200 );
            game->GetWorld ()->plotgenerator.specialmissionscompleted |= (1 << SPECIALMISSION_MOLE);


            //
            // Schedule the murders for a week from now
            //

            Date rundate;
            rundate.SetDate ( &game->GetWorld ()->date );
            rundate.AdvanceDay ( 7 );

	        auto *ne = new NotificationEvent ();
	        ne->SetTYPE ( NOTIFICATIONEVENT_TYPE_AGENTSONLISTDIE );
	        ne->SetRunDate ( &rundate );
	        game->GetWorld ()->scheduler.ScheduleEvent ( ne );

            return true;

        }

    }

    return false;

}

void MissionGenerator::MissionCompleted ( Mission *mission, Person *person, Message *message )
{

    UplinkAssert (mission)
    UplinkAssert (person)
    UplinkAssert (message)

	std::ostrstream body;
	body << "Congratulations on your success.\n";

	if		( mission->paymentmethod == MISSIONPAYMENT_HALFATSTART )
		body << "Other half of the funds transfered : " << mission->payment / 2 << "c\n";
	else if ( mission->paymentmethod == MISSIONPAYMENT_AFTERCOMPLETION )
		body << "Funds transfered : " << mission->payment << "c\n";

	body << "\n"
		 << "Thankyou for working for " << mission->employer << ".\n\n"
         << "THIS IS AN AUTOMATED RESPONSE.  PLEASE DO NOT REPLY."
		 << '\x0';

	auto *m = new Message ();
	m->SetTo ( message->from );
	m->SetFrom ( person->name );
	m->SetSubject ( "Mission completed" );
	m->SetBody ( body.str () );
	m->Send ();

	body.rdbuf()->freeze( false );
	//delete [] body.str ();

	if		( mission->paymentmethod == MISSIONPAYMENT_HALFATSTART )		game->GetWorld ()->GetPlayer ()->ChangeBalance ( mission->payment / 2, "Anonymous benefactor" );
	else if ( mission->paymentmethod == MISSIONPAYMENT_AFTERCOMPLETION )	game->GetWorld ()->GetPlayer ()->ChangeBalance ( mission->payment, "Anonymous benefactor" );

	game->GetWorld ()->GetPlayer ()->rating.ChangeUplinkScore ( mission->difficulty );
	game->GetWorld ()->GetPlayer ()->rating.ChangeNeuromancerScore ( NEUROMANCERCHANGE [mission->TYPE] );

	ConsequenceGenerator::MissionCompleted ( mission, game->GetWorld ()->GetPlayer () );

#ifdef DEMOGAME
    game->GetWorld ()->demoplotgenerator.PlayerCompletedMission ( mission );
#endif

    // Unselect all of the links on the world map

    for ( int i = 0; i < mission->links.Size (); ++i ) {
        string ip = mission->links.GetData (i);
        assert(!ip.empty());
        VLocation *vl = game->GetWorld ()->GetVLocation (ip);
        UplinkAssert (vl)
        Computer *comp = vl->GetComputer ();
        UplinkAssert (comp)
        if ( comp->istargetable )
            vl->SetDisplayed ( false );
    }

    SgPlaySound ( RsArchiveFileOpen ("sounds/success.wav"), "sounds/success.wav", false );

}

void MissionGenerator::MissionNotCompleted (Mission *mission, Person *person, Message *message, const string &reason )
{

    std::ostrstream body;
    body << reason;
    body << "\n\nTHIS IS AN AUTOMATED RESPONSE.  PLEASE DO NOT REPLY.";
    body << '\x0';

	auto *m = new Message ();
	m->SetTo ( message->from );
	m->SetFrom ( person->name );
	m->SetSubject ( "Mission NOT completed" );
	m->SetBody ( body.str () );
	m->Send ();

	body.rdbuf()->freeze( false );
	//delete [] body.str ();
}

void MissionGenerator::MissionFailed (Mission *mission, Person *person, const string &reason )
{

	UplinkAssert (mission)
	UplinkAssert (person)

	// Knock off some points from the person's score
	game->GetWorld ()->GetPlayer ()->rating.ChangeUplinkScore ( mission->difficulty * -2 );

    if ( mission->TYPE == MISSION_PAYFINE ) {

        ConsequenceGenerator::DidntPayFine ( person, mission );

        auto *m = new Message ();
        m->SetTo ( "PLAYER" );
        m->SetFrom ( mission->employer );
        m->SetSubject ( "Failure to pay fine" );
        m->SetBody ( "You have chosen not to pay a fine. As such, "
                     "legal proceedings will begin against you shortly.\n\n"
                     "You are advised to seek legal assistance." );
        m->Send ();

    }
    else {

	    std::ostrstream body;
	    body << "You have failed the following mission: \n"
		     << mission->description
		     << "\n"
		     << reason
		     << "\n\n"
		     << "In future we will employ someone more qualified.\n\n"
             << "THIS IS AN AUTOMATED RESPONSE.  PLEASE DO NOT REPLY."
             << '\x0';

	    // Send a mail to the person
	    auto *m = new Message ();
	    m->SetTo ( "PLAYER" );
	    m->SetFrom ( mission->employer );
	    m->SetSubject ( "Mission FAILED" );
	    m->SetBody ( body.str () );
	    m->Send ();

		body.rdbuf()->freeze( false );
		//delete [] body.str ();

    }

}

