
#include <cassert>
#include <algorithm>

#include "tosser.h"

#include "bungle.h"

#include "mmgr.h"

//#include "debug.h"

struct LocalFileHeader
{

	char	signature [4];
	short	version;
	short	bitflag;
	short	compressionmethod;
	short	lastmodfiletime;
	short	lastmodfiledate;
	int		crc32;
	int		compressedsize;
	int		uncompressedsize;
	short	filenamelength;
	short	extrafieldlength;
	
	char	*filename;
	char	*extrafield;
	char	*data;

	char	*id;

};


static BTree <LocalFileHeader *> files;


void BglSlashify (string &string )
{

	// Searches out all slashes and changes them to forward slashes
    replace( string.begin(), string.end(), '\\', '/');

	// Also lowercases the string
    transform(string.begin(), string.end(), string.begin(), ::tolower);

}

bool BglOpenZipFile (const string &zipfile, const string &apppath, const string &id )
{

	FILE *file = fopen ( zipfile.c_str(), "rb" );
	if ( !file ) return false;

	bool result = BglOpenZipFile ( file, apppath, id );

	fclose ( file );

	return result;

}

bool BglOpenZipFile (FILE *file, const string &apppath, const string &id )
{
	
	if ( !file ) return false;

	while ( !feof ( file ) ) {

		LocalFileHeader *fh = new LocalFileHeader ();

		fread ( fh->signature, 4, 1, file );

		if ( fh->signature [0] != 'P' || fh->signature [1] != 'K' )	{				// No longer reading valid data
			delete fh;
			break;
		}

		fread ( &fh->version, 2, 1, file );
		fread ( &fh->bitflag, 2, 1, file );
		fread ( &fh->compressionmethod, 2, 1, file );
		fread ( &fh->lastmodfiletime, 2, 1, file );
		fread ( &fh->lastmodfiledate, 2, 1, file );
		fread ( &fh->crc32, 4, 1, file );
		fread ( &fh->compressedsize, 4, 1, file );
		fread ( &fh->uncompressedsize, 4, 1, file );
		fread ( &fh->filenamelength, 2, 1, file );
		fread ( &fh->extrafieldlength, 2, 1, file );

		if ( fh->filenamelength > 0 ) {
			fh->filename = new char [fh->filenamelength+1];
			fread ( fh->filename, fh->filenamelength, 1, file );
			*(fh->filename + fh->filenamelength) = 0;
		}
		else 
			fh->filename = nullptr;

		if ( fh->extrafieldlength > 0 ) {
			fh->extrafield = new char [fh->extrafieldlength+1];
			fread ( fh->extrafield, fh->extrafieldlength, 1, file );
			*(fh->extrafield + fh->extrafieldlength) = 0;
		}
		else
			fh->extrafield = nullptr;

		if ( fh->uncompressedsize > 0 ) {
			fh->data = new char [fh->uncompressedsize+1];
			fread ( fh->data, fh->uncompressedsize, 1, file );
			*(fh->data + fh->uncompressedsize) = 0;
		}
		else
			fh->data = nullptr;

		if ( !id.empty() ) {
			fh->id = new char [id.length() + 1];
			strcpy ( fh->id, id.c_str() );
		}
		else 
			fh->id = nullptr;

		if ( fh->compressionmethod == 0 &&
			 fh->compressedsize == fh->uncompressedsize &&
			 fh->filename ) {

			string fullfilename = apppath + fh->filename;

			BglSlashify ( fullfilename );

			files.PutData ( fullfilename, fh );

		}
		else {
            delete [] fh->filename;
            delete [] fh->extrafield;
            delete [] fh->data;
            delete [] fh->id;

			delete fh;
		}

	}

	return true;

}

bool BglFileLoaded (const string &filename )
{

	string filenamecopy = filename;
	BglSlashify ( filenamecopy );

	LocalFileHeader *lfh = files.GetData ( filenamecopy );

	return ( lfh != nullptr );

}

void BglCloseZipFile_Recursive (BTree <LocalFileHeader *> *files,
                                LList <char *> *removableids,
                                const string &id )
{

	assert (removableids);
	assert (!id.empty());

	if ( !files ) return;										// Base case - end of the binary tree
	
	LocalFileHeader *lfh = files->data;

	if ( lfh && lfh->id && lfh->id == id ) {

		// This one is a match and should be flagged for removal
		removableids->PutData ( (char *) files->id.c_str() );

	}

	// Recurse

	BglCloseZipFile_Recursive ( files->Left (), removableids, id );
	BglCloseZipFile_Recursive ( files->Right (), removableids, id );

}

