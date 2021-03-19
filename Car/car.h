#ifndef CAR_H
#define CAR_H

#include <iostream>
#include "../Date/date.h"

int getCurrentCarID();

static int car_id = getCurrentCarID();

class Car {
    private:
        int carID;
    protected:
        std::string  regNo, company, model;
        bool isRented;
        Date doa;
        double costPerDay;
    public:
        Car();
        Car(const Car &car2);
        Car(std::string, std::string, std::string, double);
        int returnSQLData();
        static int getNoOfCars();
        static void deleteCar();
        static void carAdded();
        int getCarID();
        double getCost();
        friend void save(Car c);
        friend Car getCar(int);
        friend void displayAllCars();
        friend std::ostream& operator<< (std::ostream& os, const Car& car);
        ~Car();
};

Car getCarDetails();
Car getCar(int);
void displayAllCars();

void modify(int, double);

#endif