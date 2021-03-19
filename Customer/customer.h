#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <iostream>
#include "../Date/date.h"

int getCurrentCustomerID();
static int customer_id = getCurrentCustomerID();

class Customer {
    private:
        int customerID;
    protected:
        std::string name, phoneNumber, eMail, resAddress, idProofNo;
        Date dor;
    public:
        Customer();
        Customer(std::string, std::string, std::string, std::string, std::string);
        Customer(const Customer &c2);
        friend void save(Customer);
        friend Customer getCustomer(int);
        friend void displayAllCustomers();
        static int getNoCustomers();
        static void customerRegisterd();
        static void customerDeleted();
        int getCustID();
        friend std::ostream& operator<< (std::ostream& os, const Customer& cust);
        ~Customer();

};

Customer registerCustomer();
void save(Customer);
Customer getCustomer(int);
void displayAllCustomers();
void modify(int, std::string, std::string, std::string, std::string);

#endif