
/*
	World class object
	
		Stores a database of information representing
		the game world, including companies, people, 
		computers, missions etc.

  */


#ifndef included_world_h
#define included_world_h

// ============================================================================

#include <cstdio>

#include "tosser.h"

#include "app/uplinkobject.h"
#include "world/date.h"
#include "world/scheduler/eventscheduler.h"
#include "world/generator/plotgenerator.h"
#include "world/generator/demoplotgenerator.h"

class VLocation;
class Company;
class Computer;
class Person;
class Mission;
class Player;
class GatewayDef;

// ============================================================================



class World : public UplinkObject
{

protected:

	Date nextupdate;

public:

	Date date;
	EventScheduler scheduler;
    PlotGenerator plotgenerator;
    DemoPlotGenerator demoplotgenerator;

	BTree <VLocation *>  locations;
	BTree <Company *>	 companies;
	BTree <Computer *>	 computers;
	BTree <Person *>	 people;	 

	DArray <string >		  passwords;					// No need to serialise
	DArray <GatewayDef *> gatewaydefs;					// No need to serialise

public:

	World();
	~World() override;

	// Database management functions

	VLocation *CreateVLocation (const string &ip, int phys_x, int phys_y );
	static bool       VerifyVLocation (const string &ip, int phys_x, int phys_y );
	Company   *CreateCompany   (const string &name );
	Computer  *CreateComputer  (const string &name, const string &companyname, const string &ip );
	Person    *CreatePerson	   (const string &name, const string &localhost );
	void	   CreatePassword  (const string &password );
		
	void CreateVLocation  ( VLocation  *vlocation );	
	void CreateCompany    ( Company    *company );
	void CreateComputer   ( Computer   *computer );
	void CreatePerson     ( Person     *person );	
	void CreateGatewayDef ( GatewayDef *newdef );
	
	VLocation  *GetVLocation  (const string &ip );				//  These all return NULL
	Company    *GetCompany	  (const string &name );			//  if the specified object
	Computer   *GetComputer   (const string &name );			//  is not found in the database
	Person     *GetPerson     (const string &name );			//  (check before dereferencing)
	string GetPassword   (int index ) const;
	GatewayDef *GetGatewayDef (const string &name ) const;

	Player    *GetPlayer     ();						//  Asserts that player exists

	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	void Update () override;
	
	string GetID () override;

};


#endif 
