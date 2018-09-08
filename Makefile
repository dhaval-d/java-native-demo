# See README.txt.

.PHONY: all java cpp  clean

all: java cpp

cpp:    list_people_cpp
java:   list_people_java

clean:
	rm -f list_people_java
	rm -f ListPeople*.class com/example/tutorial/*.class
	rm -f protoc_middleman addressbook.pb.cc addressbook.pb.h addressbook_pb2.py com/example/tutorial/AddressBookProtos.java
	rm -f libreadprotonative.so
	rm -f ListPeople.h
	rm -R -f com 

protoc_middleman: addressbook.proto
	protoc $$PROTO_PATH --cpp_out=. --java_out=. addressbook.proto
	@touch protoc_middleman

list_people_cpp: list_people.cpp protoc_middleman
	pkg-config --cflags protobuf  # fails if protobuf is not installed
	g++ -fPIC -I"$$JAVA_HOME/include" -I"$$JAVA_HOME/include/linux" -shared -o libreadprotonative.so list_people.cpp addressbook.pb.cc `pkg-config --cflags --libs protobuf`

list_people_java: ListPeople.java protoc_middleman
	javac -h . -cp $$CLASSPATH2 ListPeople.java com/example/tutorial/AddressBookProtos.java
