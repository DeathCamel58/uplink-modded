
#include <sstream>

#include "app/globals.h"

#include "game/game.h"
#include "game/data/data.h"

#include "interface/interface.h"
#include "interface/remoteinterface/remoteinterface.h"

#include "world/world.h"
#include "world/message.h"
#include "world/person.h"
#include "world/player.h"
#include "world/computer/computer.h"
#include "world/computer/bankcomputer.h"
#include "world/company/mission.h"
#include "world/company/companyuplink.h"

#include "world/generator/missiongenerator.h"
#include "world/generator/consequencegenerator.h"
#include "world/generator/newsgenerator.h"
#include "world/generator/recordgenerator.h"
#include "world/generator/numbergenerator.h"

#include "world/scheduler/eventscheduler.h"
#include "world/scheduler/arrestevent.h"
#include "world/scheduler/shotbyfedsevent.h"
#include "world/scheduler/seizegatewayevent.h"

#include "mmgr.h"


void ConsequenceGenerator::Initialise ()
{
}

void ConsequenceGenerator::CaughtHacking ( Person *person, Computer *comp )
{

	UplinkAssert (person)
	UplinkAssert (comp)


	//
	// Get the computer to "remember" that is was hacked
	//

	comp->AddToRecentHacks ( 1 );

	//
	// Lookup the company owner
	//

	Company *company = game->GetWorld ()->GetCompany ( comp->companyname );
	UplinkAssert ( company )

	int severity = company->size / 10 + 1;

	/*
    //
    // SPECIAL CASE
    // If this is the player, and he has the VIRTUANET gateway (total anonymity)
    //

    if ( strcmp ( person->name, "PLAYER" ) == 0 &&
         game->GetWorld ()->GetPlayer ()->gateway.type == 3 ) {

		// Disconnect user if he is still connected here

		if ( person->GetConnection ()->GetTarget () &&
			 strcmp ( person->GetConnection ()->GetTarget (), comp->ip ) == 0 ) {

			person->GetConnection ()->Disconnect ();
			person->GetConnection ()->Reset ();
			game->GetInterface ()->GetRemoteInterface ()->RunNewLocation ();
			game->GetInterface ()->GetRemoteInterface ()->RunScreen ( 2);

        }

        // Don't do anything else
        return;

    }
	*/

	//
	// Add this to their criminal record
	// And change their ratings
	//

	if ( comp->traceaction > 3 ) {

		// ie not "NONE", "DISCONNECT" or "WARNINGMAIL"

		Record *rec = RecordGenerator::GetCriminal ( person->name );

		if ( rec ) {
			char *existing = rec->GetField ( "Convictions" );
			//UplinkAssert (existing);

			string newconvictions;

			if ( existing && strstr ( existing, "None" ) == nullptr ) {
                newconvictions += existing;
                newconvictions += "\n";
            }

			newconvictions += "Unauthorised systems access\n";

			rec->ChangeField ( "Convictions", newconvictions );
		}

		//
		// Decrease ratings
		//

		person->rating.ChangeUplinkScore ( severity * -1 );
		person->rating.ChangeNeuromancerScore ( severity * -1 );

	}


	//
	// comp->traceaction is a bitfield - so act on each possibility
	//

	if ( comp->traceaction & COMPUTER_TRACEACTION_DISCONNECT ) {

		// Disconnect user if he is still connected here

		if ( !person->GetConnection ()->GetTarget ().empty() &&
			 person->GetConnection ()->GetTarget () == comp->ip ) {

			person->GetConnection ()->Disconnect ();
			person->GetConnection ()->Reset ();

			if ( strcmp ( person->name, "PLAYER" ) == 0 ) {

				game->GetInterface ()->GetRemoteInterface ()->RunNewLocation ();
				game->GetInterface ()->GetRemoteInterface ()->RunScreen ( 2);

			}

		}

	}

	if ( comp->traceaction & COMPUTER_TRACEACTION_WARNINGMAIL ) {

		// Send warning mail

		auto *msg = new Message ();
		msg->SetTo ( person->name );
		msg->SetFrom ( comp->companyname );
		msg->SetSubject ( "You were caught hacking our computer systems" );
		msg->SetBody ( "Our system administrator has informed us that you have been using "
					   "one of our accounts to gain unauthorised access to our computer systems.\n"
					   "We have disconnected your computer from our system and have changed "
					   "the access codes to this account.  Do not attempt to gain access to "
					   "our systems again.\n"
					   "We will be notifying your employer and the police of this action.  "
					   "This was a warning - in future, more severe action will be taken." );
		msg->Send ();

       // Reset defenses

        comp->ChangeSecurityCodes ();

	}

	if ( comp->traceaction & COMPUTER_TRACEACTION_FINE ) {

		// Pay a fine

		int finesize = severity * 1000;

		// No point in fining anyone other than the player
		// As no one would ever notice

		if ( strcmp ( person->name, "PLAYER" ) == 0 ) {

			// Generate a "pay fine" mission

			string reason= "Our system security agents have caught you making unauthorised access to our computer system ";
            reason += comp->name;
            reason += ".";

			// You have a few days to complete it

			Date *date = new Date ();
			date->SetDate ( &(game->GetWorld ()->date) );
			date->AdvanceMinute ( TIME_TOPAYLEGALFINE );
			date->SetDate ( 0, 0, 0, date->GetDay (), date->GetMonth (), date->GetYear () );			// Round to midnight

			Mission *mission = MissionGenerator::Generate_PayFine ( person, company, finesize, date, reason );
			game->GetWorld ()->GetPlayer ()->GiveMission ( mission );

			delete date;

		}


       // Reset defenses

        comp->ChangeSecurityCodes ();

	}

	if ( comp->traceaction & COMPUTER_TRACEACTION_LEGAL ) {

		// Legal action begins - the person is arrested
		// (In a few hours)
		// If its an agent, seize his gateway

		Date duedate;
		duedate.SetDate ( &(game->GetWorld ()->date) );
		duedate.AdvanceMinute ( TIME_LEGALACTION );

		Date warningdate;
		warningdate.SetDate ( &duedate );
		warningdate.AdvanceMinute ( TIME_LEGALACTION_WARNING * -1 );


		if ( strcmp ( person->name, "PLAYER" ) != 0 ) {

			string reason = "hacking into the ";
			reason += comp->name;
			reason += ".\n";

			auto *event = new ArrestEvent ();
			event->SetName ( person->name );
			event->SetReason ( reason );
			event->SetIP ( comp->ip );
			event->SetRunDate ( &duedate );

			game->GetWorld ()->scheduler.ScheduleWarning ( event, &warningdate );
			game->GetWorld ()->scheduler.ScheduleEvent ( event );

		}
		else {

			// This is the player - seize his gateway

			string reason = "Hacking into the ";
			reason += comp->name;
			reason += ".\n";

			auto *event = new SeizeGatewayEvent ();
			event->SetName ( person->name );
			event->SetGatewayID ( game->GetWorld ()->GetPlayer ()->gateway.id );
			event->SetReason ( reason );
			event->SetRunDate ( &duedate );

			game->GetWorld ()->scheduler.ScheduleWarning ( event, &warningdate );
			game->GetWorld ()->scheduler.ScheduleEvent ( event );

		}

	}

	if ( comp->traceaction & COMPUTER_TRACEACTION_TACTICAL ) {

		// Tactical action begins - the person is shot
		// (In a few minutes)
		// If its an agent, seize his gateway

		Date duedate;
		duedate.SetDate ( &(game->GetWorld ()->date) );
		duedate.AdvanceMinute ( TIME_TACTICALACTION );

		Date warningdate;
		warningdate.SetDate ( &duedate );
		warningdate.AdvanceMinute ( TIME_TACTICALACTION_WARNING * -1 );

		if ( strcmp ( person->name, "PLAYER" ) != 0 ) {

			string reason = "hacking into the ";
			reason += comp->name;
			reason += ".\n";

			auto *event = new ShotByFedsEvent ();
			event->SetName ( person->name );
			event->SetReason ( reason );
			event->SetRunDate ( &duedate );

			game->GetWorld ()->scheduler.ScheduleWarning ( event, &warningdate );
			game->GetWorld ()->scheduler.ScheduleEvent ( event );

		}
		else {

			// This is the player - seize his gateway

			string reason = "Hacking into the ";
			reason += comp->name;
			reason += ".\n";

			auto *event = new SeizeGatewayEvent ();
			event->SetName ( person->name );
			event->SetGatewayID ( game->GetWorld ()->GetPlayer ()->gateway.id );
			event->SetReason ( reason );
			event->SetRunDate ( &duedate );

			game->GetWorld ()->scheduler.ScheduleWarning ( event, &warningdate );
			game->GetWorld ()->scheduler.ScheduleEvent ( event );

		}

	}

}

