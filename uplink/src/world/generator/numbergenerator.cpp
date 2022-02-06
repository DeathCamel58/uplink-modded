
#include <cstdlib>

#include "world/generator/numbergenerator.h"

#include "mmgr.h"



void NumberGenerator::Initialise ()
{

}


int NumberGenerator::RandomNumber ( int range )
{

	int result = (int) ( ( (float) rand () / (float) RAND_MAX ) * range );
	if ( result < 0 ) result = 0;
	if ( result >= range ) result = range - 1;

	return result;

}

float NumberGenerator::RandomUniformNumber ()	
{
    return (float) rand () / (float) RAND_MAX;
}

float NumberGenerator::RandomNormalNumber ( float mean, float range )	
{
	
	// result ~ N ( mean, range/3 )

	float s = 0;

	for ( int i = 0; i < 12; ++i )
		s += ( (float) rand () / (float) RAND_MAX );

	s = ( s-6.0f ) * ( range/3.0f ) + mean;

	if ( s < mean - range ) s = mean - range;
	if ( s > mean + range ) s = mean + range;

	return s;

/*

	// Test for random normal generator ========================================

	cout << endl << endl;

	int results [21];
	int outside = 0;
	for ( int i = 0; i < 20; ++i ) 
		results [i] = 0;

	for ( i = 0; i < 4000; ++i ) {
		int result = RandomNormalNumber ( 10, 9 );
		if ( result >= 0 && result <= 20 )
			results [result] ++;
		else
			outside ++;
	}

	for ( i = 0; i < 21; ++i ) {
		cout << i << " : ";
		for ( int j = 0; j < results [i]; j+=10 )
			cout << "#";
		cout << endl;
	}	

	cout << "Outside : " << outside << endl;

	cout << endl << endl;

	// Test for random normal generator ========================================

*/

}

int NumberGenerator::ApplyVariance ( int num, int variance )
{

	float variancefactor = 100.0f + RandomNormalNumber ( 0.0f, (float) variance );
	num = (int) ( num * ( variancefactor / 100.0f ) );

	return num;

}
