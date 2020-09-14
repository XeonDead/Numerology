#ifndef CULMINATIONMODEL_H
#define CULMINATIONMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <qqml.h>

namespace Numerology {
using NumerologyPair = QPair<int, int>;

class CulminationModel : public QAbstractTableModel
{
    QML_ELEMENT
public:
    explicit CulminationModel(QObject *parent = nullptr);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent    = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    void clear();
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    void setDate(const QDate &date);

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
