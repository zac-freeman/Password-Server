#include "passserver.h"
#include <iostream>

using namespace cop4530;

void Menu();

int main()
{
    char userInput;     //stores user input for menu choices
    size_t userSize;    //stores user input for preferred hash size

    char inputFile[100];    //stores user entered name of input file
    char outputFile[100];   //stores user entered name of output file
    char username[100];     //stores user entered username
    char password[100];     //stores user entered password
    char newPass[100];      //stores user entered newPass

    std::cout << "Enter preferred hash table capacity: ";
    std::cin >> userSize;

    PassServer userServer(userSize);    //stores user entered username-password pairs

    std::cout << userServer.capacity() << std::endl;

    do
    {
        Menu();
        std::cin >> userInput;

        switch(userInput)
        {
            case 'l':   std::cout << "Enter password file name to load from: ";
                        std::cin >> inputFile;

                        if (!userServer.load(inputFile))
                            std::cout << "Error: Cannot open file "
                                      << inputFile << "\n";

                        break;

            case 'a':   std::cout << "Enter username: ";
                        std::cin >> username;
                        std::cout << "Enter password: ";
                        std::cin >> password;
                        std::cout << std::endl;

                        if ( userServer.addUser(std::make_pair(username, password)) )
                            std::cout << "User " << username << " added.\n";
                        else
                            std::cout << "*****Error: User already exists. "
                                      << "Could not add user.\n";

                        break;

            case 'r':   std::cout << "Enter username: ";
                        std::cin >> username;

                        if ( userServer.removeUser(username) )
                            std::cout << "User " << username << " deleted.\n";
                        else
                            std::cout << "*****Error: User not found.  "
                                      << "Could not delete user\n";

                        break;

            case 'c':   std::cout << "Enter username    : ";
                        std::cin >> username;
                        std::cout << "Enter password    : ";
                        std::cin >> password;
                        std::cout << std::endl;
                        std::cout << "Enter new password: ";
                        std::cin >> newPass;

                        if (userServer.changePassword(std::make_pair(username, password),newPass))
                            std::cout << "\nPassword changed for user " << username;
                        else
                            std::cout << "*****Error: Could not change user password\n";

                        break;

            case 'f':   std::cout << "Enter username    : ";
                        std::cin >> username;

                        if ( userServer.find(username) )
                            std::cout << "\nUser '" << username << "' found. \n";
                        else
                            std::cout << "\nUser '" << username << "' not found. \n";

                        break;

            case 'd':   userServer.dump();
                        break;

            case 's':   std::cout << "Size of hashtable: "
                                  << userServer.size() << "\n";
                        break;

            case 'w':   std::cout << "Enter password file name to write to: ";
                        std::cin >> outputFile;

                        if ( !userServer.write_to_file(outputFile) )
                            std::cout << "Error: Cannot open file to write\n"; 

                        break;

            case 'x':   break;

            default :   std::cout << "*****Error: Invalid entry.  Try again.\n";
                        break;
        }
    }
    while (userInput != 'x');

    return 0;
}


void Menu()
{
    std::cout << "\n\n";
    std::cout << "l - Load From File" << std::endl;
    std::cout << "a - Add User" << std::endl;
    std::cout << "r - Remove User" << std::endl;
    std::cout << "c - Change User Password" << std::endl;
    std::cout << "f - Find User" << std::endl;
    std::cout << "d - Dump HashTable" << std::endl;
    std::cout << "s - HashTable Size" << std::endl;
    std::cout << "w - Write to Password File" << std::endl;
    std::cout << "x - Exit program" << std::endl;
    std::cout << "\nEnter choice : ";
}
