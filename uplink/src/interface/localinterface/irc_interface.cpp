

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

#pragma warning( disable:4786 )

#include "redshirt.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/opengl_interface.h"
#include "app/miscutils.h"

#include "options/options.h"

#include "game/game.h"

#include "world/world.h"
#include "world/player.h"

#include "interface/interface.h"
#include "interface/scrollbox.h"
#include "interface/localinterface/localinterface.h"
#include "interface/localinterface/irc_interface.h"

#include "mmgr.h"

#define min(a,b) (((a) < (b)) ? (a) : (b))

bool IRCInterface::connected = false;
LList <UplinkIRCMessage *> IRCInterface::buffer;
LList <UplinkIRCUser *> IRCInterface::users;

string IRCInterface::channelName;

WinsockInit *IRCInterface::winSockInit = nullptr;
CIrcSession *IRCInterface::cIrcSession = nullptr;
UplinkIRCMonitor *IRCInterface::uplinkIRCMonitor = nullptr;
//CIrcSessionInfo *IRCInterface::cIrcSessionInfo = nullptr;

Image *IRCInterface::imgSmileyHappy = nullptr;
Image *IRCInterface::imgSmileySad = nullptr;
Image *IRCInterface::imgSmileyWink = nullptr;


#define COLOUR_DEFAULT      0.6f, 0.6f, 0.6f
#define COLOUR_USER         0.7f, 0.8f, 1.0f
#define COLOUR_TEXT         1.0f, 1.0f, 1.0f
#define COLOUR_ACTION       1.0f, 0.7f, 0.8f
#define COLOUR_JOINPART     0.6f, 1.0f, 0.6f
#define COLOUR_TOPIC        1.0f, 0.8f, 0.7f
#define COLOUR_MOTD         0.7f, 0.8f, 1.0f
#define COLOUR_MODE         1.0f, 0.8f, 0.8f


void IRCInterface::MainTextDraw ( Button *button, bool highlighted, bool clicked )
{

	UplinkAssert (button)

    // Clipping

	int screenh = app->GetOptions ()->GetOptionValue ( "graphics_screenheight" );
	glScissor ( button->x, screenh - (button->y + button->height), button->width, button->height );	
	glEnable ( GL_SCISSOR_TEST );

	clear_draw( button->x, button->y, button->width, button->height );
	glColor3f( 1.0f, 1.0f, 1.0f );
	border_draw( button );

	//int mainHeight = (int) ( screenh * 0.8 );
	// Dimension to match linksscreen_interface.cpp
	int mainHeight = ( 150 + ( ( screenh - 120 - 145 ) / 15 ) * 15 + 15 ) - 30;

	int numRows = mainHeight / 15 - 1;
    ScrollBox *scrollBox = ScrollBox::GetScrollBox( "ircscroller" );
    if ( !scrollBox ) return;
    int baseOffset = scrollBox->currentIndex;
	if ( scrollBox->numItems < scrollBox->windowSize )
		baseOffset = scrollBox->numItems - scrollBox->windowSize;

	for ( int i = 0; i < numRows; ++i ) {

        int thisRow = baseOffset + i + 1;

		if ( buffer.ValidIndex (thisRow) ) {

			UplinkIRCMessage *msg = buffer.GetData(thisRow);
			UplinkAssert (msg)

			int xpos = button->x + 10;
			int ypos = button->y + 10 + i * 15;

            if ( !msg->user.empty() ) {
                glColor3f( COLOUR_USER );
                GciDrawText ( xpos, ypos, msg->user );
                xpos += 80;
                // TODO : Handle big nick names
            }

            glColor3f( msg->red, msg->green, msg->blue );
          	GciDrawText ( xpos, ypos, msg->text );
			
            AddEmoticons ( i, ":)", imgSmileyHappy );
            AddEmoticons ( i, "=)", imgSmileyHappy );
            AddEmoticons ( i, ":-)", imgSmileyHappy );
            AddEmoticons ( i, ":(", imgSmileySad );
            AddEmoticons ( i, ":-(", imgSmileySad );
            AddEmoticons ( i, ";)", imgSmileyWink );

		}

	}

    glDisable ( GL_SCISSOR_TEST );

}

void IRCInterface::AddEmoticons (int row, const string& smiley, Image *imgSmiley )
{

    ScrollBox *scrollBox = ScrollBox::GetScrollBox( "ircscroller" );
    if ( !scrollBox ) return;
    int baseOffset = scrollBox->currentIndex;
	if ( scrollBox->numItems < scrollBox->windowSize )
		baseOffset = scrollBox->numItems - scrollBox->windowSize;

    int thisRow = baseOffset + row + 1;

	if ( buffer.ValidIndex (thisRow) ) {

		UplinkIRCMessage *msg = buffer.GetData(thisRow);
		UplinkAssert (msg)

        Button *button = EclGetButton ( "irc_maintext" );
        if ( !button ) return;

		int xpos = button->x + 10;
		int ypos = button->y + 10 + row * 15;
        if ( !msg->user.empty() ) xpos += 80;
        
        if ( imgSmiley ) {

            string textRemaining = msg->text;
            size_t nextSmiley = textRemaining.find( smiley );

            while ( nextSmiley != string::npos ) {
				size_t lenCopy = min ((nextSmiley - msg->text.length()), textRemaining.length() );
                string beforeSmiley = textRemaining.substr(0, nextSmiley);
                int smileyXpos = xpos + GciTextWidth ( beforeSmiley );
                textRemaining = textRemaining.substr(smiley.length(), textRemaining.length());

                imgSmiley->Draw ( smileyXpos, ypos - 7 );

                nextSmiley = textRemaining.find( smiley );
            }

        }

    }

}

