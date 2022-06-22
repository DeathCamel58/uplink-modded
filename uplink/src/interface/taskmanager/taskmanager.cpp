
#include "app/app.h"
#include "app/globals.h"
#include "app/opengl.h"

#include "game/game.h"

#include "world/world.h"
#include "world/player.h"
#include "world/computer/securitysystem.h"

// === Include all Task objects here ==========================================

#include "interface/taskmanager/passwordbreaker.h"
#include "interface/taskmanager/dictionaryhacker.h"
#include "interface/taskmanager/tracetracker.h"
#include "interface/taskmanager/filecopier.h"
#include "interface/taskmanager/filedeleter.h"
#include "interface/taskmanager/defrag.h"
#include "interface/taskmanager/logdeleter.h"
#include "interface/taskmanager/logundeleter.h"
#include "interface/taskmanager/logmodifier.h"
#include "interface/taskmanager/decrypter.h"
#include "interface/taskmanager/iplookup.h"
#include "interface/taskmanager/gatewaynuke.h"
#include "interface/taskmanager/motionsensor.h"
#include "interface/taskmanager/proxydisable.h"
#include "interface/taskmanager/firewalldisable.h"
#include "interface/taskmanager/securitybypass.h"
#include "interface/taskmanager/ipprobe.h"
#include "interface/taskmanager/revelation.h"
#include "interface/taskmanager/tutorial.h"
#include "interface/taskmanager/decypher.h"
#include "interface/taskmanager/voiceanalyser.h"
#include "interface/taskmanager/revelationtracker.h"
#include "interface/taskmanager/uplinkagentlist.h"
#include "interface/taskmanager/faith.h"
#include "interface/taskmanager/lanscan.h"
#include "interface/taskmanager/lanprobe.h"
#include "interface/taskmanager/lanspoof.h"
#include "interface/taskmanager/lanforce.h"

// ============================================================================


#include "interface/interface.h"
#include "interface/remoteinterface/remoteinterface.h"
#include "interface/taskmanager/taskmanager.h"

#include "mmgr.h"


TaskManager::TaskManager ()
{
	
	SvbReset ();

	targetprogram = -1;
	targetprogramreason = 0;
	lasttargetprogram = -1;
	programtarget = nullptr;
	lastcycle = 0;

}

TaskManager::~TaskManager ()
= default;


void TaskManager::RunSoftware (const string &name, float version )
{

	UplinkTask *task = nullptr;

	// Create the new task

	if		( name == "Password_Breaker" ) {		task = new PasswordBreaker (); }
	else if ( name == "Dictionary_Hacker" ) {		task = new DictionaryHacker (); }
	else if ( name == "Trace_Tracker" ) {			task = new TraceTracker (); }
	else if ( name == "File_Copier" )	{			task = new FileCopier ();
                                                    task->SetFollowMouse ( true ); }
	else if ( name == "File_Deleter" ) {			task = new FileDeleter ();
                                                    task->SetFollowMouse ( true ); }
	else if ( name == "Defrag" )					task = new Defrag ();
	else if ( name == "Log_Deleter" )	{			task = new LogDeleter ();
                                                    task->SetFollowMouse ( true ); }
	else if ( name == "Log_UnDeleter" ) {			task = new LogUnDeleter ();
                                                    task->SetFollowMouse ( true ); }
	else if ( name == "Log_Modifier" ) {			task = new LogModifier ();
                                                    task->SetFollowMouse ( true ); }
	else if ( name == "Decrypter" ) {				task = new Decrypter ();
                                                    task->SetFollowMouse ( true ); }
	else if ( name == "IP_Lookup" ) {				task = new IPLookup (); }
	else if ( name == "Gateway_Nuke" ) {			task = new GatewayNuke (); }
	else if ( name == "Motion_Sensor" ) {			task = new MotionSensor (); }
	else if ( name == "Proxy_Disable" ) {			task = new ProxyDisable (); }
	else if ( name == "Firewall_Disable" ) {		task = new FirewallDisable (); }
	else if ( name == "IP_Probe" ) {				task = new IPProbe (); }
	else if ( name == "Proxy_Bypass" ) {			task = new SecurityBypass ( SECURITY_TYPE_PROXY );
                                                    task->SetFollowMouse ( true ); }
	else if ( name == "Firewall_Bypass" ) {		    task = new SecurityBypass ( SECURITY_TYPE_FIREWALL );
                                                    task->SetFollowMouse ( true ); }
	else if ( name == "Encryption_Bypass" ) {		task = new SecurityBypass ( SECURITY_TYPE_ENCRYPTION );
                                                    task->SetFollowMouse ( true ); }
	else if ( name == "Monitor_Bypass" ) {		    task = new SecurityBypass ( SECURITY_TYPE_MONITOR );
                                                    task->SetFollowMouse ( true ); }
	else if ( name == "Revelation" ) {			    task = new Revelation (); }
	else if ( name == "Tutorial" ) {				task = new Tutorial (); }
	else if ( name == "Decypher" ) {				task = new Decypher ();
                                                    task->SetFollowMouse ( true ); }
	else if ( name == "Voice_Analyser" ) {		    task = new VoiceAnalyser (); }
    else if ( name == "Revelation_Tracker" ) {      task = new RevelationTracker ();
                                                    task->SetFollowMouse ( true ); }
    else if ( name == "Uplink_Agent_List" )         task = new UplinkAgentList ();
    else if ( name == "Faith" )                     task = new Faith ();
	else if ( name == "LAN_Scan" )				    task = new LanScan ();
	else if ( name == "LAN_Probe" ) {				task = new LanProbe ();
                                                    task->SetFollowMouse ( true ); }
    else if ( name == "LAN_Spoof" ) {               task = new LanSpoof ();
                                                    task->SetFollowMouse ( true ); }
    else if ( name == "LAN_Force" ) {               task = new LanForce ();
                                                    task->SetFollowMouse ( true ); }

	
	else {
		cout << "Task Manager warning : Called RunSoftware, name not recognised '" << name << "'" << endl;
		return;
	}

	// Run the new task

	task->SetVersion ( version );
	int pid = SvbRegisterTask ( name, task );
	SetTargetProgram ( pid );

}


