// -*- tab-width:4; c-file-mode:"cc-mode" -*-

/*

  Options class object

	Part of the GAME subsystem
	Stores all of the options specified by the user.

	This class is different in that the Load/Save methods don't write to the 
	specified file - they write to their own options file.

  */

#ifndef included_options_h
#define included_options_h

#include "app/globals.h"
#include "app/uplinkobject.h"


class Option;
class OptionChange;
class ColourOption;


class Options : public UplinkObject  
{

protected:

	BTree <Option *> options;
    LList <OptionChange *> shutdownChanges;

    string themeName;
    string themeAuthor;
    string themeTitle;
    string themeDescription;
    BTree <ColourOption *> colours;

public:

	Options();
	~Options() override;

	void CreateDefaultOptions ();													// Fills in all options not yet created


	Option *GetOption (const string &name );												// Returns NULL if not found
	int     GetOptionValue (const string &name );											// asserts existence of name
	bool    IsOptionEqualTo (const string &name, int value );
	
	void    SetOptionValue (const string &name, int newvalue );							// Does not create new
	void	SetOptionValue (const string &name, int newvalue, const string &tooltip,
                            bool yesorno = false, bool visible = true );			// Creates new if neccisary

	LList <Option *> *GetAllOptions ( char *searchstring, bool returnhidden );		// String can be NULL - will return ALL

    void SetThemeName (string newThemeName);
    string GetThemeName ();
    string GetThemeTitle ();
    string GetThemeDescription ();
    
    ColourOption *GetColour     (const string &colourName );                                   // Always safe - returns BLACK if not found

    string ThemeFilename (const string &filename );

    void RequestShutdownChange (string optionName, int newValue );
    void ApplyShutdownChanges ();

	// Common functions

	bool Load   ( FILE *file ) override;														// Does not actually save to file
	void Save   ( FILE *file ) override;														// "" ""
	void Print  () override;
	void Update () override;
	string GetID () override;

};

// ============================================================================


#define SIZE_OPTION_NAME		64
#define SIZE_OPTION_TOOLTIP		128


class Option : public UplinkObject
{

public:

	char name [SIZE_OPTION_NAME];
	char tooltip [SIZE_OPTION_TOOLTIP];

	bool yesorno;										// Is this a boolean option
	bool visible;										// Is this a hidden option
	int value;

public:

	Option();
	~Option() override;

	void SetName    (const string &newname );
	void SetTooltip (const string &newtooltip );
	void SetYesOrNo	( bool newyesorno );
	void SetVisible ( bool newvisible );
	void SetValue   ( int newvalue );


	// Common functions

	bool Load   ( FILE *file ) override;
	void Save   ( FILE *file ) override;
	void Print  () override;
	void Update () override;
	string GetID () override;
	int  GetOBJECTID () override;

};


class OptionChange
{
public:
    OptionChange () = default;;

    string name;
    int value;
};

class ColourOption
{

public:

    ColourOption ( float _r, float _g, float _b )
        : r ( _r ), g ( _g ), b ( _b ) {}

    float r;
    float g;
    float b;

};

#endif 
