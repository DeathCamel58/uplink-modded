
/*

    Demo Plot Generator

        Only one instance occurs, and only in the demo
        Responsible for ensuring there is always something
        interesting to do in the demo

  */


#ifndef _included_demoplotgenerator_h
#define _included_demoplotgenerator_h


#include "app/uplinkobject.h"


class DemoPlotGenerator : public UplinkObject 
{

protected:

    int scene;
    char arcmissiontarget [SIZE_PERSON_NAME];

protected:

    static void MoveNewMissionToRandomLocation ( Mission *mission );
    
    static bool NumAvailableMissions ( int missiontype );                      // Counts number available to player

    static Mission *Generate_FileCopyMission ();
    static Mission *Generate_FileDeleteMission ();
    static Mission *Generate_ChangeAcademicMission ();
    Mission *Generate_ChangeSocialRecordARC ();

public:

    DemoPlotGenerator ();
    ~DemoPlotGenerator () override;

    void Initialise ();

    void PlayerRatingChange ();
    void PlayerCompletedMission ( Mission *mission );

    static void GenerateNewMission ( bool randomdate = false );                // Occurs every few hours

   	bool PlayerContactsARC ( Message *msg );					        // These handle all emails from the player to the main co's.  

    void RunScene ( int scene );


	// 
	// Common functions

	bool Load  ( FILE *file ) override;
	void Save  ( FILE *file ) override;
	void Print () override;
	void Update () override;
	
	string GetID () override;

};


#endif

