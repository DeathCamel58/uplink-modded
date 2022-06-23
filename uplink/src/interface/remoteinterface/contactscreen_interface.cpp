
#ifdef WIN32
#include <windows.h>
#endif

#include <strstream>

#include <GL/gl.h>

#include <sstream>

#include "game/game.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/opengl_interface.h"

#include "world/world.h"
#include "world/player.h"
#include "world/company/companyuplink.h"
#include "world/company/mission.h"
#include "world/computer/recordbank.h"
#include "world/computer/computerscreen/genericscreen.h"
#include "world/generator/recordgenerator.h"
#include "world/generator/numbergenerator.h"

#include "interface/interface.h"
#include "interface/remoteinterface/remoteinterface.h"
#include "interface/remoteinterface/contactscreen_interface.h"

#include "mmgr.h"


#define		NUMLINES		14



ContactScreenInterface::ContactScreenInterface ()
{

	TYPE = CONTACT_TYPE_NONE;
	mission = nullptr;
	waiting = false;
	numquestions = 0;
	originalpayment = 0;
	originalpaymentmethod = 0;

}

ContactScreenInterface::~ContactScreenInterface ()
= default;


void ContactScreenInterface::BorderDraw ( Button *button, bool highlighted, bool clicked )
{

	glColor3f ( 1.0f, 1.0f, 1.0f );
	border_draw ( button );

}

void ContactScreenInterface::MessageDraw ( Button *button, bool highlighted, bool clicked )
{

	glColor3f ( 0.7f, 0.7f, 0.7f );
	clear_draw ( button->x, button->y, button->width, button->height );
	text_draw ( button, highlighted, clicked );

}

void ContactScreenInterface::PostClick ( Button *button )
{

	Button *b = EclGetButton ( "contact_typehere" );
	//UplinkAssert (b);

	if ( b ) {

		PutMessage ( 1, b->caption );

		b->SetCaption ( " " );
		EclDirtyButton ( b->name );

	}

}

void ContactScreenInterface::TypeHereDraw ( Button *button, bool highlighted, bool clicked )
{

	textbutton_draw ( button, highlighted, clicked );

}

void ContactScreenInterface::QuestionDraw ( Button *button, bool highlighted, bool clicked )
{

	textbutton_draw ( button, highlighted, clicked );

}

void ContactScreenInterface::QuestionClick ( Button *button )
{

	// Find the question index

	int index;
	char name [32];
    istringstream stream(button->name);
    stream >> name >> index;

	// Post this message on the board

	PutMessage ( 1, button->caption );

	// Trigger a response

	AskQuestion ( index );

}

void ContactScreenInterface::PutMessage (int userid, const string& message )
{

	// Work out the full message

	string fullmessage;

	auto *thisint = (ContactScreenInterface *) GetInterfaceScreen ( SCREEN_CONTACTSCREEN );

	if	( userid == 0 )	fullmessage = message;
	else				fullmessage = thisint->users.GetData (userid) + " : " + message;


	// Add the message to the queue

	thisint->messagequeue.PutData ( fullmessage );

}

void ContactScreenInterface::WaitingCallback ()
{

	auto *thisint = (ContactScreenInterface *) GetInterfaceScreen ( SCREEN_CONTACTSCREEN );

	thisint->waiting = false;

}

void ContactScreenInterface::SetTYPE ( int newTYPE )
{

	TYPE = newTYPE;

}

void ContactScreenInterface::SetMission ( Mission *newmission )
{

	UplinkAssert ( newmission )
	assert( TYPE == CONTACT_TYPE_MISSION );

	mission = newmission;

	originalpayment = mission->payment;
	originalpaymentmethod = mission->paymentmethod;
	askedpayment = false;
	askedpaymentmethod = false;

	//
	// Add the contact to the screen
	//

	AddUser ( "Anonymous" );

	//
	// Will the contact speak to us?
	//

	Record *rec = RecordGenerator::GetCriminal ( "PLAYER" );
	UplinkAssert (rec)
	char *crimrec = nullptr;
	crimrec = rec->GetField ( "Convictions" );
	UplinkAssert (crimrec)

	int numconvictions = 0;
	char *currentconviction = strchr ( crimrec, '\n' );
	while ( currentconviction ) {
		++numconvictions;
		currentconviction = strchr ( currentconviction+1, '\n' );
	}

	PutMessage ( 0, " " );

	if ( game->GetWorld ()->GetPlayer ()->rating.uplinkrating == mission->minuplinkrating &&
		 numconvictions > 0 ) {

		// If the player is only just good enough,
		// and has a criminal record, the employer
		// will choose someone else

		PutMessage ( 2, "I'm sorry, but we were looking for someone" );
		PutMessage ( 0, "with a cleaner criminal history." );

		AddQuestion ( "OK, Fine", 7 );

	}
	else {

		PutMessage ( 2, "Thank you for your time, agent." );
		PutMessage ( 0, "How can we help?" );

		// Add the questions

		AddQuestion ( "Why so much money?" );
		AddQuestion ( "Who is the target?" );
		AddQuestion ( "How secure is the target?" );
		AddQuestion ( "I want more money" );
		AddQuestion ( "I want half the money now" );
		AddQuestion ( "I want all the money now" );
		AddQuestion ( "OK, agreed." );
		AddQuestion ( "No thanks." );

	}

}

