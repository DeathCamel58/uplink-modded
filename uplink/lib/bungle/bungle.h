
/*

  B U N G L E 

	Zip file access library
	By Christopher Delay

  */


#ifndef included_bungle_h
#define included_bungle_h

#include <cstdio>

#include "tosser.h"

//#ifdef _DEBUG
//#include "slasher.h"
//#endif


bool BglOpenZipFile (const string &zipfile, const string &apppath, const string &id = "" );
bool BglOpenZipFile (FILE *file, const string &apppath, const string &id = "" );

bool BglFileLoaded  (const string &filename );
bool BglExtractFile (const string &filename, const string &target = "" );

void BglCloseZipFile (const string &id );

void BglExtractAllFiles (const string &zipfile );

DArray <char *> *BglListFiles (const string &path, const string &directory = "", const string &filter = "" );

void BglCloseAllFiles();

#endif
