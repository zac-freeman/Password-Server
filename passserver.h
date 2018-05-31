//Written by Zac Freeman

#include "hashtable.h"
#include <unistd.h>
#include <crypt.h>

class PassServer
{

    public:
    //constructor
    PassServer(size_t size = 101) : server(size) {};

    //destructor
    ~PassServer() {};

    //loads encrypted password file into server
    bool load(const char *filename);

    //saves username-password pairs to file
    bool write_to_file(const char *filename) const;

    //add user with name-password pair kv and remove user with name k
    bool addUser(const std::pair<std::string, std::string> & kv);
    bool addUser(std::pair<std::string, std::string> && kv);
    bool removeUser(const std::string & k);

    //changes password of username-password pair to newPass
    bool changePassword(const std::pair<std::string, std::string> & kv,
                        const std::string & newPass);

    //checks is user exists in server
    bool find(const std::string & user) const;

    //prints contents of server to screen
    void dump() const;    

    //returns number of username-password pairs in server
    size_t size() const;

    //returns the number of pair lists in the server
    size_t capacity() const;

    private:
        //PassServer hash table implemented using HashTable
        //stores username, encrypted password pairs
        typename cop4530::HashTable<std::string, std::string> server;

        //encrypts passwords
        //each plaintext password is encypted before it is used with server
        std::string encrypt(const std::string & str);
};