void IRCInterface::ConnectDraw ( Button *button, bool highlighted, bool clicked )
{

	if ( connected )
		button->SetCaption( "Disconnect" );

	else
		button->SetCaption( "Connect" );

	button_draw ( button, highlighted, clicked );

}

void IRCInterface::BackBlackoutDraw ( Button *button, bool highlighted, bool clicked )
{

    clear_draw( button->x, button->y, button->width, button->height );

}

void IRCInterface::UserListDraw ( Button *button, bool highlighted, bool clicked )
{

	int screenheight = app->GetOptions ()->GetOptionValue ( "graphics_screenheight" );
	glScissor ( button->x, screenheight - (button->y + button->height), button->width, button->height );	
	glEnable ( GL_SCISSOR_TEST );

    LocalInterfaceScreen::BackgroundDraw( button, highlighted, clicked );

    ScrollBox *scrollBox = ScrollBox::GetScrollBox( "irc_userscroll" );
    if ( !scrollBox ) return;
    int baseOffset = scrollBox->currentIndex;
    int numRows = (button->height - 20) / 17;

    if ( connected ) {

        int xpos = button->x + 20;

        for ( int i = 0; i < numRows; ++i ) {

            if ( users.ValidIndex( i + baseOffset ) ) {

                int ypos = button->y + 20 + i * 17;
            
                if ( users.GetData(i + baseOffset)->status == 0 ) 
                    glColor3f ( 1.0f, 1.0f, 1.0f );
                else
                    glColor3f ( 1.0f, 0.5f, 0.5f );

                GciDrawText ( xpos, ypos, users.GetData(i + baseOffset)->name ); 

            }

        }

    }

    glDisable ( GL_SCISSOR_TEST );

}

void IRCInterface::ConnectClick ( Button *button )
{

    auto *thisint = (IRCInterface *) game->GetInterface ()->GetLocalInterface ()->GetInterfaceScreen ();
    UplinkAssert (thisint)

    if ( connected ) {
		
        cIrcSession->Disconnect( "Bye everyone!" );
        cIrcSession->RemoveMonitor( uplinkIRCMonitor );
        delete uplinkIRCMonitor;
        delete cIrcSession;
        delete winSockInit;
        uplinkIRCMonitor = nullptr;
        cIrcSession = nullptr;
        winSockInit = nullptr;
        
        ResetUsers();
        thisint->RemoveTalkWindow();

		for ( int i = 0; i < buffer.Size (); ++i )
			if ( buffer.ValidIndex ( i ) )
				delete buffer.GetData ( i );

		buffer.Empty();

		connected = false;
        
    }
    else {

		thisint->CreateTalkWindow();
        ResetUsers ();
        
		for ( int i = 0; i < buffer.Size (); ++i )
			if ( buffer.ValidIndex ( i ) )
				delete buffer.GetData ( i );

		buffer.Empty();

		winSockInit = new WinsockInit();
        cIrcSession = new CIrcSession();
        uplinkIRCMonitor = new UplinkIRCMonitor( *cIrcSession );
        cIrcSession->AddMonitor( uplinkIRCMonitor );

        int port = stoi( EclGetButton("irc_port")->caption );

        CIrcSessionInfo cIrcSessionInfo;

	    cIrcSessionInfo.Reset();
	    cIrcSessionInfo.sServer            = EclGetButton("irc_server")->caption;
	    cIrcSessionInfo.sServerName        = EclGetButton("irc_server")->caption;
	    cIrcSessionInfo.iPort              = port;
	    cIrcSessionInfo.sNick              = String( EclGetButton("irc_nickname")->caption );
        cIrcSessionInfo.sFullName          = EclGetButton("irc_fullname")->caption;
        cIrcSessionInfo.sUserID            = EclGetButton("irc_fullname")->caption;
	    cIrcSessionInfo.bIdentServer       = true;
	    cIrcSessionInfo.iIdentServerPort   = 113;
	    cIrcSessionInfo.sIdentServerType   = "UNIX";

        channelName =EclGetButton("irc_channel")->caption;

	    bool success = cIrcSession->Connect( cIrcSessionInfo );

		if ( success )
			connected = true;

    }

}

