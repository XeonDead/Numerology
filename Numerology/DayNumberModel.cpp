#include "DayNumberModel.h"

#include <QDate>

#include "Numerology.h"

using namespace Numerology;

DayNumberModel::DayNumberModel(QObject *parent) : QAbstractTableModel(parent) {}

QVariant DayNumberModel::headerData(int section, Qt::Orientation orientation,
                                    int role) const {
  if (orientation == Qt::Orientation::Horizontal) {
    if (role == Qt::DisplayRole) {
      switch (section) {
        case 0: {
          return QObject::tr("Date");
        }
        case 1: {
          return QObject::tr("DayNumber");
        }
      }
    } else {
      return QVariant();
    }
  }
  return QVariant();
}

QVariant DayNumberModel::data(const QModelIndex &index, int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  } else {
    switch (index.column()) {
      case 0: {
        return _numbers.at(index.row()).first;
      }
      case 1: {
        return _numbers.at(index.row()).second;
      }
    }
  }
  return QVariant();
}  // DayNumberModel::data

int DayNumberModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent)
  return _numbers.length();
}

int DayNumberModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent)
  return 2;
}

void DayNumberModel::clear() { _numbers.clear(); }

void DayNumberModel::setBirthDate(const QDate &birthDate) {
  _birthDate = birthDate;
  _numbers.push_back(
      qMakePair(birthDate.toString("dd.MM.yyyy"),
                getDateNumber(DateNumberType::BirthDay, birthDate)));
}

void DayNumberModel::setDateRange(const QDate &firstDate,
                                  const QDate &secondDate) {
  int years = (secondDate.year() - firstDate.year()) + 1;
  int months = (secondDate.month() - firstDate.month()) + 1;
  int days = firstDate.daysTo(secondDate);

  for (int i = 0; i < years; ++i) {
    QDate date = firstDate.addYears(i);
    _numbers.push_back(qMakePair(date.toString("yyyy"),
                                 getDateNumber(DateNumberType::Year, date)));
  }
  for (int i = 0; i < months; ++i) {
    QDate date = firstDate.addMonths(i);
    _numbers.push_back(qMakePair(date.toString("MM.yyyy"),
                                 getDateNumber(DateNumberType::Month, date)));
  }
  for (int i = 0; i <= days; ++i) {
    QDate date = firstDate.addDays(i);
    _numbers.push_back(qMakePair(date.toString("dd.MM.yyyy"),
                                 getDateNumber(DateNumberType::Day, date)));
  }
}

const QString DayNumberModel::getDateNumber(const DateNumberType &type,
                                            const QDate &date) {
  int firstNumber = 0;
  int secondNumber = 0;
  int thirdNumber = 0;
  int fourthNumber = 0;

  if (type != DateNumberType::BirthDay) {
    QString birthdayStringThisYear =
        date.toString("yyyy") + _birthDate.toString("MMdd");
    for (const auto i : birthdayStringThisYear) {
      if (i.isDigit()) firstNumber += i.digitValue();
    }
  }

  QString dateString;
  switch (type) {
    case BirthDay: {
      dateString = date.toString("yyyyMMdd");
      break;
    }
    case Day: {
      dateString = date.toString("MMdd");
      break;
    }
    case Month: {
      dateString = date.toString("MM");
      break;
    }
    case Year: {
      break;
    }
  }

  for (const auto i : dateString) {
    if (i.isDigit()) firstNumber += i.digitValue();
  }

  for (const auto i : QString("%1").arg(firstNumber)) {
    if (i.isDigit()) secondNumber += i.digitValue();
  }

  thirdNumber = firstNumber - (date.toString("d").front().digitValue() * 2);

  for (const auto i : QString("%1").arg(thirdNumber)) {
    if (i.isDigit()) fourthNumber += i.digitValue();
  }

  return QString("%1.%2.%3.%4")
      .arg(firstNumber)
      .arg(secondNumber)
      .arg(thirdNumber)
      .arg(fourthNumber);
}