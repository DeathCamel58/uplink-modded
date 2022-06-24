
#ifndef included_namegenerator_h
#define included_namegenerator_h

#define MAX_COMPUTERNAME 128
#define MAX_PERSONNAME   128

/**
 * Generator for random names
 */
class NameGenerator
{

protected:

    /**
     * LList of all possible forenames
     */
	static LList <string> fornames;

    /**
     * LList of all possible surnames
     */
	static LList <string> surnames;

    /**
     * LList of all possible Agent aliases
     */
	static LList <string> agentaliases;

    /**
     * LList of all possible starts of company names
     */
	static LList <string> companynamesA;

    /**
     * LList of all possible endings of company names
     */
	static LList <string> companynamesB;

    /**
     * Loads all forenames, surnames, agent aliases, companynameA, and companynameB entries into LLists
     */
	static void LoadNames ();

public:

	static void Initialise ();
    static void Shutdown ();

    /**
     * Gets a random companynameA and companynameB
     * @return A random companynameA and companynameB in format "<companynameA> <companynameB>"
     */
	static string GenerateCompanyName ();
    /**
     * Gets a random forname and surname
     * @return A random forname and surname in format "<forname> <surname>"
     */
	static string GeneratePersonName ();
    /**
     * Gets a random agent alias from the list of possible aliases
     * @return A random agent alias
     */
	static string GenerateAgentAlias ();

    /**
     * Gets a password from the list of possible ones
     * @return The password
     */
	static string GeneratePassword ();

	/**
	 * Gets an easy password
	 * @return The easy password
	 * @note The easy passwords can be "god", "admin", "password", "love", or "secure"
	 */
	static string GenerateEasyPassword ();

    /**
     * Gets a normal password with up to half of the letters changed
     * @return The complex password
     */
	static string GenerateComplexPassword ();

	/**
	 * Gets a file name
	 * @param companyname Name of the company to generate file name for
	 * @param DATATYPE The type of data to generate
	 * @return The name of the file
	 * @note This is in the format "<First three letters of company name>-<file|data|prog>-<Random number up to 99999>"
	 */
	static string GenerateDataName (const string &companyname, int DATATYPE );

    /**
     * Gets public access server name of given company
     * @param companyname Company to get public access server name for
     * @return The phone system name in the format "<companyname> Public Access Server"
     */
    static string GeneratePublicAccessServerName     (const string &companyname );

    /**
     * Gets access terminal name of given company
     * @param companyname Company to get access terminal name for
     * @return The phone system name in the format "<companyname> Access Terminal"
     */
    static string GenerateAccessTerminalName         (const string &companyname );

    /**
     * Gets internal services server name of given company
     * @param companyname Company to get internal services server name for
     * @return The phone system name in the format "<companyname> Internal Services Machine"
     */
    static string GenerateInternalServicesServerName (const string &companyname );

    /**
     * Gets central mainframe name of given company
     * @param companyname Company to get central mainframe name for
     * @return The phone system name in the format "<companyname> Central Mainframe"
     */
    static string GenerateCentralMainframeName       (const string &companyname );

    /**
     * Gets international bank name of given company
     * @param companyname Company to get international bank name for
     * @return The phone system name in the format "<companyname> International Bank"
     */
    static string GenerateInternationalBankName      (const string &companyname );

    /**
     * Gets file server name of given company
     * @param companyname Company to get file server name for
     * @return The phone system name in the format "<companyname> File server"
     */
    static string GenerateFileServerName             (const string &companyname );

    /**
     * Gets LAN name of given company
     * @param companyname Company to get LAN name for
     * @return The phone system name in the format "<companyname> Local Area Network"
     */
    static string GenerateLANName                    (const string &companyname );

    /**
     * Gets personal computer name of given person
     * @param personname Person to get personal computer name for
     * @return The phone system name in the format "<personname> Personal Computer"
     */
    static string GeneratePersonalComputerName       (const string &personname );

    /**
     * Gets voice phone system name of given person
     * @param personname Person to get voice phone system name for
     * @return The phone system name in the format "<personname> Voice Phone System"
     */
    static string GenerateVoicePhoneSystemName       (const string &personname );
};

#endif

