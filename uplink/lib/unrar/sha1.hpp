#ifndef _RAR_SHA1_
#define _RAR_SHA1_

#define HW 5

#include "rartypes.hpp"

typedef struct {
    uint32 state[5];
    uint32 count[2];
    unsigned char buffer[64];
} hash_context;

void hash_initial( hash_context * c );
void hash_process( hash_context * c, unsigned char * data, unsigned len );
void hash_final( hash_context * c, uint32[HW] );

#endif
