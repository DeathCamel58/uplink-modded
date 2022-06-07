
#ifndef included_namegenerator_h
#define included_namegenerator_h

#define MAX_COMPUTERNAME 128
#define MAX_PERSONNAME   128

class NameGenerator
{

protected:

	static LList <string> fornames;
	static LList <string> surnames;
	static LList <string> agentaliases;
	static LList <string> companynamesA;
	static LList <string> companynamesB;

	static void LoadNames ();

public:

	static void Initialise ();
    static void Shutdown ();

	static string GenerateCompanyName ();
	static string GeneratePersonName ();
	static string GenerateAgentAlias ();
		
	static string GeneratePassword ();
	static string GenerateEasyPassword ();					// Guessable
	static string GenerateComplexPassword ();				// Non dictionary hackable
	
	static string GenerateDataName (const string &companyname, int DATATYPE );

    static string GeneratePublicAccessServerName     (const string &companyname );
    static string GenerateAccessTerminalName         (const string &companyname );
    static string GenerateInternalServicesServerName (const string &companyname );
    static string GenerateCentralMainframeName       (const string &companyname );
    static string GenerateInternationalBankName      (const string &companyname );
    static string GenerateFileServerName             (const string &companyname );
    static string GenerateLANName                    (const string &companyname );
    static string GeneratePersonalComputerName       (const string &personname );
    static string GenerateVoicePhoneSystemName       (const string &personname );
};

#endif

