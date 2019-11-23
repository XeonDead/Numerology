#include <QStandardItemModel>
#include <QtCharts/QLineSeries>

#include "Numerology/Numerology.h"

#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      _descriptions(new Numerology::Descriptions(this)) {
  ui->setupUi(this);
  QMetaObject::connectSlotsByName(this);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_pushButton_clicked() {
  auto ui_dateEdit = findChild<QDateEdit *>("dateEdit");
  auto date = ui_dateEdit->date();

  auto dayNumber = date.day();
  auto monthNumber = date.month();
  auto yearNumber = Numerology::sumNumbers(date.year());
  auto lifeNumber =
      Numerology::sumNumbers(dayNumber + monthNumber + std::get<0>(yearNumber));
  QString energy = QString("%1").arg(date.toString("ddMM").toInt() *
                                         date.toString("yyyy").toInt(),
                                     7, 10, QChar('0'));

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

  QtCharts::QChart *chart = ui_chart->chart();
  chart->removeAllSeries();
  chart->addSeries(series);
  chart->legend()->hide();
  chart->createDefaultAxes();
  ui_chart->setRenderHint(QPainter::Antialiasing);

  QValueAxis *axisX =
      qobject_cast<QtCharts::QValueAxis *>(chart->axes(Qt::Horizontal).first());
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

  QMap<int, QPair<int, int>> peaksChallenges;

  peaksChallenges.insert(1, {std::abs(dayNumber + monthNumber),
                             std::abs(dayNumber - monthNumber)});
  peaksChallenges.insert(2, {std::abs(dayNumber + std::get<0>(yearNumber)),
                             std::abs(dayNumber - std::get<0>(yearNumber))});
  peaksChallenges.insert(3, {std::abs(peaksChallenges.value(1).first +
                                      peaksChallenges.value(2).first),
                             std::abs(peaksChallenges.value(1).second -
                                      peaksChallenges.value(2).second)});
  peaksChallenges.insert(4, {std::abs(monthNumber + std::get<0>(yearNumber)),
                             std::abs(monthNumber - std::get<0>(yearNumber))});
  peaksChallenges.insert(5, {std::abs(peaksChallenges.value(3).first +
                                      peaksChallenges.value(4).first),
                             std::abs(peaksChallenges.value(3).second -
                                      peaksChallenges.value(4).second)});

  QStringList strList;
  for (int i = 0; i < 5; ++i) {
    if (strList.empty()) {
      strList.push_back(
          QString("%1-%2")
              .arg(Numerology::NumerologyNumber - std::get<0>(lifeNumber))
              .arg(Numerology::NumerologyNumber - std::get<0>(lifeNumber) +
                   Numerology::NumerologyNumber / 3));
    } else {
      auto tempStr = strList.last();
      int start = tempStr.remove(0, 3).toInt();
      strList.push_back(QString("%1-%2").arg(start + 1).arg(
          start + Numerology::NumerologyNumber / 3));
    }
  }

  auto ui_tableWidget = findChild<QTableWidget *>("tableWidget");
  ui_tableWidget->clear();
  ui_tableWidget->setRowCount(5);
  ui_tableWidget->setColumnCount(4);
  QMapIterator<int, QPair<int, int>> i(peaksChallenges);
  while (i.hasNext()) {
    i.next();
    int row = i.key() - 1;

    auto firstElemValue = Numerology::sumNumbers(i.value().first);
    if (std::get<2>(firstElemValue)) {
      ui_tableWidget->setItem(
          row, 0,
          new QTableWidgetItem(QString("(%1) %2")
                                   .arg(std::get<1>(firstElemValue))
                                   .arg(std::get<0>(firstElemValue)),
                               0));
    } else {
      ui_tableWidget->setItem(
          row, 0,
          new QTableWidgetItem(QString("%1").arg(std::get<0>(firstElemValue)),
                               0));
    }

    auto secondElemValue = Numerology::sumNumbers(i.value().second);
    if (std::get<2>(secondElemValue)) {
      ui_tableWidget->setItem(
          row, 1,
          new QTableWidgetItem(QString("(%1) %2")
                                   .arg(std::get<1>(secondElemValue))
                                   .arg(std::get<0>(secondElemValue)),
                               0));
    } else {
      ui_tableWidget->setItem(
          row, 1,
          new QTableWidgetItem(QString("%1").arg(std::get<0>(secondElemValue)),
                               0));
    }

    ui_tableWidget->setItem(
        row, 2, new QTableWidgetItem(Numerology::LifeCategories[i.key()], 0));

    ui_tableWidget->setItem(row, 3, new QTableWidgetItem(strList.at(row)));
  }
  ui_tableWidget->setColumnWidth(0, 30);
  ui_tableWidget->setColumnWidth(1, 30);
  ui_tableWidget->setColumnWidth(2, 180);
  ui_tableWidget->setColumnWidth(3, 30);
  ui_tableWidget->setVerticalHeaderLabels(
      QStringList{"I", "II", "III", "IV", "V"});
  ui_tableWidget->setEditTriggers(QTableWidget::EditTrigger::NoEditTriggers);
}

void MainWindow::on_tableWidget_cellClicked(int row, int column) {
  auto descriptionTextBrowser =
      findChild<QTextBrowser *>("descriptionTextBrowser");
  auto table = findChild<QTableWidget *>("tableWidget");
  auto tableText = table->item(row, column)->data(Qt::DisplayRole).toString();
  bool ok = false;
  int tableNumber = tableText.toInt(&ok);
  if (!ok) {
      tableText.remove(0, 1);
      int numParenthesis = tableText.indexOf(')');
      tableText.remove(numParenthesis, tableText.count() - numParenthesis);
      tableNumber = tableText.toInt(&ok);
  }
  descriptionTextBrowser->setText(_descriptions->describe(tableNumber));
} // MainWindow::on_pushButton_clicked
