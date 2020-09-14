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

void DayNumberModel::clear() {
    beginRemoveRows(QModelIndex(), 0, _numbers.size() - 1);
    _numbers.clear();
    endRemoveRows();
}

bool DayNumberModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(role)
    Q_UNUSED(index)
    if (!value.toString().contains(";")) {
        QDate date = QDate::fromString(value.toString(), QString("dd.MM.yyyy"));
        if (date.isValid()) {
          this->setBirthDate(date);
        } else {
          return false;
        }
        return true;
    } else {
        auto dates = value.toString().split(QString(";"), Qt::SkipEmptyParts);
        QDate firstDate = QDate::fromString(dates.first(), QString("dd.MM.yyyy"));
        QDate lastDate = QDate::fromString(dates.last(), QString("dd.MM.yyyy"));
        if (firstDate.isValid() && lastDate.isValid()) {
          this->setDateRange(firstDate, lastDate);
        } else {
          return false;
        }
        return true;
    }
    return false;
}

void DayNumberModel::setBirthDate(const QDate &birthDate) {
  _birthDate = birthDate;
  _numbers.push_back(
      qMakePair(birthDate.toString("dd.MM.yyyy"),
                getDateNumber(DateNumberType::BirthDay, birthDate)));
}

void DayNumberModel::setDateRange(const QDate &firstDate,
                                  const QDate &secondDate) {
  this->clear();
  int years = (secondDate.year() - firstDate.year()) + 1;
  int months = (secondDate.month() - firstDate.month()) + 1;
  int days = firstDate.daysTo(secondDate);
  beginInsertRows(QModelIndex(), 0, years + months + days - 1);

  _numbers.push_back(qMakePair(_birthDate.toString("dd.MM.yyyy"),
                               getDateNumber(DateNumberType::BirthDay, _birthDate)));
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
  endInsertRows();
  emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
}

const QString DayNumberModel::getDateNumber(const DateNumberType &type,
                                            const QDate &date) {
  int firstNumber = 0;
  int secondNumber = 0;
  int thirdNumber = 0;
  int fourthNumber = 0;

  auto qCharToInt = [](int x, QChar c) {
      auto val = c.digitValue();
      if (val != -1) {
          return x+val;
      } else {
          return x;
      }
  };

  if (type != DateNumberType::BirthDay) {
    QString birthdayStringThisYear =
        date.toString("yyyy") + _birthDate.toString("MMdd");
    firstNumber = std::accumulate(birthdayStringThisYear.begin(), birthdayStringThisYear.end(), 0, qCharToInt);
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

  firstNumber += std::accumulate(dateString.begin(), dateString.end(), 0, qCharToInt);
  QString firstNumStr = QString("%1").arg(firstNumber);

  secondNumber += std::accumulate(firstNumStr.begin(), firstNumStr.end(), 0, qCharToInt);
  if (secondNumber > 25 && secondNumber % 10 > 0) {
      int temp = secondNumber;
      secondNumber  = secondNumber / 10;
      secondNumber += temp % 10;
  }

  thirdNumber += std::abs(firstNumber - (_birthDate.toString("d").front().digitValue() * 2));
  QString thirdNumStr = QString("%1").arg(thirdNumber);

  fourthNumber += std::accumulate(thirdNumStr.begin(), thirdNumStr.end(), 0, qCharToInt);

  return QString("%1.%2.%3.%4")
      .arg(firstNumber)
      .arg(secondNumber)
      .arg(thirdNumber)
      .arg(fourthNumber);
}
