# ~~~
# Copyright 2018, Google Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ~~~
# See README.md.

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
