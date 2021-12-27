
/*

  Revelation

	THE virus ;)

  */


#include "interface/taskmanager/uplinktask.h"


class Revelation : public UplinkTask
{

protected:

	time_t timesync;
    int animationtime;
    int nextnumber;
    int numcopies;

protected:

    static void DrawRevelation ( Button *button, bool highlighted, bool clicked );

public:

	void Initialise () override;
	void Tick ( int n ) override;

	void CreateInterface () override;
	void RemoveInterface () override;
	void ShowInterface () override;

	bool IsInterfaceVisible () override;

};
