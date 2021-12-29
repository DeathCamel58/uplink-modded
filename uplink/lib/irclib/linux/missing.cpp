
#ifndef WIN32

#include "missing.h"
#include <cstring>
#include <cctype>

char *_strupr(char *p)
{
  for (char *s = p; *s; s++)
    *s = toupper(*s);
  return p;
}

#endif
