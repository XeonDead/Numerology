#include <QDebug>
#include <QFile>

#include "Numerology.h"

using namespace Numerology;

Descriptions::Descriptions(QObject *parent)
    : QObject(parent)
{
    QFile file {"resources/descriptions.json"};
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString descriptions = file.readAll();
    QJsonParseError err;
    this->descriptionDoc = QJsonDocument::fromJson(descriptions.toUtf8(), &err);
    qDebug() << err.error;
}

QString Descriptions::describe(int number)
{
    auto valObj = this->descriptionDoc.object().value(QString("%1").arg(number)).toObject();
    auto descriptionStr = valObj.value("description").toString();
    return descriptionStr;
}
