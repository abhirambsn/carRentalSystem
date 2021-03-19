#ifndef AUTH_H
#define AUTH_H

#include <iostream>
 

class User {
    private:
        std::string username, password;
        int shift;
    public:
        User();
        User(const User& user2);
        User(std::string, std::string, int shift);
        void changePassword(std::string, std::string);
        friend bool verifyCredentials(std::string, std::string, int);
        friend void save(User);   
};

extern std::string currentUser;

std::string obfuscate(std::string, int);

void save(User);

User signUp();
bool verifyCredentials(std::string, std::string, int);
bool logIn();

#endif