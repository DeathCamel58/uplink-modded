
#ifndef _included_namegenerator_h
#define _included_namegenerator_h

#define MAX_COMPUTERNAME 128
#define MAX_PERSONNAME   128

class NameGenerator
{

protected:

	static LList <char *> fornames;
	static LList <char *> surnames;
	static LList <char *> agentaliases;
	static LList <char *> companynamesA;
	static LList <char *> companynamesB;

	static void LoadNames ();

public:

	static void Initialise ();
    static void Shutdown ();

	static char *GenerateCompanyName ();
	static string GeneratePersonName ();
	static char *GenerateAgentAlias ();
	static char *GenerateBankName ();
		
	static char *GeneratePassword ();
	static string GenerateEasyPassword ();					// Guessable
	static char *GenerateComplexPassword ();				// Non dictionary hackable
	
	static char *GenerateDataName (const string &companyname, int DATATYPE );

    static char *GeneratePublicAccessServerName     (const string &companyname );
    static char *GenerateAccessTerminalName         (const string &companyname );
    static char *GenerateInternalServicesServerName (const string &companyname );
    static char *GenerateCentralMainframeName       (const string &companyname );
    static char *GenerateInternationalBankName      (const string &companyname );
    static char *GenerateFileServerName             (const string &companyname );
    static char *GenerateLANName                    (const string &companyname );
    static char *GeneratePersonalComputerName       (const string &personname );
    static char *GenerateVoicePhoneSystemName       (const string &personname );
};

#endif

