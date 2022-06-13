
#ifndef included_newsgenerator_h
#define included_newsgenerator_h

// ============================================================================

class Computer;
class AccessLog;
class Person;

// ============================================================================


//! News Generator
/**
 * Receives information on important events that have occured (e.gg a compan going bust), and generates a news story.
 * Also handles hard-coded news events.
 */
class NewsGenerator
{

public:

	static void Initialise ();

	// Events related to Company computers

	/**
	 * Generates a news story about a hacked computer whose hacker isn't known
	 * @param comp Computer that was hacked
	 * @param al Access Log
	 */
	static void ComputerHacked		( Computer *comp, AccessLog *al );

	/**
	 * Generates a news story about a computer that was destroyed
	 * @param comp Computer that was hacked
	 * @param filesdeleted true if the files were unabled to be recovered
	 */
	static void ComputerDestroyed	( Computer *comp, bool filesdeleted );

	/**
	 * Generates a news story about files being stolen
	 * @param comp Computer that was hacked
	 * @param filetype Type of files stolen
	 * @param totalfilesize The size of the files stolen
	 * @note filetype can be SCIENTIFIC, CORPORATE, CUSTOMER, or SOFTWARE
	 */
	static void AllFilesStolen		(Computer *comp, const string& filetype, int totalfilesize );

	/**
	 * Generates a news story about files being deleted
	 * @param comp Computer that was hacked
	 * @param filetype Type of files deleted
	 * @note filetype can be SCIENTIFIC, CORPORATE, CUSTOMER, or SOFTWARE
	 */
	static void AllFilesDeleted		(Computer *comp, const string& filetype );

	// Events related to people

	/**
	 * Generates a news story about an arrest
	 * @param person Person who was arrested
	 * @param comp TODO: Document
	 * @param reason Reason person was arrested
	 */
	static void Arrested (Person *person, Computer *comp, const string& reason );

};



#endif

