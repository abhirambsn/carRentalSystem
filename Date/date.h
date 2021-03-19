#ifndef DATE_H
#define DATE_H

#include <iostream>

class Date {
    private:
        int d, m, y;
        int daysOfMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    public:
        Date();
        Date(const Date &d2);
        Date(int, int, int);
        std::string returnDate();
        std::string returnSQLDate();
        friend int getDifference(Date& , Date&);
        friend int countLeapYears(Date dt);
        bool operator!= (Date dt2);
        bool operator== (Date dt2);
        friend std::ostream& operator<< (std::ostream& os, const Date& dt);
        friend std::istream& operator>> (std::istream& is, Date &dt);
};

extern Date todayDate;

Date getDate();

Date strToDate(char *);

int countLeapYears(Date dt);

int getDifference(Date&, Date&);


#endif