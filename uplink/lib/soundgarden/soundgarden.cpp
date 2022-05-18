
/*

  Platform independant Soundgarden functions

  */


#include "soundgarden.h"
#include "sgplaylist.h"
#include "tosser.h"
#include "redshirt.h"
#include "eclipse.h"

#include "mmgr.h"


static LList <SgPlaylist *> playlists;
static string currentplaylist;
static string currentsong;

static string requestedplaylist;
static int requestedtime;

static int songindex;


void SgPlaylist_Initialise ()
{

    currentplaylist = currentsong = requestedplaylist = "None";
    requestedtime = -1;
    songindex = -1;

}

void SgPlaylist_Shutdown ()
{

    while ( playlists.GetData (0) ) {

        SgPlaylist *playlist = playlists.GetData (0);
        playlists.RemoveData (0);
        delete playlist;

    }

    playlists.Empty ();

}

void SgPlaylist_Create (const string &pname )
{

    auto *playlist = new SgPlaylist ();
    playlist->SetName ( pname );
    playlists.PutData ( playlist );

}

SgPlaylist *SgPlaylist_GetPlaylist (const string &pname )
{

    for ( int i = 0; i < playlists.Size (); ++i ) 
        if ( playlists.GetData (i) )            
            if ( playlists.GetData (i)->name == pname )
                return playlists.GetData(i);

    return nullptr;

}

void SgPlaylist_AddSong (const string &pname, const string &songname )
{

    SgPlaylist *playlist = SgPlaylist_GetPlaylist(pname);

    if ( playlist ) 
        playlist->AddSong (songname);

    else
        cout << "SgPlaylist_AddSong : Failed because playlist " << pname << " does not exist" << endl;

}

void SgPlaylist_Play (const string &pname )
{

    if ( currentplaylist == "None" ) {

        // Nothing is currently playing
        // Start the new playlist immediately

        currentplaylist = pname;
//        SgSetModVolume ( 100 );
        songindex = -1;
        SgPlaylist_NextSong ();

    }
    else if ( currentplaylist != pname ) {

        // Something is currently playing
        // Fade it out slowly before changing

        if ( requestedplaylist == "None" )
            requestedtime = (int)EclGetAccurateTime ();

        requestedplaylist = pname;
            
    }

}

void SgPlaylist_Stop ()
{

    SgStopMod ();
    currentplaylist = currentsong = requestedplaylist = "None";
    requestedtime = -1;
    songindex = -1;

}

void SgPlaylist_NextSong ()
{

    SgPlaylist *playlist = SgPlaylist_GetPlaylist(currentplaylist);

    if ( playlist ) {

        ++songindex;
        if ( songindex >= playlist->songs.Size() ) songindex = 0;
        string songtitle = playlist->songs.GetData(songindex);

        SgPlayMod ( RsArchiveFileOpen ( songtitle ) );
        currentsong = songtitle;

    } else {
        cout << "SgPlaylist_NextSong : Failed because playlist " << currentplaylist << " does not exist" << endl;
    }

}

void SgPlaylist_RandomSong ()
{

    SgPlaylist *playlist = SgPlaylist_GetPlaylist(currentplaylist);

    if ( playlist ) {

        string songtitle = playlist->GetRandomSong ( currentsong );
        SgPlayMod ( RsArchiveFileOpen ( songtitle ) );
        currentsong = songtitle;

    }
    else
        cout << "SgPlaylist_RandomSong : Failed because playlist " << currentplaylist << " does not exist" << endl;

}

void SgUpdate ()
{

    // Are we fading out the current playlist?

    if ( requestedplaylist != "None" ) {

        int timediff = (int)EclGetAccurateTime () - requestedtime;
        int volume = 20 - (int)(20 * ((float) timediff / (float) 4000));

        if ( volume <= 0 ) {
            SgStopMod ();
            currentplaylist = requestedplaylist;
            requestedplaylist = "None";
            requestedtime = -1;
            songindex = -1;
            SgSetModVolume ( 20 );
            SgPlaylist_NextSong ();
        }
        else {
            SgSetModVolume ( volume );
        }

    }
    else {

        // Playing a playlist as usual
        // Has the current mod finished?

        if ( currentplaylist != "None" )
            if ( SgModFinished () ) 
                SgPlaylist_NextSong ();

    }

}
