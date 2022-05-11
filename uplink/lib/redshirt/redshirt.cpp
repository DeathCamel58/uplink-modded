// -*- tab-width:4 c-file-style:"cc-mode" -*-
#ifdef WIN32
#include <direct.h>
#include <io.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#endif

#include <cstring>
#include <cassert>
#include <cstdlib>

#include "bungle.h"
#include "tosser.h"
#include "redshirt.h"
#include "hash.h"

#include "mmgr.h"


const char marker  [] = "REDSHIRT\x0";
const char marker2 [] = "REDSHRT2\x0";
const int SIZE_MARKER = 9;

const int SIZE_RSFILENAME = 256;

static string tempfilename = "";                         // Returned by RsArchiveFileOpen
static string rsapppath = "";
static string tempdir = "";
static bool rsInitialised = false;

#define BUFFER_SIZE 16384

typedef void filterFunc(unsigned char *, unsigned);
typedef bool headerFunc(FILE *);

/**
 * Transfers all the data from input to output via filterFunc(buffer, length)
 * @param input
 * @param output
 * @param filterFunc
 * @return
 */
bool filterStream(FILE *input, FILE *output, filterFunc *filterFunc)
{
	unsigned char buffer[BUFFER_SIZE];

	do {
		size_t bytesread = fread(buffer, 1, BUFFER_SIZE, input);
		if (bytesread <= 0)
			break;

		filterFunc(buffer, (unsigned) bytesread);

		if (fwrite(buffer, 1, bytesread, output) < bytesread) 
			return false;

	} while (true);

	return true;
}

static unsigned int RsFileCheckSum ( FILE *input, unsigned char *hashbuffer, unsigned int hashsize )
{

	unsigned char buffer [ BUFFER_SIZE ];

	void *context = HashInitial ();

	size_t bytesread;
	while ( ( bytesread = fread ( buffer, 1, BUFFER_SIZE, input ) ) > 0 ) {
		HashData ( context, buffer, (unsigned int) bytesread );
	}

	return HashFinal ( context, hashbuffer, hashsize );

}

bool writeRsEncryptedCheckSum ( FILE *output )
{

	bool result = false;

	unsigned int hashsize = HashResultSize ();
	auto *hashbuffer = new unsigned char [ hashsize ];

	fseek ( output, SIZE_MARKER + hashsize, SEEK_SET );

	if ( RsFileCheckSum ( output, hashbuffer, hashsize ) == hashsize ) {
		fseek ( output, SIZE_MARKER, SEEK_SET );
		result = ( fwrite ( hashbuffer, hashsize, 1, output ) == 1 );
	}

	delete [] hashbuffer;

	return result;

}

/**
 * This takes an input buffer, and flips first bit of every byte
 * @param buffer
 * @param length
 */
void flipBits(unsigned char *buffer, unsigned int length)
{
	// Decrypt each byte in the buffer.
	
	for (unsigned i = 0; i < length; i++)
		buffer[i] += 128;
}

/**
 * Check if file exists
 * @param filename
 * @return `true` if file exists, `false` if not
 */
bool RsFileExists  (const string &filename )
{

	FILE *file = fopen ( filename.c_str(), "r" );

	bool success = file != nullptr;

	if ( success ) fclose ( file );

	return success;

}

/**
 * Checks if given file is encrypted, then skips to data
 * @param input File handle to check
 * @return `true` if encrypted, `false` if not
 */
bool readRsEncryptedHeader( FILE *input )
{

	bool result = false;

	char newmarker [SIZE_MARKER];
	if ( fread ( newmarker, SIZE_MARKER, 1, input ) == 1 ) {
		if ( strcmp ( newmarker, marker2 ) == 0 ) {
			unsigned int hashsize = HashResultSize ();
			auto *hashbuffer = new unsigned char [ hashsize ];
			result = ( fread ( hashbuffer, hashsize, 1, input ) == 1 );
			delete [] hashbuffer;
		}
		else if ( strcmp ( newmarker, marker ) == 0 ) {
			result = true;
		}
	}

	return result;

}

