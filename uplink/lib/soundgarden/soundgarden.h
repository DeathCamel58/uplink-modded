
/*
		S O U N D G A R D E N 

			Sound library
		 By Christopher Delay
		 and John Knottenbelt

  */

#ifndef _included_soundgarden_h
#define _included_soundgarden_h

#include <cstdio>
#include <string>

//#ifdef _DEBUG
//#include "slasher.h"
//#endif


// ===========================================================================
// Core interface
// Must be implemented for each platform
//


void SgInitialise ();
void SgShutdown ();
							
void SgPlaySound (const std::string &fullfilename, const std::string &id = "",
                  bool synchronised = true );				// id must be unique to each sample.
															// If NULL, filename is used.

void SgPlayMod   (const std::string &fullfilename );
void SgSetModVolume ( int newvolume );						// 0 = silent, 100 = full
int  SgGetModVolume ();
bool SgModFinished ();
void SgStopMod   ();


// ===========================================================================
// Platform independent extensions
// (soundgarden.cpp)
//


void SgPlaylist_Initialise ();
void SgPlaylist_Shutdown ();
void SgPlaylist_Create (const std::string &pname );
void SgPlaylist_AddSong (const std::string &pname, char *songname );
void SgPlaylist_Play (const std::string &pname );
void SgPlaylist_Stop ();
void SgPlaylist_NextSong ();
void SgPlaylist_RandomSong ();

void SgUpdate ();

#endif
