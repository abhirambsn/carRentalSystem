#include <iostream>
#include <sqlite3.h>
#include "rentals.h"

int getCurrentRentalID() {
    sqlite3 *DB;
    int conn = 0;
    conn = sqlite3_open("database.db", &DB);
    sqlite3_stmt *statement;
    std::string sql = "SELECT rental_id FROM rental;";
    sqlite3_prepare(DB, sql.c_str(), -1, &statement, 0);
    int id = 0;

    while(sqlite3_step(statement) != SQLITE_DONE) {
        id = sqlite3_column_int(statement, 0);
    }
    sqlite3_close(DB);
    return id + 1;
}

Rental::Rental() {
    rentalID = -1;
    rentDate = Date(1, 1, 2000);
    returnDate = Date(0,0,0);
    Customer c;
    cust = c;
    Car cr;
    car = cr;
    costPaid = 0;
    isPaid = false;
}

Rental::Rental(const Rental& rental2) {
    rentalID = rental2.rentalID;
    rentDate = rental2.rentDate;
    returnDate = rental2.returnDate;
    cust = rental2.cust;
    car = rental2.car;
    costPaid = rental2.costPaid;
    isPaid = false;
}

Rental::Rental(Date _rentDate, Customer _cust, Car _car) {
    rentalID = rental_id;
    rentDate = _rentDate;
    returnDate = Date(0,0,0);
    cust = _cust;
    car = _car;
    costPaid = 0;
    isPaid = false;
}

bool Rental::chkPaid() {
    if (isPaid) {
        return true;
    }
    return false;
}

double Rental::calculateBill() {
    returnDate = todayDate;
    int noOfdays = getDifference(rentDate, returnDate);
    costPaid = car.getCost()*noOfdays;
    std::string sql = "UPDATE rental SET dateOfReturn = \"" + returnDate.returnSQLDate() + "\", cost = " + std::to_string(costPaid) + ", isPaid = true WHERE rental_id = " + std::to_string(rentalID) + ";";
    sqlite3 *DB;
    int connection = 0;
    connection = sqlite3_open("database.db", &DB);
    char *errMsg;
    connection = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errMsg);
    if (connection != SQLITE_OK) {
        std::cerr << "DB Error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    sql = "UPDATE cars SET isRented = false WHERE car_id = " + std::to_string(car.getCarID()) + ";";
    connection = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errMsg);
    if (connection != SQLITE_OK) {
        std::cerr << "DB Error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    sqlite3_close(DB);
    return costPaid;
}

std::ostream& operator<< (std::ostream& os, const Rental& rental) {
    os << "\t\t\t\tRental ID: " << rental.rentalID << std::endl;
    os << "\nRent Date: " << rental.rentDate << "\t\t\t\t";
    os << "Return Date: " << rental.returnDate << std::endl;
    // if ((Date) rental.returnDate != Date(0, 0, 0)) {
    //     os << rental.returnDate << std::endl;
    // } else {
    //     os << "N/A" << std::endl;
    // }
    os << "\n================================ Customer Details ===============================\n" << std::endl;
    os << rental.cust << std::endl;
    os << "\n================================ Car Detials ==================================\n" << std::endl;
    os << rental.car;

    if (rental.isPaid) {
        os << std::endl;
        os << "\n============================= Payment Detials ===============================\n" << std::endl;
        os << "Rent Paid: YES\t\t\t\tCost Paid: " << rental.costPaid;
    }

    return os;
}

void save(Rental r) {
    std::string sql = "INSERT INTO rental(rental_id, cust_id, car_id, dateOfRent, dateOfReturn, cost, isPaid) VALUES (" + std::to_string(r.rentalID) + ", " + std::to_string(r.cust.getCustID()) + ", " + std::to_string(r.car.getCarID()) + ", \"" + r.rentDate.returnSQLDate() + "\", NULL, " + std::to_string(r.costPaid) + ", false);";
    sqlite3 *DB;
    int conn = 0;
    conn = sqlite3_open("database.db", &DB);
    char *errMsg;
    conn = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errMsg);
    if (conn != SQLITE_OK) {
        std::cerr << "Error Inserting Record" << std::endl;
        std::cerr << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        sql = "UPDATE cars SET isRented = true WHERE car_id = " + std::to_string(r.car.getCarID()) + ";";
        conn = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errMsg);
        if (conn != SQLITE_OK) {
            std::cerr << "Error Inserting Record" << std::endl;
            std::cerr << errMsg << std::endl;
            sqlite3_free(errMsg);
        } else {
            std::cout << "Record Save Successful" << std::endl;
        }
    }
    rental_id = getCurrentRentalID();
    sqlite3_close(DB);
}

Rental getRental(int r_id) {
    sqlite3 *DB;
    int conn = 0;
    conn = sqlite3_open("database.db", &DB);
    sqlite3_stmt *statement;
    std::string sql = "SELECT * FROM rental WHERE rental_id = " + std::to_string(r_id) + ";";
    sqlite3_prepare(DB, sql.c_str(), -1, &statement, 0);

    Rental r;

    while(sqlite3_step(statement) != SQLITE_DONE) {
        r.rentalID = sqlite3_column_int(statement, 0);
        r.cust = getCustomer(sqlite3_column_int(statement, 1));
        r.car = getCar(sqlite3_column_int(statement, 2));
        r.rentDate = strToDate((char *) sqlite3_column_text(statement, 3));
        r.returnDate = strToDate((char *) sqlite3_column_text(statement, 4));
        r.costPaid = sqlite3_column_double(statement, 5);
        r.isPaid = sqlite3_column_int(statement, 6);
    }

    sqlite3_close(DB);
    return r; 
}


Rental newRental() {
    int cust_id, car_id;
    std::cout << "\t\t\t\tRental ID: " << rental_id << std::endl;
    std::cout << "\t\t\t\tEnter Customer ID: ";
    std::cin >> cust_id;
    std::cin.ignore();
    Customer cst = getCustomer(cust_id);
    std::cout << "\t\t\t\tEnter Car ID: ";
    std::cin >> car_id;
    std::cin.ignore();
    Car cr = getCar(car_id);
    std::cout << "\t\t\t\tRent Date: " << todayDate << std::endl;
    Rental r(todayDate, cst, cr);

    return r;
}