#include <iostream>
#include <sqlite3.h>
#include "customer.h"
#include "../Date/date.h"

Customer::Customer() {
    customerID = -1;
    name = "John Doe";
    phoneNumber = "0000000000";
    eMail = "john.doe@example.com";
    resAddress = "N/A";
    idProofNo = "000000000000";
    dor = todayDate;
}

Customer::Customer(const Customer &c2) {
    customerID = c2.customerID;
    name = c2.name;
    phoneNumber = c2.phoneNumber;
    eMail = c2.eMail;
    resAddress = c2.resAddress;
    idProofNo = c2.idProofNo;
    dor = c2.dor;
}

Customer::Customer(std::string _name, std::string _phoneNumber, std::string _eMail, std::string _resAddress, std::string _idProofNo) {
    customerID = customer_id;
    name = _name;
    phoneNumber = _phoneNumber;
    eMail = _eMail;
    resAddress = _resAddress;
    idProofNo = _idProofNo;
    dor = todayDate;
}

int Customer::getNoCustomers() {
    return customer_id;
}

void Customer::customerRegisterd() {
    customer_id = getCurrentCustomerID();
}

void Customer::customerDeleted() {
    customer_id = getCurrentCustomerID();
}

std::ostream& operator<< (std::ostream& os, const Customer& cust) {
    os << "\t\t\t\tCustomer ID: " << cust.customerID << std::endl;
    os << "\n\t\t\tCustomer Name: " << cust.name << std::endl;
    os << "\n\tEmail: " << cust.eMail << "\t" << "Phone Number: " << cust.phoneNumber << std::endl;
    os << "\n\tResidential Address: " << cust.resAddress << std::endl;
    os << "\n\tID Proof Number: " << cust.idProofNo << std::endl;
    os << "\n\tDate of Registration: " << cust.dor << std::endl;
    return os;
    
}

Customer::~Customer() {
    
}

Customer registerCustomer() {
    std::string name, phoneNumber, eMail, resAddress, idProofNo;
    std::cout << "\t\t\t\tCustomer ID: " << customer_id << std::endl;
    std::cout << "\t\t\t\tEnter Name: ";
    std::getline(std::cin, name);
    std::cout << "\t\t\t\tEnter Phone Number: ";
    std::cin >> phoneNumber;
    std::cin.ignore();
    while (phoneNumber.length() != 10) {
        std::cout << "\t\t\t\tWrong Phone Number format Re-Enter: " << std::endl;
        std::cout << "\t\t\t\tEnter Phone Number: ";
        std::cin >> phoneNumber;
        std::cin.ignore();
    }

    std::cout << "\t\t\t\tEnter Email: ";
    std::cin >> eMail;
    std::cin.ignore();
    std::cout << "\t\t\t\tEnter Residential Address: ";
    std::getline(std::cin, resAddress);
    std::cout << "\t\t\t\tEnter ID Proof Number: ";
    std::cin >> idProofNo;
    std::cin.ignore();
    Customer c(name, phoneNumber, eMail, resAddress, idProofNo);
    
    return c;
}

void save(Customer c) {
    sqlite3 *DB;
    std::string sql = "INSERT INTO customers VALUES (" + std::to_string(c.customerID) +  ", \"" +  c.name + "\", \"" + c.eMail + "\", \"" + c.phoneNumber + "\", \"" + c.resAddress + "\", \"" + c.idProofNo + "\", \"" + c.dor.returnSQLDate() + "\");";
    int connection = 0;
    connection = sqlite3_open("database.db", &DB);
    char *errMsg;
    connection = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errMsg);
    if (connection != SQLITE_OK) {
        std::cerr << "Error Inserting Record" << std::endl;
        std::cerr << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Customer Registration Successful" << std::endl;
    }
    c.customerRegisterd();
    sqlite3_close(DB);
}

