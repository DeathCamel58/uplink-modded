

/*
	Misc string/file utils

  */


#ifndef _included_miscutils_h
#define _included_miscutils_h

#include "tosser.h"

string LowerCaseString       (const string &thestring );
string StripCarriageReturns  (const string &thestring );               // Replaces first cr with \x0
char *TrimSpaces            ( const char *thestring );

char *GetFilePath           ( const char *filename );
void MakeDirectory          (const string &dirname );
bool DoesFileExist          (const string& filename );
void EmptyDirectory         (const string &directory );
bool CopyFilePlain          (const string &oldfilename, const string &newfilename );
bool CopyFileUplink         ( const char *oldfilename, const char *newfilename );
bool RemoveFile             (const string &filename );

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

void PrintPadded(const string& name);

#endif

