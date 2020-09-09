#include <QDate>

#include "Numerology.h"
#include "CulminationModel.h"

using namespace Numerology;

CulminationModel::CulminationModel(QObject *parent)
    : QAbstractTableModel(parent),
    rowCountStore(4)
{ }

QVariant CulminationModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Orientation::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch (section) {
                case 0: {
                    return QObject::tr("Shadows");
                }
                case 1: {
                    return QObject::tr("Peaks");
                }
                case 2: {
                    return QObject::tr("Challenges");
                }
                case 3: {
                    return QObject::tr("Exits");
                }
            }
        } else { return QString(""); }
    } else if (orientation == Qt::Orientation::Vertical) {
        if (role == Qt::DisplayRole) {
            return Numerology::to_roman(section + 1);
        } else { return QString(""); }
    }
    return QString("");
}

QVariant CulminationModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole) {
        return QString("");
    } else {
        switch (index.column()) {
            case 0: {
                if (shadows.empty()) return QString("");
                if (shadows.at(index.row()).first == 0) {
                    return QString("%1").arg(shadows.at(index.row()).second);
                } else {
                    return QString("(%1) %2").arg(shadows.at(index.row()).first).arg(shadows.at(index.row()).second);
                }
            }
            case 1: {
                if (peaks.empty()) return QString("");
                if (peaks.at(index.row()).first == 0) {
                    return QString("%1").arg(peaks.at(index.row()).second);
                } else {
                    return QString("(%1) %2").arg(peaks.at(index.row()).first).arg(peaks.at(index.row()).second);
                }
            }
            case 2: {
                if (challenges.empty()) return QString("");
                if (challenges.at(index.row()).first == 0) {
                    return QString("%1").arg(challenges.at(index.row()).second);
                } else {
                    return QString("(%1) %2").arg(challenges.at(index.row()).first).arg(challenges.at(index.row()).second);
                }
            }
            case 3: {
                if (exits.empty()) return QString("");
                if (exits.at(index.row()).first == 0) {
                    return QString("%1").arg(exits.at(index.row()).second);
                } else {
                    return QString("(%1) %2").arg(exits.at(index.row()).first).arg(exits.at(index.row()).second);
                }
            }
            case 4: {
                return Numerology::LifeCategories[index.row()];
            }
            case 5: {
                if (strList.empty())
                    return QString("");
                else
                    return strList.at(index.row());
            }
        }
    }
    return QVariant();
} // CulminationModel::data

int CulminationModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return rowCountStore;
}

int CulminationModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 6;
}

void CulminationModel::setRowCount(int rowCountNew)
{
    rowCountStore = rowCountNew;
}

void CulminationModel::clear()
{
    if (rowCountStore == 7) {
        beginRemoveRows(QModelIndex(), 4, 6);
        rowCountStore = 4;
        endRemoveRows();
    }
    peaks.clear();
    challenges.clear();
    shadows.clear();
    exits.clear();
    strList.clear();
}

bool CulminationModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(index)
    Q_UNUSED(role)
    QDate date = QDate::fromString(value.toString(), QString("dd.MM.yyyy"));
    this->setDate(date);
    return true;
}

void CulminationModel::setDate(const QDate& date)
{
    this->clear();
    auto dayNumber   = Numerology::sumNumbers(date.day());
    auto monthNumber = Numerology::sumNumbers(date.month());
    auto yearNumber  = Numerology::sumNumbers(date.year());
    auto lifeNumber  =
      Numerology::sumNumbers(dayNumber.second + monthNumber.second + yearNumber.second);

    for (int i = 0; i < 7; ++i) {
        if (strList.empty()) {
            strList.push_back(
                QString("%1-%2")
                .arg(Numerology::NumerologyNumber - lifeNumber.second)
                .arg(Numerology::NumerologyNumber - lifeNumber.second
                + Numerology::NumerologyNumber / 3));
        } else {
            auto tempStr = strList.last();
            int start    = tempStr.remove(0, 3).toInt();
            strList.push_back(QString("%1-%2").arg(start + 1).arg(
                  start + Numerology::NumerologyNumber / 3));
        }
    }

    if ((QDate::currentDate().year() - date.year()) > strList.at(4).right(2).toInt()) {
        rowCountStore = 7;
        beginInsertRows(QModelIndex(), 4, 6);
        endInsertRows();
    }

    peaks.push_back(Numerology::sumNumbers(dayNumber.second + monthNumber.second));
    peaks.push_back(Numerology::sumNumbers(dayNumber.second + yearNumber.second));
    peaks.push_back(Numerology::sumNumbers(peaks.at(0).second + peaks.at(1).second));
    peaks.push_back(Numerology::sumNumbers(monthNumber.second + yearNumber.second));
    peaks.push_back(Numerology::sumNumbers(peaks.at(2).second + peaks.at(3).second));
    peaks.push_back(Numerology::sumNumbers(peaks.at(3).second + peaks.at(4).second));
    peaks.push_back(Numerology::sumNumbers(peaks.at(4).second + peaks.at(5).second));
    peaks.push_back(Numerology::sumNumbers(peaks.at(5).second + peaks.at(6).second));

    challenges.push_back(Numerology::sumNumbers(std::abs(dayNumber.second - monthNumber.second)));
    challenges.push_back(Numerology::sumNumbers(std::abs(dayNumber.second - yearNumber.second)));
    challenges.push_back(Numerology::sumNumbers(std::abs(challenges.at(0).second - challenges.at(1).second)));
    challenges.push_back(Numerology::sumNumbers(std::abs(monthNumber.second - yearNumber.second)));
    challenges.push_back(Numerology::sumNumbers(std::abs(challenges.at(2).second - challenges.at(3).second)));
    challenges.push_back(Numerology::sumNumbers(std::abs(challenges.at(3).second - challenges.at(4).second)));
    challenges.push_back(Numerology::sumNumbers(std::abs(challenges.at(4).second - challenges.at(5).second)));
    challenges.push_back(Numerology::sumNumbers(std::abs(challenges.at(5).second - challenges.at(6).second)));

    shadows.push_back(Numerology::sumNumbers(peaks.at(0).second + date.day()));
    shadows.push_back(Numerology::sumNumbers(peaks.at(1).second + date.month()));
    shadows.push_back(Numerology::sumNumbers(peaks.at(2).second + date.year()));
    shadows.push_back(Numerology::sumNumbers(peaks.at(3).second + lifeNumber.second));
    shadows.push_back(Numerology::sumNumbers(peaks.at(4).second + date.day()));
    shadows.push_back(Numerology::sumNumbers(peaks.at(5).second + date.month()));
    shadows.push_back(Numerology::sumNumbers(peaks.at(6).second + date.year()));
    shadows.push_back(Numerology::sumNumbers(peaks.at(7).second + lifeNumber.second));

    for (int i = 0; i < 8; ++i) {
        exits.push_back((Numerology::sumNumbers(peaks.at(i).second + challenges.at(i).second + shadows.at(i).second)));
    }

    emit dataChanged(index(0, 0), index(rowCountStore - 1, 5));
} // CulminationModel::setDate