bool writeRsEncryptedHeader ( FILE *output )
{

	bool result = false;

	if ( fwrite ( marker2, SIZE_MARKER, 1, output ) == 1 ) {
		unsigned int hashsize = HashResultSize ();
		auto *hashbuffer = new unsigned char [ hashsize ];
		memset ( hashbuffer, 0, hashsize );
		result = ( fwrite ( hashbuffer, hashsize, 1, output ) == 1 );
		delete [] hashbuffer;
	}

	return result;

}

/**
 * Does nothing
 * @return Returns `true` no matter what
 */
bool noHeader( FILE * )
{
	// Useful for a no-operation
	return true;
}

/**
 * Opens a file and checks if it's encrypted
 * @param filename The filename to check
 * @return `true` if encrypted
 */
bool RsFileEncryptedNoVerify (const string &filename )
{

	FILE *input = fopen ( filename.c_str(), "rb" );
	if ( !input ) return false;

	bool result = readRsEncryptedHeader ( input );

	fclose ( input );

	return result;

}

/**
 * Checks if file header starts with `REDSHIRT\x0` or `REDSHIRT2\x0`
 * @param filename The filename to check
 * @return `true` if header is `REDSHIRT\x0`, `false`
 */
bool RsFileEncrypted (const string &filename )
{
	
	FILE *input = fopen ( filename.c_str(), "rb" );
	if ( !input ) return false;

	bool result = false;

	char newmarker [SIZE_MARKER];
	if ( fread ( newmarker, SIZE_MARKER, 1, input ) == 1 ) {
		if ( strcmp ( newmarker, marker2 ) == 0 ) {
			unsigned int hashsize = HashResultSize ();
			auto *hashbuffer = new unsigned char [ hashsize ];

			if ( fread ( hashbuffer, hashsize, 1, input ) == 1 ) {
				auto *hashbuffer2 = new unsigned char [ hashsize ];
				unsigned int retsize = RsFileCheckSum ( input, hashbuffer2, hashsize );
				if ( retsize > 0 && memcmp ( hashbuffer, hashbuffer2, retsize ) == 0 )
					result = true;

				delete [] hashbuffer2;
			}

			delete [] hashbuffer;
		}
		else if ( strcmp ( newmarker, marker ) == 0 ) {
			result = true;
		}
	}

	fclose ( input );

	return result;

}

/**
 * Takes input file (infile) and produces outfile
 * @param infile File to read
 * @param outfile File to write
 * @param readHeader Function which attempts to read the header and returns true on success
 * @param writeHeader Function which attempts to write the header and returns true on success
 * @param writeChecksum
 * @param filter Function which is used to transform the bytes in the file
 * @return `true` on success, `false` on failure
 */
bool filterFile(const string &infile, const string &outfile,
                headerFunc *readHeader,
                headerFunc *writeHeader,
                headerFunc *writeChecksum,
                filterFunc *filter)
{
	FILE *input = fopen ( infile.c_str(), "rb" );
	if ( !input ) 
		return false;

	// Read header from input file
	if (!readHeader(input)) {
		printf("redshirt: failed to read header!");
		fclose(input);
		return false;
	}

	FILE *output = fopen ( outfile.c_str(), "w+b" );
    if ( !output ) {
        fclose ( input );
        return false;
    }

	// Set completely unbuffered (we do our own)
	//setvbuf(input, nullptr, _IONBF, 0);
	//setvbuf(output, nullptr, _IONBF, 0);

	// Write header into output file
	if (!writeHeader(output)) {
		printf("redshirt: failed to write header!");
		fclose(input);
		fclose(output);
		remove(outfile.c_str());
		return false;
	}
		
	if (!filterStream(input, output, filter)){
		printf("redshirt: failed to write containning bytes!");
		fclose(input);
		fclose(output);
		remove(outfile.c_str());
		return false;
	}

	// Write checksum into output file
	if (!writeChecksum(output)) {
		printf("redshirt: failed to write checksum!");
		fclose(input);
		fclose(output);
		remove(outfile.c_str());
		return false;
	}

	fclose ( input );
	fclose ( output );

	return true;
}

