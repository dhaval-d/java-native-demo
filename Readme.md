# Sample Java JNI example calling C++ native code

This is an example code to pass information between Java and C++ program using Protobuf.

Java code calls C++ using JNI(Java Native Interface).

This demo gives you 3 options to read and read/write protos.

1. Pass file name to C++ and read proto using C++.
2. Read file in Java and then pass byte array to C++. Convert byte array into proto in C++ and print.
3. Read file in Java and then pass byte array to C++. Add extra elements to proto in C++ and return byte array in Java. Then read byte array in Java.



## Build instructions
This example only has been tested on Linux environment. As native C++ code depends on the platform it runs on, you need to modify
your build process and java code(native library name in java) to make it work on the platform.

### Pre-requisites
#### Install Protobuf 
This example leverages Protobuf to transfer data from Java to native code. Here's a link that provides instructions to
install protobuf: <a href="https://github.com/protocolbuffers/protobuf/blob/master/src/README.md" target="_blank">Click here</a>



#### Set PROTOBUFJAR environment variable to Protobuf jar location
I am using Makefile to build files in order. I am using PROTOBUFJAR environment variable to keep the location of protobuf jar file. I have included protobuf-java-3.6.0.jar along with this code.

      $ export PROTOBUFJAR=/home/<user>/java-native-demo/protobuf-java-3.6.0.jar



### Build

#### Makefile
I am using Makefile to perform build activities. Run following command to build your classes.

      $ make all




#### Build order if you are planning to build manually.
1. Build protobuf file to generate java class and C++ class based on proto definition.

2. Build Java file that generates a header file for C++ code.

3. Build C++ file that will be run using JNI by Java code.


### How to test
1. Standalone C++ application
        
        $ ./list_people.out <outputfile>
        
2. Pass filename from  java to C++ and read a file using C++:

        $ java -classpath .:$PROTOBUFJAR -Djava.library.path=. ListPeople <outputfile> 1


3. Read a file in Java, convert it to byte array and read this byte array in C++, convert it to Proto in C++ and print it.

        $ java -classpath .:$PROTOBUFJAR -Djava.library.path=. ListPeople <outputfile> 2


4. Read a file in Java, convert it to byte array and read this byte array in C++ and convert it to Proto in C++. Now, add a record to this proto in C++, return an updated byte array to Java, convert it to Proto in Java and print there.

        $ java -classpath .:$PROTOBUFJAR -Djava.library.path=. ListPeople <outputfile> 3

