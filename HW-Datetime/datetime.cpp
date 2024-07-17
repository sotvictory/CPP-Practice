#include <iostream>
#include <iomanip>
#include <cmath>

class DateTimeSpan {
private:
    double timeSpan;

public:
    DateTimeSpan(double ts = 0.0) : timeSpan(ts) {}

    DateTimeSpan(int days = 0, int hours = 0, int minutes = 0, int seconds = 0)
        : timeSpan(days + hours / 24.0 + minutes / (24.0 * 60.0) + seconds / (24.0 * 3600.0)) {}

    double getTimeSpan() const {
        return timeSpan;
    }    

    void printTimeSpan() const {
        int totalDays = static_cast<int>(timeSpan);
        double remainingTime = timeSpan - totalDays;

        int totalHours = static_cast<int>(remainingTime * 24);
        remainingTime -= totalHours / 24.0;

        int totalMinutes = static_cast<int>(remainingTime * 24 * 60);
        remainingTime -= totalMinutes / (24.0 * 60.0);

        int totalSeconds = static_cast<int>(remainingTime * 24 * 3600);

        std::cout << "Days: " << totalDays << ", Time: " 
                  << std::setfill('0') << std::setw(2) << totalHours 
                  << ':' << std::setw(2) << totalMinutes 
                  << ':' << std::setw(2) << totalSeconds 
                  << std::endl;
    }

    DateTimeSpan operator-() const {
        return DateTimeSpan(-timeSpan);
    }

    DateTimeSpan operator-(const DateTimeSpan& other) const {
        return DateTimeSpan(timeSpan - other.timeSpan);
    }

    DateTimeSpan operator+(const DateTimeSpan& other) const {
        return DateTimeSpan(timeSpan + other.timeSpan);
    }
};

class DateTime {
private:
    double datetime;

public:
    DateTime(double dt = 0.0) : datetime(dt) {}

    void setDateTime(double dt) {
        datetime = dt;
    }

    int getDay() const {
        return static_cast<int>(std::floor(datetime >= 0 ? datetime : datetime - 1)) + 1;
    }

    double getTime() const {
        return (datetime < 0 ? (std::abs(datetime - getDay())) : (std::abs(datetime - getDay() + 1)));
    }

    void printDateTime() const {
        int day = getDay();
        double time = getTime();

        int year = 2000;
        int month = 1;
        int daysInMonth;
        int daysPrevMonth;

        while (true) {
            daysInMonth = 31;
            daysPrevMonth = 30;
            if (month == 4 || month == 6 || month == 9 || month == 11) {
                daysInMonth = 30;
            } else if (month == 2) {
                daysInMonth = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ? 29 : 28;
            }
            
            if (month == 3) {
                daysPrevMonth = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ? 29 : 28;
            } else if (month == 1 || month == 2 || month == 4 || month == 6 ||month == 8 || month == 9 || month == 11) {
                daysPrevMonth = 31;
            }
            
            if (day >= 0) {
                if (day <= daysInMonth) {
                    break;
                }
                day -= daysInMonth;
                ++month;
                if (month > 12) {
                    ++year;
                    month = 1;
                }
            } else {
                --month;
                day += daysPrevMonth;
                if (month < 1) {
                    --year;
                    month = 12;
                }
                if (day >= 0) day++;
            }
        }

        int totalSeconds = static_cast<int>(time * 24 * 60 * 60);
        int hours = totalSeconds / 3600;
        int minutes = (totalSeconds % 3600) / 60;
        int seconds = totalSeconds % 60;

        std::cout << std::setfill('0') << std::setw(2) << day << '.' << std::setw(2) << month << '.' << year << ' '
                  << std::setw(2) << hours << ':' << std::setw(2) << minutes << ':' << std::setw(2) << seconds << std::endl;
    }

    DateTimeSpan operator-(const DateTime &other) const {
        return DateTimeSpan(datetime - other.datetime);
    }

    DateTime operator+(const DateTimeSpan &span) const {
        return DateTime(datetime + span.getTimeSpan());
    }

    DateTime operator-(const DateTimeSpan &span) const {
        return DateTime(datetime - span.getTimeSpan());
    }
};

int main() {
    /*
    DateTimeSpan s(-0.75);
    s.printTimeSpan();
    DateTimeSpan s1(1.0);
    s1.printTimeSpan();
    s = s + s1;
    s.printTimeSpan();
    */

    DateTime d(0.5);
    d.printDateTime();
    DateTime d1(0.5);
    d1.printDateTime();
    DateTimeSpan s = d - d1;
    s.printTimeSpan();
    std::cout << '\n';

    DateTime d2(0.5);
    d2.printDateTime();
    DateTimeSpan s1(1.0);
    s1.printTimeSpan();
    DateTime d3 = d2 - s1;
    d3.printDateTime();
    std::cout << '\n';

    DateTimeSpan sp(2, 2, 2, 2);
    sp.printTimeSpan();

    DateTime dt0(0.0);
    //dt0.printDateTime(); // 01.01.2000 00:00:00

    DateTime dt1(1.0);
   // dt1.printDateTime(); // 2.01.2000 00:00:00 

    DateTime dt2(2.1);
    //dt2.printDateTime(); // 3.01.2000 02:24:00

    DateTime dt3(1.75);
    //dt3.printDateTime(); // 02.01.2000 18:00:00

    DateTime dt4(-1.0);
    //dt4.printDateTime(); // 31.12.1999 00:00:00

    DateTime dt5(-1.5);
    //dt5.printDateTime(); // 30.12.1999 12:00:00

    DateTime dt6(-1.25);
    //dt6.printDateTime(); // 30.12.1999 18:00:00

    DateTime dt7(-1.1);
    //dt7.printDateTime(); // 30.12.1999 21:36:00

    return 0;
}
