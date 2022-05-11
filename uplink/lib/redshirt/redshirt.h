
/*

  RedShirt library

	Designed to encrypt and decrypt sensitive files,
	such as save games or data files

  */


#ifndef _included_redshirt_h
#define _included_redshirt_h

#include <cstdio>

#include "tosser.h"

//#ifdef _DEBUG
//#include "slasher.h"
//#endif


// ============================================================
// Basic routines =============================================


void RsInitialise (const string &newapppath );                                            // Creates a temp directory
void RsCleanUp ();																// Deletes all created files



// ============================================================
// Encryption routines ========================================



bool RsFileExists		(const string &filename );
bool RsFileEncrypted	(const string &filename );
bool RsFileEncryptedNoVerify (const string &filename );


bool RsEncryptFile		(const string &filename );						// Overwrites origional with encrypted
bool RsDecryptFile		(const string &filename );						// Overwrites origional with decrypted


FILE *RsFileOpen		(const string &filename, const string &mode = "rb" );		// preserves origional
void  RsFileClose		(const string &filename, FILE *file );



// ============================================================
// Archive file routines ======================================


bool RsLoadArchive			(const string &filename );

FILE *RsArchiveFileOpen		(const string &filename, const string &mode );		      // Looks for file apppath/filename
string RsArchiveFileOpen		(const string &filename );					      // Opens from filename first, then from zip file
bool RsArchiveFileLoaded	( char *filename );

void RsArchiveFileClose		(const string &filename, FILE *file = nullptr );

void RsCloseArchive			( char *filename );									// Frees all memory (how nice)

DArray <char *> *RsListArchive ( char *path = nullptr, char *filter = nullptr );

#endif
