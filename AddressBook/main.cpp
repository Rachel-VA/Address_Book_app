/*
Rachael Savage
CSC275: C++ 2
Professor Tony Hinton
7/1/23
*/

#include <iostream> //input/output
#include <fstream> //working with files
#include <string> //string data type
#include <memory> //necessary directive for smart pointers functionalities
using namespace std; //declare using namespace std for to use cout sometimes. But it's best to use std:: to ensure no issue, especially when using OOP


//Create a struct and name it Contact to group the address book as a group
struct Contact {
    //this struct does not have an explicitly defined constructor.(we don't need to explicitely define a constructor. the compiler can auto generate a default constructor)
    //In this case, the default constructor for std::string will be used
/*
we can explicitely define the constructor for the struct Ccontact like this:
Contact(const std::string& n, const std::string& p, const std::string& e, const std::string& a)
    : name(n), phone(p), email(e), address(a)
*/
    std::string name;
    std::string phone;
    std::string email;
    std::string address;

//destructor ~Contact is created and it'll print out a message to show when a contact is deleted.
//When an object of the class goes out of scope or is explicitly deleted, the destructor is called, and display the message
    ~Contact() {
        std::cout << "Deleting contact: " << name << std::endl;
    }
};

/*
create a struct named it Node to represent a node in the linked list.
Each node object contains the shared_ptr member named Contact pointing to a Contact object.
The member named 'next', which also points to the next node in the list.
*/
struct Node {
    std::shared_ptr<Contact> contact;
    std::shared_ptr<Node> next;
    //construct Node takes in para and initialize the contact member with value
    //'next' member is initialized to nullptr, meaning no next node
    Node(std::shared_ptr<Contact> c) : contact(c), next(nullptr) {
        std::cout << "Node created. Contact: " << contact->name << std::endl;
    }
//destructor of the Node struct. It'll print out a message to show the contact's name before the node is deleted
    ~Node() {
////When an object of the class goes out of scope or is explicitly deleted, the destructor is called, and display the message
        std::cout << "Deleting node with contact: " << contact->name << std::endl;
    }
};


