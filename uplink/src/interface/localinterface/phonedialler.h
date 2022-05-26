
#ifndef included_dialler_h
#define included_dialler_h


#include "interface/localinterface/localinterfacescreen.h"


class PhoneDialler : public LocalInterfaceScreen
{

protected:

	static char buttonnames [][9];

	char *ip;
	char *infosup;

	int nextscene;

	int lastupdate;
	int curnumber;

	void UpdateDisplay ();

public:

    PhoneDialler ();
    ~PhoneDialler () override;

	void Create ( int x, int y );
	void Remove () override;
	bool UpdateSpecial ();
	bool IsVisible () override;

	void DialNumber ( int x, int y, char *number, int scenario, char *info = nullptr );

};

#endif

