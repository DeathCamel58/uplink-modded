
#ifdef WIN32
#include <windows.h>
#endif

#include <strstream>
#include <app/miscutils.h>

#include "app/globals.h"
#include "app/serialise.h"

#include "game/game.h"
#include "game/data/data.h"

#include "world/world.h"
#include "world/vlocation.h"
#include "world/player.h"
#include "world/computer/computer.h"
#include "world/computer/bankaccount.h"
#include "world/scheduler/bankrobberyevent.h"

#include "mmgr.h"



BankRobberyEvent::BankRobberyEvent ()
{

    source_ip = " ";
    source_accno = " ";
    target_ip = " ";
    target_accno = " ";
    amount = 0;

}

BankRobberyEvent::~BankRobberyEvent ()
= default;

void BankRobberyEvent::Run ()
{

    char amount_s [64];
    UplinkSnprintf ( amount_s, sizeof ( amount_s ), "%d", amount )

    bool rumbled = false;

    //
    // Has the player left a log on the source bank system

    BankAccount *source = BankAccount::GetAccount ( source_ip, source_accno );

    if ( source ) {
        for ( int i = 0; i < source->log.internallogs.Size (); ++i ) {

			if ( source->log.internallogs.ValidIndex ( i ) ) {

				AccessLog *al = source->log.internallogs.GetData (i);

				if ( al && al->TYPE == LOG_TYPE_TRANSFERTO ) {

					if ( al->data1 && al->data2 ) {

						if ( strstr ( al->data1, target_ip.c_str() )    != nullptr &&
							 strstr ( al->data1, target_accno.c_str() ) != nullptr &&
							 strcmp ( al->data2, amount_s )     == 0 ) {

							rumbled = true;
							break;

						}

					}

				}

			}

        }
    }

    //
    // Has the player left a log on the target bank system

    if ( !rumbled ) {

        BankAccount *target = BankAccount::GetAccount ( target_ip, target_accno );

        if ( target ) {

            for ( int i = 0; i < target->log.internallogs.Size (); ++i ) {

				if ( target->log.internallogs.ValidIndex ( i ) ) {

					AccessLog *al = target->log.internallogs.GetData (i);

					if ( al && al->TYPE == LOG_TYPE_TRANSFERFROM ) {

						if ( al->data1 && al->data2 ) {

							if ( strstr ( al->data1, source_ip.c_str() )    != nullptr &&
								 strstr ( al->data1, source_accno.c_str() ) != nullptr &&
								 strcmp ( al->data2, amount_s )     == 0 ) {

								rumbled = true;
								break;

							}

						}

					}

				}

			}



        }

    }


    //
    // To be or not to be

    if ( rumbled ) {

        VLocation *vl = game->GetWorld ()->GetVLocation ( source_ip );
        UplinkAssert (vl)
        Computer *comp = vl->GetComputer ();
        UplinkAssert (comp)

		std::ostrstream deathmsg;
		deathmsg << "Disavowed by Uplink Corporation at " << game->GetWorld ()->date.GetLongString ()
				 << "\nFor attempting to steal money from "
				 << comp->name << "\n"
                 << '\x0';

		game->GameOver ( deathmsg.str () );

    }
    else {

        game->GetWorld ()->GetPlayer ()->rating.ChangeUplinkScore ( UPLINKCHANGE_ROBBANK );
        game->GetWorld ()->GetPlayer ()->rating.ChangeNeuromancerScore ( NEUROMANCERCHANGE_ROBBANK );

    }

}

string BankRobberyEvent::GetShortString ()
{
    return "Bank Robbery Event";
}

string BankRobberyEvent::GetLongString ()
{

    string longstring = "Bank Robbery Event\nSource : " + source_ip + ", " + source_accno + "\nTarget : " + target_ip + ", " + target_accno + "\nAmount : " + to_string(amount) + "\n";
    longstring += hackdate.GetLongString ();

    return longstring;

}

void BankRobberyEvent::SetDetails (const string &newsourceip, const string &newsourceaccno,
                                   const string &newtargetip, const string &newtargetaccno,
                                   int newamount, Date *newhackdate )
{

    source_ip = newsourceip;
    source_accno = newsourceaccno;
    target_ip = newtargetip;
    target_accno = newtargetaccno;

    amount = newamount;
    hackdate.SetDate ( newhackdate );

}

bool BankRobberyEvent::Load  ( FILE *file )
{

    LoadID ( file );

    if ( !UplinkEvent::Load ( file ) ) return false;

	if ( strcmp( game->GetLoadedSavefileVer(), "SAV59" ) >= 0 ) {
		if ( !LoadDynamicStringInt ( source_ip, file ) ) return false;
		if ( !LoadDynamicStringInt ( source_accno, file ) ) return false;
		if ( !LoadDynamicStringInt ( target_ip, file ) ) return false;
		if ( !LoadDynamicStringInt ( target_accno, file ) ) return false;
	}
	else {
		if ( !FileReadDataInt ( source_ip, 128, file ) ) {
			return false;
		}
		if ( !FileReadDataInt ( source_accno, 128, file ) ) {
			return false;
		}
		if ( !FileReadDataInt ( target_ip, 128, file ) ) {
			return false;
		}
		if ( !FileReadDataInt ( target_accno, 128, file ) ) {
			return false;
		}
	}

    if ( !FileReadData ( &amount, sizeof(amount), 1, file ) ) return false;
    if ( !hackdate.Load ( file ) ) return false;

    LoadID_END ( file );

	return true;

}

void BankRobberyEvent::Save  ( FILE *file )
{

    SaveID ( file );

    UplinkEvent::Save ( file );

	SaveDynamicString ( source_ip, sizeof(source_ip), file );
	SaveDynamicString ( source_accno, sizeof(source_accno), file );
	SaveDynamicString ( target_ip, sizeof(target_ip), file );
	SaveDynamicString ( target_accno, sizeof(target_accno), file );

    fwrite ( &amount, sizeof(amount), 1, file );
    hackdate.Save ( file );

    SaveID_END ( file );

}

void BankRobberyEvent::Print ()
{

    cout << "Bank Robbery Event" << endl;
    UplinkEvent::Print ();
    PrintValue("Source IP", source_ip);
    PrintValue("Souce Acc No", source_accno);
    PrintValue("Target IP", target_ip);
    PrintValue("Target Acc No", target_accno);
    PrintValue("Amount", amount);
    cout << "Date:" << endl;
    hackdate.Print ();

}

string BankRobberyEvent::GetID ()
{

    return "EVT_BANK";

}

int BankRobberyEvent::GetOBJECTID ()
{

    return OID_BANKROBBERYEVENT;

}

