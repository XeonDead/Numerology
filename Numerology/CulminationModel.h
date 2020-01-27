#ifndef CULMINATIONMODEL_H
#define CULMINATIONMODEL_H

#include <QObject>
#include <QAbstractTableModel>

namespace Numerology {
using NumerologyPair = QPair<int, int>;

class CulminationModel : public QAbstractTableModel
{
public:
    CulminationModel(QObject * parent);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent    = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    void setRowCount(int rowCountNew);
    void clear();

    void setDate(const QDate& date);
private:
    int rowCountStore;
    QVector<NumerologyPair> peaks;
    QVector<NumerologyPair> challenges;
    QVector<NumerologyPair> shadows;
    QVector<NumerologyPair> exits;
    QStringList strList;
};
} // Numerology

#endif // CULMINATIONMODEL_H
