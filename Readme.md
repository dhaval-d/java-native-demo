# Sample Java JNI example calling C++ native code

This is an example code to pass information between Java and C++ program using Protobuf.

Java code calls C++ using JNI(Java Native Interface).

Program gives you 3 options to read and read/write protos.

1. Pass file name to C++ and read proto using C++.
2. Read file in Java and then pass byte array to C++. Convert byte array into proto in C++.
3. Read file in Java and then pass byte array to C++. Add extra elements to proto in C++ and return byte array in Java. Then read byte array in Java.



## Build instructions
This example only has been tested on Linux environment. As native C++ code depends on the platform it runs on, you need to modify
your build process and java code(native library name in java) to make it work on the platform.

### Pre-requisites
#### Install Protobuf
This example leverages Protobuf to transfer data from Java to native code. Here's a link that provides instructions to
install protobuf: <a href="https://github.com/protocolbuffers/protobuf/blob/master/src/README.md" target="_blank">Click here</a>



#### Set classpath for Protobuf jar
I am using Makefile to build files in order. I am using CLASSPATH2 to keep the location of protobuf jar file.




### Build

#### Makefile
I am using Makefile to perform build activities. Run following command to build your classes.

make all




#### Build order if you are planning to build manually.
1. Build protobuf file to generate java class based on proto definition.

2. Build Java file that generates a header file for C++ code.

3. Build C++ file that will be run using JNI by Java code.