void ConsequenceGenerator::ShotByFeds (Person *person, const string& reason )
{

	UplinkAssert (person)

}

void ConsequenceGenerator::Arrested (Person *person, Computer *comp, const string& reason )
{

	UplinkAssert (person)

	NewsGenerator::Arrested ( person, comp, reason );

}

void ConsequenceGenerator::DidntPayFine ( Person *person, Mission *fine )
{

	UplinkAssert (person)
	UplinkAssert (fine)

	//
	// Schedule an ARREST event for this person
	// Or a SeizeGateway event if its the player

	Date duedate;
	duedate.SetDate ( &(game->GetWorld ()->date) );
	duedate.AdvanceMinute ( TIME_LEGALACTION );

	Date warningdate;
	warningdate.SetDate ( &duedate );
	warningdate.AdvanceMinute ( TIME_LEGALACTION_WARNING * -1 );

	int finesize = stoi(fine->completionC);

	if ( strcmp ( person->name, "PLAYER" ) != 0 ) {

		int jailtime = finesize / 1000;

		string reason = "For refusing to pay a " + to_string(finesize) + "c fine.\n"
			            "Send to jail for " + to_string(jailtime) + " years.\n";

		auto *event = new ArrestEvent ();
		event->SetName ( person->name );
		event->SetReason ( reason );
		event->SetRunDate ( &duedate );

		game->GetWorld ()->scheduler.ScheduleWarning ( event, &warningdate );
		game->GetWorld ()->scheduler.ScheduleEvent ( event );

	}
	else {

		// This is the player - seize his gateway

		string reason = "For refusing to pay a " + to_string(finesize) + "c fine.\n";

		auto *event = new SeizeGatewayEvent ();
		event->SetName ( person->name );
		event->SetGatewayID ( game->GetWorld ()->GetPlayer ()->gateway.id );
		event->SetReason ( reason );
		event->SetRunDate ( &duedate );

		game->GetWorld ()->scheduler.ScheduleWarning ( event, &warningdate );
		game->GetWorld ()->scheduler.ScheduleEvent ( event );

	}

}

