### FMU4j

FMU4j is a software package for the JVM that enables
export of models compatible with FMI 2.0 for Co-simulation.
This fork is intended to grow into support for importing
DEVS models implemented in Java into a C++ simulation.

Linux is supported. Maybe Windows, eventually.

The original has aged some. You will want to get and install
JDK 15. Make sure your JAVA_HOME points to this directory
before building anything.

To build the support library, do the following in the
top level directory:

./gradlew clean; ./gradlew buildNative ; ./gradlew build ; ./gradlew installDist

To run the example, go to devs-fmu. Edit the build.sh and test_fmu.cpp files
to make your environment. Then execute

bash build.sh ; ./a.out

If all goes well, the example will build and run.

To create your own FMU importer for your C++ DEVS simulator check out fmu.cpp
for an example / starting point.

To wrap your Java DEVS simulator in an FMU, look at the example model in devsfmu.
You will need to implement your own version of DevsFMU.java to support your 
simulation tool and and produce a version of GenrFMU that specializes your DevsFMU
with the details of any particular simulation model.

Good luck! You'll need it!

Looking forwards, the FMU4j offers a nice starting point that mostly works out of
the box. But it suffers from compatibility issues with modern tools, libraries,
etc. that would need to be resolved and then maintained if this is going to
turn into a really useful tool.

#### JFrog artifactory

Artifacts are available through JFrog artifactory.

```groovy
repositories {
    maven { url "https://ais.jfrog.io/artifactory/ais-gradle-dev-local/"} // SNAPSHOTS
    maven { url "https://ais.jfrog.io/artifactory/ais-gradle-release-local/"} // STABLE
}

dependencies {
    def fmu4j_version = "..."
    implementation "no.ntnu.ais.fmu4j:fmi-export:$version" // FMI skeleton
    implementation "no.ntnu.ais.fmu4j:fmi-builder:$version" // FMU generation from code
}


```


###### Write the code

```java
@SlaveInfo(
        modelName = "MyJavaSlave",
        author = "John Doe"
)
public class JavaSlave extends Fmi2Slave {
    
    @ScalarVariable
    private int intOut = 99;
    @ScalarVariable
    private double realOut = 2.0;
    @ScalarVariable
    private double[] realsOut = {50.0, 200.0};
    @ScalarVariable
    private String[] string = {"Hello", "world!"};
    
    private ComplexObject obj = ComplexObject();
    
    public JavaSlave(Map<String, Object> args) {
        super(args);
    }

    @Override
    protected void registerVariables() {
        register(integer("complexInt", () -> obj.integer)
                .causality(Fmi2Causality.output));
        register(real("complexReal", () -> obj.real)
                .causality(Fmi2Causality.output));
    }

    @Override
    public void doStep(double currentTime, double dt) {
        realOut += dt;
    }

}
```
###### Build the FMU

```
Usage: fmu-builder [-h] [-d=<destFile>] -f=<jarFile> -m=<mainClass>
  -d, --dest=<destFile>    Where to save the FMU.
  -f, --file=<jarFile>     Path to the Jar.
  -h, --help               Print this message and quits.
  -m, --main=<mainClass>   Fully qualified name of the main class.
```

In order to build the `fmu-builder` tool, clone this repository and invoke `./gradlew installDist`.
The distribution will be located in the folder _fmu-builder-app/build/install_.

Note that the produced Jar file must be a fatJar/shadowJar.

```groovy
jar {
    from {
        configurations.runtimeClasspath.collect { it.isDirectory() ? it : zipTree(it) }
    }
}
```

*** 

Would you rather build FMUs using Python? Check out [PythonFMU](https://github.com/NTNU-IHB/PythonFMU)! <br>
Or would you rather simulate FMUs using C++? Check out [FMI4cpp](https://github.com/NTNU-IHB/FMI4cpp)! <br>
Need to distribute your FMUs? [FMU-proxy](https://github.com/NTNU-IHB/FMU-proxy) to the rescue! <br>
Need a complete co-simulation framework with SSP support? Check out [Vico](https://github.com/NTNU-IHB/Vico) <br>
