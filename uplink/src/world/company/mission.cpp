
#include <cstring>
#include <app/miscutils.h>

#include "game/game.h"

#include "app/globals.h"
#include "app/serialise.h"

#include "world/world.h"
#include "world/company/mission.h"
#include "world/vlocation.h"
#include "world/generator/missiongenerator.h"

#include "mmgr.h"


Mission::Mission ()
{

	details = nullptr;
	fulldetails = nullptr;
	duedate = nullptr;
    
	whysomuchmoney = nullptr;
	howsecure = nullptr;
	whoisthetarget = nullptr;
	
	TYPE = MISSION_NONE;
	UplinkStrncpy ( description, "", sizeof ( description ) )
	UplinkStrncpy ( employer, "", sizeof ( employer ) )
	UplinkStrncpy ( contact, "", sizeof ( contact ) )

	UplinkStrncpy ( completionA, "", sizeof ( completionA ) )
	UplinkStrncpy ( completionB, "", sizeof ( completionB ) )
	UplinkStrncpy ( completionC, "", sizeof ( completionC ) )
	UplinkStrncpy ( completionD, "", sizeof ( completionD ) )
	UplinkStrncpy ( completionE, "", sizeof ( completionE ) )

	payment = 0;
	maxpayment = 0;
	paymentmethod = MISSIONPAYMENT_AFTERCOMPLETION;

    difficulty = 0;
    minuplinkrating = -1;
    acceptrating = 0;
	npcpriority = false;

}

Mission::~Mission ()
{

    delete [] details;
    delete [] fulldetails;
    delete [] whysomuchmoney;
    delete [] howsecure;
    delete [] whoisthetarget;

	delete duedate;

	DeleteLListData ( links );
	DeleteBTreeData ( &codes );

}

void Mission::SetTYPE ( int newTYPE )
{

	TYPE = newTYPE;

}

void Mission::SetCompletion (const string& newA, const string& newB, const string& newC, const string& newD, const string& newE )
{

	// No need to set them if nullptr is passed in - they will not be examined after all

	if ( !newA.empty() ) {
		assert( newA.length() < SIZE_MISSION_COMPLETION );
		UplinkStrncpy ( completionA, newA.c_str(), sizeof ( completionA ) )
	}

	if ( !newB.empty() ) {
		assert( newB.length() < SIZE_MISSION_COMPLETION );
		UplinkStrncpy ( completionB, newB.c_str(), sizeof ( completionB ) )
	}

	if ( !newC.empty() ) {
		assert( newC.length() < SIZE_MISSION_COMPLETION );
		UplinkStrncpy ( completionC, newC.c_str(), sizeof ( completionC ) )
	}

	if ( !newD.empty() ) {
		assert( newD.length() < SIZE_MISSION_COMPLETION );
		UplinkStrncpy ( completionD, newD.c_str(), sizeof ( completionD ) )
	}

	if ( !newE.empty() ) {
		assert( newE.length() < SIZE_MISSION_COMPLETION );
		UplinkStrncpy ( completionE, newE.c_str(), sizeof ( completionE ) )
	}

}

void Mission::SetCreateDate ( Date *date )
{

	UplinkAssert ( date )
	createdate.SetDate ( date );

}

void Mission::SetNpcPriority ( bool newpriority )
{

	npcpriority = newpriority;

}

void Mission::SetDescription (const string &newdescription )
{

	assert( newdescription.length() < SIZE_MISSION_DESCRIPTION );
	UplinkStrncpy ( description, newdescription.c_str(), sizeof ( description ) )

}

void Mission::SetDetails (const string &newdetails )
{

	delete [] details;

	details = new char [newdetails.length()+1];
	UplinkSafeStrcpy ( details, newdetails.c_str() )

}

void Mission::SetFullDetails (const string &newdetails )
{

	delete [] fulldetails;

	fulldetails = new char [newdetails.length()+1];
	UplinkSafeStrcpy ( fulldetails, newdetails.c_str() )

}

void Mission::SetWhySoMuchMoney	(const string &answer )
{

	delete [] whysomuchmoney;
	whysomuchmoney = nullptr;

	if ( !answer.empty() ) {

		whysomuchmoney = new char [answer.length()+1];
		UplinkSafeStrcpy ( whysomuchmoney, answer.c_str() )

	}

}

void Mission::SetHowSecure (const string &answer )
{

	delete [] howsecure;
	howsecure = nullptr;

	if ( !answer.empty() ) {

		howsecure = new char [answer.length()+1];
		UplinkSafeStrcpy ( howsecure, answer.c_str() )

	}

}