/**
 * Process a file in place
 * @param filename The filename to decrypt
 * @param ext The extension for the decrypted file
 * @param readHeader Function which attempts to read the header and returns true on success
 * @param writeHeader Function which attempts to write the header and returns true on success
 * @param writeChecksum
 * @param filterFuncF unction which is used to transform the bytes in the file
 * @return `true` if successfully decrypted, `false` if not
 */
bool filterFileInPlace(const string &filename, const string &ext,
                       headerFunc *readHeader,
                       headerFunc *writeHeader,
                       headerFunc *writeChecksum,
                       filterFunc *filterFunc)
{
	string tempfilename = filename + ext;

	if (filterFile(filename, tempfilename, readHeader, writeHeader, writeChecksum, filterFunc)) {
		remove(filename.c_str());
		rename(tempfilename.c_str(), filename.c_str());
		return true;
	}
	else {
		printf( "Redshirt ERROR : Failed to write output file\n" );
		return false;
	}
}

/**
 * Encrypt a file in-place
 * @param filename The file name to encrypt
 * @return `true` if encrypted, `false` if not
 */
bool RsEncryptFile (const string &filename )
{
	return filterFileInPlace(filename, ".e", noHeader, writeRsEncryptedHeader, writeRsEncryptedCheckSum, flipBits);
}

/**
 * Decrypt a file in-place
 * @param filename The file name to decrypt
 * @return `true` if decrypted, `false` if not
 */
bool RsDecryptFile (const string &filename )
{
	if ( !RsFileEncrypted ( filename ) ) {
		// Not encrypted, so nothing to do
		return true;
	}

	return filterFileInPlace(filename, ".d", readRsEncryptedHeader, noHeader, noHeader, flipBits);
};

/**
 * Returns the basename of the file (minus any directory prefixes)
 * @param filename The file to get the base name of
 * @return The base name of the file
 */
string RsBasename(const string &filename)
{
	// Return the basename of the file (minus any directory prefixes)
	string p = filename.substr(filename.find_last_of("/\\") + 1);

	return p;
}

/**
 * Opens a file and returns the file handle of decrypted version
 * @param filename The filename to open
 * @param mode The mode to open the file in
 * @return The ifstream file handle of decrypted version
 */
FILE *RsFileOpen (const string &filename, const string &mode )
{

	if ( !RsFileExists ( filename ) ) return nullptr;

	if ( !RsFileEncrypted ( filename ) ) {

		// Not encrypted, so just open it
		FILE *file = fopen ( filename.c_str(), mode.c_str() );
		return file;

	}
    else {

	    string dfilename = tempdir + RsBasename(filename) + ".d";
		
		if (filterFile(filename, dfilename, readRsEncryptedHeader, noHeader, noHeader, flipBits)) {
			// Open the result and return it
			FILE *result = fopen ( dfilename.c_str(), mode.c_str() );
			return result;
		}
		else {
			printf ( "Redshirt ERROR : Failed to write to output file\n" );
			return nullptr;
		}
    }
}

/**
 * Close file and delete decrypted versions
 * @param filename The file name to close
 * @param file The file handle to close
 */
void RsFileClose (const string &filename, FILE *file )
{

	fclose ( file );

	// Delete any decrypted versions

	string dfilename = filename + ".d";

	int result = remove ( dfilename.c_str() );

}

/**
 * Opens an archive
 * @param filename Filename to open
 * @return `true` if archive opened, `false` if not
 */
