// Copyright 2018 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <fstream>
#include <google/protobuf/util/time_util.h>
#include <iostream>
#include <string>

#include "addressbook.pb.h"
#include "ListPeople.h"
#include <unistd.h>

using namespace std;
using google::protobuf::util::TimeUtil;

// Iterates through all people in the AddressBook and prints info about them.
void ListPeople(const tutorial::AddressBook& address_book) {
    cout << "Reading AddressBook Proto using C++\n";
    // Loop through all persons in address book
    for (int i = 0; i < address_book.people_size(); i++) {
    	const tutorial::Person& person = address_book.people(i);

    	cout << "Person ID: " << person.id() << endl;
    	cout << "  Name: " << person.name() << endl;
    	
	if (person.email() != "") {
      		cout << "  E-mail address: " << person.email() << endl;
    	}

    	// Loop through all phone numbers
    	for (int j = 0; j < person.phones_size(); j++) {
      		const tutorial::Person::PhoneNumber& phone_number = person.phones(j);

      		switch (phone_number.type()) {
        		case tutorial::Person::MOBILE:
          			cout << "  Mobile phone #: ";
          			break;
        		case tutorial::Person::HOME:
          			cout << "  Home phone #: ";
          			break;
        		case tutorial::Person::WORK:
          			cout << "  Work phone #: ";
          			break;
        		default:
          			cout << "  Unknown phone #: ";
          			break;
      		}
      		cout << phone_number.number() << endl;
    	}

    	if (person.has_last_updated()) {
      		cout << "  Updated: " << TimeUtil::ToString(person.last_updated()) << endl;
    	}
  }
}

// Prompt to get new person record in address book.
void PromptForAddress(tutorial::Person* person) {
	cout <<"\n\n\nEnter new  person information"<<endl;
	cout << "Enter person ID number: ";
	int id;
	cin >> id;
	person->set_id(id);
	cin.ignore(256, '\n');

	cout << "Enter name: ";
 	getline(cin, *person->mutable_name());

	cout << "Enter email address (blank for none): ";
	string email;
	getline(cin, email);
	if (!email.empty()) {
		person->set_email(email);
	}

	while (true) {
		cout << "Enter a phone number (or leave blank to finish): ";
		string number;
		getline(cin, number);
		
		if (number.empty()) {
			break;
		}

		tutorial::Person::PhoneNumber* phone_number = person->add_phones();
		phone_number->set_number(number);

		cout << "Is this a mobile, home, or work phone? ";
		string type;
		getline(cin, type);
		if (type == "mobile") {
			phone_number->set_type(tutorial::Person::MOBILE);
		} else if (type == "home") {
			phone_number->set_type(tutorial::Person::HOME);
		} else if (type == "work") {
			phone_number->set_type(tutorial::Person::WORK);
		} else {
			cout << "Unknown phone type.  Using default." << endl;
		}
	}
	*person->mutable_last_updated() = TimeUtil::SecondsToTimestamp(time(NULL));
}

// initialize proto
void initProto(){
	//Verify that the version of the library that we linksed against is
	//compatible with the version of the headers we compiled agaist.
	GOOGLE_PROTOBUF_VERIFY_VERSION;
}

// This method reads proto from a file name passed as an argument.
JNIEXPORT void JNICALL Java_ListPeople_readProtoNative
  (JNIEnv *env, jclass thisClass,jstring jFilePath){
	  cout << "C++ reading of Proto from file.\n";
	  initProto();
	  const char *fileStr = env->GetStringUTFChars(jFilePath, NULL);

	  cout <<"File name provided by Java: "<<fileStr <<endl;

	  tutorial::AddressBook address_book;
	  {
		// Read the existing address book.
		fstream input(fileStr, ios::in | ios::binary);
		
		if (!address_book.ParseFromIstream(&input)) {
		         cerr << "Failed to parse address book." << endl;
		         exit(0);
		}
	   }
	  env->ReleaseStringUTFChars(jFilePath, fileStr);  // release resources
	  // List address book
	  ListPeople(address_book);
 }

// This method reads byte array from java and converts it to Protobuf object.
JNIEXPORT void JNICALL Java_ListPeople_readProtoNativeByteArray
  (JNIEnv *env, jclass thisClass, jbyteArray bArray){
	  cout << "C++ reading of Proto from ByteArray.\n";
	  initProto();
	  tutorial::AddressBook address_book;
	  
	  jbyte *bufferElems = env->GetByteArrayElements(bArray, 0);
	  int len = env->GetArrayLength(bArray);
	  
	  try {
		  address_book.ParseFromArray(reinterpret_cast<unsigned char*>(bufferElems),len);
		 // handle message here
	  } catch (...) {}
	 env->ReleaseByteArrayElements(bArray, bufferElems, JNI_ABORT);
         ListPeople(address_book);
  }

// This method first reads byte array from java and converts to Protobuf object.
// Then makes modifications to Protobuf and sends byte array to java.
JNIEXPORT jbyteArray JNICALL Java_ListPeople_readProtoNativeByteArrayAndModify
  (JNIEnv *env, jclass thisClass, jbyteArray bArray){
	  cout << "C++ reading and modification of Proto from ByteArray.\n";
	  initProto();
	  tutorial::AddressBook address_book;
	  
	  jbyte *bufferElems = env->GetByteArrayElements(bArray, 0);
	  int len = env->GetArrayLength(bArray);
	  
	  try {
		 address_book.ParseFromArray(reinterpret_cast<unsigned char*>(bufferElems),len);
		 ListPeople(address_book);
	  } catch (...) {}
	 env->ReleaseByteArrayElements(bArray, bufferElems, JNI_ABORT);

	 cout<<"\n\nLet's add objects to Proto";
	 PromptForAddress(address_book.add_people());
  
	 // Finally convert Proto to bytes and return to java
	 int size = address_book.ByteSize();
	 char *ret = new char[size];
	 address_book.SerializeToArray(ret,size);

	 jbyteArray array = env->NewByteArray(size);
	 env->SetByteArrayRegion( array, 0, size, (const jbyte*)ret );

	 delete[] ret;
	 return array;
  }

// Main function:  Reads the entire address book from a file and prints all
//   the information inside.
int main(int argc, char* argv[]) {
  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  if (argc != 2) {
    cerr << "Usage:  " << argv[0] << " ADDRESS_BOOK_FILE" << endl;
    return -1;
  }

  tutorial::AddressBook address_book;
  {
    // Read the existing address book.
    fstream input(argv[1], ios::in | ios::binary);
    if (!input) {	
      cout << argv[1] << ": File not found.  Creating a new file." << endl;
    } else if (!address_book.ParseFromIstream(&input)) {
      cerr << "Failed to parse address book." << endl;
      return -1;
    }
  }

  // Add an address.
  PromptForAddress(address_book.add_people());
  {
    // Write the new address book back to disk.
    fstream output(argv[1], ios::out | ios::trunc | ios::binary);
    if (!address_book.SerializeToOstream(&output)) {
      cerr << "Failed to write address book." << endl;
      return -1;
    }
  }

  cout << "C++ reading proto from file. ";
  ListPeople(address_book);

  // Optional:  Delete all global objects allocated by libprotobuf.
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
