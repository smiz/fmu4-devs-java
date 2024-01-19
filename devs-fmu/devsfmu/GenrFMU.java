package devsfmu;

import no.ntnu.ais.fmu4j.export.fmi2.Fmi2Slave;
import no.ntnu.ais.fmu4j.export.fmi2.ScalarVariable;
import no.ntnu.ais.fmu4j.export.fmi2.RealVariable;
import no.ntnu.ais.fmu4j.export.fmi2.IntVariable;
import no.ntnu.ais.fmu4j.export.fmi2.BooleanVariable;
import no.ntnu.ais.fmu4j.modeldescription.fmi2.Fmi2Causality;
import no.ntnu.ais.fmu4j.modeldescription.fmi2.Fmi2Variability;
import java.util.Map;

/**
  * This is the base class for an FMI interface to a DEVS
  * atomic model. 
  */
public class GenrFMU
	extends DevsFMU
{

	private Genr genr;

	public GenrFMU(Map<String, Object> args)
	{
		super(args);
		model = genr = new Genr();
	}

	/// The derived class must register all variables of its
	/// model that will be input or output. 
	@Override
	protected void registerVariables()
	{
		super.registerVariables();
		/// Register our output variable
		register(new IntVariable("job", () -> genr.jobs)
			.causality(Fmi2Causality.output)
			.variability(Fmi2Variability.discrete));
		/// Register our input variable
		register(new BooleanVariable("stopped", () -> genr.stopped)
			.setter((value) -> genr.stopped = value)
			.causality(Fmi2Causality.input)
			.variability(Fmi2Variability.discrete));
	}
}