void BglCloseZipFile (const string &id )
{

	//
	// Create a DArray of filenames to be removed 
	// from our Binary Tree
	//

	LList <char *> removableids;

	//
	// Fill the DArray with a recursive search algorithm
	//

	BglCloseZipFile_Recursive ( &files, &removableids, id );

	//
	// Run through that DArray, looking up the file, 
	// deleting the data and removing it from the full btree
	//

	for ( int i = 0; i < removableids.Size (); ++i ) {
		
		char *filename = removableids.GetData (i);
		assert (filename);
		
		LocalFileHeader *lfi = files.GetData (filename);
		assert (lfi);

		files.RemoveData ( filename );

        delete [] lfi->filename;
        delete [] lfi->extrafield;
        delete [] lfi->data;
        delete [] lfi->id;

		delete lfi;
			
	}

}

bool BglExtractFile (const string &filename, const string &target )
{

	string filenamecopy = filename;
	BglSlashify ( filenamecopy );

	LocalFileHeader *lfh = files.GetData ( filenamecopy );

	if ( lfh ) {

		FILE *output;
		
		if ( !target.empty() )	output = fopen ( target.c_str(), "wb" );
		else			output = fopen ( filename.c_str(), "wb" );

		if ( !output ) return false;

		fwrite ( lfh->data, lfh->uncompressedsize, 1, output );

		fclose ( output );

		/*
		if ( target )
			DEBUG_PRINTF( "Written %s to %s\n", filename, target );
		else
			DEBUG_PRINTF( "Written %s to %s\n", filename, filename );
		*/

		return true;
	}		

	return false;

}


void BglExtractAllFiles (const string &zipfile )
{

	FILE *file = fopen ( zipfile.c_str(), "rb" );
	assert (file);

	while ( !feof ( file ) ) {

		LocalFileHeader fh;

		fread ( &fh.signature, 4, 1, file );
		fread ( &fh.version, 2, 1, file );
		fread ( &fh.bitflag, 2, 1, file );
		fread ( &fh.compressionmethod, 2, 1, file );
		fread ( &fh.lastmodfiletime, 2, 1, file );
		fread ( &fh.lastmodfiledate, 2, 1, file );
		fread ( &fh.crc32, 4, 1, file );
		fread ( &fh.compressedsize, 4, 1, file );
		fread ( &fh.uncompressedsize, 4, 1, file );
		fread ( &fh.filenamelength, 2, 1, file );
		fread ( &fh.extrafieldlength, 2, 1, file );

		if ( fh.filenamelength > 0 ) {
			fh.filename = new char [fh.filenamelength];
			fread ( fh.filename, fh.filenamelength, 1, file );
			*(fh.filename + fh.filenamelength) = 0;
		}
		else 
			fh.filename = nullptr;

		if ( fh.extrafieldlength > 0 ) {
			fh.extrafield = new char [fh.extrafieldlength+1];
			fread ( fh.extrafield, fh.extrafieldlength, 1, file );
			*(fh.extrafield + fh.extrafieldlength) = 0;
		}
		else
			fh.extrafield = nullptr;

		if ( fh.uncompressedsize > 0 ) {
			fh.data = new char [fh.uncompressedsize];
			fread ( fh.data, fh.uncompressedsize, 1, file );
			*(fh.data + fh.uncompressedsize) = 0;
		}
		else
			fh.data = nullptr;


		if ( fh.compressionmethod == 0 &&
			 fh.compressedsize == fh.uncompressedsize &&
			 fh.filename ) {

			FILE *output = fopen ( fh.filename, "wb" );
			assert (output);

			fwrite ( fh.data, fh.uncompressedsize, 1, output );

			fclose ( output );

		}

	}

	fclose ( file );

}


DArray <char *> *BglListFiles (const string &path, const string &directory, const string &filter )
{

    string dirCopy = path + directory;
    BglSlashify ( dirCopy );

    DArray <char *> *result = files.ConvertIndexToDArray();

    for ( int i = 0; i < result->Size(); ++i ) {
        if ( result->ValidIndex(i) ) {
        
            char *fullPath = result->GetData(i);
            string thisDir = fullPath;
            thisDir[dirCopy.length()] = '\x0';

            bool removeMe = false;

            if ( thisDir != dirCopy )         removeMe = true;
            if ( strstr ( fullPath, filter.c_str() ) == nullptr )      removeMe = true;

            if ( removeMe )
                result->RemoveData( i );

        }
    }

    return result;

}


void BglCloseAllFiles( BTree<LocalFileHeader *> *files )
{
	if ( !files ) return;

	BglCloseAllFiles( files->Left() );
	BglCloseAllFiles( files->Right() );

	LocalFileHeader *lfi = files->data;
	if ( lfi ) {
        delete [] lfi->filename;
        delete [] lfi->extrafield;
        delete [] lfi->data;
        delete [] lfi->id;
		delete lfi;
	}

	files->Empty();
}

void BglCloseAllFiles()
{
	BglCloseAllFiles( &files );
}
