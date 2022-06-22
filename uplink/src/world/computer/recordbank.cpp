// recordbank.cpp: implementation of the recordbank class.
//
//////////////////////////////////////////////////////////////////////

#include <app/miscutils.h>

#include "app/app.h"
#include "app/globals.h"
#include "app/serialise.h"

#include "world/computer/recordbank.h"
#include "world/generator/numbergenerator.h"

#include "mmgr.h"


RecordBank::RecordBank ()
= default;


RecordBank::~RecordBank ()
{

    DeleteLListData ( (LList <UplinkObject *> *) &records );

}

void RecordBank::AddRecord ( Record *newrecord )
{

	UplinkAssert (newrecord)
	records.PutData ( newrecord );

}

void RecordBank::AddRecordSorted (Record *newrecord, const string &sortfield )
{

	UplinkAssert (newrecord)
	UplinkAssert (!sortfield.empty())

	char *newvalue = newrecord->GetField ( sortfield );
	UplinkAssert (newvalue)

	bool inserted = false;

	for ( int i = 0; i < records.Size (); ++i ) {
		if ( records.GetData (i) ) {

			char *fieldvalue = records.GetData (i)->GetField ( sortfield );
			UplinkAssert (fieldvalue)

			if ( strcmp ( fieldvalue, newvalue ) > 0 ) {
				records.PutDataAtIndex ( newrecord, i );
				inserted = true;
				break;
			}

		}
	}
			
	if ( !inserted ) records.PutDataAtEnd ( newrecord );

}

string RecordBank::MakeSafeField(const string &fieldval )
{
	string val = fieldval;
	for ( size_t i = 0; i < val.length(); ++i )
		if ( val[i] == ';' ) val[i] = '.';
	return val;
}

Record *RecordBank::GetRecord ( int index )
{

	if ( records.ValidIndex (index) )
		return records [index];

	else
		return nullptr;

}

Record *RecordBank::GetRecord (const string &query )
{

	LList <Record *> *result = GetRecords ( query );

    if ( !result ) {

		return nullptr;

	} else if ( result->Size () == 0 ) {

        delete result;
        return nullptr;

    } else if ( result->Size () > 1 ) {

		cout << "RecordBank::GetRecord, found more than 1 match, returning the first" << endl;
		UplinkAssert ( result->ValidIndex (0) )
		Record *record = result->GetData (0);
        delete result;
        return record;

	} else {

		UplinkAssert ( result->ValidIndex (0) )
        Record *record = result->GetData (0);
        delete result;
		return record;

	}

}

Record *RecordBank::GetRecordFromName(const string &name )
{
	string tempname = MakeSafeField( name );

	string query = RECORDBANK_NAME " = " + tempname;
    return GetRecord( query );
}

Record *RecordBank::GetRecordFromNamePassword (const string &name, const string &password )
{
	string tempname = MakeSafeField( name );
	string passwd = MakeSafeField( password );

	string query = RECORDBANK_NAME " = " + tempname + " ; " RECORDBANK_PASSWORD " = " + passwd;
    return GetRecord( query );
}

Record *RecordBank::GetRecordFromAccountNumber (const string &accNo )
{
	string tempAccNo = MakeSafeField( accNo );

	string query = RECORDBANK_ACCNO " = " + tempAccNo;
    return GetRecord( query );
}

