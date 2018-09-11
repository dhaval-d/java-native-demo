# Sample Java JNI example calling C++ native code

This is an example code to pass information between Java and C++ program using Protobuf.

Java code calls C++ using JNI(Java Native Interface).

Program gives you 3 options to read and read/write protos.

1. Pass file name to C++ and read proto using C++.
2. Read file in Java and then pass byte array to C++. Convert byte array into proto in C++.
3. Read file in Java and then pass byte array to C++. Add extra elements to proto in C++ and return byte array in Java. Then read byte array in Java.



## Build instructions

### Pre-requisites

### Makefile