void IRCInterface::PostClick ( Button *button )
{

    if ( !connected ) return;
    if ( !cIrcSession ) return;

	Button *textButton = EclGetButton ( "irc_inputbox" );
	UplinkAssert (textButton )

    string newMessage;

    if ( textButton->caption.find("£") == 0 ) {

        newMessage = textButton->caption.substr(2, textButton->caption.length());

    }
    else if ( textButton->caption.find( "/nick" ) != string::npos ) {

        newMessage = "nick " + cIrcSession->GetInfo().sNick + ": " + textButton->caption.substr(7, textButton->caption.length());
    }
    else if ( textButton->caption.find( "/action" ) != string::npos ) {

        newMessage = "privmsg " + cIrcSession->GetInfo().sNick + ": " + channelName + " : ACTION " + textButton->caption.substr(9, textButton->caption.length());
    }
    else {

        newMessage = "privmsg " + cIrcSession->GetInfo().sNick + ": " + channelName + " :" + textButton->caption.substr(1, textButton->caption.length());
   }

    (*cIrcSession) << newMessage.c_str();
    
    textButton->SetCaption( " " );
    EclDirtyButton ( "irc_inputbox" );

}

void IRCInterface::TextScrollChange (const string &name, int newValue )
{
    
    EclDirtyButton ( "irc_maintext" );

}

void IRCInterface::UserScrollChange (const string &name, int newValue )
{

    EclDirtyButton ( "irc_userlist" );

}

void IRCInterface::AddText (const string& user, const string& text, float r, float g, float b )
{

	assert(!text.empty());
	

	// Break up the text into word wrapped lines
	
	int screenw = app->GetOptions ()->GetOptionValue ("graphics_screenwidth");
	int screenh = app->GetOptions ()->GetOptionValue ("graphics_screenheight");
	//int mainWidth = (int) ( screenw * 0.65 );
	// Dimension to match linksscreen_interface.cpp
	int mainWidth = 50 + SY(375) + 15;

	// Don't use this value of mainHeight, must match linksscreen_interface.cpp
	//int mainHeight = (int) ( screenh * 0.8 );

	LList <string> *wrapped = wordwraptext( text, mainWidth );

	if ( wrapped ) {
		int numLines = wrapped->Size();
		
		// Add those lines into the buffer
		
		for ( int i = 0; i < numLines; ++i ) {
			string theLine = wrapped->GetData(i);
			if ( !theLine.empty() ) {
				auto *msg = new UplinkIRCMessage ();
				string thisuser = ( i == 0 ? user : "" );
				msg->Set ( thisuser, theLine, r, g, b );
				buffer.PutDataAtEnd( msg );
			}
		}

		// Now finished with the wrapped stuff

		if ( wrapped->ValidIndex (0) && wrapped->GetData (0).empty() )
			wrapped->RemoveData(0);				// Only delete first entry - since there is only one string really
		delete wrapped;
	}
	
	// Trim the buffer to size

	while ( buffer.ValidIndex( IRCBUFFERSIZE ) ) {
		UplinkIRCMessage *msg = buffer.GetData( 0 );
		delete msg;
		buffer.RemoveData( 0 );
	}

    // Update the scrollbox

    ScrollBox *scrollBox = ScrollBox::GetScrollBox( "ircscroller" );
    if ( scrollBox ) {
        bool viewingNewest = false;
        if ( scrollBox->numItems <= scrollBox->windowSize || ( scrollBox->currentIndex == scrollBox->numItems - scrollBox->windowSize ) )
            viewingNewest = true;

        scrollBox->SetNumItems( buffer.Size() );        
		if ( viewingNewest ) {
			if ( scrollBox->numItems < scrollBox->windowSize )
				scrollBox->SetCurrentIndex ( 0 );
			else
				scrollBox->SetCurrentIndex ( scrollBox->numItems - scrollBox->windowSize );
		}
    }

    // Tell the screen to redraw

    EclDirtyButton ( "irc_maintext" );

}

void IRCInterface::ResetUsers ()
{

    while ( users.ValidIndex(0) ) {
        UplinkIRCUser *user = users.GetData(0);
        delete user;
        users.RemoveData(0);
    }

    ScrollBox *scrollBox = ScrollBox::GetScrollBox( "irc_userscroll" );
    if ( scrollBox ) scrollBox->SetNumItems( users.Size() );
    EclDirtyButton ( "irc_userlist" );

}

void IRCInterface::AddUser (string name )
{

    // Does the user exist already?

    UplinkIRCUser *userExists = GetUser (name);
    if ( !userExists && name[0] == '@' )
		userExists = GetUser ( name.substr(1, name.size()) );

    if ( userExists ) return;
    if ( name.size() < 2 ) return;

    // Create the new user

    auto *user = new UplinkIRCUser ();

    // Is he a channel op?

    if ( name[0] == '@' ) {
        user->Set (name.substr(1, name.size()));
        user->status = 1;
    }
    else
        user->Set( name );

    // Insert in order

    bool inserted = false;
    string lowerCaseName = LowerCaseString (name);

    for ( int i = 0; i < users.Size(); ++i ) {
        UplinkIRCUser *thisUser = users.GetData(i);
        UplinkAssert (thisUser)
        string lowerCaseThisUser = LowerCaseString ( thisUser->name );
        if ( ( user->status == 1 && thisUser->status == 0 )                                                            ||
             (user->status == 1 && thisUser->status == 1 && lowerCaseName.compare( lowerCaseThisUser ) < 0 )          ||
             (user->status == 0 && thisUser->status == 0 && lowerCaseName.compare( lowerCaseThisUser ) < 0 ) ) {

            users.PutDataAtIndex ( user, i );
            inserted = true;
            lowerCaseThisUser = "";
            break;
        }
        lowerCaseThisUser = "";

    }

    if ( !inserted ) users.PutDataAtEnd ( user );

    lowerCaseName = "";

    // Update the scrollbar

    ScrollBox *scrollBox = ScrollBox::GetScrollBox( "irc_userscroll" );
    if ( scrollBox ) scrollBox->SetNumItems( users.Size() );

    // Update the screen

    EclDirtyButton ( "irc_userlist" );

}

