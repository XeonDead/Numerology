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
 * @return tuple(result, masterNumberMatch (if any), masterNumberMatch (if
 * happened))
 */
static std::tuple<int, int, bool> sumNumbers(int from) {
  int x = from;
  int res = 0;

  while (x > 0) {
    res += x % 10;
    x = x / 10;
  }
  auto checkMaster =
      std::make_pair(from, Numerology::checkMasterNumbers(from));
  if (res % 10 > 0) {
    int temp = res;
    res = res / 10;
    res += temp % 10;
  }
  return std::make_tuple(res, checkMaster.first,
                         checkMaster.second);
}

static bool checkMasterNumbers(int number) {
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

static QMap<int, QString> LifeCategories{
    {1, QObject::tr("Поиск предназначения")},
    {2, QObject::tr("Реализация")},
    {3, QObject::tr("Перезагрузка")},
    {4, QObject::tr("Новый виток жизни")},
    {5, QObject::tr("")}};

static const int NumerologyNumber = 27;

class Descriptions : public QObject {
  Q_OBJECT
public:
  Descriptions(QObject *parent);
  ~Descriptions() = default;
  QString describe(int number);

protected:
  QJsonDocument descriptionDoc;
};

} // namespace Numerology

#endif // NUMEROLOGY_H
