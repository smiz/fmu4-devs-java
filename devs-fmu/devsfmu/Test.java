package devsfmu;

import java.io.File;
import java.io.IOException;
import no.ntnu.ihb.fmi4j.Fmi4jVariableUtils;
import no.ntnu.ihb.fmi4j.FmiStatus;
import no.ntnu.ihb.fmi4j.SlaveInstance;
import no.ntnu.ihb.fmi4j.TestFMUs;
import no.ntnu.ihb.fmi4j.importer.fmi2.CoSimulationFmu;
import no.ntnu.ihb.fmi4j.importer.fmi2.CoSimulationSlave;
import no.ntnu.ihb.fmi4j.importer.fmi2.Fmu;
import no.ntnu.ihb.fmi4j.modeldescription.variables.RealVariable;

class Test
{
	public static void main(String[] args)
	{
		try
		{
			Fmu fmu = Fmu.from(new File("GenrFMU.fmu"));
			SlaveInstance model = fmu.asCoSimulationFmu().newInstance();
			model.simpleSetup();
			double t = 0;
			double stop = 10;
			double ta = 1.0/100;
			while(t <= stop)
			{
				//ta = model.getModelDescription().getVariableByName("timeRemaining");
				if (!model.doStep(t,ta))
				{
					break;
				}
				t += ta;
			}
			model.terminate();
			fmu.close();
		}
		catch(IOException error)
		{
			error.printStackTrace();
		}
	}
}