void IRCInterface::RemoveUser (const string& name )
{
    
    for ( int i = 0; i < users.Size(); ++i ) {
        UplinkIRCUser *user = users.GetData(i);
        UplinkAssert (user)
        if ( user->name == name ) {
            users.RemoveData( i );
            ScrollBox *scrollBox = ScrollBox::GetScrollBox( "irc_userscroll" );
            if ( scrollBox ) scrollBox->SetNumItems( users.Size() );
            return;
        }
    }

}

UplinkIRCUser *IRCInterface::GetUser (const string& name )
{

    for ( int i = 0; i < users.Size(); ++i ) {
        UplinkIRCUser *user = users.GetData(i);
        UplinkAssert (user)
        if ( user->name == name )
            return user;        
    }

    return nullptr;
}


UplinkIRCMessage::UplinkIRCMessage ()
{
	user = "";
    text = "";
}

UplinkIRCMessage::~UplinkIRCMessage ()
= default;

void UplinkIRCMessage::Set (const string& newuser, const string& newtext, float r, float g, float b )
{

    if ( !newuser.empty() ) {
        user = newuser;
    }

	if ( !newtext.empty() ) {
		text = newtext;
	}

	red = r;
	green = g;
	blue = b;

}


UplinkIRCUser::UplinkIRCUser ()
{
    name = "";
    status = 0;
}

UplinkIRCUser::~UplinkIRCUser ()
= default;

void UplinkIRCUser::Set (const string& newname )
{

    if ( !newname.empty() ) {
        name = newname;
    } else {
        name = "";
    }

}

IRCInterface::IRCInterface()
= default;

IRCInterface::~IRCInterface()
= default;

void IRCInterface::CreateTalkWindow()
{

	//
	// Main text box

	int screenw = app->GetOptions ()->GetOptionValue ("graphics_screenwidth");
	int screenh = app->GetOptions ()->GetOptionValue ("graphics_screenheight");
	//int mainWidth = (int) ( screenw * 0.65 );
	//int mainHeight = (int) ( screenh * 0.8 );
	// Dimension to match linksscreen_interface.cpp
	int mainWidth = 50 + SY(375) + 15 - 30;
	int mainHeight = ( 150 + ( ( screenh - 120 - 145 ) / 15 ) * 15 + 15 ) - 30;

	EclRegisterButton ( 30, 30, mainWidth - 15, mainHeight, " ", " ", "irc_maintext" );
	EclRegisterButtonCallbacks ( "irc_maintext", MainTextDraw, nullptr, nullptr, nullptr );

    //
    // Scroll bars

	int numRows = mainHeight / 15;
    ScrollBox::CreateScrollBox( "ircscroller", 
                                (30 + mainWidth) - 15, 30, 15, mainHeight, 
								buffer.Size(), numRows, ( buffer.Size() < numRows )? 0 : buffer.Size() - numRows, 
                                TextScrollChange );                

	//
	// Input box

	EclRegisterButton ( 30, mainHeight + 30, mainWidth - 50, 15, " ", "Enter your text here", "irc_inputbox" );
	EclRegisterButtonCallbacks ( "irc_inputbox", textbutton_draw, nullptr, button_click, button_highlight );
    EclMakeButtonEditable ( "irc_inputbox" );
        
	//
	// Post button

	EclRegisterButton ( (30 + mainWidth) - 50, mainHeight + 30, 50, 15, "Post", "Click to post your message", "irc_postbutton" );
	EclRegisterButtonCallback ( "irc_postbutton", PostClick );

}

void IRCInterface::RemoveTalkWindow()
{
	
	EclRemoveButton ( "irc_maintext" );
	EclRemoveButton ( "irc_inputbox" );
	EclRemoveButton ( "irc_postbutton" );

    ScrollBox::RemoveScrollBox( "ircscroller" );

}

bool IRCInterface::ReturnKeyPressed ()
{

    PostClick ( nullptr );
    return true;

}