void Mission::SetWhoIsTheTarget (const string &answer )
{

	delete [] whoisthetarget;
	whoisthetarget = nullptr;

	if ( !answer.empty() ) {

		whoisthetarget = new char [answer.length()+1];
		UplinkSafeStrcpy ( whoisthetarget, answer.c_str() )

	}

}

void Mission::SetEmployer (const string &newemployer )
{

	assert( newemployer.length() < 64 );
	UplinkAssert ( game->GetWorld ()->GetCompany (newemployer) )

	UplinkStrncpy ( employer, newemployer.c_str(), sizeof ( employer ) )

}

void Mission::SetContact (const string &newcontact )
{

	assert( newcontact.length() < SIZE_PERSON_NAME );
	UplinkAssert ( game->GetWorld ()->GetPerson ( newcontact ) )

	UplinkStrncpy ( contact, newcontact.c_str(), sizeof ( contact ) )

}

void Mission::SetPayment ( int newpayment, int newmaxpayment )
{

	payment = newpayment;

	if ( newmaxpayment != -1 )
		maxpayment = newmaxpayment;

	else
		maxpayment = payment;

}

void Mission::SetDifficulty ( int newdifficulty )
{

	difficulty = newdifficulty;

}

void Mission::SetMinRating ( int newrating )
{

	minuplinkrating = newrating;

}

void Mission::SetAcceptRating ( int newrating )
{

	acceptrating = newrating;

}

void Mission::GiveLink (const string &ip )
{

	links.PutData ( ip );

}

void Mission::GiveCode (const string &ip, const string &code )
{

	assert( ip.length() < SIZE_VLOCATION_IP );
	char theip [SIZE_VLOCATION_IP];
	UplinkStrncpy ( theip, ip.c_str(), sizeof ( theip ) )

	char *thecode = new char [code.length()+1];
	UplinkSafeStrcpy ( thecode, code.c_str() )

	codes.PutData ( theip, thecode );

}

void Mission::SetDueDate ( Date *newdate )
{

	if ( !duedate )
		duedate = new Date();

	duedate->SetDate( newdate );

}

Date *Mission::GetDueDate ()
{

	return duedate;

}

char *Mission::GetDetails ()
{

	UplinkAssert ( details )
	return details;

}

char *Mission::GetFullDetails ()
{

	UplinkAssert ( fulldetails )
	return fulldetails;

}

char *Mission::GetWhySoMuchMoney ()
{

	return whysomuchmoney;

}

char *Mission::GetHowSecure	()
{

	return howsecure;

}

char *Mission::GetWhoIsTheTarget ()
{

	return whoisthetarget;

}

bool Mission::Load  ( FILE *file )
{

	LoadID ( file );

	duedate = nullptr;

	if ( !FileReadData ( &TYPE, sizeof(TYPE), 1, file ) ) return false;
	
	if ( !LoadDynamicStringStatic ( completionA, SIZE_MISSION_COMPLETION, file ) ) return false;
	if ( !LoadDynamicStringStatic ( completionB, SIZE_MISSION_COMPLETION, file ) ) return false;
	if ( !LoadDynamicStringStatic ( completionC, SIZE_MISSION_COMPLETION, file ) ) return false;
	if ( !LoadDynamicStringStatic ( completionD, SIZE_MISSION_COMPLETION, file ) ) return false;
	if ( !LoadDynamicStringStatic ( completionE, SIZE_MISSION_COMPLETION, file ) ) return false;

	if ( !createdate.Load ( file ) ) return false;

	if ( !LoadDynamicStringStatic ( employer, SIZE_MISSION_EMPLOYER, file ) ) return false;
	if ( !LoadDynamicStringStatic ( contact, SIZE_PERSON_NAME, file ) ) return false;
	if ( !LoadDynamicStringStatic ( description, SIZE_MISSION_DESCRIPTION, file ) ) return false;

	if ( !FileReadData ( &payment, sizeof(payment), 1, file ) ) return false;
	if ( !FileReadData ( &difficulty, sizeof(difficulty), 1, file ) ) return false;
	if ( !FileReadData ( &minuplinkrating, sizeof(minuplinkrating), 1, file ) ) return false;
	if ( !FileReadData ( &acceptrating, sizeof(acceptrating), 1, file ) ) return false;
	if ( !FileReadData ( &npcpriority, sizeof(npcpriority), 1, file) ) return false;
	
	if ( !LoadDynamicStringPtr ( &details, file ) ) return false;
	if ( !LoadDynamicStringPtr ( &fulldetails, file ) ) return false;
	if ( !LoadDynamicStringPtr ( &whysomuchmoney, file ) ) return false;
	if ( !LoadDynamicStringPtr ( &howsecure, file ) ) return false;
	if ( !LoadDynamicStringPtr ( &whoisthetarget, file ) ) return false;
	
	if ( !LoadLList ( links, file ) ) return false;
	if ( !LoadBTree ( &codes, file ) ) return false;

	bool hasduedate;
	if ( !FileReadData ( &hasduedate, sizeof(hasduedate), 1, file ) ) return false;

	if ( hasduedate ) {
		duedate = new Date ();
		if ( !duedate->Load ( file ) ) return false;
	}

	if ( strcmp( game->GetLoadedSavefileVer(), "SAV57" ) >= 0 ) {

		if ( !FileReadData ( &maxpayment, sizeof(maxpayment), 1, file ) ) return false;
		if ( !FileReadData ( &paymentmethod, sizeof(paymentmethod), 1, file ) ) return false;

	}

	LoadID_END ( file );

	return true;

}

