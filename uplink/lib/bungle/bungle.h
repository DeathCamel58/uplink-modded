
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


bool BglOpenZipFile ( char *zipfile, char *apppath, char *id = nullptr );
bool BglOpenZipFile ( FILE *zipfile, char *apppath, char *id = nullptr );

bool BglFileLoaded  ( char *filename );
bool BglExtractFile ( char *filename, char *target = nullptr );

void BglCloseZipFile ( char *id );

void BglExtractAllFiles ( char *zipfile );

DArray <char *> *BglListFiles ( char *path, char *directory = nullptr, char *filter = nullptr );

void BglCloseAllFiles();

#endif
