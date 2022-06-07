#include <cstdio>

#include <strstream>
#include <sstream>

#include "app/globals.h"
#include "app/serialise.h"

#include "game/game.h"
#include "game/data/data.h"

#include "world/world.h"
#include "world/player.h"
#include "world/computer/computer.h"
#include "world/computer/bankcomputer.h"
#include "world/computer/recordbank.h"

#include "world/generator/worldgenerator.h"
#include "world/generator/recordgenerator.h"
#include "world/generator/namegenerator.h"
#include "world/generator/numbergenerator.h"

#include "mmgr.h"


void RecordGenerator::Initialise ()
{
}

void RecordGenerator::GenerateRecords (const string &personname )
{

	Person *person = game->GetWorld ()->GetPerson ( personname );
	UplinkAssert (person)

	int age = person->age;

	GenerateRecord_SocialSecurity ( personname, age );
	GenerateRecord_Criminal		  ( personname, age );
//	GenerateRecord_Medical		  ( personname, age );
	GenerateRecord_Academic		  ( personname, age );
	GenerateRecord_Financial	  ( personname, age );

}

void RecordGenerator::GenerateRecords_Player (const string &playername )
{

	// Social Security Record

	auto *socsec = new Record ();
	socsec->AddField ( RECORDBANK_NAME, playername );
	socsec->AddField ( "D.O.B", "24 - 3 - 1989" );
	socsec->AddField ( "Social Security", "14011401" );
	socsec->AddField ( "Marital Status", "Single" );
	socsec->AddField ( "Personal Status", "Self-Employed" );

	Computer *socdb = game->GetWorld ()->GetComputer ( "International Social Security Database" );
	UplinkAssert ( socdb )
	socdb->recordbank.AddRecordSorted ( socsec );


	// Criminal Record

	auto *crim = new Record ();
	crim->AddField ( RECORDBANK_NAME, playername );
	crim->AddField ( "Convictions", "None" );

	Computer *crimdb = game->GetWorld ()->GetComputer ( "Global Criminal Database" );
	UplinkAssert ( crimdb )
	crimdb->recordbank.AddRecordSorted ( crim );


	// Academic Record

	auto *ac = new Record ();
	ac->AddField ( RECORDBANK_NAME, playername );
	ac->AddField ( "IQ", "172" );
	ac->AddField ( "College", "Maths    -   A\n"
							  "Physics    -    A\n"
							  "Electronics    -   A\n"
							  "Computing    -    A" );
	ac->AddField ( "University", "Computer Science, Class 1" );
	ac->AddField ( "Other", "Registered Uplink Agent" );


	Computer *acdb = game->GetWorld ()->GetComputer ( "International Academic Database" );
	UplinkAssert ( acdb )
	acdb->recordbank.AddRecordSorted ( ac );

}

void RecordGenerator::GenerateRecord_SocialSecurity (const string &personname, int age )
{

	// Generate a Social Security Record
	// Add it into the International Social Security Database

	string dob = to_string(NumberGenerator::RandomNumber ( 28 ) + 1) + " - " + to_string(NumberGenerator::RandomNumber ( 12 ) + 1) + " - " +
            to_string(2010 - age);

	string socialsecurity = to_string(10000000 + NumberGenerator::RandomNumber ( 99999999 ));

	string maritalstatus;

	switch ( NumberGenerator::RandomNumber ( 5 ) ) {
		case 0		:		maritalstatus = "Single";			break;
		case 1		:		maritalstatus = "Married";			break;
		case 2		:		maritalstatus = "Seperated";			break;
		case 3		:		maritalstatus = "Divorced";			break;
		case 4		:		maritalstatus = "Widowed";			break;
	}

	string personalstatus;

	switch ( NumberGenerator::RandomNumber ( 5 ) ) {
		case 0		:		personalstatus = "Employed";			break;
		case 1		:		personalstatus = "Un-Employed";		break;
		case 2		:		personalstatus = "Student";			break;
		case 3		:		personalstatus = "Self-Employed";	break;
		case 4		:		personalstatus = "Deceased";		break;
	}

	auto *soc = new Record ();
	soc->AddField ( RECORDBANK_NAME, personname );
	soc->AddField ( "Social Security", socialsecurity );
	soc->AddField ( "D.O.B", dob );
	soc->AddField ( "Marital Status", maritalstatus );
	soc->AddField ( "Personal Status", personalstatus );

	Computer *socsec = game->GetWorld ()->GetComputer ( "International Social Security Database" );
	UplinkAssert ( socsec )

	socsec->recordbank.AddRecordSorted ( soc, "Social Security" );

}

