// -*- tab-width:4 c-file-style:"cc-mode" -*-

//#include "stdafx.h"

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

#include <GL/gl.h>

#include <GL/glu.h> /* glu extention library */
#include <fstream>
#include <algorithm>
#include <filesystem>

#include "redshirt.h"

#include "gucci.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/miscutils.h"

#include "options/options.h"

#include "mmgr.h"


char *GetFilePath ( const char *filename )
{

	UplinkAssert (filename)

	size_t newstringsize = strlen (filename) + 1;
    char *newstring = new char [newstringsize];
    UplinkStrncpy ( newstring, filename, newstringsize )
/*    
    char *p = newstring;
    
    while ( strchr ( p, '/' ) )	
		p = strchr ( p, '/' ) + 1;
	    
    if (p) *p = '\x0';
  */
    
    char     *p = strrchr ( newstring, '/' );
    if ( !p ) p = strrchr ( newstring, '\\' );

    if ( p ) *(p+1) = '\x0';
    else     UplinkStrncpy ( newstring, "./", newstringsize )

    return newstring;    
    
}

string LowerCaseString (const string &thestring )
{

	assert(!thestring.empty());
	
	string thecopy = thestring;

    transform(thecopy.begin(), thecopy.end(), thecopy.begin(), ::tolower);

	return thecopy;

}

/**
 * 
 * @param thestring
 * @return
 */
string StripCarriageReturns (const string &thestring )
{

    assert(!thestring.empty());

    // get location of carriage return
    size_t pos = thestring.find('\n');

    if ( pos == string::npos ) {

        // No cr found - return string
        return thestring;

    }
    else {

        // Found a cr - so shorten string to that point
        string result = thestring.substr(0, pos);
        return result;

    }

}

char *TrimSpaces ( const char *thestring )
{

    UplinkAssert (thestring)

	char *retstring;

	int indexfirstnospace = -1;
	int indexlastnospace = -1;

	for ( int i = 0; thestring [ i ] != '\0'; i++ ) {
		if ( indexlastnospace == -1 )
			indexfirstnospace = i;
		if ( thestring [ i ] != ' ' )
			indexlastnospace = i;
	}

	int lenretstring;
	if ( indexfirstnospace == -1 || indexlastnospace == -1 )
		lenretstring = 0;
	else
		lenretstring = ( indexlastnospace - indexfirstnospace ) + 1;

	retstring = new char [ lenretstring + 1 ];
	if ( lenretstring > 0 )
		memcpy ( retstring, thestring + indexfirstnospace, lenretstring );
	retstring [ lenretstring ] = '\0';

	return retstring;

}

void MakeDirectory (const string &dirname )
{

#ifdef WIN32
  _mkdir ( dirname.c_str() );
#else
  mkdir ( dirname.c_str(), 0700 );
#endif

}


bool DoesFileExist ( const char *filename )
{

    return access(filename, 0) == 0;

}

void EmptyDirectory (const string &directory )
{

    if (filesystem::exists(directory)) {
        for (const auto &entry : filesystem::directory_iterator(directory)) {
            filesystem::remove_all(entry.path());
        }
    }

}

bool CopyFilePlain (const string &oldfilename, const string &newfilename )
{

	bool success = false;
	ifstream src(oldfilename, ios::binary);
	ofstream dst(newfilename, ios::binary);

	if (!src.is_open() || !src.is_open()) {
	    src.close();
	    dst.close();
        return false;
    }

	dst << src.rdbuf();

	src.close();
	dst.close();

	return true;

}

bool CopyFileUplink ( const char *oldfilename, const char *newfilename )
{

	bool success = false;
	FILE *fileread = RsFileOpen ( const_cast<char *>( oldfilename ) );
	FILE *filewrite = fopen ( newfilename, "wb" );

	if ( fileread && filewrite ) {
		char buffer [256];
		size_t sizeread;

		while ( ( sizeread = fread ( buffer, 1, sizeof ( buffer ), fileread ) ) > 0 ) {
			fwrite ( buffer, 1, sizeread, filewrite );
		}

		success = true;
	}

	if ( filewrite )
		fclose ( filewrite );
	if ( fileread )
		RsFileClose ( const_cast<char *>( oldfilename ), fileread );

	return success;

}