void TaskManager::SetProgramTarget (UplinkObject *newprogramtarget, const string &targetstring, int targetint )
{

	programtarget = newprogramtarget;

	if ( programtarget && (targetprogram != -1) ) {

		// Link the program to the target
		auto *task = (UplinkTask *) SvbGetTask ( targetprogram );

		if ( task )
			task->SetTarget ( programtarget, targetstring, targetint );

		targetprogramreason = 1;
		lasttargetprogram = targetprogram;
		targetprogram = -1;
		programtarget = nullptr;

	}

}

void TaskManager::SetTargetProgram ( int newtargetprogram )
{

	if ( newtargetprogram != -1 )
		targetprogramreason = 0;
	lasttargetprogram = targetprogram;
	targetprogram = newtargetprogram;

}

bool TaskManager::IsTargetProgramLast ( int newtargetprogram ) const
{

	return ( targetprogramreason != 0 && targetprogram == -1 && newtargetprogram == lasttargetprogram );

}

UplinkTask *TaskManager::GetTargetProgram () const
{

	if ( targetprogram != -1 ) {
		auto *task = (UplinkTask *) SvbGetTask ( targetprogram );
		return task;
	}
	else 
        return nullptr;

}

string TaskManager::GetTargetProgramName () const
{
    
    if ( targetprogram != -1 ) {
        
        TaskWrapper *tw = SvbGetTaskWrapper ( targetprogram );
        UplinkAssert (tw)

        return tw->name;

    }
    else 
        return "";

}

bool TaskManager::Load ( FILE *file )
{
	
	LoadID ( file );
	LoadID_END ( file );

	return true;

}

void TaskManager::Save ( FILE *file )
{

	SaveID ( file );
	SaveID_END ( file );

}

void TaskManager::Print ()
{

	cout << "Task Manager" << endl;

}

void TaskManager::Update ()
{

	int numcyclespersecond = 10;

	//
	// Cycle a few times a second
	//

	int timesincelastcycle = (int) ( EclGetAccurateTime () - lastcycle );

	if ( timesincelastcycle >= 1000 / numcyclespersecond ) {

		int cpuspeed = game->GetWorld ()->GetPlayer ()->gateway.GetCPUSpeed ();
		cpuspeed /= numcyclespersecond;
		
		SvbCycle ( cpuspeed );

		lastcycle = (int) EclGetAccurateTime ();

	}
	
	//
	// Move the current software app to the mouse position
	//

	UplinkTask *task = GetTargetProgram ();
	if ( task && task->followmouse )
		task->MoveTo ( get_mouseX () + 3, get_mouseY (), 0 );

}

string TaskManager::GetID ()
{

	return "TASKMAN";

}
