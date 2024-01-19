package devsfmu;

/**
  * An atomic model interface to illustrate
  * use of the DevsFMU for wrapping a DEVS
  * model (or its resultant).
  */
public abstract class Atomic
{
	public abstract void delta_int();
	/// This is tricky. We will not provide input 
	/// via a bag to the atomic model. Instead, we
	/// will set values for its input variables and
	/// then call the state transition function.
	/// This is a literal interpretation of the FMI
	/// standard. If the various set methods build up
	/// this bag, then the external transition could
	/// accept the bag.
	public abstract void delta_ext(double e);
	/// See my comment above
	public abstract void delta_conf();
	/// This method sets the values of the output variables.
	/// Again, it does not generate messages because the
	/// FMI standard expects output to be obtained via
	/// the get methods. If the various output messages
	/// are used to set appropriate variable values, this
	/// this could return a bag of messages.
	public abstract void output();
	/// Time advance function
	public abstract double ta();
}
