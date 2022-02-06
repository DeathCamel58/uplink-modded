
#include <app/miscutils.h>
#include "gucci.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/serialise.h"

#include "world/computer/databank.h"
#include "world/company/sale.h"

#include "mmgr.h"


Sale::Sale ()
{

	UplinkStrncpy ( title, "", sizeof ( title ) )
	saleTYPE = SALETYPE_NONE;
	swhwTYPE = SOFTWARETYPE_NONE;

}

Sale::~Sale ()
{

	DeleteDArrayData ( (DArray <UplinkObject *> *) &versions );

}

void Sale::SetTitle ( char *newtitle )
{
	
	UplinkAssert ( strlen (newtitle) < SIZE_SALE_TITLE )
	UplinkStrncpy ( title, newtitle, sizeof ( title ) )

}

void Sale::SetSaleTYPE ( int newTYPE )
{

	saleTYPE = newTYPE;

}

void Sale::SetSwhwTYPE ( int newSwhwTYPE ) 
{

	swhwTYPE = newSwhwTYPE;

}

void Sale::AddVersion ( char *details, int cost, int size, int data )
{

	auto *sv = new SaleVersion ();
	sv->Set ( details, cost, size, data );

	versions.PutData ( sv );

}

SaleVersion *Sale::GetVersion ( int index )
{

	if ( versions.ValidIndex (index) )
		return versions.GetData (index);

	else
		return nullptr;

}

bool Sale::Load ( FILE *file )
{
	
	LoadID ( file );

	if ( !LoadDynamicStringStatic ( title, SIZE_SALE_TITLE, file ) ) return false;

	if ( !FileReadData ( &saleTYPE, sizeof(saleTYPE), 1, file ) ) return false;
	if ( !FileReadData ( &swhwTYPE, sizeof(swhwTYPE), 1, file ) ) return false;

	if ( !LoadDArray ( (DArray <UplinkObject *> *) &versions, file ) ) return false;

	LoadID_END ( file );

	return true;

}

void Sale::Save ( FILE *file )
{
	
	SaveID ( file );

	SaveDynamicString ( title, SIZE_SALE_TITLE, file );
	
	fwrite ( &saleTYPE, sizeof(saleTYPE), 1, file );
	fwrite ( &swhwTYPE, sizeof(swhwTYPE), 1, file );
	
	SaveDArray ( (DArray <UplinkObject *> *) &versions, file );

	SaveID_END ( file );

}

void Sale::Print ()
{

    cout << "Sale :" << endl;
    PrintValue("Title", title);
    PrintValue("Sale Type", saleTYPE);
    PrintValue("Swhw Type", swhwTYPE);

	PrintDArray ( (DArray <UplinkObject *> *) &versions );

}

void Sale::Update ()
{
}
	
string Sale::GetID ()
{
	
	return "SALE";

}

int Sale::GetOBJECTID ()
{

	return OID_SALE;

}


// ============================================================================



SaleVersion::SaleVersion ()
{
	
	details = nullptr;
	cost = -1;
	size = -1;
	data = -1;

}

SaleVersion::~SaleVersion ()
{

	delete [] details;

}

void SaleVersion::Set ( char *newdetails, int newcost, int newsize, int newdata )
{

	delete [] details;
	details = new char [strlen(newdetails)+1];
	UplinkSafeStrcpy ( details, newdetails )

	cost = newcost;
	size = newsize;
	data = newdata;

}

char *SaleVersion::GetDetails ()
{

	return details;

}

bool SaleVersion::Load ( FILE *file )
{

	LoadID ( file );

	if ( !LoadDynamicStringPtr ( &details, file ) ) return false;
	if ( !FileReadData ( &cost, sizeof(cost), 1, file ) ) return false;
	if ( !FileReadData ( &size, sizeof(size), 1, file ) ) return false;
	if ( !FileReadData ( &data, sizeof(data), 1, file ) ) return false;
		
	LoadID_END ( file );

	return true;

}

void SaleVersion::Save  ( FILE *file )
{

	SaveID ( file );

	SaveDynamicString ( details, file );
	fwrite ( &cost, sizeof(cost), 1, file );
	fwrite ( &size, sizeof(size), 1, file );
	fwrite ( &data, sizeof(data), 1, file );
		
	SaveID_END ( file );

}

void SaleVersion::Print ()
{

    cout << "SaleVersion :" << details << endl;
    PrintValue("Cost", cost);
    PrintValue("Size", size);
    PrintValue("Data", data);

}

void SaleVersion::Update ()
{
}
	
string SaleVersion::GetID ()
{

	return "SALEVER";

}

int SaleVersion::GetOBJECTID ()
{
	
	return OID_SALEVERSION;

}

