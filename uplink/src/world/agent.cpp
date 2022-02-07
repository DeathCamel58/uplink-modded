// Agent.cpp: implementation of the Agent class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#ifdef WIN32
#include <conio.h>
#include <io.h>
#else
#include <unistd.h>
#include <app/miscutils.h>

#endif


#include "gucci.h"

#include "game/game.h"
#include "game/data/data.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/serialise.h"

#include "interface/interface.h"
#include "interface/remoteinterface/remoteinterface.h"
#include "interface/remoteinterface/linksscreen_interface.h"

#include "world/world.h"
#include "world/message.h"
#include "world/agent.h"
#include "world/vlocation.h"
#include "world/player.h"
#include "world/company/mission.h"
#include "world/computer/computer.h"
#include "world/computer/bankcomputer.h"

#include "world/generator/numbergenerator.h"
#include "world/generator/consequencegenerator.h"
#include "world/generator/missiongenerator.h"
#include "world/generator/worldgenerator.h"

#include "world/scheduler/eventscheduler.h"
#include "world/scheduler/notificationevent.h"

#include "mmgr.h"



//#define		VERBOSEAI_ENABLED									// Print AI messages




Agent::Agent() : Person ()
{
}

Agent::~Agent()
{

	DeleteLListData ( &links );
	DeleteBTreeData ( &codes );
	DeleteLListData ( (LList <UplinkObject *> *) &missions );

}

void Agent::SetHandle (const string &newhandle )
{

	assert( newhandle.length() < SIZE_AGENT_HANDLE );
	UplinkStrncpy ( handle, newhandle.c_str(), sizeof ( handle ) )

}

void Agent::GiveMission ( Mission *mission )
{

	missions.PutData ( mission );

	//
	// Pay the agent the starting amount
	//

	if		( mission->paymentmethod == MISSIONPAYMENT_HALFATSTART ) 	ChangeBalance ( mission->payment / 2, "Anonymous benefactor" );
	else if ( mission->paymentmethod == MISSIONPAYMENT_ALLATSTART ) 	ChangeBalance ( mission->payment, "Anonymous benefactor" );

	//
	// Transfer all the links from the mission to the agent
	//

	for ( int li = 0; li < mission->links.Size (); ++li )
		GiveLink ( mission->links.GetData (li) );

	//
	// Transfer all the access codes from the mission to the agent
	//

	DArray <char *> *darray = mission->codes.ConvertToDArray ();
	DArray <char *> *darray_index = mission->codes.ConvertIndexToDArray ();
	UplinkAssert ( darray )
	UplinkAssert ( darray_index )

	for ( int ai = 0; ai < darray->Size (); ++ai )
		if ( darray->ValidIndex (ai) && darray_index->ValidIndex (ai) )
			GiveCode ( darray_index->GetData (ai), darray->GetData (ai) );

    delete darray;
    delete darray_index;

#ifdef	VERBOSEAI_ENABLED
	cout << "Gave mission to " << name << ": " << mission->description;
#endif

}

bool Agent::ParseAccessCode (const string &thecode, string &username, size_t usernamesize, string &password, size_t passwordsize )
{

	/*
		Access Codes

		Only 2 varieties are permitted:
		1.   ... 'name' .... 'code'
		2.   ... 'code' ....

		In other words, names/codes are always enclosed in '

		*/

    char fullcode [256];
    UplinkStrncpy ( fullcode, thecode.c_str(), sizeof ( fullcode ) )

	//
	// Count the number of dits
	//

	int numdits = 0;
	const char *currentdit = fullcode;
	while ( strchr ( currentdit, '\'' ) != nullptr ) {
		currentdit = strchr ( currentdit, '\'' ) + 1;
		++numdits;
	}

	//
	// Deal with each case
	//

	if ( numdits == 2 ) {

		char *code;

		code = strchr ( fullcode, '\'' ) + 1;
		*( strchr ( code, '\'' ) ) = '\x0';

        username = code;
        password = code;
        return true;

	}
	else if ( numdits == 4 ) {

		char *name;
		char *code;

		name = strchr ( fullcode, '\'' ) + 1;
		code = strchr ( name, '\'' ) + 1;
		code = strchr ( code, '\'' ) + 1;

		*( strchr ( name, '\'' ) ) = '\x0';
		*( strchr ( code, '\'' ) ) = '\x0';

        username = name;
        password = code;
        return true;

	}
	else {

        username = "";
        password = "";
        return false;

	}

}


