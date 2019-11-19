#ifndef NUMEROLOGY_H
#define NUMEROLOGY_H

#include <QObject>
#include <QPair>
#include <QMap>

namespace Numerology {

static QPair<int,bool> sumNumbers (int from) {
    bool encounteredEleven = false;
    int x = from;
    if (from % 11 == 0) encounteredEleven = true;
    int res = 0;
    while (x > 0) {
        res += x % 10;
        x = x / 10;
    }
    if (res % 11 == 0) encounteredEleven = true;
    if (res % 10 > 0) {
        int temp = res;
        res = res / 10;
        res += temp % 10;
    }
    return qMakePair(res, encounteredEleven);
}

static QMap<int, QString> LifeCategories {
    {1, QObject::tr("Поиск предназначения")},
    {2, QObject::tr("Реализация")},
    {3, QObject::tr("Перезагрузка")},
    {4, QObject::tr("Новый виток жизни")},
    {5, QObject::tr("")}
};

static const int NumerologyNumber = 27;
}

#endif // NUMEROLOGY_H