/*
* 
Create a class to manage the address book data structure:
create a private member 'head' which is the shared_ptr to the first node in the linked list.
The addContact func handle the add a new contact to the address book. 
It creates a new Contact object using make_shared<Contact>() func and sets its properties
Then, creates a new Node with the Contact object and inserts it into the linked list
to sort the input into alphabetical order by name
*/
class AddressBook {
private:
    std::shared_ptr<Node> head; //the first node in the linkedlist to store contacts

public: //public member to handle add new contact into the address book
    void addContact(const std::string& name, const std::string& phone, const std::string& email, const std::string& address) {
        //create new Contact object
        //this will create a shared pointer to a new Contact object and initialize it with the default constructor of the Contact struct
        std::shared_ptr<Contact> newContact = std::make_shared<Contact>();
        //now assigns values for the shared pointer to the paras
        newContact->name = name;
        newContact->phone = phone;
        newContact->email = email;
        newContact->address = address;
        //Create a new Node object
        std::shared_ptr<Node> newNode = std::make_shared<Node>(newContact);
        //check whether the head is nullptr(address book is empty or not) or the name of the new contact is less than the name of the head contact
        //If either are true, the new node is inserted at the beginning of the linked list by updating the next pointer of the new node to point to the current head/updating the head pointer to the new node.
        if (head == nullptr || name < head->contact->name) {
            newNode->next = head;
            head = newNode;
        }
        else { //conditions are not met, insert new contact somewhere 
            std::shared_ptr<Node> current = head;
            while (current->next != nullptr && name > current->next->contact->name) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
    }

    //member func of the class to iterate through the linked list of contacts and displays their details
    void displayAddressBook() {
        //creating a local shared pointer 'current' and initializing it with the head pointer. 
        //This pointer will go rthough the link list start from the head
        std::shared_ptr<Node> current = head;
        // check if the current pointer is nullptr(empty)
        if (current == nullptr) {
            std::cout << "Address book is empty." << std::endl;
        }
        else {
            //create a loop to iterate through each node in the linked list until the current pointer become nullptr/empty
            while (current != nullptr) {
                //accessed through current->contact which is a shared pointer to the Contact object stored in the current node.
                std::cout << "Name: " << current->contact->name << std::endl;
                std::cout << "Phone: " << current->contact->phone << std::endl;
                std::cout << "Email: " << current->contact->email << std::endl;
                std::cout << "Address: " << current->contact->address << std::endl;
                std::cout << std::endl;
                //updates the current pointer to point to the next node in the linked list
                current = current->next;
            }
        }
    }

/*
* create a member func for the class to save file/write the contents of the address book to a file.
employ try and catch block to handle errors.
Must create a folder with a file name then paste in the path and add in the file name in the end, ex: C:\UAT\CSC275-C++\W3\My_Contacts\My_Address.txt
*/
    void saveToFile(const std::string& filename) {
        try {
            //creating a local ofstream object named file with the specified filename
            std::ofstream file(filename);
            if (!file) {
                throw std::runtime_error("Failed to open file for writing: " + filename);
            }

            std::shared_ptr<Node> current = head;
            while (current != nullptr) {
                file << "Name: " << current->contact->name << std::endl;
                file << "Phone: " << current->contact->phone << std::endl;
                file << "Email: " << current->contact->email << std::endl;
                file << "Address: " << current->contact->address << std::endl;
                file << std::endl;
                current = current->next;
            }

            file.close();//to ensure that the changes are saved
            std::cout << "Address book saved to file: " << filename << std::endl;
        }
        //If any exception occurs during the file operations
        //it'll catche the exception using a catch block and prints an error message
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }

    /*
    create a member func for the class to handle load a file
    load data from a file and handles errors
    */
    void loadFromFile(const std::string& filename) {
        try {
            std::ifstream file(filename);//creating a local ifstream object named it file with the specified filename
            if (!file) {
                throw std::runtime_error("Failed to open file for reading: " + filename);
            }

            clearAddressBook();//handle clear data to take in new
            //to store each line of text read from the file during the loading process.
            std::string line;
            //to store a shared pointer to a Contact object that is being loaded from the file.
            std::shared_ptr<Contact> newContact;
            //to store a shared pointer to a Node object that is being created and added to the address book.
            std::shared_ptr<Node> newNode;

            //uses string comparisons (line.find()) to identify the different fields (name, phone, email, address).
            while (std::getline(file, line)) {
                if (line.find("Name: ") == 0) {
                    //make_shared is to ensure proper memory management and allows the shared pointer to be safely shared and accessed across multiple parts of the code.
                    newContact = std::make_shared<Contact>();
                    newContact->name = line.substr(6);//extract a substring from the line starting from the 6th character 
                }
                else if (line.find("Phone: ") == 0) {
                    newContact->phone = line.substr(7);
                }
                else if (line.find("Email: ") == 0) {
                    newContact->email = line.substr(7);
                }
                else if (line.find("Address: ") == 0) {
                    newContact->address = line.substr(9);
                    newNode = std::make_shared<Node>(newContact);
                    addContact(newContact->name, newContact->phone, newContact->email, newContact->address);
                }
            }

            file.close();//it's imprtant to close the file to release sys resources
            std::cout << "Address book loaded from file: " << filename << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }
    //member func to  clears the address book by setting the head pointer to nullptr
    // this will effectively releasing all the memory allocated for the linked list.
    void clearAddressBook() {
        head = nullptr;
    }
};

//-------------------------------------------------------------------------------------------------------------------------

int main() {
    //Create an instance of the AddressBook class and named it addressBook to perform operations 
    AddressBook addressBook;
    //vars to store the user's menu choice
    std::string name, phone, email, address;

    std::cout << "\n\n                  WELCOME TO THE CONTACT ORGANIZER APP" << std::endl;
    cout << "*****************************************************************************************************************\n" << endl;
    cout << "\n                OVERVIEW" << endl;
    cout << "          ========================\n" << endl;

    cout << "Contact Organizer app is a convenient tool for managing your contacts in a digital format." << endl;
    cout << "You can easily add, view, and organize your contacts' names, phone numbers, email addresses, and physical addresses." << endl;
    cout << "The program will automatically organize the name in alphabetical order." << endl;
    cout << "You can save the contact list in your computer and load up the file to view, edit, or add in more info." << endl;
    cout << "Stay organized and keep all your important contacts in one place with this Contact Organizer app." << endl;
    cout << "\n" << endl;
    cout << "                 INSTRUCTION TO USE THE APP" << endl;
    cout << "         ===========================================\n" << endl;
    cout << "From the menu list, select an option and enter the number that associates with the it.\n" << endl;
    cout << "1. Adding contacts to the list:  To add a new contact, choose option 1 and follow the prompts." << endl;
    cout << "2. Viewing the contact list:     Select option 2 to display the entire list." << endl;
    cout << "3. Saving the contact list:      Choose options 3, and paste in the path to a folder, a back slash and the name, ex: \my_contact_list.txt " << endl;
    cout << "4. Loading the contact list:     Select option 4, paste in the path to the folder, back slash, and the name of the file.\n" << endl;
    cout << "5. To exit the program:          Use option 5.\n" << endl;
    cout << "Enjoy the simplicity and organization of the Contact organizer app to help you manage your contacts efficiently and effortlessly.\n" << endl;
    

    //the loop continuously displays the menu and prompts the user for input until the user chooses to exit the program
    while (true) {
        std::cout << "1. Add Contact" << std::endl;
        std::cout << "2. Display Contact List" << std::endl;
        std::cout << "3. Save Contact List to a File" << std::endl;
        std::cout << "4. Load Contact List from a File" << std::endl;
        std::cout << "5. Quit" << std::endl;

        int choice;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(); // Ignore the newline character
        //handle different cases based on the user's choice:
        switch (choice) {
        case 1: {
            std::cout << "Enter the name: ";
            std::getline(std::cin, name);

            std::cout << "Enter the phone number: ";
            std::getline(std::cin, phone);

            std::cout << "Enter the email address: ";
            std::getline(std::cin, email);

            std::cout << "Enter the address: ";
            std::getline(std::cin, address);

            addressBook.addContact(name, phone, email, address);
            std::cout << "Contact added successfully!" << std::endl;
            std::cout << std::endl;
            break;
        }

        case 2:
            std::cout << "Address Book:" << std::endl;
            addressBook.displayAddressBook();
            break;

        case 3: {
            std::string saveFilename;
            std::cout << "Enter the filename to save the address book: ";
            std::getline(std::cin, saveFilename);
            addressBook.saveToFile(saveFilename);
            break;
        }

        case 4: {
            std::string loadFilename;
            std::cout << "Enter the filename to load the address book: ";
            std::getline(std::cin, loadFilename);
            addressBook.loadFromFile(loadFilename);
            break;
        }

        case 5:
            std::cout << "Exiting the program..." << std::endl;
            return 0;

        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
            break;
        }

    }
    std::cout << "\n\n\n" << std::endl;


    return 0;
}

