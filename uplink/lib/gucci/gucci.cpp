#ifdef WIN32
#include <windows.h>
#endif

#include <cstdio>

#include <GL/gl.h>

//#define USE_FTGL
//#define USE_GLTT

#include <map>

#include "gucci.h"
#include "gucci_internal.h"
#include "tosser.h"

#ifdef USE_FTGL
#  ifdef WIN32
#include <FTFace.h>
#include <FTGLBitmapFont.h>
//#include <FTGLPixmapFont.h>
#  else
#include <FTFace.h>
#  endif

static std::map<int, FTGLBitmapFont *> fonts;
//static std::map<int, FTGLPixmapFont *> fonts;
#endif // USE_FTGL

#ifdef USE_GLTT
#  ifdef WIN32
#include <FTFace.h>
#include <FTEngine.h>
#include <GLTTBitmapFont.h>
#  else
#include <gltt/FTFace.h>
#include <gltt/FTEngine.h>
#include <gltt/GLTTBitmapFont.h>
#  endif

static std::map<int, GLTTBitmapFont *> fonts;
#endif // USE_GLTT

#include <cmath>

using namespace std;

#include "mmgr.h"

static BTree<FTFace *> faces;

int  gci_defaultfont = HELVETICA_12;
bool gci_truetypeenabled = false;

void GciSetDefaultFont ( int STYLE )
{

	gci_defaultfont = STYLE;

}

void GciDrawText (int x, int y, const string &text )
{

	GciDrawText ( x, y, text, gci_defaultfont );

}

int GciTextWidth (const string &text )
{
	
	return GciTextWidth ( text, gci_defaultfont );

}

void GciEnableTrueTypeSupport ()
{

	gci_truetypeenabled = true;

}

void GciDisableTrueTypeSupport ()
{

	gci_truetypeenabled = false;

}

bool GciRegisterTrueTypeFont(const string &filename )
{
    return true;
}

bool GciUnregisterTrueTypeFont(const string &filename )
{
    return true;
}

static FTFace * RegisterFace( const char *familyname, const char *filename )
{
    if (faces.GetData(familyname))
        return faces.GetData(familyname);
    
    FTFace *face;
#ifdef USE_GLTT
	face = new FTFace();
    if (!face->open(filename)) {
        delete face;
        return nullptr;
    }
#endif // USE_GLTT
#ifdef USE_FTGL
	face = new FTFace(filename);
    if (face->Error() != 0) {
        delete face;
        return nullptr;
    }
#endif // USE_GLTT
    faces.PutData(familyname, face);
    return face;
}

static void UnregisterFace( const char *familyname )
{
    FTFace *face = faces.GetData(familyname);
    if (face) {
        faces.RemoveData(familyname);
        delete face;
    }
}


void GciDrawText (int x, int y, const string &text, int STYLE )
{

    if ( gci_truetypeenabled && fonts[STYLE] ) {
            
        // Use true type fonts
#ifdef USE_GLTT        
        GLTTBitmapFont *font = fonts[STYLE];
        font->output( x, y, text );
#endif // USE_GLTT
#ifdef USE_FTGL
		FTGLBitmapFont *font = fonts[STYLE];
		//FTGLPixmapFont *font = fonts[STYLE];
        glRasterPos2i(x, y);
		font->Render(text.c_str());
#endif // USE_FTGL

        }
    else {
      GciFallbackDrawText( x, y, text, STYLE );
    }
}

int GciTextWidth (const string &text, int STYLE )
{
  if (fonts[STYLE]) {
#ifdef USE_GLTT
        return fonts[STYLE]->getWidth(text);
#endif
#ifdef USE_FTGL
    float llx, lly, llz, urx, ury, urz;
    fonts[STYLE]->BBox( text.c_str(), llx, lly, llz, urx, ury, urz );
    return (int)(fabs(llx - urx) + 0.5);
#endif
  }
    else 
    return GciFallbackTextWidth( text, STYLE );
}

bool GciLoadTrueTypeFont (int index, const string &fontname, const string &filename, int size )
{

    if (gci_truetypeenabled) {
        FTFace *face = RegisterFace( fontname.c_str(), filename.c_str() );
        
        if (!face)
            return false;
        
#ifdef USE_GLTT
        int pointSize = int (size * 72.0 / 96.0 + 0.5);
        
        GLTTBitmapFont *font = new GLTTBitmapFont(face);

        if (!font->create(pointSize)) {
            delete font;
            return false;
        }
#endif // USE_GLTT
#ifdef USE_FTGL
        int pointSize = int (size * 72.0 / 96.0 + 0.5);
        
        auto *font = new FTGLBitmapFont(filename.c_str());
        //FTGLPixmapFont *font = new FTGLPixmapFont(filename);
        font->GlyphLoadFlags(FT_LOAD_TARGET_MONO);
        if (font->Error() != 0 || !font->FaceSize(pointSize, 96)) {
            delete font;
            return false;
        }
#endif // USE_FTGL
            
        GciDeleteTrueTypeFont(index);
        fonts[index] = font;

        return true;
    }
    else {
        printf ( "GciLoadTrueTypeFont called, but truetypes are not enabled\n" );
        return false;
    }
}

void GciDeleteTrueTypeFont ( int index )
{
#ifdef USE_FTGL
    if (fonts[index]) 
        delete fonts[index];
#endif

#ifdef USE_GLTT
  // FTGL's destructor hangs, for some reason!
    delete fonts[index];
#endif
    fonts[index] = nullptr;
}

void GciDeleteAllTrueTypeFonts ()
{
    // Delete all the frickin' fonts
#ifdef USE_GLTT    
    for (map<int, GLTTBitmapFont *>::iterator x = fonts.begin(); x != fonts.end(); x++)
        GciDeleteTrueTypeFont(x->first);
#else
    for (auto & font : fonts)
    //for (map<int, FTGLPixmapFont *>::iterator x = fonts.begin(); x != fonts.end(); x++)
        GciDeleteTrueTypeFont(font.first);
#endif
    // Delete all the frickin' faces

    DArray <FTFace *> *thefaces = faces.ConvertToDArray ();
    
    for ( int id = 0; id < thefaces->Size (); ++id ) {
        if ( thefaces->ValidIndex (id) ) {

            FTFace *theface = thefaces->GetData (id);
            delete theface;

        }
    }

    // Delete the structures

    delete thefaces;
    faces.Empty ();
    fonts.clear ();

#ifdef USE_GLTT
	// Added to kill the remaining memory leaks, not needed, could be removed if a updated library come and this function is not in.
	//FTEngine::destroyStaticEngine();
#endif

}
