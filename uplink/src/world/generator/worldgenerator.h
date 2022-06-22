
#ifndef included_worldgenerator_h
#define included_worldgenerator_h

// ============================================================================

#include "gucci.h"
#include "tosser.h"

class VLocation;
class Company;
class Computer;
class Person;
class Agent;
class Player;
class Mission;
class Sale;
class Button;

// ============================================================================


//! Interface for the WorldGenerator class
class WorldGenerator  
{

protected:

    /**
     * Used to determine legal computer positions
     */
	static Image *worldmapmask;
	
public:

    /**
     * Sets up the worldmapmask
     */
	static void Initialise ();

	/**
	 * Clears up memory used
	 */
    static void Shutdown();
    
	/** @name Generate General
	 * High level functions that generate large amounts of data
	 */
	 ///@{

	/**
	 * The main entry function which generates the entire world
	 * @note Called in a new game
	 */
	static void GenerateAll ();

	/**
	 * Generates some Company, bank, Person, Agent, Mission, and employment data items
	 */
	static void GenerateRandomWorld ();

	/**
	 * Creates the Player Company, and some specific Company and Computer items
     * @note This only generates OCP, SJGames, and Introversion when the build is not a DEMOGAME
	 */
	static void GenerateSpecifics ();

	/**
	 * Loads all LANs from files
	 */
    static void LoadDynamics ();

    /**
     * Loads all Gateway items from files
     */
    static void LoadDynamicsGatewayDefs ();        // Loads GatewayDefs

    ///@}

	/** @name Generate specific
	 * Middle level functions for generating specific items of data
	 */
	 ///@{

	 /**
	  * Generates the Player
	  * @param handle The handle of the Player
	  * @return The Player object
	  * @note This populates the Player's starting links, creates a player bank account, sets ratings, etc
	  */
	static Player *GeneratePlayer (const string &handle );

	/**
	 * Create a local machine for players to fall back to Which can also act as a LinksScreen
	 */
	static void	   GenerateLocalMachine ();

	/**
	 * Generates the Uplink Company
	 * @note Creates the Bank server, generates all hardware and software items for sale
	 */
	static void GenerateCompanyUplink ();

	/**
	 * Generates the Uplink Public Access Server
	 * @note This generates the Computer and constructs all MessageScreen items
	 */
	static void GenerateUplinkPublicAccessServer ();

	/**
	 * Generates the Uplink Internal Services System
	 * @note This generates the Computer and constructs all MessageScreen items
	 */
	static void GenerateUplinkInternalServicesSystem ();

	/**
	 * Generates the Uplink Test Machine
	 * @note This generates the Computer and constructs all MessageScreen items
	 */
	static void GenerateUplinkTestMachine ();

	/**
	 * Generates the Uplink Credits Machine
	 * @note This generates the Computer and constructs all MessageScreen items
	 */
	static void GenerateUplinkCreditsMachine ();

	/**
	 * Generates the Government
	 * @note This generates the Company "Government"
	 */
	static void GenerateCompanyGovernment ();

	/**
	 * Generates the Global Criminal Database
	 * @note This generates the Computer and constructs all MessageScreen items
	 */
	static void GenerateGlobalCriminalDatabase ();

	/**
	 * Generates the International Social Security Database
	 * @note This generates the Computer and constructs all MessageScreen items
	 */
	static void GenerateInternationalSocialSecurityDatabase ();

	/**
	 * Generates the Central Medical Database
	 * @note This generates the Computer and constructs all MessageScreen items
	 * @note This is unused
	 */
	static void GenerateCentralMedicalDatabase ();

	/**
	 * Does nothing
	 */
	static void GenerateGlobalIntelligenceAgency ();

	/**
	 * Generates the International Academic Database
	 * @note This generates the Computer and constructs all MessageScreen items
	 */
	static void GenerateInternationalAcademicDatabase ();

	/**
	 * Generates InterNIC
	 * @note This generates the Computer and constructs all MessageScreen items
	 */
	static void GenerateInterNIC ();

	/**
	 * Generates the Stock Market
	 * @note This generates the Computer and constructs all MessageScreen items
	 */
	static void GenerateStockMarket ();

	/**
	 * Generates the Protovision server
	 * @note This generates the Computer and constructs all MessageScreen items
	 */
    static void GenerateProtoVision ();

    /**
     * Generates the OCP server
	 * @note This generates the Computer and constructs all MessageScreen items
     */
    static void GenerateOCP ();

    /**
     * Generates the Steve Jackson Games server
	 * @note This generates the Computer and constructs all MessageScreen items
     */
    static void GenerateSJGames ();

    /**
     * Generates the Introversion server
	 * @note This generates the Computer and constructs all MessageScreen items
     */
    static void GenerateIntroversion ();

    /**
     * Generates two very easy Mission items for the player at the start
     */
	static void GenerateSimpleStartingMissionA ();

	/**
	 * Generate a delete file Mission
	 */
	static void GenerateSimpleStartingMissionB ();

	///@}

	/** @name Generate Random
	 * Low level functions for generating random items of data
	 * They all add the data into the World object automatically, and then return it
	 */
	 ///@{

	 /**
	  * Generates a random VLocation
	  * @return The VLocation
	  */
	static VLocation *GenerateLocation ();

	/**
	 * Generates a random Company
	 * @return the Company
	 */
	static Company   *GenerateCompany ();

