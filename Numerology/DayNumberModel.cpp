#include <QDate>

#include "Numerology.h"
#include "DayNumberModel.h"

using namespace Numerology;

DayNumberModel::DayNumberModel(QObject *parent)
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

        switch (index.column()) {
            case 0: { // Date
                QDate newDate = _firstDate.addDays(index.row());
                return newDate;
            }
            case 1: { // DayNumber
                QDate newDate = _firstDate.addDays(index.row());
                return _birthdayNumber->getDayString(newDate);
            }
        }
    }
    return QVariant();
}

int DayNumberModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return _firstDate.daysTo(_secondDate);
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
    _birthDate = birthDate;
    _birthdayNumber = new NumerologyDayNumber(_birthDate, this);
}

void DayNumberModel::setDateRange(const QDate &firstDate, const QDate &secondDate)
{
    _firstDate = firstDate;
    _secondDate = secondDate;
}
