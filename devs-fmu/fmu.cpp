#include "fmu.h"

FMU::FMU(
	const char* modelname,
		const char* guid,
		const char* resource_location,
		const char* so_file_name):
	so_hndl(NULL)
{
	fmi2Status status;
	// Get points to the FMI functions
	fmi2CallbackFunctions tmp = {FMU::fmilogger,calloc,free,NULL,NULL};
	callbackFuncs = new fmi2CallbackFunctions(tmp);
	so_hndl = OPEN_LIB(so_file_name);
	if (so_hndl == NULL)
	{
		throw std::runtime_error("Could not load so file");
    }
	// This only works with a POSIX compliant compiler/system
	_fmi2Instantiate = (fmi2Component (*)(fmi2String, fmi2Type,
		fmi2String, fmi2String, const fmi2CallbackFunctions*,
		fmi2Boolean, fmi2Boolean))GET_FUNC(so_hndl,"fmi2Instantiate");
	assert(_fmi2Instantiate != NULL);
	_fmi2FreeInstance = (void (*)(fmi2Component))GET_FUNC(so_hndl,"fmi2FreeInstance");
	assert(_fmi2FreeInstance != NULL);
	_fmi2SetupExperiment = (fmi2Status (*)(fmi2Component, fmi2Boolean,
		fmi2Real, fmi2Real, fmi2Boolean, fmi2Real))GET_FUNC(so_hndl,"fmi2SetupExperiment");
	assert(_fmi2SetupExperiment != NULL);
	_fmi2EnterInitializationMode = (fmi2Status (*)(fmi2Component))GET_FUNC(so_hndl,"fmi2EnterInitializationMode");
	assert(_fmi2EnterInitializationMode != NULL);
	_fmi2ExitInitializationMode = (fmi2Status (*)(fmi2Component))GET_FUNC(so_hndl,"fmi2ExitInitializationMode");
	assert(_fmi2ExitInitializationMode != NULL);
	_fmi2GetReal = (fmi2Status (*)(fmi2Component, const fmi2ValueReference*, size_t, fmi2Real*))
		GET_FUNC(so_hndl,"fmi2GetReal");
	assert(_fmi2GetReal != NULL);
	_fmi2GetInteger = (fmi2Status (*)(fmi2Component, const fmi2ValueReference*, size_t, fmi2Integer*)) 
		GET_FUNC(so_hndl,"fmi2GetInteger");
	assert(_fmi2GetInteger != NULL);
	_fmi2GetBoolean = (fmi2Status (*)(fmi2Component, const fmi2ValueReference*, size_t, fmi2Boolean*))
		GET_FUNC(so_hndl,"fmi2GetBoolean");
	assert(_fmi2GetBoolean != NULL);
	_fmi2GetString = (fmi2Status (*)(fmi2Component, const fmi2ValueReference*, size_t, fmi2String*))
		GET_FUNC(so_hndl,"fmi2GetString");
	assert(_fmi2GetString != NULL);
	_fmi2SetReal = (fmi2Status (*)(fmi2Component, const fmi2ValueReference*, size_t, const fmi2Real*))
		GET_FUNC(so_hndl,"fmi2SetReal");
	assert(_fmi2SetReal != NULL);
	_fmi2SetInteger = (fmi2Status (*)(fmi2Component, const fmi2ValueReference*, size_t, const fmi2Integer*))
		GET_FUNC(so_hndl,"fmi2SetInteger");
	assert(_fmi2SetInteger != NULL);
	_fmi2SetBoolean = (fmi2Status (*)(fmi2Component, const fmi2ValueReference*, size_t, const fmi2Boolean*))
		GET_FUNC(so_hndl,"fmi2SetBoolean");
	assert(_fmi2SetBoolean != NULL);
	_fmi2SetString = (fmi2Status (*)(fmi2Component, const fmi2ValueReference*, size_t, const fmi2String*))
		GET_FUNC(so_hndl,"fmi2SetString");
	assert(_fmi2SetString != NULL);
	_fmi2DoStep = (fmi2Status (*)(fmi2Component, fmi2Real, fmi2Real, fmi2Boolean))
		GET_FUNC(so_hndl,"fmi2DoStep");
	assert(_fmi2DoStep != NULL);
	// Create the FMI component
	c = _fmi2Instantiate(modelname,fmi2CoSimulation,guid,resource_location,callbackFuncs,fmi2False,fmi2False);
	assert(c != NULL);
	// No numerical tolerance and no predefined stop time
	_fmi2SetupExperiment(c,fmi2False,0.0,-1.0,fmi2False,-1.0);
	// Initialize all variables
	status = _fmi2EnterInitializationMode(c);
	assert(status == fmi2OK);
	// Done with initialization
	status = _fmi2ExitInitializationMode(c);
	assert(status == fmi2OK);
}

FMU::~FMU()
{
	_fmi2FreeInstance(c);
	delete callbackFuncs;
	CLOSE_LIB(so_hndl);
}

double FMU::get_real(int k)
{
	const fmi2ValueReference ref = k;
	fmi2Real val;
	fmi2Status status = _fmi2GetReal(c,&ref,1,&val);
	assert(status == fmi2OK);
	return val;
}

void FMU::set_real(int k, double val)
{
	const fmi2ValueReference ref = k;
	fmi2Real fmi_val = val;
	fmi2Status status = _fmi2SetReal(c,&ref,1,&fmi_val);
	assert(status == fmi2OK);
}

int FMU::get_int(int k)
{
	const fmi2ValueReference ref = k;
	fmi2Integer val;
	fmi2Status status = _fmi2GetInteger(c,&ref,1,&val);
	assert(status == fmi2OK);
	return val;
}

void FMU::set_int(int k, int val)
{
	const fmi2ValueReference ref = k;
	fmi2Integer fmi_val = val;
	fmi2Status status = _fmi2SetInteger(c,&ref,1,&fmi_val);
	assert(status == fmi2OK);
}

bool FMU::get_bool(int k)
{
	const fmi2ValueReference ref = k;
	fmi2Boolean val;
	fmi2Status status = _fmi2GetBoolean(c,&ref,1,&val);
	assert(status == fmi2OK);
	return (val == fmi2True);
}

void FMU::set_bool(int k, bool val)
{
	const fmi2ValueReference ref = k;
	fmi2Boolean fmi_val = fmi2False;
	if (val) fmi_val = fmi2True;
	fmi2Status status = _fmi2SetBoolean(c,&ref,1,&fmi_val);
	assert(status == fmi2OK);
}

std::string FMU::get_string(int k)
{
	const fmi2ValueReference ref = k;
	fmi2String val;
	fmi2Status status = _fmi2GetString(c,&ref,1,&val);
	assert(status == fmi2OK);
	return val;
}

void FMU::set_string(int k, std::string& val)
{
	const fmi2ValueReference ref = k;
	fmi2String fmi_val = fmi2False;
	fmi2Status status = _fmi2SetString(c,&ref,1,&fmi_val);
	assert(status == fmi2OK);
}

void FMU::doStep(double tNow, double h)
{
	fmi2Status status = _fmi2DoStep(c,tNow,h,fmi2True);
	assert(status == fmi2OK);
}
