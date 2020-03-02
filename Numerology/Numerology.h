#ifndef NUMEROLOGY_H
#define NUMEROLOGY_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QObject>
#include <QPair>

namespace Numerology {
static bool checkMasterNumbers(int number);

/*
 * Sum numbers in an integer, one by one
 *
 * @param integer number to be summed
 * @return pair(masterNumberMatch, result)
 */
static QPair<int, int> sumNumbers(int from)
{
    int x           = from;
    int res         = 0;
    int checkMaster = 0;

    if (Numerology::checkMasterNumbers(from)) {
        checkMaster = from;
    }

    while (x > 0) {
        res += x % 10;
        x    = x / 10;
    }

    if (checkMaster == 0) {
        if (Numerology::checkMasterNumbers(res)) {
            checkMaster = res;
        }
    }

    if (res % 10 > 0) {
        int temp = res;
        res  = res / 10;
        res += temp % 10;
    }

    if (checkMaster == 0) {
        if (Numerology::checkMasterNumbers(res)) {
            checkMaster = res;
        }
    }

    if (res % 10 == 0) {
        res = res / 10;
    }

    return qMakePair(checkMaster, res);
} // sumNumbers

static bool checkMasterNumbers(int number)
{
    switch (number) {
        case 11: {
            return true;
        }
        case 22: {
            return true;
        }
        case 33: {
            return true;
        }
        case 13: {
            return true;
        }
        case 14: {
            return true;
        }
        case 16: {
            return true;
        }
        case 19: {
            return true;
        }
        default: {
            return false;
        }
    }
}

static QString to_roman(unsigned int value)
{
    struct romandata_t { unsigned int value; char const * numeral; };
    const struct romandata_t romandata[] =
    {
        { 1000, "M" }, { 900, "CM" },
        {  500, "D" }, { 400, "CD" },
        {  100, "C" }, {  90, "XC" },
        {   50, "L" }, {  40, "XL" },
        {   10, "X" }, {   9, "IX" },
        {    5, "V" }, {   4, "IV" },
        {    1, "I" },
        {    0, NULL} // end marker
    };

    QString result;

    for (const romandata_t * current = romandata; current->value > 0; ++current) {
        while (value >= current->value) {
            result += current->numeral;
            value  -= current->value;
        }
    }
    return result;
}

static QMap<int, QString> LifeCategories{
    { 0, QObject::tr("Поиск предназначения") },
    { 1, QObject::tr("Реализация") },
    { 2, QObject::tr("Перезагрузка") },
    { 3, QObject::tr("Новый виток жизни") },
    { 4, QObject::tr("") },
    { 5, QObject::tr("") },
    { 6, QObject::tr("") },
    { 7, QObject::tr("") } };

static const int NumerologyNumber = 27;

class Descriptions : public QObject {
    Q_OBJECT
public:
    Descriptions(QObject * parent);
    ~Descriptions() = default;
    QString describe(int number);

protected:
    QJsonDocument descriptionDoc;
};
} // namespace Numerology

#endif // NUMEROLOGY_H