bool RemoveFile (const string &filename )
{

#ifdef WIN32
	return ( _unlink ( filename.c_str() ) == 0 );
#else
	return ( unlink ( filename.c_str() ) == 0 );
#endif

}

DArray <char *> *ListDirectory  ( char *directory, char *filter )
{

    //
    // Start with a listing from Redshirt archives

    DArray <char *> *result = RsListArchive ( directory, filter );

    //
    // Now add on all files found locally

#ifdef WIN32

	char searchstring [_MAX_PATH + 1];
	UplinkSnprintf ( searchstring, sizeof ( searchstring ), "%s%s*%s", app->path, directory, filter ); 

	_finddata_t thisfile;
	intptr_t fileindex = _findfirst ( searchstring, &thisfile );

	int exitmeplease = 0;

	while ( fileindex != -1 && !exitmeplease ) {

		size_t newnamesize = _MAX_PATH + 1;
		char *newname = new char [newnamesize];
		UplinkSnprintf ( newname, newnamesize, "%s%s", directory, thisfile.name );      
        result->PutData ( newname );
		exitmeplease = _findnext ( fileindex, &thisfile );

	}

	if ( fileindex != -1 )
		_findclose ( fileindex );

#else

	string userdir = app->path + directory;
	DIR *dir = opendir( userdir.c_str() );
	if (dir != nullptr) {
	    struct dirent *entry = readdir ( dir );

	    while (entry != nullptr) {
	    
		    char *p = strstr(entry->d_name, filter);
		    if ( p ) {
				size_t newnamesize = 256;
		        char *newname = new char [newnamesize];
		        UplinkSnprintf ( newname, newnamesize, "%s%s", directory, entry->d_name )
		        result->PutData ( newname );
		    }
	        
		    entry = readdir ( dir );
	    
	    }
	  
	    closedir( dir );
	}
#endif

    //
    // Now make absolutely sure there are no duplicates

    for ( int i = 0; i < result->Size(); ++i ) {
        if ( result->ValidIndex(i) ) {

            for ( int j = i + 1; j < result->Size(); ++j ) {
                if ( result->ValidIndex(j) ) {

                    char *resultI = result->GetData(i);
                    char *resultJ = result->GetData(j);

					if ( strcmp ( resultI, resultJ ) == 0 ) {
						delete [] resultJ;
                        result->RemoveData( j );
					}

                }
            }

        }
    }

    //
    // All done

    return result;

}

DArray <char *> *ListSubdirs (const string &directory )
{

    auto *result = new DArray <char *> ();

    //
    // Now add on all files found locally

#ifdef WIN32

	_finddata_t thisfile;
	char filter[256];
	UplinkSnprintf ( filter, sizeof ( filter ), "%s*.*", directory );
	intptr_t fileindex = _findfirst ( filter, &thisfile );

	int exitmeplease = 0;

	while ( fileindex != -1 && !exitmeplease ) {
		if ( thisfile.attrib & _A_SUBDIR &&
			 strcmp ( thisfile.name, "." ) != 0 &&
			 strcmp ( thisfile.name, ".." ) != 0 ) {		

			size_t newnamesize = 256;
			char *newname = new char [newnamesize];
			UplinkStrncpy ( newname, thisfile.name, newnamesize );
			result->PutData ( newname );

		}
		exitmeplease = _findnext ( fileindex, &thisfile );
	}

	if ( fileindex != -1 )
		_findclose ( fileindex );

#else

	DIR *dir = opendir(directory.c_str());
	if (dir != nullptr) {
		for (struct dirent *d; (d = readdir(dir)) != nullptr;) {
			char fullfilename[256];
			struct stat info;

			UplinkSnprintf(fullfilename, sizeof ( fullfilename ), "%s/%s", directory.c_str(), d->d_name)
			
			if (stat(fullfilename, &info) == 0 
				&& S_ISDIR(info.st_mode) 
				&& d->d_name[0] != '.') { 
					char *newname = new char [strlen(d->d_name) + 1];
					UplinkSafeStrcpy(newname, d->d_name)
					result->PutData( newname );
			}
		}
		closedir(dir);
	}
#endif

	return result;

}