void ContactScreenInterface::AddUser (string name )
{

	users.PutDataAtEnd ( name );

	if ( name == "System" ) {

		PutMessage ( 0, "Secure online communication channel established, Port 80" );
		PutMessage ( 0, "Running SCi V2.6, copyright Frontier Communications Ltd" );
		PutMessage ( 0, "Conference online" );
		PutMessage ( 0, " " );

	}
	else {

		string message = "User '" + name + "' has entered the conference";

		PutMessage ( 0, message );

	}

}

void ContactScreenInterface::AddQuestion (const string& question, int index )
{

	if ( index == -1 ) {
		index = numquestions;
		++numquestions;
	}
	else {
		if ( index > numquestions )
			numquestions = index + 1;
	}

	string name1 = "contact_questionb " + to_string(index);
    string name2 = "contact_question " + to_string(index);

	int x = ( index < 5 ? 20 : 230 );
	int y = ( index < 5 ? 300 + index * 20 : 300 + (index-5) * 20 );

	EclRegisterButton ( x, y, 16, 16, question, "Click to ask this question", name1 );
	button_assignbitmaps ( name1, "menuscreenoption.tif", "menuscreenoption_h.tif", "menuscreenoption_c.tif" );
	EclRegisterButtonCallback ( name1, QuestionClick );

	EclRegisterButton ( x + 15, y, 185, 15, question, "Click to ask this question", name2 );
	EclRegisterButtonCallbacks ( name2, QuestionDraw, QuestionClick, button_click, button_highlight );

}

