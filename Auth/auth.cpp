#include <iostream>
#include <sqlite3.h>
#include <unistd.h>
#include <cstring>
#include "auth.h"

std::string obfuscate(std::string text, int s) {
    std::string result = ""; 
    for (int i=0;i<text.length();i++) { 
        if (isupper(text[i])) {
            result += char(int(text[i]+s-65)%26 +65); 
        } else {
            result += char(int(text[i]+s-97)%26 +97); 
        }
    }  
    return result; 
}

User::User() {
    username = "john";
    password = "doe";
    shift = 2;
    password = obfuscate(password, shift);
}

User::User(const User& user2) {
    username = user2.username;
    password = user2.password;
    shift = user2.shift;
}

User::User(std::string _username, std::string _password, int _shift) {
    username = _username;
    password = _password;
    shift = _shift;
    password = obfuscate(password, shift);
}

bool verifyCredentials(std::string username, std::string password, int shift) {
    std::string query = "SELECT password from users WHERE username = \"" + username + "\";";
    sqlite3 *DB;
    int connection = 0;
    connection = sqlite3_open("database.db", &DB);
    char *errMsg;
    sqlite3_stmt* statement;
    sqlite3_prepare(DB, query.c_str(), -1, &statement, 0);

    std::string passwd;
    while(sqlite3_step(statement) != SQLITE_DONE) {
        passwd = (char *) sqlite3_column_text(statement, 0);
    }
    if (passwd != obfuscate(password, shift)) {
        return false;
    } else {
        return true;
    }
}

void save(User u) {
    sqlite3 *DB;
    std::string sql = "INSERT INTO users VALUES (\"" + u.username +  "\", \"" +  u.password + "\", " + std::to_string(u.shift) + ");";
    int connection = 0;
    connection = sqlite3_open("database.db", &DB);
    char *errMsg;
    connection = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errMsg);
    if (connection != SQLITE_OK) {
        std::cerr << "Error Registering User" << std::endl;
        std::cerr << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "User Registration Successful" << std::endl;
    }
    sqlite3_close(DB);
}

User signUp() {
    std::string username;
    char *passwd, *rPass;
    int shift;
    std::cout << "Enter New Username: ";
    std::getline(std::cin, username);
    passwd = getpass("Enter New Password: ");
    do {
        rPass = getpass("Confirm Password: ");
    } while (passwd != rPass);
    std::cout << "Enter Key: ";
    std::cin >> shift;
    std::cin.ignore();
    User u(username, passwd, shift);

    return u;
}

bool logIn() {
    std::string username;
    std::cout << "Username: ";
    std::getline(std::cin, username);
    char *password = getpass("Password: ");
    int shift;
    std::cout << "Key: ";
    std::cin >> shift;
    std::cin.ignore();

    if (verifyCredentials(username, std::string(password), shift)) {
        currentUser = username;
        return true;
    }
    return false;
}