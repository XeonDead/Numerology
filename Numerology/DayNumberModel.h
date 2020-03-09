#ifndef DAYNUMBERMODEL_H
#define DAYNUMBERMODEL_H

#include <QtCore/QAbstractTableModel>
#include <QtCore/QDate>

namespace Numerology {
class DayNumberModel : public QAbstractTableModel
{
    enum DateNumberType {
        BirthDay = 0,
        Day,
        Month,
        Year
    };

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
    const QString getDateNumber(const DateNumberType& type, const QDate& date);

    QList<QPair<QString, QString>> _numbers;
    QDate _birthDate;
};
} // Numerology

#endif // DAYNUMBERMODEL_H