void IRCInterface::Create ()
{

	if ( !IsVisible() ) {

		//
		// Title button

		int screenw = app->GetOptions ()->GetOptionValue ("graphics_screenwidth");
		int screenh = app->GetOptions ()->GetOptionValue ("graphics_screenheight");
		int paneltop = (int) ( 100.0 * ( (screenw * PANELSIZE) / 188.0 ) + 30 );
		int panelwidth = (int) ( screenw * PANELSIZE );

		LocalInterfaceScreen::Create ();

		//EclRegisterButton ( screenw - panelwidth - 3, paneltop, panelwidth, 15, "INTERNET RELAY CHAT", "Remove the IRC screen", "irc_title" );
		EclRegisterButton ( screenw - panelwidth, paneltop + 3, panelwidth - 7, 15, "INTERNET RELAY CHAT", "Remove the IRC screen", "irc_title" );

        //
        // Background blackout

		//int mainWidth = (int) ( screenw * 0.65 );
		//int mainHeight = (int) ( screenh * 0.8 );
		// Dimension to match linksscreen_interface.cpp
		int mainWidth = 50 + SY(375) + 15;
		int mainHeight = 150 + ( ( screenh - 120 - 145 ) / 15 ) * 15 + 15;
		
        EclRegisterButton ( 0, 30, mainWidth, mainHeight, " ", " ", "irc_backblackout" );
        EclRegisterButtonCallbacks ( "irc_backblackout", BackBlackoutDraw, nullptr, nullptr, nullptr );

        //
        // User list

		int boxheight = SY(300) - 3 - 20 - 20 - 3;

        //EclRegisterButton ( screenw - panelwidth - 3, paneltop + 20, panelwidth - 15, 370, "USERS", " ", "irc_userlist" );
        EclRegisterButton ( screenw - panelwidth, (paneltop + 3) + 20, (panelwidth - 7) - 20, boxheight, "USERS", " ", "irc_userlist" );
        EclRegisterButtonCallbacks ( "irc_userlist", UserListDraw, nullptr, nullptr, nullptr );

        //int numUsers = 350 / 17;
        //ScrollBox::CreateScrollBox( "irc_userscroll", 
        //                            screenw - 18, paneltop + 20, 15, 370, 
        //                            users.Size(), numUsers, 0, 
        //                            UserScrollChange );

        int numUsers = boxheight / 17;
        ScrollBox::CreateScrollBox( "irc_userscroll", 
                                    (screenw - 7) - 15, (paneltop + 3) + 20, 15, boxheight, 
                                    users.Size(), numUsers, 0, 
                                    UserScrollChange );
        
		//
		// Connect / Disconnect button

		//EclRegisterButton ( screenw - panelwidth - 3, paneltop + 400, panelwidth, 15, "CONNECT", "Click to connect/disconnect", "irc_connect" );
		EclRegisterButton ( screenw - panelwidth, (paneltop + 3) + 20 + boxheight + 5, (panelwidth - 7), 15, "CONNECT", "Click to connect/disconnect", "irc_connect" );
		EclRegisterButtonCallbacks ( "irc_connect", ConnectDraw, ConnectClick, button_click, button_highlight );


		//
		// Connect buttons

		EclRegisterButton ( 30, 150, 100, 15, "Server", "The location of the server to connect to", "irc_servertitle" );
		EclRegisterButton ( 30, 170, 100, 15, "Port", "The port to connect on", "irc_porttitle" );
		EclRegisterButton ( 30, 190, 100, 15, "Channel", "The name of the channel to join", "irc_channeltitle" );
		EclRegisterButton ( 30, 210, 100, 15, "Nickname", "Your nickname", "irc_nicknametitle" );
        EclRegisterButton ( 30, 230, 100, 15, "RealName", "Your real name", "irc_fullnametitle" );

		EclRegisterButton ( 130, 150, 300, 15, "irc.uplinkcorp.net", "Enter the Server name here", "irc_server" );
		EclRegisterButton ( 130, 170, 300, 15, "6667", "Enter the port number here", "irc_port" );
		EclRegisterButton ( 130, 190, 300, 15, "#Uplink", "Enter the channel name here", "irc_channel" );
		//EclRegisterButton ( 130, 210, 300, 15, "My Nickname", "Enter your nickname here", "irc_nickname" );
		EclRegisterButton ( 130, 210, 300, 15, game->GetWorld ()->GetPlayer ()->handle, "Enter your nickname here", "irc_nickname" );
        //EclRegisterButton ( 130, 230, 300, 15, "My Real Name", "Enter your real name here", "irc_fullname" );
        EclRegisterButton ( 130, 230, 300, 15, "Uplink Agent", "Enter your real name here", "irc_fullname" );

		EclRegisterButtonCallbacks ( "irc_server", textbutton_draw, nullptr, button_click, button_highlight );
		EclRegisterButtonCallbacks ( "irc_port", textbutton_draw, nullptr, button_click, button_highlight );
		EclRegisterButtonCallbacks ( "irc_channel", textbutton_draw, nullptr, button_click, button_highlight );
		EclRegisterButtonCallbacks ( "irc_nickname", textbutton_draw, nullptr, button_click, button_highlight );
        EclRegisterButtonCallbacks ( "irc_fullname", textbutton_draw, nullptr, button_click, button_highlight );

        EclMakeButtonEditable ( "irc_server" );
        EclMakeButtonEditable ( "irc_port" );
        EclMakeButtonEditable ( "irc_channel" );
        EclMakeButtonEditable ( "irc_nickname" );
        EclMakeButtonEditable ( "irc_fullname" );

        if ( connected ) CreateTalkWindow ();

        //
        // Smiley graphics

        if ( !imgSmileyHappy ) {   
            imgSmileyHappy = new Image ();   
            string filename = app->GetOptions ()->ThemeFilename ("irc/smileyhappy.tif");
            imgSmileyHappy->LoadTIF ( RsArchiveFileOpen ( filename ) );
        }
        if ( !imgSmileySad ) {
            imgSmileySad = new Image ();   
            string filename = app->GetOptions ()->ThemeFilename ("irc/smileysad.tif");
            imgSmileySad->LoadTIF ( RsArchiveFileOpen ( filename ) );
        }
        if ( !imgSmileyWink )
        {   
            string filename = app->GetOptions ()->ThemeFilename ("irc/smileywink.tif");
            imgSmileyWink = new Image ();   
            imgSmileyWink->LoadTIF ( RsArchiveFileOpen ( filename ) );
        }

	}

}