void SetColour (const string &colourName )
{

    if ( !app || 
         !app->GetOptions () || 
         !app->GetOptions()->GetColour( colourName ) ) {

        cout << "SetColour WARNING : Failed to find colour " << colourName << endl;
        glColor3f ( 0.0f, 0.0f, 0.0f );
        return;

    }

    ColourOption *col = app->GetOptions ()->GetColour ( colourName );
    UplinkAssert (col)
    glColor3f ( col->r, col->g, col->b );

}


unsigned *getRetAddress(unsigned *mBP)
{
#ifdef WIN32
	unsigned *retAddr;

	__asm {
		mov eax, [mBP]
		mov eax, ss:[eax+4];
		mov [retAddr], eax
	}

	return retAddr;
#else
	auto **p = (unsigned **) mBP;
	return p[1];
#endif
}

void PrintStackTrace()
{

	// Get our frame pointer, chain upwards
	unsigned *framePtr;
    unsigned *previousFramePtr = nullptr;

    
//#ifdef WIN32
//	__asm {
//		mov [framePtr], ebp
//	}
//#else
//	asm (
//	    "movl %%ebp, %0;"
//	    :"=r"(framePtr)
//	    );
//#endif
	while(framePtr) {
		                
		cout <<"retAddress = " << getRetAddress(framePtr) << endl;
		framePtr = *(unsigned **)framePtr;

	    // Frame pointer must be aligned on a
	    // DWORD boundary.  Bail if not so.
	    if ( (unsigned long) framePtr & 3 )   
		break;                    

        if ( framePtr <= previousFramePtr )
            break;

        // Can two DWORDs be read from the supposed frame address?          
#ifdef WIN32
        if ( IsBadWritePtr(framePtr, sizeof(PVOID)*2) ||
             IsBadReadPtr(framePtr, sizeof(PVOID)*2) )
            break;
#endif

        previousFramePtr = framePtr;
    
    }
	

}

void PrintValue(const string& valuename, const string& value) {
    cout << "=== USED THE PRINTVALUE FUNCTION ===" << endl;
    cout << "\t" << valuename << ": " << value << endl;
}

void PrintValue(const string& valuename, int value) {
    cout << "=== USED THE PRINTVALUE FUNCTION ===" << endl;
    cout << "\t" << valuename << ": " << value << endl;
}

void PrintValue(int valuename, int value) {
    cout << "=== USED THE PRINTVALUE FUNCTION ===" << endl;
    cout << "\t" << valuename << ": " << value << endl;
}

void PrintValue(const string& valuename, unsigned int value) {
    cout << "=== USED THE PRINTVALUE FUNCTION ===" << endl;
    cout << "\t" << valuename << ": " << value << endl;
}

void PrintValue(const string& valuename, float value) {
    cout << "=== USED THE PRINTVALUE FUNCTION ===" << endl;
    cout << "\t" << valuename << ": " << value << endl;
}

void PrintValue(const string& valuename, time_t value) {
    cout << "=== USED THE PRINTVALUE FUNCTION ===" << endl;
    cout << "\t" << valuename << ": " << value << endl;
}

void PrintPadded(const string& name) {
    int pad = (60 - name.length()) / 2 - 1;
    string out;
    for (int i = 0; i<pad; i++) {
        out += "=";
    }
    out += " " + name + " ";
    for (int i = 0; i<pad; i++) {
        out += "=";
    }
    cout << out << endl;
}
