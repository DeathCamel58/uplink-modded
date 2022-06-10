
#ifndef included_uplinkagentlist_h
#define included_uplinkagentlist_h


#include "vanbakel.h"

#include "interface/taskmanager/uplinktask.h"



class UplinkAgentList : public UplinkTask  
{


protected:

	static void UplinkAgentListDraw ( Button *button, bool highlighted, bool clicked );	
	
    static void ScrollUpClick ( Button *button );
    static void ScrollDownClick ( Button *button );
    static void TitleClick ( Button *button );

    /**
     * Closes the program
     * @param button
     * @note button is unused
     */
    static void CloseClick ( Button *button );

protected:

    DArray <char *> handles;
    DArray <char *> names;

    static int baseoffset;

public:

	UplinkAgentList();
	virtual ~UplinkAgentList();

    void UpdateLists ();

    /**
     * Does nothing
     * @param uo
     * @param uos
     * @param uoi
     * @note uo, uos, and uoi are unused
     */
	void SetTarget (UplinkObject *uo, const string &uos, int uoi );

	void Initialise ();     
	void Tick ( int n );    

	void MoveTo ( int x, int y, int time_ms );			
    
	void CreateInterface ();       
	void RemoveInterface ();
	void ShowInterface ();

	bool IsInterfaceVisible ();     

};

#endif