void IRCInterface::Remove ()
{

	if ( IsVisible() ) {

		LocalInterfaceScreen::Remove ();

		EclRemoveButton ( "irc_title" );
        EclRemoveButton ( "irc_backblackout" );
        EclRemoveButton ( "irc_connect" );

		EclRemoveButton ( "irc_servertitle" );
		EclRemoveButton ( "irc_porttitle" );
		EclRemoveButton ( "irc_channeltitle" );
		EclRemoveButton ( "irc_nicknametitle" );
        EclRemoveButton ( "irc_fullnametitle" );

		EclRemoveButton ( "irc_server" );	
		EclRemoveButton ( "irc_port" );
		EclRemoveButton ( "irc_channel" );
		EclRemoveButton ( "irc_nickname" );
        EclRemoveButton ( "irc_fullname" );

        EclRemoveButton ( "irc_userlist" );

        ScrollBox::RemoveScrollBox( "irc_userscroll" );

        RemoveTalkWindow ();

        //
        // Smileys

        if ( imgSmileyHappy )   {   delete imgSmileyHappy;      imgSmileyHappy = nullptr;  }
        if ( imgSmileySad )     {   delete imgSmileySad;        imgSmileySad = nullptr;    }
        if ( imgSmileyWink )    {   delete imgSmileyWink;       imgSmileyWink = nullptr;   }

	}
		
}

void IRCInterface::Update ()
{	
//#ifndef WIN32
    CCrossThreadsMessagingDevice::ProcessMessages ();
//#endif
}

bool IRCInterface::IsVisible ()
{
	
	return ( EclGetButton("irc_title") != nullptr );

}

int  IRCInterface::ScreenID ()
{

	return SCREEN_IRC;

}

// This is a safety macro, designed to get a parameter or " " if the param doesn't exist
#define GETIRCPARAM(n)      pmsg->parameters.size() > (n) ? (char *) pmsg->parameters[n].c_str() : (char *) " "

string UplinkIRCMonitor::GetIrcParam(const CIrcMessage *pmsg, int n) {
    if (pmsg->parameters.size() > n) {
        return pmsg->parameters[n];
    } else {
        return "";
    }
}


void UplinkIRCMonitor::OnIrcDefault( const CIrcMessage* pmsg )
{                  

    string command = pmsg->sCommand + " : " + pmsg->AsString();
	// Useless output?
    //IRCInterface::AddText( nullptr, command, 1.0, 1.0, 1.0 );

    unsigned int i = 0;
    while ( i < pmsg->parameters.size() ) {
        string thisParam = "Param" + to_string(i) + ": " + pmsg->parameters[i];
		// Useless output?
        //IRCInterface::AddText( nullptr, thisParam, COLOUR_DEFAULT );
        ++i;
    }

}


bool UplinkIRCMonitor::Received_RPL_WELCOME(const CIrcMessage* pmsg)
{

    // We can now join the channel

    string joinCommand = "join " + IRCInterface::cIrcSession->GetInfo().sNick + ": " + IRCInterface::channelName;
    (*IRCInterface::cIrcSession) << joinCommand.c_str();

    return true;

}

bool UplinkIRCMonitor::Received_PRIVMSG (const CIrcMessage* pmsg)
{
    
	if ( pmsg->m_bIncoming == 0 )
	{
		// This is a message from us
		IRCInterface::AddText(GetIrcParam(pmsg, 0), GetIrcParam(pmsg, 2), COLOUR_TEXT );
		
	}
	else
	{
		// This is a message from somebody else

        string action, version, ping, finger;

        string tmp;
        if (GetIrcParam(pmsg, 1).find( "ACTION" ) != string::npos ) {
            tmp = GetIrcParam(pmsg, 1);
            tmp = tmp.substr(tmp.find( "ACTION" ), tmp.size() );
        }
        if (GetIrcParam(pmsg, 1).find( "VERSION" ) != string::npos ) {
            tmp = GetIrcParam(pmsg, 1);
            tmp = tmp.substr(tmp.find( "VERSION" ), tmp.size() );
        }
        if (GetIrcParam(pmsg, 1).find( "PING" ) != string::npos ) {
            tmp = GetIrcParam(pmsg, 1);
            tmp = tmp.substr(tmp.find( "PING" ), tmp.size() );
        }
        if (GetIrcParam(pmsg, 1).find( "FINGER" ) != string::npos ) {
            tmp = GetIrcParam(pmsg, 1);
            tmp = tmp.substr(tmp.find( "FINGER" ), tmp.size() );
        }

        if ( !action.empty() ) {

		    string parsedMessage = pmsg->prefix.sNick + " " + action;
		    IRCInterface::AddText( "", parsedMessage, COLOUR_ACTION );
            
        }
        else if ( !version.empty() )
        {
            string reply = "NOTICE " + IRCInterface::cIrcSession->GetInfo().sNick + ": " + pmsg->prefix.sNick + " : VERSION UplinkIRC v" + app->version;
            (*IRCInterface::cIrcSession) << reply.c_str();
        }
        else if ( !ping.empty() )
        {
            string reply = "NOTICE " + IRCInterface::cIrcSession->GetInfo().sNick + ": " + pmsg->prefix.sNick + " :" + GetIrcParam(pmsg, 1);
            (*IRCInterface::cIrcSession) << reply.c_str();
        }
        else if ( !finger.empty() )
        {
            string reply = "NOTICE " + IRCInterface::cIrcSession->GetInfo().sNick + ": " + pmsg->prefix.sNick +" : FINGER " + IRCInterface::cIrcSession->GetInfo().sNick + " running UplinkIRC v" + app->version + ", www.introversion.co.uk";
            (*IRCInterface::cIrcSession) << reply.c_str();
        }
        else {

		    IRCInterface::AddText( pmsg->prefix.sNick, GetIrcParam(pmsg, 1), COLOUR_TEXT );

        }

	}

    return true;

}

