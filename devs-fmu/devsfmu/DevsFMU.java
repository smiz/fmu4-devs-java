package devsfmu;

import no.ntnu.ais.fmu4j.export.fmi2.Fmi2Slave;
import no.ntnu.ais.fmu4j.export.fmi2.ScalarVariable;
import no.ntnu.ais.fmu4j.export.fmi2.RealVariable;
import no.ntnu.ais.fmu4j.modeldescription.fmi2.Fmi2Causality;
import no.ntnu.ais.fmu4j.modeldescription.fmi2.Fmi2Variability;
import java.util.Map;

/**
  * This is the base class for an FMI interface to a DEVS
  * atomic model. 
  */
public class DevsFMU extends Fmi2Slave
{

	/// The model that will be simulated. This is
	/// created by the derived class. It could be 
	/// root coordinator in a more sophisticated
	/// implementation.
	protected Atomic model;
	/// Elapsed time for the atomic model
	private double e;
	/// Should be set to true by the derived class
	/// whenever an input variable value is set.
	protected boolean hasInput;

	public DevsFMU(Map<String, ?> args)
	{
		super(args);
		/// The derived class must create the
		/// model and it assign it to the atomic
		/// member variable.
		model = null;
		e = 0.0;
	}

	/// The derived class must register all variables of its
	/// model that will be input or output. 
	@Override
	protected void registerVariables()
	{
		/// We create a method for getting the
		/// time to next event.
		register(new RealVariable("timeRemaining", () -> (model.ta()-e))
			.causality(Fmi2Causality.output));
		/// Derived class must create the remainder of the model variables
		/// that act as input and output.
	}

	@Override
	public void setupExperiment(double startTime, double stopTime, double tolerance)
	{
		super.setupExperiment(startTime, stopTime, tolerance);
		/// Nothing for us to do here at the moment
	}
	@Override
	public void enterInitialisationMode()
	{
		super.enterInitialisationMode();
		/// Nothing for us to do here at the moment
	}
	@Override
	public void exitInitialisationMode()
	{
		super.exitInitialisationMode();
		/// Nothing for us to do here at the moment
	}

	@Override
	public void doStep(double currentTime, double dt)
	{
		/// Advance the clock by dt units of time. We expect
		/// dt <= model.ta()-e. A more sophisticated
		/// implementation might relax this.
		e += dt;
		/// If we have input, then call the external
		/// or confluent transition function. Assume
		/// that the input occurs immediately.
		if (hasInput)
		{
			if (e < model.ta())
				model.delta_ext(e);
			else
			{
				model.output();
				model.delta_conf();
			}
			// Reset the elapsed time
			e = 0.0;
			// Clear the input
			hasInput = false;
		}
		/// Return if there is nothing to do
		else if (e < model.ta())
		{
			return;
		}
		/// Otherwise call the internal transition function
		else
		{
			model.output();
			model.delta_int();
			e = 0.0;
		}
	}

	@Override
	public void terminate()
	{
		super.terminate();
		/// Nothing for us to do here
	}
}
