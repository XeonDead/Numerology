#ifndef NUMEROLOGY_H
#define NUMEROLOGY_H

#include <QObject>
#include <QPair>
#include <QMap>

namespace Numerology {

static QPair<int, bool> checkMasterNumbers(int number);

static QPair<int, bool> sumNumbers(int from)
{
    bool encounteredEleven = false;
    int x   = from;
    int res = 0;

    while (x > 0) {
        res += x % 10;
        x    = x / 10;
    }
    encounteredEleven = Numerology::checkMasterNumbers(res).second;
    if (res % 10 > 0) {
        int temp = res;
        res  = res / 10;
        res += temp % 10;
    }
    if ((res % 11 == 0) && (res != 0)) {
        encounteredEleven = true;
    }
    return qMakePair(res, encounteredEleven);
}

static QPair<int, bool> checkMasterNumbers(int number)
{
    switch (number) {
        case 11: {
            return qMakePair(number, true);
        }
        case 22: {
            return qMakePair(number, true);
        }
        case 33: {
            return qMakePair(number, true);
        }
        case 13: {
            return qMakePair(number, true);
        }
        case 14: {
            return qMakePair(number, true);
        }
        case 16: {
            return qMakePair(number, true);
        }
        case 19: {
            return qMakePair(number, true);
        }
        default: {
            return qMakePair(number, false);
        }
    }
}

static QMap<int, QString> LifeCategories {
    { 1, QObject::tr("Поиск предназначения") },
    { 2, QObject::tr("Реализация") },
    { 3, QObject::tr("Перезагрузка") },
    { 4, QObject::tr("Новый виток жизни") },
    { 5, QObject::tr("") }
};

static const int NumerologyNumber = 27;
}

#endif // NUMEROLOGY_H
