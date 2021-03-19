#include <iostream>
#include "date.h"

Date strToDate(char * date) {
    if (date ==  NULL) {
        return Date(0,0,0);
    }
    std::string dt = std::string(date);
    std::string y = dt.substr(0, 4), m = std::string(&dt[5]), d = std::string(&dt[7]);
    if (d == "0") {
        d = std::string(&date[6]);
    }
    if (m == "0") {
        m = std::string(&date[8]);
    }
    return Date(atoi(d.c_str()), atoi(m.c_str()), atoi(y.c_str()));
}

bool checkLeap(int yr) {
    if (yr % 400 == 0) {
        return true;
    } else if (yr % 100 == 0) {
        return false;
    } else if (yr % 4 == 0) {
        return true;
    }
    return false;
}

Date::Date() {
    d = 1;
    m = 1;
    y = 2000;
}

Date::Date(const Date &d2) {
    d = d2.d;
    m = d2.m;
    y = d2.y;
}

Date::Date(int _d, int _m, int _y) {
    d = _d;
    m = _m;
    y = _y;
}

std::string Date::returnDate() {
    std::string date = std::to_string(d) + "/" + std::to_string(m) + "/" + std::to_string(y);
    return date;
}

std::string Date::returnSQLDate() {
    std::string date = std::to_string(y) + "-" + std::to_string(m) + "-" + std::to_string(d);
    return date;
}

std::ostream& operator<< (std::ostream& os , const Date& dt) {
    if (dt.d < 10) {
        os << "0";
    }
    os << dt.d << "/";
    if (dt.m < 10) {
        os << "0";
    }
    os << dt.m << "/" << dt.y;
    return os;
}

std::istream& operator>> (std::istream& is, Date &dt) {
    bool isDate = false, isMonth = false;
    do {
        is >> dt.d >> dt.m >> dt.y;
        if (dt.m == 2 && checkLeap(dt.y)) {
            dt.daysOfMonth[1] = 29;
        } else {
            dt.daysOfMonth[1] = 28;
        }
        if (dt.d <= dt.daysOfMonth[dt.m-1]) {
            isDate = true;
        } else {
            isDate = false;
            std::cout << "Invalid Date" << std::endl;
        }
        if (dt.m <= 12) {
            isMonth = true;
        } else {
            isMonth = false;
            std::cout << "Invalid Month" << std::endl;
        }
    } while (!(isDate && isMonth));
    return is;
}

int countLeapYears(Date dt) {
    int years = dt.y;

    if (dt.m <= 2)
        years--;
    return years / 4 - years / 100 + years / 400;
}

int getDifference(Date &dt1, Date& dt2) {
    const int monthDays[12]
    = { 31, 28, 31, 30, 31, 30, 
       31, 31, 30, 31, 30, 31 };

    long int n1 = dt1.y * 365 + dt1.d;

    for (int i = 0; i < dt1.m - 1; i++)
        n1 += monthDays[i];

    n1 += countLeapYears(dt1);

    long int n2 = dt2.y * 365 + dt2.d;
    for (int i = 0; i < dt2.m - 1; i++)
        n2 += monthDays[i];
    n2 += countLeapYears(dt2);

    return (n2 - n1);
}

bool Date::operator!=(Date dt2) {
    if (d != dt2.d || m != dt2.m || y != dt2.y) {
        return false;
    }
    return true;
}

bool Date::operator== (Date dt2) {
    if (d == dt2.d && m == dt2.m && y == dt2.y) {
        return true;
    }
    return false;
}

Date getDate() {
    Date d;
    std::cin >> d;
    return d;
}