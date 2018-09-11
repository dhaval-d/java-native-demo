// See README.txt for information and build instructions.

import com.example.tutorial.AddressBookProtos.AddressBook;
import com.example.tutorial.AddressBookProtos.Person;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.PrintStream;

public class ListPeople {
    // load native library
  static{
	  System.loadLibrary("readprotonative");
  }
  // Iterates though all people in the AddressBook and prints info about them.
  static void Print(AddressBook addressBook) {	  
     System.out.println("Reading AddressBook Proto from Java");
     for (Person person: addressBook.getPeopleList()) {
      System.out.println("Person ID: " + person.getId());
      System.out.println("  Name: " + person.getName());
      if (!person.getEmail().isEmpty()) {
        System.out.println("  E-mail address: " + person.getEmail());
      }

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

  static native void readProtoNative(String filename);

  static native void readProtoNativeByteArray(byte[] byteArray);

  static native byte[] readProtoNativeByteArrayAndModify(byte[] byteArray);

  // Main function:  Reads the entire address book from a file and prints all
  //   the information inside.
  public static void main(String[] args) throws Exception {
    if (args.length != 2) {
      System.err.println("Usage:  ListPeople ADDRESS_BOOK_FILE OPERATION_TYPE");
      System.out.println("OPERATION_TYPES");
      System.out.println("1 -> READ NATIVE FROM FILE");
      System.out.println("2 -> READ BYTE ARRAY FROM JAVA AND CONVERT TO PROTO IN C++");
      System.out.println("3 -> READ BYTE ARRAY FROM JAVA, MODIFY PROTO IN C++ AND THEN RETURN BYTE ARRAY TO JAVA");
      System.exit(-1);
    }

   int operation_type = Integer.parseInt(args[1]);
   switch(operation_type){
	   case 1:
		System.out.println("READ NATIVE FROM FILE OPTION SELECTED");
		readProtoNative(args[0]);
		break;
	   case 2:
		System.out.println("READ NATIVE FROM BYTE ARRAY SELECTED");
		AddressBook abook = 
			AddressBook.parseFrom(new FileInputStream(args[0]));
		byte[] ba = abook.toByteArray();
		readProtoNativeByteArray(ba);
		break;
	   case 3:
		System.out.println("READ NATIVE, MODIFY AND RETURN BYTE ARRAY TO JAVA ");
		AddressBook abook1 =
			AddressBook.parseFrom(new FileInputStream(args[0]));
		byte[] ba2 = abook1.toByteArray();
		byte[] ba3 = readProtoNativeByteArrayAndModify(ba2);
		AddressBook ab2 = AddressBook.parseFrom(ba3);
		System.out.println("\n\n");
		Print(ab2);

   }
  }
}
