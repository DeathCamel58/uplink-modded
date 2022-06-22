
#ifndef included_numbergenerator_h
#define included_numbergenerator_h


class NumberGenerator
{

public:

	static void Initialise ();


	static int RandomNumber ( int range );								
				// result ~ U ( 0, range ),      0 <= result < range
	

	static float RandomNormalNumber ( float mean, float range );		
				// result ~ N ( mean, range/3 ), mean - range < result < mean + range	
  
	static float RandomUniformNumber ();

	/**
	 * Applies a random percentage variance on the given number
	 * @param num The number to change
	 * @param variance The maximum percentage change
	 * @return The number with the variance applied
	 */
	static int ApplyVariance ( int num, int variance );

};


#endif
