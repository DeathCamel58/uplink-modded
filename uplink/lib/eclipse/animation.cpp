
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

	cout << "ANIMATION : buttonname:'" << buttonname << "'" << endl;
	cout << "			  sX:" << sourceX << ", sY:" << sourceY << ", tX:" << targetX << ", tY:" << targetY << ", dX:" << dX << ", dY:" << dY << endl;
	cout << "		      sW:" << sourceW << ", sH:" << sourceH << ", tW:" << targetW << ", tH:" << targetH << ", dH:" << dH << ", dW:" << dW << endl;
	cout << "		      target caption : '" << targetC << "', dC:" << dC << endl;
	cout << "			  time:" << time << ", starttime:" << starttime << ", finishtime:" << finishtime << ", MOVETYPE:" << MOVETYPE << endl;

}