void ConsequenceGenerator::SeizeGateway  (Person *person, const string& reason )
{

	UplinkAssert (person)

}


void ConsequenceGenerator::ComputerHacked ( Computer *comp, AccessLog *al )
{

	UplinkAssert (comp)
	UplinkAssert (al)

	comp->AddToRecentHacks ( 1 );

	NewsGenerator::ComputerHacked ( comp, al );

	if ( comp->traceaction >= COMPUTER_TRACEACTION_LEGAL ) {

		Person *hacker = game->GetWorld ()->GetPerson ( al->fromname );
		UplinkAssert (hacker)

		if ( hacker == game->GetWorld ()->GetPlayer () ||
			NumberGenerator::RandomNumber (100) < PROB_GENERATETRACEHACKERMISSION ) {

			// If this hasn't already been noticed, generate a new trace hacker mission
			// No point in generating another tracehacker mission if that person and machine
			// are already under investigation

			auto *uplink = (CompanyUplink *) game->GetWorld ()->GetCompany ( "Uplink" );
			UplinkAssert (uplink)

			Date testdate;
			testdate.SetDate ( &(game->GetWorld ()->date) );
			testdate.AdvanceHour ( -12 );

			bool alreadynoticed = false;

			for ( int i = 0; i < uplink->missions.Size (); ++i ) {

				Mission *m = uplink->GetMission ( i );
				UplinkAssert (m)

				if ( m->TYPE == MISSION_TRACEUSER &&
					 m->completionA == hacker->name &&
					 m->links.GetData (0) == comp->ip ) {

					alreadynoticed = true;
					break;

				}

			}

			if ( !alreadynoticed ) MissionGenerator::Generate_TraceHacker ( comp, hacker );

		}

	}

}

void ConsequenceGenerator::MissionCompleted ( Mission *mission, Person *person )
{

	UplinkAssert ( mission )
	UplinkAssert ( person )

	switch ( mission->TYPE ) {

		case MISSION_STEALFILE:				MissionCompleted_StealFile		( mission, person );		break;
		case MISSION_DESTROYFILE:			MissionCompleted_DestroyFile	( mission, person );		break;
		case MISSION_CHANGEACCOUNT:			MissionCompleted_ChangeAccount	( mission, person );		break;
		case MISSION_TRACEUSER:				MissionCompleted_TraceUser		( mission, person );		break;

		case MISSION_NONE:
		default:																						break;


	}

}

