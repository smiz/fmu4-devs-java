#ifndef _fmu_h_
#define _fmu_h_
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <exception>
#include "fmi2Functions.h"
#include "fmi2FunctionTypes.h"
#include "fmi2TypesPlatform.h"

// Functions for loading DLL and so files
#ifdef _WIN32
#include <windows.h>
#define OPEN_LIB(name) LoadLibrary(name)
#define GET_FUNC(hndl,name) GetProcAddress(hndl,name)
#define CLOSE_LIB(hndl) FreeLibrary(hndl)
#else
#include <dlfcn.h>
#define OPEN_LIB(name) dlopen(name,RTLD_LAZY)
#define GET_FUNC(hndl,name) dlsym(hndl,name)
#define CLOSE_LIB(hndl) dlclose(hndl)
#endif

class FMU
{
	public:
		/**
		 * This constructs a wrapper around an FMU. The constructor
		 * must be provided with the FMI's GUID, the number of state variables,
		 * number of event indicators, and the path to the .so file
		 * that contains the FMI functions for this model.
		 */
		FMU(const char* modelname,
			const char* guid,
			const char* resource_location,
			const char* shared_lib_name);
		/// Destructor
		virtual ~FMU();
		/// Get the value of a real variable
		double get_real(int k);
		/// Set the value of a real variable
		void set_real(int k, double val);
		/// Get the value of an integer variable
		int get_int(int k);
		/// Set the value of an integer variable
		void set_int(int k, int val);
		/// Get the value of a boolean variable
		bool get_bool(int k);
		/// Set the value of a boolean variable
		void set_bool(int k, bool val);
		/// Get the value of a string variable
		std::string get_string(int k);
		/// Set the value of a string variable
		void set_string(int k, std::string& val);
		/// Advance the simulation time to tNow+h
		void doStep(double tNow, double h);
	private:
		// Reference to the FMI
		fmi2Component c;
		// Pointer to the FMI interface
		fmi2Component (*_fmi2Instantiate)(fmi2String, fmi2Type,
				fmi2String, fmi2String, const fmi2CallbackFunctions*,
				fmi2Boolean, fmi2Boolean);
		void (*_fmi2FreeInstance)(fmi2Component);
		fmi2Status (*_fmi2SetupExperiment)(fmi2Component, fmi2Boolean,
				fmi2Real, fmi2Real, fmi2Boolean, fmi2Real);
		fmi2Status (*_fmi2EnterInitializationMode)(fmi2Component);
		fmi2Status (*_fmi2ExitInitializationMode)(fmi2Component);
		fmi2Status (*_fmi2GetReal)(fmi2Component, const fmi2ValueReference*, size_t, fmi2Real*);
		fmi2Status (*_fmi2GetInteger)(fmi2Component, const fmi2ValueReference*, size_t, fmi2Integer*);
		fmi2Status (*_fmi2GetBoolean)(fmi2Component, const fmi2ValueReference*, size_t, fmi2Boolean*);
		fmi2Status (*_fmi2GetString)(fmi2Component, const fmi2ValueReference*, size_t, fmi2String*);
		fmi2Status (*_fmi2SetReal)(fmi2Component, const fmi2ValueReference*, size_t, const fmi2Real*);
		fmi2Status (*_fmi2SetInteger)(fmi2Component, const fmi2ValueReference*, size_t, const fmi2Integer*);
		fmi2Status (*_fmi2SetBoolean)(fmi2Component, const fmi2ValueReference*, size_t, const fmi2Boolean*);
		fmi2Status (*_fmi2SetString)(fmi2Component, const fmi2ValueReference*, size_t, const fmi2String*);
		fmi2Status (*_fmi2EnterEventMode)(fmi2Component);
		fmi2Status (*_fmi2NewDiscreteStates)(fmi2Component,fmi2EventInfo*);
		fmi2Status (*_fmi2DoStep)(fmi2Component,fmi2Real,fmi2Real,fmi2Boolean);
		// so library handle
		#ifdef _WIN32
		HINSTANCE so_hndl;
		#else
		void* so_hndl;
		#endif

		static void fmilogger(
			fmi2ComponentEnvironment componentEnvironment,
			fmi2String instanceName,
			fmi2Status status,
			fmi2String category,
			fmi2String message,...)
		{
			if (message != NULL){			
		
				fprintf(stderr, message,"\n");
			}
		}

		fmi2CallbackFunctions* callbackFuncs;

};

#endif