void ContactScreenInterface::AskQuestion ( int index )
{

	auto *thisint = (ContactScreenInterface *) GetInterfaceScreen ( SCREEN_CONTACTSCREEN );

	UplinkAssert (thisint->mission)

	switch ( thisint->TYPE ) {

		case CONTACT_TYPE_NONE:
			break;

		case CONTACT_TYPE_MISSION:
			{

			auto *cu = (CompanyUplink *) game->GetWorld ()->GetCompany ( "Uplink" );
			UplinkAssert ( cu )
			int indexmission = cu->missions.FindData ( thisint->mission );

			// ??? Mission could have expired or could have been taken by a NPC Agent ???

			if ( indexmission == -1 && index != 7 ) {
				PutMessage ( 2, "The mission is not available anymore." );
				break;
			}

			switch ( index ) {

				case 0:									// Why so much money?

					if ( thisint->mission->GetWhySoMuchMoney () ) {

						PutMessage ( 2, thisint->mission->GetWhySoMuchMoney () );

					}
					else {

						PutMessage ( 2, "No particular reason - it's a standard mission." );

					}

					PutMessage ( 0, " " );
					break;

				case 1:									// Who is the target?

					if ( thisint->mission->GetWhoIsTheTarget () ) {

						PutMessage ( 2, thisint->mission->GetWhoIsTheTarget () );

					}
					else {

						PutMessage ( 2, "We'd rather not reveal that at this time." );

					}

					PutMessage ( 0, " " );
					break;

				case 2:									// How secure is the target

					if ( thisint->mission->GetHowSecure () ) {

						PutMessage ( 2, thisint->mission->GetHowSecure () );

					}
					else {

						switch ( thisint->mission->difficulty ) {

							case 0 :
							case 1 :	PutMessage ( 2, "The security will be non-existent." );			break;
							case 2 :
							case 3 :	PutMessage ( 2, "The security will be low." );					break;
							case 4 :
							case 5 :	PutMessage ( 2, "There will be some defense in action." );		break;
							case 6 :
							case 7 :	PutMessage ( 2, "We expect security to be high." );				break;
							case 8 :
							case 9 :	PutMessage ( 2, "Security will be very tight." );				break;
							case 10: case 11: case 12: case 13: case 14: case 15: case 16: case 17: case 18: case 19:
							case 20:	PutMessage ( 2, "Security will be extremely high." );			break;

							default:	UplinkAbortArgs ( "Unrecognised mission difficulty %d", thisint->mission->difficulty )


						}

						if ( thisint->mission->difficulty > 6 )
							PutMessage ( 0, "You will certainly encounter Proxy's or Firewalls." );

						else if ( thisint->mission->difficulty > 4 )
							PutMessage ( 0, "You may encounter Proxy servers or Firewalls." );

					}

					PutMessage ( 0, " " );
					break;

				case 3:									// I want more money

					if ( thisint->askedpayment ) {

						PutMessage ( 2, "We've already agreed on the amount." );

					}
					else {

						// Chance of more money - 90% if he has rating of (missiondifficulty+3) or more
						// Decreasing by 30% for each rating below that
						// no chance if rating <= missiondifficulty

						int ratingdifference = game->GetWorld ()->GetPlayer ()->rating.uplinkrating - (thisint->mission->acceptrating+3);
						int chance = 90 + ratingdifference * 30;

						if ( chance >= 100 ||
							 ( chance > 0 && NumberGenerator::RandomNumber (100) <= chance ) ) {

							if ( thisint->mission->maxpayment == 0 ||
								 thisint->mission->payment == thisint->mission->maxpayment ) {

								PutMessage ( 2, "I'm afraid we can't offer you any more money." );

							}
							else {

								string message = "OK, we'll give you " + to_string(thisint->mission->maxpayment) + "c because of your reputation.";
								PutMessage ( 2, message );

								thisint->mission->payment = thisint->mission->maxpayment;

							}

						}
						else {

							PutMessage ( 2, "I'm afraid we can't offer you any more money." );

						}

						thisint->askedpayment = true;

					}

					PutMessage ( 0, " " );
					break;

				case 4:									// I want half the money now

					if ( thisint->askedpaymentmethod ) {

						PutMessage ( 2, "We've already agreed on payment." );

					}
					else {

						// Chance of half payment : 100% at rating 7,
						// decreasing by 10% for each rating

						int ratingdifference = game->GetWorld ()->GetPlayer ()->rating.uplinkrating - 7;
						int chance = 100 + ratingdifference * 10;

						if ( chance >= 100 ||
							 ( chance > 0 && NumberGenerator::RandomNumber (100) <= chance ) ) {

							PutMessage ( 2, "OK, agreed." );
							thisint->mission->paymentmethod = MISSIONPAYMENT_HALFATSTART;

						}
						else {

							PutMessage ( 2, "I'm afraid I can't agree to that." );

						}

						thisint->askedpaymentmethod = true;

					}

					PutMessage ( 0, " " );
					break;

				case 5:									// I want all the money now

					if ( thisint->askedpaymentmethod ) {

						PutMessage ( 2, "We've already agreed on payment." );

					}
					else {

						// Chance of full payment : 100% at rating 10,
						// decreasing by 10% for each rating

						int ratingdifference = game->GetWorld ()->GetPlayer ()->rating.uplinkrating - 10;
						int chance = 100 + ratingdifference * 10;

						if ( chance >= 100 ||
							 ( chance > 0 && NumberGenerator::RandomNumber (100) <= chance ) ) {

							PutMessage ( 2, "OK, since you have such an excellent record." );
							thisint->mission->paymentmethod = MISSIONPAYMENT_ALLATSTART;

						}
						else {

							PutMessage ( 2, "No way - do you think i'm stupid?" );

						}

						thisint->askedpaymentmethod = true;

					}

					PutMessage ( 0, " " );
					break;

				case 6:									// OK, agreed
				{

					// Give the mission to the player

					game->GetWorld ()->GetPlayer ()->GiveMission ( thisint->mission );

					// Remove the mission from the bulletin board

					cu->missions.RemoveData ( indexmission );

					// Run the bulletin board

					game->GetInterface ()->GetRemoteInterface ()->RunScreen ( thisint->GetComputerScreen ()->nextpage, thisint->GetComputerScreen ()->GetComputer () );

				}
				break;

				case 7:									// No thanks

					if ( indexmission != -1 ) {

						// Reset the mission
						thisint->mission->payment = thisint->originalpayment;
						thisint->mission->paymentmethod = thisint->originalpaymentmethod;

					}

					game->GetInterface ()->GetRemoteInterface ()->RunScreen ( thisint->GetComputerScreen ()->nextpage, thisint->GetComputerScreen ()->GetComputer () );

					break;

				default:
					UplinkAbort ( "Unrecognised question index" )

			}

			}
			break;

		default:
			UplinkAbort ( "Unrecognised contact TYPE" )

	}

}

