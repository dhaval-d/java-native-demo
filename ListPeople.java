/*
 * Copyright 2018 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// See README.txt for information and build instructions.
import com.example.tutorial.AddressBookProtos.AddressBook;
import com.example.tutorial.AddressBookProtos.Person;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.io.PrintStream;

public class ListPeople { 
  // load native library
  static{
    System.loadLibrary("readprotonative");
  }
  
  // Iterates though all people in the AddressBook and prints info about them.
  static void Print(AddressBook addressBook) {	  
    System.out.println("Reading AddressBook Proto from Java");
    
    // Iterate through persons
    for (Person person: addressBook.getPeopleList()) {
      System.out.println("Person ID: " + person.getId());
      System.out.println("  Name: " + person.getName());
      if (!person.getEmail().isEmpty()) {
        System.out.println("  E-mail address: " + person.getEmail());
      }

      // Iterate through phone numbers    
      for (Person.PhoneNumber phoneNumber : person.getPhonesList()) {
        switch (phoneNumber.getType()) {
          case MOBILE:
            System.out.print("  Mobile phone #: ");
            break;
          case HOME:
            System.out.print("  Home phone #: ");
            break;
          case WORK:
            System.out.print("  Work phone #: ");
            break;
          default:
            System.out.println(" Unknown phone #: ");
            break;
        }
        System.out.println(phoneNumber.getNumber());
      }
    }
  }

  // Native method that takes proto file name and reads it
  static native void readProtoNative(String filename);

  // Native method that takes a byte array from Java program and reads it
  static native void readProtoNativeByteArray(byte[] byteArray);

  // Native method that takes a byte array, allows user to change it and returns it
  static native byte[] readProtoNativeByteArrayAndModify(byte[] byteArray);

  // Main function:  Reads the entire address book from a file and prints all
  // the information inside.
  public static void main(String[] args) throws Exception {
    if (args.length != 2) {
      System.err.println("Usage:  ListPeople ADDRESS_BOOK_FILE OPERATION_TYPE");
      System.out.println("OPERATION_TYPES");
      System.out.println("1 -> Read file using native C++ application");
      System.out.println("2 -> Read file in Java, pass byte array to C++ application and parse byte array in C++");
      System.out.println("3 -> Read file in Java, pass and modify byte array in C++ application and return to Java");
      System.exit(-1);
    }

    int operation_type = Integer.parseInt(args[1]);
    switch(operation_type){
      case 1:
        System.out.println("Read file using native C++ application");
	readProtoNative(args[0]);
	break;
      case 2:
        System.out.println("Read file in Java, pass byte array to C++ application and parse byte array in C++");
        AddressBook aBook = 
                    AddressBook.parseFrom(new FileInputStream(args[0]));
        byte[] bArray = aBook.toByteArray();
        readProtoNativeByteArray(bArray);
        break;
      case 3:
        System.out.println("Read file in Java, pass and modify byte array in C++ application and return to Java");
        //AddressBook addressBook = null;
        // Read the existing address book.
        AddressBook addressBook =
                    AddressBook.parseFrom(new FileInputStream(args[0]));

        byte[] bArray2 = addressBook.toByteArray();
        byte[] bArray3 = readProtoNativeByteArrayAndModify(bArray2);
        AddressBook abookReturned = AddressBook.parseFrom(bArray3);
        Print(abookReturned);
        
        // Store updated proto to file
        FileOutputStream outputStream = null;
        try {
          outputStream = new FileOutputStream(args[0]);
          outputStream.write(bArray3);
        } catch(IOException ex) {
          throw ex;
        } finally {
          outputStream.close();
        }
        System.out.println("File stored!!!\n\n");
     }
  }
}
