
#include <app/miscutils.h>

#include "app/globals.h"
#include "app/serialise.h"

#include "world/company/news.h"

#include "mmgr.h"


News::News ()
{

	details = "";
	NEWSTYPE = NEWS_TYPE_NONE;

    headline = " ";
    UplinkStrncpy ( data1, " ", sizeof ( data1 ) )
    UplinkStrncpy ( data2, " ", sizeof ( data2 ) )

}

News::~News ()
= default;

void News::SetDate ( Date *newdate )
{

	date.SetDate ( newdate );

}

void News::SetHeadline (const string &newheadline )
{

	headline = newheadline;

}

void News::SetDetails (const string &newdetails )
{

    details = newdetails;

}

string News::GetDetails ()
{

	return details;

}

void News::SetData (int newNEWSTYPE, const string &newdata1, const string &newdata2 )
{

	NEWSTYPE = newNEWSTYPE;

	if ( !newdata1.empty() ) {
		assert(newdata1.length() < SIZE_NEWS_DATA);
		UplinkStrncpy ( data1, newdata1.c_str(), sizeof ( data1 ) )
	}
	else
		UplinkStrncpy ( data1, " ", sizeof ( data1 ) )

	if ( !newdata2.empty() ) {
		assert(newdata2.length() < SIZE_NEWS_DATA);
		UplinkStrncpy ( data2, newdata2.c_str(), sizeof ( data2 ) )
	}
	else
		UplinkStrncpy ( data2, " ", sizeof ( data2 ) )

}

bool News::Load  ( FILE *file )
{
	
	LoadID ( file );

	if ( !date.Load ( file ) ) return false;
	if ( !FileReadData ( &NEWSTYPE,	sizeof(NEWSTYPE), 1, file ) ) return false;

	if ( !LoadDynamicStringInt( headline, file ) ) return false;
    if ( !LoadDynamicStringStatic ( data1, SIZE_NEWS_DATA, file ) ) return false;
    if ( !LoadDynamicStringStatic ( data2, SIZE_NEWS_DATA, file ) ) return false;

	if ( !LoadDynamicStringInt ( details, file ) ) return false;

	LoadID_END ( file );

	return true;

}

void News::Save  ( FILE *file )
{
	
	SaveID ( file );

	date.Save ( file );
	fwrite ( &NEWSTYPE, sizeof(NEWSTYPE), 1, file );

	SaveDynamicString ( headline, SIZE_NEWS_HEADLINE, file );
	SaveDynamicString ( data1, SIZE_NEWS_DATA, file );
	SaveDynamicString ( data2, SIZE_NEWS_DATA, file );

	SaveDynamicString ( details, file );

	SaveID_END ( file );

}

void News::Print ()
{

    cout << "News:" << endl;
    PrintValue("Headline", headline);
    date.Print ();
    PrintValue("Details", details);

    PrintValue("NEWSTYPE", NEWSTYPE);
    PrintValue("Data1", data1);
    PrintValue("Data2", data2);
		
}
	
string News::GetID ()
{

	return "NEWS";

}

int News::GetOBJECTID ()
{

	return OID_NEWS;

}
