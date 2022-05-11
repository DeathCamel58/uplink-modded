
/*

  B U N G L E 

	Zip file access library
	By Christopher Delay

  */


#ifndef _included_bungle_h
#define _included_bungle_h

#include <cstdio>

#include "tosser.h"

//#ifdef _DEBUG
//#include "slasher.h"
//#endif


bool BglOpenZipFile (const string &zipfile, const string &apppath, const string &id = nullptr );
bool BglOpenZipFile (FILE *file, const string &apppath, const string &id = nullptr );

bool BglFileLoaded  (const string &filename );
bool BglExtractFile (const string &filename, const string &target = nullptr );

void BglCloseZipFile (const string &id );

void BglExtractAllFiles (const string &zipfile );

DArray <char *> *BglListFiles (const string &path, const string &directory = nullptr, const string &filter = nullptr );

void BglCloseAllFiles();

#endif