void ConsequenceGenerator::MissionCompleted_StealFile	( Mission *mission, Person *person )
{

	UplinkAssert ( mission )
	UplinkAssert ( person )

	string ip = mission->completionA;
	string filename = mission->completionB;
	string filetype = mission->completionE;

    int numfiles;
    int totalfilesize;
    stringstream stream(mission->completionD);
    stream >> numfiles >> totalfilesize;

	if ( filename == "ALL" ) {

		VLocation *vl = game->GetWorld ()->GetVLocation ( ip );
		UplinkAssert (vl)
		Computer *comp = vl->GetComputer ();
		UplinkAssert (comp)

		NewsGenerator::AllFilesStolen ( comp, filetype, totalfilesize );

		if ( person == game->GetWorld ()->GetPlayer () ||
			 NumberGenerator::RandomNumber (100) < PROB_GENERATETRACEHACKERMISSION ) {

			MissionGenerator::Generate_TraceHacker ( mission, person );

		}

	}

}

void ConsequenceGenerator::MissionCompleted_DestroyFile ( Mission *mission, Person *person )
{

	UplinkAssert (mission)
	UplinkAssert (person)

	string filename = mission->completionB;

	if ( filename == "ALL" ) {

		if ( person == game->GetWorld ()->GetPlayer () ||
			 NumberGenerator::RandomNumber (100) < PROB_GENERATETRACEHACKERMISSION ) {

			MissionGenerator::Generate_TraceHacker ( mission, person );

		}

	}

}

void ConsequenceGenerator::MissionCompleted_ChangeAccount ( Mission *mission, Person *person )
{

	UplinkAssert (mission)
	UplinkAssert (person)

	if ( person == game->GetWorld ()->GetPlayer () ||
		 NumberGenerator::RandomNumber (100) < PROB_GENERATETRACEHACKERMISSION ) {

		// Extract values from the mission

		string source_ip, target_ip, source_accs, target_accs;
		int amount = stoi(mission->completionC);

		UplinkAssert (mission)

		stringstream streamA(mission->completionA);
		streamA >> source_ip >> source_accs;
		stringstream streamB(mission->completionB);
		streamB >> target_ip >> target_accs;

		// Get the source and target computers

		VLocation *vls = game->GetWorld ()->GetVLocation ( source_ip );
		UplinkAssert (vls)
		auto *source = (BankComputer *) vls->GetComputer ();
		UplinkAssert (source)

		VLocation *vlt = game->GetWorld ()->GetVLocation ( target_ip );
		UplinkAssert (vlt)
		auto *target = (BankComputer *) vlt->GetComputer ();
		UplinkAssert (target)

		// Get the source and target bank accounts

		BankAccount *source_acc = BankAccount::GetAccount ( source_ip, source_accs );
		BankAccount *target_acc = BankAccount::GetAccount ( target_ip, target_accs );
		UplinkAssert (source_acc)
		UplinkAssert (target_acc)

		// Get the employer

		Company *employer = game->GetWorld ()->GetCompany ( source->companyname );
		UplinkAssert (employer)

		MissionGenerator::Generate_TraceUser_MoneyTransfer ( employer, source, target, source_acc, target_acc, amount, person );

	}

}

void ConsequenceGenerator::MissionCompleted_TraceUser ( Mission *mission, Person *person )
{

	UplinkAssert ( mission )
	UplinkAssert ( person )

	//
	// Look up the computer that was hacked
	//

	string hackedip = mission->links.GetData (0);
	VLocation *vl = game->GetWorld ()->GetVLocation ( hackedip );
	UplinkAssert (vl)
	Computer *comp = vl->GetComputer ();
	UplinkAssert (comp)

	//
	// Look up the guilty person
	//

	string personname = mission->completionA;
	Person *guiltyperson = game->GetWorld ()->GetPerson ( personname );
	UplinkAssert ( guiltyperson )

	//
	// This person is effectivly caught
	//

	if ( guiltyperson->GetStatus () == PERSON_STATUS_NONE ) CaughtHacking ( guiltyperson, comp );

}

