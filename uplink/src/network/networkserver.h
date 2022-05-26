// NetworkServer.h: interface for the NetworkServer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef included_networkserver_h
#define included_networkserver_h

// ============================================================================

#include <tcp4u.h>

#include <ctime>

#include "tosser.h"

#include "app/uplinkobject.h"

class ClientConnection;

// ============================================================================

class NetworkServer : public UplinkObject  
{

protected:

	SOCKET listensocket;							
	time_t lastlisten;	

	bool listening;

public:

	DArray <ClientConnection *> clients;	

public:

	NetworkServer();
	~NetworkServer() override;

	bool StartServer ();
	void StopServer ();

	void Listen ();
	void StopListening ();

	char *GetRemoteHost ( int socketindex ) const;
	char *GetRemoteIP ( int socketindex ) const;

	// Common functions

	bool Load   ( FILE *file ) override;
	void Save   ( FILE *file ) override;
	void Print  () override;
	void Update () override;
	string GetID () override;

};

#endif 
