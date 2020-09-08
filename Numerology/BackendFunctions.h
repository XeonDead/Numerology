#ifndef BACKENDFUNC_H
#define BACKENDFUNC_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QXYSeries>
#include <QTimer>
#include <QtCharts/QAbstractSeries>
#include <QtCore/QObject>
class QTimer;

QT_BEGIN_NAMESPACE
class QQuickView;
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class BackendFunctionEnergy : public QObject
{
    Q_OBJECT
public:
    explicit BackendFunctionEnergy(QQuickView *appViewer = nullptr, QObject *parent = nullptr);

    Q_INVOKABLE void setSeries(QAbstractSeries *series);
    Q_INVOKABLE void setEnergyData(const QString& input);
    Q_INVOKABLE int getFirstYear();
    Q_INVOKABLE int getLastYear();
public slots:
    void update();
private:
    QQuickView *_appViewer;
    QXYSeries *_series;
    QTimer *_timer;
    QVector<QPointF> _data;
};

#endif
