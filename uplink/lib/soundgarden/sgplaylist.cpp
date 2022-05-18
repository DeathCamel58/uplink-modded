
#include "sgplaylist.h"

#include "mmgr.h"



SgPlaylist::SgPlaylist ()
{

    name = "New playlist";
            
}

SgPlaylist::~SgPlaylist ()
{

    songs.Empty();

}


void SgPlaylist::SetName (const string &newname )
{

    name = newname;

}

void SgPlaylist::AddSong (const string &name )
{

    const string& namecopy = name;
    songs.PutData(namecopy);

}

int SgPlaylist::NumSongs ()
{
    
    return songs.Size ();

}

string SgPlaylist::GetRandomSong ()
{

  	int songindex = (int)( ( (float) rand () / (float) RAND_MAX ) * NumSongs () );
    return songs.GetData(songindex);

}

string SgPlaylist::GetRandomSong (const string &oldsong )
{

    if ( oldsong.empty() ) {

        return GetRandomSong ();

    }
    else if ( NumSongs () == 1 ) {

        return songs.GetData (0);

    }
    else {

        string candidate = GetRandomSong ();
        while ( candidate == oldsong )
            candidate = GetRandomSong ();

        return candidate;

    }

}