void Agent::GiveLink (const string &newip )
{

	if ( !HasLink ( newip ) ) {

		assert( newip.length() < SIZE_VLOCATION_IP );
		size_t theipsize = SIZE_VLOCATION_IP;
		char *theip = new char [theipsize];
		UplinkStrncpy ( theip, newip.c_str(), theipsize )
		links.PutDataAtStart ( theip );

        // If this was the player and he is looking at his links screen
        // Update it now

        if ( strcmp ( name, "PLAYER" ) == 0 &&
             strcmp ( game->GetWorld ()->GetPlayer ()->remotehost, IP_LOCALHOST ) == 0 &&
             game->GetInterface ()->GetRemoteInterface ()->currentscreenindex == 0 ) {

            ((LinksScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ())->SetFullList ( &links );
            ((LinksScreenInterface *) game->GetInterface ()->GetRemoteInterface ()->GetInterfaceScreen ())->ApplyFilter ("");

        }

	}

    // Make sure it is set to appear on the world map

    VLocation *vl = game->GetWorld ()->GetVLocation (newip);
    UplinkAssert (vl)
    vl->SetDisplayed (true);

}

int Agent::HasAccount  (const string &ip )
{

	int securityLevel = -1;
	bool firstime = true;

	BTree <char *> *treeCode = nullptr;

	do {

		if ( firstime ) {
			firstime = false;
			treeCode = codes.LookupTree ( ip );
		} else if ( treeCode ){
			if ( treeCode->Left () )
				treeCode = treeCode->Left ()->LookupTree ( ip );
			else
				treeCode = nullptr;
		}

		if ( treeCode ) {

			char *code = treeCode->data;

			if ( !code )
				continue;

			// Parse the access code for this IP

			string username;
			string password;
			if ( !ParseAccessCode ( code, username, sizeof ( username ), password, sizeof ( password ) ) )
				continue;

			// Lookup the computer

			VLocation *vl = game->GetWorld ()->GetVLocation ( ip );
			if ( !vl )
				continue;

			Computer *comp = vl->GetComputer ();
			if ( !comp )
				continue;

			// Lookup the account we have compromised

			Record *rec = comp->recordbank.GetRecordFromNamePassword ( username, password );

			if ( rec ) {

				// Check the security level

				int security = -1;
				char *securitytext = rec->GetField ( RECORDBANK_SECURITY );
				if ( securitytext ) {

                    // TODO: Check if this could be a stream
					sscanf ( securitytext, "%d", &security );

					if ( security != -1 && ( securityLevel == -1 || security < securityLevel ) )
						securityLevel = security;

				}

			}

		}

	} while ( treeCode );

    return securityLevel;

}

bool Agent::HasLink (const string &newip )
{

	for ( int i = 0; i < links.Size (); ++i )
		if ( links.ValidIndex (i) )
			if ( newip == links.GetData (i) )
				return true;

	return false;

}

void Agent::RemoveLink (const string &newip )
{

	for ( int i = 0; i < links.Size (); ++i ) {
		if ( links.ValidIndex (i) ) {
			if ( newip == links.GetData (i) ) {

				links.RemoveData (i);
				break;

			}
		}
	}

}

void Agent::GiveMessage ( Message *message )
{

	Person::GiveMessage ( message );

	// Transfer all links from the message to the agent

	for ( int li = 0; li < message->links.Size (); ++li )
		GiveLink ( message->links.GetData (li) );

	// Transfer all access codes from the mission to the agent

	DArray <char *> *darray = message->codes.ConvertToDArray ();
	DArray <char *> *darray_index = message->codes.ConvertIndexToDArray ();
	UplinkAssert ( darray )
	UplinkAssert ( darray_index )

	for ( int ai = 0; ai < darray->Size (); ++ai )
		if ( darray->ValidIndex (ai) && darray_index->ValidIndex (ai) )
			GiveCode ( darray_index->GetData (ai), darray->GetData (ai) );

    delete darray;
    delete darray_index;

}

void Agent::GiveCode (const string &newip, const string &newcode )
{

    // Do we already have this code?
    //

    DArray <char *> *ips = codes.ConvertIndexToDArray ();
    DArray <char *> *thecodes = codes.ConvertToDArray ();
    UplinkAssert (ips)
    UplinkAssert (thecodes)

    bool found = false;

    for ( int i = 0; i < ips->Size (); ++i ) {
        if ( ips->ValidIndex (i) && thecodes->ValidIndex (i) ) {

            char *thisip = ips->GetData (i);
            UplinkAssert (thisip)

            if ( thisip == newip ) {

                char *thiscode = thecodes->GetData (i);
                UplinkAssert (thiscode)

                if ( thiscode == newcode ) {

                    // We already have the IP and the code
                    found = true;
                    break;

                }
                else {

                    // We have the IP but not exactly the same code

                    string newusername;
                    string newpassword;
                    string thisusername;
                    string thispassword;

                    bool successA = ParseAccessCode ( newcode, newusername, sizeof ( newusername ), newpassword, sizeof ( newpassword ) );
                    bool successB = ParseAccessCode ( thiscode, thisusername, sizeof ( thisusername ), thispassword, sizeof ( thispassword ) );

                    if ( successA && successB ) {

                        if ( newusername == thisusername ) {

                            // Usernames match up, but we have an old code
                            // Replace old code with new code

                            //strcpy ( thiscode, newcode );

							codes.RemoveData ( thisip, thiscode );
							delete [] thiscode;

							size_t newcodecopysize = 128;
							char *newcodecopy = new char [newcodecopysize];
							UplinkStrncpy ( newcodecopy, newcode.c_str(), newcodecopysize )
    						codes.PutData ( thisip, newcodecopy );

                            found = true;
                            break;

                        }

                    }

                }

            }
        }
    }

    delete ips;
    delete thecodes;

    if ( !found ) {

		size_t newcodecopysize = 128;
	    char *newcodecopy = new char [newcodecopysize];
	    UplinkStrncpy ( newcodecopy, newcode.c_str(), newcodecopysize )
    	codes.PutData ( newip, newcodecopy );

    }

}

int  Agent::CreateNewAccount ( char *bankip, char *accname, char *password, int balance, int loan )
{

	int accno = Person::CreateNewAccount ( bankip, accname, password, balance, loan );

	// Give the access code to this agent

	char code [128];
    Computer::GenerateAccessCode( accno, password, code, sizeof ( code ) );
	GiveCode ( bankip, code );

	return accno;

}

bool Agent::HasMissionLink (const string &newip )
{

	for ( int ii = 0; ii < missions.Size (); ii++ )
		if ( missions.ValidIndex ( ii ) ) {
			LList<char*> *links = &(missions.GetData ( ii )->links);
			for ( int i = 0; i < links->Size () ; i++ )
				if ( links->ValidIndex ( i ) )
					if ( newip == links->GetData ( i ) )
						return true;
		}

	return false;

}

void Agent::CheckMissionDueDates ()
{

	for ( int i = 0; i < missions.Size (); ++i ) {

		Mission *m = missions.GetData (i);
		UplinkAssert (m)

		if ( m->GetDueDate () ) {

			// This mission has a due date - check it

			if ( game->GetWorld ()->date.After ( m->GetDueDate () ) ) {

				// This mission has expired

				if ( m->TYPE == MISSION_PAYFINE ) {

					// Legal action begins
					ConsequenceGenerator::DidntPayFine ( this, m );

				}
				else {

					MissionGenerator::MissionFailed ( m, this, "Time has run out." );

				}

				// Remove the mission

				delete m;
				missions.RemoveData ( i );
				i--;

			}

		}

	}

}

void Agent::AttemptMission ()
{

	if ( !(missions.GetData (0)) ) return;
	Mission *m = missions.GetData (0);

	//
	// Perform the mission
	//

	switch ( m->TYPE ) {

        case MISSION_STEALFILE:                 AttemptMission_StealFile ();                    break;
        case MISSION_DESTROYFILE:				AttemptMission_DeleteFile ();					break;
		case MISSION_CHANGEACCOUNT:				AttemptMission_ChangeAccount ();				break;
		case MISSION_TRACEUSER:					AttemptMission_TraceUser ();					break;
		case MISSION_REMOVECOMPUTER:			AttemptMission_RemoveComputer ();				break;

		default:
			break;

	}

#ifdef	VERBOSEAI_ENABLED
	cout << name << " Attempted mission: " << m->description << endl;
#endif

	delete m;
    missions.RemoveData (0);

}

void Agent::AttemptMission_StealFile ()
{

	if ( !(missions.GetData (0)) ) return;
	Mission *m = missions.GetData (0);

	char *targetip = m->completionA;
	char *filename = m->completionB;

	if ( strcmp ( filename, "ALL" ) == 0 ) {

		EstablishConnection ( targetip );

		VLocation *vl = game->GetWorld ()->GetVLocation ( targetip );
		UplinkAssert (vl)
		Computer *comp = vl->GetComputer ();
		UplinkAssert (comp)

		ConsequenceGenerator::MissionCompleted ( m, this );

	}
    else {

        // Not worth bothering - nobody will notice a
        // single file stolen

    }

}


void Agent::AttemptMission_DeleteFile ()
{

	if ( !(missions.GetData (0)) ) return;
	Mission *m = missions.GetData (0);

	char *targetip = m->completionA;
	char *filename = m->completionB;

	if ( strcmp ( filename, "ALL" ) == 0 ) {

		EstablishConnection ( targetip );

		VLocation *vl = game->GetWorld ()->GetVLocation ( targetip );
		UplinkAssert (vl)
		Computer *comp = vl->GetComputer ();
		UplinkAssert (comp)

		comp->databank.Format ();

		ConsequenceGenerator::MissionCompleted ( m, this );

	}
    else {

        // Not worth bothering - nobody will notice a
        // single file deletion

    }

}

void Agent::AttemptMission_ChangeAccount ()
{

	if ( !(missions.GetData (0)) ) return;
	Mission *m = missions.GetData (0);

	char sourceip [SIZE_VLOCATION_IP];
	char targetip [SIZE_VLOCATION_IP];
	char sourceaccno [16];
	char targetaccno [16];

	int amount;

	sscanf ( m->completionA, "%s %s", sourceip, sourceaccno );
	sscanf ( m->completionB, "%s %s", targetip, targetaccno );
	sscanf ( m->completionC, "%d", &amount );

	EstablishConnection ( sourceip );

	BankAccount *ba = BankAccount::GetAccount ( sourceip, sourceaccno );
	UplinkAssert (ba)
	ba->TransferMoney ( sourceip, sourceaccno, targetip, targetaccno, amount, this );

	ConsequenceGenerator::MissionCompleted ( m, this );

}

void Agent::AttemptMission_TraceUser ()
{

	//
	// [ DEEP BREATH ]
	//

	if ( !(missions.GetData (0)) ) return;
	Mission *m = missions.GetData (0);

	char *hackername = m->completionA;
	Person *hacker = game->GetWorld ()->GetPerson ( hackername );
	UplinkAssert (hacker)

	//
	// Get the hacked computer system
	//

	char *compip = m->links.GetData (0);
	VLocation *vl = game->GetWorld ()->GetVLocation ( compip );
	UplinkAssert (vl)
	Computer *comp = vl->GetComputer ();
	UplinkAssert (comp)

	//
	// Go through each log, try to trace back to the hacker
	//

	for ( int il = 0; il < comp->logbank.logs.Size (); ++il ) {
		if ( comp->logbank.logs.ValidIndex (il) ) {

			AccessLog *al = comp->logbank.logs.GetData (il);
			UplinkAssert (al)

			// If the log was deleted/overwritten we may be able to recover it

			if ( al->TYPE == LOG_TYPE_DELETED &&
				 rating.uplinkrating >= MINREQUIREDRATING_UNDELETELOGLEVEL1 ) {

				if ( comp->logbank.internallogs.ValidIndex (il) ) {

					AccessLog *recovered = comp->logbank.internallogs.GetData (il);
					if ( recovered ) {
						auto *internalcopy = new AccessLog ();
						internalcopy->SetProperties ( recovered );
						comp->logbank.logs.PutData ( internalcopy, il );
						delete al;
						al = internalcopy;
					}

				}

			}
			else if ( al->TYPE != LOG_TYPE_DELETED &&
					  comp->logbank.internallogs.ValidIndex (il) &&
					  comp->logbank.LogModified (il) &&
					  rating.uplinkrating >= MINREQUIREDRATING_UNDELETELOGLEVEL3 ) {

				// Log was overwritten or modified

				AccessLog *recovered = comp->logbank.internallogs.GetData (il);
				if ( recovered ) {
					auto *internalcopy = new AccessLog ();
					internalcopy->SetProperties ( recovered );
					comp->logbank.logs.PutData ( internalcopy, il );
					delete al;
					al = internalcopy;
				}

			}

			// Now look at the log

			if ( al->SUSPICIOUS != LOG_NOTSUSPICIOUS &&
				 strcmp ( al->fromname, hackername ) == 0 &&
				 al->TYPE != LOG_TYPE_CONNECTIONOPENED ) {

				// This is by the hacker but can't be investigated further

				al->SetSuspicious ( LOG_NOTSUSPICIOUS );
				if ( comp->logbank.internallogs.ValidIndex (il) )
					comp->logbank.internallogs.GetData (il)->SetSuspicious ( LOG_NOTSUSPICIOUS );

			}
			else if ( al->SUSPICIOUS != LOG_NOTSUSPICIOUS &&
				 al->TYPE == LOG_TYPE_CONNECTIONOPENED &&
				 strcmp ( al->fromname, hackername ) == 0 ) {

				// Now investigated - so set it back to LOG_NOTSUSPICIOUS

				al->SetSuspicious ( LOG_NOTSUSPICIOUS );
				if ( comp->logbank.internallogs.ValidIndex (il) )
					comp->logbank.internallogs.GetData (il)->SetSuspicious ( LOG_NOTSUSPICIOUS );

				VLocation *vlf = game->GetWorld ()->GetVLocation ( al->fromip );
				Computer *fromcomp = nullptr;
				if ( vlf )
					fromcomp = vlf->GetComputer ();

				if ( fromcomp ) {

					string traced_ip = fromcomp->logbank.TraceLog ( comp->ip, fromcomp->ip, &(al->date), rating.uplinkrating );

					if ( !traced_ip.empty() ) {

						// Trace successful
						// Does the guilty person live at this IP?

						if ( hacker->localhost == traced_ip ) {

							// Mission successful

	#ifdef VERBOSEAI_ENABLED
							cout << "Agent " << name << " successfully traced hacker " << hackername << " (" << rating.uplinkrating << " V " << hacker->rating.uplinkrating")" << endl;
	#endif

							ConsequenceGenerator::MissionCompleted ( m, this );

							return;

						}
						else {

							// The trace has been completed but a different IP has been found
							// This means the logs have been altered, probably to incriminate
							// someone else

							// Look up the unfortunate individual

							DArray <Person *> *people = game->GetWorld ()->people.ConvertToDArray ();
							Person *framed = nullptr;

							for ( int ip = 0; ip < people->Size (); ++ip ) {
								if ( people->ValidIndex (ip) ) {
									if ( people->GetData (ip) ) {
										if ( people->GetData (ip)->localhost == traced_ip ) {
											framed = people->GetData (ip);
											break;
										}
									}
								}
							}

							delete people;

							if ( framed ) {

	#ifdef VERBOSEAI_ENABLED
								cout << "Agent " << name << " traced " << framed->name << " (probably framed)" << endl;
	#endif

								UplinkStrncpy ( m->completionA, framed->name, sizeof ( m->completionA ) )

								ConsequenceGenerator::MissionCompleted ( m, this );
								return;

							}
							else {

	#ifdef VERBOSEAI_ENABLED
								cout << "Agent " << name << " nearly traced a framed person, but not quite ;)" << endl;
								return;
	#endif

							}

						}

					}

				}

			}

		}

	}

	//
	// If we reach here then the hacker could not be traced
	//

#ifdef VERBOSEAI_ENABLED
	cout << "Agent " << name << " failed to trace hacker " << hackername << " (" << rating.uplinkrating << " V " << hacker->rating.uplinkrating << ")" << endl;
#endif

}

void Agent::AttemptMission_RemoveComputer ()
{

	if ( !(missions.GetData (0)) ) return;
	Mission *m = missions.GetData (0);

	char *targetip = m->completionA;

	VLocation *vl = game->GetWorld ()->GetVLocation ( targetip );
	UplinkAssert (targetip)
	Computer *comp = vl->GetComputer ();
	UplinkAssert (comp)

	EstablishConnection ( targetip );

	comp->SetIsRunning ( false );

	ConsequenceGenerator::MissionCompleted ( m, this );

}

void Agent::EstablishConnection (const string &ip )
{

	connection.Reset ();

	//
	// Add in a few bounces
	//

	int numbounces = 2 + NumberGenerator::RandomNumber ( rating.uplinkrating / 3 );

	for ( int i = 0; i < numbounces; ++i ) {

		Computer *comp = WorldGenerator::GetRandomComputer (	COMPUTER_TYPE_INTERNALSERVICESMACHINE	|
																COMPUTER_TYPE_CENTRALMAINFRAME			|
																COMPUTER_TYPE_PUBLICBANKSERVER			);
		UplinkAssert (comp)

		if ( !connection.LocationIncluded ( comp->ip ) &&
			 comp->ip != ip )
			connection.AddVLocation ( comp->ip );

		else
			--i;

	}

	//
	// Add in the target and establish the connection
	//

	connection.AddVLocation ( ip );
	connection.Connect ();
	connection.BeginTrace ();								// To make it "suspect"

	//
	// Cover our tracks on 1 system (not the target)
	// The date on the logs should remain the same - so they don't get out of sync

	int coverindex = NumberGenerator::RandomNumber ( numbounces - 1 ) + 1;
	char *fromip = connection.vlocations.GetData (coverindex-1);
	char *toip   = connection.vlocations.GetData (coverindex);

	VLocation *vl = game->GetWorld ()->GetVLocation ( toip );
	UplinkAssert (vl)
	Computer *comp = vl->GetComputer ();
	UplinkAssert (comp)

	for ( int il = 0; il < comp->logbank.logs.Size (); ++il ) {
		if ( comp->logbank.logs.ValidIndex (il) ) {

			if ( strcmp ( comp->logbank.logs.GetData (il)->fromname, name ) == 0 &&
				 strcmp ( comp->logbank.logs.GetData (il)->fromip, fromip ) == 0 ) {

				Date logdate;
				logdate.SetDate ( &(comp->logbank.logs.GetData (il)->date) );

				if ( rating.uplinkrating >= MINREQUIREDRATING_DELETELOGLEVEL3 ) {

					// Copy an existing log over the top
					delete comp->logbank.logs.GetData (il);
					comp->logbank.logs.RemoveData (il);
					bool overwritten = false;

					// Look for a valid log to put in the blank place
					for ( int is = 0; is < comp->logbank.logs.Size (); ++is ) {
						if ( comp->logbank.logs.ValidIndex (is) ) {
							if ( strcmp ( comp->logbank.logs.GetData (is)->fromip, fromip ) != 0 ) {

								AccessLog *copyme = comp->logbank.logs.GetData (is);
								// This log was made by someone else
								auto *al = new AccessLog ();
								al->SetProperties ( copyme );
								al->date.SetDate ( &logdate );
								al->SetSuspicious ( LOG_NOTSUSPICIOUS );
								comp->logbank.logs.PutData ( al, il );
								overwritten = true;
								break;

							}
						}
					}

					if ( !overwritten ) {

						// Could not find log to overwrite with -
						// So make up a new one

						auto *al = new AccessLog ();
						al->SetProperties ( &logdate, WorldGenerator::GetRandomLocation ()->ip, " ",
											LOG_NOTSUSPICIOUS, LOG_TYPE_TEXT );
						al->SetData1 ( "Accessed File" );
						comp->logbank.logs.PutData ( al, il );

					}

				}
				else if ( rating.uplinkrating >= MINREQUIREDRATING_DELETELOGLEVEL2 ) {

					delete comp->logbank.logs.GetData (il);

					// Blank the log
					auto *al = new AccessLog ();
					al->SetProperties ( &logdate, "Unknown", name,
										LOG_NOTSUSPICIOUS, LOG_TYPE_DELETED);
					comp->logbank.logs.PutData ( al, il );

				}
				else if ( rating.uplinkrating >= MINREQUIREDRATING_DELETELOGLEVEL1 ) {

					delete comp->logbank.logs.GetData (il);

					// Delete the log, leave marker
					auto *al = new AccessLog ();
					al->SetProperties ( &logdate, "Unknown", name,
										LOG_NOTSUSPICIOUS, LOG_TYPE_DELETED);
					comp->logbank.logs.PutData ( al, il );


				}

			}

		}

	}

	//
	// Disconnect me
	//

	connection.Disconnect ();
	connection.Reset ();

}

bool Agent::Load ( FILE *file )
{

	LoadID ( file );

	if ( !Person::Load ( file ) ) return false;

	if ( !LoadDynamicStringStatic ( handle, SIZE_AGENT_HANDLE, file ) ) return false;

	if ( !LoadLList ( &links, file ) ) return false;
	if ( !LoadBTree ( &codes, file ) ) return false;
	if ( !LoadLList ( (LList <UplinkObject *> *) &missions, file ) ) return false;

	LoadID_END ( file );

	// Fix a problem with invalid access code

    DArray <char *> *ips = codes.ConvertIndexToDArray ();
    DArray <char *> *thecodes = codes.ConvertToDArray ();

	if ( ips && thecodes ) {
		for ( int i = 0; i < ips->Size (); ++i ) {
			if ( ips->ValidIndex ( i ) && thecodes->ValidIndex ( i ) && ips->GetData ( i ) ) {
				char *thisip = ips->GetData ( i );
				char *thiscode = thecodes->GetData ( i );

				bool success = false;
				if ( thiscode && strlen ( thiscode ) < 256 ) {
					string thisusername;
					string thispassword;
					success = ParseAccessCode ( thiscode, thisusername, sizeof ( thisusername ), thispassword, sizeof ( thispassword ) );

					if ( success )
						for ( int ii = (int) strlen ( thiscode ) - 1; ii >= 0; ii-- )
							if ( thiscode [ ii ] < 32 ) {
								success = false;
								break;
							}
				}

				if ( !success ) {
					codes.RemoveData ( thisip, thiscode );
                    delete [] thiscode;
				}
			}
		}
	}

    delete ips;
    delete thecodes;

	return true;

}

void Agent::Save ( FILE *file )
{

	SaveID ( file );

	Person::Save ( file );

	SaveDynamicString ( handle, SIZE_AGENT_HANDLE, file );

	SaveLList ( &links, file );
	SaveBTree ( &codes, file );
	SaveLList ( (LList <UplinkObject *> *) &missions, file );

	SaveID_END ( file );

}

void Agent::Print ()
{

    cout << "Uplink Agent : " << endl;
    PrintValue("Handle", handle);
	Person::Print ();

	PrintLList ( &links );
	PrintBTree ( &codes );
	PrintLList ( (LList <UplinkObject *> *) &missions );

}

string Agent::GetID ()
{

	return "AGENT";

}

int Agent::GetOBJECTID ()
{

	return OID_AGENT;

}

void Agent::Update ()
{

	Person::Update ();

}

