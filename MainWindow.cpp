#include "MainWindow.h"

#include <QStandardItemModel>
#include <QtCharts/QLineSeries>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      _descriptions(new Numerology::Descriptions(this)),
      _culminationModel(new Numerology::CulminationModel(this)),
      _dayNumberModel(new Numerology::DayNumberModel(this)) {
  ui->setupUi(this);
  QMetaObject::connectSlotsByName(this);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_pushButton_clicked() {
  auto ui_dateEdit = findChild<QDateEdit *>("dateEdit");
  auto date = ui_dateEdit->date();

  auto ui_stackWidget = findChild<QStackedWidget *>("stackedWidget");

  if (ui_stackWidget->currentIndex() == 0)  // Culmination Page
  {
    _culminationModel->clear();
    _culminationModel->setDate(date);

    QString energy =
        QString("%1")
            .arg(date.toString("ddMM").toInt() * date.toString("yyyy").toInt())
            .leftJustified(7, '0');

    QDate curYear = QDateTime::currentDateTime().date();
    QDate year = date;

    while (year < curYear.addYears(-7)) {
      year = year.addYears(7);
    }

    auto ui_chart = findChild<QtCharts::QChartView *>("chartView");
    QtCharts::QLineSeries *series = new QtCharts::QLineSeries(this);

    for (const auto &i : energy.split("", QString::SkipEmptyParts)) {
      series->append(year.year(), i.toDouble());
      year = year.addYears(1);
    }

    int periods = 4;  // mostly const
    int lastPeriodYear = _culminationModel->data(_culminationModel->index(3, 5))
                             .toString()
                             .remove(0, 3)
                             .toInt();
    int currentlyYears = curYear.year() - date.year();

    if (currentlyYears > lastPeriodYear) {
      periods = 7;
    }

    _culminationModel->setRowCount(periods);

    QtCharts::QChart *chart = ui_chart->chart();
    chart->removeAllSeries();
    chart->addSeries(series);
    chart->legend()->hide();
    chart->createDefaultAxes();
    ui_chart->setRenderHint(QPainter::Antialiasing);

    QValueAxis *axisX = qobject_cast<QtCharts::QValueAxis *>(
        chart->axes(Qt::Horizontal).first());
    axisX->setRange(series->at(0).x(), series->at(6).x());
    axisX->setTickCount(7);
    axisX->setTickType(QValueAxis::TickType::TicksFixed);
    axisX->setLabelFormat("%d");

    QValueAxis *axisY =
        qobject_cast<QtCharts::QValueAxis *>(chart->axes(Qt::Vertical).first());
    axisY->setRange(0, 9);
    axisY->setTickType(QValueAxis::TickType::TicksDynamic);
    axisY->setTickInterval(1);
    axisY->setLabelFormat("%d");

    auto ui_tableView = findChild<QTableView *>("culminationTableView");
    ui_tableView->reset();

    ui_tableView->setModel(_culminationModel);

    ui_tableView->setColumnWidth(0, 70);
    ui_tableView->setColumnWidth(1, 40);
    ui_tableView->setColumnWidth(2, 90);
    ui_tableView->setColumnWidth(3, 30);
    ui_tableView->setColumnWidth(4, 160);
    ui_tableView->setColumnWidth(5, 30);
    ui_tableView->setEditTriggers(QTableView::EditTrigger::NoEditTriggers);
  } else if (ui_stackWidget->currentIndex() == 1) {
    auto ui_tableView = findChild<QTableView *>("dayNumberTableView");
    ui_tableView->reset();

    _dayNumberModel->clear();
    _dayNumberModel->setBirthDate(date);

    auto firstDateEdit = findChild<QDateEdit *>("dayNumberFirstDate");
    auto secondDateEdit = findChild<QDateEdit *>("dayNumberSecondDate");

    _dayNumberModel->setDateRange(firstDateEdit->date(), secondDateEdit->date());

    ui_tableView->setModel(_dayNumberModel);
  }
}  // MainWindow::on_pushButton_clicked

void MainWindow::on_comboBox_currentIndexChanged(int index) {
  auto ui_stackWidget = findChild<QStackedWidget *>("stackedWidget");

  ui_stackWidget->setCurrentIndex(index);
}