void Mission::Save  ( FILE *file )
{

	SaveID ( file );

	fwrite ( &TYPE, sizeof(TYPE), 1, file );
	
	SaveDynamicString ( completionA, SIZE_MISSION_COMPLETION, file );
	SaveDynamicString ( completionB, SIZE_MISSION_COMPLETION, file );
	SaveDynamicString ( completionC, SIZE_MISSION_COMPLETION, file );
	SaveDynamicString ( completionD, SIZE_MISSION_COMPLETION, file );
	SaveDynamicString ( completionE, SIZE_MISSION_COMPLETION, file );

	createdate.Save ( file );

	SaveDynamicString ( employer, SIZE_MISSION_EMPLOYER, file );
	SaveDynamicString ( contact, SIZE_PERSON_NAME, file );
	SaveDynamicString ( description, SIZE_MISSION_DESCRIPTION, file );

	fwrite ( &payment, sizeof(payment), 1, file );
	fwrite ( &difficulty, sizeof(difficulty), 1, file );
	fwrite ( &minuplinkrating, sizeof(minuplinkrating), 1, file );
	fwrite ( &acceptrating, sizeof(acceptrating), 1, file );
	fwrite ( &npcpriority, sizeof(npcpriority), 1, file );
	
	SaveDynamicString ( details, file );
	SaveDynamicString ( fulldetails, file );
	SaveDynamicString ( whysomuchmoney, file );
	SaveDynamicString ( howsecure, file );
	SaveDynamicString ( whoisthetarget, file );

	SaveLList ( links, file );
	SaveBTree ( &codes, file );

	bool hasduedate = (duedate != nullptr);
	fwrite ( &hasduedate, sizeof(hasduedate), 1, file );
	if ( hasduedate ) 	duedate->Save ( file );

	fwrite ( &maxpayment, sizeof(maxpayment), 1, file );
	fwrite ( &paymentmethod, sizeof(paymentmethod), 1, file );
	
	SaveID_END ( file );

}

void Mission::Print ()
{

	cout <<"Mission : " << endl;
	PrintValue("TYPE", TYPE);
    PrintValue("Employer", employer);
    PrintValue("Payment", payment);
    PrintValue("Difficulty", difficulty);
    PrintValue("MinRating", minuplinkrating);
    PrintValue("AcceptRating", acceptrating);
    PrintValue("Description", description);
    PrintValue("NPC priority", npcpriority);
	createdate.Print ();
    PrintValue("CompletionA", completionA);
    PrintValue("CompletionB", completionB);
    PrintValue("CompletionC", completionC);
    PrintValue("CompletionD", completionD);
    PrintValue("CompletionE", completionE);
    PrintValue("Contact", contact);
    PrintValue("Details", details);
    PrintValue("FullDetails", fulldetails);
    PrintValue("WhySoMuchMoney", whysomuchmoney);
    PrintValue("HowSecure", howsecure);

	PrintLList ( links );
	PrintBTree ( &codes );

	if ( duedate ) {
        cout << "\tDue date :" << endl;
		duedate->Print ();
	}
	else
        cout << "\tNo due date" << endl;

}
	
string Mission::GetID ()
{

	return "MISSION";

}

int Mission::GetOBJECTID ()
{

	return OID_MISSION;

}