	/**
	 * Generates a random bank
	 * @return The Bank's Company
	 */
	static Company   *GenerateCompany_Bank ();

	/**
	 * Generates a random Computer for a Company
	 * @param companyname Name of Company to generate Computer for
	 * @return Computer that was generated
	 */
	static Computer  *GenerateComputer (const string &companyname );

	/**
	 * Generates a random Person
	 * @return The Person
	 */
	static Person    *GeneratePerson ();

	/**
	 * Generates a random Agent
	 * @return The Agent
	 */
	static Agent	 *GenerateAgent ();

	/**
	 * Generates a specific type of Computer for a Company
	 * @param companyName Name of the Company to generate the Computer for
	 * @param TYPE The type of the Computer
	 * @return The Computer
	 */
    static Computer  *GenerateComputer                (const string &companyName, int TYPE );

    /**
     * Generates a public access server for Company
     * @param companyname Name of the Company to generate the public access server for
     * @return The Computer for the public access server
     */
	static Computer  *GeneratePublicAccessServer	  (const string &companyname );

	/**
	 * Generates an internal services machine for a company
	 * @param companyname Name of the Company to generate the internal services machine for
	 * @return The Computer for the internal services machine
	 */
	static Computer  *GenerateInternalServicesMachine (const string &companyname );

	/**
	 * Generates a Central Mainframe for a Company
	 * @param companyname Name of the Company to generate Central Mainframe for
	 * @return The Computer for the Central Mainframe
	 */
	static Computer  *GenerateCentralMainframe        (const string &companyname );

	/**
	 * Generates a Public Bank Server for a Company
	 * @param companyname Name of the Company to generate Public Bank Server for
	 * @return The Computer for the Public Bank Server
	 */
	static Computer  *GeneratePublicBankServer        (const string &companyname );

	/**
	 * Generates an empty file server for a Company
	 * @param companyname Name of Company to generate empty file server for
	 * @return The Computer for the File Server
	 * @note Used in StealResearch missions
	 */
	static Computer	 *GenerateEmptyFileServer		  (const string &companyname );

	/**
	 * Generates LAN for a Company
	 * @param companyname Name of the Company to generate the LAN for
	 * @return The LAN of the Company
	 * @note Returns nullptr if companyname is empty
	 */
    static Computer  *GenerateLAN                     (const string &companyname );

    /**
     * Generates a personal Computer
     * @param personname Name of the Person to generate the Computer for
     * @return The personal Computer
     */
	static Computer  *GeneratePersonalComputer		  (const string &personname );

	/**
	 * Generates a voice phone system
	 * @param personname Name of the Person to generate the voice phone system for
	 * @return The voice phone system Computer
	 */
	static Computer  *GenerateVoicePhoneSystem		  (const string &personname );

	///@}

	// Low level functions for "fleshing out" specific items of data

	/**
	 * TODO: Document
	 * @param companyname
	 * @param size
	 * @param TYPE
	 * @param growth
	 * @param alignment
	 * @return
	 */
	static Company   *GenerateCompany (const string &companyname, int size, int TYPE, int growth, int alignment );

	/** @name Get Random
	 * Functions which return a random entry
	 */
	///@{

	/**
	 * Gets a random VLocation
	 * @return The random VLocation
	 */
	static VLocation *GetRandomLocation ();

	/**
	 * Gets a random Company
	 * @return The random Company
	 */
	static Company   *GetRandomCompany  ();

	/**
	 * Gets a random Computer
	 * @return The random Computer
	 */
	static Computer  *GetRandomComputer ();

	/**
	 * Gets a random Computer of a specific type
	 * @param TYPE The bitfield of acceptable computer types
	 * @return The random Computer
	 */
	static Computer  *GetRandomComputer ( int TYPE );				// TYPE is a bitfield

	/**
	 * Gets a random Person
	 * @return The random Person
	 */
	static Person	 *GetRandomPerson   ();

	/**
	 * Gets a random Agent
	 * @return The random Agent
	 */
	static Agent	 *GetRandomAgent	();

	/**
	 * Gets a random Mission
	 * @return The random Mission
	 */
	static Mission   *GetRandomMission  ();

	/**
	 * Gets a random, low security Computer
	 * @param TYPE The type of the Computer
	 * @return The low security Computer
	 * @note Computers gotten through this function will not have a proxy or firewall
	 */
	static Computer  *GetRandomLowSecurityComputer ( int TYPE );
	
	///@}

	/**
	 * @name Misc
	 * Miscellaneous Functions
	 */
	 ///@{

	 /**
	  * Generates map co-ordinates inland
	  * @param x TODO: Document
	  * @param y TODO: Document
	  */
	static void GenerateValidMapPos ( int &x, int &y );

	/**
	 * For each Company that the Person is currently an administrator, replace him with a new administrator
	 * @param person The Person to replace in all Company items
	 */
	static void ReplaceAdminCompanies ( Person *person );

	/**
	 * Replace all Company administrators that are dead or in jail
	 */
	static void ReplaceInvalidCompanyAdmins ();

	/**
	 * TODO: Document
	 */
	static void UpdateSoftwareUpgrades ();

	// Not needed anymore since the Gateway store the GatewayDef
	//static bool VerifyPlayerGateway ();
	//static void VerifyPlayerGatewayCloseDialog ( Button *button );

	///@}

};

#endif
