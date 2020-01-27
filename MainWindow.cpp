#include <QStandardItemModel>
#include <QtCharts/QLineSeries>

#include "MainWindow.h"

MainWindow::MainWindow(QWidget * parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
    _descriptions(new Numerology::Descriptions(this)),
    _culminationModel(new Numerology::CulminationModel(this))
{
    ui->setupUi(this);
    QMetaObject::connectSlotsByName(this);
}

MainWindow::~MainWindow(){ delete ui; }

void MainWindow::on_pushButton_clicked()
{
    auto ui_dateEdit = findChild<QDateEdit *>("dateEdit");
    auto date        = ui_dateEdit->date();

    _culminationModel->clear();
    _culminationModel->setDate(date);

    auto dayNumber   = Numerology::sumNumbers(date.day());
    auto monthNumber = Numerology::sumNumbers(date.month());
    auto yearNumber  = Numerology::sumNumbers(date.year());
    auto lifeNumber  =
      Numerology::sumNumbers(dayNumber.second + monthNumber.second + yearNumber.second);
    QString energy = QString("%1").arg(date.toString("ddMM").toInt()
        * date.toString("yyyy").toInt()).leftJustified(7, '0');

    QDate curYear = QDateTime::currentDateTime().date();
    QDate year    = date;

    while (year < curYear.addYears(-7)) {
        year = year.addYears(7);
    }

    auto ui_chart = findChild<QtCharts::QChartView *>("chartView");
    QtCharts::QLineSeries * series = new QtCharts::QLineSeries(this);

    for (const auto &i : energy.split("", QString::SkipEmptyParts)) {
        series->append(year.year(), i.toDouble());
        year = year.addYears(1);
    }

    QStringList strList;
    for (int i = 0; i < 7; ++i) {
        if (strList.empty()) {
            strList.push_back(
                QString("%1-%2")
                .arg(Numerology::NumerologyNumber - lifeNumber.second)
                .arg(Numerology::NumerologyNumber - lifeNumber.second
                + Numerology::NumerologyNumber / 3));
        } else {
            auto tempStr = strList.last();
            int start    = tempStr.remove(0, 3).toInt();
            strList.push_back(QString("%1-%2").arg(start + 1).arg(
                  start + Numerology::NumerologyNumber / 3));
        }
    }

    int periods        = 4; // mostly const
    int lastPeriodYear = QString(strList.at(3)).remove(0, 3).toInt();
    int currentlyYears = curYear.year() - date.year();

    if (currentlyYears > lastPeriodYear) {
        periods = 7;
    }

    _culminationModel->setRowCount(periods);

    QtCharts::QChart * chart = ui_chart->chart();
    chart->removeAllSeries();
    chart->addSeries(series);
    chart->legend()->hide();
    chart->createDefaultAxes();
    ui_chart->setRenderHint(QPainter::Antialiasing);

    QValueAxis * axisX =
      qobject_cast<QtCharts::QValueAxis *>(chart->axes(Qt::Horizontal).first());
    axisX->setRange(series->at(0).x(), series->at(6).x());
    axisX->setTickCount(7);
    axisX->setTickType(QValueAxis::TickType::TicksFixed);
    axisX->setLabelFormat("%d");

    QValueAxis * axisY =
      qobject_cast<QtCharts::QValueAxis *>(chart->axes(Qt::Vertical).first());
    axisY->setRange(0, 9);
    axisY->setTickType(QValueAxis::TickType::TicksDynamic);
    axisY->setTickInterval(1);
    axisY->setLabelFormat("%d");

    auto ui_tableView = findChild<QTableView *>("tableView");

    ui_tableView->setModel(_culminationModel);

    ui_tableView->setColumnWidth(0, 70);
    ui_tableView->setColumnWidth(1, 30);
    ui_tableView->setColumnWidth(2, 90);
    ui_tableView->setColumnWidth(3, 30);
    ui_tableView->setColumnWidth(4, 180);
    ui_tableView->setColumnWidth(5, 30);
    ui_tableView->setEditTriggers(QTableView::EditTrigger::NoEditTriggers);
} // MainWindow::on_pushButton_clicked

// void MainWindow::on_tableWidget_cellClicked(int row, int column) {
//  auto descriptionTextBrowser =
//      findChild<QTextBrowser *>("descriptionTextBrowser");
//  auto table = findChild<QTableView *>("tableView");
//  auto tableText = table->currentIndex().data(Qt::DisplayRole).toString();
//  bool ok = false;
//  int tableNumber = tableText.toInt(&ok);
//  if (!ok) {
//      tableText.remove(0, 1);
//      int numParenthesis = tableText.indexOf(')');
//      tableText.remove(numParenthesis, tableText.count() - numParenthesis);
//      tableNumber = tableText.toInt(&ok);
//  }
//  descriptionTextBrowser->setText(_descriptions->describe(tableNumber));
// }
