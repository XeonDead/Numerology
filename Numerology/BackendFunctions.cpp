#include "BackendFunctions.h"
#include <QtCharts/QXYSeries>
#include <QtQuick/QQuickView>
#include <QtCharts/QValueAxis>
#include <QQmlProperty>

QT_CHARTS_USE_NAMESPACE

Q_DECLARE_METATYPE(QAbstractSeries *)
Q_DECLARE_METATYPE(QAbstractAxis *)

BackendFunctionEnergy::BackendFunctionEnergy(QQuickView *appViewer, QObject *parent)
    : QObject(parent),
    _appViewer(appViewer),
    _series(nullptr)
{}

void BackendFunctionEnergy::setSeries(QAbstractSeries *series) {
  if (series) {
    _series = static_cast<QXYSeries *>(series);
  }
}

void BackendFunctionEnergy::setEnergyData(const QString &input)
{
    QDateTime date = QDateTime::fromString(input, QString("dd.MM.yyyy"));
    if (!date.isValid()) return;
    QString energy =
      QString("%1")
      .arg(date.toString("ddMM").toInt() * date.toString("yyyy").toInt())
      .leftJustified(7, '0');

    QDate curYear = QDateTime::currentDateTime().date();
    QDate year    = date.date();

    while (year < curYear.addYears(-7)) {
        year = year.addYears(7);
    }

    QValueAxis * axisX = qobject_cast<QtCharts::QValueAxis *>(_series->attachedAxes().first());
    QQmlProperty(axisX, "min").write(year.startOfDay());

    _data.clear();
    _series->clear();
    for (const auto &i : energy.split(QString(), Qt::SkipEmptyParts)) {
        _series->append(year.year(), i.toDouble());
        _data.append(QPointF(year.year(), i.toDouble()));
        year = year.addYears(1);
    }

    QQmlProperty(axisX, "max").write(year.startOfDay());
}

int BackendFunctionEnergy::getFirstYear()
{
    return _data.first().toPoint().x();
}

int BackendFunctionEnergy::getLastYear()
{
    return _data.last().toPoint().x();
}



