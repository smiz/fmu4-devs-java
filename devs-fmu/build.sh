# This must point to where you installed FMU4j
FMU4J=${HOME}/Code/fmu4-devs-java

# This should not need to change

FMU4J_APP=${FMU4J}/fmu-builder-app/build/install/fmu-builder-app/lib
CLASSPATH=${PWD}:\
${FMU4J_APP}/picocli-4.6.3.jar:\
${FMU4J_APP}/annotations-13.0.jar:\
${FMU4J_APP}/kotlin-stdlib-common-1.6.20.jar:\
${FMU4J_APP}/kotlin-stdlib-1.6.20.jar:\
${FMU4J_APP}/fmu-builder-app-0.1.2-SNAPSHOT.jar:\
${FMU4J_APP}/fmu-builder-0.1.2-SNAPSHOT.jar:\
${FMU4J}/fmu-slaves/build/libs/fmu-slaves-0.1.2-SNAPSHOT.jar:\
${FMU4J}/fmi-export/build/libs/fmi-export-0.1.2-SNAPSHOT.jar:\
${PWD}/lib/slf4j-api-1.7.26.jar:\
${PWD}/lib/slf4j-jdk-platform-logging-2.0.9.jar:\
${PWD}/lib/slf4j-log4j12-1.7.26.jar:\
${PWD}/lib/slf4j-log4j12-1.7.9.jar:\
${PWD}/lib/slf4j-simple-1.7.26.jar:\
${PWD}/lib/jaxb-runtime.jar:\
${PWD}/lib/jaxb-api.jar:\
${PWD}/lib/javax.activation-api.jar:\
${PWD}/lib/istack-commons-runtime.jar
LD_LIBRARY_PATH=${FMU4J}/fmu-builder/src/main/resources/binaries/linux64

# Compile the java files. If you want to build your
# own model then use this as a template.
javac -classpath ${CLASSPATH} devsfmu/Atomic.java devsfmu/Genr.java devsfmu/GenrFMU.java devsfmu/DevsFMU.java
# Put them into a jar file. Must must unpack and repack the FMU4j packages or the
# class loader will fail.
jar xf ${FMU4J}/fmi-export/build/libs/fmi-export-0.1.2-SNAPSHOT.jar
jar xf ${FMU4J}/fmu-builder-app/build/install/fmu-builder-app/lib/kotlin-stdlib-common-1.6.20.jar
jar xf ${FMU4J}/fmu-builder-app/build/install/fmu-builder-app/lib/kotlin-stdlib-1.6.20.jar
# Put your stuff and their stuff into a jar file named after your model.
jar cf GenrFMU.jar devsfmu no kotlin
# Clean up their stuff
rm -rf no kotlin META-INF
# Run the FMU builder app and unpack the FMU
java -classpath ${CLASSPATH} no.ntnu.ais.fmu4j.FmuBuilder --main=devsfmu.GenrFMU --file=GenrFMU.jar
rm -rf GenrFMU
unzip -d GenrFMU GenrFMU.fmu

# This builds and runs an executable to test the FMU. You
# will need to adjust the c++ code to match the specific 
# variable names, etc, etc of your FMU. See the modelDescription.xml
# file in the unpacked fmu.
# Compile the test program
g++ -Wall -g -I${PWD} -I${PWD}/fmi2 -L${LD_LIBRARY_PATH} fmu.cpp test_fmu.cpp -ldl -lfmu4j
# Run the test program
#./a.out
