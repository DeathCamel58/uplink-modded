
#include <cstdio>

#include "sgplaylist.h"

#include "mmgr.h"



SgPlaylist::SgPlaylist ()
{

    sprintf ( name, "New playlist" );
            
}

SgPlaylist::~SgPlaylist ()
{

	for ( int i = 0; i < songs.Size (); ++i ) 
		if ( songs.GetData (i) )
			if ( strlen(songs.GetData (i)) != 0 )
				if ( strcmp(songs.GetData (i), "") != 0 )
					delete [] songs.GetData (i);

}


void SgPlaylist::SetName (const string &newname )
{

    strcpy ( name, newname.c_str() );

}

void SgPlaylist::AddSong (const string &name )
{

    char *namecopy = new char [name.length()+1];
    sprintf ( namecopy, name.c_str() );
    songs.PutData(namecopy);

}

int SgPlaylist::NumSongs ()
{
    
    return songs.Size ();

}

char *SgPlaylist::GetRandomSong ()
{

  	int songindex = (int)( ( (float) rand () / (float) RAND_MAX ) * NumSongs () );
    return songs.GetData(songindex);

}

char *SgPlaylist::GetRandomSong (const string &oldsong )
{

    if ( oldsong.empty() ) {

        return GetRandomSong ();

    }
    else if ( NumSongs () == 1 ) {

        return songs.GetData (0);

    }
    else {

        char *candidate = GetRandomSong ();
        while ( candidate == oldsong )
            candidate = GetRandomSong ();

        return candidate;

    }

}