void RecordGenerator::GenerateRecord_Criminal (const string &personname, int age )
{

	// Generate a possible criminal past
	// Only a small percentage of people have any convictions

	std::ostrstream convictions;

	Person *person = game->GetWorld ()->GetPerson ( personname );
	UplinkAssert (person)

	if ( person->rating.uplinkrating == 0 ) {

		// Ordinary person

		if ( NumberGenerator::RandomNumber (100) < PERCENTAGE_PEOPLEWITHCONVICTIONS ) {

			// Greater age = more possible convictions

			int numconv = 0;
			int numconvictions = ( age / 10 ) + NumberGenerator::RandomNumber ( 4 );

			for ( int i = 0; i < numconvictions; ++i ) {

				switch ( (int) NumberGenerator::RandomNormalNumber ( 6, 6 ) ) {

					case 0  :			convictions << "Murder\n"; numconv++;					break;
					case 1  :			convictions << "Armed Robbery\n"; numconv++;			break;
					case 2  :			convictions << "Robbery\n"; numconv++;					break;
					case 3  :			convictions << "Petty theft\n"; numconv++;				break;
					case 4  :
					case 5  :
					case 6  :
					case 7  :	break;
					case 8  :			convictions << "Disturbing the peace\n"; numconv++;		break;
					case 9  :			convictions << "Reckless driving\n"; numconv++;			break;
					case 10 :			convictions << "Man-slaughter\n"; numconv++;			break;
					case 11 :			convictions << "G-B-H\n"; numconv++;					break;
					case 12 :			convictions << "Rape\n"; numconv++;						break;

					default :
						UplinkAbort ( "RecordGenerator::GenerateRecord_Criminal, unrecognised conviction id" )
						break;

				}

			}

			if ( numconv == 0 )
				convictions << "None";

		}
		else {

			convictions << "None";

		}

	}
	else {

		// This guy is an Uplink Agent

		if ( NumberGenerator::RandomNumber (100) < PERCENTAGE_AGENTSWITHCONVICTIONS ) {

			// Greater rating = more possible convictions

			int numconv = 0;
			int numconvictions = ( person->rating.uplinkrating ) + NumberGenerator::RandomNumber ( 2 );

			for ( int i = 0; i < numconvictions; ++i ) {

				switch ( (int) NumberGenerator::RandomNormalNumber ( 6, 6 ) ) {

					case 0  :			convictions << "High tech fraud\n"; numconv++;							break;
					case 1  :			convictions << "Willful destruction of data\n"; numconv++;				break;
					case 2  :			convictions << "Theft of sensitive data\n"; numconv++;					break;
					case 3  :			convictions << "Unlawful access of classified data\n"; numconv++;		break;
					case 4  :
					case 5  :
					case 6  :
					case 7  :	break;
					case 8  :			convictions << "Falsifying academic results\n"; numconv++;				break;
					case 9  :			convictions << "Illegal modification of credit rating\n"; numconv++;	break;
					case 10 :			convictions << "Illegally transfering funds\n"; numconv++;				break;
					case 11 :			convictions << "Credit card fraud\n"; numconv++;						break;
					case 12 :			convictions << "Destruction of financial data\n"; numconv++;			break;

					default :
						UplinkAbort ( "RecordGenerator::GenerateRecord_Criminal, unrecognised conviction id" )
						break;

				}

			}

			if ( numconv == 0 )
				convictions << "None";

		}
		else {

			convictions << "None";

		}

	}

	convictions << '\x0';

	auto *crim = new Record ();
	crim->AddField ( RECORDBANK_NAME, personname );
	crim->AddField ( "Convictions", convictions.str () );

	Computer *crimdb = game->GetWorld ()->GetComputer ( "Global Criminal Database" );
	UplinkAssert ( crimdb )

	crimdb->recordbank.AddRecordSorted ( crim );

	convictions.rdbuf()->freeze( false );
	//delete [] convictions.str ();

}

