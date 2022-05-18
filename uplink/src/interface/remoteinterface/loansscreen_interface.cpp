
#include "eclipse.h"

#include "app/globals.h"
#include "app/serialise.h"
#include "app/opengl_interface.h"

#include "game/game.h"
#include "game/data/data.h"

#include "world/world.h"
#include "world/player.h"
#include "world/computer/computer.h"
#include "world/computer/bankcomputer.h"

#include "interface/interface.h"
#include "interface/remoteinterface/remoteinterface.h"
#include "interface/remoteinterface/loansscreen_interface.h"

#include "mmgr.h"



LoansScreenInterface::LoansScreenInterface ()
{
	
	account = nullptr;
	loanchange = 0;

}

LoansScreenInterface::~LoansScreenInterface ()
= default;

bool LoansScreenInterface::EscapeKeyPressed ()
{

    CloseClick (nullptr);
    return true;

}

void LoansScreenInterface::IncreaseLoanClick ( Button *button )
{

	auto *thisint = (LoansScreenInterface *) GetInterfaceScreen ( SCREEN_LOANSSCREEN );
	UplinkAssert (thisint)

	Person *accountowner;
	if ( thisint->account && thisint->IsVisible () && ( accountowner = thisint->account->GetPerson () ) ) {
		
		// Make sure we don't take out too much

		int maxcredit = thisint->account->loan + accountowner->rating.creditrating * 100;

		if ( thisint->account->loan < maxcredit ) {

			thisint->loanchange += 100;
			if ( thisint->account->loan + thisint->loanchange > maxcredit ) 
				thisint->loanchange = maxcredit - thisint->account->loan;

			int newloan = thisint->account->loan + thisint->loanchange;
			int newbalance = thisint->account->balance + thisint->loanchange;

			// Update the balance

			string balance = to_string(newbalance);
			EclGetButton ( "loansscreen_balance" )->SetCaption ( balance );
		
			// Update the loan

			string loan = to_string(newloan);
			EclGetButton ( "loansscreen_loan" )->SetCaption ( loan );

			// Update the interest rate 

			string rate;
			if		( newloan <= SMALLLOAN_MAX ) {			rate = "(" + to_string(int (100.0 * SMALLLOAN_INTEREST)) + "%% apr)";
			} else if ( newloan <= MEDIUMLOAN_MAX ) {		rate = "(" + to_string(int (100.0 * MEDIUMLOAN_INTEREST)) + "%% apr)";
			} else if ( newloan <= LARGELOAN_MAX ) {		rate = "(" + to_string(int (100.0 * LARGELOAN_INTEREST)) + "%% apr)";
			} else {										rate = "(" + to_string(int (100.0 * MAXLOAN_INTEREST)) + "%% apr)";
			}

			EclGetButton ( "loansscreen_loanrate" )->SetCaption ( rate );

		}

	}

}

void LoansScreenInterface::DecreaseLoanClick ( Button *button )
{

	auto *thisint = (LoansScreenInterface *) GetInterfaceScreen ( SCREEN_LOANSSCREEN );
	UplinkAssert (thisint)

	Person *accountowner;
	if ( thisint->account && thisint->IsVisible () && ( accountowner = thisint->account->GetPerson () ) ) {
		
		thisint->loanchange -= 100;
		if ( thisint->account->loan + thisint->loanchange < 0 ) thisint->loanchange = thisint->account->loan * -1;

		int newloan = thisint->account->loan + thisint->loanchange;
		int newbalance = thisint->account->balance + thisint->loanchange;

		// Update the balance

		string balance = to_string(newbalance);
		EclGetButton ( "loansscreen_balance" )->SetCaption ( balance );
	
		// Update the loan

		string loan = to_string(newloan);
		EclGetButton ( "loansscreen_loan" )->SetCaption ( loan );

		// Update the interest rate 

		string rate;
		if		( newloan <= SMALLLOAN_MAX ) {			rate= "(" + to_string(int (100.0 * SMALLLOAN_INTEREST)) + "%% apr)";
		} else if ( newloan <= MEDIUMLOAN_MAX ) {		rate= "(" + to_string(int (100.0 * MEDIUMLOAN_INTEREST)) + "%% apr)";
		} else if ( newloan <= LARGELOAN_MAX ) {		rate= "(" + to_string(int (100.0 * LARGELOAN_INTEREST)) + "%% apr)";
		} else {										rate= "(" + to_string(int (100.0 * MAXLOAN_INTEREST)) + "%% apr)";
		}

		EclGetButton ( "loansscreen_loanrate" )->SetCaption ( rate );
		
	}

}

