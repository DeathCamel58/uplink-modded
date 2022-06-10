

/*
	Misc string/file utils

  */


#ifndef included_miscutils_h
#define included_miscutils_h

#include "tosser.h"

/**
 * Converts a string to lowercase
 * @param thestring The string to change
 * @return The string in all lowercase letters
 */
string LowerCaseString       (const string &thestring );

/**
 * Gets a substring of the string up to the first newline
 * @param thestring The string to substring
 * @return The substring up to the newline
 */
string StripCarriageReturns  (const string &thestring );
char *TrimSpaces            ( const char *thestring );

char *GetFilePath           ( const char *filename );

/**
 * Creates given directory
 * @param dirname Directory to create
 */
void MakeDirectory          (const string &dirname );

/**
 * Checks if given filename exists
 * @param filename File name to check
 * @return `true` if file exists
 */
bool DoesFileExist          (const string& filename );

/**
 * Deletes all files and folders within a directory while preserving directory
 * @param directory Directory to delete everything in
 */
void EmptyDirectory         (const string &directory );

/**
 * Copies file from one location to another
 * @param oldfilename File path to copy
 * @param newfilename File path to put copy
 * @return `true` on success
 */
bool CopyFilePlain          (const string &oldfilename, const string &newfilename );
bool CopyFileUplink         ( const char *oldfilename, const char *newfilename );

/**
 * Deletes a file
 * @param filename File name to delete
 */
void RemoveFile             (const string &filename );

DArray <char *> *ListDirectory  ( char *directory, char *filter );
DArray <char *> *ListSubdirs (const string &directory );

void SetColour              (const string &colourName );                    // calls glColour3f

void PrintStackTrace();

void PrintValue(const string& valuename, const string& value);
void PrintValue(const string& valuename, int value);
void PrintValue(int valuename, int value);
void PrintValue(const string& valuename, unsigned int value);
void PrintValue(const string& valuename, float value);
void PrintValue(const string& valuename, time_t value);

/**
 * Prints text centered with `=`
 * @param name Text to print
 */
void PrintPadded(const string& name);

#endif

