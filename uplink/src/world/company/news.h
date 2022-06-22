
#ifndef included_news_h
#define included_news_h

// ============================================================================

#include "app/uplinkobject.h"
#include "world/date.h"

#define SIZE_NEWS_HEADLINE	128
#define SIZE_NEWS_DATA		128

#define NEWS_TYPE_NONE		0
#define NEWS_TYPE_HACKED	1				// Data1 = IP
#define NEWS_TYPE_ARREST	2				// Data1 = name of person
#define NEWS_TYPE_DELETED   3				// Data1 = IP
#define NEWS_TYPE_STOLEN	4				// Data1 = IP
#define NEWS_TYPE_COMPUTERSHUTDOWN 5		// Data1 = IP			
#define NEWS_TYPE_COMPUTERDESTROYED 6		// Data1 = IP			All databanks formatted first


// ============================================================================

//! News UplinkObject
/**
 * Represents a single news story, posted onto one of the news servers
 */
class News : public UplinkObject
{

protected:

    /**
     * The text content of the news item
     */
	string details;

public:

    /**
     * The Date of the news item
     */
	Date date;

	/**
	 * The headline of the news item
	 */
	string headline;

	/**
	 * The type of the news item
	 */
	int NEWSTYPE;

	/**
	 * Varies based on newNEWSTYPE
	 */
	char data1 [SIZE_NEWS_DATA];

	/**
	 * Varies based on newNEWSTYPE
	 */
	char data2 [SIZE_NEWS_DATA];

public:

	News ();
	~News () override;

	/**
	 * Sets the Date of the news item
	 * @param newdate The new Date
	 */
	void SetDate     ( Date *newdate );

	/**
	 * Sets the headline of the news item
	 * @param newheadline The news headline
	 */
	void SetHeadline (const string &newheadline );

	/**
	 * Sets the text content of the news item
	 * @param newdetails The news content
	 */
	void SetDetails  (const string &newdetails );

	/**
	 * Gets the text content of the news item
	 * @return
	 */
	string GetDetails ();

	/**
	 * Sets the data of the news item
	 * @param newNEWSTYPE The type of the news item
	 * @param newdata1 Varies based on newNEWSTYPE
	 * @param newdata2 Varies based on newNEWSTYPE
	 * @note Depending on the newNEWSTYPE, newdata1 and newdata2 can represent different data
	 */
	void SetData (int newNEWSTYPE, const string &newdata1 = "", const string &newdata2 = "" );

	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	
	string GetID () override;
	int   GetOBJECTID () override;

};



#endif

