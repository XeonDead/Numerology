#include <QApplication>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>

#include "Numerology/Numerology.h"
#include "Numerology/BackendFunctions.h"
#include "Numerology/CulminationModel.h"
#include "Numerology/DayNumberModel.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    qmlRegisterType<Numerology::CulminationModel>("CulminationModel", 0, 1, "CulminationModel");
    qmlRegisterType<Numerology::DayNumberModel>("DayNumberModel", 0, 1, "DayNumberModel");

    QQuickView viewer;

    // The following are needed to make examples run without having to install the
    // module in desktop environments.
  #ifdef Q_OS_WIN
    QString extraImportPath(QStringLiteral("%1/../../../../%2"));
  #else
    QString extraImportPath(QStringLiteral("%1/../../../%2"));
  #endif
    viewer.engine()->addImportPath(extraImportPath.arg(
        QGuiApplication::applicationDirPath(), QString::fromLatin1("qml")));
    QObject::connect(viewer.engine(), &QQmlEngine::quit, &viewer,
                     &QWindow::close);

    viewer.setTitle(QObject::tr("Нумерологический помощник"));

    BackendFunctionEnergy energyFunc(&viewer);

    viewer.rootContext()->setContextProperty("BackendFunctionEnergy", &energyFunc);

    viewer.setSource(QUrl("qrc:/main.qml"));
    viewer.setResizeMode(QQuickView::SizeRootObjectToView);
    viewer.setColor(QColor("#404040"));
    viewer.show();

    return app.exec();
}
