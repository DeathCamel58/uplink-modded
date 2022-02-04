
#include <cstdio>

#include "animation.h"

#include "mmgr.h"

Animation::Animation()
    : button(nullptr)
{
}

Animation::~Animation()
= default;

void Animation::DebugPrint () const
{

	printf ( "ANIMATION : buttonname:'%s'\n", buttonname );
	printf ( "			  sX:%d, sY:%d, tX:%d, tY:%d, dX:%f, dY:%f\n", sourceX, sourceY, targetX, targetY, dX, dY );
	printf ( "		      sW:%d, sH:%d, tW:%d, tH:%d, dH:%f, dW:%f\n", sourceW, sourceH, targetW, targetH, dH, dW );
	printf ( "		      target caption : '%s', dC:%f\n", targetC, dC );
	printf ( "			  time:%d, starttime:%d, finishtime:%d, MOVETYPE:%d\n", time, starttime, finishtime, MOVETYPE );

}

