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

  auto dayNumber = Numerology::sumNumbers(date.day());
  auto monthNumber = Numerology::sumNumbers(date.month());
  auto yearNumber = Numerology::sumNumbers(date.year());
  auto lifeNumber =
      Numerology::sumNumbers(std::get<0>(dayNumber) + std::get<0>(monthNumber) + std::get<0>(yearNumber));
  QString energy = QString("%1").arg(date.toString("ddMM").toInt() *
                                         date.toString("yyyy").toInt()).leftJustified(7, '0');

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

  QStringList strList;
  for (int i = 0; i < 7; ++i) {
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

  int periods = 4; // mostly const
  int lastPeriodYear = QString(strList.at(3)).remove(0, 3).toInt();
  int currentlyYears = curYear.year() - date.year();

  if (currentlyYears > lastPeriodYear) {
      periods = 7;
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

  QVector<int> peaks;

  peaks.push_back(std::get<0>(dayNumber) + std::get<0>(monthNumber));
  peaks.push_back(std::get<0>(dayNumber) + std::get<0>(yearNumber));
  peaks.push_back(peaks.at(0) + peaks.at(1));
  peaks.push_back(std::get<0>(monthNumber) + std::get<0>(yearNumber));
  if (periods > 4) {
      peaks.push_back(peaks.at(2) + peaks.at(3));
      peaks.push_back(peaks.at(3) + peaks.at(4));
      peaks.push_back(peaks.at(4) + peaks.at(5));
      peaks.push_back(peaks.at(5) + peaks.at(6));
  }

  QVector<int> challenges;

  challenges.push_back(std::abs(std::get<0>(dayNumber) - std::get<0>(monthNumber)));
  challenges.push_back(std::abs(std::get<0>(dayNumber) - std::get<0>(yearNumber)));
  challenges.push_back(std::abs(challenges.at(0) - challenges.at(1)));
  challenges.push_back(std::abs(std::get<0>(monthNumber) - std::get<0>(yearNumber)));
  if (periods > 4) {
      challenges.push_back(std::abs(challenges.at(2) - challenges.at(3)));
      challenges.push_back(std::abs(challenges.at(3) - challenges.at(4)));
      challenges.push_back(std::abs(challenges.at(4) - challenges.at(5)));
      challenges.push_back(std::abs(challenges.at(5) - challenges.at(6)));
  }

  QVector<int> shadows;

  shadows.push_back(std::get<0>(Numerology::sumNumbers(peaks.at(0) + date.day())));
  shadows.push_back(std::get<0>(Numerology::sumNumbers(peaks.at(1) + date.month())));
  shadows.push_back(std::get<0>(Numerology::sumNumbers(peaks.at(2) + date.year())));
  shadows.push_back(std::get<0>(Numerology::sumNumbers(peaks.at(3) + std::get<0>(lifeNumber))));
  if (periods > 4) {
      shadows.push_back(std::get<0>(Numerology::sumNumbers(peaks.at(4) + date.day())));
      shadows.push_back(std::get<0>(Numerology::sumNumbers(peaks.at(5) + date.month())));
      shadows.push_back(std::get<0>(Numerology::sumNumbers(peaks.at(6) + date.year())));
      shadows.push_back(std::get<0>(Numerology::sumNumbers(peaks.at(7) + std::get<0>(lifeNumber))));
  }

  QVector<int> exits;

  for (int i = 0; i < periods; ++i) {
      exits.push_back(std::get<0>(Numerology::sumNumbers(peaks.at(i)+challenges.at(i)+shadows.at(i))));
  }

  auto ui_tableWidget = findChild<QTableWidget *>("tableWidget");
  ui_tableWidget->clear();
  ui_tableWidget->setRowCount(periods);
  ui_tableWidget->setColumnCount(6);

  for (int i = 0; i < periods; ++i) {

    ui_tableWidget->setItem(i, 0, new QTableWidgetItem(QString("%1").arg(shadows.at(i))));

    auto firstElemValue = Numerology::sumNumbers(peaks.at(i));
    if (std::get<2>(firstElemValue)) {
      ui_tableWidget->setItem(
          i, 1,
          new QTableWidgetItem(QString("(%1) %2")
                                   .arg(std::get<1>(firstElemValue))
                                   .arg(std::get<0>(firstElemValue)),
                               0));
    } else {
      ui_tableWidget->setItem(
          i, 1,
          new QTableWidgetItem(QString("%1").arg(std::get<0>(firstElemValue)),
                               0));
    }

    auto secondElemValue = Numerology::sumNumbers(challenges.at(i));
    if (std::get<2>(secondElemValue)) {
      ui_tableWidget->setItem(
          i, 2,
          new QTableWidgetItem(QString("(%1) %2")
                                   .arg(std::get<1>(secondElemValue))
                                   .arg(std::get<0>(secondElemValue)),
                               0));
    } else {
      ui_tableWidget->setItem(
          i, 2,
          new QTableWidgetItem(QString("%1").arg(std::get<0>(secondElemValue)),
                               0));
    }

    ui_tableWidget->setItem(i, 3, new QTableWidgetItem(QString("%1").arg(exits.at(i))));

    ui_tableWidget->setItem(
        i, 4, new QTableWidgetItem(Numerology::LifeCategories[i], 0));

    ui_tableWidget->setItem(i, 5, new QTableWidgetItem(strList.at(i)));
  }
  ui_tableWidget->setColumnWidth(0, 30);
  ui_tableWidget->setColumnWidth(1, 30);
  ui_tableWidget->setColumnWidth(2, 30);
  ui_tableWidget->setColumnWidth(3, 30);
  ui_tableWidget->setColumnWidth(4, 180);
  ui_tableWidget->setColumnWidth(5, 30);
  ui_tableWidget->setVerticalHeaderLabels(
      QStringList{"I", "II", "III", "IV", "V", "VI", "VII"});
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