void RecordGenerator::GenerateRecord_Medical (const string &personname, int age )
{

	auto *rec = new Record ();
	rec->AddField ( RECORDBANK_NAME, personname );
	rec->AddField ( "Known alergies", "None" );
	rec->AddField ( "History", "None" );

	Computer *meddb = game->GetWorld ()->GetComputer ( "Central Medical Database" );
	UplinkAssert ( meddb )

	meddb->recordbank.AddRecordSorted ( rec );

}

void RecordGenerator::GenerateRecord_Academic (const string &personname, int age )
{

	// Generate an academic history based on age

	int brains = (int) NumberGenerator::RandomNormalNumber ( 50, 50 );					// 0 = thick, 50 = average, 100 = rocket scientist

	std::ostrstream collegequals;
	std::ostrstream universityquals;
	std::ostrstream otherquals;

	if ( age >= 16 ) {

		// Has some college quals

		LList <string> possiblequals;
		possiblequals.PutData ( "Science" );
		possiblequals.PutData ( "Maths" );
		possiblequals.PutData ( "Physics" );
		possiblequals.PutData ( "Biology" );
		possiblequals.PutData ( "Art" );
		possiblequals.PutData ( "History ");
		possiblequals.PutData ( "Psychology" );
		possiblequals.PutData ( "Electronics" );
		possiblequals.PutData ( "Computing" );
		possiblequals.PutData ( "Media studies" );
		possiblequals.PutData ( "Sociology" );

		string grades [] = { "A", "B", "C", "D", "E", "F", "G" };

		int numquals = NumberGenerator::RandomNumber ( brains / 10 );

		if ( numquals > 0 ) {

			for ( int i = 0; i < numquals; ++i ) {

				int thisqualindex = NumberGenerator::RandomNumber ( possiblequals.Size () );
				assert( possiblequals.ValidIndex (thisqualindex) );

				int grade = (int) ( 1 + NumberGenerator::RandomNormalNumber ( 100.0f - brains, 40.0f ) / 15 );
				if ( grade < 0 ) grade = 0;
				if ( grade > 6 ) grade = 6;

				collegequals << possiblequals.GetData (thisqualindex) << "      " << grades [grade] << "\n";

				possiblequals.RemoveData (thisqualindex);

			}

		}
		else {

			collegequals << "None";

		}

	}
	else {

		collegequals << "None";

	}

	collegequals << '\x0';

	if ( age >= 21 && brains > 50 ) {

		// Has a degree

		string degreesubject;

		switch ( NumberGenerator::RandomNumber ( 10 ) ) {

			case 0 :		degreesubject = "Computing";					break;
			case 1 :		degreesubject = "Physics";						break;
			case 2 :		degreesubject = "Electrical Engineering";		break;
			case 3 :		degreesubject = "Mechanical Engineering";		break;
			case 4 :		degreesubject = "Software Engineering";			break;
			case 5 :		degreesubject = "Systems Engineering";			break;
			case 6 :		degreesubject = "Management";					break;
			case 7 :		degreesubject = "Artificial Intelligence";		break;
			case 8 :		degreesubject = "Philosophy";					break;
			case 9 :		degreesubject = "Media studies";				break;

			default:
				UplinkAbort ( "RecordGenerator::GenerateRecord_Academic, error in Degree Type" )

		}

		int degreelevel = 1 + ( 100 - brains ) / 25;

		universityquals << degreesubject << ", Class " << degreelevel;

	}
	else {

		// Does not have a degree
		universityquals << "None";

	}

	universityquals << '\x0';

	if ( age > 18 && brains > 50 ) {

		// Generate qualification

		string qualification;

		switch ( NumberGenerator::RandomNumber ( 5 ) ) {

			case 0 :		qualification = "Diploma in Computing";			break;
			case 1 :		qualification = "IEEE Accreditation";		    break;
			case 2 :		qualification = "Certified systems engineer";	break;
			case 3 :		qualification = "Btec in IT";					break;
			case 4 :		qualification = "Systems Engineering diploma";	break;

			default:
				UplinkAbort ( "RecordGenerator::GenerateRecord_Academic, error in Qualification Type" )

		}

		otherquals << qualification;

	}
	else {

		otherquals << "None";

	}

	otherquals << '\x0';


	auto *rec = new Record ();
	rec->AddField ( RECORDBANK_NAME, personname );
	rec->AddField ( "IQ", brains + 50 );
	rec->AddField ( "College", collegequals.str () );
	rec->AddField ( "University", universityquals.str () );
	rec->AddField ( "Other", otherquals.str () );

	Computer *ac = game->GetWorld ()->GetComputer ( "International Academic Database" );
	UplinkAssert ( ac )

	ac->recordbank.AddRecordSorted ( rec );

	collegequals.rdbuf()->freeze( false );
	universityquals.rdbuf()->freeze( false );
	otherquals.rdbuf()->freeze( false );
	//delete [] collegequals.str ();
	//delete [] universityquals.str ();
	//delete [] otherquals.str ();

}

