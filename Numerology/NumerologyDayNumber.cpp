#include "NumerologyDayNumber.h"

#include <QtCore/QDate>

#include "Numerology.h"

using namespace Numerology;

NumerologyDayNumber::NumerologyDayNumber(const QDate &birthday, QObject *parent)
    : QObject(parent), _birthday(birthday), _strFromDate(birthday.toString("yyyyMMdd")), _birthdayFirstNumber(0) {
  for (const auto i : _strFromDate) {
    if (i.isDigit()) _birthdayFirstNumber += i.digitValue();
  }
}

QString NumerologyDayNumber::getBirthdayString() {
  int secondNumber = 0;
  int thirdNumber = 0;
  int fourthNumber = 0;
  for (const auto i : QString("%1").arg(_birthdayFirstNumber)) {
    if (i.isDigit()) secondNumber += i.digitValue();
  }
  thirdNumber = _birthdayFirstNumber - (_birthday.day()*2);
  for (const auto i : QString("%1").arg(thirdNumber)) {
    if (i.isDigit()) fourthNumber += i.digitValue();
  }
  return QString("%1.%2.%3.%4")
      .arg(_birthdayFirstNumber)
      .arg(secondNumber)
      .arg(thirdNumber)
      .arg(fourthNumber);
}
QString NumerologyDayNumber::getYearString(const QDate &date) {
    auto strFromNewDate = _birthday.addYears(date.year() - _birthday.year()).toString("yyyyMMdd");
    int firstNumber = 0;
    int secondNumber = 0;
    int thirdNumber = 0;
    int fourthNumber = 0;
    for (const auto i : strFromNewDate) {
        if (i.isDigit()) firstNumber += i.digitValue();
    }
    for (const auto i : QString("%1").arg(firstNumber)) {
        if (i.isDigit()) secondNumber += i.digitValue();
    }
    thirdNumber = firstNumber - (_birthday.day()*2);
    for (const auto i : QString("%1").arg(thirdNumber)) {
        if (i.isDigit()) fourthNumber += i.digitValue();
    }
    return QString("%1.%2.%3.%4")
        .arg(firstNumber)
        .arg(secondNumber)
        .arg(thirdNumber)
        .arg(fourthNumber);
}

QString NumerologyDayNumber::getMonthString(const QDate &date) {
    int firstNumber = this->getYearString(date).split(".").first().toInt();
    firstNumber += date.month();
    int secondNumber = 0;
    int thirdNumber = 0;
    int fourthNumber = 0;
    for (const auto i : QString("%1").arg(firstNumber)) {
        if (i.isDigit()) secondNumber += i.digitValue();
    }
    thirdNumber = firstNumber - (_birthday.day()*2);
    for (const auto i : QString("%1").arg(thirdNumber)) {
        if (i.isDigit()) fourthNumber += i.digitValue();
    }
    return QString("%1.%2.%3.%4")
        .arg(firstNumber)
        .arg(secondNumber)
        .arg(thirdNumber)
        .arg(fourthNumber);
}

QString NumerologyDayNumber::getDayString(const QDate &date) {
    int firstNumber = this->getMonthString(date).split(".").first().toInt();
    firstNumber += date.day();
    int secondNumber = 0;
    int thirdNumber = 0;
    int fourthNumber = 0;
    for (const auto i : QString("%1").arg(firstNumber)) {
        if (i.isDigit()) secondNumber += i.digitValue();
    }
    thirdNumber = firstNumber - (_birthday.day()*2);
    for (const auto i : QString("%1").arg(thirdNumber)) {
        if (i.isDigit()) fourthNumber += i.digitValue();
    }
    return QString("%1.%2.%3.%4")
        .arg(firstNumber)
        .arg(secondNumber)
        .arg(thirdNumber)
        .arg(fourthNumber);
}


