#include <QApplication>

#ifdef Q_OS_ANDROID
#include "AndroidMainWindow.h"
#else
#include "MainWindow.h"
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifdef Q_OS_ANDROID
    AndroidMainWindow w;
    w.show();
#else
    MainWindow w;
    w.show();
#endif
    return a.exec();
}