void RecordGenerator::GenerateRecord_Financial (const string &personname, int age )
{

	// Find a financial computer - owned by one of the banks of course
	// And create an account for this person

	Computer *comp = WorldGenerator::GetRandomComputer ( COMPUTER_TYPE_PUBLICBANKSERVER );
	UplinkAssert (comp)
	assert(comp->GetOBJECTID () == OID_BANKCOMPUTER );
	auto *bank = (BankComputer *) comp;

	// Base level value - used to determine this man's situation
	//

	int financial_situation = (int) NumberGenerator::RandomNormalNumber ( 10, 10 );			// 0 = fucked, 10 = average, 20 = Gates

	int balance = (int) ( (financial_situation - 6) * NumberGenerator::RandomNormalNumber ( 1000, 500 ) );
	int loan = financial_situation < 10 ? (int) ( NumberGenerator::RandomNormalNumber ( 10.0f - financial_situation, 5.0f ) * 1000 ) : 0;
	loan = ( loan / 100 ) * 100;
	if ( loan < 0 )
		loan =  0;

	// Add this account in

	Person *person = game->GetWorld ()->GetPerson ( personname );
	UplinkAssert (person)
	int accno = person->CreateNewAccount ( bank->ip, personname, NameGenerator::GeneratePassword (), balance, loan );
	person->rating.SetCreditRating ( financial_situation );

	string accno_s = to_string(accno);

	BankAccount *myaccount = bank->accounts.GetData (accno_s);
	UplinkAssert (myaccount)


	// Generate some records
	// By performing a transfer every few days
	// (This won't do anything if we have passed the game start date)

	if ( myaccount->balance > 0 ) {

		Date transferdate;
		Date nomoredate;
		transferdate.SetDate ( &(game->GetWorld ()->date) );
		nomoredate.SetDate ( GAME_START_DATE );

		while ( transferdate.Before ( &nomoredate ) ) {

			auto *target_bank = (BankComputer *) WorldGenerator::GetRandomComputer ( COMPUTER_TYPE_PUBLICBANKSERVER );
			UplinkAssert ( target_bank )
			BankAccount  *target_acc = target_bank->GetRandomAccount ();

			if ( target_acc ) {

				if ( target_acc->name != game->GetWorld ()->GetPlayer ()->handle ) {		// Prevent transfers to player

					if ( target_acc != myaccount ) {			// Prevent transfers to my own account

						int amount = (int) NumberGenerator::RandomNormalNumber ( myaccount->balance * 0.3f, myaccount->balance * 0.2f );
						amount = amount / 50 * 50;

						if ( amount > 0 ) {

							char source_data [128];
							char target_data [128];
							char s_amount [16];
							UplinkSnprintf ( source_data, sizeof ( source_data ), "%s %d", target_bank->ip, target_acc->accountnumber )
							UplinkSnprintf ( target_data, sizeof ( target_data ), "%s %d", bank->ip, myaccount->accountnumber )
							UplinkSnprintf ( s_amount, sizeof ( s_amount ), "%d", amount )

							auto *source_log = new AccessLog ();
							source_log->SetProperties ( &transferdate, bank->ip, person->name, LOG_NOTSUSPICIOUS, LOG_TYPE_TRANSFERTO );
							source_log->SetData1 ( source_data );
							source_log->SetData2 ( s_amount );
							source_log->SetData3 ( person->name );
							myaccount->log.AddLog ( source_log );

							auto *target_log = new AccessLog ();
							target_log->SetProperties ( &transferdate, bank->ip, person->name, LOG_NOTSUSPICIOUS, LOG_TYPE_TRANSFERFROM );
							target_log->SetData1 ( target_data );
							target_log->SetData2 ( s_amount );
							target_log->SetData3 ( person->name );
							target_acc->log.AddLog ( target_log );

						}

					}

				}

			}

			transferdate.AdvanceDay ( NumberGenerator::RandomNumber ( 14 ) );
			transferdate.AdvanceHour ( NumberGenerator::RandomNumber ( 24 ) );
			transferdate.AdvanceMinute ( NumberGenerator::RandomNumber ( 60 ) );

		}

	}

}

