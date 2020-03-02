#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Numerology/Numerology.h"
#include "Numerology/CulminationModel.h"
#include "Numerology/DayNumberModel.h"

#include "ui_MainWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget * parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow * ui;
    Numerology::Descriptions * _descriptions;
    Numerology::CulminationModel * _culminationModel;
    Numerology::DayNumberModel * _dayNumberModel;
};
#endif // MAINWINDOW_H
