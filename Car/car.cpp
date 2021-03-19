#include <iostream>
#include <sqlite3.h>
#include "car.h"
#include "../Date/date.h"

Car::Car() {
    carID = -1;
    regNo = "N/A";
    company = "N/A";
    model = "N/A";
    isRented = false;
    doa = todayDate;
    costPerDay = 0;
}

int Car::returnSQLData() {
    return carID;
}

Car::Car(const Car &car2) {
    carID = car2.carID;
    regNo = car2.regNo;
    company = car2.company;
    model = car2.model;
    isRented = car2.isRented;
    doa = car2.doa;
    costPerDay = car2.costPerDay;
}

Car::Car(std::string _regNo, std::string _company, std::string _model, double _costPerDay) {
    carID = car_id;
    regNo = _regNo;
    company = _company;
    model = _model;
    isRented = false;
    doa = todayDate;
    costPerDay = _costPerDay;
}

int Car::getNoOfCars() {
    return car_id;
}

double Car::getCost() {
    return costPerDay;
}

std::ostream& operator<< (std::ostream& os, const Car& car) {
    os << "\t\t\t\tCar ID: " << car.carID << std::endl;
    os << "\n\t\tRegistration No: " << car.regNo << std::endl;
    os << "\n\t\tCompany and Model: " << car.company << " " << car.model << std::endl;
    os << "\n\t\tIs Rented: " << car.isRented << std::endl;
    os << "\n\t\tCost Per Day: " << car.costPerDay << std::endl;
    os << "\n\t\tDate of Addition: " << car.doa;

    return os;
}

void Car::carAdded() {
    car_id = getCurrentCarID();
}

void save(Car c) {
    sqlite3 *DB;
    std::string sql = "INSERT INTO cars VALUES (" + std::to_string(c.carID) +  ", \"" +  c.regNo + "\", \"" + c.company + "\", \"" + c.model + "\", " + std::to_string(c.isRented) + ", \"" + c.doa.returnSQLDate() + "\", " + std::to_string(c.costPerDay) + ");";
    int connection = 0;
    connection = sqlite3_open("database.db", &DB);
    char *errMsg;
    connection = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errMsg);
    if (connection != SQLITE_OK) {
        std::cerr << "Error Inserting Record" << std::endl;
        std::cerr << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Record Save Successful" << std::endl;
    }
    c.carAdded();
    sqlite3_close(DB);
}

Car getCar(int c_id) {
    sqlite3 *DB;
    int connection = 0;
    connection = sqlite3_open("database.db", &DB);
    char *errMsg;
    sqlite3_stmt* statement;
    std::string sql = "SELECT * FROM cars";
    if (c_id != 0) {
        sql += " WHERE car_id = " + std::to_string(c_id) + ";";
    }
    sqlite3_prepare(DB, sql.c_str(), -1, &statement, 0);
    Car c;
    
    while (sqlite3_step(statement) != SQLITE_DONE) {
        c.carID = sqlite3_column_int(statement, 0);
        c.regNo = (char *) sqlite3_column_text(statement, 1);
        c.company = (char *) sqlite3_column_text(statement, 2);
        c.model = (char *) sqlite3_column_text(statement, 3);
        c.isRented = sqlite3_column_int(statement, 4);
        c.doa = strToDate((char *) sqlite3_column_text(statement, 5));
        c.costPerDay = sqlite3_column_double(statement, 6);
    }

    sqlite3_close(DB);

    return c;
}

void displayAllCars() {
    sqlite3 *DB;
    int connection = 0;
    connection = sqlite3_open("database.db", &DB);
    char *errMsg;
    sqlite3_stmt* statement;
    std::string sql = "SELECT * FROM cars;";
    sqlite3_prepare(DB, sql.c_str(), -1, &statement, 0);
    Car c;
    
    while (sqlite3_step(statement) != SQLITE_DONE) {
        c.carID = sqlite3_column_int(statement, 0);
        c.regNo = std::string((char *) sqlite3_column_text(statement, 1));
        c.company = std::string((char *) sqlite3_column_text(statement, 2));
        c.model = std::string((char *) sqlite3_column_text(statement, 3));
        c.isRented = sqlite3_column_int(statement, 4);
        c.doa = strToDate((char *) sqlite3_column_text(statement, 5));
        c.costPerDay = sqlite3_column_double(statement, 6);

        std::cout << c << '\n' << std::endl;
    }

    sqlite3_close(DB);
}

int getCurrentCarID() {
    sqlite3 *DB;
    int connection = 0;
    connection = sqlite3_open("database.db", &DB);
    sqlite3_stmt* statement;
    std::string sql = "SELECT car_id FROM cars;";
    int id = 0;
    sqlite3_prepare(DB, sql.c_str(), -1, &statement, 0); // 
    while(sqlite3_step(statement) != SQLITE_DONE) {
        id = sqlite3_column_int(statement, 0);
    }

    sqlite3_close(DB);
    return id + 1;
}

int Car::getCarID() {
    return carID;
}

void Car::deleteCar() {
    car_id = getCurrentCarID();
}

Car::~Car() {
    
}

Car getCarDetails() {
    std::string rNo, comp, mod;
    double cost;
    std::cout << "\t\t\t\tCar ID: " << Car::getNoOfCars() << std::endl;
    std::cout << "\n\t\t\tEnter Registration Number: ";
    std::getline(std::cin, rNo);
    std::cout << "\n\t\t\tEnter Company: ";
    std::getline(std::cin, comp);
    std::cout << "\n\t\t\tEnter Model: ";
    std::getline(std::cin, mod);
    std::cout << "\n\t\t\tEnter Rent Cost per Day: ";
    std::cin >> cost;
    std::cin.ignore();
    Car c(rNo, comp, mod, cost);
    return c;
}

void modify(int car_id, double newCost) {
    std::string sql = "UPDATE cars SET cost_per_day = " + std::to_string(newCost) + " WHERE car_id = " + std::to_string(car_id) + ";";
    sqlite3 *Database;
    int connection = 0;
    connection = sqlite3_open("database.db", &Database);
    char *errMsg;
    connection = sqlite3_exec(Database, sql.c_str(), NULL, 0, &errMsg);
    if (connection != SQLITE_OK) {
        std::cerr << "\t\t\t\tError Updating Record" << std::endl;
        std::cerr << "\t\t\t\t" << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "\t\t\t\tUpdation Successful" << std::endl;
    }
    sqlite3_close(Database);
}
