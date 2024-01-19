#include "fmu.h"
#include <iostream>
#include <cfloat>

/// You need these to match what is in the modelDescription.xml file
#define GUID "7a4a8812-7daa-4c9c-95ee-c0e7e4020421" // This isn't checked, so don't worry about it
#define MODEL_NAME "GenrFMU"
// This MUST point to the resources directory of your unpacked FMU!
#define RESOURCES "file:///home/rotten/Code/fmu4-devs-java/devs-fmu/GenrFMU/resources"
#define SHARED_LIB "./GenrFMU/binaries/linux64/GenrFMU.so"

// These are the FMU variable identifiers. See modelDecription.xml
#define TIME_ADVANCE 0
#define JOB 0
#define STOPPED 0

int main()
{
	// Create the FMU object
	FMU* fmu = new FMU(MODEL_NAME,GUID,RESOURCES,SHARED_LIB);
	// Current simulation time
	double tNow = 0;
	// Simulate the FMU object
	while (true)
	{
		double ta = fmu->get_real(TIME_ADVANCE);
		// Time advance is infinite so stop
		if (ta == DBL_MAX)
			break;
		// Report current state, output, and time advance
		std::cout << tNow << " " << ta << " " << fmu->get_bool(STOPPED) << " " << fmu->get_int(JOB) << std::endl;
		// Go to the next step
		fmu->doStep(tNow,ta);
		// Update the clock
		tNow += ta;
		// Inject and input and stop the generator
		if (tNow >= 10.0)
		{
			fmu->set_bool(STOPPED,true);
		}
	}
	delete fmu;
	return 0;	
}