bool RsLoadArchive (const string &filename )
{

	string fullfilename = rsapppath + filename;

	FILE *archive = RsFileOpen ( fullfilename, "rb" );

	if ( !archive ) {
		int len = (int) rsapppath.length();
		if ( len >= 5 ) {
			char c1 = rsapppath[ len - 5 ];
			char c2 = rsapppath[ len - 4 ];
			char c3 = rsapppath[ len - 3 ];
			char c4 = rsapppath[ len - 2 ];
			char c5 = rsapppath[ len - 1 ];

			if ( ( c1 == '\\' || c1 == '/' ) &&
			     ( c2 == 'l' || c2 == 'L' ) &&
			     ( c3 == 'i' || c3 == 'I' ) &&
			     ( c4 == 'b' || c4 == 'B' ) &&
				 ( c5 == '\\' || c5 == '/' ) ) {

				fullfilename[ len - 4 ] = '\0';
				fullfilename += filename;
				archive = RsFileOpen ( fullfilename, "rb" );
			}
		}

		if ( !archive ) return false;
	}

	bool result = BglOpenZipFile ( archive, rsapppath, filename );						// use the short filename as the id

	RsFileClose ( filename, archive );

	if ( result ) cout << "Successfully loaded data archive " << filename << endl;
	else		  cout << "Failed to load data archive " << filename << endl;

	return result;
}

/**
 * Opens a given file with a given mode and returns file handle
 * @param filename File name to open
 * @param mode Mode to open the file with
 * @return File handle
 */
FILE *RsArchiveFileOpen	(const string &filename, const string &mode )
{

	FILE *file = nullptr;
	string fname = RsArchiveFileOpen ( filename );

	if ( !fname.empty() ) {
		//printf( "Opening file %s\n", fname );
		file = fopen ( fname.c_str(), mode.c_str() );
	}

	return file;

}

/**
 * Opens archive file and returns location
 * @param filename File to open
 * @return Temporary file location WARNING: CHANGES ON EVERY CALL
 */
string RsArchiveFileOpen (const string &filename )
{

	string fullfilename = rsapppath + filename;

	//
	// Look to see if the file exists
	// If it does, just return its filename for use
	//

	//printf( "Trying file %s: ", fullfilename );

	if ( RsFileExists ( fullfilename ) ) {

		//printf( "Found.\n" );
        tempfilename = fullfilename;
        return tempfilename;

	}

	//
	// Now look in our data files for the file
	// Extract it to a temporary file in the same direcory if we find it
	//

	if ( BglFileLoaded ( fullfilename ) ) {

		string extension = fullfilename.substr(fullfilename.find_last_of('.'));
		assert (!extension.empty());
		
		int attempt = 0;
		bool success = false;
		string targetfilename;

		while ( !success && attempt < 3 ) {
			targetfilename = tempdir + "temp" + to_string(attempt) + extension;
			success = BglExtractFile ( fullfilename, targetfilename );
			++attempt;
		}
		
		if ( success ) {
			tempfilename = targetfilename;
            //printf( "Found as %s.\n", targetfilename );
			return tempfilename;
		}
	
		//printf( "Loaded, but no success.\n" );

	} else {
		//printf( "Not loaded.\n" );
	}

	//
	// Ooops - the file is nowhere to be found
	//

	cout << "REDSHIRT : Failed to load file : " << fullfilename << endl;
	return "";

}

/**
 * Check if file archive is open
 * @param filename Filename to check
 * @return `true` is loaded, `false` if not
 */
bool RsArchiveFileLoaded ( char *filename )
{

	string fullfilename = rsapppath + filename;

	if ( RsFileExists ( fullfilename ) ) return true;
	if ( BglFileLoaded ( fullfilename ) ) return true;

	return false;

}

/**
 * Close file, and delete temp file
 * @param filename The file name to close
 * @param file The file handle to close
 */
void RsArchiveFileClose	(const string &filename, FILE *file )
{

	if ( file ) fclose ( file );

	string extension = filename.substr(filename.find('.'));
	assert(!extension.empty());
		
	string targetfilename;
	int attempt = 0;

	while ( attempt < 3 ) {
	    targetfilename = tempdir + "temp" + to_string(attempt) + extension;
		int result = remove ( targetfilename.c_str() );
		++attempt;
	}

}

