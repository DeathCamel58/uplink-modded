// NetworkServer.cpp: implementation of the NetworkServer class.
//
//////////////////////////////////////////////////////////////////////

#include <app/miscutils.h>
#include "eclipse.h"

#include "app/app.h"
#include "app/serialise.h"
#include "app/globals.h"

#include "network/networkserver.h"
#include "network/networkclient.h"
#include "network/clientconnection.h"

#include "mmgr.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NetworkServer::NetworkServer()
{
	listensocket = -1;
	listening = false;
}

NetworkServer::~NetworkServer()
{

    DeleteDArrayData ( (DArray <UplinkObject *> *) &clients );

}

bool NetworkServer::StartServer ()
{

	// Establish a listen socket

	unsigned short portnum = 31337;
	int result = TcpGetListenSocket ( &listensocket, nullptr, &portnum, 5 );
	lastlisten = time (nullptr);

	if ( result != TCP4U_SUCCESS )
		return false;
	
	else 
		return true;

}

void NetworkServer::StopServer ()
{

	// Close all open sockets

	for ( int i = 0; i < clients.Size (); ++i ) {
		if ( clients.ValidIndex (i) ) {

			int result = TcpClose ( &(clients.GetData (i)->socket) );

			if ( result != TCP4U_SUCCESS ) 
				cout << "NetworkServer::StopServer, failed to close connection " << i << endl;

		}
	}

	DeleteDArrayData ( (DArray <UplinkObject *> *) &clients );
	clients.Empty ();

}

char *NetworkServer::GetRemoteHost ( int socketindex ) const
{

	UplinkAssert ( clients.ValidIndex (socketindex) )

	DWORD ip;
	char *host = new char [128];
	int result = TcpGetRemoteID ( clients.GetData (socketindex)->socket, host, 128, &ip );
			     
	if ( result == TCP4U_SUCCESS ) 
		return host;

	else {
		cout << "NetworkServer::GetRemoteHost, failed to get remote host" << endl;
		return nullptr;
	}

}

char *NetworkServer::GetRemoteIP ( int socketindex ) const
{

	UplinkAssert ( clients.ValidIndex (socketindex) )

	DWORD ip;
	char *sip = (char *) &ip;
	char *host = new char [128];
	int result = TcpGetRemoteID ( clients.GetData (socketindex)->socket, host, 128, &ip );

	size_t fullipsize = 64;
	char *fullip = new char [fullipsize];
	UplinkSnprintf ( fullip, fullipsize, "%u.%u.%u.%u", sip [0], sip [1], sip [2], sip [3] )

	delete [] host;

	if ( result == TCP4U_SUCCESS ) 
		return fullip;

	else {
		cout << "NetworkServer::GetRemoteIP, failed to get remote ip" << endl;
		return nullptr;
	}

}

void NetworkServer::Listen ()
{

	listening = true;

}

void NetworkServer::StopListening ()
{

	listening = false;

}

bool NetworkServer::Load ( FILE *file )
{
	// not needed
	return true;
}

void NetworkServer::Save ( FILE *file )
{
	// not needed
}

void NetworkServer::Print ()
{

    cout << "NetworkServer :" << endl;
    PrintValue("ListenSocket", (int) listensocket);
    PrintValue("LastListen", lastlisten);

	PrintDArray ( (DArray <UplinkObject *> *) &clients );

}

void NetworkServer::Update ()
{

	// Check the listen socket for new connections every 5 seconds

	if ( listening && listensocket != -1 ) {

		if ( time (nullptr) > lastlisten + 5 ) {

			SOCKET thesocket;
			int result = TcpAccept ( &thesocket, listensocket, 1 );

			if ( result == TCP4U_SUCCESS ) {

				// New connection requested - deal with it
				
				auto *cc = new ClientConnection ();
				cc->SetSocket          ( thesocket );
				cc->SetClientType      ( CLIENT_NONE );
				cc->SetConnectionTime  ( (int) EclGetAccurateTime () );

				int index = clients.PutData ( cc );
				cc->SetIndex ( index );

			}

			lastlisten = time (nullptr);

		}

	}

	// Read any input from the open sockets
	// And send out the neccisary data

	UpdateDArray ( (DArray <UplinkObject *> *) &clients );

}

string NetworkServer::GetID ()
{

	return "SERVER";

}
