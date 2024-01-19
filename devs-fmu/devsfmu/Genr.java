package devsfmu;

/// Generator that produces jobs at a fixed rate
public class Genr
	extends Atomic
{

	/// Number of jobs that we have produced. This is
	/// an output variable.
	public int jobs;
	/// Are we running. This is an input variable.
	public boolean stopped;

	public Genr()
	{
		super();
		jobs = 0;
		stopped = false;
	}

	public void delta_int() {}
	// Input stops the generator
	public void delta_ext(double e){}
	public void delta_conf(){}
	public void output(){ jobs++; }
	public double ta() { return (stopped) ? Double.MAX_VALUE : 1.0; }
}