void RsCloseArchive	( char *filename )
{

	BglCloseZipFile ( filename );

}

/**
 * Creates given directory
 * @param dirname Directory to create
 * @return `true` if successful, `false` if not
 */
bool RsMakeDirectory (const string &dirname )
{
#ifdef WIN32
	return _mkdir ( dirname ) == 0;
#else
	return mkdir ( dirname.c_str(), 0700 ) == 0;
#endif
}

/**
 * Deletes given directory
 * @param dirname Directory to delete
 */
void RsDeleteDirectory (const string &dirname )
{

#ifdef WIN32
    _rmdir ( dirname );
#else
    rmdir( dirname.c_str() );
#endif

}

/**
 * Creates the temp directory
 * NOTE: The given `newapppath` **IS NOT** the created directory.
 * The created directory is in the form: `<newapppath>temp/`
 * @param newapppath Prefix for directory
 */
void RsInitialise (const string &newapppath )
{
    rsapppath = newapppath;

    // create a temp directory

#ifdef WIN32
    sprintf ( tempdir, "%stemp/", rsapppath );
    RsMakeDirectory ( tempdir );
#else
	// Try in the current directory

    tempdir = rsapppath + "temp/";
    if (!RsMakeDirectory ( tempdir )) {
		// Try in /tmp

		tempdir = "/tmp/uplink-XXXXXX";
		tempdir = mkdtemp((char *) tempdir.c_str());

		if (tempdir.empty()) {
			printf( "Failed to make temporary directory\n");
			abort();
		}

		tempdir += "/";
	}
#endif

	rsInitialised = true;

	// clean up, even on aborts
	atexit(RsCleanUp); 
}

/**
 * Deletes all files in the temp directory, and deletes the directory
 */
void RsCleanUp ()
{
	if (!rsInitialised) 
		return;

	rsInitialised = false;

    // Delete all files in the temp directory

#ifdef WIN32

	char searchstring [SIZE_RSFILENAME];
	sprintf ( searchstring, "%s*.*", tempdir );

	_finddata_t thisfile;
	intptr_t fileindex = _findfirst ( searchstring, &thisfile );

	int exitmeplease = 0;

	while ( fileindex != -1 && !exitmeplease ) {

        if ( thisfile.attrib & _A_NORMAL || thisfile.attrib & _A_ARCH ) {

            char fullfilename [SIZE_RSFILENAME];
            sprintf ( fullfilename, "%s%s", tempdir, thisfile.name );
            int result = remove ( fullfilename );

        }

        exitmeplease = _findnext ( fileindex, &thisfile );

	}

	if ( fileindex != -1 )
		_findclose ( fileindex );

#else
    {
	DIR *dir = opendir( tempdir.c_str() );
	if (dir != nullptr) {
	    struct dirent *entry = readdir ( dir );

	    while (entry != nullptr) {
                
			string fullfilename = tempdir + entry->d_name;
                
			remove ( fullfilename.c_str() );

    		entry = readdir ( dir );
	    
	    }
	  
	    closedir( dir );
	}
    }
#endif

    // Delete the temp directory

    RsDeleteDirectory ( tempdir );

	BglCloseAllFiles();

}


DArray <char *> *RsListArchive ( char *path, char *filter )
{

    DArray <char *> *result = BglListFiles ( rsapppath, path, filter );

    // Strip rsapppath from every result

    for ( int i = 0; i < result->Size(); ++i ) {
        if ( result->ValidIndex(i) ) {

            char *thisResult = result->GetData(i);
            char *newResult = new char [strlen(thisResult) - rsapppath.length() + 1];
            strcpy ( newResult, thisResult + rsapppath.length() );
            result->PutData( newResult, i );

        }
    }

    return result;

}
