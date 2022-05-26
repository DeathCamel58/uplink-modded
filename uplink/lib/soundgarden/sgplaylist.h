
/*

  SgPlaylist class object

    Stores a list of song names 
    to be played for a particular mood

  */


#ifndef included_sgplaylist_h
#define included_sgplaylist_h

#define SIZE_SGPLAYLIST_NAME 64

#include "tosser.h"

//#ifdef _DEBUG
//#include "slasher.h"
//#endif


class SgPlaylist
{

public:

    string name;
    LList <string> songs;

public:

    SgPlaylist ();
    ~SgPlaylist ();

    void SetName (const string &newname );
    void AddSong (const string &name );

    int NumSongs ();
    string GetRandomSong ();
    string GetRandomSong (const string &oldsong );                      // Any song but not the old one
                                                                // Warning - infinite loop if all songs have same name
};



#endif