void LoansScreenInterface::CloseClick ( Button *button )
{

	// Update the account if changed

	auto *thisint = (LoansScreenInterface *) GetInterfaceScreen ( SCREEN_LOANSSCREEN );
	UplinkAssert (thisint)

	Person *accountowner;
	if ( thisint->account && thisint->loanchange != 0 && ( accountowner = thisint->account->GetPerson () ) ) {

		thisint->account->loan += thisint->loanchange;
		thisint->account->balance += thisint->loanchange;

        int creditchange = thisint->loanchange / 100;
        accountowner->rating.ChangeCreditRating ( creditchange *-1 );

	}

	// Go to the next page

	int nextpage = thisint->GetComputerScreen ()->nextpage;
	if ( nextpage != -1 ) 
        game->GetInterface ()->GetRemoteInterface ()->RunScreen ( nextpage, thisint->GetComputerScreen ()->GetComputer () );

}

void LoansScreenInterface::Create ()
{

	if ( cs ) Create ( cs );
	else cout << "LoansScreenInterface::Create, tried to create when GenericScreen==nullptr" << endl;

}

void LoansScreenInterface::Create ( ComputerScreen *newcs )
{

	cs = newcs;

	// Look up the BankAccount object and store it
	Computer *comp = cs->GetComputer ();
	assert(comp->GetOBJECTID () == OID_BANKCOMPUTER );
	auto *bank = (BankComputer *) comp;

	// Look up the account number based on the log in name
	Record *rec = bank->recordbank.GetRecordFromName ( game->GetInterface ()->GetRemoteInterface ()->security_name );
	if ( !rec ) {
		account = nullptr;
		return;
	}

	char *accno = rec->GetField ( RECORDBANK_ACCNO );
	if ( !accno ) {
		account = nullptr;
		return;
	}

	// Now look up the account based on that account number
	account = bank->accounts.GetData ( accno );

	if ( !IsVisible () ) {

		EclRegisterButton ( 80, 60, 350, 25, GetComputerScreen ()->maintitle, "", "loansscreen_maintitle" );
		EclRegisterButtonCallbacks ( "loansscreen_maintitle", DrawMainTitle, nullptr, nullptr, nullptr );
		EclRegisterButton ( 80, 80, 350, 20, GetComputerScreen ()->subtitle, "", "loansscreen_subtitle" );
		EclRegisterButtonCallbacks ( "loansscreen_subtitle", DrawSubTitle, nullptr, nullptr, nullptr );

		// Account Name

		EclRegisterButton ( 70, 150, 100, 15, "Account Name", "Shows the name your account is in", "loansscreen_accountname_title" );
		EclRegisterButton ( 190, 150, 100, 15, "Accessing...", "", "loansscreen_accountname" );
		EclRegisterButtonCallbacks ( "loansscreen_accountname", textbutton_draw, nullptr, nullptr, nullptr );

		// Account number

		EclRegisterButton ( 70, 175, 100, 15, "Account No.", "Shows your account number", "loansscreen_accountnumber_title" );
		EclRegisterButton ( 190, 175, 100, 15, "Accessing...", "", "loansscreen_accountnumber" );
		EclRegisterButtonCallbacks ( "loansscreen_accountnumber", textbutton_draw, nullptr, nullptr, nullptr );

		// Balance

		EclRegisterButton ( 70, 200, 100, 15, "Balance", "Shows your bank balance", "loansscreen_balance_title" );
		EclRegisterButton ( 190, 200, 100, 15, "Accessing...", "", "loansscreen_balance" );
		EclRegisterButtonCallbacks ( "loansscreen_balance", textbutton_draw, nullptr, nullptr, nullptr );

		// Loan

		EclRegisterButton ( 70, 225, 100, 15, "Loan", "Shows your the size of your loan", "loansscreen_loan_title" );
		EclRegisterButton ( 190, 225, 100, 15, "Accessing...", "", "loansscreen_loan" );
		EclRegisterButtonCallbacks ( "loansscreen_loan", textbutton_draw, nullptr, nullptr, nullptr );

		// Interest rate

		EclRegisterButton ( 300, 225, 80, 15, "", "", "loansscreen_loanrate" );
		EclRegisterButtonCallbacks ( "loansscreen_loanrate", textbutton_draw, nullptr, nullptr, nullptr );


		if ( account ) {

			// Account name

			EclGetButton ( "loansscreen_accountname" )->SetCaption ( account->name );

			// Account number

			string accno = to_string(account->accountnumber);
			EclGetButton ( "loansscreen_accountnumber" )->SetCaption ( accno );

			// Update the balance

			string balance = to_string(account->balance);
			EclGetButton ( "loansscreen_balance" )->SetCaption ( balance );
		
			// Update the loan

			string loan = to_string(account->loan);
			EclGetButton ( "loansscreen_loan" )->SetCaption ( loan );

			// Update the interest rate 

			string rate;
			if		( account->loan <= SMALLLOAN_MAX ) {		rate = "(" + to_string(int (100.0 * SMALLLOAN_INTEREST)) + "%% apr)";
			} else if ( account->loan <= MEDIUMLOAN_MAX ) {		rate = "(" + to_string(int (100.0 * MEDIUMLOAN_INTEREST)) + "%% apr)";
			} else if ( account->loan <= LARGELOAN_MAX ) {		rate = "(" + to_string(int (100.0 * LARGELOAN_INTEREST)) + "%% apr)";
			} else {											rate = "(" + to_string(int (100.0 * MAXLOAN_INTEREST)) + "%% apr)";
			}

			EclGetButton ( "loansscreen_loanrate" )->SetCaption ( rate );

			loanchange = 0;

		}
		else {

			EclGetButton ( "loansscreen_accountname" )->SetCaption ( "Unknown" );
			EclGetButton ( "loansscreen_accountnumber" )->SetCaption ( "Unknown" );

		}

		// Increase loan button

		EclRegisterButton ( 150, 260, 100, 15, "Increase loan", "Click to take out a larger loan", "loansscreen_increaseloan" );
		EclRegisterButtonCallback ( "loansscreen_increaseloan", IncreaseLoanClick );

		// Pay off loan button

		EclRegisterButton ( 150, 280, 100, 15, "Decrease loan", "Click to pay off some of the loan", "loansscreen_decreaseloan" );
		EclRegisterButtonCallback ( "loansscreen_decreaseloan", DecreaseLoanClick );

		// Max available loan button

		string maxloan;
		Person *accountowner;
		if ( account && ( accountowner = account->GetPerson () ) ) {
			maxloan = "Max loan: " + to_string(account->loan + accountowner->rating.creditrating * 100) + "c";
		}
		else {
			maxloan = "Max loan: 0dc";
		}

		EclRegisterButton ( 270, 260, 100, 15, maxloan, "", "loansscreen_maxloan" );
		EclRegisterButtonCallbacks ( "loansscreen_maxloan", textbutton_draw, nullptr, nullptr, nullptr );
		
		// OK button

		EclRegisterButton ( 150, 340, 100, 15, "OK", "Return to the account management screen", "loansscreen_close" );
		EclRegisterButtonCallback ( "loansscreen_close", CloseClick ); 


	}

}