bool ContactScreenInterface::ReturnKeyPressed ()
{

	PostClick (nullptr);
	return true;

}

void ContactScreenInterface::Create ( ComputerScreen *newcs )
{

	UplinkAssert (newcs)
	cs = newcs;

	if ( !IsVisible () ) {

		EclRegisterButton ( 20, 30, 405, 15, "Secure Communication Channel open", "", "contact_title" );

		EclRegisterButton ( 20, 45, 405, NUMLINES * 15 + 10, "", "", "contact_border" );
		EclRegisterButtonCallbacks ( "contact_border", BorderDraw, nullptr, nullptr, nullptr );

		for ( int i = 0; i < NUMLINES; ++i ) {

			int y = 50 + ( i * 15 );
			string name = "contact_text " + to_string(i);

			EclRegisterButton ( 22, y, 400, 15, "", "", name );
			EclRegisterButtonCallbacks ( name, MessageDraw, nullptr, nullptr, nullptr );

		}

		int ybottom = 50 + NUMLINES * 15 + 6;

/*
		EclRegisterButton ( 20, ybottom, 355, 15, "Type your message here", "", "contact_typehere" );
		EclRegisterButtonCallbacks ( "contact_typehere", TypeHereDraw, nullptr, button_click, button_highlight );

		EclRegisterButton ( 375, ybottom, 50, 15, "Post", "Click here to post your message", "contact_post" );
		EclRegisterButtonCallback ( "contact_post", PostClick );

		EclMakeButtonEditable ( "contact_typehere" );
*/

		// Add in the users

		AddUser ( "System" );
		AddUser ( game->GetWorld ()->GetPlayer ()->handle );

	}

}

void ContactScreenInterface::Remove ()
{

	if ( IsVisible () ) {

		EclRemoveButton ( "contact_title" );
		EclRemoveButton ( "contact_border" );

		for ( int ti = 0; ti < NUMLINES; ++ti ) {

			string name = "contact_text " + to_string(ti);
			EclRemoveButton ( name );

		}

/*
		EclRemoveButton ( "contact_typehere" );
		EclRemoveButton ( "contact_post" );
*/

		// Remove question buttons

		for ( int qi = 0; qi < numquestions; ++qi ) {

			string name = "contact_question " + to_string(qi);
			EclRemoveButton ( name );

			name = "contact_questionb " + to_string(qi);
			EclRemoveButton ( name );

		}

	}

}

bool ContactScreenInterface::IsVisible ()
{

	return ( EclGetButton ( "contact_title" ) != nullptr );

}

void ContactScreenInterface::Update ()
{

	if ( !waiting && messagequeue.Size () > 0 ) {

		// Scroll the messages up

		for ( int i = 0; i < NUMLINES-1; ++i ) {

			string name1 = "contact_text " + to_string(i);
            string name2 = "contact_text " + to_string(i + 1);

			Button *b1 = EclGetButton ( name1 );
			UplinkAssert ( b1 )

			Button *b2 = EclGetButton ( name2 );
			UplinkAssert ( b2 )

			b1->SetCaption ( b2->caption );

			EclDirtyButton ( name1 );

		}

		// Set the message on the last button and remove the message from the queue

		string message = messagequeue.GetData (0);
		int time = (int) ( message.length() * 5 );

		string name1 = "contact_text " + to_string(NUMLINES-1);

		EclRegisterCaptionChange ( name1, message, time, WaitingCallback );
		waiting = true;

		messagequeue.RemoveData (0);

	}

}

int ContactScreenInterface::ScreenID ()
{

	return SCREEN_CONTACTSCREEN;

}

GenericScreen *ContactScreenInterface::GetComputerScreen ()
{

	return (GenericScreen *) cs;

}

