#include <QDate>

#include "Numerology.h"
#include "DayNumberModel.h"

using namespace Numerology;

DayNumberModel::DayNumberModel(QObject * parent)
    : QAbstractTableModel(parent)
{ }

QVariant DayNumberModel::headerData(int section, Qt::Orientation orientation, int role) const
{
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
        } else { return QVariant(); }
    }
    return QVariant();
}

QVariant DayNumberModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    } else {
        if (index.row() == 0) {
            switch (index.column()) {
                case 0: { // Date
                    return _firstDate.year();
                }
                case 1: { // DayNumber
                    QDate newDate = QDate(_firstDate.year(), 1, 1);
                    return _birthdayNumber->getYearString(newDate);
                }
            }
        } else if (index.row() == 1) {
            switch (index.column()) {
                case 0: { // Date
                    return _firstDate.toString("MM.yyyy");
                }
                case 1: { // DayNumber
                    return _birthdayNumber->getMonthString(_firstDate);
                }
            }
        } else if ((index.row() == 2) && (months == 2)) {
            switch (index.column()) {
                case 0: { // Date
                    return _secondDate.toString("MM.yyyy");
                }
                case 1: { // DayNumber
                    return _birthdayNumber->getMonthString(_secondDate);
                }
            }
        } else {
            switch (index.column()) {
                case 0: { // Date
                    QDate newDate = _firstDate.addDays(index.row() - years - months);
                    return newDate;
                }
                case 1: { // DayNumber
                    QDate newDate = _firstDate.addDays(index.row() - years - months);
                    return _birthdayNumber->getDayString(newDate);
                }
            }
        }
    }
    return QVariant();
} // DayNumberModel::data

int DayNumberModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return years + months + _firstDate.daysTo(_secondDate);
}

int DayNumberModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}

void DayNumberModel::clear()
{
    _birthDate.setDate(0, 0, 0);
    _firstDate.setDate(0, 0, 0);
    _secondDate.setDate(0, 0, 0);
}

void DayNumberModel::setBirthDate(const QDate &birthDate)
{
    _birthDate      = birthDate;
    _birthdayNumber = new NumerologyDayNumber(_birthDate, this);
}

void DayNumberModel::setDateRange(const QDate &firstDate, const QDate &secondDate)
{
    _firstDate  = firstDate;
    _secondDate = secondDate;
    if ((firstDate.daysTo(secondDate)) > 30) months = 2;
}