void LoansScreenInterface::Remove ()
{
	
	if ( IsVisible () ) {

		EclRemoveButton ( "loansscreen_maintitle" );
		EclRemoveButton ( "loansscreen_subtitle" );

		EclRemoveButton ( "loansscreen_accountname_title" );
		EclRemoveButton ( "loansscreen_accountname" );

		EclRemoveButton ( "loansscreen_accountnumber_title" );
		EclRemoveButton ( "loansscreen_accountnumber" );

		EclRemoveButton ( "loansscreen_balance_title" );
		EclRemoveButton ( "loansscreen_balance" );

		EclRemoveButton ( "loansscreen_loan_title" );
		EclRemoveButton ( "loansscreen_loan" );
		EclRemoveButton ( "loansscreen_loanrate" );

		EclRemoveButton ( "loansscreen_increaseloan" );
		EclRemoveButton ( "loansscreen_decreaseloan" );
		EclRemoveButton ( "loansscreen_maxloan" );

		EclRemoveButton ( "loansscreen_close" );

	}

}

void LoansScreenInterface::Update ()
{
}

bool LoansScreenInterface::IsVisible ()
{
	
	return ( EclGetButton ( "loansscreen_maintitle" ) != nullptr );

}

int LoansScreenInterface::ScreenID ()
{

	return SCREEN_LOANSSCREEN;

}


GenericScreen *LoansScreenInterface::GetComputerScreen ()
{

	return (GenericScreen *) cs;

}

