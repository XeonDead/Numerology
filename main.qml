import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Universal 2.12
import QtCharts 2.3
import CulminationModel 0.1
import DayNumberModel 0.1

Item {
    id: main
    width: 800
    height: 800
    visible: true

    TextField {
        id: textDate
        x: 8
        y: 8
        width: 175
        height: 33
        text: ""
        placeholderText: qsTr("Введите дату")
        font.bold: true
        font.family: "MS Shell Dlg 2"
        font.pointSize: 12
        validator: RegExpValidator {
            regExp: /^(0[1-9]|[12][0-9]|3[01])[- /.](0[1-9]|1[012])[- /.](19|20)\d\d$/
        }
    }

    Button {
        id: button
        x: 198
        y: 8
        width: 101
        height: 33
        text: qsTr("Рассчитать")
        onClicked: {
            BackendFunctionEnergy.setSeries(dateEnergyView.series(0))
            BackendFunctionEnergy.setEnergyData(textDate.text)
            axisX.min = BackendFunctionEnergy.getFirstYear()
            axisX.max = BackendFunctionEnergy.getLastYear()
            if (tabBar.currentIndex == 0) {
                var culModel = culminationTableView.model
                var q_model_index = culModel.index(0, 0)
                culModel.setData(q_model_index, textDate.text, 1)
            } else if (tabBar.currentIndex == 1) {
                var dayModel = dayNumberTableView.model
                var q_bday_index = dayModel.index(1, 0)
                var q_range_index = dayModel.index(2, 0)
                dayModel.setData(q_bday_index, textDate.text, 1)
                dayModel.setData(
                            q_range_index,
                            dayNumberFirst.text + ";" + dayNumberLast.text, 1)
            }
        }
    }

    TabBar {
        id: tabBar
        x: 0
        y: 47
        width: parent.width

        TabButton {
            text: qsTr("Кульминационные циклы")
        }
        TabButton {
            text: qsTr("Расчёт числа дня")
        }
    }

    StackLayout {
        id: stackLayout
        x: 0
        y: 87
        width: parent.width
        height: 714
        currentIndex: tabBar.currentIndex

        Item {
            id: culminationCycleTab
            ColumnLayout {
                id: columnLayout
                x: 0
                y: 0

                ChartView {
                    id: dateEnergyView
                    width: 400
                    height: 300
                    antialiasing: true
                    legend.visible: false
                    animationOptions: ChartView.NoAnimation
                    theme: ChartView.ChartThemeQt
                    property bool openGL: true
                    property bool openGLSupported: true

                    ValueAxis {
                        id: axisY
                        min: 0
                        max: 9
                        gridVisible: true
                        labelFormat: "%.0f"
                        tickCount: 10
                    }

                    ValueAxis {
                        id: axisX
                        min: 1900
                        max: 2100
                        gridVisible: true
                        labelFormat: "%.0f"
                        tickCount: 7
                    }

                    LineSeries {
                        id: lineSeries
                        axisX: axisX
                        axisY: axisY
                        useOpenGL: true
                        capStyle: "RoundCap"
                        style: "SolidLine"
                        color: "steelblue"
                    }
                }

                TableView {
                    id: culminationTableView
                    x: 0
                    y: 310
                    width: 600
                    height: 500
                    leftMargin: rowsHeader.implicitWidth
                    topMargin: columnsHeader.implicitHeight
                    clip: true

                    property var columnWidths: [90, 60, 110, 50, 160, 50]
                    columnWidthProvider: function (column) {
                        return columnWidths[column]
                    }

                    model: CulminationModel {}

                    delegate: Rectangle {
                        Text {
                            text: display
                            anchors.fill: parent
                            anchors.margins: 10
                            color: 'black'
                            font.pixelSize: 12
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                    Rectangle {
                        // mask the headers
                        z: 3
                        color: "#222222"
                        y: culminationTableView.contentY
                        x: culminationTableView.contentX
                        width: culminationTableView.leftMargin
                        height: culminationTableView.topMargin
                    }

                    Row {
                        id: columnsHeader
                        y: culminationTableView.contentY
                        z: 2
                        Repeater {
                            model: culminationTableView.columns
                                   > 0 ? culminationTableView.columns : 1
                            Label {
                                width: culminationTableView.columnWidthProvider(
                                           modelData)
                                height: 35
                                text: culminationTableView.model.headerData(
                                          modelData, Qt.Horizontal)
                                color: '#aaaaaa'
                                font.pixelSize: 13
                                padding: 10
                                verticalAlignment: Text.AlignVCenter

                                background: Rectangle {
                                    color: "#333333"
                                }
                            }
                        }
                    }
                    Column {
                        id: rowsHeader
                        x: culminationTableView.contentX
                        z: 2
                        Repeater {
                            model: culminationTableView.rows > 0 ? culminationTableView.rows : 1
                            Label {
                                width: 60
                                height: 50
                                text: culminationTableView.model.headerData(
                                          modelData, Qt.Vertical)
                                color: '#aaaaaa'
                                font.pixelSize: 13
                                padding: 10
                                verticalAlignment: Text.AlignVCenter

                                background: Rectangle {
                                    color: "#333333"
                                }
                            }
                        }
                    }
                }
            }
        }

        Item {
            id: dayNumberTab

            ColumnLayout {
                id: dayNumberColumnLayout
                visible: true
                anchors.fill: parent

                RowLayout {
                    id: dayNumberRowLayout
                    TextField {
                        id: dayNumberFirst
                        placeholderText: qsTr("Дата начала периода")
                        font.family: "MS Shell Dlg 2"
                        font.pointSize: 12
                        validator: RegExpValidator {
                            regExp: /^(0[1-9]|[12][0-9]|3[01])[- /.](0[1-9]|1[012])[- /.](19|20)\d\d$/
                        }
                        //text: Qt.formatDate(Date.getMonth() + 1, "dd.MM.yyyy")
                    }

                    TextField {
                        id: dayNumberLast
                        placeholderText: qsTr("Дата конца периода")
                        font.family: "MS Shell Dlg 2"
                        font.pointSize: 12
                        validator: RegExpValidator {
                            regExp: /^(0[1-9]|[12][0-9]|3[01])[- /.](0[1-9]|1[012])[- /.](19|20)\d\d$/
                        }
                        //text: Qt.formatDate(Date::getMonth() + 2, "dd.MM.yyyy")
                    }
                }

                TableView {
                    id: dayNumberTableView
                    x: 0
                    y: 0
                    width: 200
                    height: 600
                    clip: true

                    property var columnWidths: [100, 100]
                    columnWidthProvider: function (column) {
                        return columnWidths[column]
                    }

                    model: DayNumberModel {}

                    delegate: Rectangle {
                        Text {
                            text: display
                            anchors.fill: parent
                            anchors.margins: 10
                            color: 'black'
                            font.pixelSize: 12
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.6600000262260437}
}
##^##*/

