package devsfmu;

import no.ntnu.ihb.fmi4j.Fmi4jVariableUtils;
import no.ntnu.ihb.fmi4j.FmiStatus;
import no.ntnu.ihb.fmi4j.SlaveInstance;
import no.ntnu.ihb.fmi4j.importer.fmi2.CoSimulationFmu;
import no.ntnu.ihb.fmi4j.importer.fmi2.Fmu;
import no.ntnu.ihb.fmi4j.modeldescription.variables.RealVariable;
import no.ntnu.ihb.fmi4j.modeldescription.variables.IntegerVariable;
import java.io.File;
import java.io.IOException;

class TestGenr
{
	public static void main(String[] args) throws IOException
	{
		double t = 0.0, stop = 1.0;
		Fmu fmu = Fmu.from(new File("GenrFMU.fmu"));
		SlaveInstance model = fmu.asCoSimulationFmu().newInstance();
		model.simpleSetup();
		RealVariable sigma = model.getModelDescription().getVariableByName("timeRemaining").asRealVariable();
		IntegerVariable job = model.getModelDescription().getVariableByName("job").asIntegerVariable(); 
		while(t <= stop)
		{
			double h = Fmi4jVariableUtils.read(sigma,model).getValue();
			if (!model.doStep(t,h))
			{
				break;
			}
			t += h;
			System.out.println(t+" "+Fmi4jVariableUtils.read(job,model).getValue());
		}
		model.terminate();
		fmu.close();
	}
}
