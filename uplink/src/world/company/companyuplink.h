
/*

  This is a specific version of the company class, 
  Modified to represent the company Uplink only.
	
  */

#ifndef included_companyuplink_h
#define included_companyuplink_h

// ============================================================================

#include "world/company/company.h"

class Mission;
class Sale;
class News;

// ============================================================================



class CompanyUplink : public Company  
{

public:

	LList <Mission *> missions;
	LList <Sale *>	  hw_sales;				// Hardware
	LList <Sale *>	  sw_sales;				// Software
	LList <News *>	  news;

public:

	CompanyUplink();
	~CompanyUplink() override;

	void CreateMission ( Mission *mission );							
	void CreateMission (const string& employer, int payment, const string& description,
                        const string& details, const string& fulldetails );

	void CreateHWSale ( Sale *newsale );
	void CreateSWSale ( Sale *newsale );
	void CreateNews   ( News *newnews );

	Mission   *GetMission  ( int index );
	Sale	  *GetSWSale   ( int index );
	Sale	  *GetHWSale   ( int index );
	News	  *GetNews	   ( int index );

	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	void Update () override;
	
	string GetID () override;
	int   GetOBJECTID () override;

};

#endif 
