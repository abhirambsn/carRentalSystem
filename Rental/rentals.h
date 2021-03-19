#ifndef RENTALS_H
#define RENTALS_H

#include <iostream>
#include "../Date/date.h"
#include "../Car/car.h"
#include "../Customer/customer.h"

int getCurrentRentalID();
static int rental_id = getCurrentRentalID();

class Rental {
    private:
        int rentalID;
        Date rentDate, returnDate;
        Customer cust;
        Car car;
        double costPaid;
        bool isPaid;
    public:
        Rental();
        Rental(const Rental&);
        Rental(Date, Customer, Car);
        double calculateBill();
        bool chkPaid();
        friend void save(Rental);
        friend Rental getRental(int);
        friend std::ostream& operator<< (std::ostream&, const Rental&);
};

Rental newRental();
void save(Rental);
Rental getRental(int);

#endif