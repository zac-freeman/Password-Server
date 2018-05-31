//Written by Zac Freeman

#include "passserver.h"

//inputs username-password pairs from file to server
bool PassServer::load(const char *filename)
{   return ( server.load(filename) ); }

//outputs username-password pairs from server to file
bool PassServer::write_to_file(const char *filename) const
{   return ( server.write_to_file(filename) ); }

//adds username-password pair kv to server
//if there is a username-password pair already with the username, false is returned
//otherwise, true is returned
bool PassServer::addUser(const std::pair<std::string, std::string> & kv)
{
    if ( find(kv.first) )
        return false;

    std::pair<std::string, std::string> encryptedPair;
    encryptedPair = std::make_pair( kv.first, encrypt(kv.second) );
    server.insert(encryptedPair);
    return true;
}

//move version of addUser
bool PassServer::addUser(std::pair<std::string, std::string> && kv)
{
    if ( find(kv.first) )
        return false;

    kv.second = encrypt(kv.second);
    server.insert( std::move(kv) );
    return true;
}

//removes pair with username k if it exists
//returns true if pair is removed, returns false otherwise
bool PassServer::removeUser(const std::string & k)
{   return ( server.remove(k) ); }

//changes the password of an existing username-password pair
//if the username-password pair is not in the server, returns false
//if the old password and the new password are the same, returns false
//otherwise, returns true
bool PassServer::changePassword(const std::pair<std::string, std::string> & kv,
                                const std::string & newPass)
{
    if (kv.second == newPass)
        return false;

    std::pair<std::string, std::string> encryptedPair;
    encryptedPair = std::make_pair( kv.first, encrypt(kv.second) );

    if ( !server.match(encryptedPair) )
        return false;

    encryptedPair = std::make_pair( kv.first, encrypt(newPass) );
    server.insert(encryptedPair);
    return true;
}

//returns true if username user is found in the server, returns false otherwise
bool PassServer::find(const std::string & user) const
{   return ( server.contains(user) ); }

//prints the contents of the server to the screen
void PassServer::dump() const
{   server.dump(); }

//returns number of elements in the server
size_t PassServer::size() const
{   return ( server.size() ); }

//returns the number of pair lists in the server
size_t PassServer::capacity() const
{   return ( server.capacity() ); }

//returns an encrypted version of str
//code copied from given code
std::string PassServer::encrypt(const std::string & str)
{
    char salt[] = "$1$########";

    return crypt(str.c_str(), salt);
}