Customer getCustomer(int cus_id) {
    sqlite3 *DB;
    int connection = 0;
    connection = sqlite3_open("database.db", &DB);
    char *errMsg;
    sqlite3_stmt* statement;
    std::string sql = "SELECT * FROM customers";
    if (cus_id != 0) {
        sql += " WHERE cust_id = " + std::to_string(cus_id) + ";";
    }
    sqlite3_prepare(DB, sql.c_str(), -1, &statement, 0);
    Customer c;
    
    while (sqlite3_step(statement) != SQLITE_DONE) {
        c.customerID = sqlite3_column_int(statement, 0);
        c.name = (char *) sqlite3_column_text(statement, 1);
        c.eMail = (char *) sqlite3_column_text(statement, 2);
        c.phoneNumber = (char *) sqlite3_column_text(statement, 3);
        c.resAddress = (char *) sqlite3_column_text(statement, 4);
        c.idProofNo = (char *) sqlite3_column_text(statement, 5);
        c.dor = strToDate((char *) sqlite3_column_text(statement, 6));
    }
    
    sqlite3_close(DB);

    return c;
}

void displayAllCustomers() {
    sqlite3 *DB;
    int connection = 0;
    connection = sqlite3_open("database.db", &DB);
    char *errMsg;
    sqlite3_stmt* statement;
    std::string sql = "SELECT * FROM customers;";
    sqlite3_prepare(DB, sql.c_str(), -1, &statement, 0);
    Customer c;
    
    while (sqlite3_step(statement) != SQLITE_DONE) {
        c.customerID = sqlite3_column_int(statement, 0);
        c.name = (char *) sqlite3_column_text(statement, 1);
        c.eMail = (char *) sqlite3_column_text(statement, 2);
        c.phoneNumber = (char *) sqlite3_column_text(statement, 3);
        c.resAddress = (char *) sqlite3_column_text(statement, 4);
        c.idProofNo = (char *) sqlite3_column_text(statement, 5);
        c.dor = strToDate((char *) sqlite3_column_text(statement, 6));

        std::cout << c << '\n' << std::endl;
    }

    sqlite3_close(DB);
}


int Customer::getCustID() {
    return customerID;
}

int getCurrentCustomerID() {
    sqlite3 *DB;
    int connection = 0;
    connection = sqlite3_open("database.db", &DB);
    char *errMsg;
    sqlite3_stmt* statement;
    std::string sql = "SELECT cust_id FROM customers;";
    int id = 0;
    sqlite3_prepare(DB, sql.c_str(), -1, &statement, 0);
    while(sqlite3_step(statement) != SQLITE_DONE) {
        id = sqlite3_column_int(statement, 0);
    }
    sqlite3_close(DB);
    return id + 1;
}

void sqlExec(std::string sql) {
    sqlite3 *Database;
    int connection = 0;
    connection = sqlite3_open("database.db", &Database);
    char *errMsg;
    connection = sqlite3_exec(Database, sql.c_str(), NULL, 0, &errMsg);
    if (connection != SQLITE_OK) {
        std::cerr << "Error Updating Record" << std::endl;
        std::cerr << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Updation Successful" << std::endl;
    }
    sqlite3_close(Database);
}

void modify(int cust_id, std::string name = "", std::string email = "", std::string phNo = "", std::string resAdd = "") {
    std::string sql = "UPDATE customers SET ", query = "", footStr = " WHERE cust_id = " + std::to_string(cust_id) + ";";
    if (name != "") {
        query = sql + "name = \"" + name + "\"" + footStr;
        sqlExec(query);
    }
    if (email != "") {
        query = sql + "email = \"" + email + "\"" + footStr;
        sqlExec(query);
    }
    if (phNo != "") {
        query = sql + "phNo = \"" + phNo + "\"" + footStr;
        sqlExec(query);
    }
    if (resAdd != "") {
        query = sql + "resAddress = \"" + resAdd + "\"" + footStr;
        sqlExec(query);
    }
}