LList <Record *> *RecordBank::GetRecords (const string &query )
{

	// Make a copy of the query
	
	char *localquery = new char [query.length()+1];
	UplinkSafeStrcpy ( localquery, query.c_str() )

	// Calculate the number of conditions 

	int numconditions = 1;
	for ( char *p = localquery; *p != 0; ++p )
		if ( *p == ';' )
			++numconditions;

	// Create our data structures to represent the conditions

	char **condition = new char *[numconditions];
	condition [0] = localquery;
	int i;

	for ( i = 1; i < numconditions; ++i ) {
		condition [i] = strchr ( condition [i-1], ';' );		
		assert( *(condition [i]-1) == ' ' );			// Check the ';' is surrounded by spaces
		assert( *(condition [i]+1) == ' ' );
		*(condition [i] - 1) = '\x0';						// Replace the space before the ';' with a '\x0'
		(condition [i]) += 2;								// Point condition at char after '\x0'
	}

	// Parse each condition into 3 pieces of data - fieldname, operation and required value
	
	char **fields	= new char *[numconditions];
	char *ops		= new char  [numconditions];
	char **values	= new char *[numconditions];

	for ( i = 0; i < numconditions; ++i ) {

		fields [i] = condition [i];

		char *oplocation;

		if      ( strchr ( condition [i], '=' )	)	oplocation = strchr ( condition [i], '=' );
		else if ( strchr ( condition [i], '!' )	)	oplocation = strchr ( condition [i], '!' );
		else if ( strchr ( condition [i], '+' )	)	oplocation = strchr ( condition [i], '+' );
		else if ( strchr ( condition [i], '-' )	)	oplocation = strchr ( condition [i], '-' );
		else	UplinkAbort ( "RecordBank::GetRecords, invalid query" )
		
		assert( *(oplocation - 1) == ' ' );	// Check the op is surrounded by spaces
		assert( *(oplocation + 1) == ' ' );

		*(oplocation - 1) = '\x0';					// Terminate the field string before the op
		
		ops [i] = *(oplocation);					// Get the operation character
		values [i] = oplocation + 2;				// Get the required value

	}

	// Test the conditions on each record 
	// Add any successes into the results list
	
	auto *results = new LList <Record *> ();
	
	for ( int ri = 0; ri < records.Size (); ++ri ) {
		if ( records.ValidIndex (ri) ) {

			Record *rec = records.GetData (ri);
			int nummatches = 0;

			for ( i = 0; i < numconditions; ++i ) {

				char *reqfield = fields [i];							// Required field name
				char *reqvalue = values [i];							// Required field value
				char *thisvalue = rec->GetField ( reqfield );			// Actual field value
				
				if ( thisvalue ) {

					switch ( ops [i] ) {
						case '=':	if ( strcmp ( thisvalue, reqvalue ) == 0 ) ++nummatches;		break;
						case '!':   if ( strcmp ( thisvalue, reqvalue ) != 0 ) ++nummatches;		break;
						case '+':	if ( strstr ( thisvalue, reqvalue ) )	   ++nummatches;		break;
						case '-':	if ( !strstr ( thisvalue, reqvalue ) )	   ++nummatches;		break;
						default:	UplinkAbort ( "RecordBank::GetRecord, unrecognised op code" )
					}

				}

			}

			if ( nummatches == numconditions )
				results->PutData ( rec );

		}
	}

    // Clean up used memory

    delete [] localquery;
    delete [] condition;
    delete [] fields;
    delete [] ops;
    delete [] values;

	// Return the results

	if ( results->Size () > 0 ) 
		return results;

    else {
        delete results;
		return nullptr;
    }

}

Record *RecordBank::GetRandomRecord (const string &query )
{

	LList <Record *> *records = GetRecords ( query );

	if ( records ) {

		int index = NumberGenerator::RandomNumber ( records->Size () );
		UplinkAssert ( records->ValidIndex (index) )
        Record *result = records->GetData (index);
        delete records;
        return result;

	} else {
	    return nullptr;
	}

}

bool RecordBank::Load ( FILE *file )
{

	LoadID ( file );

	if ( !LoadLList ( (LList <UplinkObject *> *) &records, file ) ) return false;

	LoadID_END ( file );

	return true;

}

void RecordBank::Save ( FILE *file )
{

	SaveID ( file );

	SaveLList ( (LList <UplinkObject *> *) &records, file );

	SaveID_END ( file );

}

void RecordBank::Print ()
{

	cout << "RecordBank" << endl;
	PrintLList ( (LList <UplinkObject *> *) &records );

}

void RecordBank::Update ()
{

}
	