bool UplinkIRCMonitor::Received_JOIN (const CIrcMessage* pmsg)
{

	if ( !pmsg->prefix.sNick.empty() ) {
		string parsedMessage1 = pmsg->prefix.sNick + " has joined this channel from " + pmsg->prefix.sHost;

		IRCInterface::AddText( "", parsedMessage1, COLOUR_JOINPART );
		IRCInterface::AddUser( pmsg->prefix.sNick );
	}

    return true;
}

bool UplinkIRCMonitor::Received_PART (const CIrcMessage* pmsg)
{

	if ( !pmsg->prefix.sNick.empty() ) {
		string parsedMessage1 = pmsg->prefix.sNick + " has left this channel";

		IRCInterface::AddText( "", parsedMessage1, COLOUR_JOINPART );
		IRCInterface::RemoveUser( pmsg->prefix.sNick );
	}

    return true;
}

bool UplinkIRCMonitor::Received_RPL_TOPIC (const CIrcMessage* pmsg)
{

    string parsedMessage = "The topic is '" + GetIrcParam(pmsg, 2) + "'";
    IRCInterface::AddText( "", parsedMessage, COLOUR_TOPIC );

    return true;

}

bool UplinkIRCMonitor::Received_TOPIC (const CIrcMessage* pmsg)
{

    string parsedMessage = pmsg->prefix.sNick + " has changed the topic to '" + GetIrcParam(pmsg, 1) + "'";
    IRCInterface::AddText( "", parsedMessage, COLOUR_TOPIC );

    return true;

}

bool UplinkIRCMonitor::Received_RPL_NAMREPLY (const CIrcMessage* pmsg)
{

	// Useless message?
    //IRCInterface::AddText( nullptr, GETIRCPARAM(3), COLOUR_JOINPART );

	char *thisParam = GETIRCPARAM ( 3 );
	int paramlen = (int) strlen( thisParam );
    char *thisParamCopy = new char [ paramlen + 1 ];
    UplinkSafeStrcpy ( thisParamCopy, thisParam )

	char *currentName = nullptr;
	for ( int i = 0; i < paramlen; i++ ) {
		if ( thisParamCopy [ i ] == ' ' ) {
			thisParamCopy [ i ] = '\0';
			if ( currentName ) {
				IRCInterface::AddUser( currentName );
				currentName = nullptr;
			}
		}
		else if ( !currentName ) {
			currentName = &(thisParamCopy [ i ]);
		}
	}
	if ( currentName ) {
		IRCInterface::AddUser( currentName );
		currentName = nullptr;
	}

	delete [] thisParamCopy;

    return true;

}

bool UplinkIRCMonitor::Received_RPL_ENDOFNAMES (const CIrcMessage* pmsg)
{

	// Useless message?
    //IRCInterface::AddText( nullptr, GETIRCPARAM(2), COLOUR_JOINPART );
    return true;

}

bool UplinkIRCMonitor::Received_MOTD (const CIrcMessage* pmsg)
{

    IRCInterface::AddText( "MOTD", GetIrcParam(pmsg, 1), COLOUR_MOTD );
    return true;

}

bool UplinkIRCMonitor::Received_RPL_LUSER (const CIrcMessage* pmsg)
{

    IRCInterface::AddText( "", GetIrcParam(pmsg, 1), COLOUR_MOTD );
    return true;

}

bool UplinkIRCMonitor::Received_MODE (const CIrcMessage* pmsg)
{

    string parsedMessage = pmsg->prefix.sNick + " sets mode to " + GetIrcParam(pmsg, 2) + " " + GetIrcParam(pmsg, 1);

    IRCInterface::AddText( "", parsedMessage, COLOUR_MODE );

    UplinkIRCUser *user = IRCInterface::GetUser( GetIrcParam(pmsg, 2) );
    if ( user ) {
        int newStatus = GetIrcParam(pmsg, 1).find( '+' ) != string::npos ? 1 : 0;
        user->status = newStatus;
    }

    return true;

}

