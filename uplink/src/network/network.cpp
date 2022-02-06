// Network.cpp: implementation of the Network class.
//
//////////////////////////////////////////////////////////////////////

#include <tcp4u.h>
#include <smtp4u.h>
#include <app/miscutils.h>

#include "gucci.h"

#include "app/app.h"
#include "app/serialise.h"
#include "app/globals.h"

#include "network/network.h"

#include "mmgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Network::Network()
{

	// Initialise the TCP library

		
	STATUS = NETWORK_NONE;

	int initresult = Tcp4uInit ();

	if ( initresult != TCP4U_SUCCESS ) {

		cout << "Network error : failed to initialise TCP" << endl;
		return;

	}


#ifdef DOCLABRELEASE

	// 
	// Prepare a message to send back to me
	//

	char from		[] = "uplink@doc.ic.ac.uk";
	char to			[] = "uplink@introversion.co.uk"; 
	char smtpserver [] = "hex.doc.ic.ac.uk";
	char domain		[] = "doc.ic.ac.uk";
	char message	[128];

	char currentuser [32];
	char computer	 [32];

#ifdef WIN32
	UplinkStrncpy ( currentuser, getenv ( "USERNAME" ), sizeof ( currentuser ) );
	UplinkStrncpy ( computer, getenv ( "COMPUTERNAME" ), sizeof ( computer ) );
#else
	UplinkStrncpy ( currentuser, getenv ( "USER" ), sizeof ( currentuser ) );
	UplinkStrncpy ( computer, getenv ( "HOST" ), sizeof ( computer ) );
#endif

	// 
	// Build and send the message
	//

	UplinkSnprintf ( message, sizeof ( message ), "To:%s\r\n"
												   "Subject: User %s used Doc release of Uplink\r\n\r\n"
												   "Username : %s\r\n"
												   "Computer : %s\r\n", 
													to, currentuser, currentuser, computer );

	int result = SmtpSendMessage ( from, to, message, smtpserver, domain );

#endif

}

Network::~Network()
{

	Tcp4uCleanup ();

}

NetworkServer *Network::GetServer ()
{

	UplinkAssert ( STATUS == NETWORK_SERVER )
	return &server;

}

NetworkClient *Network::GetClient ()
{

	UplinkAssert ( STATUS == NETWORK_CLIENT )
	return &client;

}

void Network::SetStatus ( int newSTATUS )
{

	STATUS = newSTATUS;

}

char *Network::GetLocalHost ()
{

	DWORD ip;
	char *host = new char [128];
	int result = TcpGetLocalID ( host, 128, &ip );
			     
	if ( result == TCP4U_SUCCESS ) 
		return host;

	else {
		cout << "Network::GetLocalHost, failed to get local host" << endl;
		return nullptr;
	}

}

char *Network::GetLocalIP ()
{

	DWORD ip;
	char *sip = (char *) &ip;
	char *host = new char [128];
	int result = TcpGetLocalID ( host, 128, &ip );

	size_t fullipsize = 64;
	char *fullip = new char [fullipsize];
	UplinkSnprintf ( fullip, fullipsize, "%u.%u.%u.%u", sip [0], sip [1], sip [2], sip [3] )

	delete [] host;

	if ( result == TCP4U_SUCCESS ) 
		return fullip;

	else {
		cout << "Network::GetLocalIP, failed to get local ip" << endl;
		return nullptr;
	}

}

void Network::StartServer ()
{

	if ( STATUS == NETWORK_NONE ) {

		int result = server.StartServer ();

		if ( result )
			STATUS = NETWORK_SERVER;

		else
			cout << "Network::StartServer, failed to start server" << endl;

	}
	else if ( STATUS == NETWORK_CLIENT ) {

		cout << "Network::StartServer, Cannot start server when running as a client" << endl;

	}
	else if ( STATUS == NETWORK_SERVER ) {

		cout << "Network::StartServer, Cannot start server when server is already running" << endl;
	
	}

}

void Network::StopServer ()
{

	if ( STATUS == NETWORK_SERVER ) {

		server.StopServer ();
		STATUS = NETWORK_NONE;

	}

}

void Network::StartClient ( char *ip )
{

	if ( STATUS == NETWORK_NONE ) {

		int result = client.StartClient (ip);

		if ( result )
			STATUS = NETWORK_CLIENT;

		else
			cout << "Network::StartClient, failed to start client" << endl;

	}
	else if ( STATUS == NETWORK_CLIENT ) {

		cout << "Network::StartClient, Cannot start client when running as a client" << endl;

	}
	else if ( STATUS == NETWORK_SERVER ) {

		cout << "Network::StartClient, Cannot start client when server is already running" << endl;
	
	}

}

void Network::StopClient ()
{

	if ( STATUS == NETWORK_CLIENT ) {

		int result = GetClient ()->StopClient ();

		if ( !result )
			cout << "Network::StopClient, failed to stop client" << endl;

		else
			STATUS = NETWORK_NONE;

	}

}

bool Network::IsActive ()
{

	return ( STATUS != NETWORK_NONE );

}

bool Network::Load ( FILE *file )
{
	// not needed
	return true;
}

void Network::Save ( FILE *file )
{

	// not needed

}

void Network::Print ()
{

	cout << "============== N E T W O R K ===============================" << endl;

	PrintValue("Status", STATUS);

	if ( STATUS == NETWORK_SERVER )
		GetServer ()->Print ();

	else if ( STATUS == NETWORK_CLIENT ) 
		GetClient ()->Print ();

	cout << "============== E N D  O F  N E T W O R K ===================" << endl;

}

void Network::Update ()
{

	if ( STATUS == NETWORK_SERVER ) {

		GetServer ()->Update ();

	}
	else if ( STATUS == NETWORK_CLIENT ) {
		
		GetClient ()->Update ();

	}

}

string Network::GetID ()
{

	return "NETWORK";

}
