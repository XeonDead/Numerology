#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Numerology/Numerology.h"
#include "Numerology/CulminationModel.h"
#include "Numerology/DayNumberModel.h"

#include "ui_AndroidMainWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class AndroidMainWindow : public QMainWindow {
    Q_OBJECT

public:
    AndroidMainWindow(QWidget * parent = nullptr);
    ~AndroidMainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow * ui;
    Numerology::Descriptions * _descriptions;
    Numerology::CulminationModel * _culminationModel;
    Numerology::DayNumberModel * _dayNumberModel;
};
#endif // MAINWINDOW_H