bool UplinkIRCMonitor::Received_NICK (const CIrcMessage* pmsg)
{

	OnIrc_NICK ( pmsg );

    string parsedMessage = pmsg->prefix.sNick + " is now known as " + GetIrcParam(pmsg, 0);
    
    IRCInterface::AddText( "", parsedMessage, COLOUR_JOINPART );
    
    UplinkIRCUser *user = IRCInterface::GetUser( pmsg->prefix.sNick );
    if ( user ) user->Set( GetIrcParam(pmsg, 0) );

    // Update the screen

    EclDirtyButton ( "irc_userlist" );

    return true;

}

bool UplinkIRCMonitor::Received_QUIT (const CIrcMessage* pmsg)
{

	if ( !pmsg->prefix.sNick.empty() ) {
		string parsedMessage = pmsg->prefix.sNick + " has quit (" + GetIrcParam(pmsg, 0) + ")";
	    
		IRCInterface::AddText( "", parsedMessage, COLOUR_JOINPART );

		IRCInterface::RemoveUser( pmsg->prefix.sNick );
	}

    return true;

}

bool UplinkIRCMonitor::Received_ERROR (const CIrcMessage* pmsg)
{

	IRCInterface::AddText( "", GetIrcParam(pmsg, 0), COLOUR_JOINPART );

	IRCInterface::AddText( "", "Disconnected", COLOUR_JOINPART );

	IRCInterface::cIrcSession->Disconnect ();

    return true;

}

bool UplinkIRCMonitor::Received_ERR_BANNEDFROMCHAN (const CIrcMessage* pmsg)
{

	if( IRCInterface::cIrcSession->GetInfo().sNick == GetIrcParam(pmsg, 0) ) {

	    string parsedMessage = GetIrcParam(pmsg, 1) + " : " + GetIrcParam(pmsg, 2);

		IRCInterface::AddText( "", parsedMessage, COLOUR_JOINPART );

	}

    return true;

}

UplinkIRCMonitor::UplinkIRCMonitor( CIrcSession &session )
    : CIrcDefaultMonitor( session )
{   

	// See RFC 1459 for all the codes. ( http://tools.ietf.org/html/rfc1459 )

	IRC_MAP_ENTRY(UplinkIRCMonitor, "001", Received_RPL_WELCOME )
    //IRC_MAP_ENTRY(UplinkIRCMonitor, "002", Received_RPL_LUSER )
    IRC_MAP_ENTRY(UplinkIRCMonitor, "003", Received_RPL_LUSER )
    IRC_MAP_ENTRY(UplinkIRCMonitor, "250", Received_RPL_LUSER )
    IRC_MAP_ENTRY(UplinkIRCMonitor, "251", Received_RPL_LUSER )
    IRC_MAP_ENTRY(UplinkIRCMonitor, "252", Received_RPL_LUSER )
    IRC_MAP_ENTRY(UplinkIRCMonitor, "253", Received_RPL_LUSER )
    IRC_MAP_ENTRY(UplinkIRCMonitor, "254", Received_RPL_LUSER )
    IRC_MAP_ENTRY(UplinkIRCMonitor, "255", Received_RPL_LUSER )
    IRC_MAP_ENTRY(UplinkIRCMonitor, "265", Received_RPL_LUSER )
    IRC_MAP_ENTRY(UplinkIRCMonitor, "266", Received_RPL_LUSER )
    IRC_MAP_ENTRY(UplinkIRCMonitor, "332", Received_RPL_TOPIC )
    IRC_MAP_ENTRY(UplinkIRCMonitor, "331", Received_RPL_TOPIC )
    IRC_MAP_ENTRY(UplinkIRCMonitor, "353", Received_RPL_NAMREPLY )
    IRC_MAP_ENTRY(UplinkIRCMonitor, "366", Received_RPL_ENDOFNAMES )
    IRC_MAP_ENTRY(UplinkIRCMonitor, "372", Received_MOTD )
    IRC_MAP_ENTRY(UplinkIRCMonitor, "375", Received_MOTD )
    IRC_MAP_ENTRY(UplinkIRCMonitor, "376", Received_MOTD )

    IRC_MAP_ENTRY(UplinkIRCMonitor, "PRIVMSG", Received_PRIVMSG )
    IRC_MAP_ENTRY(UplinkIRCMonitor, "NOTICE", Received_RPL_LUSER )
	IRC_MAP_ENTRY(UplinkIRCMonitor, "JOIN", Received_JOIN )
	IRC_MAP_ENTRY(UplinkIRCMonitor, "PART", Received_PART )
    IRC_MAP_ENTRY(UplinkIRCMonitor, "TOPIC", Received_TOPIC ) 
    IRC_MAP_ENTRY(UplinkIRCMonitor, "MODE", Received_MODE ) 
    IRC_MAP_ENTRY(UplinkIRCMonitor, "NICK", Received_NICK )
    IRC_MAP_ENTRY(UplinkIRCMonitor, "QUIT", Received_QUIT )

    IRC_MAP_ENTRY(UplinkIRCMonitor, "ERROR", Received_ERROR )
    IRC_MAP_ENTRY(UplinkIRCMonitor, "474", Received_ERR_BANNEDFROMCHAN )

    // TODO : Kick
    //      : Query
    
}


DECLARE_IRC_MAP(UplinkIRCMonitor, CIrcDefaultMonitor)

