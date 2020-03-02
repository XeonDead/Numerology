#ifndef NUMEROLOGYDAYNUMBER_H
#define NUMEROLOGYDAYNUMBER_H

#include <QtCore/QObject>
#include <QtCore/QDate>

namespace Numerology {
class NumerologyDayNumber : public QObject
{
    Q_OBJECT
public:
    NumerologyDayNumber(const QDate& birthday, QObject * parent = nullptr);
    QString getBirthdayString();
    QString getYearString(const QDate& date);
    QString getMonthString(const QDate& date);
    QString getDayString(const QDate& date);
private:
    QDate _birthday;
    QString _strFromDate;
    int _birthdayFirstNumber;
};
}

#endif // NUMEROLOGYDAYNUMBER_H
