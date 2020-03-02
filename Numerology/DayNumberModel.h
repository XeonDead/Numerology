#ifndef DAYNUMBERMODEL_H
#define DAYNUMBERMODEL_H

#include <QtCore/QAbstractTableModel>
#include <QtCore/QDate>
#include "NumerologyDayNumber.h"

namespace Numerology {

class DayNumberModel : public QAbstractTableModel
{
public:
    DayNumberModel(QObject * parent);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent    = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    void clear();

    void setBirthDate(const QDate& birthDate);
    void setDateRange(const QDate& firstDate, const QDate& secondDate);
private:
    QDate _birthDate;
    QDate _firstDate;
    QDate _secondDate;
    NumerologyDayNumber * _birthdayNumber;
};
} // Numerology

#endif // DAYNUMBERMODEL_H
