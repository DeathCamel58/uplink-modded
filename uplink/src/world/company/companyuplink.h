
#ifndef included_companyuplink_h
#define included_companyuplink_h

// ============================================================================

#include "world/company/company.h"

class Mission;
class Sale;
class News;

// ============================================================================

//! Modified Company class for Uplink Company
/**
 * This is a specific version of the company class, modified to represent the company Uplink only.
 */
class CompanyUplink : public Company  
{

public:

    /**
     * LList of available Mission items
     */
	LList <Mission *> missions;

	/**
	 * LList of available hardware Sale items
	 */
	LList <Sale *>	  hw_sales;

	/**
	 * LList of available software Sale items
	 */
	LList <Sale *>	  sw_sales;

	/**
	 * LList of News items
	 */
	LList <News *>	  news;

public:

	CompanyUplink();
	~CompanyUplink() override;

	/**
	 * Adds given Mission to the list of available
	 * @param mission Mission to add
	 */
	void CreateMission ( Mission *mission );

	/**
	 * Adds Mission to the list of available
	 * @param employer Employer for the Mission
	 * @param payment Payment for the Mission
	 * @param description Description of the Mission
	 * @param details Details for the Mission
	 * @param fulldetails Full details for the Mission
	 */
	void CreateMission (const string& employer, int payment, const string& description,
                        const string& details, const string& fulldetails );

    /**
     * Adds given hardware Sale item to the list of available
     * @param mission Hardware Sale item to add
     */
	void CreateHWSale ( Sale *newsale );

    /**
     * Adds given software Sale item to the list of available
     * @param mission Software Sale item to add
     */
	void CreateSWSale ( Sale *newsale );

    /**
     * Adds given News item to the list all News
     * @param mission News item to add
     */
	void CreateNews   ( News *newnews );

    /**
     * Gets the Mission at a given index
     * @param index Index of the Mission
     * @return Mission
     */
	Mission   *GetMission  ( int index );

    /**
     * Gets the software Sale item at a given index
     * @param index Index of the software Sale item
     * @return Software Sale item
     */
	Sale	  *GetSWSale   ( int index );

	/**
	 * Gets the hardware Sale item at a given index
	 * @param index Index of the hardware Sale item
	 * @return Hardware Sale item
	 */
	Sale	  *GetHWSale   ( int index );

	/**
	 * Gets the News item at a given index
	 * @param index Index of the News item
	 * @return The News item
	 */
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