Record *RecordGenerator::GetSocialSecurity ( char *personname )
{

	Computer *comp = game->GetWorld ()->GetComputer ( "International Social Security Database" );
	UplinkAssert ( comp )

	char *name = ( strcmp ( personname, "PLAYER" ) == 0 ?
							game->GetWorld ()->GetPlayer ()->handle :
							personname );

	Record *rec = comp->recordbank.GetRecordFromName ( name );

	return rec;

}

Record *RecordGenerator::GetCriminal (const string &personname )
{

	Computer *comp = game->GetWorld ()->GetComputer ( "Global Criminal Database" );
	UplinkAssert ( comp )

	Record *rec;
	if (personname == "PLAYER") {
        rec = comp->recordbank.GetRecordFromName ( game->GetWorld()->GetPlayer()->handle );
	} else {
        rec = comp->recordbank.GetRecordFromName ( personname );
	}

	return rec;

}

Record *RecordGenerator::GetMedical ( char *personname )
{

	Computer *comp = game->GetWorld ()->GetComputer ( "Central Medical Database" );
	UplinkAssert ( comp )

	char *name = ( strcmp ( personname, "PLAYER" ) == 0 ?
							game->GetWorld ()->GetPlayer ()->handle :
							personname );

	Record *rec = comp->recordbank.GetRecordFromName ( name );

	return rec;

}

Record *RecordGenerator::GetAcademic (char *personname )
{

	Computer *comp = game->GetWorld ()->GetComputer ( "International Academic Database" );
	UplinkAssert ( comp )

	char *name = ( strcmp ( personname, "PLAYER" ) == 0 ?
							game->GetWorld ()->GetPlayer ()->handle :
							personname );

	Record *rec = comp->recordbank.GetRecordFromName ( name );

	return rec;

}

BankAccount *RecordGenerator::GetFinancial (const string &personname )
{

	Person *person = game->GetWorld ()->GetPerson ( personname );
	UplinkAssert ( person )

	string fullcurrentaccount = person->accounts.GetData (0);
	assert(!fullcurrentaccount.empty());

	string ip, accno;
	stringstream stream(fullcurrentaccount);
	stream >> ip >> accno;

	BankAccount *ba = BankAccount::GetAccount ( ip, accno );
	UplinkAssert (ba)

	return ba;

}