string RecordBank::GetID ()
{

	return "RECBANK";

}

int RecordBank::GetOBJECTID ()
{

	return OID_RECORDBANK;

}

//////////////////////////////////////////////////////////////////////

Record::Record()
= default;

Record::~Record()
{

    DeleteBTreeData ( &fields );

}

void Record::AddField (const string &name, const string& value )
{

	char *newvalue = new char [value.length() + 1];
	UplinkSafeStrcpy ( newvalue, value.c_str() )
	fields.PutData ( name, newvalue );

}

void Record::AddField (const string &name, int value )
{

	size_t newvaluesize = 16;
	char *newvalue = new char [newvaluesize];
	UplinkSnprintf ( newvalue, newvaluesize, "%d", value )
	fields.PutData ( name, newvalue );

}

void Record::ChangeField (const string &name, const string &newvalue )
{

	BTree <char *> *tree = fields.LookupTree ( name );

	if ( tree ) {

		delete [] tree->data;
		size_t tree__datasize = newvalue.length() + 1;
		tree->data = new char [tree__datasize];
		UplinkStrncpy ( tree->data, newvalue.c_str(), tree__datasize )

	}
	else {

		cout << "Record::ChangeField, WARNING : field " << name << " not found (created instead)" << endl;
		AddField ( name, newvalue );

	}

}

void Record::ChangeField (const string &name, int newvalue )
{

	BTree <char *> *tree = fields.LookupTree ( name );

	if ( tree ) {

		delete [] tree->data;
		size_t tree__datasize = 8;
		tree->data = new char [tree__datasize];
		UplinkSnprintf ( tree->data, tree__datasize, "%d", newvalue )

	}
	else {

		cout << "Record::ChangeField, WARNING : field " << name << " not found (created instead)" << endl;
		AddField ( name, newvalue );

	}

}

char *Record::GetField (const string &name )
{

	if ( name.empty() ) return nullptr;

	BTree <char *> *tree = fields.LookupTree ( name );

	if ( tree ) {

		return tree->data;

	}
	else {

		return nullptr;

	}

}

void Record::DeleteField (const string &name )
{

	fields.RemoveData ( name );

}

int RecordBank::FindNextRecordIndexNameNotSystemAccount ( int curindex )
{

	int recordindex = 0;
	if ( curindex != -1 )
		recordindex = curindex + 1;

	while ( recordindex < records.Size () ) {

		Record *rec = GetRecord ( recordindex );
		UplinkAssert (rec)
		char *thisname = rec->GetField ( RECORDBANK_NAME );
		UplinkAssert (thisname)

		if ( strcmp ( thisname, RECORDBANK_ADMIN ) != 0 && 
		     strcmp ( thisname, RECORDBANK_READWRITE ) != 0 &&
			 strcmp ( thisname, RECORDBANK_READONLY ) != 0 ) {

			return recordindex;
		}

		recordindex++;

	}

	return -1;

}

bool Record::Load  ( FILE *file )
{

	LoadID ( file );

	if ( !LoadBTree ( &fields, file ) ) return false;

	LoadID_END ( file );

	return true;

}

void Record::Save  ( FILE *file )
{

	SaveID ( file );

	SaveBTree ( &fields, file );

	SaveID_END ( file );

}

void Record::Print ()
{

    cout << "Record :" << endl;

	DArray <char *> *field_names = fields.ConvertIndexToDArray ();
	DArray <char *> *field_values = fields.ConvertToDArray ();

	for ( int i = 0; i < field_names->Size (); ++i ) {

		UplinkAssert ( field_names->ValidIndex (i) )
		UplinkAssert ( field_values->ValidIndex (i) )

        PrintValue(field_names->GetData(i), field_values->GetData(i));

	}

	delete field_names;
	delete field_values;

}

void Record::Update ()
{
}
	
string Record::GetID ()
{

	return "RECORD";

}

int Record::GetOBJECTID ()
{

	return OID_RECORD;

}

