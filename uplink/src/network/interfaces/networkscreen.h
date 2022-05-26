
/*

	Network Screen base class

	All screens accessable as Network clients should be derived from this

  */

#ifndef included_networkscreen_h
#define included_networkscreen_h


class NetworkScreen
{

public:

	NetworkScreen();
	virtual ~NetworkScreen();

	virtual void Create ();
	virtual void Remove ();
	virtual void Update ();
	virtual bool IsVisible ();

	virtual int  ScreenID ();			

};

#endif
